#include <paging.h>

static void* pte2addr(pte_t e) {
  return (void*)(((e >> 10) & 0xFFFFFFFFFFF) << 12);
}

void map_page(ptb_t* dir, void* virt, void* phys, u8 flags) {

  pte_t entry;

  /* walk 3 level page table */
  for(u64 i = 0; i < 2; i++) {

    /* get entry */
    entry = dir->entries[((usize)virt >> (30 - (i * 9))) & 0x1FF];

    /* check if entry is valid */
    if((entry & 1) == 0) {
      /* alloc new page and let entry point to */
      ptb_t* p = (ptb_t*)align4k(kalloc(PAGE_SIZE << 1));
      /* construct new entry */
      entry = (((usize)p >> 12) << 10) | (usize)0x1;
      /* save entry in page table */
      dir->entries[((usize)virt >> (30 - (i * 9))) & 0x1FF] = entry;
    }

    /* set new directory */
    dir = (ptb_t*)pte2addr(entry);
  }

  /* set final entry */
  dir->entries[((usize)virt >> 12) & 0x1FF] = (((usize)phys >> 12) << 10) | flags;
}

void map_range(ptb_t* dir, void* virt, void* phys, usize len, u8 flags) {

  u64 pages_to_map = (len / PAGE_SIZE);

  if(len % PAGE_SIZE != 0) {
    pages_to_map += 1;
  }

  for(u64 i = 0; i < pages_to_map; i++) {
    map_page(dir, (void*)(virt + i * PAGE_SIZE), (void*)(phys + i * PAGE_SIZE), flags);
  }
}

void satp_page(void* root) 
{
    asm volatile("sfence.vma");

    write_satp(((reg_t)8 << 60) | (((reg_t)root >> 12) & 0xFFFFFFFFFFF));

    asm volatile("sfence.vma");
}
