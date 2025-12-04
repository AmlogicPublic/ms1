#ifndef __CLOCK_TREE_V2_H__
#define __CLOCK_TREE_V2_H__

#include <stdint.h>
#include "ms1/register.h"

// Clock Tree Configuration Interface - Version 2
// Based on MS1 SOC Clock Tree Specification
// Design principle: Each branch/clock is treated as independent entity

//=============================================================================
// Unified Clock ID Definitions - All clocks and sources treated uniformly
//=============================================================================
typedef enum {
    // Fixed frequency sources (treated as "virtual clocks")
    CLK_SRC_XTAL = 0,           // 24MHz crystal (fixed)
    CLK_SRC_FCLK_DIV2,          // fclk_div2 (1000MHz, fixed)
    CLK_SRC_FCLK_DIV3,          // fclk_div3 (666MHz, fixed)
    CLK_SRC_FCLK_DIV4,          // fclk_div4 (500MHz, fixed)
    CLK_SRC_FCLK_DIV5,          // fclk_div5 (400MHz, fixed)
    CLK_SRC_FCLK_DIV7,          // fclk_div7 (285MHz, fixed)
    CLK_SRC_FCLK_DIV2P5,        // fclk_div2p5 (800MHz, fixed)
    CLK_SRC_RTC_CLK,            // cts_rtc_clk (24MHz, fixed)
    CLK_SRC_RESERVED,           // Reserved/unused source
    
    // AO Clock Tree - Composite clocks (selection only)
    CLK_AO_SYS_CLK = 100,       // cts_sys_clk (composite, max 166MHz)
    CLK_AO_AHB_CLK,             // cts_ahb_clk (composite, max 400MHz)  
    CLK_AO_RTC_CLK,             // cts_rtc_clk (composite, max 24MHz)
    
    // AO Clock Tree - Branch clocks (gate + div + sel)
    CLK_AO_SYS_CLK_A,           // cts_sys_clk_a (max 166MHz)
    CLK_AO_SYS_CLK_B,           // cts_sys_clk_b (max 166MHz)
    CLK_AO_AHB_CLK_A,           // cts_ahb_clk_a (max 400MHz)
    CLK_AO_AHB_CLK_B,           // cts_ahb_clk_b (max 400MHz)
    
    // AO Clock Tree - RTC related clocks
    CLK_AO_RTC_MUX0A,           // cts_rtc_clk_mux0a (selection only, 24MHz)
    CLK_AO_RTC_MUX0B,           // cts_rtc_clk_mux0b (selection only, 24MHz)  
    CLK_AO_RTC_BYOSCIN,         // rtc_clk_byoscin (fixed, 24MHz)
    
    // AO Clock Tree - Fixed clocks
    CLK_AO_OSCIN_CLK,           // cts_oscin_clk (fixed, 24MHz)
    
    // AO Clock Tree - Peripheral clocks
    CLK_AO_CECB_CLK,            // cts_cecb_clk (selection only, 24MHz)
    CLK_AO_CECB_DIV,            // cecb_clk_div (fixed, 24MHz)
    CLK_AO_PWM_A,               // cts_pwm_a_clk (gate + div + sel, max 333MHz)
    CLK_AO_PWM_B,               // cts_pwm_b_clk (gate + div + sel, max 333MHz)
    CLK_AO_PWM_C,               // cts_pwm_c_clk (gate + div + sel, max 333MHz)
    CLK_AO_PWM_D,               // cts_pwm_d_clk (gate + div + sel, max 333MHz)
    CLK_AO_TS,                  // cts_ts_clk (gate + div, max 24MHz)
    CLK_AO_HDMIRX_5M,           // cts_hdmirx_5m_clk (gate + div + sel, max 100MHz)
    CLK_AO_HDMIRX_2M,           // cts_hdmirx_2m_clk (gate + div + sel, max 100MHz)
    CLK_AO_HDMIRX_METER,        // cts_hdmirx_meter_clk (gate + div + sel, max 100MHz)
    CLK_AO_APB2,                // cts_apb2_clk (gate + div + sel, max 200MHz)
    
    // EE Clock Tree - Composite clocks (selection only)
    CLK_EE_AXI_CLK,             // cts_axi_clk (composite, max 500MHz)
    
    // EE Clock Tree - Branch clocks  
    CLK_EE_AXI_CLK_A,           // cts_axi_clk_a (max 500MHz)
    CLK_EE_AXI_CLK_B,           // cts_axi_clk_b (max 500MHz)
    
    // EE Clock Tree - Peripheral clocks
    CLK_EE_PWM_E,               // cts_pwm_e_clk (gate + div + sel, max 333MHz)
    CLK_EE_PWM_F,               // cts_pwm_f_clk (gate + div + sel, max 333MHz)
    CLK_EE_PWM_G,               // cts_pwm_g_clk (gate + div + sel, max 333MHz)
    CLK_EE_PWM_H,               // cts_pwm_h_clk (gate + div + sel, max 333MHz)
    CLK_EE_SPISG_0,             // cts_spisg_0_clk (gate + div + sel, max 500MHz)
    CLK_EE_SAR_ADC,             // cts_sar_adc_clk (gate + div + sel, max 200MHz)
    CLK_EE_TS,                  // cts_ts_clk (gate + div, max 24MHz) 
    CLK_EE_BCON,                // cts_bcon_clk (gate + div + sel, max 333MHz)
    CLK_EE_TX_PIX,              // cts_tx_pix_clk / cts_vdout_clk (gate + div + sel, max 870MHz)
    CLK_EE_VP_OPT,              // cts_vp_opt_clk (gate + div + sel, max 870MHz)
    CLK_EE_TX_DET,              // cts_tx_det_clk (gate + div + sel, max 400MHz)
    CLK_EE_APB2,                // cts_apb2_clk (gate + div + sel, max 200MHz)
    CLK_EE_HDMIRX_5M,           // cts_hdmirx_5m_clk (EE domain) (gate + div + sel, max 100MHz)
    CLK_EE_HDMIRX_CFG,          // cts_hdmirx_cfg_clk (gate + div + sel, max 200MHz)
    CLK_EE_HDMIRX_HDCP2X_E,     // cts_hdmirx_hdcp2x_eclk (gate + div + sel, max 100MHz)
    CLK_EE_HDMIRX_AUD_PLL,      // cts_hdmirx_aud_pll_clk (gate + div + sel, max 200MHz)
    CLK_EE_HDMIRX_ACR_REF,      // cts_hdmirx_acr_ref_clk (gate + div + sel, max 500MHz)
    
    CLK_ID_MAX
} clock_id_t;

//=============================================================================
// Clock Configuration Structure - Generic for all clocks
//=============================================================================
typedef struct {
    uint32_t reg_addr;          // Register address (0 = fixed freq source)
    uint8_t gate_bit;           // Gate control bit position (0xFF = no gate)
    uint8_t div_start_bit;      // Divider start bit position (0xFF = no divider)
    uint8_t div_width;          // Divider bit width
    uint8_t sel_start_bit;      // Source selection start bit (0xFF = no selection)
    uint8_t sel_width;          // Source selection bit width  
    uint32_t max_freq_mhz;      // Max frequency (for fixed sources, also the actual freq)
    clock_id_t sources[8];      // Available sources - all are clock IDs now
    clock_id_t dependencies[4]; // Upper layer clocks that must be configured first
} clock_config_t;

//=============================================================================
// Simple and Robust Clock API - ONE function for everything
//=============================================================================

// Unified Clock Interface - Configure ANY clock with target, source, and frequency
// target_id: which clock to configure
// source_id: can be CLK_SRC_* (frequency source) or CLK_* (clock source)
// freq_mhz: target frequency in MHz, 0 = disable clock
int clock_set_frequency(clock_id_t target_id, uint32_t source_id, uint32_t freq_mhz);

// Dependency checking - internal use
int clock_check_dependencies(clock_id_t clk_id);

// Query functions
uint32_t clock_get_frequency(clock_id_t clk_id);
int clock_is_enabled(clock_id_t clk_id);
const clock_config_t* clock_get_config(clock_id_t clk_id);
const char* clock_get_name(clock_id_t clk_id);


#endif // __CLOCK_TREE_V2_H__
