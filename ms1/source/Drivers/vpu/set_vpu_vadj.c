#include "set_vpu_vadj.h"

void set_vadj_matrix(int mode)
{
    int16_t matrix_coef[3][3] = {{256,   0,  394},
                                 {256, -47, -118},
                                 {256, 465,    0}};
    int16_t matrix_3x3mtrx_rs = 0;

    if(mode==1){
        for(int i=0;i<3;i++) {
            for(int j=0;j<3;j++) {
                matrix_coef[i][j] = 0;//todo
            }
        }
    }

    Wr(VPU_VADJ1_CSC_COEF_00_01, ((matrix_coef[0][0]&0x1fff)<<16)|
                                 ((matrix_coef[0][1]&0x1fff)<< 0));
    Wr(VPU_VADJ1_CSC_COEF_02_10, ((matrix_coef[0][2]&0x1fff)<<16)|
                                 ((matrix_coef[1][0]&0x1fff)<< 0));
    Wr(VPU_VADJ1_CSC_COEF_11_12, ((matrix_coef[1][1]&0x1fff)<<16)|
                                 ((matrix_coef[1][2]&0x1fff)<< 0));
    Wr(VPU_VADJ1_CSC_COEF_20_21, ((matrix_coef[2][0]&0x1fff)<<16)|
                                 ((matrix_coef[2][1]&0x1fff)<< 0));
    Wr(VPU_VADJ1_CSC_COEF_22   , ((matrix_3x3mtrx_rs&0x1fff)<<16)|
                                 ((matrix_coef[2][2]&0x1fff)<< 0));

    Wr(VPU_VADJ1_CSC_OFFST_INP_0_1, ((    0 &0x7ff)<<16)|
                                    (((-512)&0x7ff)<< 0));
    Wr(VPU_VADJ1_CSC_OFFST_INP_2  , (((-512)&0x7ff)<< 0));

    Wr(VPU_VADJ1_CSC_OFFST_OUP_0_1, ((    0 &0x7ff)<<16)|
                                    ((    0 &0x7ff)<< 0));
    Wr(VPU_VADJ1_CSC_OFFST_OUP_2  , ((    0 &0x7ff)<< 0));
}

void set_rgb_contrast_bright(MNT_VADJ_t *mnt_vpu_vadj)
{
    char rgb_constrast_en    = (mnt_vpu_vadj->vadj_en>>0)&0x1   ;

    Wr_reg_bits(VPU_RGB_CTRST , rgb_constrast_en   , 1, 1);

}

void set_video_adjust_1_regs(MNT_VADJ_t *mnt_vpu_vadj)
{
    char vadj_en                 = (mnt_vpu_vadj->vadj_en>>2)&0x1   ;
    char csc_en                  = (mnt_vpu_vadj->vadj_en>>1)&0x1   ;

    //vadj
    char     vadj_minus_black_en = mnt_vpu_vadj->vadj_minus_black_en;
    uint16_t vadj_bri            = mnt_vpu_vadj->vadj_bri           ; //9 bit
    uint16_t vadj_con            = mnt_vpu_vadj->vadj_con           ; //8 bit
    uint16_t vadj_ma             = mnt_vpu_vadj->vadj_ma            ; //10 bit
    uint16_t vadj_mb             = mnt_vpu_vadj->vadj_mb            ; //10 bit
    uint16_t vadj_mc             = mnt_vpu_vadj->vadj_mc            ; //10 bit
    uint16_t vadj_md             = mnt_vpu_vadj->vadj_md            ; //10 bit
    uint16_t soft_curve_0_a      = mnt_vpu_vadj->soft_curve_0_a     ; //12 bit
    uint16_t soft_curve_0_b      = mnt_vpu_vadj->soft_curve_0_b     ; //12 bit
    uint16_t soft_curve_0_ci     = mnt_vpu_vadj->soft_curve_0_ci    ; //8 bit
    uint16_t soft_curve_0_cs     = mnt_vpu_vadj->soft_curve_0_cs    ; //3 bit
    uint16_t soft_curve_0_g      = mnt_vpu_vadj->soft_curve_0_g     ; //9 bit
    uint16_t soft_curve_1_a      = mnt_vpu_vadj->soft_curve_1_a     ;
    uint16_t soft_curve_1_b      = mnt_vpu_vadj->soft_curve_1_b     ;
    uint16_t soft_curve_1_ci     = mnt_vpu_vadj->soft_curve_1_ci    ;
    uint16_t soft_curve_1_cs     = mnt_vpu_vadj->soft_curve_1_cs    ;
    uint16_t soft_curve_1_g      = mnt_vpu_vadj->soft_curve_1_g     ;

    Wr(VPU_VADJ1_Y     , ( vadj_bri   << 8)|
                           vadj_con        );

    Wr(VPU_VADJ1_MA_MB , ((vadj_ma  &0x3FF      )<<16)|//s10
                         ((vadj_mb  &0x3FF      )   ));//s10
    Wr(VPU_VADJ1_MC_MD , ((vadj_mc  &0x3FF      )<<16)|//s10
                         ((vadj_md  &0x3FF      )   ));//s10

    Wr(VPU_VADJ1_CURV_0, ((soft_curve_0_ci      )<<24)|//u8
                         ((soft_curve_0_b       )<<12)|//u12
                         ((soft_curve_0_a &0xFFF)   ));//s12
    Wr(VPU_VADJ1_CURV_1, ((soft_curve_0_g &0x1FF)<< 3)|//s9
                          (soft_curve_0_cs      )    );//u3
    
    Wr(VPU_VADJ1_CURV_2, ((soft_curve_1_ci      )<<24)|//u8
                         ((soft_curve_1_b       )<<12)|//u12
                         ((soft_curve_1_a& 0xFFF)   ));//s12
    Wr(VPU_VADJ1_CURV_3, ((soft_curve_1_g &0x1FF)<< 3)|//s9
                         (soft_curve_1_cs      )    );//u3
    Wr_reg_bits(VPU_VADJ1_MISC, vadj_minus_black_en, 1, 1);
    Wr_reg_bits(VPU_VADJ1_MISC, vadj_en            , 0, 1);

    //csc
    if(csc_en) set_vadj_matrix(0);

    //rgbbst
    set_rgb_contrast_bright(mnt_vpu_vadj);

}

