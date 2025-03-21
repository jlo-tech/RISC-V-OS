#include <pmp.h>

void init_pmp() {

  write_pmpaddr(0, 0);
  write_pmpaddr(1, RAM + MEM_SIZE);

  write_pmpcfg(0, (CFG_READ | CFG_WRITE | CFG_EXECUTE | CFG_A_TOR));
}
