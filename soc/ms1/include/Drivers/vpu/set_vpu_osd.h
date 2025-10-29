#ifndef _MNT_OSD_H_
#define _MNT_OSD_H_

#include "vpu_include.h"

//#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FOSD_WIN_NUM          8
#define FOSD_SHARE_RAM_WID    24 
#define FOSD_ELLIP_NUM        2
#define FOSD_RECT_NUM         5
#define FOSD_LINE_NUM         5

#define HAVE_OSD_ARR

#define FOSD_FONT_HNUM        12
#define FOSD_FONT_VNUM        18
#define CP_COLORKEY_F         1

#define FOSD_WIN_CLEAR_MODE   0

#define FOSD_FONT_REP_NUM     5

typedef enum {
    FONT_TYPE_1B  = 1,
    FONT_TYPE_2B  = 2,
    FONT_TYPE_4B  = 4,
    WIN0_TYPE_IDX = 5,    
    WIN1_TYPE_IDX = 6,    
    WIN2_TYPE_IDX = 7,    
    WIN3_TYPE_IDX = 8,    
    WIN4_TYPE_IDX = 9,    
    WIN5_TYPE_IDX = 10,    
    WIN6_TYPE_IDX = 11,    
    WIN7_TYPE_IDX = 12,    
    FONT_TYPE_REP0= 13,    
    FONT_TYPE_REP1= 14,    
    FONT_TYPE_REP2= 15,    
    FONT_TYPE_REP3= 16,    
    FONT_TYPE_REP4= 17    
} MNT_OSD_FONT_TYPE_t;

typedef enum {
    RECT_HORI_GRAD        = 1,
    RECT_HORI_CENTER_GRAD = 2,
    RECT_VERT_GRAD        = 3,
    RECT_VERT_CENTER_GRAD = 4,
    RECT_BILA_CENTER_GRAD = 5,
    RECT_PURE             = 6

} MNT_OSD_RECT_GRAD_TYPE_t;


typedef struct
{
    uint8_t u1ColorKey;
    uint8_t u8Red;
    uint8_t u8Green;
    uint8_t u8Blue;
} MNT_OSD_COLOR_PALETTE_t;


extern  MNT_OSD_COLOR_PALETTE_t osd_color_palette[];
extern  size_t PALETTE_SIZE;

extern uint32_t osd_win0_idx_arr[];
extern uint32_t osd_win1_idx_arr[];
extern uint32_t osd_win2_idx_arr[];
extern uint32_t osd_win3_idx_arr[];
extern uint32_t osd_win4_idx_arr[];
extern uint32_t osd_win5_idx_arr[];
extern uint32_t osd_win6_idx_arr[];
extern uint32_t osd_win7_idx_arr[];
extern uint32_t osd_1b_font_repack_arr0[4096];
extern uint32_t osd_1b_font_repack_arr1[4096];
extern uint32_t osd_1b_font_repack_arr2[];
extern uint32_t osd_1b_font_repack_arr3[];
extern uint32_t osd_1b_font_repack_arr4[];

extern  size_t  WIN0_SIZE;
extern  size_t  WIN1_SIZE;
extern  size_t  WIN2_SIZE;
extern  size_t  WIN3_SIZE;
extern  size_t  WIN4_SIZE;
extern  size_t  WIN5_SIZE;
extern  size_t  WIN6_SIZE;
extern  size_t  WIN7_SIZE;

extern uint32_t font_1b_arr[];
extern uint32_t font_2b_arr[];
extern uint32_t font_4b_arr[];

extern  size_t  FONT_1B_SIZE;
extern  size_t  FONT_2B_SIZE;
extern  size_t  FONT_4B_SIZE;

typedef struct {
   // uint8_t osd_win_update_en[FOSD_WIN_NUM]       ;
    uint8_t osd_win_update_mode[FOSD_WIN_NUM]     ;
    uint8_t osd_win_update_alpha[FOSD_WIN_NUM]    ;
    uint8_t osd_win_update_r[FOSD_WIN_NUM]        ;
    uint8_t osd_win_update_g[FOSD_WIN_NUM]        ;
    uint8_t osd_win_update_b[FOSD_WIN_NUM]        ;
    uint8_t osd_win_update_lt_x[FOSD_WIN_NUM]     ;
    uint8_t osd_win_update_lt_y[FOSD_WIN_NUM]     ;
    uint8_t osd_win_update_rb_x[FOSD_WIN_NUM]     ;
    uint8_t osd_win_update_rb_y[FOSD_WIN_NUM]     ;
  //  uint8_t osd_win_idx_crop_en[FOSD_WIN_NUM]     ;
  //  uint8_t osd_win_idx_hofst[FOSD_WIN_NUM]       ;
  //  uint8_t osd_win_idx_vofst[FOSD_WIN_NUM]       ;
  //  uint8_t osd_win_idx_stride[FOSD_WIN_NUM]      ;

 //   uint8_t osd_win_update_fifo_en                ;
 //   uint8_t osd_win_update_fifo_mode              ;

} MNT_OSD_WIN_UPDATE_t;
typedef struct {
    uint8_t  osd_font_rep_en                          ;
    uint8_t  osd_font_gap                             ;
    uint8_t  osd_font_nrep_num                        ;
    uint32_t osd_font_nrep_st_addr[FOSD_FONT_REP_NUM] ;
    uint32_t osd_font_nrep_sum_num[FOSD_FONT_REP_NUM] ;
    uint8_t  osd_font_rep_num                         ;
    uint32_t osd_font_rep_st_addr[FOSD_FONT_REP_NUM]  ;
    uint32_t osd_font_rep_sum_num[FOSD_FONT_REP_NUM]  ;
} MNT_OSD_FONT_REPACK_t;

typedef struct {
    //char     osd_win_idx_file[FOSD_WIN_NUM][100]           ;
    //char     osd_1bit_font_file[100]          ;
    //char     osd_2bit_font_file[100]          ;
    //char     osd_4bit_font_file[100]          ;
    //char     osd_color_file[100]              ;

    uint32_t osd_win_addr_st[FOSD_WIN_NUM]    ;
    uint32_t osd_1bit_font_addr_st            ;
    uint32_t osd_2bit_font_addr_st            ;
    uint32_t osd_4bit_font_addr_st            ;
    uint32_t osd_2bit_font_idx_st_th          ;
    uint32_t osd_4bit_font_idx_st_th          ;
   // uint32_t osd_4bit_font_idx_num            ;
    uint32_t osd_win_lt_x[FOSD_WIN_NUM]       ;
    uint32_t osd_win_lt_y[FOSD_WIN_NUM]       ;
    uint8_t  osd_win_en[FOSD_WIN_NUM]         ;
    uint8_t  osd_win_vflip_en[FOSD_WIN_NUM]   ;
    uint8_t  osd_win_hmirr_en[FOSD_WIN_NUM]   ;
    uint8_t  osd_win_rot_90                   ;
    uint8_t  osd_win_rot_180                  ;
    uint8_t  osd_win_rot_270                  ;
    uint8_t  osd_font_vflip_en[FOSD_WIN_NUM]  ;
    uint8_t  osd_font_hmirr_en[FOSD_WIN_NUM]  ;
    uint32_t osd_blend_alpha[FOSD_WIN_NUM]    ;
    uint32_t osd_font_hnum[FOSD_WIN_NUM]      ;
    uint32_t osd_font_vnum[FOSD_WIN_NUM]      ;
    uint8_t  osd_win_invalid_en               ;
    uint8_t  osd_win_invalid_r                ;
    uint8_t  osd_win_invalid_g                ;
    uint8_t  osd_win_invalid_b                ;
    uint8_t  osd_win_invalid_alpha            ;   
    MNT_OSD_WIN_UPDATE_t osd_win_update_info  ;
    MNT_OSD_FONT_REPACK_t osd_font_repack_info;

} MNT_OSD_WIN_t;

typedef struct {
    uint32_t  osd_ellip_center_x[FOSD_ELLIP_NUM]    ;
    uint32_t  osd_ellip_center_y[FOSD_ELLIP_NUM]    ;
    uint8_t   osd_ellip_en[FOSD_ELLIP_NUM]          ;
    uint8_t   osd_ellip_mode[FOSD_ELLIP_NUM]        ;
    uint32_t  osd_ellip_thick[FOSD_ELLIP_NUM]       ;
    uint32_t  osd_ellip_hor_radius[FOSD_ELLIP_NUM]  ;
    uint32_t  osd_ellip_ver_radius[FOSD_ELLIP_NUM]  ;
    uint8_t   osd_ellip_r[FOSD_ELLIP_NUM]           ;
    uint8_t   osd_ellip_g[FOSD_ELLIP_NUM]           ;
    uint8_t   osd_ellip_b[FOSD_ELLIP_NUM]           ;
    uint8_t   osd_ellip_alpha[FOSD_ELLIP_NUM]       ;
   
} MNT_OSD_ELLIP_t;

typedef struct {
    uint32_t  osd_rect_st_x[FOSD_RECT_NUM]     ;
    uint32_t  osd_rect_st_y[FOSD_RECT_NUM]     ;
    uint32_t  osd_rect_w[FOSD_RECT_NUM]        ;
    uint32_t  osd_rect_h[FOSD_RECT_NUM]        ;
    uint8_t   osd_rect_en[FOSD_RECT_NUM]       ;
    uint32_t  osd_rect_thick[FOSD_RECT_NUM]    ;
    MNT_OSD_RECT_GRAD_TYPE_t  osd_rect_grad_mode[FOSD_RECT_NUM];
    uint32_t  osd_rect_mode[FOSD_RECT_NUM]     ;
    uint8_t   osd_rect_step_r[FOSD_RECT_NUM]   ;
    uint8_t   osd_rect_step_g[FOSD_RECT_NUM]   ;
    uint8_t   osd_rect_step_b[FOSD_RECT_NUM]   ;
    uint8_t   osd_rect_st_r[FOSD_RECT_NUM]     ;
    uint8_t   osd_rect_st_g[FOSD_RECT_NUM]     ;
    uint8_t   osd_rect_st_b[FOSD_RECT_NUM]     ;
    uint8_t   osd_rect_ed_r[FOSD_RECT_NUM]     ;
    uint8_t   osd_rect_ed_g[FOSD_RECT_NUM]     ;
    uint8_t   osd_rect_ed_b[FOSD_RECT_NUM]     ;
    uint8_t   osd_rect_alpha[FOSD_RECT_NUM]    ; 
    uint8_t   osd_rect_invalid_en              ;
    uint8_t   osd_rect_invalid_r               ;
    uint8_t   osd_rect_invalid_g               ;
    uint8_t   osd_rect_invalid_b               ;
    uint8_t   osd_rect_invalid_alpha           ;
} MNT_OSD_RECT_t;

typedef struct {
    uint32_t  osd_line_st_x[FOSD_LINE_NUM]     ;
    uint32_t  osd_line_st_y[FOSD_LINE_NUM]     ;
    uint32_t  osd_line_ed_x[FOSD_LINE_NUM]     ;
    uint32_t  osd_line_ed_y[FOSD_LINE_NUM]     ;
    uint8_t   osd_line_en[FOSD_LINE_NUM]       ;
    uint32_t  osd_line_thick[FOSD_LINE_NUM]    ;
    uint8_t   osd_line_r[FOSD_LINE_NUM]        ;
    uint8_t   osd_line_g[FOSD_LINE_NUM]        ;
    uint8_t   osd_line_b[FOSD_LINE_NUM]        ;
    uint8_t   osd_line_alpha[FOSD_LINE_NUM]    ; 
    uint8_t   osd_line_invalid_en              ;
    uint8_t   osd_line_invalid_r               ;
    uint8_t   osd_line_invalid_g               ;
    uint8_t   osd_line_invalid_b               ;
    uint8_t   osd_line_invalid_alpha           ;
} MNT_OSD_LINE_t;

typedef struct MNT_OSD_TYPE{
    uint32_t         osd_en         ;
    MNT_OSD_WIN_t    osd_win_info   ;
    MNT_OSD_ELLIP_t  osd_ellip_info ;
    MNT_OSD_RECT_t   osd_rect_info  ;
    MNT_OSD_LINE_t   osd_line_info  ;

} MNT_OSD_t; 







extern void init_vpu_osd(MNT_OSD_t *mnt_vpu_osd);
extern void set_vpu_osd(MNT_OSD_t *mnt_vpu_osd);

extern void read_font_to_mem(uint32_t *arr,int addr_st,uint32_t arr_size);
extern void init_color_mem(MNT_OSD_COLOR_PALETTE_t* arr, int addr_st);

extern void init_osd_win_font(MNT_OSD_t *mnt_vpu_osd);
extern void set_osd_win_font(MNT_OSD_t *mnt_vpu_osd);
extern void init_osd_rect(MNT_OSD_t *mnt_vpu_osd);
extern void set_osd_rect(MNT_OSD_t *mnt_vpu_osd);
extern void init_osd_ellip(MNT_OSD_t *mnt_vpu_osd);
extern void set_osd_ellip(MNT_OSD_t *mnt_vpu_osd);
extern void init_osd_line(MNT_OSD_t *mnt_vpu_osd);
extern void set_osd_line(MNT_OSD_t *mnt_vpu_osd);
extern void cfg_win_font_ctrl(MNT_OSD_t *mnt_vpu_osd );
extern uint8_t get_font_rot_info(MNT_OSD_t *mnt_vpu_osd );
extern int  get_font_type_start_idx(MNT_OSD_FONT_TYPE_t font_type, MNT_OSD_t *mnt_vpu_osd );
extern void cfg_font_rot_ctrl(MNT_OSD_t *mnt_vpu_osd );
extern void cfg_win_en(int win_idx, uint8_t win_en );
extern void cfg_win_alpha(int win_idx, int win_alpha );
extern void cfg_win_baseaddr(int win_idx, int win_base_addr );
extern void cfg_win_position(int win_idx, int win_st_x, int win_st_y,int font_hnum, int font_vnum ,uint8_t rot_en);
extern void cfg_font_baseaddr(MNT_OSD_FONT_TYPE_t font_type, int font_baddr );
extern void cfg_font_start_idx(MNT_OSD_FONT_TYPE_t font_type, int font_st_idx );
extern void cfg_ram_lut_addr(uint8_t is_share_ram, int buf_addr );
extern void cfg_ram_lut_data(uint8_t is_share_ram, int buf_data );
extern void cfg_rect_en(int rect_idx, uint8_t rect_en );
extern void cfg_rect_position(int rect_idx, int rect_st_x, int rect_st_y, int rect_width, int rect_height  );
extern void cfg_rect_mode_info(int rect_idx, MNT_OSD_RECT_GRAD_TYPE_t rect_grad_mode, int rect_mode, int rect_thick, int rect_st_r ,int rect_st_g ,int rect_st_b ,int rect_ed_r ,int rect_ed_g ,int rect_ed_b ,int rect_alpha,int rect_width ,int rect_height ,int step_r,int step_g ,int step_b  );
extern int  get_rect_delta(int rect_step_level, int rect_width , int color_delta );
extern void cfg_ellip_en(int ellip_idx, uint8_t ellip_en, uint8_t ellip_mode );
extern void cfg_ellip_position(int ellip_idx, int ellip_center_x, int ellip_center_y,int ellip_line_width, int ellip_hradius, int ellip_vradius);
extern void cfg_ellip_color(int ellip_idx, int ellip_color_r, int ellip_color_g, int ellip_color_b, int ellip_alpha);
extern void cfg_line_en(int line_idx, uint8_t line_en );
extern void cfg_line_position(int line_idx, int line_st_x, int line_st_y, int line_ed_x, int line_ed_y,int line_width);
extern void cfg_line_color(int line_idx, int line_color_r, int line_color_g, int line_color_b, int line_alpha);
extern int  get_line_k_abs (int line_st_x, int line_st_y, int line_ed_x, int line_ed_y);
extern void update_osd_share_ram(MNT_OSD_t *mnt_vpu_osd, int upd_win_en, int upd_win_alpha ,int upd_win_baddr,int upd_font_baddr,int upd_font_th_st,int wait_update_en);
extern void clear_win_info(int win_idx, int clear_local_win, int win_alpha, int win_color_rgb,int win_st_x, int win_st_y ,int win_ed_x, int win_ed_y);
extern void close_clear_win_info();
extern void osd_win_idx_local_update(int win_idx, uint32_t *share_ram_arr,int win_new_addr,int arr_size);
extern void osd_font_local_update(MNT_OSD_FONT_TYPE_t font_type,uint32_t *share_ram_arr,int font_new_addr,int arr_size);
extern void osd_color_ram_local_update(MNT_OSD_COLOR_PALETTE_t *color_ram_arr, int color_new_addr);
extern void cfg_osd_win_crop_mode(int win_idx,int hofst, int vofst, int stride,int win_st_x,int win_st_y,int font_hnum,int font_vnum,int rot_en);

extern void cfg_win_invalid_area(uint8_t en ,uint8_t color_r,uint8_t color_g,uint8_t color_b,uint8_t color_alpha ) ;
extern void cfg_rect_invalid_area(uint8_t en ,uint8_t color_r,uint8_t color_g,uint8_t color_b,uint8_t color_alpha );
extern void cfg_line_invalid_area(uint8_t en ,uint8_t color_r,uint8_t color_g,uint8_t color_b,uint8_t color_alpha );
extern void set_osd_en(int osd_en);
extern void cfg_font_repack_mode(uint8_t repack_en);
extern void cfg_font_repack_gap(uint8_t font_gap);
extern void upd_init_arr(MNT_OSD_FONT_TYPE_t sel, uint32_t *arr );
extern void cfg_font_repack(MNT_OSD_FONT_REPACK_t *osd_font_repack_info);
 

#ifdef VPU_FPGA_EN
extern void init_osd_win_font_fpga(MNT_OSD_t *mnt_vpu_osd);
extern void init_osd_rect_fpga(MNT_OSD_t *mnt_vpu_osd);
extern void init_osd_ellip_fpga(MNT_OSD_t *mnt_vpu_osd);
extern void init_osd_line_fpga(MNT_OSD_t *mnt_vpu_osd);
#endif































#endif //_MNT_OSD_H_
