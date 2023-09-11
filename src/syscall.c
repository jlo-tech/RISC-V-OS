/*
 *
 * In this file the syscall handler of the os is implemented
 *
 */

#include <syscall.h>

void handle_syscall(calling_mode_t calling_mode, cpu_t* cpu_state) {

  /* NOTE: syscall number and syscall result are stored in the a0 register of the calling task */
  u64 syscall_no = (u64)cpu_state->a0;

  switch(syscall_no) {

    /* first syscall - read mtime */
    case 1:
      do_time(cpu_state);
      break;
  }
}
