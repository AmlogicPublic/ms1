#include "Drivers/clock_tree/clock_tree.h"
#include <stddef.h>
#include <stdio.h>

//=============================================================================
// Clock Configuration Table - Each clock treated independently
//=============================================================================

static const clock_config_t clock_configs[CLK_ID_MAX] = {
    // Fixed frequency sources (reg_addr=0, max_freq_mhz = actual frequency)
    [CLK_SRC_XTAL]      = { .reg_addr = 0, .max_freq_mhz = 24,   .sources = {CLK_ID_MAX} },
    [CLK_SRC_FCLK_DIV2] = { .reg_addr = 0, .max_freq_mhz = 1000, .sources = {CLK_ID_MAX} },
    [CLK_SRC_FCLK_DIV3] = { .reg_addr = 0, .max_freq_mhz = 666,  .sources = {CLK_ID_MAX} },
    [CLK_SRC_FCLK_DIV4] = { .reg_addr = 0, .max_freq_mhz = 500,  .sources = {CLK_ID_MAX} },
    [CLK_SRC_FCLK_DIV5] = { .reg_addr = 0, .max_freq_mhz = 400,  .sources = {CLK_ID_MAX} },
    [CLK_SRC_FCLK_DIV7] = { .reg_addr = 0, .max_freq_mhz = 285,  .sources = {CLK_ID_MAX} },
    [CLK_SRC_FCLK_DIV2P5]={ .reg_addr = 0, .max_freq_mhz = 800,  .sources = {CLK_ID_MAX} },
    [CLK_SRC_RTC_CLK]   = { .reg_addr = 0, .max_freq_mhz = 24,   .sources = {CLK_ID_MAX} },
    
    // AO Clock Tree - Composite clocks (selection only, no gate/div)
    [CLK_AO_SYS_CLK] = {
        .reg_addr = AO_CLKCTRL_SYS_CLK_CTRL0,
        .gate_bit = 0xFF,           // No gate
        .div_start_bit = 0xFF,      // No divider
        .div_width = 0,
        .sel_start_bit = 15,        // Selection bit [15]
        .sel_width = 1,
        .max_freq_mhz = 166,
        .sources = {CLK_AO_SYS_CLK_A, CLK_AO_SYS_CLK_B},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_AO_AHB_CLK] = {
        .reg_addr = AO_CLKCTRL_AHB_CLK_CTRL0,
        .gate_bit = 0xFF,           // No gate
        .div_start_bit = 0xFF,      // No divider
        .div_width = 0,
        .sel_start_bit = 15,        // Selection bit [15]
        .sel_width = 1,
        .max_freq_mhz = 400,
        .sources = {CLK_AO_AHB_CLK_A, CLK_AO_AHB_CLK_B},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_AO_RTC_CLK] = {
        .reg_addr = AO_CLKCTRL_RTC_CTRL,
        .gate_bit = 0xFF,           // No gate
        .div_start_bit = 0xFF,      // No divider
        .div_width = 0,
        .sel_start_bit = 1,         // Selection bit [1]
        .sel_width = 1,
        .max_freq_mhz = 24,
        .sources = {CLK_AO_RTC_MUX0A, CLK_AO_RTC_MUX0B},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    // AO Clock Tree - Branch clocks (gate + div + sel)
    [CLK_AO_SYS_CLK_A] = {
        .reg_addr = AO_CLKCTRL_SYS_CLK_CTRL0,
        .gate_bit = 13,             // Gate bit [13]
        .div_start_bit = 0,         // Divider bits [9:0]
        .div_width = 10,
        .sel_start_bit = 10,        // Source selection bits [12:10]
        .sel_width = 3,
        .max_freq_mhz = 166,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV2, CLK_SRC_FCLK_DIV3, CLK_SRC_FCLK_DIV4,
                   CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV7, CLK_SRC_RESERVED, CLK_SRC_RTC_CLK},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_AO_SYS_CLK_B] = {
        .reg_addr = AO_CLKCTRL_SYS_CLK_CTRL0,
        .gate_bit = 29,             // Gate bit [29]
        .div_start_bit = 16,        // Divider bits [25:16] 
        .div_width = 10,
        .sel_start_bit = 26,        // Source selection bits [28:26]
        .sel_width = 3,
        .max_freq_mhz = 166,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV2, CLK_SRC_FCLK_DIV3, CLK_SRC_FCLK_DIV4,
                   CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV7, CLK_SRC_RESERVED, CLK_SRC_RTC_CLK},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_AO_AHB_CLK_A] = {
        .reg_addr = AO_CLKCTRL_AHB_CLK_CTRL0,
        .gate_bit = 13,             // Gate bit [13]
        .div_start_bit = 0,         // Divider bits [9:0]
        .div_width = 10,
        .sel_start_bit = 10,        // Source selection bits [12:10]
        .sel_width = 3,
        .max_freq_mhz = 400,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV2, CLK_SRC_FCLK_DIV3, CLK_SRC_FCLK_DIV4,
                   CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV7, CLK_SRC_FCLK_DIV2P5, CLK_SRC_RTC_CLK},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_AO_AHB_CLK_B] = {
        .reg_addr = AO_CLKCTRL_AHB_CLK_CTRL0,
        .gate_bit = 29,             // Gate bit [29]
        .div_start_bit = 16,        // Divider bits [25:16]
        .div_width = 10,
        .sel_start_bit = 26,        // Source selection bits [28:26]
        .sel_width = 3,
        .max_freq_mhz = 400,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV2, CLK_SRC_FCLK_DIV3, CLK_SRC_FCLK_DIV4,
                   CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV7, CLK_SRC_FCLK_DIV2P5, CLK_SRC_RTC_CLK},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    // AO Clock Tree - RTC related clocks
    [CLK_AO_RTC_MUX0A] = {
        .reg_addr = AO_CLKCTRL_RTC_CTRL,
        .gate_bit = 0xFF,           // No gate
        .div_start_bit = 0xFF,      // No divider
        .div_width = 0,
        .sel_start_bit = 0,         // Selection bit [0]
        .sel_width = 1,
        .max_freq_mhz = 24,
        .sources = {CLK_SRC_XTAL, CLK_AO_RTC_BYOSCIN},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_AO_RTC_MUX0B] = {
        .reg_addr = AO_CLKCTRL_RTC_CTRL,
        .gate_bit = 0xFF,           // No gate
        .div_start_bit = 0xFF,      // No divider
        .div_width = 0,
        .sel_start_bit = 0,         // Selection bit [0] (same as mux0a)
        .sel_width = 1,
        .max_freq_mhz = 24,
        .sources = {CLK_SRC_RESERVED, CLK_SRC_XTAL},  // rtc_clk_fr_pad (external pad input), xtal
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_AO_RTC_BYOSCIN] = {
        .reg_addr = 0,              // Fixed clock
        .gate_bit = 0xFF,           // Always enabled
        .div_start_bit = 0xFF,      // No divider
        .div_width = 0,
        .sel_start_bit = 0xFF,      // No selection
        .sel_width = 0,
        .max_freq_mhz = 24,
        .sources = {CLK_SRC_XTAL},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    // AO Clock Tree - Fixed clocks
    [CLK_AO_OSCIN_CLK] = {
        .reg_addr = 0,              // Fixed clock
        .gate_bit = 0xFF,           // Always enabled
        .div_start_bit = 0xFF,      // No divider
        .div_width = 0,
        .sel_start_bit = 0xFF,      // No selection
        .sel_width = 0,
        .max_freq_mhz = 24,
        .sources = {CLK_SRC_XTAL},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    // AO Clock Tree - Peripheral clocks
    [CLK_AO_CECB_CLK] = {
        .reg_addr = AO_CLKCTRL_CECB_CTRL1,
        .gate_bit = 0xFF,           // No gate
        .div_start_bit = 0xFF,      // No divider 
        .div_width = 0,
        .sel_start_bit = 31,        // Selection bit [31]
        .sel_width = 1,
        .max_freq_mhz = 24,
        .sources = {CLK_AO_CECB_DIV, CLK_AO_RTC_CLK},
        .dependencies = {CLK_AO_RTC_CLK, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_AO_CECB_DIV] = {
        .reg_addr = 0,              // Fixed clock
        .gate_bit = 0xFF,           // Always enabled
        .div_start_bit = 0xFF,      // No divider
        .div_width = 0,
        .sel_start_bit = 0xFF,      // No selection
        .sel_width = 0,
        .max_freq_mhz = 24,
        .sources = {CLK_SRC_XTAL},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_AO_PWM_A] = {
        .reg_addr = AO_CLKCTRL_PWM_CLK_AB_CTRL,
        .gate_bit = 8,              // Gate bit [8]
        .div_start_bit = 0,         // Divider bits [7:0]
        .div_width = 8,
        .sel_start_bit = 9,         // Source selection bits [10:9]
        .sel_width = 2,
        .max_freq_mhz = 333,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_AO_PWM_B] = {
        .reg_addr = AO_CLKCTRL_PWM_CLK_AB_CTRL,
        .gate_bit = 24,             // Gate bit [24]
        .div_start_bit = 16,        // Divider bits [23:16]
        .div_width = 8,
        .sel_start_bit = 25,        // Source selection bits [26:25]
        .sel_width = 2,
        .max_freq_mhz = 333,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_AO_PWM_C] = {
        .reg_addr = AO_CLKCTRL_PWM_CLK_CD_CTRL,
        .gate_bit = 8,              // Gate bit [8]
        .div_start_bit = 0,         // Divider bits [7:0]
        .div_width = 8,
        .sel_start_bit = 9,         // Source selection bits [10:9]
        .sel_width = 2,
        .max_freq_mhz = 333,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_AO_PWM_D] = {
        .reg_addr = AO_CLKCTRL_PWM_CLK_CD_CTRL,
        .gate_bit = 24,             // Gate bit [24]
        .div_start_bit = 16,        // Divider bits [23:16]
        .div_width = 8,
        .sel_start_bit = 25,        // Source selection bits [26:25]
        .sel_width = 2,
        .max_freq_mhz = 333,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_AO_TS] = {
        .reg_addr = AO_CLKCTRL_TS_CLK_CTRL,
        .gate_bit = 8,              // Gate bit [8]
        .div_start_bit = 0,         // Divider bits [7:0]
        .div_width = 8,
        .sel_start_bit = 0xFF,      // No selection
        .sel_width = 0,
        .max_freq_mhz = 24,
        .sources = {CLK_SRC_XTAL},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_AO_HDMIRX_5M] = {
        .reg_addr = AO_CLKCTRL_HRX_CLK_CTRL0,
        .gate_bit = 8,              // Gate bit [8]
        .div_start_bit = 0,         // Divider bits [6:0]
        .div_width = 7,
        .sel_start_bit = 9,         // Source selection bits [10:9]
        .sel_width = 2,
        .max_freq_mhz = 100,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3, CLK_SRC_FCLK_DIV5},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_AO_HDMIRX_2M] = {
        .reg_addr = AO_CLKCTRL_HRX_CLK_CTRL0,
        .gate_bit = 24,             // Gate bit [24]
        .div_start_bit = 16,        // Divider bits [22:16]
        .div_width = 7,
        .sel_start_bit = 25,        // Source selection bits [26:25]
        .sel_width = 2,
        .max_freq_mhz = 100,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3, CLK_SRC_FCLK_DIV5},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_AO_HDMIRX_METER] = {
        .reg_addr = AO_CLKCTRL_HRX_CLK_CTRL1,
        .gate_bit = 8,              // Gate bit [8]
        .div_start_bit = 0,         // Divider bits [6:0]
        .div_width = 7,
        .sel_start_bit = 9,         // Source selection bits [10:9]
        .sel_width = 2,
        .max_freq_mhz = 100,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3, CLK_SRC_FCLK_DIV5},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_AO_APB2] = {
        .reg_addr = AO_CLKCTRL_HRX_CLK_CTRL1,
        .gate_bit = 24,             // Gate bit [24]
        .div_start_bit = 16,        // Divider bits [22:16]
        .div_width = 7,
        .sel_start_bit = 25,        // Source selection bits [26:25]
        .sel_width = 2,
        .max_freq_mhz = 200,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3, CLK_SRC_FCLK_DIV5},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    // EE Clock Tree - Composite clocks (selection only)
    [CLK_EE_AXI_CLK] = {
        .reg_addr = EE_CLKCTRL_AXI_CLK_CTRL0,
        .gate_bit = 0xFF,           // No gate
        .div_start_bit = 0xFF,      // No divider
        .div_width = 0,
        .sel_start_bit = 15,        // Selection bit [15]
        .sel_width = 1,
        .max_freq_mhz = 500,
        .sources = {CLK_EE_AXI_CLK_A, CLK_EE_AXI_CLK_B},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    // EE Clock Tree - Branch clocks
    [CLK_EE_AXI_CLK_A] = {
        .reg_addr = EE_CLKCTRL_AXI_CLK_CTRL0,
        .gate_bit = 13,             // Gate bit [13]
        .div_start_bit = 0,         // Divider bits [9:0]
        .div_width = 10,
        .sel_start_bit = 10,        // Source selection bits [12:10]
        .sel_width = 3,
        .max_freq_mhz = 500,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV2, CLK_SRC_FCLK_DIV3, CLK_SRC_FCLK_DIV4,
                   CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV7, CLK_SRC_FCLK_DIV2P5, CLK_SRC_RESERVED},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_EE_AXI_CLK_B] = {
        .reg_addr = EE_CLKCTRL_AXI_CLK_CTRL0,
        .gate_bit = 29,             // Gate bit [29]
        .div_start_bit = 16,        // Divider bits [25:16]
        .div_width = 10,
        .sel_start_bit = 26,        // Source selection bits [28:26]
        .sel_width = 3,
        .max_freq_mhz = 500,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV2, CLK_SRC_FCLK_DIV3, CLK_SRC_FCLK_DIV4,
                   CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV7, CLK_SRC_FCLK_DIV2P5, CLK_SRC_RESERVED},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    // EE Clock Tree - Peripheral clocks
    [CLK_EE_PWM_E] = {
        .reg_addr = EE_CLKCTRL_PWM_CLK_EF_CTRL,
        .gate_bit = 8,              // Gate bit [8]
        .div_start_bit = 0,         // Divider bits [7:0]
        .div_width = 8,
        .sel_start_bit = 9,         // Source selection bits [10:9]
        .sel_width = 2,
        .max_freq_mhz = 333,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_EE_PWM_F] = {
        .reg_addr = EE_CLKCTRL_PWM_CLK_EF_CTRL,
        .gate_bit = 24,             // Gate bit [24]
        .div_start_bit = 16,        // Divider bits [23:16]
        .div_width = 8,
        .sel_start_bit = 25,        // Source selection bits [26:25]
        .sel_width = 2,
        .max_freq_mhz = 333,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_EE_PWM_G] = {
        .reg_addr = EE_CLKCTRL_PWM_CLK_GH_CTRL,
        .gate_bit = 8,              // Gate bit [8]
        .div_start_bit = 0,         // Divider bits [7:0]
        .div_width = 8,
        .sel_start_bit = 9,         // Source selection bits [10:9]
        .sel_width = 2,
        .max_freq_mhz = 333,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_EE_PWM_H] = {
        .reg_addr = EE_CLKCTRL_PWM_CLK_GH_CTRL,
        .gate_bit = 24,             // Gate bit [24]
        .div_start_bit = 16,        // Divider bits [23:16]
        .div_width = 8,
        .sel_start_bit = 25,        // Source selection bits [26:25]
        .sel_width = 2,
        .max_freq_mhz = 333,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_EE_SPISG_0] = {
        .reg_addr = EE_CLKCTRL_SPISG_CLK_CTRL,
        .gate_bit = 6,              // Gate bit [6]
        .div_start_bit = 0,         // Divider bits [5:0]
        .div_width = 6,
        .sel_start_bit = 7,         // Source selection bits [9:7]
        .sel_width = 3,
        .max_freq_mhz = 500,
        .sources = {CLK_SRC_XTAL, CLK_AO_SYS_CLK, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3,
                   CLK_SRC_FCLK_DIV2, CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV7, CLK_SRC_RESERVED},
        .dependencies = {CLK_AO_SYS_CLK, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_EE_SAR_ADC] = {
        .reg_addr = EE_CLKCTRL_SAR_CLK_CTRL0,
        .gate_bit = 8,              // Gate bit [8]
        .div_start_bit = 0,         // Divider bits [7:0]
        .div_width = 8,
        .sel_start_bit = 9,         // Source selection bits [10:9]
        .sel_width = 2,
        .max_freq_mhz = 200,
        .sources = {CLK_SRC_XTAL, CLK_AO_SYS_CLK, CLK_SRC_FCLK_DIV5, CLK_SRC_RESERVED},
        .dependencies = {CLK_AO_SYS_CLK, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_EE_TS] = {
        .reg_addr = EE_CLKCTRL_TS_CLK_CTRL,
        .gate_bit = 8,              // Gate bit [8]
        .div_start_bit = 0,         // Divider bits [7:0]
        .div_width = 8,
        .sel_start_bit = 0xFF,      // No selection
        .sel_width = 0,
        .max_freq_mhz = 24,
        .sources = {CLK_SRC_XTAL},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_EE_BCON] = {
        .reg_addr = EE_CLKCTRL_BCON_CLK_CTRL,
        .gate_bit = 6,              // Gate bit [6]
        .div_start_bit = 0,         // Divider bits [5:0]
        .div_width = 6,
        .sel_start_bit = 7,         // Source selection bits [9:7]
        .sel_width = 3,
        .max_freq_mhz = 333,
        .sources = {CLK_SRC_XTAL, CLK_AO_SYS_CLK, CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV4,
                   CLK_SRC_RESERVED, CLK_SRC_RESERVED, CLK_SRC_RESERVED, CLK_SRC_RESERVED},  // tx_pll_div_clk not handled
        .dependencies = {CLK_AO_SYS_CLK, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_EE_TX_PIX] = {
        .reg_addr = EE_CLKCTRL_VDOUT_CLK_CTRL,
        .gate_bit = 6,              // Gate bit [6]
        .div_start_bit = 0,         // Divider bits [5:0]
        .div_width = 6,
        .sel_start_bit = 7,         // Source selection bits [9:7]
        .sel_width = 3,
        .max_freq_mhz = 870,
        .sources = {CLK_SRC_XTAL, CLK_SRC_RESERVED, CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV4,
                   CLK_SRC_FCLK_DIV3, CLK_SRC_FCLK_DIV2, CLK_SRC_RESERVED, CLK_SRC_RESERVED},  // Special PLL sources not handled
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_EE_VP_OPT] = {
        .reg_addr = EE_CLKCTRL_VP_OPT_CLK_CTRL,
        .gate_bit = 6,              // Gate bit [6]
        .div_start_bit = 0,         // Divider bits [5:0]
        .div_width = 6,
        .sel_start_bit = 7,         // Source selection bits [9:7]
        .sel_width = 3,
        .max_freq_mhz = 870,
        .sources = {CLK_SRC_XTAL, CLK_SRC_RESERVED, CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV4,
                   CLK_SRC_FCLK_DIV3, CLK_SRC_RESERVED, CLK_SRC_RESERVED, CLK_SRC_RESERVED},  // Special PLL sources not handled
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_EE_TX_DET] = {
        .reg_addr = EE_CLKCTRL_TX_DET_CLK_CTRL,
        .gate_bit = 8,              // Gate bit [8]
        .div_start_bit = 0,         // Divider bits [6:0]
        .div_width = 7,
        .sel_start_bit = 9,         // Source selection bits [11:9]
        .sel_width = 3,
        .max_freq_mhz = 400,
        .sources = {CLK_SRC_XTAL, CLK_AO_SYS_CLK, CLK_SRC_FCLK_DIV5, CLK_SRC_FCLK_DIV4},
        .dependencies = {CLK_AO_SYS_CLK, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_EE_APB2] = {
        .reg_addr = EE_CLKCTRL_APB2_CLK_CTRL,
        .gate_bit = 8,              // Gate bit [8]
        .div_start_bit = 0,         // Divider bits [6:0]
        .div_width = 7,
        .sel_start_bit = 9,         // Source selection bits [11:9]
        .sel_width = 3,
        .max_freq_mhz = 200,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3, CLK_SRC_FCLK_DIV5},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_EE_HDMIRX_5M] = {
        .reg_addr = EE_CLKCTRL_HRX_CLK_CTRL0,
        .gate_bit = 8,              // Gate bit [8]
        .div_start_bit = 0,         // Divider bits [6:0]
        .div_width = 7,
        .sel_start_bit = 9,         // Source selection bits [10:9]
        .sel_width = 2,
        .max_freq_mhz = 100,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3, CLK_SRC_FCLK_DIV5},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_EE_HDMIRX_CFG] = {
        .reg_addr = EE_CLKCTRL_HRX_CLK_CTRL1,
        .gate_bit = 8,              // Gate bit [8]
        .div_start_bit = 0,         // Divider bits [6:0]
        .div_width = 7,
        .sel_start_bit = 9,         // Source selection bits [10:9]
        .sel_width = 2,
        .max_freq_mhz = 200,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3, CLK_SRC_FCLK_DIV5},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_EE_HDMIRX_HDCP2X_E] = {
        .reg_addr = EE_CLKCTRL_HRX_CLK_CTRL1,
        .gate_bit = 24,             // Gate bit [24]
        .div_start_bit = 16,        // Divider bits [22:16]
        .div_width = 7,
        .sel_start_bit = 25,        // Source selection bits [26:25]
        .sel_width = 2,
        .max_freq_mhz = 100,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3, CLK_SRC_FCLK_DIV5},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_EE_HDMIRX_AUD_PLL] = {
        .reg_addr = EE_CLKCTRL_HRX_CLK_CTRL2,
        .gate_bit = 8,              // Gate bit [8]
        .div_start_bit = 0,         // Divider bits [6:0]
        .div_width = 7,
        .sel_start_bit = 9,         // Source selection bits [10:9]
        .sel_width = 2,
        .max_freq_mhz = 200,
        .sources = {CLK_SRC_RESERVED, CLK_SRC_RESERVED, CLK_SRC_RESERVED, CLK_SRC_RESERVED},  // hdmirx_aud_pll_clk source not handled
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    },
    
    [CLK_EE_HDMIRX_ACR_REF] = {
        .reg_addr = EE_CLKCTRL_HRX_CLK_CTRL2,
        .gate_bit = 24,             // Gate bit [24]
        .div_start_bit = 16,        // Divider bits [22:16]
        .div_width = 7,
        .sel_start_bit = 25,        // Source selection bits [26:25]
        .sel_width = 2,
        .max_freq_mhz = 500,
        .sources = {CLK_SRC_XTAL, CLK_SRC_FCLK_DIV4, CLK_SRC_FCLK_DIV3, CLK_SRC_FCLK_DIV5},
        .dependencies = {CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX, CLK_ID_MAX}
    }
};

// Clock names for debugging
static const char* clock_names[CLK_ID_MAX] = {
    [CLK_AO_SYS_CLK] = "cts_sys_clk",
    [CLK_AO_AHB_CLK] = "cts_ahb_clk",
    [CLK_AO_RTC_CLK] = "cts_rtc_clk",
    [CLK_AO_SYS_CLK_A] = "cts_sys_clk_a",
    [CLK_AO_SYS_CLK_B] = "cts_sys_clk_b", 
    [CLK_AO_AHB_CLK_A] = "cts_ahb_clk_a",
    [CLK_AO_AHB_CLK_B] = "cts_ahb_clk_b",
    [CLK_AO_RTC_MUX0A] = "cts_rtc_clk_mux0a",
    [CLK_AO_RTC_MUX0B] = "cts_rtc_clk_mux0b",
    [CLK_AO_RTC_BYOSCIN] = "rtc_clk_byoscin",
    [CLK_AO_OSCIN_CLK] = "cts_oscin_clk",
    [CLK_AO_CECB_CLK] = "cts_cecb_clk",
    [CLK_AO_CECB_DIV] = "cecb_clk_div",
    [CLK_AO_PWM_A] = "cts_pwm_a_clk",
    [CLK_AO_PWM_B] = "cts_pwm_b_clk",
    [CLK_AO_PWM_C] = "cts_pwm_c_clk", 
    [CLK_AO_PWM_D] = "cts_pwm_d_clk",
    [CLK_AO_TS] = "cts_ts_clk",
    [CLK_AO_HDMIRX_5M] = "cts_hdmirx_5m_clk",
    [CLK_AO_HDMIRX_2M] = "cts_hdmirx_2m_clk",
    [CLK_AO_HDMIRX_METER] = "cts_hdmirx_meter_clk",
    [CLK_AO_APB2] = "cts_apb2_clk",
    [CLK_EE_AXI_CLK] = "cts_axi_clk",
    [CLK_EE_AXI_CLK_A] = "cts_axi_clk_a",
    [CLK_EE_AXI_CLK_B] = "cts_axi_clk_b",
    [CLK_EE_PWM_E] = "cts_pwm_e_clk",
    [CLK_EE_PWM_F] = "cts_pwm_f_clk",
    [CLK_EE_PWM_G] = "cts_pwm_g_clk",
    [CLK_EE_PWM_H] = "cts_pwm_h_clk",
    [CLK_EE_SPISG_0] = "cts_spisg_0_clk",
    [CLK_EE_SAR_ADC] = "cts_sar_adc_clk",
    [CLK_EE_TS] = "cts_ts_clk",
    [CLK_EE_BCON] = "cts_bcon_clk",
    [CLK_EE_TX_PIX] = "cts_vdout_clk",
    [CLK_EE_VP_OPT] = "cts_vp_opt_clk",
    [CLK_EE_TX_DET] = "cts_tx_det_clk",
    [CLK_EE_APB2] = "cts_apb2_clk",
    [CLK_EE_HDMIRX_5M] = "cts_hdmirx_5m_clk_ee",
    [CLK_EE_HDMIRX_CFG] = "cts_hdmirx_cfg_clk",
    [CLK_EE_HDMIRX_HDCP2X_E] = "cts_hdmirx_hdcp2x_eclk",
    [CLK_EE_HDMIRX_AUD_PLL] = "cts_hdmirx_aud_pll_clk",
    [CLK_EE_HDMIRX_ACR_REF] = "cts_hdmirx_acr_ref_clk"
};

//=============================================================================
// Helper Functions - Common utility
//=============================================================================

static uint32_t reg_read(uint32_t addr) {
    return *(volatile uint32_t*)addr;
}

static void reg_write(uint32_t addr, uint32_t value) {
    // printf("reg_write: addr = 0x%08X, value = 0x%08X\n", addr, value);
    *(volatile uint32_t*)addr = value;
}

static void reg_update_bits(uint32_t addr, uint32_t mask, uint32_t value) {
    uint32_t reg_val = reg_read(addr);
    reg_val = (reg_val & ~mask) | (value & mask);
    reg_write(addr, reg_val);
}

static uint32_t create_mask(uint8_t start_bit, uint8_t width) {
    if (width == 0 || start_bit == 0xFF) return 0;
    return ((1U << width) - 1) << start_bit;
}

static int is_valid_clock_id(clock_id_t clk_id) {
    return (clk_id >= CLK_SRC_XTAL && clk_id < CLK_ID_MAX);
}

// Get source frequency in MHz - unified for all clock IDs
static uint32_t get_source_frequency(clock_id_t clk_id) {
    // Simply call clock_get_frequency - it handles both fixed and configurable clocks
    return clock_get_frequency(clk_id);
}

// Calculate divider for target frequency
static uint32_t calculate_divider(uint32_t source_freq_mhz, uint32_t target_freq_mhz) {
    if (target_freq_mhz == 0) return 0;
    uint32_t divider = (source_freq_mhz + target_freq_mhz - 1) / target_freq_mhz;
    return (divider > 0) ? divider - 1 : 0;  // Hardware divider is N+1
}

//=============================================================================
// Dependency Checking - User must configure upper layer clocks manually
//=============================================================================

int clock_check_dependencies(clock_id_t clk_id) {
    if (!is_valid_clock_id(clk_id)) return -1;
    
    const clock_config_t* config = &clock_configs[clk_id];
    
    // Check each dependency
    for (int i = 0; i < 4; i++) {
        clock_id_t dep_clk = config->dependencies[i];
        if (dep_clk == CLK_ID_MAX) break;  // End of dependencies
        
        if (!is_valid_clock_id(dep_clk)) continue;
        
        // Check if dependency is enabled/configured
        if (!clock_is_enabled(dep_clk)) {
            return -1;  // Dependency not configured - user must set it manually
        }
    }
    
    return 0;  // All dependencies satisfied
}

//=============================================================================
// STANDARD Clock API Implementation - Each clock treated independently
//=============================================================================

// Unified Clock Configuration - ONE function for everything
int clock_set_frequency(clock_id_t target_id, uint32_t source_id, uint32_t freq_mhz) {
    if (!is_valid_clock_id(target_id)) return -1;
    
    const clock_config_t* config = &clock_configs[target_id];
    
    // freq_mhz = 0 means disable clock
    if (freq_mhz == 0) {
        if (config->gate_bit != 0xFF && config->reg_addr != 0) {
            uint32_t mask = 1U << config->gate_bit;
            reg_update_bits(config->reg_addr, mask, 0);  // Disable gate
        }
        return 0;
    }
    
    // Check frequency limit
    if (freq_mhz > config->max_freq_mhz) return -1;
    
    // Check dependencies first - user must configure upper layer clocks manually
    if (clock_check_dependencies(target_id) != 0) {
        return -1;  // Dependencies not satisfied - report error
    }
    
    // Validate source_id is in available sources
    int source_index = -1;
    for (int i = 0; i < 8; i++) {
        if (config->sources[i] == source_id) {
            source_index = i;
            break;
        }
    }
    if (source_index == -1) {
        printf("  ✗ Invalid source: %u not found in sources array\n", source_id);
        return -1;  // Invalid source
    }
    
    // Get source frequency
    uint32_t source_freq = get_source_frequency((clock_id_t)source_id);
    if (source_freq == 0) return -1;
    
    // Configure source selection if clock has selection
    if (config->sel_start_bit != 0xFF && config->sel_width > 0) {
        uint32_t mask = create_mask(config->sel_start_bit, config->sel_width);
        uint32_t value = source_index << config->sel_start_bit;
        reg_update_bits(config->reg_addr, mask, value);
    }
    
    // Configure divider if clock has divider
    if (config->div_start_bit != 0xFF && config->div_width > 0) {
        uint32_t divider = calculate_divider(source_freq, freq_mhz);
        uint32_t max_div = (1U << config->div_width) - 1;
        if (divider > max_div) return -1;
        
        uint32_t mask = create_mask(config->div_start_bit, config->div_width);
        uint32_t value = divider << config->div_start_bit;
        reg_update_bits(config->reg_addr, mask, value);
    }
    
    // Enable gate if clock has gate
    if (config->gate_bit != 0xFF && config->reg_addr != 0) {
        uint32_t mask = 1U << config->gate_bit;
        reg_update_bits(config->reg_addr, mask, mask);  // Enable gate
    }
    
    return 0;
}

//=============================================================================
// Query functions - Standard implementation
//=============================================================================

uint32_t clock_get_frequency(clock_id_t clk_id) {
    if (!is_valid_clock_id(clk_id)) return 0;
    
    const clock_config_t* config = &clock_configs[clk_id];
    
    // Fixed frequency sources (reg_addr == 0)
    if (config->reg_addr == 0) {
        return config->max_freq_mhz;  // For fixed sources, this IS the frequency
    }
    
    // Configurable clock - read register value
    uint32_t reg_val = reg_read(config->reg_addr);
    
    // Get divider value (N+1 divider)
    uint32_t divider = 0;
    if (config->div_start_bit != 0xFF && config->div_width > 0) {
        divider = (reg_val >> config->div_start_bit) & ((1U << config->div_width) - 1);
    }
    
    // Get source
    uint32_t source = config->sources[0];  // Default to first source
    if (config->sel_start_bit != 0xFF && config->sel_width > 0) {
        uint32_t source_sel = (reg_val >> config->sel_start_bit) & ((1U << config->sel_width) - 1);
        if (source_sel < 8) source = config->sources[source_sel];
    }
    
    uint32_t source_freq = get_source_frequency(source);
    
    // Apply divider (N+1 formula)
    if (config->div_start_bit != 0xFF && config->div_width > 0) {
        return source_freq / (divider + 1);
    } else {
        return source_freq;  // No divider
    }
}

int clock_is_enabled(clock_id_t clk_id) {
    if (!is_valid_clock_id(clk_id)) return 0;
    
    const clock_config_t* config = &clock_configs[clk_id];
    if (config->gate_bit == 0xFF) return 1;  // Always enabled
    
    uint32_t reg_val = reg_read(config->reg_addr);
    return (reg_val >> config->gate_bit) & 1;
}

const clock_config_t* clock_get_config(clock_id_t clk_id) {
    if (!is_valid_clock_id(clk_id)) return NULL;
    return &clock_configs[clk_id];
}

const char* clock_get_name(clock_id_t clk_id) {
    if (!is_valid_clock_id(clk_id)) return "invalid";
    return clock_names[clk_id];
}

