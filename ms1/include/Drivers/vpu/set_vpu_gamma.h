#ifndef _MNT_GAMMA_H_
#define _MNT_GAMMA_H_

#include "vpu_include.h"

typedef struct MNT_GAMMA_TYPE {   
    char      gamma_en        ;
    int16_t   gamma_lut_r[257];
    int16_t   gamma_lut_g[257];
    int16_t   gamma_lut_b[257];
} MNT_GAMMA_t;

extern void init_lcd_gamma   (MNT_GAMMA_t *mnt_vpu_gamma);
extern void set_lcd_gamma_new(MNT_GAMMA_t *mnt_vpu_gamma);

#endif //_MNT_GAMMA_H_
