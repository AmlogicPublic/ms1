/*******************************************************************
 * 
 *  Copyright C 2008 by Amlogic, Inc. All Rights Reserved.
 *
 *  Description: 
 *
 *  Author:  Amlogic 
 *  Created: 07/09/2025
 *
 *******************************************************************/
#ifndef MNT_VOUT_H
#define MNT_VOUT_H

#include "Drivers/vpu/cfg_mnt_pps.h"

#define VOUT_APB_BASE_ADDR 0xfe3c0000 
typedef enum {
    MNT_LVDS_1920x1080_300Hz_4P   = 0,
    MNT_LVDS_1920x1080_240Hz_4P   = 1,
    MNT_LVDS_1920x1080_220Hz_4P   = 2,
    MNT_LVDS_1920x1080_200Hz_4P   = 3,
    MNT_LVDS_1920x1080_120Hz_2P   = 4,
    MNT_LVDS_1920x1080_60Hz_1P    = 5,
    MNT_eDP_2560x1440_220Hz       = 6,
    MNT_eDP_2560x1440_200Hz       = 7,
    MNT_eDP_2560x1440_180Hz       = 8,
} mnt_vout_type_t;  

typedef struct {
    volatile uint32_t pack_cntl;
    //bit       four_port;      // pack_cntl1[15+16];
    //bit       two_port_2pair; // pack_cntl1[14+16];
    //bit       one_port_4pair; // pack_cntl1[13+16];
    //bit[12:0] rsvd;

    //bit [1:0] b_select;     // pack_cntl[15:14];
    //bit [1:0] g_select;     // pack_cntl[13:12];
    //bit [1:0] r_select;     // pack_cntl[11:10]; // 0:R, 1:G, 2:B, 3:0
    //bit [1:0] bit_size;     // pack_cntl[9:8]; // 0:10bits, 1:8bits, 2:6bits, 3:4bits.		
    //bit       use_tcon_ctrl;// pack_cntl[7]; // hs, vs, de controlled by tcon.
    //bit       dual_port;	// pack_cntl[6]; // dual port mapping.
    //bit       bitwise_neg;  // pack_cntl[5]; // pn swap.
    //bit       lsb_first; 	// pack_cntl[4]; // 0: MSB first, 1: LSB first.
    //bit       resv; 		// pack_cntl[3]; // reserve bit value.
    //bit       odd_even_swp; // pack_cntl[2];	// 0: normal, 1: swap.
    //bit [1:0] repack;       // pack_cntl[1:0]; // 0: normal, 1,2: repack.
} aml_lvds_cfg_t;

void vout_1920x32_free_run_cmpr_lvds(aml_lvds_cfg_t lvds_cfg);
void vout_2560x1440_cmpr_od_test();
void vout_1080p_to_qhd_lvds(aml_lvds_cfg_t lvds_cfg,uint32_t scaler_in_h,uint32_t scaler_in_v,uint32_t scaler_o_h,uint32_t scaler_o_v,uint32_t h_start,uint32_t v_start,uint32_t h_tot_o,uint32_t v_tot_o,uint32_t in_patt_en);
void vout_in_patt_1080p_to_qhd_lvds(aml_lvds_cfg_t lvds_cfg,uint32_t scaler_in_h,uint32_t scaler_in_v,uint32_t scaler_o_h,uint32_t scaler_o_v,uint32_t h_start,uint32_t v_start,uint32_t h_tot_o,uint32_t v_tot_o,uint32_t in_patt_en);

void vout_in_patt_vpu_1080p_to_qhd_lvds(aml_lvds_cfg_t lvds_cfg,uint32_t htot_in,uint32_t vtot_in,uint32_t scaler_in_h,uint32_t scaler_in_v,uint32_t scaler_o_h,uint32_t scaler_o_v,uint32_t h_start,uint32_t v_start,uint32_t h_tot_o,uint32_t v_tot_o,uint32_t in_patt_en);

void vout_in_patt_vpu_1080p_to_qhd_lvds_padding(aml_lvds_cfg_t lvds_cfg,uint32_t htot_in,uint32_t vtot_in,uint32_t pps_in_h,uint32_t pps_in_v,uint32_t scaler_o_h,uint32_t scaler_o_v,uint32_t pps_o_h,uint32_t pps_o_v,uint32_t h_start,uint32_t v_start,uint32_t h_tot_o,uint32_t v_tot_o,uint32_t in_patt_en);

void vout_video_config_phase(aml_lvds_cfg_t lvds_cfg,uint32_t scaler_in_h,uint32_t scaler_in_v,uint32_t scaler_o_h,uint32_t scaler_o_v,uint32_t h_start,uint32_t v_start,uint32_t h_tot_o,uint32_t v_tot_o,uint32_t in_patt_en,uint32_t pll_freq);
void vout_rdma_test_reg(int test_num,uint32_t exp_data);

void vout_retimer_code(uint32_t scaler_in_h,uint32_t scaler_in_v,uint32_t h_tot_i,uint32_t v_tot_i,uint32_t scaler_o_h,uint32_t scaler_o_v,uint32_t h_tot_o,uint32_t v_tot_o,uint32_t in_patt_en,uint32_t force_mode,uint32_t adjust_mode,uint32_t pix_freq);

void vout_retimer_code_pps_padding(uint32_t scaler_in_h,uint32_t scaler_in_v,uint32_t h_tot_i,uint32_t v_tot_i,uint32_t scaler_o_h,uint32_t scaler_o_v,uint32_t pps_o_h,uint32_t pps_o_v,uint32_t h_tot_o,uint32_t v_tot_o,uint32_t in_patt_en,uint32_t force_mode,uint32_t adjust_mode,uint32_t pix_freq);

void vout_rdma_lut_test();
void vout_test_reg();
void vout_test_ram();

#endif /* MNT_VOUT_H */
