#ifndef _MNT_VPU_H_
#define _MNT_VPU_H_

#include "vpu_include.h"
//#include "cfg_mnt_pps.h"
#include "set_vpu_rdma.h"
#include "set_pat_gen.h"
#include "set_vpu_hdr.h"
#include "set_vpu_stat.h"
#include "set_vpu_sharp.h"
#include "set_vpu_cm.h"
#include "set_vpu_osd.h"
#include "set_vpu_vadj.h"
#include "set_vpu_blend.h"
#include "set_vpu_lut3d.h"
#include "set_vpu_wb.h"
#include "set_vpu_gamma.h"
#include "set_vpu_ldim.h"

#define STIMULUS_VPU_TOP_LOAD_SRC_EVENT 0

typedef struct MNT_VPU_TOP_TYPE {   
    //input  size
    uint32_t  src_input_hsize  ;
    uint32_t  src_input_vsize  ;

    //output size
    uint32_t  disp_output_hsize;
    uint32_t  disp_output_vsize;

    //module enable
    uint32_t  patgen_en        ;
    uint32_t  scaler_en        ;
    uint32_t  hdr_en           ;
    uint32_t  stat_en          ;
    uint32_t  sharp_en         ;
    uint32_t  contrast_en      ;
    uint32_t  cm_en            ;
    uint32_t  osd_en           ;
    uint32_t  vadj_conv_en     ;
    uint32_t  lut3d_en         ;
    uint32_t  gamut_en         ;
    uint32_t  wb_en            ;
    uint32_t  gamma_en         ;
    uint32_t  ldc_en           ;

    uint32_t  vadj_pos         ;
    uint32_t  gamma_pos        ;
} MNT_VPU_TOP_t;

typedef enum {
    ALLON   = 0,
    ALLBYPS = 1,
    DEFAULT = 2,

} MNT_ENABLE_e;

typedef struct MNT_VPU_TYPE { 
    MNT_RDMA_t      mnt_rdma     ;
    MNT_PAT_GEN_t   mnt_pat_gen  ;
    MNT_VPU_TOP_t   mnt_vpu_top  ;
    //MNT_PPS_t       mnt_pps      ;
    MNT_HDR_t       mnt_vpu_hdr  ;
    MNT_STAT_t      mnt_vpu_stat ;
    MNT_SHARP_t     mnt_vpu_sharp;   
    MNT_CM_t        mnt_vpu_cm   ;
    MNT_OSD_t       mnt_vpu_osd  ;
    MNT_VADJ_t      mnt_vpu_vadj ;
    MNT_BLEND_t     mnt_vpu_bld  ;    
    MNT_LUT3D_t     mnt_vpu_lut3d;
    MNT_WB_t        mnt_vpu_wb   ;
    MNT_GAMMA_t     mnt_vpu_gamma;        
    MNT_LDC_t       mnt_vpu_ldc  ;

} MNT_VPU_t;   

extern void init_mnt_vpu(MNT_VPU_t *mnt_vpu, MNT_ENABLE_e mode);
extern void  set_mnt_vpu(MNT_VPU_t *mnt_vpu);

void close_mnt_vpu_reg_update();
void open_mnt_vpu_reg_update();
void set_vpu_top_load_data_event();

#endif //_MNT_VPU_H_
