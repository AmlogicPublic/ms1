#ifndef _MNT_VADJ_H_
#define _MNT_VADJ_H_

#include "vpu_include.h"

typedef struct MNT_VADJ_TYPE{
    uint8_t  vadj_en            ;  //{vadj_en,csc_en,rgbbst_en}
    char     vadj_minus_black_en;
    uint16_t vadj_bri           ;
    uint16_t vadj_con           ;
    uint16_t vadj_ma            ;
    uint16_t vadj_mb            ;
    uint16_t vadj_mc            ;
    uint16_t vadj_md            ;
    uint16_t soft_curve_0_a     ;
    uint16_t soft_curve_0_b     ;
    uint16_t soft_curve_0_ci    ;
    uint16_t soft_curve_0_cs    ;
    uint16_t soft_curve_0_g     ;
    uint16_t soft_curve_1_a     ;
    uint16_t soft_curve_1_b     ;
    uint16_t soft_curve_1_ci    ;
    uint16_t soft_curve_1_cs    ;
    uint16_t soft_curve_1_g     ;
} MNT_VADJ_t;

extern void set_vadj_matrix(int mode);
extern void set_rgb_contrast_bright(MNT_VADJ_t *mnt_vpu_vadj);
extern void set_video_adjust_1_regs(MNT_VADJ_t *mnt_vpu_vadj);

#endif //_MNT_VADJ_H_
