#ifndef _SET_HDR2_V0__H_
#define _SET_HDR2_V0__H_

#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
//#include "f2v.h"
#include "vpu_include.h"

#define HDR2     0
#define HDR2_MAT 0

#define FLTZERO 0xfc000



typedef struct MNT_HDR_TYPE {   
         char hdr2_sel      ; // 
         char hdr2_top_en   ; // hdr enable 1:open hdr, 0 :close hdr
         char hdr2_only_mat ; // 1: hdr only used as matrix
         char hdr2_fmt_cfg  ; // 0:sdr to hdr    1:hdr to sdr 
         char in_fmt        ; // 1:yuv in    0:rgb in
         char rgb_out_en    ; // 1:rgb out   0:yuv out
} MNT_HDR_t;

extern void set_hdr2_v0(MNT_HDR_t *mnt_vpu_hdr);

extern void set_hdr2_mtrix(
         char only_matrix   , // hdr2 used as matrix
         char mat_sel       , //
         char rgb2yuv_in    , // in  matrix: 1:rgb to yuv 0 :yuv to rgb
         char hdr2_fmt      , // in  matrix: yuv to rgb
         char out_matrix_fmt  // out matrix: rgb to yuv
);

void  read_hdr_hist(int64_t hdr_hist[128], uint32_t hist_rd_reg);

typedef int64_t AMLFLT;

typedef struct aml_vm_reg {

	int16_t reg_gmut_coef[3][3];          
	int8_t  reg_gmut_shift;

	int8_t  reg_adpscl_mode;             
	int8_t  reg_adpscl1_mode;            
	int8_t  reg_adpscl1_sft;             
	char    reg_adpscl_bypass[3];        
	int16_t reg_adpscl_alpha[3];  
	char    reg_adpscl_beta_s[3];       
	AMLFLT  reg_adpscl_beta[3];      
    int16_t reg_adpscl_ys_coef[3];        
	int8_t  reg_adpscl_shift0;
	int8_t  reg_adpscl_shift1;             
	char    reg_adpscl_clip_en;            
	AMLFLT  reg_adpscl_max;             
    char    reg_ogain_blend;               
	int8_t  reg_maxRGB_sel;                
                                       
	char    reg_maxRGB_rshift;             
	char    reg_piecewise_mode;            
	char    reg_hist_win_mode;             
	int16_t reg_hist_proc_x_st;
	int16_t reg_hist_proc_x_ed;
	int16_t reg_hist_proc_y_st;
	int16_t reg_hist_proc_y_ed;

	uint16_t reg_c_gain_lim_coef[3];            
    uint16_t reg_c_gain_lim_maxrgb;             
	int64_t  ro_tm_hist[128];

//HW related registers
	int16_t reg_mtrxi_comp_thrd;
	int16_t reg_mtrxo_comp_thrd;
    int16_t reg_cgain_oft[3];
    char    reg_sel_opt;
    char    reg_adpscl_sel_opt;
    int8_t  reg_adpscl_shift2[3];
    char    reg_new_mode;
    int8_t  reg_matrixi_en_ctrl;
    int8_t  reg_matrixo_en_ctrl;
    int8_t  reg_vcbus_rd_idx;
    int     reg_gclk_ctrl0;

//new add
    char reg_hdr2_gm_comp_en ;
    int reg_hdr_comp_ofst_r ;
    int reg_hdr_comp_ofst_g ;
    int reg_hdr_comp_ofst_b ;
    int reg_hdr_comp_min_r  ;
    int reg_hdr_comp_min_g  ;
    int reg_hdr_comp_min_b  ;
    int reg_hdr_comp_rat_r  ;
    int reg_hdr_comp_rat_g  ;
    int reg_hdr_comp_rat_b  ;
} AML_VM_REG;

extern void sdr2hdr_regs_set(AML_VM_REG *aml_vm_reg);
extern void hdr2sdr_regs_set(AML_VM_REG *aml_vm_reg);
extern void hdr2_reg_sys_wr(AML_VM_REG *aml_vm_reg);

#endif  //_SET_HDR2_V0__H_

