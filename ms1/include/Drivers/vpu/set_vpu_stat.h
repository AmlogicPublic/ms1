#ifndef _MNT_STAT_H_
#define _MNT_STAT_H_

#include "vpu_include.h"

typedef struct MNT_STAT_TYPE{
    uint8_t  stat_en      ;//{hist_en,lc_en,vesa_en}
    uint16_t din_hsize    ;
    uint16_t din_vsize    ;
    //dnlp_hist
    uint8_t  hist_din_sel ;//0:hdr_din, 1:hdr_dout, 2:sharp_dout, 3:cm_dout, 4:blend_dout
    char     hist_din_fmt ;//0: yuv in, 1: rgb in
    char     hist_rgb_mode;//0: y/r in, 1: max{r,g,b}
    char     hist_win_en  ;
    uint16_t hist_win_hbgn;
    uint16_t hist_win_hend;
    uint16_t hist_win_vbgn;
    uint16_t hist_win_vend;

    uint8_t  lc_pix_drop  ;//
    char     lc_eol_en    ;
    uint8_t  lc_hist_mode ;//hist mode: 0: comp0 hist only, 1: Max(comp0,1,2) for hist, 2: the hist of all comp0,1,2 are calculated
    char     lc_lpf_en    ;//1: 1,2,1 filter on before finding max& hist
    uint8_t  lc_din_sel   ;//0:di  1:vdin  2:null  3:postblend  4:vpp out  5:vd1  6:vd2  7:osd1
    char     lc_matrx_mode;//0:rgb2yuv  1:yuv2rgb  

    uint16_t lc_col_start ;
    uint16_t lc_row_start ;
    //lc_curve
    uint8_t  lc_blk_hnum  ;//1~12
    uint8_t  lc_blk_vnum  ;//1~8
    char     curv_intr_en ;
    char     curv_rd_en   ;
    int      curv_info[12*8*2]; 
    //vesa stat
    uint8_t  vesa_glb_sel ;//0:blend out 1:vadj out  2:lud3d out 3:wb out    4:gamma out  5:ldc out  7:dnlp_csc out
    uint8_t  vesa_wnd_sel ;//0:csc out   1:lud3d out 2:wb out    3:gamma out 4:ldc out  7:dnlp_csc out
    char     vesa_rgb_mode;//when input is rgb, 0:rgb blend  1:rgb max
    uint16_t vesa_win_hbgn;
    uint16_t vesa_win_hend;
    uint16_t vesa_win_vbgn;
    uint16_t vesa_win_vend;
} MNT_STAT_t;

extern void set_vi_hist_regs(
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
);

extern void lc_set_matrix_rgb2ycbcr(int mode);
extern void lc_set_matrix_ycbcr2rgb(int mode);

extern void lc_stts_en(
        char     enable       ,
        uint16_t hsize        ,
        uint16_t vsize        ,
        uint8_t  pix_drop_mode,//
        char     eol_en       ,
        uint8_t  hist_mode    ,//hist mode: 0: comp0 hist only, 1: Max(comp0,1,2) for hist, 2: the hist of all comp0,1,2 are calculated
        char     lpf_en       ,//1: 1,2,1 filter on before finding max& hist
        uint8_t  din_sel      ,//0:di  1:vdin  2:null  3:postblend  4:vpp out  5:vd1  6:vd2  7:osd1
        char     matrix_mode   //0:rgb2yuv  1:yuv2rgb  
);

extern void lc_set_region(
        uint16_t hsize     ,
        uint16_t vsize     ,
        uint16_t col_start,
        uint16_t row_start ,
        uint8_t  blk_hnum , //1~12
        uint8_t  blk_vnum   //1~8
);

extern void set_lc_curve(char enable, uint16_t width, uint16_t height, uint8_t blk_hnum, uint8_t blk_vnum, char intr_enable);
extern void read_lc_curve(int szCurveInfo[12*8*2]);

extern void set_vesa_statics(
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
);
extern void get_vesa_statics(uint16_t glb_max_luma, uint16_t glb_avg_luma, uint16_t wnd_avg_color[3]);

extern void set_static_infos(MNT_STAT_t *prm_stat);

#endif //_MNT_STAT_H_

