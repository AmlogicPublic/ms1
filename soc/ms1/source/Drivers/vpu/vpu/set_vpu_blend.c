#include "set_vpu_blend.h"


void set_mnt_blend(MNT_BLEND_t *mnt_blend)
{
    Wr(VPU_PROC_BLD_SRC0_HSCOPE,(mnt_blend->vid_proc.layer_h_end<<16) | mnt_blend->vid_proc.layer_h_start);
    Wr(VPU_PROC_BLD_SRC0_VSCOPE,(mnt_blend->vid_proc.layer_v_end<<16) | mnt_blend->vid_proc.layer_v_start);
    
    Wr(VPU_PROC_BLD_SRC1_HSCOPE,(mnt_blend->osd_font.layer_h_end<<16) | mnt_blend->osd_font.layer_h_start);
    Wr(VPU_PROC_BLD_SRC1_VSCOPE,(mnt_blend->osd_font.layer_v_end<<16) | mnt_blend->osd_font.layer_v_start);
                                                                                           
    Wr(VPU_PROC_BLD_SRC2_HSCOPE,(mnt_blend->osd_rect.layer_h_end<<16) | mnt_blend->osd_rect.layer_h_start);
    Wr(VPU_PROC_BLD_SRC2_VSCOPE,(mnt_blend->osd_rect.layer_v_end<<16) | mnt_blend->osd_rect.layer_v_start);
                                                                                           
    Wr(VPU_PROC_BLD_SRC3_HSCOPE,(mnt_blend->osd_line.layer_h_end<<16) | mnt_blend->osd_line.layer_h_start);
    Wr(VPU_PROC_BLD_SRC3_VSCOPE,(mnt_blend->osd_line.layer_v_end<<16) | mnt_blend->osd_line.layer_v_start);
    
    //Wr(VPU_PROC_BLD_DIN_REORDER,(1                       <<31)| 
    //                            (mnt_blend->disp_order[3]<<12)|
    //                            (mnt_blend->disp_order[2]<<8 )|
    //                            (mnt_blend->disp_order[1]<<4 )|
    //                            (mnt_blend->disp_order[0]<<0 ));

    Wr(VPU_PROC_BLD_EN,1);
    Wr(VPU_PROC_BLD0_CTRL,mnt_blend->disp_order[0]);
    Wr(VPU_PROC_BLD1_CTRL,mnt_blend->disp_order[1]);
    Wr(VPU_PROC_BLD2_CTRL,mnt_blend->disp_order[2]);
    Wr(VPU_PROC_BLD3_CTRL,mnt_blend->disp_order[3]);
}
