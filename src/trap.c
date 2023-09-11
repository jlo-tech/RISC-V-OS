#include <cpu.h>
#include <task.h>
#include <trap.h>
#include <timer.h>
#include <console.h>
#include <syscall.h>

/* enable interrupts */
void init_traps() {

  /* register trap handler */
  write_mtvec((reg_t)&handle_traps);

  /* enable software, timer and external interrupts */
  write_mie(read_mie() | 1 << 3);
  write_mie(read_mie() | 1 << 7);
  //write_mie(read_mie() | 1 << 11);

  /* globally enable machine mode interrupts */
  write_mstatus(read_mstatus() | 1 << 3);
}

/* set mpie to activate interrupts after mret */
void init_previous_traps() {

  /* register trap handler */
  write_mtvec((reg_t)&handle_traps);

  /* enable software, timer and external interrupts */
  write_mie(read_mie() | 1 << 3);
  write_mie(read_mie() | 1 << 7);
  //write_mie(read_mie() | 1 << 11);

  /* globally enable previous machine mode interrupts */
  write_mstatus(read_mstatus() | 1 << 7);
}

/* this is the overall trap handler */
cpu_t* handle_trq(cpu_t* cpu_state, reg_t mcause, reg_t mtval) {

  u8 code = mcause & 0xff;

  //putf_console("MSTATUS: %b \n", read_mstatus());
  //putf_console("Code: %b \n", mcause);

  /* mcause seems to be 32 Bit long */
  if(mcause & (1 << 31)) {

    putf_console("I: %d \n", code);

    /* interrupt */
    switch(code) {

      /* handle ipi */
      case 3:
        /* clear ipi */
        putf_console("IPI Trap \n");
        resolve_ipi();

        /* TODO: schedule tasks on different harts */

        return cpu_state;

      /* handle timer irq */
      case 7:
        /* continue timer */
        putf_console("Timer Trap \n");
        init_timer();

        return schedule_task(cpu_state);
    }

  } else {

    //putf_console("E: %d HART: %u MEPC: %u MTVAL: %u \n", code, read_mhartid(), read_mepc(), mtval);

    /* exception */
    switch(code) {

      case 8:
        /* handle syscall from user mode */
        handle_syscall(USER_CALLING_MODE, cpu_state);
        break;

      case 9:
        /* handle syscall from supervisor mode */
        handle_syscall(SUPERVISOR_CALLING_MODE, cpu_state);
        break;

      case 11:
        /* handle syscall from machine mode */
        handle_syscall(MACHINE_CALLING_MODE, cpu_state);
        break;
    }

    /* increase pc so that we over jump the exception triggering instruction */
    cpu_state->pc = cpu_state->pc + 4;
  }

  return cpu_state;
}
