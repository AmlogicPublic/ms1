#ifndef _MNT_WB_H_
#define _MNT_WB_H_

#include "vpu_include.h"

typedef struct MNT_WB_TYPE{
    char          enable     ;
    int16_t       gain0      ;
    int16_t       gain1      ;
    int16_t       gain2      ;
    int16_t       pre_offset0;
    int16_t       pre_offset1;
    int16_t       pre_offset2;
    int16_t       offset0    ;
    int16_t       offset1    ; 
    int16_t       offset2    ; 
} MNT_WB_t;

extern void set_gain_offset_regs(MNT_WB_t *mnt_vpu_wb);

#endif //_MNT_WB_H_
