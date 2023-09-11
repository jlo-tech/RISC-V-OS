#include <pmp.h>

void init_pmp() {

  write_pmpaddr(0, RAM);
  write_pmpaddr(1, RAM + 268435456);

  write_pmpcfg(0, (CFG_READ | CFG_WRITE | CFG_EXECUTE | CFG_A_TOR));
}
