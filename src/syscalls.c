/*
 *
 * In this file the syscalls were implemented
 *
 */

#include <syscall.h>

#include <timer.h>

void do_time(cpu_t* cpu_state) {

  /* read mtime and return result in a0 */
  cpu_state->a0 = read_mtime();

  return;
}
