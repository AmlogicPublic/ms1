#include "mnt_vpu.h"
#include "edp_tx_cfg.h"
#include <stdio.h>

volatile MNT_VPU_t mnt_vpu;

extern void vpu_run() {
  // ========================================================================
  // MNT Process,cfg initial
  // ========================================================================

  printf("init_mnt_vpu start\n\r");
  // init
  init_mnt_vpu((MNT_VPU_t*)&mnt_vpu, ALLON);
  printf("init_mnt_vpu end\n\r");

  // set
  mnt_vpu.mnt_vpu_top.hdr_en = 1;
  mnt_vpu.mnt_vpu_hdr.in_fmt = 0;     // 1:yuv in   0:rgb in
  mnt_vpu.mnt_vpu_hdr.rgb_out_en = 0; // 0:yuv      1:rgb
  mnt_vpu.mnt_vpu_hdr.hdr2_only_mat = 0;
  mnt_vpu.mnt_vpu_hdr.hdr2_fmt_cfg = 1; // 0:dark     1:light

  mnt_vpu.mnt_vpu_top.src_input_hsize = 1920;
  mnt_vpu.mnt_vpu_top.src_input_vsize = 1080;
  mnt_vpu.mnt_vpu_top.disp_output_hsize = 1920;
  mnt_vpu.mnt_vpu_top.disp_output_vsize = 1080;

  mnt_vpu.mnt_vpu_top.sharp_en = 0;
  // patgen
  mnt_vpu.mnt_vpu_top.patgen_en = 0;
  mnt_vpu.mnt_pat_gen.patgen_tmn = PATGEN_TMN_1920X1080;
  mnt_vpu.mnt_pat_gen.patgen_mode = 1; // fied value

  // osd start
  init_osd_win_font_fpga((MNT_OSD_t*)&mnt_vpu.mnt_vpu_osd);
  init_osd_rect_fpga((MNT_OSD_t*)&mnt_vpu.mnt_vpu_osd);
  init_osd_ellip_fpga((MNT_OSD_t*)&mnt_vpu.mnt_vpu_osd);
  init_osd_line_fpga((MNT_OSD_t*)&mnt_vpu.mnt_vpu_osd);
  // osd end
  // mnt_vpu.mnt_vpu_osd.osd_win_info.osd_win_rot_90 =1;
  // mnt_vpu.mnt_vpu_osd.osd_win_info.osd_win_rot_180 =1;
  // mnt_vpu.mnt_vpu_osd.osd_win_info.osd_win_rot_270 =1;
  update_osd_share_ram((MNT_OSD_t*)&mnt_vpu.mnt_vpu_osd, 0, 0, 0, 0, 0, 1);
  mnt_vpu.mnt_vpu_top.osd_en = 1;

  // blend
  // mnt_vpu.mnt_vpu_bld.vid_proc.layer_h_start = 0;
  // mnt_vpu.mnt_vpu_bld.vid_proc.layer_h_end   = 1279;
  // mnt_vpu.mnt_vpu_bld.vid_proc.layer_v_start = 0;
  // mnt_vpu.mnt_vpu_bld.vid_proc.layer_v_end   = 719;

  mnt_vpu.mnt_vpu_bld.osd_font.layer_h_start = 0;
  mnt_vpu.mnt_vpu_bld.osd_font.layer_h_end = 1279;
  mnt_vpu.mnt_vpu_bld.osd_font.layer_v_start = 0;
  mnt_vpu.mnt_vpu_bld.osd_font.layer_v_end = 719;

  mnt_vpu.mnt_vpu_bld.osd_rect.layer_h_start = 0;
  mnt_vpu.mnt_vpu_bld.osd_rect.layer_h_end = 1279;
  mnt_vpu.mnt_vpu_bld.osd_rect.layer_v_start = 0;
  mnt_vpu.mnt_vpu_bld.osd_rect.layer_v_end = 719;

  mnt_vpu.mnt_vpu_bld.osd_line.layer_h_start = 0;
  mnt_vpu.mnt_vpu_bld.osd_line.layer_h_end = 1279;
  mnt_vpu.mnt_vpu_bld.osd_line.layer_v_start = 0;
  mnt_vpu.mnt_vpu_bld.osd_line.layer_v_end = 719;

  mnt_vpu.mnt_vpu_bld.disp_order[0] = VID_PROC_LAYER;
  mnt_vpu.mnt_vpu_bld.disp_order[1] = mnt_vpu.mnt_vpu_top.osd_en == 0 ? BLD_NULL_LAYER : OSD_LINE_LAYER;
  mnt_vpu.mnt_vpu_bld.disp_order[2] = mnt_vpu.mnt_vpu_top.osd_en == 0 ? BLD_NULL_LAYER : OSD_RECT_LAYER;
  mnt_vpu.mnt_vpu_bld.disp_order[3] = mnt_vpu.mnt_vpu_top.osd_en == 0 ? BLD_NULL_LAYER : OSD_FONT_LAYER;

  // cfg
  set_mnt_vpu((MNT_VPU_t*)&mnt_vpu);

  printf("wait blank\n\r");
  // while(~Rd(VPU_TOP_BLANK_PHS)){};

  printf("data to vpu \n\r");
  // Wr_reg_bits(0x10200,1,30,1);//vout send pix to vpu
  edp_tx_cfg_v1();
};
