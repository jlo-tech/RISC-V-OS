#ifndef PAGING_H
#define PAGING_H

/* ************************** */
/* Sv39 Paging Implementation */
/* ************************** */

#include <mem.h>
#include <types.h>

#define PAGE_VALID    (1)
#define PAGE_READ     (1 << 1)
#define PAGE_WRITE    (1 << 2)
#define PAGE_EXECUTE  (1 << 3)
#define PAGE_USER     (1 << 4)
#define PAGE_GLOBAL   (1 << 5)
#define PAGE_ACCESSED (1 << 6)
#define PAGE_DIRTY    (1 << 7)

#define VPN_L2(x)  ((x >> 30) & 0x1FF)
#define VPN_L1(x)  ((x >> 21) & 0x1FF)
#define VPN_L0(x)  ((x >> 12) & 0x1FF)
#define VPN_OFF(x) (x & 0xFFF)

#define PPN_L2(x)  ((x >> 30) & 0x3FFFFFF)
#define PPN_L1(x)  ((x >> 21) & 0x1FF)
#define PPN_L0(x)  ((x >> 12) & 0x1FF)
#define PPN_OFF(x) (x & 0xFFF)

/* Page Table Entry Type*/
typedef reg_t pte_t;

/* Page Table Structure */
typedef struct pagetable {

  pte_t entries[512];

} __attribute__((packed)) ptb_t;

static inline reg_t read_satp() {
  reg_t r;
  asm volatile("csrr %0, satp" : "=r" (r));
  return r;
}

static inline void write_satp(reg_t r) {
  asm volatile("csrw satp, %0" :: "r" (r));
}

/* write satp with root of page table */
void satp_page(void* root);

/* map virtual to physical address */
void map_page(ptb_t* dir, void* virt, void* phys, u8 flags);

/* map memory region */
void map_range(ptb_t* dir, void* virt, void* phys, usize len, u8 flags);

#endif
