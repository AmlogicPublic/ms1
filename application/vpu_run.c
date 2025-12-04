#include <stdint.h>  // IWYU pragma: keep
#include <stdio.h>
#include "Drivers/vout/vout.h"
extern int vpu_setting_load(void);
extern void vpu_run() {
  // ========================================================================
  // MNT Process,cfg initial
  // ========================================================================
  printf("init_vpu start\n\r");
  //vout_setting_load();
  vpu_setting_load();

};
