#include <mem.h>
#include <list.h>
#include <mutex.h>

struct mem_chunk 
{
    usize size;
    struct list_anchor anchor;
};

static volatile reg_t mem_lock = 0;

static struct list_anchor mem_used __attribute__((aligned(4096)));
static struct list_anchor mem_free __attribute__((aligned(4096)));

void* align4k(void *ptr)
{
    u64 p = (u64)ptr;
    if((p & 0xFFF) != 0)
    {
        p &= ~0xFFFUL;
        p += 0x1000;
    }
    return (void*)p;
}

void init_mem()
{
    // Nothing used yet
    mem_used.next = &mem_used;
    mem_used.prev = &mem_used;

    // Mark free space
    struct mem_chunk *mf = (struct mem_chunk*)&kernel_end;
    mf->size = MEM_SIZE - ((u64)(&kernel_end) - (u64)RAM);
    mf->anchor.next = &mem_free;
    mf->anchor.prev = &mem_free;
    mem_free.next = &mf->anchor;
    mem_free.prev = &mf->anchor;
}

void* kalloc(usize size)
{
    // Align allocations
    // NOTE: PAGE_SIZE is space reserved for struct mem_chunk and to keep things aligned 
    size = (usize)align4k((void*)size + PAGE_SIZE);

    lock(&mem_lock);

    bool found = false;
    struct mem_chunk *curr = LIST_ELEMENT(mem_free.next, struct mem_chunk, anchor);
    
    do 
    {
        if(curr->size >= size + sizeof(struct mem_chunk))
        {
            found = true;
            break;
        }

        curr = LIST_NEXT_ELEMENT(curr, struct mem_chunk, anchor);
    } 
    while(curr->anchor.next != &mem_free);

    if(!found)
    {
        // Out of memory
        return NULL;
    }
    
    void *retptr = NULL;
    struct mem_chunk *new = curr;

    if(new->size > size)
    {
        // Split chunk 

        // Remove from free list
        LIST_REMOVE(&new->anchor);
    
        // Insert back free space
        struct mem_chunk *old = curr + size;
        old->size = curr->size - size;
        LIST_APPEND(&mem_free, &old->anchor);

        // Mark as used
        new->size = size;
        LIST_APPEND(&mem_used, &new->anchor);
    }
    else
    {
        // Remove from free list
        LIST_REMOVE(&new->anchor);
        // Insert into used list
        LIST_APPEND(&mem_used, &new->anchor);
    }

    // Return new chunk
    retptr = new;

    unlock(&mem_lock);

    return retptr + PAGE_SIZE;
}

/*
    Frees block and tries to merge with others
*/
static void try_merge(struct mem_chunk *addr)
{
    bool found = false;
    struct mem_chunk *curr = LIST_ELEMENT(mem_free.next, struct mem_chunk, anchor);

    do
    {
        if((curr + curr->size) == addr)
        {
            // Extend previous chunk
            curr->size += addr->size;
        }

        if(addr + addr->size == curr)
        {
            // Extend current chunk
            addr->size += curr->size;
            // Remove old chunk 
            LIST_REMOVE(&curr->anchor);
            break;
        }

        curr = LIST_NEXT_ELEMENT(curr, struct mem_chunk, anchor);
    } 
    while(curr->anchor.next != &mem_used);

    // Insert free chunk when we couldn't merged or when we extended the "addr" chunk in the loop above
    LIST_APPEND(&mem_free, &addr->anchor);
}

void kfree(void *addr)
{
    bool found = false;
    struct mem_chunk *curr = LIST_ELEMENT(mem_used.next, struct mem_chunk, anchor);
    
    do 
    {
        if(curr == (addr - PAGE_SIZE))
        {
            found = true;
            break;
        }

        curr = LIST_NEXT_ELEMENT(curr, struct mem_chunk, anchor);
    } 
    while(curr->anchor.next != &mem_used);

    if(found)
    {
        // Remove from used list
        LIST_REMOVE(&curr->anchor);

        // Search free list and try to merge
        try_merge(curr);
    }
}

void* zkalloc(usize size)
{
    void *ret = kalloc(size);
    memset(ret, 0, size);
    return ret;
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

  unsigned char* d = (unsigned char*)dest;

  for(u64 i = 0; i < size; i++) {
    d[i] = val;
  }
}
