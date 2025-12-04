#ifndef _MNT_BLEND_H_
#define _MNT_BLEND_H_

#include "vpu_include.h"

typedef enum {
    VID_PROC_LAYER = 0,
    OSD_FONT_LAYER = 1,
    OSD_RECT_LAYER = 2,
    OSD_LINE_LAYER = 3,    
    BLD_NULL_LAYER = 4
} MNT_DISP_LAYER_t;   

typedef struct MNT_BLD_LAYER_TYPE{
   uint16_t layer_h_start;
   uint16_t layer_h_end  ;
   uint16_t layer_v_start;
   uint16_t layer_v_end  ;
} MNT_BLD_LAYER_t;

typedef struct MNT_BLEND_TYPE{   
    MNT_BLD_LAYER_t  vid_proc     ;
    MNT_BLD_LAYER_t  osd_font     ;
    MNT_BLD_LAYER_t  osd_rect     ;
    MNT_BLD_LAYER_t  osd_line     ;

    MNT_DISP_LAYER_t disp_order[4];    
    //uint8_t        disp_order[4];    
} MNT_BLEND_t;

extern void set_mnt_blend(MNT_BLEND_t *mnt_blend);

#endif //_MNT_BLEND_H_


