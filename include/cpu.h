#ifndef CPU_H
#define CPU_H

#include <types.h>

#define NUMBER_OF_HARTS 4

#define MACHINE_MODE    3
#define SUPERVISOR_MODE 1
#define USER_MODE       0

/* cpu context */

typedef struct cpu {

  reg_t pp;  /* privilege mode */
  reg_t pc;  /* pc  */

  reg_t ra;  /* x1  */
  reg_t sp;  /* x2  */
  reg_t gp;  /* x3  */
  reg_t tp;  /* x4  */
  reg_t t0;  /* x5  */
  reg_t t1;  /* x6  */
  reg_t t2;  /* x7  */
  reg_t s0;  /* x8  */
  reg_t s1;  /* x9  */
  reg_t a0;  /* x10 */
  reg_t a1;  /* x11 */
  reg_t a2;  /* x12 */
  reg_t a3;  /* x13 */
  reg_t a4;  /* x14 */
  reg_t a5;  /* x15 */
  reg_t a6;  /* x16 */
  reg_t a7;  /* x17 */
  reg_t s2;  /* x18 */
  reg_t s3;  /* x19 */
  reg_t s4;  /* x20 */
  reg_t s5;  /* x21 */
  reg_t s6;  /* x22 */
  reg_t s7;  /* x23 */
  reg_t s8;  /* x24 */
  reg_t s9;  /* x25 */
  reg_t s10; /* x26 */
  reg_t s11; /* x27 */
  reg_t t3;  /* x28 */
  reg_t t4;  /* x29 */
  reg_t t5;  /* x30 */
  reg_t t6;  /* x31 */

} __attribute__ ((packed)) cpu_t;


/* msr access operations */

static inline reg_t read_mhartid() {
  reg_t r;
  asm volatile("csrr %0, mhartid" : "=r" (r));
  return r;
}

static inline reg_t read_mstatus() {
  reg_t r;
  asm volatile("csrr %0, mstatus" : "=r" (r));
  return r;
}

static inline void write_mstatus(reg_t r) {
  asm volatile("csrw mstatus, %0" :: "r" (r));
}

static inline reg_t read_mtvec() {
  reg_t r;
  asm volatile("csrr %0, mtvec" : "=r" (r));
  return r;
}

static inline void write_mtvec(reg_t r) {
  asm volatile("csrw mtvec, %0" :: "r" (r));
}

static inline reg_t read_mie() {
  reg_t r;
  asm volatile("csrr %0, mie" : "=r" (r));
  return r;
}

static inline void write_mie(reg_t r) {
  asm volatile("csrw mie, %0" :: "r" (r));
}

static inline reg_t read_mepc() {
  reg_t r;
  asm volatile("csrr %0, mepc" : "=r" (r));
  return r;
}

static inline void write_mepc(reg_t r) {
  asm volatile("csrw mepc, %0" :: "r" (r));
}

#endif
