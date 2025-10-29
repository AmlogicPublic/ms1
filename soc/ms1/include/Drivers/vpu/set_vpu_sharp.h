#ifndef _MNT_SHARP_H_
#define _MNT_SHARP_H_

#include "vpu_include.h"

typedef struct MNT_SHARP_TYPE{
    char          sharp_en        ;
    uint8_t       contrast_en     ;  //{lc_en,dnlp_en,cc_en}
    char          lc_wr_lut_en    ;
    char          lc_wr_lut_hw    ;
    uint8_t       lc_mapping_hnum ;
    uint8_t       lc_mapping_vnum ;
    uint16_t      lc_mapping_hsize;
    uint16_t      lc_mapping_vsize;
    int           curve_info[12*8*2];
} MNT_SHARP_t;

extern void set_vpu_sharp(MNT_SHARP_t *mnt_vpu_sharp);
extern void set_lc_mapping_lut(uint8_t blk_hnum, uint8_t blk_vnum, uint16_t blk_hidx[13], uint16_t blk_vidx[9]);

int update_sr_lcrams(uint8_t blk_hnum, uint8_t blk_vnum, int szCurveInfo[12*8*2], int bInit, int bCheck);

#endif  //_MNT_SHARP_H_

