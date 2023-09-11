#include <timer.h>

reg_t read_mtime() {
  return *(reg_t*)(MTIME);
}

reg_t read_mtimecmp() {
  return *(reg_t*)(MTIMECMP + 8 * read_mhartid());
}

void write_mtimecmp(reg_t t) {
  *(reg_t*)(MTIMECMP + 8 * read_mhartid()) = t;
}

void init_timer() {
  write_mtimecmp(read_mtime() + (reg_t)TIMER_INTERVAL);
}

void delay(u64 microseconds) {
  u64 s = read_mtime();
  while((s + (microseconds * 10000)) > read_mtime()) {
    asm volatile("nop");
  }
  return;
}
