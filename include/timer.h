#ifndef TIMER_H
#define TIMER_H

#include <cpu.h>
#include <types.h>

#define CLINT 0x2000000
#define MTIMECMP (CLINT + 0x4000)
#define MTIME (CLINT + 0xBFF8)

#define TIMER_INTERVAL 5000000

/* returns time since boot */
reg_t read_mtime();

/* read and write memory-mapped timecmp register */
reg_t read_mtimecmp();
void write_mtimecmp(reg_t t);

/* init and step timer */
void init_timer();

/* wait */
void delay(u64 microseconds);

#endif
