#ifndef SYSCALL_H
#define SYSCALL_H

#include <cpu.h>

typedef enum calling_mode {
  USER_CALLING_MODE,
  SUPERVISOR_CALLING_MODE,
  MACHINE_CALLING_MODE
} calling_mode_t;

/* syscall handler from os */
void handle_syscall(calling_mode_t calling_mode, cpu_t* cpu_state);


/* actual syscalls are listed here ... */
void do_time(cpu_t* cpu_state);

#endif
