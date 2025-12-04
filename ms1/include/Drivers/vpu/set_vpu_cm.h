#ifndef _MNT_CM_H_
#define _MNT_CM_H_

#include "vpu_include.h"

typedef struct MNT_CM_TYPE{
    uint16_t hsize      ;
    uint16_t vsize      ;
    char     cm2_en     ;
    char     hue_adj_en ;
    char     sat_adj_en ;
    char     luma_adj_en;
    char     xvycc_mode ;
} MNT_CM_t;

extern int set_chroma_regs (MNT_CM_t *cm);

#endif //_MNT_CM_H_


