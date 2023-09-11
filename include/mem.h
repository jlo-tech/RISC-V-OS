#ifndef MEM_H
#define MEM_H

#include <types.h>

#define RAM 0x80000000

#define MEM_SIZE  268435456 /* 256M */
#define PAGE_SIZE 4096      /* 4K   */

#define PAGE_FREE 0
#define PAGE_USED 1

extern void kernel_start;
extern void kernel_end;

void init_mem();

void* alloc_mem();
void* zalloc_mem();
void free_mem(void* page);

/* standard memory operations */
void memcpy(void* dest, void* src, usize size);
void memset(void* dest, u8 val, usize size);

#endif
