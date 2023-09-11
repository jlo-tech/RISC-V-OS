#ifndef MUTEX_H
#define MUTEX_H

#include <types.h>

void lock_mutex(reg_t* lock_value);
void unlock_mutex(reg_t* lock_value);

#endif
