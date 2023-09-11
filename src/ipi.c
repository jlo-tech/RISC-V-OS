#include <ipi.h>
#include <cpu.h>

void trigger_ipi(u64 hart_id) {

  /* get address of msip field in clint */
  u32* ipi_port = (u32*)(0x2000000 + 4 * hart_id);

  /* trigger ipi on hart (hart_id) */
  *ipi_port = 1;
}

void resolve_ipi() {

  /* get address of msip field in clint */
  u32* ipi_port = (u32*)(0x2000000 + 4 * read_mhartid());

  /* resolve ipi on hart */
  *ipi_port = 0;
}
