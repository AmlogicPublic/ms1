#include "set_vpu_stat.h"

//===============================================
// set post histogram statistics
//===============================================
void set_vi_hist_regs(
        char     enable   ,
        uint8_t  din_sel  ,
        char     din_fmt  ,
        char     max_rgb  ,
        uint16_t hsize    ,
        uint16_t vsize    ,
        char     win_en   ,
        uint16_t win_h_bgn,
        uint16_t win_h_end,
        uint16_t win_v_bgn,
        uint16_t win_v_end 
)
{
    Wr(VI_HIST_CTRL       ,(  din_sel <<11)|   //hist_din_sel 0:hdr_din, 1:hdr_dout, 2:sharp_dout, 3:cm_dout, 4:blend_dout
                           (   win_en << 1)|   //hist_win_en 1:en
                           (   enable << 0));  //hist_spl_en 1:en
    Wr(STAT_HIST_PIC_SIZE ,(    hsize <<16)|   //pic width
                           (    vsize << 0));  //pic height
    Wr(VI_HIST_H_START_END,(win_h_end <<16)|   //win_h_end
                           (win_h_bgn << 0));  //win_h_start
    Wr(VI_HIST_V_START_END,(win_v_end <<16)|   //win_v_end
                           (win_v_bgn << 0));  //win_v_start
    Wr_reg_bits(VI_RO_HIST_LOW_IDX, 1, 7, 1);  //hist_idx_clr
    Wr(VI_RO_HIST_LOW_IDX ,(        1 << 6)|   //idx_auto_clr
                           (        0 << 0));  //hist_idx_init

    //add xiaomi dnlp csc cfg
    Wr(DNLP_MAX_RGB_EN    ,(  max_rgb << 1)|   //0: y hist, 1: max{r,g,b} hist
                           (  din_fmt << 0));  //0: yuv in, 1: rgb in
    Wr(DNLP_HIST_MINMAX_8B,(       64 << 8)|   //hist_min_8b
                           (      128 << 0));  //hist_max_8b
    //dnlp_scene_change
    Wr(STAT_DNLP_SCH_ENABLE,   enable);
}

void lc_set_matrix_rgb2ycbcr (int mode)
{
    int16_t matrix_coef[3][3] = {{0x0107, 0x0204, 0x0064}, // 0.257   0.504   0.098
                                 {0x1f68, 0x1ed6, 0x01c2}, //-0.148  -0.291   0.439
                                 {0x01c2, 0x1e87, 0x1fb7}};// 0.439  -0.368  -0.071

    if (mode == 0) //ycbcr not full range, 601 conversion
    {
        Wr(LC_STA_CSC_MAT_0_1, ((matrix_coef[0][0]&0x1fff)<<16)|
                               ((matrix_coef[0][1]&0x1fff)    ));
        Wr(LC_STA_CSC_MAT_2_3, ((matrix_coef[0][2]&0x1fff)<<16)|
                               ((matrix_coef[1][0]&0x1fff)    ));
        Wr(LC_STA_CSC_MAT_4_5, ((matrix_coef[1][1]&0x1fff)<<16)|
                               ((matrix_coef[1][2]&0x1fff)    ));
        Wr(LC_STA_CSC_MAT_6_7, ((matrix_coef[2][0]&0x1fff)<<16)|
                               ((matrix_coef[2][1]&0x1fff)    ));
        Wr(LC_STA_CSC_MAT_8  , ((matrix_coef[2][2]&0x1fff)    ));

        Wr(LC_STA_CSC_OFST   , (0x0200<<16)|
                               (0     <<0 ));
    }
    else if (mode == 1) //ycbcr full range, 601 conversion
    {
    }
}  

void lc_set_matrix_ycbcr2rgb (int mode)
{
    int16_t matrix_coef[3][3] = {{0x04a8, 0x0000, 0x0662}, //1.164   0       1.596
                                 {0x04a8, 0x1e6f, 0x1cbf}, //1.164  -0.392  -0.813
                                 {0x04a8, 0x0811, 0x0000}};//1.164   2.017   0

    if (mode == 0) //ycbcr not full range, 601 conversion 
    {
        Wr(LC_STA_CSC_OFST   , (0xfc0<<16)|
                               (0xe00    )); //0xfc00e00);

        Wr(LC_STA_CSC_MAT_0_1, ((matrix_coef[0][0]&0x1fff)<<16)|
                               ((matrix_coef[0][1]&0x1fff)    ));
        Wr(LC_STA_CSC_MAT_2_3, ((matrix_coef[0][2]&0x1fff)<<16)|
                               ((matrix_coef[1][0]&0x1fff)    ));
        Wr(LC_STA_CSC_MAT_4_5, ((matrix_coef[1][1]&0x1fff)<<16)|
                               ((matrix_coef[1][2]&0x1fff)    ));
        Wr(LC_STA_CSC_MAT_6_7, ((matrix_coef[2][0]&0x1fff)<<16)|
                               ((matrix_coef[2][1]&0x1fff)    ));
        Wr(LC_STA_CSC_MAT_8  , ((matrix_coef[2][2]&0x1fff)    ));

    }
    else if (mode == 1) //ycbcr full range, 601 conversion
    {
    }
}

void lc_stts_en(
        char     enable       ,
        uint16_t hsize        ,
        uint16_t vsize        ,
        uint8_t  pix_drop_mode, //
        char     eol_en       ,
        uint8_t  hist_mode    , //hist mode: 0: compy hist only, 1: Max(comp0,1,2) for hist
        char     lpf_en       , //1: 1,2,1 filter on before finding max& hist
        uint8_t  din_sel      , //0:hdr in   1:hdr out  2:sharp out  3:cm out  4:blend out
        char     matrix_mode    //0:yuv2rgb  1:rgb2yuv
)
{
    Wr_reg_bits(STAT_INFO_GCLK_CTRL,  0x0, 16, 8 );
    Wr(STAT_LC_INPUT_SIZE, ((hsize&0x1fff) << 16)|
                           ((vsize&0x1fff) <<  0));
    Wr(STAT_LC_STTS_CTRL0, ((eol_en & 0x1) <<  4)|
                           ((enable & 0x1) <<  2));

    Wr(LC_STA_CTRL, ((din_sel       & 0x7) << 20)|
                    ((matrix_mode   & 0x1) << 16)|
                    ((lpf_en        & 0x1) << 12)|
                    ((hist_mode     & 0x1) <<  4)|
                    ((pix_drop_mode & 0x3) <<  0));

    if(matrix_mode)
        lc_set_matrix_rgb2ycbcr(0);
    else
        lc_set_matrix_ycbcr2rgb(0);
}

void lc_set_region(
        uint16_t hsize     ,
        uint16_t vsize     ,
        uint16_t col_start,
        uint16_t row_start ,
        uint8_t  blk_hnum , //1~12
        uint8_t  blk_vnum   //1~8
)
{
    uint16_t blk_width  = (hsize-col_start)/blk_hnum;
    uint16_t blk_height = (vsize-row_start)/blk_vnum;
  uint16_t hend0 , hend1 , hend2 , hend3 , hend4 , hend5 , hend6 , hend7 , hend8 , hend9;
  uint16_t hend10, hend11;
  uint16_t vend0 , vend1 , vend2 , vend3 , vend4 , vend5 , vend6 , vend7;
  int      data32;
    blk_width  = (blk_width >20) ? blk_width  : 20;
    blk_height = (blk_height>10) ? blk_height : 10;

    hend0  = col_start + blk_width - 1;
    hend1  = (hend0 +blk_width)>=hsize ? 0x1fff : (hend0 +blk_width);
    hend2  = (hend1 +blk_width)>=hsize ? 0x1fff : (hend1 +blk_width);
    hend3  = (hend2 +blk_width)>=hsize ? 0x1fff : (hend2 +blk_width);
    hend4  = (hend3 +blk_width)>=hsize ? 0x1fff : (hend3 +blk_width);
    hend5  = (hend4 +blk_width)>=hsize ? 0x1fff : (hend4 +blk_width);
    hend6  = (hend5 +blk_width)>=hsize ? 0x1fff : (hend5 +blk_width);
    hend7  = (hend6 +blk_width)>=hsize ? 0x1fff : (hend6 +blk_width);
    hend8  = (hend7 +blk_width)>=hsize ? 0x1fff : (hend7 +blk_width);
    hend9  = (hend8 +blk_width)>=hsize ? 0x1fff : (hend8 +blk_width);
    hend10 = (hend9 +blk_width)>=hsize ? 0x1fff : (hend9 +blk_width);
    hend11 = (hend10+blk_width)>=hsize ? 0x1fff : hsize;//(hend10+blk_width);

    vend0 = row_start + blk_height - 1;
    vend1  = (vend0+blk_height)>=vsize ? 0x1fff : (vend0+blk_height);
    vend2  = (vend1+blk_height)>=vsize ? 0x1fff : (vend1+blk_height);
    vend3  = (vend2+blk_height)>=vsize ? 0x1fff : (vend2+blk_height);
    vend4  = (vend3+blk_height)>=vsize ? 0x1fff : (vend3+blk_height);
    vend5  = (vend4+blk_height)>=vsize ? 0x1fff : (vend4+blk_height);
    vend6  = (vend5+blk_height)>=vsize ? 0x1fff : (vend5+blk_height);
    vend7  = (vend6+blk_height)>=vsize ? 0x1fff : vsize;//(vend6+blk_height);

    data32 = Rd(LC_STTS_HIST_REGION_IDX);
    Wr(LC_STTS_HIST_REGION_IDX, 0xffe0ffff & data32);

    Wr(LC_STA_HIDX_0_1  , (((hend0  & 0x1fff) << 16) | (col_start & 0x1fff)));
    Wr(LC_STA_HIDX_2_3  , (((hend2  & 0x1fff) << 16) | (hend1     & 0x1fff)));
    Wr(LC_STA_HIDX_4_5  , (((hend4  & 0x1fff) << 16) | (hend3     & 0x1fff)));
    Wr(LC_STA_HIDX_6_7  , (((hend6  & 0x1fff) << 16) | (hend5     & 0x1fff)));
    Wr(LC_STA_HIDX_8_9  , (((hend8  & 0x1fff) << 16) | (hend7     & 0x1fff)));
    Wr(LC_STA_HIDX_10_11, (((hend10 & 0x1fff) << 16) | (hend9     & 0x1fff)));
    Wr(LC_STA_HIDX_12   ,   (hend11 & 0x1fff)                               );
    Wr(LC_STA_VIDX_0_1  , (((vend0  & 0x1fff) << 16) | (row_start & 0x1fff)));
    Wr(LC_STA_VIDX_2_3  , (((vend2  & 0x1fff) << 16) | (vend1     & 0x1fff)));
    Wr(LC_STA_VIDX_4_5  , (((vend4  & 0x1fff) << 16) | (vend3     & 0x1fff)));
    Wr(LC_STA_VIDX_6_7  , (((vend6  & 0x1fff) << 16) | (vend5     & 0x1fff)));
    Wr(LC_STA_VIDX_8    ,   (vend7  & 0x1fff)                               );
    Wr(LC_STA_HV_NUM    , (((blk_vnum & 0x1f) <<  8) | (blk_hnum  & 0x1f  ))); //region number
}

void set_lc_curve(char enable, uint16_t width, uint16_t height, uint8_t blk_hnum, uint8_t blk_vnum, char intr_enable) {
  unsigned int reg_lc_histvld_thrd;
  unsigned int reg_lc_blackbar_mute_thrd;
  unsigned int reg_lmtrat_minmax;

  reg_lmtrat_minmax = (Rd(LC_CURVE_LMT_RAT)>>8) & 0xff; 
  reg_lc_histvld_thrd = (((width*height)/(blk_hnum*blk_vnum)) * reg_lmtrat_minmax)>>10;
  reg_lc_blackbar_mute_thrd = ((width*height)/(blk_hnum*blk_vnum))>>3;

  if(enable==0) {
     Wr_reg_bits(LC_CURVE_CTRL,0,0,1); 
   } else {
     
     Wr(LC_CURVE_HV_NUM,(blk_hnum<<8)|blk_vnum);
     Wr(LC_CURVE_HISTVLD_THRD,reg_lc_histvld_thrd);
     Wr(LC_CURVE_BB_MUTE_THRD,reg_lc_blackbar_mute_thrd);
     //interrupt mask
     //Wr_reg_bits(LC_CURVE_CTRL,1,31,1); 
     Wr_reg_bits(LC_CURVE_CTRL,1,0,1); 
     Wr_reg_bits(LC_CURVE_CTRL,intr_enable,31,1); //interrupt unmask 
   }
}

void read_lc_curve(int szCurveInfo[12*8*2]) {
  int blk_hnum;
  int blk_vnum;
  int i;
  unsigned int dwTemp;

  dwTemp = Rd(LC_CURVE_HV_NUM);
  blk_hnum = (dwTemp>>8)&0x1f;
  blk_vnum = (dwTemp)&0x1f;

  Wr(LC_CURVE_RAM_CTRL,1);
  Wr(LC_CURVE_RAM_ADDR,0);
  for(i=0;i<blk_hnum*blk_vnum;i++) {
      szCurveInfo[i*2+0] = dwTemp = Rd(LC_CURVE_RAM_DATA);
      szCurveInfo[i*2+1] = dwTemp = Rd(LC_CURVE_RAM_DATA);
   }
  Wr(LC_CURVE_RAM_CTRL,0);
}

void set_vesa_statics(
        char     enable     ,
        uint8_t  glb_din_sel,  //0:blend out 1:vadj out  2:lud3d out 3:wb out    4:gamma out  5:ldc out  7:dnlp_csc out
        uint8_t  wnd_din_sel,  //0:csc out   1:lud3d out 2:wb out    3:gamma out 4:ldc out  7:dnlp_csc out
        char     glb_rgb_mod,  //when input is rgb, 0:rgb blend  1:rgb max
        uint16_t hsize      ,
        uint16_t vsize      ,
        uint16_t win_h_bgn  ,
        uint16_t win_h_end  ,
        uint16_t win_v_bgn  ,
        uint16_t win_v_end   
)
{
    char glb_inp_rgb = glb_din_sel<2 ? 0 //input is yuv: blend/vadj out
                                     : 1;//input is rgb: lut3d/wb/gamma/ldc out

    Wr(STAT_VESA_STTS_CTRL,(     enable << 1)| //glb_en
                           (     enable << 0));//wnd_en
    Wr(STAT_VESA_FRM_SIZE ,(      hsize <<16)|
                           (      vsize << 0));

    Wr(VESA_STATICS_CTRL  ,(glb_din_sel << 8)|
                           (wnd_din_sel << 4)|
                           (glb_rgb_mod << 1)|
                           (glb_inp_rgb << 0));
    Wr(VESA_STATICS_WND   ,(  win_v_end <<16)|
                           (  win_v_bgn << 0));
    Wr(VESA_STATICS_WND_1 ,(  win_h_end <<16)|
                           (  win_h_bgn << 0));
}

void get_vesa_statics(uint16_t glb_max_luma, uint16_t glb_avg_luma, uint16_t wnd_avg_color[3])
{
    uint32_t glb_luma_val = Rd(VESA_STATICS_GLB      );
    uint32_t wnd_rgb_val0 = Rd(VESA_STATICS_WND_AVG  );
    uint32_t wnd_rgb_val1 = Rd(VESA_STATICS_WND_AVG_1);
    glb_max_luma     = (uint16_t)((glb_luma_val>>16)&0xfff);
    glb_avg_luma     = (uint16_t)((glb_luma_val>> 0)&0xfff);
    wnd_avg_color[2] = (uint16_t)((wnd_rgb_val0>>16)&0xfff);
    wnd_avg_color[1] = (uint16_t)((wnd_rgb_val0>> 0)&0xfff);
    wnd_avg_color[0] = (uint16_t)((wnd_rgb_val1>> 0)&0xfff);
}
void set_static_infos(MNT_STAT_t *prm_stat)
{
    char hist_enable = (prm_stat->stat_en>>2) & 0x1;
    char lc_enable   = (prm_stat->stat_en>>1) & 0x1;
    char vesa_enable = (prm_stat->stat_en>>0) & 0x1;

    //dnlp_hist
    prm_stat->hist_win_hbgn =                     0;
    prm_stat->hist_win_hend = prm_stat->din_hsize-1;
    prm_stat->hist_win_vbgn =                     0;
    prm_stat->hist_win_vend = prm_stat->din_vsize-1;

    set_vi_hist_regs(
        hist_enable            ,
        prm_stat->hist_din_sel ,
        prm_stat->hist_din_fmt ,
        prm_stat->hist_rgb_mode,
        prm_stat->din_hsize    ,
        prm_stat->din_vsize    ,
        prm_stat->hist_win_en  ,
        prm_stat->hist_win_hbgn,
        prm_stat->hist_win_hend,
        prm_stat->hist_win_vbgn,
        prm_stat->hist_win_vend 
    );

    //lc_stts
    lc_stts_en(
        lc_enable              ,
        prm_stat->din_hsize    ,//int hsize ,
        prm_stat->din_vsize    ,//int vsize ,
        prm_stat->lc_pix_drop  ,//int pix_drop_mode, //
        prm_stat->lc_eol_en    ,//int eol_en,
        prm_stat->lc_hist_mode ,//int hist_mode,     //hist mode: 0: compy hist only, 1: Max(comp0,1,2) for hist
        prm_stat->lc_lpf_en    ,//int lpf_en,        //1: 1,2,1 filter on before finding max& hist
        prm_stat->lc_din_sel   ,//int din_sel,       //0:di  1:vdin  2:null  3:postblend  4:core1 input  5:vd1  6:vd2  7:osd1
        prm_stat->lc_matrx_mode //int matrix_mode    //0:yuv2rgb  1:rgb2yuv
    );


    lc_set_region(
        prm_stat->din_hsize    ,//int hsize  ,
        prm_stat->din_vsize    ,//int vsize  ,
        prm_stat->lc_col_start ,
        prm_stat->lc_row_start ,//int row_start ,
        prm_stat->lc_blk_hnum  ,//1~12
        prm_stat->lc_blk_vnum   //1~8
    );

    //lc_curve
    set_lc_curve(
        lc_enable              ,
        prm_stat->din_hsize    ,
        prm_stat->din_vsize    ,
        prm_stat->lc_blk_hnum  ,
        prm_stat->lc_blk_vnum  ,
        prm_stat->curv_intr_en  
    );

    //lc_curve_iir fw&hw sel
    if(lc_enable) {
        if(prm_stat->curv_rd_en) {
//          read_lc_curve(&(prm_stat->curv_info[0])); 
            Wr_reg_bits(STAT_LC_CURVE_CTRL, 0, 0, 1);
        } else {
            Wr_reg_bits(STAT_LC_CURVE_CTRL, 1, 0, 1);
        }
    }

    //vesa_stts
    set_vesa_statics(
        vesa_enable            ,
        prm_stat->vesa_glb_sel ,
        prm_stat->vesa_wnd_sel ,
        prm_stat->vesa_rgb_mode,
        prm_stat->din_hsize    ,
        prm_stat->din_vsize    ,
        prm_stat->vesa_win_hbgn,
        prm_stat->vesa_win_hend,
        prm_stat->vesa_win_vbgn,
        prm_stat->vesa_win_vend 
    );
}
