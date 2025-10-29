#ifndef _MNT_PATGEN_H_
#define _MNT_PATGEN_H_

#include "vpu_include.h"

typedef enum {
    PATGEN_TMN_256X144   = 0,
    PATGEN_TMN_2560X1440 = 1,
    PATGEN_TMN_1920X32   = 2,   
    PATGEN_TMN_1920X1080 = 3,        
    PATGEN_ENC_TYPE_MAX
} PATGEN_TMN_t;   /* tv encoder output format */

typedef struct MNT_PAT_GEN_TYPE{
    PATGEN_TMN_t patgen_tmn ;
    uint8_t     patgen_mode;        
} MNT_PAT_GEN_t;

extern void set_pat_gen(PATGEN_TMN_t output_type, uint8_t mode);

#endif //_MNT_PATGEN_H_
