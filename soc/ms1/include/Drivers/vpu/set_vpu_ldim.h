#ifndef _LDIM_H_ 
#define _LDIM_H_

#include "vpu_include.h"



//=============================================================


typedef struct {
    char    ldc_en            ;
    int32_t base_addr         ;
    int32_t img_width         ;
    int32_t img_height        ;
    int8_t  blk_xnum          ;
    int8_t  blk_ynum          ;
    int8_t  cmp_mask_x        ;
    int8_t  cmp_mask_y        ;
    int8_t  seg_col           ;
    int8_t  seg_row           ;


    char    power_is_16bit    ;

} MNT_LDC_t;

extern void ldc_reg_init(MNT_LDC_t *ldc_cfg);
extern void ldc_reg_cfg (MNT_LDC_t *ldc_cfg);

#endif

