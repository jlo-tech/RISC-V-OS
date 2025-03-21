#include <cpu.h>
#include <mem.h>
#include <list.h>
#include <task.h>
#include <mutex.h>
#include <paging.h>
#include <limits.h>

extern void lock(volatile reg_t *l);
extern void unlock(volatile reg_t *l);

// Idle task
extern void idle();

// Tasks to schedule
static task_t tasks;    // Circuilar list of tasks
static task_t* current_tasks[NUMBER_OF_HARTS];  // Current task of each hart from tasks list
static volatile reg_t tasks_lock;

void switch_task(task_t *task)
{
    // Set current task
    reg_t hart_id = read_mhartid();
    current_tasks[hart_id] = task;
    // Load page table
    satp_page((void*)(((reg_t)task->page_table)));
    // Switch to task 
    _switch_task(&task->cpu_state);
}

void init_tasks()
{
    // Init lock
    tasks_lock = 0;

    // Init current task pointers and locks
    for(int i = 0; i < NUMBER_OF_HARTS; i++)
    {
        current_tasks[i] = NULL;
    }

    // Init idle task
    task_t *idle_task = &tasks;
    idle_task->hart_id = HART_ANY;
    idle_task->cpu_state.pp = MACHINE_MODE;
    idle_task->cpu_state.pc = (reg_t)&idle;
    idle_task->cpu_state.sp = (reg_t)((usize)zkalloc(PAGE_SIZE) + PAGE_SIZE - 1);
    idle_task->page_table = (ptb_t*)zkalloc(PAGE_SIZE);

    map_range(idle_task->page_table, (void*)&idle, (void*)&idle, PAGE_SIZE, PAGE_VALID | PAGE_READ | PAGE_WRITE | PAGE_EXECUTE | PAGE_USER);
    LIST_INIT(&idle_task->anchor);
}

void next_task_for_hart(reg_t hart_id)
{
    task_t *tp = current_tasks[hart_id];
    while(LIST_ELEMENT(tp->anchor.next, task_t, anchor) != current_tasks[hart_id])
    {
        // Next element 
        tp = LIST_ELEMENT(tp->anchor.next, task_t, anchor);
    
        if(tp->hart_id == hart_id)
        {
            // Set new task 
            current_tasks[hart_id] = tp;
            return;
        }
    }

    // If no task was found run idle task
    current_tasks[hart_id] = &tasks;
}

cpu_t* schedule_task(cpu_t *old_cpu_state)
{
    // Get hart id 
    reg_t hart_id = read_mhartid();

    // Lock tasks list
    lock(&tasks_lock);

    // Save old state
    current_tasks[hart_id]->cpu_state = *old_cpu_state;

    // Scheduling
    next_task_for_hart(hart_id);

    unlock(&tasks_lock);

    // Activate page table
    satp_page(current_tasks[hart_id]->page_table);

    return &current_tasks[hart_id]->cpu_state;
}

void deploy_task(task_t *task) 
{
    // Add task to schedule
    lock(&tasks_lock);
    LIST_APPEND(&tasks.anchor, &task->anchor);
    unlock(&tasks_lock);
}

void dispose_task(task_t *task)
{
    // Remove task from schedule
    lock(&tasks_lock);
    LIST_REMOVE(&task->anchor);
    unlock(&tasks_lock);
    // Free memory
    kfree(task);
}
