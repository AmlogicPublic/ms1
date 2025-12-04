#include <stdint.h>  // IWYU pragma: keep
#include <stdio.h>
#include "Drivers/vout/vout.h"
extern int vout_setting_load(void);
extern void vout_run() {
  // ========================================================================
  // VOUT Process,cfg initial
  // ========================================================================

  printf("init_vout start\n\r");
  vout_setting_load();
  // // Configure LVDS settings
  // aml_lvds_cfg_t lvds_cfg;
  // lvds_cfg.pack_cntl = 0x00009151; // Configure as needed
  
  // printf("vout_1920x32_free_run_cmpr_lvds start\n\r");
  // vout_1920x32_free_run_cmpr_lvds(lvds_cfg);
  // printf("vout_1920x32_free_run_cmpr_lvds end\n\r");
  
  // printf("data to vout \n\r");
};

