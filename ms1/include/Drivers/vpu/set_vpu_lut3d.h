#ifndef _MNT_LUT3D_H_
#define _MNT_LUT3D_H_

#include "vpu_include.h"

typedef struct MNT_LUT3D_TYPE{
    char     lut3d_en   ;
    uint8_t  gamut_en   ;//{matrix_en,eotf_en,oetf_en}

    char     lut3d_load ;
    char     lut3d_check;
    uint8_t  lut3d_mode ;
} MNT_LUT3D_t;

extern int  vpp_set_lut3d(char enable, char bLut3DLoad, int *pLut3D, char bLut3DCheck, uint8_t lut3d_mode);
extern void vpp_lut3d_table_init(int *pLut3D, int bitdepth);
extern void ycbcr2rgbpc_nb(int *R, int *G, int *B, int Y, int Cb, int Cr, int bitdepth);

extern void set_gamut_map1(uint32_t gamut_en);

extern void set_lut3d_gamut(MNT_LUT3D_t *mnt_vpu_lut3d);

#endif //_MNT_LUT3D_H_
