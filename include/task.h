#ifndef TASK_H
#define TASK_H

#include <cpu.h>
#include <paging.h>

/* task structure */

typedef struct task {

  /* id of the hart the task should run on */
  reg_t hart_id;

  /* cpu state of the task */
  cpu_t cpu_state;

  /* address translation and protection */
  ptb_t* page_table;

  /* next task in list */
  struct task* next;

} __attribute__((packed)) task_t;

extern void switch_task(cpu_t* cpu_state);

cpu_t* schedule_task(cpu_t* old_cpu_state);

void add_task(task_t* task);

#endif
