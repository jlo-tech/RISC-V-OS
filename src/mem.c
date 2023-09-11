#include <mem.h>

static u8 mem_map[MEM_SIZE / PAGE_SIZE];

void init_mem() {

  /* mark every page in memory as free */
  for(u64 i = 0; i < MEM_SIZE / PAGE_SIZE; i++) {
    mem_map[i] = PAGE_FREE;
  }

  /* mark kernel pages as used */
  u64 kernel_size = (u64*)&kernel_end - (u64*)&kernel_start;
  u64 iterations = (kernel_size % PAGE_SIZE == 0) ? (kernel_size / PAGE_SIZE) : ((kernel_size / PAGE_SIZE) + 1);

  for(u64 i = 0; i < iterations; i++) {
    mem_map[i] = PAGE_USED;
  }
}

void* alloc_mem() {

  /* iterate over mem_map until we find a free page */
  u64 i = 0;
  while(i < (MEM_SIZE / PAGE_SIZE)) {
    if(mem_map[i] == PAGE_FREE) {
      /* dont forget to mark page as used */
      mem_map[i] = PAGE_USED;
      /* calculate return address */
      return (void*)(RAM + PAGE_SIZE * i);
    }
    i++;
  }

  /* return null pointer, NOTE: always do null checks! */
  return (void*) 0;
}

void* zalloc_mem() {

  void* n = alloc_mem();

  memset(n, 0, PAGE_SIZE);

  return n;
}

void free_mem(void* page) {

  /* calculate position in mem_map */
  u64 pos = ((u64)page - RAM) / PAGE_SIZE;

  /* check that we do not write out of bounds */
  if(pos < MEM_SIZE / PAGE_SIZE) {
    mem_map[pos] = PAGE_FREE;
  }
}

/* standard memory operations */

void memcpy(void* dest, void* src, usize size) {

  char* d = (char*)dest;
  char* s = (char*)src;

  for(u64 i = 0; i < size; i++) {
    d[i] = s[i];
  }
}

void memset(void* dest, u8 val, usize size) {

  char* d = (char*)dest;

  for(u64 i = 0; i < size; i++) {
    d[i] = val;
  }
}
