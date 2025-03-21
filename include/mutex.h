#ifndef MUTEX_H
#define MUTEX_H

#include <types.h>

void lock(volatile reg_t* lock_value);
void unlock(volatile reg_t* lock_value);

#endif
