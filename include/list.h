#ifndef LIST_H
#define LIST_H

#include <types.h>

struct list_anchor
{
    struct list_anchor *prev;
    struct list_anchor *next;
} __attribute__((packed));

#define LIST_ELEMENT(PTR, TYPE, MEMBER) (TYPE*)(((u8*)PTR) - __builtin_offsetof(TYPE, MEMBER))

#define LIST_NEXT_ENTRY(PTR, MEMBER) (PTR->MEMBER.next)

#define LIST_NEXT_ELEMENT(PTR, TYPE, MEMBER) LIST_ELEMENT(LIST_NEXT_ENTRY(PTR, MEMBER), TYPE, MEMBER)

static void LIST_INIT(struct list_anchor *l)
{ 
    l->prev = l;
    l->next = l;
}

static void LIST_INSERT(struct list_anchor *p, struct list_anchor *e, struct list_anchor *n)
{
    p->next = e;
    e->prev = p;
    e->next = n;
    n->prev = e;
}

static void LIST_APPEND(struct list_anchor *l, struct list_anchor *e)
{
    LIST_INSERT(l->prev, e, l);
}

static void LIST_REMOVE(struct list_anchor *l)
{
    l->prev->next = l->next;
    l->next->prev = l->prev;
}

#endif