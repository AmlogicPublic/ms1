#include "mnt_vpu.h"

///////////////////////////////////////////////////////////////////////////////////////////
// init_mnt_vpu
///////////////////////////////////////////////////////////////////////////////////////////

void init_mnt_vpu(MNT_VPU_t *mnt_vpu, MNT_ENABLE_e mode){

//====================================================
// patgen
//====================================================
    mnt_vpu->mnt_pat_gen.patgen_tmn           = PATGEN_TMN_256X144;
    mnt_vpu->mnt_pat_gen.patgen_mode          = 1;

//====================================================
// Top
//====================================================
    mnt_vpu->mnt_vpu_top.src_input_hsize      = 2560;
    mnt_vpu->mnt_vpu_top.src_input_vsize      = 1440;
    mnt_vpu->mnt_vpu_top.disp_output_hsize    = 2560;
    mnt_vpu->mnt_vpu_top.disp_output_vsize    = 1440;

    mnt_vpu->mnt_vpu_top.patgen_en            = 0;
    mnt_vpu->mnt_vpu_top.scaler_en            = mode==ALLON ? 1 : mode==ALLBYPS ? 0 : 0;
    mnt_vpu->mnt_vpu_top.hdr_en               = mode==ALLON ? 1 : mode==ALLBYPS ? 0 : 1;
    mnt_vpu->mnt_vpu_top.stat_en              = mode==ALLON ? 7 : mode==ALLBYPS ? 0 : 0;//{hist_en,lc_en,vesa_en}
    mnt_vpu->mnt_vpu_top.sharp_en             = mode==ALLON ? 1 : mode==ALLBYPS ? 0 : 0;
    mnt_vpu->mnt_vpu_top.contrast_en          = mode==ALLON ? 7 : mode==ALLBYPS ? 0 : 0;//{lc_en,dnlp_en,cc_en}
    mnt_vpu->mnt_vpu_top.cm_en                = mode==ALLON ? 1 : mode==ALLBYPS ? 0 : 1;
    mnt_vpu->mnt_vpu_top.osd_en               = mode==ALLON ? 1 : mode==ALLBYPS ? 0 : 0;
    mnt_vpu->mnt_vpu_top.vadj_conv_en         = mode==ALLON ? 7 : mode==ALLBYPS ? 2 : 2;//{vadj_en,csc_en,rgbbst_en}
    mnt_vpu->mnt_vpu_top.lut3d_en             = mode==ALLON ? 1 : mode==ALLBYPS ? 0 : 0;
    mnt_vpu->mnt_vpu_top.gamut_en             = mode==ALLON ? 3 : mode==ALLBYPS ? 0 : 0;//{matrix_en,eotf_en,oetf_en}
    mnt_vpu->mnt_vpu_top.wb_en                = mode==ALLON ? 1 : mode==ALLBYPS ? 0 : 1;
    mnt_vpu->mnt_vpu_top.gamma_en             = mode==ALLON ? 1 : mode==ALLBYPS ? 0 : 1;
    mnt_vpu->mnt_vpu_top.ldc_en               = mode==ALLON ? 1 : mode==ALLBYPS ? 0 : 0;

    mnt_vpu->mnt_vpu_top.vadj_pos             = 0;//vadj  before blend
    mnt_vpu->mnt_vpu_top.gamma_pos            = 0;//gamma before ldc

//====================================================
// vpu_proc
//====================================================

    //scaler
    //mnt_vpu->mnt_pps.src_hsize                = 2560;
    //mnt_vpu->mnt_pps.src_vsize                = 1440;
    //mnt_vpu->mnt_pps.dst_hsize                = 2560;
    //mnt_vpu->mnt_pps.dst_vsize                = 1440;
    //mnt_vpu->mnt_pps.prehsc_en                = 0;
    //mnt_vpu->mnt_pps.prevsc_en                = 0;
    //mnt_vpu->mnt_pps.prehsc_ratio             = 0;
    //mnt_vpu->mnt_pps.prevsc_ratio             = 0;
    //mnt_vpu->mnt_pps.posthsc_en               = 1;
    //mnt_vpu->mnt_pps.postvsc_en               = 1;
    //mnt_vpu->mnt_pps.pps_dbg_en               = 0;

    //hdr
    mnt_vpu->mnt_vpu_hdr.hdr2_sel             = HDR2; // 
    mnt_vpu->mnt_vpu_hdr.hdr2_top_en          = 1;    //hdr enable 1:open hdr, 0 :close hdr
    mnt_vpu->mnt_vpu_hdr.hdr2_only_mat        = 0;    //1: hdr only used as matrix
    mnt_vpu->mnt_vpu_hdr.hdr2_fmt_cfg         = 0;    //0: sdr to hdr    1: hdr to sdr, 2: ?, 3: hlg+ 
    mnt_vpu->mnt_vpu_hdr.in_fmt               = 1;    //1:yuv in   0:rgb in
    mnt_vpu->mnt_vpu_hdr.rgb_out_en           = 0;    //1:rgb out  0:yuv out

    //stat
    mnt_vpu->mnt_vpu_stat.stat_en             = 0x7;  //{hist_en,lc_en,vesa_en}
    mnt_vpu->mnt_vpu_stat.din_hsize           = 2560;
    mnt_vpu->mnt_vpu_stat.din_vsize           = 1440;
    mnt_vpu->mnt_vpu_stat.hist_din_sel        = 0;    //0:hdr in  1:hdr out  2:sharp out  3:cm out  4:blend out
    mnt_vpu->mnt_vpu_stat.hist_din_fmt        = 0;   //0: yuv in, 1: rgb in
    mnt_vpu->mnt_vpu_stat.hist_rgb_mode       = 0;   //0: y hist, 1: max{r,g,b} hist
    mnt_vpu->mnt_vpu_stat.hist_win_en         = 1;
    mnt_vpu->mnt_vpu_stat.hist_win_hbgn       = 0;
    mnt_vpu->mnt_vpu_stat.hist_win_hend       = 2559;
    mnt_vpu->mnt_vpu_stat.hist_win_vbgn       = 0;
    mnt_vpu->mnt_vpu_stat.hist_win_vend       = 1439;
    mnt_vpu->mnt_vpu_stat.lc_pix_drop         = 0;   //
    mnt_vpu->mnt_vpu_stat.lc_eol_en           = 1;
    mnt_vpu->mnt_vpu_stat.lc_hist_mode        = 0;   //hist mode: 0: comp0 hist only, 1: Max(comp0,1,2) for hist, 2: the hist of all comp0,1,2 are calculated
    mnt_vpu->mnt_vpu_stat.lc_lpf_en           = 0;   //1: 1,2,1 filter on before finding max& hist
    mnt_vpu->mnt_vpu_stat.lc_din_sel          = 0;   //0:hdr in   1:hdr out  2:sharp out  3:cm out  4:blend out
    mnt_vpu->mnt_vpu_stat.lc_matrx_mode       = 0;   //0:yuv2rgb  1:rgb2yuv
    mnt_vpu->mnt_vpu_stat.lc_col_start        = 0;
    mnt_vpu->mnt_vpu_stat.lc_row_start        = 0;
    mnt_vpu->mnt_vpu_stat.lc_blk_hnum         = 12;
    mnt_vpu->mnt_vpu_stat.lc_blk_vnum         = 8;
    mnt_vpu->mnt_vpu_stat.curv_intr_en        = 1;
    mnt_vpu->mnt_vpu_stat.curv_rd_en          = 0;
    mnt_vpu->mnt_vpu_stat.vesa_glb_sel        = 0;   //0:blend out 1:vadj out  2:lud3d out 3:wb out    4:gamma out  5:ldc out  7:dnlp_csc out
    mnt_vpu->mnt_vpu_stat.vesa_wnd_sel        = 4;   //0:csc out   1:lud3d out 2:wb out    3:gamma out 4:ldc out  7:dnlp_csc out
    mnt_vpu->mnt_vpu_stat.vesa_rgb_mode       = 0;   //when input is rgb, 0:rgb blend  1:rgb max
    mnt_vpu->mnt_vpu_stat.vesa_win_hbgn       = 0;
    mnt_vpu->mnt_vpu_stat.vesa_win_hend       = 127;
    mnt_vpu->mnt_vpu_stat.vesa_win_vbgn       = 0;
    mnt_vpu->mnt_vpu_stat.vesa_win_vend       = 127;

    //sharp
    mnt_vpu->mnt_vpu_sharp.sharp_en           = 0;
    mnt_vpu->mnt_vpu_sharp.contrast_en        = 0x7; //{lc_en,dnlp_en,cc_en}
    mnt_vpu->mnt_vpu_sharp.lc_wr_lut_en       = 0;
    mnt_vpu->mnt_vpu_sharp.lc_wr_lut_hw       = 0;
    mnt_vpu->mnt_vpu_sharp.lc_mapping_hnum    = 12;
    mnt_vpu->mnt_vpu_sharp.lc_mapping_vnum    = 8;
    mnt_vpu->mnt_vpu_sharp.lc_mapping_hsize   = 2560;
    mnt_vpu->mnt_vpu_sharp.lc_mapping_hsize   = 1440;

    for(uint8_t m=0;m<12*8*2;m++) {
        mnt_vpu->mnt_vpu_stat.curv_info[m]    = 0;
        mnt_vpu->mnt_vpu_sharp.curve_info[m]  = 0;
    }

    //cm
    mnt_vpu->mnt_vpu_cm.hsize                 = 2560;
    mnt_vpu->mnt_vpu_cm.vsize                 = 1440;
    mnt_vpu->mnt_vpu_cm.cm2_en                = 0;
    mnt_vpu->mnt_vpu_cm.hue_adj_en            = 1;
    mnt_vpu->mnt_vpu_cm.sat_adj_en            = 1;
    mnt_vpu->mnt_vpu_cm.luma_adj_en           = 1;
    mnt_vpu->mnt_vpu_cm.xvycc_mode            = 0;

    //osd
    mnt_vpu->mnt_vpu_osd.osd_en               = 1;
    init_vpu_osd(&mnt_vpu->mnt_vpu_osd);

    //blend
    mnt_vpu->mnt_vpu_bld.vid_proc.layer_h_start = 0;
    mnt_vpu->mnt_vpu_bld.vid_proc.layer_h_end   = 2559;
    mnt_vpu->mnt_vpu_bld.vid_proc.layer_v_start = 0;
    mnt_vpu->mnt_vpu_bld.vid_proc.layer_v_end   = 1439;

    mnt_vpu->mnt_vpu_bld.osd_font.layer_h_start = 0;
    mnt_vpu->mnt_vpu_bld.osd_font.layer_h_end   = 2559;
    mnt_vpu->mnt_vpu_bld.osd_font.layer_v_start = 0;
    mnt_vpu->mnt_vpu_bld.osd_font.layer_v_end   = 1439;

    mnt_vpu->mnt_vpu_bld.osd_rect.layer_h_start = 0;
    mnt_vpu->mnt_vpu_bld.osd_rect.layer_h_end   = 2559;
    mnt_vpu->mnt_vpu_bld.osd_rect.layer_v_start = 0;
    mnt_vpu->mnt_vpu_bld.osd_rect.layer_v_end   = 1439;

    mnt_vpu->mnt_vpu_bld.osd_line.layer_h_start = 0;
    mnt_vpu->mnt_vpu_bld.osd_line.layer_h_end   = 2559;
    mnt_vpu->mnt_vpu_bld.osd_line.layer_v_start = 0;
    mnt_vpu->mnt_vpu_bld.osd_line.layer_v_end   = 1439;

    mnt_vpu->mnt_vpu_bld.disp_order[0]          = VID_PROC_LAYER;
    mnt_vpu->mnt_vpu_bld.disp_order[1]          = BLD_NULL_LAYER;
    mnt_vpu->mnt_vpu_bld.disp_order[2]          = BLD_NULL_LAYER;
    mnt_vpu->mnt_vpu_bld.disp_order[3]          = BLD_NULL_LAYER;

    //vadj&csc&rgbbst
    mnt_vpu->mnt_vpu_vadj.vadj_en               = 0x7;//{vadj_en,csc_en,rgbbst_en}
    mnt_vpu->mnt_vpu_vadj.vadj_minus_black_en   = 1;
    mnt_vpu->mnt_vpu_vadj.vadj_bri              = 0;
    mnt_vpu->mnt_vpu_vadj.vadj_con              = 128;
    mnt_vpu->mnt_vpu_vadj.vadj_ma               = 256;
    mnt_vpu->mnt_vpu_vadj.vadj_mb               = 0;
    mnt_vpu->mnt_vpu_vadj.vadj_mc               = 0;
    mnt_vpu->mnt_vpu_vadj.vadj_md               = 256;
    mnt_vpu->mnt_vpu_vadj.soft_curve_0_a        = 0;
    mnt_vpu->mnt_vpu_vadj.soft_curve_0_b        = 64;
    mnt_vpu->mnt_vpu_vadj.soft_curve_0_ci       = 128;
    mnt_vpu->mnt_vpu_vadj.soft_curve_0_cs       = 5;
    mnt_vpu->mnt_vpu_vadj.soft_curve_0_g        = -255;
    mnt_vpu->mnt_vpu_vadj.soft_curve_1_a        = 576;
    mnt_vpu->mnt_vpu_vadj.soft_curve_1_b        = 480;
    mnt_vpu->mnt_vpu_vadj.soft_curve_1_ci       = 137;
    mnt_vpu->mnt_vpu_vadj.soft_curve_1_cs       = 2;
    mnt_vpu->mnt_vpu_vadj.soft_curve_1_g        = 255;

    //lut3d
    mnt_vpu->mnt_vpu_lut3d.lut3d_en             = 1;
    mnt_vpu->mnt_vpu_lut3d.gamut_en             = 0x3; //{matrix_en,eotf_en,oetf_en}
    mnt_vpu->mnt_vpu_lut3d.lut3d_load           = 1;
    mnt_vpu->mnt_vpu_lut3d.lut3d_check          = 0;
    mnt_vpu->mnt_vpu_lut3d.lut3d_mode           = 0x15;

    //wb
    mnt_vpu->mnt_vpu_wb.enable                  = 1;
    mnt_vpu->mnt_vpu_wb.gain0                   = 1024;
    mnt_vpu->mnt_vpu_wb.gain1                   = 1024;
    mnt_vpu->mnt_vpu_wb.gain2                   = 1024;
    mnt_vpu->mnt_vpu_wb.pre_offset0             = 0;
    mnt_vpu->mnt_vpu_wb.pre_offset1             = 0;
    mnt_vpu->mnt_vpu_wb.pre_offset2             = 0;
    mnt_vpu->mnt_vpu_wb.offset0                 = 0;
    mnt_vpu->mnt_vpu_wb.offset1                 = 0;
    mnt_vpu->mnt_vpu_wb.offset2                 = 0;

//====================================================
// vpu_disp
//====================================================

    //gamma
    init_lcd_gamma(&mnt_vpu->mnt_vpu_gamma);

    //ldc
    ldc_reg_init(&mnt_vpu->mnt_vpu_ldc);

}


///////////////////////////////////////////////////////////////////////////////////////////
// set_mnt_vpu
///////////////////////////////////////////////////////////////////////////////////////////

void set_mnt_vpu(MNT_VPU_t *mnt_vpu){

//====================================================
// Vout
//====================================================

#ifndef VERIFICATION
    //Wr_reg_bits(VOUT_PRE_TOP_CTRL0, 0,30,1);//vout send pix to vpu
    //Wr_reg_bits(0x10200, 0,30,1);//vout send pix to vpu
#endif

    open_mnt_vpu_reg_update();
//====================================================
// Top
//====================================================

    //OSD_SIZE == BLEND_SIZE,JUST FOR TESTING
    Wr(VPU_TOP_FONT_OSD_SIZE,(mnt_vpu->mnt_vpu_top.disp_output_hsize<<16)|
                             (mnt_vpu->mnt_vpu_top.disp_output_vsize    ));
    Wr(VPU_TOP_RECT_OSD_SIZE,(mnt_vpu->mnt_vpu_top.disp_output_hsize<<16)|
                             (mnt_vpu->mnt_vpu_top.disp_output_vsize    ));
    Wr(VPU_TOP_LINE_OSD_SIZE,(mnt_vpu->mnt_vpu_top.disp_output_hsize<<16)|
                             (mnt_vpu->mnt_vpu_top.disp_output_vsize    ));

    Wr(VPU_TOP_BLEND_SIZE,   (mnt_vpu->mnt_vpu_top.disp_output_hsize<<16)|
                             (mnt_vpu->mnt_vpu_top.disp_output_vsize    ));
    
    Wr(VPU_SC_SIZE_IN    ,   (mnt_vpu->mnt_vpu_top.src_input_hsize  <<16)|
                             (mnt_vpu->mnt_vpu_top.src_input_vsize      ));
    Wr(VPU_VIDEO_SIZE    ,   (mnt_vpu->mnt_vpu_top.src_input_hsize  <<16)|
                             (mnt_vpu->mnt_vpu_top.src_input_vsize      ));    

    Wr_reg_bits(VPU_PROC_VADJ_POS, mnt_vpu->mnt_vpu_top.vadj_pos ,0,1);//reg_vadj_aft_bld
    Wr_reg_bits(LDC_GAMMA_POS    , mnt_vpu->mnt_vpu_top.gamma_pos,0,1);//reg_gam_aft_ldc

//====================================================
// vpu_proc
//====================================================
    char contrast_en   =  mnt_vpu->mnt_vpu_top.contrast_en != 0;
    char vadj_en       = (mnt_vpu->mnt_vpu_top.vadj_conv_en>>2) & 0x1;
    char csc_en        = (mnt_vpu->mnt_vpu_top.vadj_conv_en>>1) & 0x1;
    char lut3d_wrap_en =  mnt_vpu->mnt_vpu_top.lut3d_en || mnt_vpu->mnt_vpu_top.gamut_en;

//  Wr(VPU_PROC_SHARP_SIZE, (mnt_vpu->mnt_vpu_top.disp_output_hsize<<16)|
//                          (mnt_vpu->mnt_vpu_top.disp_output_vsize    ));

    Wr(VPU_TOP_ENHANCE_ENABLE,  (mnt_vpu->mnt_vpu_top.sharp_en<<7)|
                                (mnt_vpu->mnt_vpu_top.cm_en   <<6)|
                                (vadj_en                      <<5)|
                                (contrast_en                  <<4)|
                                (csc_en                       <<3)|
                                (lut3d_wrap_en                <<2)|
                                (mnt_vpu->mnt_vpu_top.wb_en   <<1)|
                                (mnt_vpu->mnt_vpu_top.gamma_en<<0));

    //----------------------------------------------------
    // scaler
    //----------------------------------------------------
    //mnt_vpu->mnt_pps.src_hsize       = mnt_vpu->mnt_vpu_top.src_input_hsize  ;
    //mnt_vpu->mnt_pps.src_vsize       = mnt_vpu->mnt_vpu_top.src_input_vsize  ;
    //mnt_vpu->mnt_pps.dst_hsize       = mnt_vpu->mnt_vpu_top.disp_output_hsize;
    //mnt_vpu->mnt_pps.dst_vsize       = mnt_vpu->mnt_vpu_top.disp_output_vsize;

    //if(mnt_vpu->mnt_vpu_top.scaler_en==0) {
    //    //mnt_vpu->mnt_pps.prehsc_en   = 0;
    //    //mnt_vpu->mnt_pps.prevsc_en   = 0;
    //    mnt_vpu->mnt_pps.posthsc_en  = 0;
    //    mnt_vpu->mnt_pps.postvsc_en  = 0;
    //}

    //if(mnt_vpu->mnt_pps.pps_dbg_en!=0){//just for debug,generate pps cfg  
    //    cfg_mnt_pps(0,&mnt_vpu->mnt_pps);
    //}

    //----------------------------------------------------
    // hdr
    //----------------------------------------------------
    mnt_vpu->mnt_vpu_hdr.hdr2_top_en = mnt_vpu->mnt_vpu_top.hdr_en;

    set_hdr2_v0(&mnt_vpu->mnt_vpu_hdr);

    printf("[set_mnt_vpu] set_vpu_hdr2!!!\n");

    //----------------------------------------------------
    // stat
    //----------------------------------------------------
    mnt_vpu->mnt_vpu_stat.stat_en    = mnt_vpu->mnt_vpu_top.stat_en          ;
    mnt_vpu->mnt_vpu_stat.din_hsize  = mnt_vpu->mnt_vpu_top.disp_output_hsize;
    mnt_vpu->mnt_vpu_stat.din_vsize  = mnt_vpu->mnt_vpu_top.disp_output_vsize;

    set_static_infos(&mnt_vpu->mnt_vpu_stat);

    printf("[set_mnt_vpu] set_vpu_static_infos!!!\n");

    //----------------------------------------------------
    // sharp&contrast
    //----------------------------------------------------
    mnt_vpu->mnt_vpu_sharp.sharp_en         = mnt_vpu->mnt_vpu_top.sharp_en         ;
    mnt_vpu->mnt_vpu_sharp.contrast_en      = mnt_vpu->mnt_vpu_top.contrast_en      ;
    mnt_vpu->mnt_vpu_sharp.lc_mapping_hsize = mnt_vpu->mnt_vpu_top.disp_output_hsize;
    mnt_vpu->mnt_vpu_sharp.lc_mapping_vsize = mnt_vpu->mnt_vpu_top.disp_output_vsize;
    mnt_vpu->mnt_vpu_sharp.lc_mapping_hnum  = mnt_vpu->mnt_vpu_stat.lc_blk_hnum     ;
    mnt_vpu->mnt_vpu_sharp.lc_mapping_vnum  = mnt_vpu->mnt_vpu_stat.lc_blk_vnum     ;
    mnt_vpu->mnt_vpu_sharp.lc_wr_lut_en     = mnt_vpu->mnt_vpu_stat.curv_rd_en      ;

    for(int i=0;i<(mnt_vpu->mnt_vpu_sharp.lc_mapping_hnum*mnt_vpu->mnt_vpu_sharp.lc_mapping_vnum*2);i++) {
        mnt_vpu->mnt_vpu_sharp.curve_info[i] = mnt_vpu->mnt_vpu_sharp.lc_wr_lut_en ? mnt_vpu->mnt_vpu_stat.curv_info[i] : 0;
    }

    set_vpu_sharp(&mnt_vpu->mnt_vpu_sharp);
    printf("[set_mnt_vpu] set_vpu_sharp!!\n");

    //----------------------------------------------------
    // cm
    //----------------------------------------------------
    mnt_vpu->mnt_vpu_cm.cm2_en       = mnt_vpu->mnt_vpu_top.cm_en   ;

    set_chroma_regs(&mnt_vpu->mnt_vpu_cm);

    printf("[set_mnt_vpu] set_vpu_cm!!!\n");

    //----------------------------------------------------
    // osd
    //----------------------------------------------------
    mnt_vpu->mnt_vpu_osd.osd_en      = mnt_vpu->mnt_vpu_top.osd_en  ;

    if(mnt_vpu->mnt_vpu_osd.osd_en){
        set_vpu_osd(&mnt_vpu->mnt_vpu_osd);
    }

    printf("[set_mnt_vpu] set_vpu_osd!!!\n");

    //----------------------------------------------------
    // blend
    //----------------------------------------------------
    mnt_vpu->mnt_vpu_bld.vid_proc.layer_h_start =                                        0;
    mnt_vpu->mnt_vpu_bld.vid_proc.layer_h_end   = mnt_vpu->mnt_vpu_top.disp_output_hsize-1;
    mnt_vpu->mnt_vpu_bld.vid_proc.layer_v_start =                                        0;
    mnt_vpu->mnt_vpu_bld.vid_proc.layer_v_end   = mnt_vpu->mnt_vpu_top.disp_output_vsize-1;

    mnt_vpu->mnt_vpu_bld.osd_font.layer_h_start =                                        0;
    mnt_vpu->mnt_vpu_bld.osd_font.layer_h_end   = mnt_vpu->mnt_vpu_top.disp_output_hsize-1;
    mnt_vpu->mnt_vpu_bld.osd_font.layer_v_start =                                        0;
    mnt_vpu->mnt_vpu_bld.osd_font.layer_v_end   = mnt_vpu->mnt_vpu_top.disp_output_vsize-1;

    mnt_vpu->mnt_vpu_bld.osd_rect.layer_h_start =                                        0;
    mnt_vpu->mnt_vpu_bld.osd_rect.layer_h_end   = mnt_vpu->mnt_vpu_top.disp_output_hsize-1;
    mnt_vpu->mnt_vpu_bld.osd_rect.layer_v_start =                                        0;
    mnt_vpu->mnt_vpu_bld.osd_rect.layer_v_end   = mnt_vpu->mnt_vpu_top.disp_output_vsize-1;

    mnt_vpu->mnt_vpu_bld.osd_line.layer_h_start =                                        0;
    mnt_vpu->mnt_vpu_bld.osd_line.layer_h_end   = mnt_vpu->mnt_vpu_top.disp_output_hsize-1;
    mnt_vpu->mnt_vpu_bld.osd_line.layer_v_start =                                        0;
    mnt_vpu->mnt_vpu_bld.osd_line.layer_v_end   = mnt_vpu->mnt_vpu_top.disp_output_vsize-1;

    set_mnt_blend(&mnt_vpu->mnt_vpu_bld);

    printf("[set_mnt_vpu] set_vpu_blend!!!\n");

    //----------------------------------------------------
    // vadj&csc
    //----------------------------------------------------
    mnt_vpu->mnt_vpu_vadj.vadj_en    = mnt_vpu->mnt_vpu_top.vadj_conv_en;

    set_video_adjust_1_regs(&mnt_vpu->mnt_vpu_vadj);

    //----------------------------------------------------
    // lut3d
    //----------------------------------------------------
    mnt_vpu->mnt_vpu_lut3d.lut3d_en  = mnt_vpu->mnt_vpu_top.lut3d_en;
    mnt_vpu->mnt_vpu_lut3d.gamut_en  = mnt_vpu->mnt_vpu_top.gamut_en;

    printf("config lut3d\n");
    set_lut3d_gamut(&mnt_vpu->mnt_vpu_lut3d);

    //----------------------------------------------------
    // wb
    //----------------------------------------------------
    mnt_vpu->mnt_vpu_wb.enable       = mnt_vpu->mnt_vpu_top.wb_en   ;

    printf("config gain_offset\n");
    set_gain_offset_regs(&mnt_vpu->mnt_vpu_wb);

//====================================================
// vpu_disp
//====================================================

    //----------------------------------------------------
    // gamma
    //----------------------------------------------------
    mnt_vpu->mnt_vpu_gamma.gamma_en  = mnt_vpu->mnt_vpu_top.gamma_en;

    set_lcd_gamma_new(&mnt_vpu->mnt_vpu_gamma);

    //----------------------------------------------------
    // ldc
    //----------------------------------------------------

    mnt_vpu->mnt_vpu_ldc.ldc_en     = mnt_vpu->mnt_vpu_top.ldc_en;
    mnt_vpu->mnt_vpu_ldc.img_width  = mnt_vpu->mnt_vpu_top.disp_output_hsize;
    mnt_vpu->mnt_vpu_ldc.img_height = mnt_vpu->mnt_vpu_top.disp_output_vsize;
    printf("config begin mnt_vpu_ldc\n");
    ldc_reg_cfg(&mnt_vpu->mnt_vpu_ldc);  //todo @liqiang

//====================================================
// Patgen&timing
//====================================================

    printf("[set_mnt_vpu] patgen_en=%d\n", mnt_vpu->mnt_vpu_top.patgen_en);
    if(mnt_vpu->mnt_vpu_top.patgen_en) {
        set_pat_gen(mnt_vpu->mnt_pat_gen.patgen_tmn, mnt_vpu->mnt_pat_gen.patgen_mode);
        Wr(VPU_TOP_TIMING_CTRL    ,(0x210)| (1<<20)| (1<<24));
    } else {
        Wr(VPU_TOP_TIMING_CTRL    ,(0x210)| (0<<20)| (0<<24));
    }

    printf("[set_mnt_vpu] set_vpu_end!!!\n");
    
#ifdef VERIFICATION    
    set_vpu_top_load_data_event();//TODO
#endif
    
}

void close_mnt_vpu_reg_update(){
    Wr_reg_bits(VPU_TOP_UPDATE_CTRL0,0,24,2);//reg_disp_reg_update_en
    Wr_reg_bits(VPU_TOP_UPDATE_CTRL0,0,4,16);//reg_proc_reg_update_en
    Wr_reg_bits(VPU_TOP_UPDATE_CTRL0,0,0,1 );//reg_vtop_reg_update_en
}
void open_mnt_vpu_reg_update(){
    Wr_reg_bits(VPU_TOP_UPDATE_CTRL0,3     ,24,2);//reg_disp_reg_update_en
    Wr_reg_bits(VPU_TOP_UPDATE_CTRL0,0xffff,4,16);//reg_proc_reg_update_en
    Wr_reg_bits(VPU_TOP_UPDATE_CTRL0,1     ,0,1 );//reg_vtop_reg_update_en
}
void set_vpu_top_load_data_event()
{
#ifndef VPU_FPGA_EN
    stimulus_event(STIMULUS_VPU_TOP_LOAD_SRC_EVENT, 0);
#else
    //stimulus_event(STIMULUS_VPU_TOP_LOAD_SRC_EVENT, 0);
#endif
    //stimulus_wait_event_done(STIMULUS_VPU_TOP_LOAD_SRC_EVENT)
}
