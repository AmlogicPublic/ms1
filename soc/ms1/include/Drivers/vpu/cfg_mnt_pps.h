#ifndef _MNT_PPS_H_
#define _MNT_PPS_H_

#include "vpu_include.h"
#ifdef VPU_FPGA_EN
    #define VOUT_APB_BASE_ADDR 0x10000
#else
    // #include "vout/vout.h"
#endif

typedef struct MNT_PPS_TYPE{
    uint32_t      src_hsize    ;//input  frame hszie
    uint32_t      src_vsize    ;//input  frame vszie
    uint32_t      dst_hsize    ;//output frame hszie
    uint32_t      dst_vsize    ;//output frame vszie
    char          posthsc_en   ;//1 bits posthscaler en 
    char          postvsc_en   ;//1 bits posthscaler en 

    char          pps_dbg_en   ;//debug en
} MNT_PPS_t;

extern uint32_t my_clz(uint32_t x);
extern uint32_t div_u64(uint64_t dividend, uint32_t divisor);

extern void cfg_mnt_pps (uint32_t index, MNT_PPS_t *pps);
extern void cfg_mnt_pps_coef(uint32_t index, uint32_t pps_scale_coef_idx , uint32_t pps_scale_coef);

extern void mnt_pps_reg_debug();

#endif

