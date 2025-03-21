#include <cpu.h>
#include <ipi.h>
#include <mem.h>
#include <pmp.h>
#include <task.h>
#include <trap.h>
#include <mutex.h>
#include <timer.h>
#include <paging.h>
#include <console.h>

/* helper function */
u64 exec_time_syscall() {

  reg_t r;

  /* make syscall and save result */
  __asm__(
    "li a0, 1 \n"
    "ecall \n"
    "addi %0, a0, 0\n"
    : "=r" (r)
  );

  return (u64)r;
}

void task0_func() {
  while(1) {
    putf_console("Hello, from Task 0! \n");
    putf_console("Time: %u\n", exec_time_syscall());
    /* delay */
    for(int i = 0; i < 2048 * 512; i++);
  }
}

void task1_func() {
  while(1) {
    putf_console("Hello, from Task 1! \n");

    /* delay */
    for(int i = 0; i < 8192 * 512; i++);
  }
}

void task2_func() {
  while(1) {
    putf_console("Hello, from Task 2! \n");

    /* delay */
    for(int i = 0; i < 8192 * 512; i++);
  }
}

extern void kernel_mapping_start;
extern void kernel_mapping_end;

extern void stack_start;
extern void stack_end;

cpu_t state0;
cpu_t state1;
cpu_t state2;

task_t t0;
task_t t1;
task_t t2;

volatile reg_t sync_lock = 1;
volatile reg_t print_lock = 0;

void kmain() {

  if(read_mhartid() == 0) {
    putf_console("[%u] The kernel is running...\n", read_mtime());
  }

  /* wait one 10th of a second */
  delay(100);

  lock(&print_lock);
  putf_console("[%u] Hart %u initialized...\n", read_mtime(), read_mhartid());
  unlock(&print_lock);

  init_pmp();

  if(read_mhartid() == 0) {

    init_mem();

    init_previous_traps();
    init_timer();

    state0.pp = USER_MODE;
    state0.pc = (reg_t)&task0_func;
    state0.sp = (reg_t)((usize)kalloc(PAGE_SIZE) + PAGE_SIZE - 1);

    state1.pp = MACHINE_MODE;
    state1.pc = (reg_t)&task1_func;
    state1.sp = (reg_t)((usize)kalloc(PAGE_SIZE) + PAGE_SIZE - 1);

    state2.pp = SUPERVISOR_MODE;
    state2.pc = (reg_t)&task2_func;
    state2.sp = (reg_t)((usize)kalloc(PAGE_SIZE) + PAGE_SIZE - 1);

    t0.hart_id = 0;
    t0.cpu_state = state0;
    t0.page_table = NULL;
    LIST_INIT(&t0.anchor);

    t1.hart_id = 1;
    t1.cpu_state = state1;
    t1.page_table = NULL;
    LIST_INIT(&t1.anchor);

    t2.hart_id = 0;
    t2.cpu_state = state2;
    t2.page_table = NULL;
    LIST_INIT(&t2.anchor);

    t0.page_table = (ptb_t*)align4k(kalloc(PAGE_SIZE << 1));

    t2.page_table = (ptb_t*)kalloc(PAGE_SIZE << 1);

    u8 cflags = PAGE_VALID | PAGE_READ | PAGE_WRITE | PAGE_EXECUTE;

    /* map t0 */
    /* map text segment */
    map_range(t0.page_table,
              &kernel_mapping_start, 
              &kernel_mapping_start, 
              (usize)&kernel_mapping_end - (usize)&kernel_mapping_start, 
              cflags | PAGE_USER);
    /* map uart */
    map_range(t0.page_table, (void*)0x10000000, (void*)0x10000000, 1, cflags | PAGE_USER);
    /* map stack of t0 */
    map_page(t0.page_table, (void*)t0.cpu_state.sp, (void*)t0.cpu_state.sp, cflags | PAGE_USER);

    /* t1 runs in machine mode, we dont have to map anything for t1 */

    /* map t2 */
    /* map text segment */
    map_range(t2.page_table, &kernel_mapping_start, &kernel_mapping_start, (usize)&kernel_mapping_end - (usize)&kernel_mapping_start, cflags);
    /* map uart */
    map_range(t2.page_table, (void*)0x10000000, (void*)0x10000000, 1, cflags);
    /* map stack of t2 */
    map_page(t2.page_table, (void*)t2.cpu_state.sp, (void*)t2.cpu_state.sp, cflags);

    init_tasks();

    deploy_task(&t0);
    deploy_task(&t1);
    deploy_task(&t2);

    /* sync hart 0 and 1 */
    unlock(&sync_lock);

    /* run t0 */
    switch_task(&t0);
  }

  if(read_mhartid() == 1) {

    /* sync hart 0 and 1 */
    lock(&sync_lock);

    init_previous_traps();
    init_timer();

    /* run t1 */
    switch_task(&t1);
  }

  while(1) {
    asm volatile("wfi");
  }
}
