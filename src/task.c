#include <cpu.h>
#include <mem.h>
#include <task.h>
#include <mutex.h>
#include <paging.h>

static task_t* root_task = NULL;
static task_t* current_tasks[NUMBER_OF_HARTS] = { NULL };

cpu_t* schedule_task(cpu_t* old_cpu_state) {

  reg_t hart = read_mhartid();

  /* if no tasks just continue with whatever */
  if(root_task == NULL || current_tasks[hart] == NULL) {
    return old_cpu_state;
  }

  /* save old cpu state */
  current_tasks[hart]->cpu_state = *old_cpu_state;

  /* search for next task for the current hart */
  task_t* st = current_tasks[hart];
  while(1) {
    if(current_tasks[hart]->next != NULL) {
      current_tasks[hart] = current_tasks[hart]->next;
    } else {
      current_tasks[hart] = root_task;
    }

    if(current_tasks[hart]->hart_id == hart || current_tasks[hart] == st) {
      break;
    }
  }

  /* switch paging context*/
  if(current_tasks[hart]->page_table != NULL) {
    /* activate new context */
    satp_page(current_tasks[hart]->page_table);
  }

  return &current_tasks[hart]->cpu_state;
}

void add_task(task_t* task) {

  /* make sure next doesnt point to random location */
  task->next = NULL;

  /* if we got no tasks yet set task to root_task */
  if(root_task == NULL) {

    /* init root task */
    root_task = alloc_mem();
    *root_task = *task;
    current_tasks[task->hart_id] = root_task;

  } else {

    /* init current task variable */
    task_t* ct = root_task;

    /* find end of linked task list */
    while(ct->next != NULL) {
      ct = ct->next;
    }

    /* alloc mem and save new task in list */
    ct->next = (task_t*)alloc_mem();
    ct->next = alloc_mem();
    *ct->next = *task;
  }

  return;
}
