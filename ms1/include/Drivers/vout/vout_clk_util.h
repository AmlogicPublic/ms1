//
//***************************************************************************
// Title:      vpu_clk_util.h
//
// Author:     chong.gu	
//
// Created:    08:48:31 01/12/2020
//
// Description:
//
// Note:       
//
// History:    
//
//***************************************************************************

#ifndef VPU_CLK_UTIL_H
#define VPU_CLK_UTIL_H 

//#include "define.h"
#include "vout.h"

#define CLK_UTIL_VID_PLL_DIV_1      0
#define CLK_UTIL_VID_PLL_DIV_2      1
#define CLK_UTIL_VID_PLL_DIV_3      2
#define CLK_UTIL_VID_PLL_DIV_3p5    3
#define CLK_UTIL_VID_PLL_DIV_3p75   4
#define CLK_UTIL_VID_PLL_DIV_4      5
#define CLK_UTIL_VID_PLL_DIV_5      6
#define CLK_UTIL_VID_PLL_DIV_6      7
#define CLK_UTIL_VID_PLL_DIV_6p25   8
#define CLK_UTIL_VID_PLL_DIV_7      9
#define CLK_UTIL_VID_PLL_DIV_7p5    10
#define CLK_UTIL_VID_PLL_DIV_12     11
#define CLK_UTIL_VID_PLL_DIV_14     12
#define CLK_UTIL_VID_PLL_DIV_15     13

#define SET_CRT_VIDEO_CLK( vformat, freq_mul )       vclk_set_crt_video(vformat, freq_mul, 1)
#define SET_CRT_VIDEO_CLK_V2( vformat, freq_mul )    vclk_set_crt_video_v2(vformat, freq_mul, 1)

void clocks_set_vid_clk_div(uint32_t div_sel);
void vclk_set_crt_video(uint32_t vformat, uint32_t upsample, uint32_t pll_sel);
uint32_t set_hdmi_dpll_lvds_vx1(uint32_t pi_freq, uint32_t pixel_freq, uint32_t lvds_dual_port,uint32_t ana_div57_sel);
void vout_set_lvds_clk(uint32_t vformat, uint32_t lvds_ports);

uint32_t vout_set_edp_phy_pll(uint32_t freq); //for example 594M = vout_set_edp_phy_pll(594000);
uint32_t vout_set_edp_pix_pll(uint32_t freq); //for example 148.5M = vout_set_edp_pix_pll(148500);
uint32_t vout_set_pll_setting(uint32_t setting_sel);
#endif /* MNT_VOUT_H */
