#ifndef MEM_H
#define MEM_H

#include <types.h>

#define RAM 0x80000000

#define MEM_SIZE  268435456 /* 256M */
#define PAGE_SIZE 4096      /* 4K   */

extern void kernel_start;
extern void kernel_end;

void init_mem();
void* kalloc(usize size);
void* zkalloc(usize size);
void kfree(void *addr);

void* align4k(void *ptr);

/* standard memory operations */
void memcpy(void* dest, void* src, usize size);
void memset(void* dest, u8 val, usize size);

#endif
