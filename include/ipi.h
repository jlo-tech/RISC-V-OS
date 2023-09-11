#ifndef IPI_H
#define IPI_H

#include <types.h>

void trigger_ipi(u64 hart_id);
void resolve_ipi();

#endif
