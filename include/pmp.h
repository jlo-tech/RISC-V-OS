#ifndef PMP_H
#define PMP_H

#include <cpu.h>
#include <mem.h>

/* flags for protected region */

#define CFG_READ      1
#define CFG_WRITE     2
#define CFG_EXECUTE   4

/* address-matching configuration */

#define CFG_A_OFF   0
#define CFG_A_TOR   8
#define CFG_A_NA4   16
#define CFG_A_NAPOT 24

#define read_pmpcfg(a)  \
({                     \
  reg_t ___r;          \
  asm volatile("csrr %0, pmpcfg" #a : "=r" (___r)); \
 ___r;                 \
})

#define write_pmpcfg(a, v) \
({                     \
  reg_t ___r = (reg_t)v;          \
  asm volatile("csrw pmpcfg" #a ", %0" :: "r" (___r)); \
})


#define read_pmpaddr(a) \
({                     \
  reg_t ___r;          \
  asm volatile("csrr %0, pmpaddr" #a : "=r" (___r)); \
 ___r;                 \
})

#define write_pmpaddr(a, v) \
({                     \
  reg_t ___r = (reg_t)v;          \
  asm volatile("csrw pmpaddr" #a ", %0" :: "r" (___r)); \
})

void init_pmp();

#endif
