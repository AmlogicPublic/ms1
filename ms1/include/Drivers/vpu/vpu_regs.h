#ifdef REGISTER_H
#else
#define REGISTER_H


#define RDMA_CTRL (0x40000+(0x0100<<2))//0x40400
//Bit 31:24     reserved
//Bit 23:16     ctrl_clr_rdma_done_int                  //unsigned, WO, default = 0, clear int. Write 1 to reset rdma_int level to 0. No need to clear this bit.
//Bit 15        ctrl_clr_err                            //unsigned, WO, default = 0, clear rdma err
//Bit 14:12     reserved
//Bit 11:10     ctrl_ram_wr_burst_size                  //unsigned, RW, default = 0
//Bit 9:8       ctrl_ram_rd_burst_size                  //unsigned, RW, default = 0
//Bit 7         ctrl_ddr_wr_urgent                      //unsigned, RW, default = 0 
//Bit 6         ctrl_ddr_rd_urgent                      //unsigned, RW, default = 0, 
//Bit 5:4       ctrl_ahb_wr_burst_size                  //unsigned, RW, default = 0, ABH write request burst size, 0:16 1:24: 2:32 3:48
//Bit 3:2       ctrl_ahb_rd_burst_size                  //unsigned, RW, default = 0, ABH read request burst size, 0:16 1:24: 2:32 3:48
//Bit 1         ctrl_sw_reset                           //unsigned, RW, default = 0, 1=Reset RDMA logic except register
//Bit 0         ctrl_free_clk_enable                    //unsigned, RW, default = 0, Enable clock gating. 1=No clock gating, enable free clock

#define RDMA_SRAM_CNTL (0x40000+(0x0101<<2))//0x40404
//Bit 31:18     reserved
//Bit 17        ram_incr_mode                           //unsigned, RW, default = 0, increase mode, 0=SRAM address increment after RDMA_SRAM_REGDATA; 1=SRAM address increment after RDMA_SRAM_REGADDR
//Bit 16        ram_wr_int                              //unsigned, RW, default = 0, rd/wr mode, 0=Write; 1=Read
//Bit 15:10     reserved
//Bit 9:0       ram_addr_int                            //unsigned, RW, default = 0, init sram addr

#define RDMA_SRAM_REGADDR (0x40000+(0x0102<<2))//0x40408
//Bit 31:16     reserved
//Bit 15:0      ram_regaddr                             //unsigned, RW, default = 0, reg addr which write to sram or read from sram

#define RDMA_SRAM_REGDATA (0x40000+(0x0103<<2))//0x4040c
//Bit 31:0      ram_regdata                             //unsigned, RW, default = 0, reg data which write to sram or read from sram

#define RDMA_STATUS (0x40000+(0x0104<<2))//0x40410
//Bit 31:30     reserved
//Bit 29:28     axi_w_st                                //unsigned, RO, default = 0
//Bit 27:26     axi_aw_st                               //unsigned, RO, default = 0
//Bit 25:24     axi_ar_st                               //unsigned, RO, default = 0
//Bit 22:20     rdma_id_axi                             //unsigned, RO, default = 0, axi rdma channel id
//Bit 19        reserved
//Bit 18:16     rdma_id_pipe                            //unsigned, RO, default = 0, sram rdma channel id
//Bit 15:8      reserved
//Bit 7 :0      req_latch                               //unsigned, RO, default = 0, req_latch. Requests that are yet to be serviced. E.g. 0000=No request; 0001=Req 0 waiting; 1100=Req 2 and 3 waiting.

#define RDMA_STATUS1 (0x40000+(0x0105<<2))//0x40414
//Bit 31:12     reserved
//Bit 11:4      rdma_done_flag                          //unsigned, RO, default = 0
//Bit 3         reserved
//Bit 2         err_axi_wrfifo_underflow                //unsigned, RO, default = 0
//Bit 1         err_axi_wrfifo_ovrflow                  //unsigned, RO, default = 0
//Bit 0         err_axi_rdfifo_ovrflow                  //unsigned, RO, default = 0

#define RDMA_STATUS2 (0x40000+(0x0106<<2))//0x40418
//Bit 31:30     reserved
//Bit 29:24     shr_wrfifo_cnt                          //unsigned, RO, default = 0
//Bit 23:22     reserved
//Bit 21:16     shr_rdfifo_cnt                          //unsigned, RO, default = 0
//Bit 15:14     reserved
//Bit 13:8      axi_wrfifo_room                         //unsigned, RO, default = 0
//Bit 7:6       reserved
//Bit 5:0       axi_rdfifo_room                         //unsigned, RO, default = 0

#define RDMA_STATUS3 (0x40000+(0x0107<<2))//0x4041c
//Bit 31:30     reserved
//Bit 29:28     ram_req_st                              //unsigned, RO, default = 0
//Bit 27:24     rdma_id_ram                             //unsigned, RO, default = 0
//Bit 23:8      axi_b_pending                           //unsigned, RO, default = 0
//Bit 7:0       axi_aw_pending                          //unsigned, RO, default = 0

#define RDMA_ACCESS_MAN (0x40000+(0x0110<<2))//0x40440
//Bit 31:4      reserved
//Bit 3         ctrl_buf_sel_man                        //unsigned, RW, default = 0, 0:select ddr 1:select sram
//Bit 2         ctrl_cbus_write_man                     //unsigned, RW, default = 0, Register read/write mode for manual-start. 1=Register write; 0=Register read
//Bit 1         ctrl_cbus_addr_incr_man                 //unsigned, RW, default = 0, 1=Incremental register access for manual-start; 0=Non-incremental (individual) register access
//Bit 0         ctrl_start_man                          //unsigned, WO, default = 0, Write 1 to this bit to manual-start DMA. This bit always read back 0

#define RDMA_AHB_START_ADDR_MAN_MSB (0x40000+(0x0111<<2))//0x40444
//Bit 31:2      reserved
//Bit  1:0      AHB start address_high                  //unsigned, RW, default = 0, AHB start address[33:32] for manual start DMA

#define RDMA_AHB_START_ADDR_MAN (0x40000+(0x0112<<2))//0x40448
//Bit 31:0      AHB start address_low                   //unsigned, RW, default = 0, AHB start address[31:0] for manual start DMA

#define RDMA_AHB_END_ADDR_MAN_MSB (0x40000+(0x0113<<2))//0x4044c
//Bit 31:2      reserved
//Bit  1:0      AHB end address_high                    //unsigned, RW, default = 0, AHB end address[33:32] for manual start DMA

#define RDMA_AHB_END_ADDR_MAN (0x40000+(0x0114<<2))//0x40450
//Bit 31:0      AHB end address_low                     //unsigned, RW, default = 0, AHB end address[31:0] for manual start DMA

#define RDMA_ACCESS_AUTO (0x40000+(0x0120<<2))//0x40480
//Bit 31:8      reserved
//Bit    7      ctrl_cbus_write_3                       //unsigned, RW, default = 0, Register read/write mode for auto-start 3. 1=Register write; 0=Register read
//Bit    6      ctrl_cbus_write_2                       //unsigned, RW, default = 0, Register read/write mode for auto-start 2. 1=Register write; 0=Register read
//Bit    5      ctrl_cbus_write_1                       //unsigned, RW, default = 0, Register read/write mode for auto-start 1. 1=Register write; 0=Register read
//Bit    4      reserved
//Bit    3      ctrl_cbus_addr_incr_3                   //unsigned, RW, default = 0, 1=Incremental register access for auto-start 3; 0=Non-incremental (individual) register access
//Bit    2      ctrl_cbus_addr_incr_2                   //unsigned, RW, default = 0, 1=Incremental register access for auto-start 2; 0=Non-incremental (individual) register access
//Bit    1      ctrl_cbus_addr_incr_1                   //unsigned, RW, default = 0, 1=Incremental register access for auto-start 1; 0=Non-incremental (individual) register access
//Bit    0      reserved

#define RDMA_ACCESS_AUTO2 (0x40000+(0x0121<<2))//0x40484
//Bit 31:8      reserved
//Bit    7      ctrl_cbus_write_7                       //unsigned, RW, default = 0, Register read/write mode for auto-start 7. 1=Register write; 0=Register read
//Bit    6      ctrl_cbus_write_6                       //unsigned, RW, default = 0, Register read/write mode for auto-start 6. 1=Register write; 0=Register read
//Bit    5      ctrl_cbus_write_5                       //unsigned, RW, default = 0, Register read/write mode for auto-start 5. 1=Register write; 0=Register read
//Bit    4      ctrl_cbus_write_4                       //unsigned, RW, default = 0, Register read/write mode for auto-start 4. 1=Register write; 0=Register read
//Bit    3      ctrl_cbus_addr_incr_7                   //unsigned, RW, default = 0, 1=Incremental register access for auto-start 7; 0=Non-incremental (individual) register access
//Bit    2      ctrl_cbus_addr_incr_6                   //unsigned, RW, default = 0, 1=Incremental register access for auto-start 6; 0=Non-incremental (individual) register access
//Bit    1      ctrl_cbus_addr_incr_5                   //unsigned, RW, default = 0, 1=Incremental register access for auto-start 5; 0=Non-incremental (individual) register access
//Bit    0      ctrl_cbus_addr_incr_4                   //unsigned, RW, default = 0, 1=Incremental register access for auto-start 4; 0=Non-incremental (individual) register access

#define RDMA_ACCESS_AUTO4 (0x40000+(0x0123<<2))//0x4048c
//Bit 31:8  reserved
//Bit 7      ctrl_buf_sel_7                      //unsigned, RW, default = 0
//Bit 6      ctrl_buf_sel_6                      //unsigned, RW, default = 0
//Bit 5      ctrl_buf_sel_5                      //unsigned, RW, default = 0
//Bit 4      ctrl_buf_sel_4                      //unsigned, RW, default = 0
//Bit 3      ctrl_buf_sel_3                      //unsigned, RW, default = 0
//Bit 2      ctrl_buf_sel_2                      //unsigned, RW, default = 0
//Bit 1      ctrl_buf_sel_1                      //unsigned, RW, default = 0
//Bit 0      reserved

#define RDMA_AUTO_SRC1_SEL (0x40000+(0x0124<<2))//0x40490
//Bit 31:0      ctrl_enable_int_1                       //unsigned, RW, default = 0

#define RDMA_AHB_START_ADDR_1_MSB (0x40000+(0x0125<<2))//0x40494
//Bit 31:2      reserved
//Bit  1:0      AHB start address_high                  //unsigned, RW, default = 0, AHB start address[33:32] for  auto start source 1

#define RDMA_AHB_START_ADDR_1 (0x40000+(0x0126<<2))//0x40498
//Bit 31:0      AHB start address_low                   //unsigned, RW, default = 0, AHB start address[31:0] for auto start source 1

#define RDMA_AHB_END_ADDR_1_MSB (0x40000+(0x0127<<2))//0x4049c
//Bit 31:2      reserved
//Bit  1:0      AHB end address_high                    //unsigned, RW, default = 0, AHB end address[33:32] for  auto start source 1

#define RDMA_AHB_END_ADDR_1 (0x40000+(0x0128<<2))//0x404a0
//Bit 31:0      AHB end address_low                     //unsigned, RW, default = 0, AHB end address[31:0] for auto start source 1

#define RDMA_AUTO_SRC2_SEL (0x40000+(0x0129<<2))//0x404a4
//Bit 31:0      ctrl_enable_int_2                       //unsigned, RW, default = 0

#define RDMA_AHB_START_ADDR_2_MSB (0x40000+(0x012a<<2))//0x404a8
//Bit 31:2      reserved
//Bit  1:0      AHB start address_high                  //unsigned, RW, default = 0, AHB start address[33:32] for  auto start source 2

#define RDMA_AHB_START_ADDR_2 (0x40000+(0x012b<<2))//0x404ac
//Bit 31:0      AHB start address_low                   //unsigned, RW, default = 0, AHB start address[31:0] for auto start source 2

#define RDMA_AHB_END_ADDR_2_MSB (0x40000+(0x012c<<2))//0x404b0
//Bit 31:2      reserved
//Bit  1:0      AHB end address_high                    //unsigned, RW, default = 0, AHB end address[33:32] for  auto start source 2

#define RDMA_AHB_END_ADDR_2 (0x40000+(0x012d<<2))//0x404b4
//Bit 31:0      AHB end address_low                     //unsigned, RW, default = 0, AHB end address[31:0] for auto start source 2

#define RDMA_AUTO_SRC3_SEL (0x40000+(0x012e<<2))//0x404b8
//Bit 31:0      ctrl_enable_int_3                       //unsigned, RW, default = 0

#define RDMA_AHB_START_ADDR_3_MSB (0x40000+(0x012f<<2))//0x404bc
//Bit 31:2      reserved
//Bit  1:0      AHB start address_high                  //unsigned, RW, default = 0, AHB start address[33:32] for  auto start source 3

#define RDMA_AHB_START_ADDR_3 (0x40000+(0x0130<<2))//0x404c0
//Bit 31:0      AHB start address_low                   //unsigned, RW, default = 0, AHB start address[31:0] for auto start source 3

#define RDMA_AHB_END_ADDR_3_MSB (0x40000+(0x0131<<2))//0x404c4
//Bit 31:2      reserved
//Bit  1:0      AHB end address_high                    //unsigned, RW, default = 0, AHB end address[33:32] for  auto start source 3

#define RDMA_AHB_END_ADDR_3 (0x40000+(0x0132<<2))//0x404c8
//Bit 31:0      AHB end address_low                     //unsigned, RW, default = 0, AHB end address[31:0] for auto start source 3

#define RDMA_AUTO_SRC4_SEL (0x40000+(0x0133<<2))//0x404cc
//Bit 31:0      ctrl_enable_int_4                       //unsigned, RW, default = 0

#define RDMA_AHB_START_ADDR_4_MSB (0x40000+(0x0134<<2))//0x404d0
//Bit 31:2      reserved
//Bit  1:0      AHB start address_high                  //unsigned, RW, default = 0, AHB start address[33:32] for  auto start source 4

#define RDMA_AHB_START_ADDR_4 (0x40000+(0x0135<<2))//0x404d4
//Bit 31:0      AHB start address_low                   //unsigned, RW, default = 0, AHB start address[31:0] for auto start source 4

#define RDMA_AHB_END_ADDR_4_MSB (0x40000+(0x0136<<2))//0x404d8
//Bit 31:2      reserved
//Bit  1:0      AHB end address_high                    //unsigned, RW, default = 0, AHB end address[33:32] for  auto start source 4

#define RDMA_AHB_END_ADDR_4 (0x40000+(0x0137<<2))//0x404dc
//Bit 31:0      AHB end address_low                     //unsigned, RW, default = 0, AHB end address[31:0] for auto start source 4

#define RDMA_AUTO_SRC5_SEL (0x40000+(0x0138<<2))//0x404e0
//Bit 31:0      ctrl_enable_int_5                       //unsigned, RW, default = 0

#define RDMA_AHB_START_ADDR_5_MSB (0x40000+(0x0139<<2))//0x404e4
//Bit 31:2      reserved
//Bit  1:0      AHB start address_high                  //unsigned, RW, default = 0, AHB start address[33:32] for  auto start source 5

#define RDMA_AHB_START_ADDR_5 (0x40000+(0x013a<<2))//0x404e8
//Bit 31:0      AHB start address_low                   //unsigned, RW, default = 0, AHB start address[31:0] for auto start source 5

#define RDMA_AHB_END_ADDR_5_MSB (0x40000+(0x013b<<2))//0x404ec
//Bit 31:2      reserved
//Bit  1:0      AHB end address_high                    //unsigned, RW, default = 0, AHB end address[33:32] for  auto start source 5

#define RDMA_AHB_END_ADDR_5 (0x40000+(0x013c<<2))//0x404f0
//Bit 31:0      AHB end address_low                     //unsigned, RW, default = 0, AHB end address[31:0] for auto start source 5

#define RDMA_AUTO_SRC6_SEL (0x40000+(0x013d<<2))//0x404f4
//Bit 31:0      ctrl_enable_int_6                       //unsigned, RW, default = 0

#define RDMA_AHB_START_ADDR_6_MSB (0x40000+(0x013e<<2))//0x404f8
//Bit 31:2      reserved
//Bit  1:0      AHB start address_high                  //unsigned, RW, default = 0, AHB start address[33:32] for  auto start source 6

#define RDMA_AHB_START_ADDR_6 (0x40000+(0x013f<<2))//0x404fc
//Bit 31:0      AHB start address_low                   //unsigned, RW, default = 0, AHB start address[31:0] for auto start source 6

#define RDMA_AHB_END_ADDR_6_MSB (0x40000+(0x0140<<2))//0x40500
//Bit 31:2      reserved
//Bit  1:0      AHB end address_high                    //unsigned, RW, default = 0, AHB end address[33:32] for  auto start source 6

#define RDMA_AHB_END_ADDR_6 (0x40000+(0x0141<<2))//0x40504
//Bit 31:0      AHB end address_low                     //unsigned, RW, default = 0, AHB end address[31:0] for auto start source 6

#define RDMA_AUTO_SRC7_SEL (0x40000+(0x0142<<2))//0x40508
//Bit 31:0      ctrl_enable_int_7                       //unsigned, RW, default = 0

#define RDMA_AHB_START_ADDR_7_MSB (0x40000+(0x0143<<2))//0x4050c
//Bit 31:2      reserved
//Bit  1:0      AHB start address_high                  //unsigned, RW, default = 0, AHB start address[33:32] for  auto start source 7

#define RDMA_AHB_START_ADDR_7 (0x40000+(0x0144<<2))//0x40510
//Bit 31:0      AHB start address_low                   //unsigned, RW, default = 0, AHB start address[31:0] for auto start source 7

#define RDMA_AHB_END_ADDR_7_MSB (0x40000+(0x0145<<2))//0x40514
//Bit 31:2      reserved
//Bit  1:0      AHB end address_high                    //unsigned, RW, default = 0, AHB end address[33:32] for  auto start source 7

#define RDMA_AHB_END_ADDR_7 (0x40000+(0x0146<<2))//0x40518
//Bit 31:0      AHB end address_low                     //unsigned, RW, default = 0, AHB end address[31:0] for auto start source 7

#define RDMA_INT_MODEL_SEL (0x40000+(0x01cf<<2))//0x4073c
//Bit 31:1      reserved
//Bit    0      reg_rdma_int_model     //unsigned, RW, default = 1

#define RDMA_DONE_FLAG_SEL (0x40000+(0x01f0<<2))//0x407c0
//Bit 31:8      reserved
//Bit 7 :0      reg_rdma_done_flag_sel //unsigned, RW, default = 0


#define VI_HIST_CTRL (0x40000+(0x1000<<2))//0x44000
//Bit 31:24        reg_bit_sel               // unsigned ,    RW  , default = 0
//Bit 23:20        reserved
//Bit 19:18        reg_go_field_sel          // unsigned ,    RW  , default = 0
//Bit 17:16        reg_spl_sft               // unsigned ,    RW  , default = 0
//Bit 15           reserved
//Bit 14           reg_hist_34bin_only       // unsigned ,    RW  , default = 0   0&255 and other 32bins
//Bit 13:11        reg_luma_hist_switch      // unsigned ,    RW  , default = 0   0: scaler out  1:HDR out 2:shrp out 3:CM out  4:osd_blend out
//Bit 10: 8        reg_hist_din_comp_mux     // unsigned ,    RW  , default = 0   mux of [29:22], [19:12], [9:2] for hist detect
//Bit  7: 5        reg_hist_ctrl             // unsigned ,    RW  , default = 0   the real pixels in each bins got by DNLP_VPP_GAMMA_XX should multiple with 2^(dnlp_low+3)
//Bit  4           reserved
//Bit  3: 2        reg_hist_din_sel          // unsigned ,    RW  , default = 0   the source used for hist statistics.  00: from matrix0 dout,  01: from vsc_dout, 10: from matrix1 dout, 11: form matrix1 din
//Bit  1           reg_win_en                // unsigned ,    RW  , default = 0   1'b0: hist used for full picture; 1'b1: hist used for pixels within hist window
//Bit  0           reg_luma_hist_spl_en      // unsigned ,    RW  , default = 0   disable hist readback; 1'b1: enable hist readback

#define VI_HIST_H_START_END (0x40000+(0x1001<<2))//0x44004
//Bit 31:29        reserved
//Bit 28:16        reg_luma_wnd_2            // unsigned ,    RW, default = 1919  horizontal end value to define hist window
//Bit 15:13        reserved
//Bit 12: 0        reg_luma_wnd_0            // unsigned ,    RW, default = 0     horizontal start value to define hist window

#define VI_HIST_V_START_END (0x40000+(0x1002<<2))//0x44008
//Bit 31:29        reserved
//Bit 28:16        reg_luma_wnd_3            // unsigned ,    RW, default = 1079  vertical end value to define hist window
//Bit 15:13        reserved
//Bit 12: 0        reg_luma_wnd_1            // unsigned ,    RW, default = 0     vertical start value to define hist window

#define VI_HIST_BLACK_WHITE_VALUE (0x40000+(0x1003<<2))//0x4400c
//Bit 31:16        reserved
//Bit 15: 8        reg_blk_static_thd        // unsigned ,    RW, default = 16    >= this value will be white pixel
//Bit  7: 0        reg_wht_static_thd        // unsigned ,    RW, default = 240   <= this value will be black pixel

#define DNLP_MAX_RGB_EN (0x40000+(0x1004<<2))//0x44010
//Bit 31: 2        reserved
//Bit  1           reg_max_rgb_en            // unsigned ,    RW, default = 0
//Bit  0           reg_sta_data_is_rgb       // unsigned ,    RW, default = 0

#define DNLP_VPP_HIST_SUM (0x40000+(0x1005<<2))//0x44014
//Bit 31: 0        ro_hist_sum               // unsigned ,    RO, default = 0   counts for the total calculated pixels

#define DNLP_VPP_LUMA_SUM (0x40000+(0x1006<<2))//0x44018
//Bit 31: 0        ro_vpp_luma_sum           // unsigned ,    RW, default = 0

#define DNLP_VPP_GAMMA_00_01 (0x40000+(0x1007<<2))//0x4401c
//Bit 31:0         ro_vpp_gamma_00_01        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_02_03 (0x40000+(0x1008<<2))//0x44020
//Bit 31:0         ro_vpp_gamma_02_03        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_04_05 (0x40000+(0x1009<<2))//0x44024
//Bit 31:0         ro_vpp_gamma_04_05        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_06_07 (0x40000+(0x100a<<2))//0x44028
//Bit 31:0         ro_vpp_gamma_06_07        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_08_09 (0x40000+(0x100b<<2))//0x4402c
//Bit 31:0         ro_vpp_gamma_08_09        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_10_11 (0x40000+(0x100c<<2))//0x44030
//Bit 31:0         ro_vpp_gamma_10_11        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_12_13 (0x40000+(0x100d<<2))//0x44034
//Bit 31:0         ro_vpp_gamma_12_13        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_14_15 (0x40000+(0x100e<<2))//0x44038
//Bit 31:0         ro_vpp_gamma_14_15        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_16_17 (0x40000+(0x100f<<2))//0x4403c
//Bit 31:0         ro_vpp_gamma_16_17        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_18_19 (0x40000+(0x1010<<2))//0x44040
//Bit 31:0         ro_vpp_gamma_18_19        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_20_21 (0x40000+(0x1011<<2))//0x44044
//Bit 31:0         ro_vpp_gamma_20_21        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_22_23 (0x40000+(0x1012<<2))//0x44048
//Bit 31:0         ro_vpp_gamma_22_23        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_24_25 (0x40000+(0x1013<<2))//0x4404c
//Bit 31:0         ro_vpp_gamma_24_25        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_26_27 (0x40000+(0x1014<<2))//0x44050
//Bit 31:0         ro_vpp_gamma_26_27        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_28_29 (0x40000+(0x1015<<2))//0x44054
//Bit 31:0         ro_vpp_gamma_28_29        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_30_31 (0x40000+(0x1016<<2))//0x44058
//Bit 31:0         ro_vpp_gamma_30_31        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_32_33 (0x40000+(0x1017<<2))//0x4405c
//Bit 31:0         ro_vpp_gamma_32_33        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_34_35 (0x40000+(0x1018<<2))//0x44060
//Bit 31:0         ro_vpp_gamma_34_35        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_36_37 (0x40000+(0x1019<<2))//0x44064
//Bit 31:0         ro_vpp_gamma_36_37        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_38_39 (0x40000+(0x101a<<2))//0x44068
//Bit 31:0         ro_vpp_gamma_38_39        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_40_41 (0x40000+(0x101b<<2))//0x4406c
//Bit 31:0         ro_vpp_gamma_40_41        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_42_43 (0x40000+(0x101c<<2))//0x44070
//Bit 31:0         ro_vpp_gamma_42_43        // unsigned ,    RO, default = 0   

#define DNLP_VPP_GAMMA_44_45 (0x40000+(0x101d<<2))//0x44074
//Bit 31:0         ro_vpp_gamma_44_45        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_46_47 (0x40000+(0x101e<<2))//0x44078
//Bit 31:0         ro_vpp_gamma_46_47        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_48_49 (0x40000+(0x101f<<2))//0x4407c
//Bit 31:0         ro_vpp_gamma_48_49        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_50_51 (0x40000+(0x1020<<2))//0x44080
//Bit 31:0         ro_vpp_gamma_50_51        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_52_53 (0x40000+(0x1021<<2))//0x44084
//Bit 31:0         ro_vpp_gamma_52_53        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_54_55 (0x40000+(0x1022<<2))//0x44088
//Bit 31:0         ro_vpp_gamma_54_55        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_56_57 (0x40000+(0x1023<<2))//0x4408c
//Bit 31:0         ro_vpp_gamma_56_57        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_58_59 (0x40000+(0x1024<<2))//0x44090
//Bit 31:0         ro_vpp_gamma_58_59        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_60_61 (0x40000+(0x1025<<2))//0x44094
//Bit 31:0         ro_vpp_gamma_60_61        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_62_63 (0x40000+(0x1026<<2))//0x44098
//Bit 31:0         ro_vpp_gamma_62_63        // unsigned ,    RO, default = 0

#define DNLP_VPP_GAMMA_64_65 (0x40000+(0x1027<<2))//0x4409c
//Bit 31:0         ro_vpp_gamma_64_65        // unsigned ,    RO, default = 0

#define VI_RO_HIST_LOW_IDX (0x40000+(0x1028<<2))//0x440a0
//Bit 31:7         reserved
//Bit  6           reg_idx_auto_inc          // unsigned ,    RW, default = 0 , 0:not increase 1:auto increase
//Bit  5:0         reg_rd_idx                // unsigned ,    RW, default = 0 , init hist value

#define VI_RO_HIST_LOW (0x40000+(0x1029<<2))//0x440a4
//Bit 31:24        reserved
//Bit 23: 0        ro_vi_ro_hist_low         // unsigned ,    RO, default = 0 , hist counts in each bin for 0-63, 64 bins, add automatically 

#define DNLP_HIST_MINMAX_8B (0x40000+(0x102a<<2))//0x440a8
//Bit 31:16        reserved
//Bit 15: 8        reg_vpp_hist_min_8b       // unsigned ,    RW, default = 0
//Bit  7: 0        reg_vpp_hist_max_8b       // unsigned ,    RW, default = 0

#define DNLP_STA_CSC_PRE_OFST (0x40000+(0x102b<<2))//0x440ac
//Bit 31           reserved
//Bit 30:29        reg_sta_csc_rs            // unsigned ,    RW, default = 0  norm= (1<<(10+rs))
//Bit 28:16        reg_sta_csc_pre_ofset_1   //   signed ,    RW, default = -512  pre-offset for chroma
//Bit 15:13        reserved
//Bit 12: 0        reg_sta_csc_pre_ofset_0   //   signed ,    RW, default = 0  pre-offset for luma

#define DNLP_STA_CSC_PST_OFST (0x40000+(0x102c<<2))//0x440b0
//Bit 31:29        reserved
//Bit 28:16        reg_sta_csc_pst_ofset_1   //   signed ,    RW, default = 0  pre-offset for chroma
//Bit 15:13        reserved
//Bit 12: 0        reg_sta_csc_pst_ofset_0   //   signed ,    RW, default = 0  pre-offset for luma

#define DNLP_STA_CSC_CLIP_RS (0x40000+(0x102d<<2))//0x440b4
//Bit 31:28        reserved
//Bit 27:16        reg_sta_csc_clip_1        // unsigned ,    RW, default = 1023  down clipping for rgb
//Bit 15:12        reserved
//Bit 11: 0        reg_sta_csc_clip_0        // unsigned ,    RW, default = 0  down clipping for rgb

#define DNLP_STA_CSC_MAT_0_1 (0x40000+(0x102e<<2))//0x440b8
//Bit 31:29        reserved
//Bit 28:16        reg_sta_csc_mat3x3_1      //   signed ,    RW, default = 0  3x3 matrix coef
//Bit 15:13        reserved
//Bit 12: 0        reg_sta_csc_mat3x3_0      //   signed ,    RW, default = 1024  3x3 matrix coef

#define DNLP_STA_CSC_MAT_2_3 (0x40000+(0x102f<<2))//0x440bc
//Bit 31:29        reserved
//Bit 28:16        reg_sta_csc_mat3x3_3      //   signed ,    RW, default = 1024  3x3 matrix coef
//Bit 15:13        reserved
//Bit 12: 0        reg_sta_csc_mat3x3_2      //   signed ,    RW, default = 1577  3x3 matrix coef

#define DNLP_STA_CSC_MAT_4_5 (0x40000+(0x1030<<2))//0x440c0
//Bit 31:29        reserved
//Bit 28:16        reg_sta_csc_mat3x3_5      //   signed ,    RW, default = -470  3x3 matrix coef
//Bit 15:13        reserved
//Bit 12: 0        reg_sta_csc_mat3x3_4      //   signed ,    RW, default = -187  3x3 matrix coef

#define DNLP_STA_CSC_MAT_6_7 (0x40000+(0x1031<<2))//0x440c4
//Bit 31:29        reserved
//Bit 28:16        reg_sta_csc_mat3x3_7      //   signed ,    RW, default = 1860  3x3 matrix coef
//Bit 15:13        reserved
//Bit 12: 0        reg_sta_csc_mat3x3_6      //   signed ,    RW, default = 1024  3x3 matrix coef

#define DNLP_STA_CSC_MAT_8 (0x40000+(0x1032<<2))//0x440c8
//Bit 31:13        reserved
//Bit 12: 0        reg_sta_csc_mat3x3_8      //   signed ,    RW, default = 0  3x3 matrix coef

#define DNLP_HIST_6BIN_MIN_0_1 (0x40000+(0x1033<<2))//0x440cc
//Bit 31:16        ro_vpp_hist_min_1         // unsigned ,    RW, default = 0
//Bit 15: 0        ro_vpp_hist_min_0         // unsigned ,    RW, default = 0

#define DNLP_HIST_6BIN_MIN_2_3 (0x40000+(0x1034<<2))//0x440d0
//Bit 31:16        ro_vpp_hist_min_3         // unsigned ,    RW, default = 0
//Bit 15: 0        ro_vpp_hist_min_2         // unsigned ,    RW, default = 0

#define DNLP_HIST_6BIN_MIN_4_5 (0x40000+(0x1035<<2))//0x440d4
//Bit 31:16        ro_vpp_hist_min_5         // unsigned ,    RW, default = 0
//Bit 15: 0        ro_vpp_hist_min_4         // unsigned ,    RW, default = 0

#define DNLP_HIST_6BIN_MAX_0_1 (0x40000+(0x1036<<2))//0x440d8
//Bit 31:16        ro_vpp_hist_max_1         // unsigned ,    RW, default = 0
//Bit 15: 0        ro_vpp_hist_max_0         // unsigned ,    RW, default = 0

#define DNLP_HIST_6BIN_MAX_2_3 (0x40000+(0x1037<<2))//0x440dc
//Bit 31:16        ro_vpp_hist_max_3         // unsigned ,    RW, default = 0
//Bit 15: 0        ro_vpp_hist_max_2         // unsigned ,    RW, default = 0

#define DNLP_HIST_6BIN_MAX_4_5 (0x40000+(0x1038<<2))//0x440e0
//Bit 31:16        ro_vpp_hist_max_5         // unsigned ,    RW, default = 0
//Bit 15: 0        ro_vpp_hist_max_4         // unsigned ,    RW, default = 0


#define DNLP_SINGLE_BIN_FLAG (0x40000+(0x1040<<2))//0x44100
//Bit 31:29        reserved                           
//Bit 28           ro_dnlp_single_bin_flag_7 // unsigned ,    RO, default = 0  
//Bit 27:25        reserved                           
//Bit 24           ro_dnlp_single_bin_flag_6 // unsigned ,    RO, default = 0  
//Bit 23:21        reserved                           
//Bit 20           ro_dnlp_single_bin_flag_5 // unsigned ,    RO, default = 0  
//Bit 19:17        reserved                           
//Bit 16           ro_dnlp_single_bin_flag_4 // unsigned ,    RO, default = 0  
//Bit 15:13        reserved                           
//Bit 12           ro_dnlp_single_bin_flag_3 // unsigned ,    RO, default = 0  
//Bit 11: 9        reserved                           
//Bit  8           ro_dnlp_single_bin_flag_2 // unsigned ,    RO, default = 0  
//Bit  7: 5        reserved                           
//Bit  4           ro_dnlp_single_bin_flag_1 // unsigned ,    RO, default = 0  
//Bit  3: 1        reserved                           
//Bit  0           ro_dnlp_single_bin_flag_0 // unsigned ,    RO, default = 0  

#define DNLP_MAXBIN_SCH_FLAG (0x40000+(0x1041<<2))//0x44104
//Bit 31:29        reserved                           
//Bit 28           ro_dnlp_maxbin_sch_flag_7 // unsigned ,    RO, default = 0  
//Bit 27:25        reserved                           
//Bit 24           ro_dnlp_maxbin_sch_flag_6 // unsigned ,    RO, default = 0  
//Bit 23:21        reserved                           
//Bit 20           ro_dnlp_maxbin_sch_flag_5 // unsigned ,    RO, default = 0  
//Bit 19:17        reserved                           
//Bit 16           ro_dnlp_maxbin_sch_flag_4 // unsigned ,    RO, default = 0  
//Bit 15:13        reserved                           
//Bit 12           ro_dnlp_maxbin_sch_flag_3 // unsigned ,    RO, default = 0  
//Bit 11: 9        reserved                           
//Bit  8           ro_dnlp_maxbin_sch_flag_2 // unsigned ,    RO, default = 0  
//Bit  7: 5        reserved                           
//Bit  4           ro_dnlp_maxbin_sch_flag_1 // unsigned ,    RO, default = 0  
//Bit  3: 1        reserved                           
//Bit  0           ro_dnlp_maxbin_sch_flag_0 // unsigned ,    RO, default = 0  

#define DNLP_SCH_CHG_FLAG (0x40000+(0x1042<<2))//0x44108
//Bit 31:29        reserved                           
//Bit 28           ro_dnlp_scn_chg_7         // unsigned ,    RO, default = 0  
//Bit 27:25        reserved                           
//Bit 24           ro_dnlp_scn_chg_6         // unsigned ,    RO, default = 0  
//Bit 23:21        reserved                           
//Bit 20           ro_dnlp_scn_chg_5         // unsigned ,    RO, default = 0  
//Bit 19:17        reserved                           
//Bit 16           ro_dnlp_scn_chg_4         // unsigned ,    RO, default = 0  
//Bit 15:13        reserved                           
//Bit 12           ro_dnlp_scn_chg_3         // unsigned ,    RO, default = 0  
//Bit 11: 9        reserved                           
//Bit  8           ro_dnlp_scn_chg_2         // unsigned ,    RO, default = 0  
//Bit  7: 5        reserved                           
//Bit  4           ro_dnlp_scn_chg_1         // unsigned ,    RO, default = 0  
//Bit  3: 1        reserved                           
//Bit  0           ro_dnlp_scn_chg_0         // unsigned ,    RO, default = 0  

#define DNLP_SCN_CHG_FLAG_NORM (0x40000+(0x1043<<2))//0x4410c
//Bit 31: 9        reserved                           
//Bit  8           reg_dnlp_respond_flag     // unsigned ,    RW, default = 0  
//Bit  7: 5        reserved                           
//Bit  4: 0        reg_dnlp_norm             // unsigned ,    RW, default = 10  

#define DNLP_SCNG_THD_AND_SBIN (0x40000+(0x1044<<2))//0x44110
//Bit 31:24        reserved                           
//Bit 23:20        reg_dnlp_single_bin_bw    // unsigned ,    RW, default = 1  
//Bit 19:18        reserved                           
//Bit 17:16        reg_dnlp_single_bin_method // unsigned ,    RW, default = 2  
//Bit 15: 8        reg_dnlp_scn_chg_th       // unsigned ,    RW, default = 48  
//Bit  7: 0        reg_dnlp_tmaxbin_ratio    // unsigned ,    RW, default = 204  

#define DNLP_SCNG_HIST_SUM_RATIO (0x40000+(0x1045<<2))//0x44114
//Bit 31:16        reserved                           
//Bit 15: 0        reg_dnlp_scn_chg_hist_sum_ratio // unsigned ,    RW, default = 4142  


#define LC_STA_CTRL (0x40000+(0x1050<<2))//0x44140
//Bit 31:23        reserved                           
//Bit 22:20        reg_lc_hist_switch        // unsigned ,    RW, default = 0  0: scaler out  1:HDR out 2:shrp out 3:CM out  4:osd_blend out
//Bit 19:17        reserved                           
//Bit 16           reg_lcsta_inputrgb        // unsigned ,    RW, default = 0  LC statistic input 3 components is RGB or YUV:  0: YUV; 1:RGB
//Bit 15:13        reserved                           
//Bit 12           reg_lcsta1max_lpf         // unsigned ,    RW, default = 0  STA1max statistics on [1 2 1]/4 filtered results
//Bit 11: 9        reserved                           
//Bit  8           reg_lcstahist_lpf         // unsigned ,    RW, default = 0  STAhist statistics on [1 2 1]/4 filtered results
//Bit  7: 5        reserved                           
//Bit  4           reg_lcstahist_mode        // unsigned ,    RW, default = 1  histogram statistics on XX separately 20bits*16bins:  0: Y-only; 1: MAX(R,G,B)
//Bit  3: 2        reserved                           
//Bit  1: 0        reg_lcstahist_pix_drop_mode // unsigned ,    RW, default = 0  

#define LC_STA_HV_NUM (0x40000+(0x1051<<2))//0x44144
//Bit 31:13        reserved                           
//Bit 12: 8        reg_lcsta_vnum            // unsigned ,    RW, default = 8  Maximum to BLKVMAX, should be same as reg_lc_blk_vnum
//Bit  7: 5        reserved                           
//Bit  4: 0        reg_lcsta_hnum            // unsigned ,    RW, default = 12  Maximum to BLKHMAX, should be same as reg_lc_blk_hnum

#define LC_STA_HIDX_0_1 (0x40000+(0x1052<<2))//0x44148
//Bit 31:29        reserved                           
//Bit 28:16        reg_lcsta_hidx_1          // unsigned ,    RW, default = 320  block boundary x-index
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lcsta_hidx_0          // unsigned ,    RW, default = 0  block boundary x-index

#define LC_STA_HIDX_2_3 (0x40000+(0x1053<<2))//0x4414c
//Bit 31:29        reserved                           
//Bit 28:16        reg_lcsta_hidx_3          // unsigned ,    RW, default = 960  block boundary x-index
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lcsta_hidx_2          // unsigned ,    RW, default = 640  block boundary x-index

#define LC_STA_HIDX_4_5 (0x40000+(0x1054<<2))//0x44150
//Bit 31:29        reserved                           
//Bit 28:16        reg_lcsta_hidx_5          // unsigned ,    RW, default = 1600  block boundary x-index
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lcsta_hidx_4          // unsigned ,    RW, default = 1280  block boundary x-index

#define LC_STA_HIDX_6_7 (0x40000+(0x1055<<2))//0x44154
//Bit 31:29        reserved                           
//Bit 28:16        reg_lcsta_hidx_7          // unsigned ,    RW, default = 2240  block boundary x-index
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lcsta_hidx_6          // unsigned ,    RW, default = 1920  block boundary x-index

#define LC_STA_HIDX_8_9 (0x40000+(0x1056<<2))//0x44158
//Bit 31:29        reserved                           
//Bit 28:16        reg_lcsta_hidx_9          // unsigned ,    RW, default = 2880  block boundary x-index
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lcsta_hidx_8          // unsigned ,    RW, default = 2560  block boundary x-index

#define LC_STA_HIDX_10_11 (0x40000+(0x1057<<2))//0x4415c
//Bit 31:29        reserved                           
//Bit 28:16        reg_lcsta_hidx_11         // unsigned ,    RW, default = 3520  block boundary x-index
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lcsta_hidx_10         // unsigned ,    RW, default = 3200  block boundary x-index

#define LC_STA_HIDX_12 (0x40000+(0x1058<<2))//0x44160
//Bit 31:13        reserved                           
//Bit 12: 0        reg_lcsta_hidx_12         // unsigned ,    RW, default = 3840  block boundary x-index

#define LC_STA_VIDX_0_1 (0x40000+(0x1059<<2))//0x44164
//Bit 31:29        reserved                           
//Bit 28:16        reg_lcsta_vidx_1          // unsigned ,    RW, default = 270  block boundary y-index
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lcsta_vidx_0          // unsigned ,    RW, default = 0  block boundary y-index

#define LC_STA_VIDX_2_3 (0x40000+(0x105a<<2))//0x44168
//Bit 31:29        reserved                           
//Bit 28:16        reg_lcsta_vidx_3          // unsigned ,    RW, default = 810  block boundary y-index
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lcsta_vidx_2          // unsigned ,    RW, default = 540  block boundary y-index

#define LC_STA_VIDX_4_5 (0x40000+(0x105b<<2))//0x4416c
//Bit 31:29        reserved                           
//Bit 28:16        reg_lcsta_vidx_5          // unsigned ,    RW, default = 1350  block boundary y-index
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lcsta_vidx_4          // unsigned ,    RW, default = 1080  block boundary y-index

#define LC_STA_VIDX_6_7 (0x40000+(0x105c<<2))//0x44170
//Bit 31:29        reserved                           
//Bit 28:16        reg_lcsta_vidx_7          // unsigned ,    RW, default = 1890  block boundary y-index
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lcsta_vidx_6          // unsigned ,    RW, default = 1620  block boundary y-index

#define LC_STA_VIDX_8 (0x40000+(0x105d<<2))//0x44174
//Bit 31:13        reserved                           
//Bit 12: 0        reg_lcsta_vidx_8          // unsigned ,    RW, default = 2160  block boundary y-index

#define LC_STA_CSC_OFST (0x40000+(0x105e<<2))//0x44178
//Bit 31:27        reserved                           
//Bit 26:16        reg_lcsta_csc_ofset_1     // signed ,    RW, default = -512  pre-offset for chroma
//Bit 15:11        reserved                           
//Bit 10: 0        reg_lcsta_csc_ofset_0     // signed ,    RW, default = 0  pre-offset for luma

#define LC_STA_CSC_CLIP_RS (0x40000+(0x105f<<2))//0x4417c
//Bit 31            reserved                           
//Bit 30:28        reg_lcsta_csc_rs          // unsigned ,    RW, default = 0  
//Bit 27:16        reg_lcsta_csc_clip_1      // unsigned ,    RW, default = 1023  down clipping for rgb
//Bit 15:12        reserved                           
//Bit 11: 0        reg_lcsta_csc_clip_0      // unsigned ,    RW, default = 0  down clipping for rgb

#define LC_STA_CSC_MAT_0_1 (0x40000+(0x1060<<2))//0x44180
//Bit 31:29        reserved                           
//Bit 28:16        reg_lcsta_csc_mat3x3_1    // signed ,    RW, default = 0  3x3 matrix coef
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lcsta_csc_mat3x3_0    // signed ,    RW, default = 1024  3x3 matrix coef

#define LC_STA_CSC_MAT_2_3 (0x40000+(0x1061<<2))//0x44184
//Bit 31:29        reserved                           
//Bit 28:16        reg_lcsta_csc_mat3x3_3    // signed ,    RW, default = 1024  3x3 matrix coef
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lcsta_csc_mat3x3_2    // signed ,    RW, default = 1577  3x3 matrix coef

#define LC_STA_CSC_MAT_4_5 (0x40000+(0x1062<<2))//0x44188
//Bit 31:29        reserved                           
//Bit 28:16        reg_lcsta_csc_mat3x3_5    // signed ,    RW, default = -470  3x3 matrix coef
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lcsta_csc_mat3x3_4    // signed ,    RW, default = -187  3x3 matrix coef

#define LC_STA_CSC_MAT_6_7 (0x40000+(0x1063<<2))//0x4418c
//Bit 31:29        reserved                           
//Bit 28:16        reg_lcsta_csc_mat3x3_7    // signed ,    RW, default = 1860  3x3 matrix coef
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lcsta_csc_mat3x3_6    // signed ,    RW, default = 1024  3x3 matrix coef

#define LC_STA_CSC_MAT_8 (0x40000+(0x1064<<2))//0x44190
//Bit 31:13        reserved                           
//Bit 12: 0        reg_lcsta_csc_mat3x3_8    // signed ,    RW, default = 0  3x3 matrix coef

#define LC_STA_BLK_BRT_THRD (0x40000+(0x1065<<2))//0x44194
//Bit 31:16        reserved                           
//Bit 15: 8        reg_lcsta_blk_thrd        // unsigned ,    RW, default = 1  to luma_u8 to count number of black pixels luma_u8<thrd;
//Bit  7: 0        reg_lcsta_brt_thrd        // unsigned ,    RW, default = 254  to luma_u8 to count number of bright pixels luma_u8>thrd;

#define RO_LC_STA_BLK_NUM (0x40000+(0x1066<<2))//0x44198
//Bit 31:24        reserved                           
//Bit 23: 0        ro_lcsta_blk_num          // unsigned ,    RO, default = 0  of pixels within luma< sta_blk_thrd.

#define RO_LC_STA_BRT_NUM (0x40000+(0x1067<<2))//0x4419c
//Bit 31:24        reserved                           
//Bit 23: 0        ro_lcsta_brt_num          // unsigned ,    RO, default = 0  of pixels within luma> sta_brt_thrd.

#define LC_STTS_HIST_REGION_IDX (0x40000+(0x1068<<2))//0x441a0

#define LC_STTS_HIST_READ_REGION (0x40000+(0x1069<<2))//0x441a4

#define LC_STTS_HIST_START_RD_REGION (0x40000+(0x106a<<2))//0x441a8


#define LDIM_STTS_GCLK_CTRL0 (0x40000+(0x1080<<2))//0x44200

#define LDIM_STTS_CTRL0 (0x40000+(0x1081<<2))//0x44204

#define LDIM_STTS_WIDTHM1_HEIGHTM1 (0x40000+(0x1082<<2))//0x44208

#define LDIM_STTS_MATRIX_COEF00_01 (0x40000+(0x1083<<2))//0x4420c

#define LDIM_STTS_MATRIX_COEF02_10 (0x40000+(0x1084<<2))//0x44210

#define LDIM_STTS_MATRIX_COEF11_12 (0x40000+(0x1085<<2))//0x44214

#define LDIM_STTS_MATRIX_COEF20_21 (0x40000+(0x1086<<2))//0x44218

#define LDIM_STTS_MATRIX_COEF22 (0x40000+(0x1087<<2))//0x4421c

#define LDIM_STTS_MATRIX_OFFSET0_1 (0x40000+(0x1088<<2))//0x44220

#define LDIM_STTS_MATRIX_OFFSET2 (0x40000+(0x1089<<2))//0x44224

#define LDIM_STTS_MATRIX_PRE_OFFSET0_1 (0x40000+(0x108a<<2))//0x44228

#define LDIM_STTS_MATRIX_PRE_OFFSET2 (0x40000+(0x108b<<2))//0x4422c

#define LDIM_STTS_MATRIX_HL_COLOR (0x40000+(0x108c<<2))//0x44230

#define LDIM_STTS_MATRIX_PROBE_POS (0x40000+(0x108d<<2))//0x44234

#define LDIM_STTS_MATRIX_PROBE_COLOR (0x40000+(0x108e<<2))//0x44238
//     //Bit 31, local dimming statistic enable
//     //Bit 29, 1: output region histogram 16bit 0:output region histogram 20bit 
//     //Bit 28, eol enable
//     //Bit 27:25, vertical line overlap number for max finding
//     //Bit 24:22, horizontal pixel overlap number, 0: 17 pix, 1: 9 pix, 2: 5 pix, 3: 3 pix, 4: 0 pix
//     //Bit 20, 1,2,1 low pass filter enable before max/hist statistic
//     //Bit 19:16, region H/V position index, refer to VDIN_LDIM_STTS_HIST_SET_REGION
//     //Bit 15:14, 1: region read index auto increase per block read finished to VDIN_LDIM_STTS_HIST_READ_REGION 
//     //Bit 13:8, region read sub index, which mux the hist & max-finding result to cbus port, refer to LDIM_STTS_HIST_READ_REGION
//     //Bit 6:0, region read index

#define LDIM_STTS_HIST_REGION_IDX (0x40000+(0x108f<<2))//0x4423c
//Bit 28:0, if VDIN_LDIM_STTS_HIST_REGION_IDX[19:16] == 5'h0: read/write hvstart0
//     //hvstart0, Bit 28:16 row0 vstart, Bit 12:0 col0 hstart
//     //hend01, Bit 28:16 col1 hend, Bit 12:0 col0 hend
//     //vend01, Bit 28:16 row1 vend, Bit 12:0 row0 vend
//     //hend23, Bit 28:16 col3 hend, Bit 12:0 col2 hend
//     //vend23, Bit 28:16 row3 vend, Bit 12:0 row2 vend
//     //hend45, Bit 28:16 col5 hend, Bit 12:0 col4 hend
//     //vend45, Bit 28:16 row5 vend, Bit 12:0 row4 vend
//     //hend67, Bit 28:16 col7 hend, Bit 12:0 col6 hend
//     //vend67, Bit 28:16 row7 vend, Bit 12:0 row6 vend
//     //hend89, Bit 28:16 col9 hend, Bit 12:0 col8 hend
//     //vend89, Bit 28:16 row9 vend, Bit 12:0 row8 vend

#define LDIM_STTS_HIST_SET_REGION (0x40000+(0x1090<<2))//0x44240

#define LDIM_STTS_HIST_READ_REGION (0x40000+(0x1091<<2))//0x44244

#define LDIM_STTS_HIST_START_RD_REGION (0x40000+(0x1092<<2))//0x44248

#define LDIM_STTS_PCTL_TH (0x40000+(0x1093<<2))//0x4424c


#define LC_CURVE_CTRL (0x40000+(0x10a0<<2))//0x44280
//Bit 31:14        reserved                           
//Bit 13:12        reg_lc_hist_curve_nodes_hlpf // unsigned ,    RW, default = 1  horizontal lpf of the ram_curve_nodes, 0: no LPF, 1= [1 2 1]; 2: [1 2 2 2 1]/8
//Bit 11:10        reserved                           
//Bit  9: 8        reg_lc_hist_curve_nodes_vlpf // unsigned ,    RW, default = 1  vertical lpf of the ram_curve_nodes, 0: no LPF, 1= [1 2 1]; 2: [1 2 2 2 1]/8
//Bit  7: 5        reserved                           
//Bit  4           reg_lc_blackbar_mute_en   // unsigned ,    RW, default = 1  the black bar corresponding bin, 0: no mute, 1: mute enable; default=1
//Bit  3: 1        reserved                           
//Bit  0           reg_lc_curve_en           // unsigned ,    RW, default = 0  

#define LC_CURVE_HV_NUM (0x40000+(0x10a1<<2))//0x44284
//Bit 31:13        reserved                           
//Bit 12: 8        reg_lc_cur_blk_hnum       // unsigned ,    RW, default = 12  lc processing region number of V, maximum to (STA_LEN_V-1)   (0~8), should be same as reg_lc_blk_hnum
//Bit  7: 5        reserved                           
//Bit  4: 0        reg_lc_cur_blk_vnum       // unsigned ,    RW, default = 8  lc processing region number of H, maximum to (STA_LEN_H-1)   (0~12), should be same as reg_lc_blk_vnum

#define LC_CURVE_LMT_RAT (0x40000+(0x10a2<<2))//0x44288
//Bit 31:16        reserved                           
//Bit 15: 8        reg_lmtrat_minmax         // unsigned ,    RW, default = 6  of amount
//Bit  7: 0        reg_lmtrat_valid          // unsigned ,    RW, default = 20  of amount

#define LC_CURVE_CONTRAST_LH (0x40000+(0x10a3<<2))//0x4428c
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_contrast_low       // unsigned ,    RW, default = 20  gain to the lc for dark side, normalized 256 as "1", set adaptive TODO
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_contrast_hig       // unsigned ,    RW, default = 32  gain to the lc for bright side, normalized 256 as "1"

#define LC_CURVE_CONTRAST_LMT_LH (0x40000+(0x10a4<<2))//0x44290
//Bit 31:24        reg_lc_cntstlmt_low_0     // unsigned ,    RW, default = 10  for the contrast low, delta_low = MIN(delta_low, MIN( MAX((minBV-min_val)*scl_low/8, lmt_low[0]),lmt_low[1]))
//Bit 23:16        reg_lc_cntstlmt_hig_0     // unsigned ,    RW, default = 20  for the contrast high,delta_hig = MIN(delta_hig, MIN( MAX((max_val-maxBV)*scl_hig/8, lmt_hig[0]),lmt_hig[1]))
//Bit 15: 8        reg_lc_cntstlmt_low_1     // unsigned ,    RW, default = 255  for the contrast low, delta_low = MIN(delta_low, MIN( MAX((minBV-min_val)*scl_low/8, lmt_low[0]),lmt_low[1]))
//Bit  7: 0        reg_lc_cntstlmt_hig_1     // unsigned ,    RW, default = 255  for the contrast high,delta_hig = MIN(delta_hig, MIN( MAX((max_val-maxBV)*scl_hig/8, lmt_hig[0]),lmt_hig[1]))

#define LC_CURVE_CONTRAST_SCL_LH (0x40000+(0x10a5<<2))//0x44294
//Bit 31:16        reserved                           
//Bit 15: 8        reg_lc_cntstscl_low       // unsigned ,    RW, default = 20  for the contrast low, norm 8 as 1; delta_low = MIN(delta_low, MIN(MAX((minBV-min_val)*scl_low/8, lmt_low[0]),lmt_low[1]))
//Bit  7: 0        reg_lc_cntstscl_hig       // unsigned ,    RW, default = 32  for the contrast high,norm 8 as 1; delta_hig = MIN(delta_hig, MIN(MAX((max_val-maxBV)*scl_hig/8, lmt_hig[0]),lmt_hig[1]))

#define LC_CURVE_CONTRAST_BVN_LH (0x40000+(0x10a6<<2))//0x44298
//Bit 31:16        reserved                           
//Bit 15: 8        reg_lc_cntstbvn_low       // unsigned ,    RW, default = 20  to num_m as limit of min_val to minBV distance, to protect mono-color, default = 32; min_val= MAX(min_val, minBV- MAX(num_m-1,0)*bvn_low)
//Bit  7: 0        reg_lc_cntstbvn_hig       // unsigned ,    RW, default = 32  to num_m as limit of max_val to maxBV distance, to protect mono-color, default = 32; min_val= MIN(max_val, maxBV+ MAX(num_m-1,0)*bvn_lo

#define LC_CURVE_MISC0 (0x40000+(0x10a7<<2))//0x4429c
//Bit 31:20        reserved                           
//Bit 19:16        reg_lc_num_m_coring       // unsigned ,    RW, default = 2  to num_m, soft coring,default = 2;
//Bit 15: 8        reg_lc_vbin_min           // unsigned ,    RW, default = 48  is min width of valid histogram bin num,
//Bit  7: 0        reg_lc_slope_max_face     // unsigned ,    RW, default = 32  maximum slope for the pkBin-maxBV range curve to do face protection, normalized to 32 as 1, default= 48

#define LC_YPKBV_RATIO (0x40000+(0x10a8<<2))//0x442a0
//Bit 31:24        reg_lc_ypkbv_ratio_3      // unsigned ,    RW, default = 96  x= ratio*(maxBv-minBv)+min_val as low bound of the ypkBV; normalized to 256 as 1
//Bit 23:16        reg_lc_ypkbv_ratio_2      // unsigned ,    RW, default = 120  x= ratio*(maxBv-minBv)+min_val as low bound of the ypkBV; normalized to 256 as 1
//Bit 15: 8        reg_lc_ypkbv_ratio_1      // unsigned ,    RW, default = 160  x= ratio*(maxBv-minBv)+min_val as low bound of the ypkBV; normalized to 256 as 1
//Bit  7: 0        reg_lc_ypkbv_ratio_0      // unsigned ,    RW, default = 140  x= ratio*(maxBv-minBv)+min_val as low bound of the ypkBV; normalized to 256 as 1

#define LC_CURVE_YPKBV_SLP_LMT (0x40000+(0x10a9<<2))//0x442a4
//Bit 31:16        reserved                           
//Bit 15: 8        reg_lc_ypkbv_slope_lmt_1  // unsigned ,    RW, default = 50  min max slop for the curves to avoid artifacts, [0] for min_slope, [1] for max_slop, e.g.max_slope= limit*(pkBv-minBv)+min_val as high bound of the ypkBV; normalized to 32 as 1
//Bit  7: 0        reg_lc_ypkbv_slope_lmt_0  // unsigned ,    RW, default = 38  

#define LC_CURVE_YMINVAL_LMT_0_1 (0x40000+(0x10aa<<2))//0x442a8
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_yminval_lmt_1      // unsigned ,    RW, default = 93  lmt_val = lmt[minBV(64:64:1023)], and yminV = MAX(yminV,lmt_val), for very dark region boost, default= [48, 80, 120, 60]
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_yminval_lmt_0      // unsigned ,    RW, default = 48  lmt_val = lmt[minBV(64:64:1023)], and yminV = MAX(yminV,lmt_val), for very dark region boost, default= [48, 80, 120, 60]

#define LC_CURVE_YMINVAL_LMT_2_3 (0x40000+(0x10ab<<2))//0x442ac
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_yminval_lmt_3      // unsigned ,    RW, default = 145  lmt_val = lmt[minBV(64:64:1023)], and yminV = MAX(yminV,lmt_val), for very dark region boost, default= [48, 80, 120, 60]
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_yminval_lmt_2      // unsigned ,    RW, default = 131  lmt_val = lmt[minBV(64:64:1023)], and yminV = MAX(yminV,lmt_val), for very dark region boost, default= [48, 80, 120, 60]

#define LC_CURVE_YMINVAL_LMT_4_5 (0x40000+(0x10ac<<2))//0x442b0
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_yminval_lmt_5      // unsigned ,    RW, default = 196  lmt_val = lmt[minBV(64:64:1023)], and yminV = MAX(yminV,lmt_val), for very dark region boost, default= [48, 80, 120, 60]
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_yminval_lmt_4      // unsigned ,    RW, default = 160  lmt_val = lmt[minBV(64:64:1023)], and yminV = MAX(yminV,lmt_val), for very dark region boost, default= [48, 80, 120, 60]

#define LC_CURVE_YMINVAL_LMT_6_7 (0x40000+(0x10ad<<2))//0x442b4
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_yminval_lmt_7      // unsigned ,    RW, default = 256  lmt_val = lmt[minBV(64:64:1023)], and yminV = MAX(yminV,lmt_val), for very dark region boost, default= [48, 80, 120, 60]
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_yminval_lmt_6      // unsigned ,    RW, default = 224  lmt_val = lmt[minBV(64:64:1023)], and yminV = MAX(yminV,lmt_val), for very dark region boost, default= [48, 80, 120, 60]

#define LC_CURVE_YMINVAL_LMT_8_9 (0x40000+(0x10ae<<2))//0x442b8
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_yminval_lmt_9      // unsigned ,    RW, default = 320  lmt_val = lmt[minBV(64:64:1023)], and yminV = MAX(yminV,lmt_val), for very dark region boost, default= [48, 80, 120, 60]
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_yminval_lmt_8      // unsigned ,    RW, default = 288  lmt_val = lmt[minBV(64:64:1023)], and yminV = MAX(yminV,lmt_val), for very dark region boost, default= [48, 80, 120, 60]

#define LC_CURVE_YMINVAL_LMT_10_11 (0x40000+(0x10af<<2))//0x442bc
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_yminval_lmt_11     // unsigned ,    RW, default = 400  lmt_val = lmt[minBV(64:64:1023)], and yminV = MAX(yminV,lmt_val), for very dark region boost, default= [48, 80, 120, 60]
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_yminval_lmt_10     // unsigned ,    RW, default = 352  lmt_val = lmt[minBV(64:64:1023)], and yminV = MAX(yminV,lmt_val), for very dark region boost, default= [48, 80, 120, 60]

#define LC_CURVE_YMINVAL_LMT_12_13 (0x40000+(0x10b0<<2))//0x442c0
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_yminval_lmt_13     // unsigned ,    RW, default = 489  lmt_val = lmt[minBV(64:64:1023)], and yminV = MAX(yminV,lmt_val), for very dark region boost, default= [48, 80, 120, 60]
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_yminval_lmt_12     // unsigned ,    RW, default = 444  lmt_val = lmt[minBV(64:64:1023)], and yminV = MAX(yminV,lmt_val), for very dark region boost, default= [48, 80, 120, 60]

#define LC_CURVE_YMINVAL_LMT_14_15 (0x40000+(0x10b1<<2))//0x442c4
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_yminval_lmt_15     // unsigned ,    RW, default = 576  lmt_val = lmt[minBV(64:64:1023)], and yminV = MAX(yminV,lmt_val), for very dark region boost, default= [48, 80, 120, 60]
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_yminval_lmt_14     // unsigned ,    RW, default = 531  lmt_val = lmt[minBV(64:64:1023)], and yminV = MAX(yminV,lmt_val), for very dark region boost, default= [48, 80, 120, 60]

#define LC_CURVE_YPKBV_LMT_0_1 (0x40000+(0x10b2<<2))//0x442c8
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_ypkbv_lmt_1        // unsigned ,    RW, default = 140  lmt_val = 4*lmt[pkBV(64:64:1023)] , and ypkBV = MAX(ypkBV,lmt[pkBV]),
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_ypkbv_lmt_0        // unsigned ,    RW, default = 68  lmt_val = 4*lmt[pkBV(64:64:1023)] , and ypkBV = MAX(ypkBV,lmt[pkBV]).

#define LC_CURVE_YPKBV_LMT_2_3 (0x40000+(0x10b3<<2))//0x442cc
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_ypkbv_lmt_3        // unsigned ,    RW, default = 272  lmt_val = 4*lmt[pkBV(64:64:1023)] , and ypkBV = MAX(ypkBV,lmt[pkBV]),
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_ypkbv_lmt_2        // unsigned ,    RW, default = 205  lmt_val = 4*lmt[pkBV(64:64:1023)] , and ypkBV = MAX(ypkBV,lmt[pkBV]).

#define LC_CURVE_YPKBV_LMT_4_5 (0x40000+(0x10b4<<2))//0x442d0
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_ypkbv_lmt_5        // unsigned ,    RW, default = 404  lmt_val = 4*lmt[pkBV(64:64:1023)] , and ypkBV = MAX(ypkBV,lmt[pkBV]),
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_ypkbv_lmt_4        // unsigned ,    RW, default = 339  lmt_val = 4*lmt[pkBV(64:64:1023)] , and ypkBV = MAX(ypkBV,lmt[pkBV]).

#define LC_CURVE_YPKBV_LMT_6_7 (0x40000+(0x10b5<<2))//0x442d4
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_ypkbv_lmt_7        // unsigned ,    RW, default = 537  lmt_val = 4*lmt[pkBV(64:64:1023)] , and ypkBV = MAX(ypkBV,lmt[pkBV]),
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_ypkbv_lmt_6        // unsigned ,    RW, default = 470  lmt_val = 4*lmt[pkBV(64:64:1023)] , and ypkBV = MAX(ypkBV,lmt[pkBV]).

#define LC_CURVE_YPKBV_LMT_8_9 (0x40000+(0x10b6<<2))//0x442d8
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_ypkbv_lmt_9        // unsigned ,    RW, default = 663  lmt_val = 4*lmt[pkBV(64:64:1023)] , and ypkBV = MAX(ypkBV,lmt[pkBV]),
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_ypkbv_lmt_8        // unsigned ,    RW, default = 600  lmt_val = 4*lmt[pkBV(64:64:1023)] , and ypkBV = MAX(ypkBV,lmt[pkBV]).

#define LC_CURVE_YPKBV_LMT_10_11 (0x40000+(0x10b7<<2))//0x442dc
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_ypkbv_lmt_11       // unsigned ,    RW, default = 784  lmt_val = 4*lmt[pkBV(64:64:1023)] , and ypkBV = MAX(ypkBV,lmt[pkBV]),
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_ypkbv_lmt_10       // unsigned ,    RW, default = 724  lmt_val = 4*lmt[pkBV(64:64:1023)] , and ypkBV = MAX(ypkBV,lmt[pkBV]).

#define LC_CURVE_YPKBV_LMT_12_13 (0x40000+(0x10b8<<2))//0x442e0
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_ypkbv_lmt_13       // unsigned ,    RW, default = 901  lmt_val = 4*lmt[pkBV(64:64:1023)] , and ypkBV = MAX(ypkBV,lmt[pkBV]),
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_ypkbv_lmt_12       // unsigned ,    RW, default = 841  lmt_val = 4*lmt[pkBV(64:64:1023)] , and ypkBV = MAX(ypkBV,lmt[pkBV]).

#define LC_CURVE_YPKBV_LMT_14_15 (0x40000+(0x10b9<<2))//0x442e4
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_ypkbv_lmt_15       // unsigned ,    RW, default = 1023  lmt_val = 4*lmt[pkBV(64:64:1023)] , and ypkBV = MAX(ypkBV,lmt[pkBV]),
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_ypkbv_lmt_14       // unsigned ,    RW, default = 962  lmt_val = 4*lmt[pkBV(64:64:1023)] , and ypkBV = MAX(ypkBV,lmt[pkBV]).

#define LC_CURVE_YMAXVAL_LMT_0_1 (0x40000+(0x10ba<<2))//0x442e8
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_ymaxval_lmt_1      // unsigned ,    RW, default = 148  lmt_val = 4*lmt[maxBV(64:64:1023)], and ymaxV = MAX(ymaxV,lmt[maxBV]),
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_ymaxval_lmt_0      // unsigned ,    RW, default = 68  lmt_val = 4*lmt[maxBV(64:64:1023)], and ymaxV = MAX(ymaxV,lmt[maxBV]).

#define LC_CURVE_YMAXVAL_LMT_2_3 (0x40000+(0x10bb<<2))//0x442ec
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_ymaxval_lmt_3      // unsigned ,    RW, default = 291  lmt_val = 4*lmt[maxBV(64:64:1023)], and ymaxV = MAX(ymaxV,lmt[maxBV]),
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_ymaxval_lmt_2      // unsigned ,    RW, default = 220  lmt_val = 4*lmt[maxBV(64:64:1023)], and ymaxV = MAX(ymaxV,lmt[maxBV]).

#define LC_CURVE_YMAXVAL_LMT_4_5 (0x40000+(0x10bc<<2))//0x442f0
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_ymaxval_lmt_5      // unsigned ,    RW, default = 421  lmt_val = 4*lmt[maxBV(64:64:1023)], and ymaxV = MAX(ymaxV,lmt[maxBV]),
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_ymaxval_lmt_4      // unsigned ,    RW, default = 360  lmt_val = 4*lmt[maxBV(64:64:1023)], and ymaxV = MAX(ymaxV,lmt[maxBV]).

#define LC_CURVE_YMAXVAL_LMT_6_7 (0x40000+(0x10bd<<2))//0x442f4
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_ymaxval_lmt_7      // unsigned ,    RW, default = 550  lmt_val = 4*lmt[maxBV(64:64:1023)], and ymaxV = MAX(ymaxV,lmt[maxBV]),
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_ymaxval_lmt_6      // unsigned ,    RW, default = 488  lmt_val = 4*lmt[maxBV(64:64:1023)], and ymaxV = MAX(ymaxV,lmt[maxBV]).

#define LC_CURVE_YMAXVAL_LMT_8_9 (0x40000+(0x10be<<2))//0x442f8
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_ymaxval_lmt_9      // unsigned ,    RW, default = 674  lmt_val = 4*lmt[maxBV(64:64:1023)], and ymaxV = MAX(ymaxV,lmt[maxBV]),
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_ymaxval_lmt_8      // unsigned ,    RW, default = 616  lmt_val = 4*lmt[maxBV(64:64:1023)], and ymaxV = MAX(ymaxV,lmt[maxBV]).

#define LC_CURVE_YMAXVAL_LMT_10_11 (0x40000+(0x10bf<<2))//0x442fc
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_ymaxval_lmt_11     // unsigned ,    RW, default = 793  lmt_val = 4*lmt[maxBV(64:64:1023)], and ymaxV = MAX(ymaxV,lmt[maxBV]),
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_ymaxval_lmt_10     // unsigned ,    RW, default = 733  lmt_val = 4*lmt[maxBV(64:64:1023)], and ymaxV = MAX(ymaxV,lmt[maxBV]).

#define LC_CURVE_YMAXVAL_LMT_12_13 (0x40000+(0x10c0<<2))//0x44300
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_ymaxval_lmt_13     // unsigned ,    RW, default = 901  lmt_val = 4*lmt[maxBV(64:64:1023)], and ymaxV = MAX(ymaxV,lmt[maxBV]),
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_ymaxval_lmt_12     // unsigned ,    RW, default = 847  lmt_val = 4*lmt[maxBV(64:64:1023)], and ymaxV = MAX(ymaxV,lmt[maxBV]).

#define LC_CURVE_YMAXVAL_LMT_14_15 (0x40000+(0x10c1<<2))//0x44304
//Bit 31:26        reserved                           
//Bit 25:16        reg_lc_ymaxval_lmt_15     // unsigned ,    RW, default = 1021  lmt_val = 4*lmt[maxBV(64:64:1023)], and ymaxV = MAX(ymaxV,lmt[maxBV]),
//Bit 15:10        reserved                           
//Bit  9: 0        reg_lc_ymaxval_lmt_14     // unsigned ,    RW, default = 960  lmt_val = 4*lmt[maxBV(64:64:1023)], and ymaxV = MAX(ymaxV,lmt[maxBV]).

#define LC_CURVE_HISTVLD_THRD (0x40000+(0x10c2<<2))//0x44308
//Bit 31:12        reserved                           
//Bit 11: 0        reg_lc_histvld_thrd       // unsigned ,    RW, default = 506  threshold to compare to bin to get number of valid bins

#define LC_CURVE_BB_MUTE_THRD (0x40000+(0x10c3<<2))//0x4430c
//Bit 31:24        reserved                           
//Bit 23: 0        reg_lc_blackbar_mute_thrd // unsigned ,    RW, default = 10800  1/8 of the region

#define LC_SCN_FLAG_REFRESH_ALPHA (0x40000+(0x10c4<<2))//0x44310
//Bit 31:17        reserved                           
//Bit 16           reg_video_scene_change_flag_en // unsigned ,    RW, default = 0  
//Bit 15:13        reserved                           
//Bit 12           reg_video_scene_change_flag // unsigned ,    RW, default = 0  
//Bit 11: 0        reg_lc_refresh_alpha      // unsigned ,    RW, default = 512  

#define LC_SCN_THD_RATIO (0x40000+(0x10c5<<2))//0x44314
//Bit 31:20        reserved                           
//Bit 19:12        reg_lc_scene_change_th    // unsigned ,    RW, default = 50  
//Bit 11: 0        reg_lc_scene_dif_ratio    // unsigned ,    RW, default = 43  

#define LC_CURVE_INT_STATUS (0x40000+(0x10c6<<2))//0x44318
//Bit 31: 1        reserved                           
//Bit  0           reg_lc_curve_done_status  // unsigned ,    RW, default = 0  

#define LC_CURVE_RAM_CTRL (0x40000+(0x10c7<<2))//0x4431c
//Bit 31: 1        reserved                           
//Bit  0           reg_lc_cbus2ram_en        // unsigned ,    RW, default = 0  

#define LC_CURVE_RAM_ADDR (0x40000+(0x10c8<<2))//0x44320
//Bit 31: 8        reserved                           
//Bit  7: 4        reg_cbus_ram_vidx         // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_cbus_ram_hidx         // unsigned ,    RW, default = 0  

#define LC_CURVE_RAM_DATA (0x40000+(0x10c9<<2))//0x44324
//Bit 31:30        reserved                           
//Bit 29: 0        ro_cbus_ram_rdata         // unsigned ,    RO, default = 0  


#define VESA_STATICS_CTRL (0x40000+(0x10d0<<2))//0x44340
//Bit 31:11        reserved
//Bit 10: 8        reg_vesa_glb_luma_switch  // unsigned ,    RW, default = 0 , 0:osd_blend 1:vadj out 2:lud3d out 3:WB out 4:gamma out  5:LDC out
//Bit  7           reserved
//Bit  6: 4        reg_vesa_wnd_rgb_switch   // unsigned ,    RW, default = 4 , 0:CSC out  1:lud3d out 2:WB out 3:gamma out  4:LDC out
//Bit  3: 2        reserved
//Bit  1           reg_vesa_glb_statics_mode // unsigned ,    RW, default = 0 , is rgb, 0:yuv  1:max_rgb
//Bit  0           reg_vesa_glb_input_is_rgb // unsigned ,    RW, default = 0 , is yuv  1:input is rgb

#define VESA_STATICS_WND (0x40000+(0x10d1<<2))//0x44344
//Bit 31:16        reg_vesa_wnd_avg_xxyy_3   // unsigned ,    RW, default = 127 , vertical   end   value of average statistic window
//Bit 15: 0        reg_vesa_wnd_avg_xxyy_2   // unsigned ,    RW, default = 0   , vertical   start value of average statistic window

#define VESA_STATICS_WND_1 (0x40000+(0x10d2<<2))//0x44348
//Bit 31:16        reg_vesa_wnd_avg_xxyy_1   // unsigned ,    RW, default = 127 , horizontal end   value of average statistic window
//Bit 15: 0        reg_vesa_wnd_avg_xxyy_0   // unsigned ,    RW, default = 0   , horizontal start value of average statistic window

#define VESA_STATICS_WND_AVG (0x40000+(0x10d3<<2))//0x4434c
//Bit 31:28        reserved
//Bit 27:16        ro_vesa_wnd_avg_rgb_2     // unsigned ,    RO, default = 0 , window average b/cr statistic
//Bit 15:12        reserved
//Bit 11: 0        ro_vesa_wnd_avg_rgb_1     // unsigned ,    RO, default = 0 , window average g/cb statistic

#define VESA_STATICS_WND_AVG_1 (0x40000+(0x10d4<<2))//0x44350
//Bit 31:12        reserved
//Bit 11: 0        ro_vesa_wnd_avg_rgb_0     // unsigned ,    RO, default = 0 , window average r/y  statistic

#define VESA_STATICS_GLB (0x40000+(0x10d5<<2))//0x44354
//Bit 31:28        reserved
//Bit 27:16        ro_vesa_glb_max_luma      // unsigned ,    RO, default = 0 , global maximum luma statistic
//Bit 15:12        reserved
//Bit 11: 0        ro_vesa_glb_apl           // unsigned ,    RO, default = 0 , global average luma statistic


#define STAT_INFO_GCLK_CTRL (0x40000+(0x10e0<<2))//0x44380
//Bit 31: 0        reg_gclk_ctrl             // unsigned ,RW  , default = 0

#define STAT_HIST_PIC_SIZE (0x40000+(0x10e1<<2))//0x44384
//Bit 31:29        reserved
//Bit 28:16        reg_hist_pic_width        // unsigned ,RW  , default = 2560 , active input data window width
//Bit 15:13        reserved
//Bit 12: 0        reg_hist_pic_height       // unsigned ,RW  , default = 1440 , active input data window height

#define STAT_HIST_MAX_MIN (0x40000+(0x10e2<<2))//0x44388
//Bit 31:16        reserved
//Bit 15: 8        ro_hist_max_luma          // unsigned ,RO  , default = 0      maximum value
//Bit  7: 0        ro_hist_min_luma          // unsigned ,RO  , default = 8'hff  minimum value

#define STAT_HIST_SPL_VAL (0x40000+(0x10e3<<2))//0x4438c
//Bit 31: 0        ro_hist_spl_sft_sum       // unsigned ,RO  , default = 0 , counts for the total     luma  >>reg_spl_sft

#define STAT_HIST_CHROMA_SUM (0x40000+(0x10e4<<2))//0x44390
//Bit 31: 0        ro_hist_chroma_sum        // unsigned ,RO  , default = 0 , counts for the total avg_chroma>>reg_spl_sft

#define STAT_DNLP_SCH_ENABLE (0x40000+(0x10e5<<2))//0x44394
//Bit 31: 1        reserved
//Bit  0           reg_dnlp_sch_en           // unsigned ,RW  , default = 0 , dnlp_scene_change_enable

#define STAT_LC_STTS_CTRL0 (0x40000+(0x10e6<<2))//0x44398
//Bit 31: 0        reg_lc_stts_ctrl          // unsigned ,RW  , default = 0

#define STAT_LC_MTX_HL_COLOR (0x40000+(0x10e7<<2))//0x4439c
//Bit 31:24        reserved
//Bit 23: 0        reg_mtx_hl_color          // unsigned ,RW  , default = 0

#define STAT_LC_MTX_PROBE_POS (0x40000+(0x10e8<<2))//0x443a0
//Bit 31:29        reserved
//Bit 28:16        reg_mtx_probe_pos_x       // unsigned ,RW  , default = 0 , reg_mtx_probe_pos_x
//Bit 15:13        reserved
//Bit 12: 0        reg_mtx_probe_pos_y       // unsigned ,RW  , default = 0 , reg_mtx_probe_pos_y

#define STAT_LC_MTX_PROBE_COLOR (0x40000+(0x10e9<<2))//0x443a4
//Bit 31: 0        ro_mtx_probe_color        // unsigned ,RO  , default = 0

#define STAT_LC_INPUT_SIZE (0x40000+(0x10ea<<2))//0x443a8
//Bit 31:29        reserved
//Bit 28:16        reg_lc_stts_width         // unsigned ,RW  , default = 2560 , active input data window width
//Bit 15:13        reserved
//Bit 12: 0        reg_lc_stts_height        // unsigned ,RW  , default = 1440 , active input data window height

#define STAT_LC_CURVE_CTRL (0x40000+(0x10eb<<2))//0x443ac
//Bit 31:19        reserved
//Bit 18:17        reg_curve_int_sel         // unsigned ,RW  , default = 0 , 0:curve_done; 1:curve_iir_done; 2/3:lc_glb_scene_change
//Bit 16           reg_curve_int_mask        // unsigned ,RW  , default = 0
//Bit 15: 1        reserved
//Bit  0           reg_curve_iir_en          // unsigned ,RW  , default = 0

#define STAT_VESA_STTS_CTRL (0x40000+(0x10ec<<2))//0x443b0
//Bit 31: 2        reserved
//Bit  1           reg_vesa_glb_lma_en       // unsigned ,RW  , default = 0
//Bit  0           reg_vesa_wnd_rgb_en       // unsigned ,RW  , default = 0

#define STAT_VESA_FRM_SIZE (0x40000+(0x10ed<<2))//0x443b4
//Bit 31:29        reserved
//Bit 28:16        reg_vesa_din_hsize        // unsigned ,RW  , default = 2560 , horizontal input frame size
//Bit 15:13        reserved
//Bit 12: 0        reg_vesa_din_vsize        // unsigned ,RW  , default = 1440 , vertical   input frame size

#define STAT_DIN_CRC_SEL (0x40000+(0x10ee<<2))//0x443b8
//Bit 31:17        reserved
//Bit 16           reg_stat_crc_en           // unsigned ,RW  , default = 0
//Bit 15: 4        reserved
//Bit  3: 0        reg_stat_crc_sel          // unsigned ,RW  , default = 10 , 0:scaler; 1:hdr; 2:sharp; 3:cm; 4:blend; 5:vadj; 6:csc; 7:lut3d; 8:wb; 9:gamma; 10:ldc

#define STAT_RO_CRC_VALUE (0x40000+(0x10ef<<2))//0x443bc
//Bit 31: 0        ro_stat_crc_value         // unsigned ,RO  , default = 0


#define VPU_TOP_ENHANCE_ENABLE (0x40000+(0x1200<<2))//0x44800
//Bit 31:8      reserved                    
//Bit 7         reg_sharp_en                //unsigned, RW, default = 0
//Bit 6         reg_cm_en                   //unsigned, RW, default = 0
//Bit 5         reg_vadj_en                 //unsigned, RW, default = 0
//Bit 4         reg_contrast_en             //unsigned, RW, default = 0
//Bit 3         reg_csc_en                  //unsigned, RW, default = 0
//Bit 2         reg_lut3d_wrapper_en        //unsigned, RW, default = 0
//Bit 1         reg_gainoff_enable          //unsigned, RW, default = 0
//Bit 0         reg_gamma_en                //unsigned, RW, default = 0

#define VPU_GCLK_CTRL_0 (0x40000+(0x1201<<2))//0x44804
//Bit 31:0      reg_gclk_ctrl_0              //unsigned, RW, default = 0

#define VPU_GCLK_CTRL_1 (0x40000+(0x1202<<2))//0x44808
//Bit 31:0      reg_gclk_ctrl_1              //unsigned, RW, default = 0

#define VPU_TOP_CTRL_0 (0x40000+(0x1206<<2))//0x44818
//Bit 31:21     reserved
//Bit 20        pls_frm_start                //unsigned, W1T, default = 0
//Bit 19:17     reserved        
//Bit 16        reg_frm_start_sel            //unsigned, RW, default = 0
//Bit 15:13     reserved
//Bit 12:0      reg_hold_line_num            //unsigned, RW, default = 8

#define VPU_TOP_CTRL_1 (0x40000+(0x1207<<2))//0x4481c
//Bit 31:28     reserved
//Bit 27:16     reg_dat_reo_sel              //unsigned, RW, default = 12'h210
//Bit 15:1      reserved
//Bit 0         reg_edge_ctrl                //unsigned, RW, default = 0

#define VPU_TOP_SW_RESET (0x40000+(0x1208<<2))//0x44820
//Bit 31:2      reserved
//Bit 1:0       pls_top_sw_rst               //unsigned, W1T, default = 0

#define VPU_TOP_DE_DLY_NUM (0x40000+(0x120a<<2))//0x44828
//Bit 31:13     reserved
//Bit 12:0      reg_de_dly_ppnum           //unsigned, RW, default = 0

#define VPU_TOP_VS_DLY_NUM (0x40000+(0x120b<<2))//0x4482c
//Bit 31:13     reserved
//Bit 12:0      reg_vs_dly_ppnum          //unsigned, RW, default = 0

#define VPU_TOP_HS_DLY_NUM (0x40000+(0x120c<<2))//0x44830
//Bit 31:13     reserved
//Bit 12:0      reg_hs_dly_ppnum           //unsigned, RW, default = 0

#define VPU_TOP_UPDATE_CTRL0 (0x40000+(0x120d<<2))//0x44834
//Bit 31:26     reserved
//Bit 25:24     reg_disp_reg_update_en      //unsigned, RW, default = 2'h3
//Bit 23:20     reserved
//Bit 19:4      reg_proc_reg_update_en      //unsigned, RW, default = 16'hffff
//Bit 3:1       reserved
//Bit 0         reg_vtop_reg_update_en      //unsigned, RW, default = 1

#define VPU_TOP_UPDATE_CTRL1 (0x40000+(0x120e<<2))//0x44838
//Bit 31:26     reserved
//Bit 25:24     reg_disp_ro_update_en      //unsigned, RW, default = 2'h3
//Bit 23:20     reserved
//Bit 19:4      reg_proc_ro_update_en      //unsigned, RW, default = 16'hffff
//Bit 3:1       reserved
//Bit 0         reg_vtop_ro_update_en      //unsigned, RW, default = 1

#define VPU_TOP_ARB_BAK_CTRL (0x40000+(0x1210<<2))//0x44840
//Bit 31:26     reserved 
//Bit 25:4      reg_apb_prot_ctrl            // unsigned ,    RW,default = 22'h200006,
//Bit 3 :1      reserved 
//Bit 0         reg_arb_bak_ctrl             // unsigned ,    RW,default = 0,

#define VPU_TOP_APB_REQ_STAT (0x40000+(0x1211<<2))//0x44844
//Bit 31:18     reserved 
//Bit 17:8      ro_apb_prot_stat             // unsigned ,    RO, default = 0
//Bit 7 :1      reserved 
//Bit 0         ro_rdma_ddr_req_busy         // unsigned ,    RO, default = 0

#define VPU_TOP_APB_CRASH_ADDR (0x40000+(0x1212<<2))//0x44848
//Bit 31:16     reserved 
//Bit 15:0      ro_apb_crash_addr            // unsigned ,    RO, default = 0

#define VPU_TOP_APB_CRASH_INT (0x40000+(0x1213<<2))//0x4484c
//Bit 31:1      reserved
//Bit 0         ro_apb_crash_int             // unsigned ,    RO, default = 0

#define VPU_AXI_RARB_REQ_EN (0x40000+(0x1215<<2))//0x44854
//Bit 31:17     reserved
//Bit 16        reg_rd_arb_mode              //unsigned, RW, default=2'h3
//Bit 15:14     reserved
//Bit 13:12     reg_rd_arb_req_en            //unsigned, RW, default=2'h3
//Bit 11: 0     reg_rd_arb_weigh             //unsigned, RW, default=12'h104    

#define VPU_AXI_RARB_IDMAP (0x40000+(0x1216<<2))//0x44858
//Bit 31: 0     reg_rd_arb_idmap0            //unsigned, RW, default=32'h1ffff000

#define VPU_AXI_WARB_REQ_EN (0x40000+(0x121a<<2))//0x44868
//Bit 31:17     reserved
//Bit 16        reg_wr_arb_mode              //unsigned, RW, default=2'h3
//Bit 15:14     reserved
//Bit 13:12     reg_wr_arb_req_en            //unsigned, RW, default=2'h3
//Bit 11: 0     reg_wr_arb_weigh             //unsigned, RW, default=12'h104   

#define VPU_AXI_WARB_IDMAP (0x40000+(0x121b<<2))//0x4486c
//Bit 31: 0     reg_wr_arb_idmap0            //unsigned, RW, default=32'h1ffff000

#define VPU_TOP_RDMA_TRIG_EN (0x40000+(0x121e<<2))//0x44878
//Bit 31: 0     reg_rdma_trig_en             //unsigned, RW, default=32'hffffffff    

#define VPU_TOP_TOTAL_SIZE (0x40000+(0x1230<<2))//0x448c0
//Bit 31:29     reserved
//Bit 28:16     reg_total_hsize             //unsigned, RW, default = 0
//Bit 15:13     reserved
//Bit 12:0      reg_total_vsize             //unsigned, RW, default = 0

#define VPU_TOP_BIST_COLOR0 (0x40000+(0x1231<<2))//0x448c4
//Bit 31:26     reserved
//Bit 25:16     reg_bist_cb                 //unsigned, RW, default = 512 
//Bit 15:10     reserved
//Bit 9 :0      reg_bist_y                  //unsigned, RW, default = 0

#define VPU_TOP_BIST_COLOR1 (0x40000+(0x1232<<2))//0x448c8
//Bit 31:10     reserved
//Bit 9 :0      reg_bist_cr                 //unsigned, RW, default = 512

#define VPU_TOP_BIST_SIZE (0x40000+(0x1233<<2))//0x448cc
//Bit 31:29     reserved
//Bit 28:16     reg_bist_clrbar_width       //unsigned, RW, default = 0
//Bit 15:13     reserved
//Bit 12 :0     reg_bist_clrbar_high        //unsigned, RW, default = 0

#define VPU_TOP_BIST_PSTRT (0x40000+(0x1234<<2))//0x448d0
//Bit 31:29     reserved
//Bit 28:16     reg_bist_clrbar_vstrt       //unsigned, RW, default = 0
//Bit 15:13     reserved
//Bit 12 :0     reg_bist_clrbar_strt        //unsigned, RW, default = 0

#define VPU_TOP_BIST_MODE (0x40000+(0x1235<<2))//0x448d4
//Bit 31:8      reserved
//Bit 7 :0      reg_bist_mode_sel           //unsigned, RW, default = 0

#define VPU_TOP_BIST_VSTRT_SET (0x40000+(0x1236<<2))//0x448d8
//Bit 31:2      reserved
//Bit 1 :0      reg_bist_vdcnt_strtset      //unsigned, RW, default = 0

#define VPU_TOP_HS_PX_SCOPE (0x40000+(0x1237<<2))//0x448dc
//Bit 31:29     reserved
//Bit 28:16     reg_hs_px_end               //unsigned, RW, default = 0
//Bit 15:13     reserved
//Bit 12:0      reg_hs_px_bgn               //unsigned, RW, default = 0

#define VPU_TOP_VS_LN_SCOPE_E (0x40000+(0x1238<<2))//0x448e0
//Bit 31:29     reserved
//Bit 28:16     reg_vs_ln_end_e             //unsigned, RW, default = 0
//Bit 15:13     reserved
//Bit 12:0      reg_vs_ln_bgn_e             //unsigned, RW, default = 0

#define VPU_TOP_VS_PX_SCOPE_E (0x40000+(0x1239<<2))//0x448e4
//Bit 31:29     reserved
//Bit 28:16     reg_vs_px_end_e             //unsigned, RW, default = 0
//Bit 15:13     reserved
//Bit 12:0      reg_vs_px_bgn_e             //unsigned, RW, default = 0

#define VPU_TOP_VS_LN_SCOPE_O (0x40000+(0x123a<<2))//0x448e8
//Bit 31:29     reserved
//Bit 28:16     reg_vs_ln_end_o             //unsigned, RW, default = 0
//Bit 15:13     reserved
//Bit 12:0      reg_vs_ln_bgn_o             //unsigned, RW, default = 0

#define VPU_TOP_VS_PX_SCOPE_O (0x40000+(0x123b<<2))//0x448ec
//Bit 31:29     reserved
//Bit 28:16     reg_vs_px_end_o             //unsigned, RW, default = 0
//Bit 15:13     reserved
//Bit 12:0      reg_vs_px_bgn_o             //unsigned, RW, default = 0

#define VPU_TOP_DE_PX_SCOPE (0x40000+(0x123c<<2))//0x448f0
//Bit 31:29     reserved
//Bit 28:16     reg_de_px_end               //unsigned, RW, default = 0
//Bit 15:13     reserved
//Bit 12:0      reg_de_px_bgn               //unsigned, RW, default = 0

#define VPU_TOP_DE_LN_SCOPE_E (0x40000+(0x123d<<2))//0x448f4
//Bit 31:29     reserved
//Bit 28:16     reg_de_ln_end_e             //unsigned, RW, default = 0
//Bit 15:13     reserved
//Bit 12:0      reg_de_ln_bgn_e             //unsigned, RW, default = 0

#define VPU_TOP_DE_LN_SCOPE_O (0x40000+(0x123e<<2))//0x448f8
//Bit 31:29     reserved
//Bit 28:16     reg_de_ln_end_o             //unsigned, RW, default = 0
//Bit 15:13     reserved
//Bit 12:0      reg_de_ln_bgn_o             //unsigned, RW, default = 0

#define VPU_TOP_FIELD_LN_SCOPE (0x40000+(0x123f<<2))//0x448fc
//Bit 31:29     reserved
//Bit 28:16     reg_field_ln_bgn_o          //unsigned, RW, default = 0
//Bit 15:13     reserved
//Bit 12:0      reg_field_ln_bgn_e          //unsigned, RW, default = 0

#define VPU_TOP_TIMING_CTRL (0x40000+(0x1240<<2))//0x44900
//Bit 31:25     reserved
//Bit 24        reg_dat_timing_en           //unsigned, RW, default = 0
//Bit 23:21     reserved
//Bit 20        reg_timing_en               //unsigned, RW, default = 0
//Bit 19:17     reserved
//Bit 16        reg_field_mode              //unsigned, RW, default = 0
//Bit 15:2      reserved
//Bit 1 :0      reg_serial_rate             //unsigned, RW, default = 0

#define VPU_TOP_SCAN_EN (0x40000+(0x1248<<2))//0x44920
//Bit 31:1      reserved
//Bit 0         reg_scan_en                 //unsigned, RW, default = 0

#define VPU_SC_SIZE_IN (0x40000+(0x1249<<2))//0x44924
//Bit 31:29     reserved
//Bit 28:16     reg_sc_hsize_in             //unsigned, RW, default = 2560
//Bit 15:13     reserved
//Bit 12:0      reg_sc_vsize_in             //unsigned, RW, default = 1440

#define VPU_TOP_RST_MASK0 (0x40000+(0x124a<<2))//0x44928
//Bit 31:26     reserved
//Bit 25:24     reg_disp_reg_rst_mask       //unsigned, RW, default = 2'h0
//Bit 23:20     reserved
//Bit 19:4      reg_proc_reg_rst_mask       //unsigned, RW, default = 16'h0
//Bit 3:1       reserved
//Bit 0         reg_vtop_reg_rst_mask       //unsigned, RW, default = 0

#define VPU_TOP_RST_MASK1 (0x40000+(0x124b<<2))//0x4492c
//Bit 31:26     reserved
//Bit 25:24     reg_disp_ro_rst_mask        //unsigned, RW, default = 2'h0
//Bit 23:20     reserved
//Bit 19:4      reg_proc_ro_rst_mask        //unsigned, RW, default = 16'h0
//Bit 3:1       reserved
//Bit 0         reg_vtop_ro_rst_mask        //unsigned, RW, default = 0

#define VPU_TOP_BLANK_PHS_CTRL (0x40000+(0x124c<<2))//0x44930
//Bit 31:1      reserved
//Bit 0         pls_blank_phs_clr           //unsigned, W1T, default = 0

#define VPU_TOP_BLANK_PHS (0x40000+(0x124d<<2))//0x44934
//Bit 31:1      reserved
//Bit 0         ro_blank_phs                //unsigned, RO, default = 0

#define VPU_TOP_HVS_CTRL (0x40000+(0x124e<<2))//0x44938
//Bit 31:9      reserved
//Bit 8         reg_vs_sel                  //unsigned, RW, default = 0
//Bit 7:6       reserved
//Bit 5:0       reg_hvs_rvs                 //unsigned, RW, default = 0

#define VPU_TOP_DBG_SEL (0x40000+(0x124f<<2))//0x4493c
//Bit 31:3      reserved
//Bit 2:0       reg_dbg_sel                 //unsigned, RW, default = 0

#define VPU_TOP_INTR_CNT (0x40000+(0x1250<<2))//0x44940
//Bit 31:28     reserved
//Bit 27:0      reg_intr_pls_cnt            //unsigned, RW, default = 0

#define VPU_TOP_FSR_FRM (0x40000+(0x1251<<2))//0x44944
//Bit 31:15     reserved
//Bit 14:12     reg_fst_frm_sel_latch_en    //unsigned, RW, default = 7
//Bit 11        reserved
//Bit 10:8      reg_fst_frm_latch_en        //unsigned, RW, default = 7
//Bit 7         reserved
//Bit 6:4       reg_fst_frm                 //unsigned, RW, default = 7
//Bit 3         reserved
//Bit 2:0       reg_fst_frm_sel             //unsigned, RW, default = 0

#define VPU_RO_DE_ERR (0x40000+(0x1252<<2))//0x44948
//Bit 31:1      reserved                   
//Bit 0         ro_vpu_de_err               //unsigned, RO, default = 0

#define VPU_ERR_CLR (0x40000+(0x1253<<2))//0x4494c
//Bit 31:1      reserved
//Bit 0         pls_de_err_clr              //unsigned, W1T, default = 0

#define VPU_FSR_FRM (0x40000+(0x1254<<2))//0x44950
//Bit 31:3      reserved
//Bit 2:0       ro_fst_frm                  //unsigned, RO, default = 7

#define VPU_TOP_DIN_CNT (0x40000+(0x1255<<2))//0x44954
//Bit 31:29     reserved
//Bit 28:16     ro_din_hcnt                 //unsigned, RO, default = 0
//Bit 15:13     reserved
//Bit 12:0      ro_din_vcnt                 //unsigned, RO, default = 0

#define VPU_TOP_DOUT_CNT (0x40000+(0x1256<<2))//0x44958
//Bit 31:29     reserved
//Bit 28:16     ro_dout_hcnt               //unsigned, RO, default = 0
//Bit 15:13     reserved
//Bit 12:0      ro_dout_vcnt               //unsigned, RO, default = 0

#define VPU_AXI_RD_CTRL (0x40000+(0x1257<<2))//0x4495c
//Bit 31:25     reserved
//Bit 24        reg_axi_rd_int_mode        //unsigned, RW, default = 0
//Bit 23:22     reserved
//Bit 21:0      reg_axi_rd_prot_ctrl       //unsigned, RW, default = 0

#define VPU_AXI_WR_CTRL (0x40000+(0x1258<<2))//0x44960
//Bit 31:25     reserved
//Bit 24        reg_axi_wr_int_mode        //unsigned, RW, default = 0
//Bit 23:22     reserved
//Bit 21:0      reg_axi_wr_prot_ctrl       //unsigned, RW, default = 0

#define VPU_AXI_CRASH (0x40000+(0x1259<<2))//0x44964
//Bit 31:29     reserved
//Bit 28        ro_axi_crash_int           //unsigned, RO, default = 0
//Bit 27:20     ro_axi_wr_crash_num        //unsigned, RO, default = 0
//Bit 19:16     ro_axi_wr_crash_id         //unsigned, RO, default = 0
//Bit 15:14     reserved                   
//Bit 13:4      ro_axi_rd_crash_num        //unsigned, RO, default = 0
//Bit 3:0       ro_axi_rd_crash_id         //unsigned, RO, default = 0       

#define VPU_RO_HBLANK_NUM (0x40000+(0x125a<<2))//0x44968
//Bit 31:13     reserved
//Bit 12:0      ro_hblank_num             //unsigned, RO, default = 0

#define VPU_BLANK_NUM_CLR (0x40000+(0x125b<<2))//0x4496c
//Bit 31:1      reserved
//Bit 0         pls_hblank_num_clr        //unsigned, W1T, default = 0

#define VPU_PROC_SW_RESET (0x40000+(0x125c<<2))//0x44970
//Bit 31:11     reserved
//Bit 10:0      pls_proc_sw_rst           //unsigned, W1T, default = 0

#define VPU_DISP_SW_RESET (0x40000+(0x125d<<2))//0x44974
//Bit 31:2      reserved
//Bit 1:0       pls_disp_sw_rst           //unsigned, W1T, default = 0

#define VPU_APB_SW_RESET (0x40000+(0x125e<<2))//0x44978
//Bit 31:1      reserved
//Bit 0         pls_apb_sw_rst            //unsigned, W1T, default = 0

#define VPU_AXI_SW_RESET (0x40000+(0x125f<<2))//0x4497c
//Bit 31:4      reserved
//Bit 3:0       pls_axi_sw_rst            //unsigned, W1T, default = 0

#define VPU_PAT_GEN_SW_RESET (0x40000+(0x1260<<2))//0x44980
//Bit 31:1      reserved
//Bit 0         pls_pat_gen_sw_rst        //unsigned, W1T, default = 0

#define VPU_TOP_INT_SEL (0x40000+(0x1261<<2))//0x44984
//Bit 31:1      reserved
//Bit 0         reg_int_sel               //unsigned, RW, default = 0

#define VPU_TOP_RDMA_INTR_MODE (0x40000+(0x1262<<2))//0x44988
//Bit 31:16     reserved
//Bit 15:0      reg_rdma_intr_mode        //unsigned, RW, default = 0

#define VPU_VENC_PROBE_CTRL (0x40000+(0x1263<<2))//0x4498c
//Bit 31:16     reserved
//Bit 15:0      reg_probe_ctrl            //unsigned, RW, default = 0

#define VPU_VENC_PROBE_POS (0x40000+(0x1264<<2))//0x44990
//Bit 31:0      reg_probe_pos             //unsigned, RW, default = 0

#define VPU_VENC_PROBE_SIZE (0x40000+(0x1265<<2))//0x44994
//Bit 31:0      reg_probe_size            //unsigned, RW, default = 0

#define VPU_VENC_PROBE_HL_COL0 (0x40000+(0x1266<<2))//0x44998
//Bit 31:0      reg_hl_color0             //unsigned, RW, default = 0

#define VPU_VENC_PROBE_HL_COL1 (0x40000+(0x1267<<2))//0x4499c
//Bit 31:0      reg_hl_color1             //unsigned, RW, default = 0

#define VPU_VENC_PROBE_STAT0 (0x40000+(0x1268<<2))//0x449a0
//Bit 31:0      ro_probe_dout0            //unsigned, RO, default = 0

#define VPU_VENC_PROBE_STAT1 (0x40000+(0x1269<<2))//0x449a4
//Bit 31:16     reserved
//Bit 15:0      ro_probe_dout1            //unsigned, RO, default = 0

#define VPU_VIDEO_SIZE (0x40000+(0x1270<<2))//0x449c0
//Bit 31:29     reserved
//Bit 28:16     reg_video_hsize           //unsigned, RW, default = 2560
//Bit 15:13     reserved
//Bit 12:0      reg_video_vsize           //unsigned, RW, default = 1440

#define VPU_TOP_BLEND_SIZE (0x40000+(0x1271<<2))//0x449c4
//Bit 31:29     reserved
//Bit 28:16     reg_blend_hsize           //unsigned, RW, default = 2560
//Bit 15:13     reserved
//Bit 12:0      reg_blend_vsize           //unsigned, RW, default = 1440

#define VPU_TOP_FONT_OSD_SIZE (0x40000+(0x1272<<2))//0x449c8
//Bit 31:29     reserved
//Bit 28:16     reg_font_osd_hsize        //unsigned, RW, default = 2560
//Bit 15:13     reserved
//Bit 12:0      reg_font_osd_vsize        //unsigned, RW, default = 1440

#define VPU_TOP_RECT_OSD_SIZE (0x40000+(0x1273<<2))//0x449cc
//Bit 31:29     reserved
//Bit 28:16     reg_rect_osd_hsize        //unsigned, RW, default = 2560
//Bit 15:13     reserved
//Bit 12:0      reg_rect_osd_vsize        //unsigned, RW, default = 1440

#define VPU_TOP_LINE_OSD_SIZE (0x40000+(0x1274<<2))//0x449d0
//Bit 31:29     reserved
//Bit 28:16     reg_line_osd_hsize        //unsigned, RW, default = 2560
//Bit 15:13     reserved
//Bit 12:0      reg_line_osd_vsize        //unsigned, RW, default = 1440

#define VPU_TOP_LINE_CNT_NUM (0x40000+(0x1275<<2))//0x449d4
//Bit 31:13     reserved
//Bit 12:0      reg_line_cnt_num          //unsigned, RW,default = 8

#define VPU_DE_SW_RST (0x40000+(0x1276<<2))//0x449d8
//Bit 31:1      reserved
//Bit 0         pls_de_sw_rst             //unsigned, W1T, default = 0


#define PPS_SCALE_EN (0x40000+(0x1280<<2))//0x44a00
//Bit 31           reg_hvsc_mode             // unsigned ,    RW, default = 0  0: v pre   1:h pre
//Bit 30:20        reserved                           
//Bit 19:16        reg_vsc_nor_rs_bits       // unsigned ,    RW, default = 7  default = 7, normalize right shift bits of vsc
//Bit 15:12        reg_hsc_nor_rs_bits       // unsigned ,    RW, default = 7  default = 7, normalize right shift bits of hsc
//Bit 11:10        reserved                           
//Bit  9           reg_vsc_en                // unsigned ,    RW, default = 0  default	=	0	,vsc_en
//Bit  8           reg_hsc_en                // unsigned ,    RW, default = 0  default	=	0	,hsc_en
//Bit  7: 4        reg_hsc_tap_num           // unsigned ,    RW, default = 6  default	=	0	,horizontal	scaler	bank	length
//Bit  3: 0        reg_vsc_tap_num           // unsigned ,    RW, default = 4  default	=	4	,vertical	scaler	bank	length

#define PPS_VSC_START_PHASE_STEP (0x40000+(0x1281<<2))//0x44a04
//Bit 31:29        reserved                           
//Bit 28           reg_vf_sep_coef_en        // unsigned ,    RW, default = 0  default = 0x0 ,if true, vertical separated coef enable
//Bit 27:24        reg_vsc_integer_part      // unsigned ,    RW, default = 1  default     =     1,vertical     start     phase     step,     (source/dest)*(2^24),integer     part     of     step
//Bit 23: 0        reg_vsc_fraction_part     // unsigned ,    RW, default = 0  default     =     0,vertical     start     phase     step,     (source/dest)*(2^24),fraction     part     of     step

#define PPS_HSC_START_PHASE_STEP (0x40000+(0x1282<<2))//0x44a08
//Bit 31:28        reserved                           
//Bit 27:24        reg_hsc_integer_part      // unsigned ,    RW, default = 1  default     =     1,integer     part     of     step
//Bit 23: 0        reg_hsc_fraction_part     // unsigned ,    RW, default = 0  default     =     0,fraction     part     of     step

#define PPS_VSC_INIT (0x40000+(0x1283<<2))//0x44a0c
//Bit 31:21        reserved                           
//Bit 20:16        reg_vsc_ini_integer       // signed ,    RW, default = -1  = 0
//Bit 15: 0        reg_vsc_ini_phase         // unsigned ,    RW, default = 0  = 0

#define PPS_HSC_INIT (0x40000+(0x1284<<2))//0x44a10
//Bit 31:21        reserved                           
//Bit 20:16        reg_hsc_ini_integer       // signed ,    RW, default = -1  = 0
//Bit 15: 0        reg_hsc_ini_phase         // unsigned ,    RW, default = 0  = 0

#define PPS_SCALE_OUT_SIZE (0x40000+(0x1285<<2))//0x44a14
//Bit 31:29        reserved   
//Bit 28:16        reg_pps_out_hsize         // unsigned ,    RW, default = 0   pps out site end
//Bit 15:13        reserved            
//Bit 12: 0        reg_pps_out_vsize         // unsigned ,    RW, default = 0   pps out site bgn

#define PPS_SCALE_CTRL (0x40000+(0x1286<<2))//0x44a18
//Bit 31: 5        reserved 
//Bit  4           reg_padding_en            // unsigned ,    RW, default = 0  0: closed, 1: padding mode open
//Bit  3: 1        reserved 
//Bit  0           reg_scale_en              // unsigned ,    RW, default = 0

#define PPS_SCALE_PAD_DATA (0x40000+(0x1287<<2))//0x44a1c
//Bit 31:30        reserved 
//Bit 29: 0        reg_padding_data         // unsigned ,    RW, default = 0   

#define PPS_SCALE_PAD_IN_VWIN (0x40000+(0x1288<<2))//0x44a20
//Bit 31:29        reserved   
//Bit 28:16        reg_pad_in_vend           // unsigned ,    RW, default = 0   pps out site end
//Bit 15:13        reserved            
//Bit 12: 0        reg_pad_in_vbgn           // unsigned ,    RW, default = 0   pps out site bgn

#define PPS_SCALE_PAD_IN_HWIN (0x40000+(0x1289<<2))//0x44a24
//Bit 31:29        reserved   
//Bit 28:16        reg_pad_in_hend           // unsigned ,    RW, default = 0   pps out site end
//Bit 15:13        reserved            
//Bit 12: 0        reg_pad_in_hbgn           // unsigned ,    RW, default = 0   pps out site bgn

#define PPS_SCALE_ERR_CLR (0x40000+(0x128a<<2))//0x44a28
//Bit 31: 1        reserved
//Bit     0        pls_err_clr               //unsigned, W1T,default = 0

#define PPS_SCALE_ERR_RO_0 (0x40000+(0x128b<<2))//0x44a2c
//Bit 31: 0        ro_pps_dbg0               //unsigned, RO, default = 0

#define PPS_SCALE_ERR_RO_1 (0x40000+(0x128c<<2))//0x44a30
//Bit 31: 7        reserved
//Bit  6: 0        ro_pps_dbg1               //unsigned, RO, default = 0

#define PPS_SCALE_COEF_IDX (0x40000+(0x1290<<2))//0x44a40
//Bit 31           reg_coef_s11_mode         // unsigned ,    RW, default = 0  sc coef bit-width 0:s9, 1:s11                  
//Bit 30:20        reserved                           
//Bit 19:17        reg_type_index_ext        // unsigned ,    RW, default = 0  default = 0x0 ,type of index, 000: vertical coef, 001: vertical chroma coef: 010: horizontal coef part A, 011: horizontal coef part B, 100: horizontal chroma coef part A, 101: horizontal chroma coef part B
//Bit 16           reg_ctype_ext_mode        // unsigned ,    RW, default = 0  default = 0x0 , if true use type_index_ext rather than reg_type_index
//Bit 15           reg_index_inc             // unsigned ,    RW, default = 0  default	= 0x0 ,default = 0x0 ,index increment, if bit9 == 1  then (0: index increase 1, 1: index increase 2) else (index increase 2)
//Bit 14           reg_rd_cbus_coef_en       // unsigned ,    RW, default = 0  default = 0x0 ,1: read coef through cbus enable, just for debug purpose in case when we wanna check the coef in ram in correct or not
//Bit 13:10        reserved                           
//Bit  9           reg_high_reso_en          // unsigned ,    RW, default = 0  default	= 0x0 ,default = 0x0 ,if true, use 9bit resolution coef, other use 8bit resolution coef
//Bit  8: 7        reg_type_index            // unsigned ,    RW, default = 0  default	= 0x0 ,default = 0x0 ,type of index, 00: vertical coef, 01: vertical chroma coef: 10: horizontal coef, 11: resevered
//Bit  6: 0        reg_coef_index            // unsigned ,    RW, default = 0  default	= 0x0 ,coef	index

#define PPS_SCALE_COEF (0x40000+(0x1291<<2))//0x44a44
//Bit 31:16        reg_coef_data_1           // unsigned, RW, default = 0
//Bit 15:0         reg_coef_data_0           // unsigned, RW, default = 0


#define HDR2_CTRL (0x40000+(0x1400<<2))//0x45000
//Bit 31:25        reserved                           
//Bit 24           reg_ergb_sel_mode         // unsigned ,    RW, default = 0  hist input source select, 0: input rgb 1: output rgb
//Bit 23:21        reserved                           
//Bit 20:18        reg_din_swap              // unsigned ,    RW, default = 0  reg_din_swap, hw reg
//Bit 17           reg_out_rgb               // unsigned ,    RW, default = 1  reg_out_fmt
//Bit 16           reg_only_mat              // unsigned ,    RW, default = 0  reg_only_mat, hw reg    
//Bit 15:14        reserved                  
//Bit 13           reg_hdr2_top_en           // unsigned ,    RW, default = 0  hdr2 top enable, hw reg    
//Bit 12           reg_c_gain_mode           // unsigned ,    RW, default = 1  
//Bit 11: 8        reserved                           
//Bit  7: 6        reg_gmut_mode             // unsigned ,    RW, default = 1  gmut mode
//Bit  5           reg_in_shift              // unsigned ,    RW, default = 0  0: use input U10 process,IE_BW=10; 1: use input u12 process
//Bit  4           reg_in_fmt                // unsigned ,    RW, default = 1  20180719,input already RGB(2020),no need trans
//Bit  3           reg_eo_enable             // unsigned ,    RW, default = 1  
//Bit  2           reg_oe_enable             // unsigned ,    RW, default = 1  
//Bit  1           reg_ogain_enable          // unsigned ,    RW, default = 1  
//Bit  0           reg_cgain_enable          // unsigned ,    RW, default = 0  

#define HDR2_CLK_GATE (0x40000+(0x1401<<2))//0x45004
//Bit 31: 0        reg_gclk_ctrl             // unsigned ,    RW, default = 0  

#define HDR2_MATRIXI_COEF00_01 (0x40000+(0x1402<<2))//0x45008
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef00        // signed ,    RW, default = 1023  reg_mtrxi_coef_00
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef01        // signed ,    RW, default = 0  reg_mtrxi_coef_01

#define HDR2_MATRIXI_COEF02_10 (0x40000+(0x1403<<2))//0x4500c
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef02        // signed ,    RW, default = 1510  reg_mtrxi_coef_02
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef10        // signed ,    RW, default = 1023  reg_mtrxi_coef_10

#define HDR2_MATRIXI_COEF11_12 (0x40000+(0x1404<<2))//0x45010
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef11        // signed ,    RW, default = -168  reg_mtrxi_coef_11
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef12        // signed ,    RW, default = -585  reg_mtrxi_coef_12

#define HDR2_MATRIXI_COEF20_21 (0x40000+(0x1405<<2))//0x45014
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef20        // signed ,    RW, default = 1023  reg_mtrxi_coef_20
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef21        // signed ,    RW, default = 1926  reg_mtrxi_coef_21

#define HDR2_MATRIXI_COEF22 (0x40000+(0x1406<<2))//0x45018
//Bit 31:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef22        // signed ,    RW, default = 0  reg_mtrxi_coef_22

#define HDR2_MATRIXI_COEF30_31 (0x40000+(0x1407<<2))//0x4501c
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef30        // signed ,    RW, default = 0  reg_mtrxi_coef_30
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef31        // signed ,    RW, default = 0  reg_mtrxi_coef_31

#define HDR2_MATRIXI_COEF32_40 (0x40000+(0x1408<<2))//0x45020
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef32        // signed ,    RW, default = 0  reg_mtrxi_coef_32
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef40        // signed ,    RW, default = 0  reg_mtrxi_coef_40

#define HDR2_MATRIXI_COEF41_42 (0x40000+(0x1409<<2))//0x45024
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef41        // signed ,    RW, default = 0  reg_mtrxi_coef_41
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef42        // signed ,    RW, default = 0  reg_mtrxi_coef_42

#define HDR2_MATRIXI_OFFSET0_1 (0x40000+(0x140a<<2))//0x45028
//Bit 31:27        reserved                           
//Bit 26:16        reg_mtrxi_offst_oup0     // signed ,    RW, default = 0  reg_mtrxi_offst_oup_0
//Bit 15:11        reserved                           
//Bit 10: 0        reg_mtrxi_offst_oup1     // signed ,    RW, default = 0  reg_mtrxi_offst_oup_1

#define HDR2_MATRIXI_OFFSET2 (0x40000+(0x140b<<2))//0x4502c
//Bit 31:11        reserved                           
//Bit 10: 0        reg_mtrxi_offst_oup2     // signed ,    RW, default = 0  reg_mtrxi_offst_oup_2

#define HDR2_MATRIXI_PRE_OFFSET0_1 (0x40000+(0x140c<<2))//0x45030
//Bit 31:27        reserved                           
//Bit 26:16        reg_mtrxi_offst_inp0     // signed ,    RW, default = 0  
//Bit 15:11        reserved                           
//Bit 10: 0        reg_mtrxi_offst_inp1     // signed ,    RW, default = -512  

#define HDR2_MATRIXI_PRE_OFFSET2 (0x40000+(0x140d<<2))//0x45034
//Bit 31:11        reserved                           
//Bit 10: 0        reg_mtrxi_offst_inp2     // signed ,    RW, default = -512  

#define HDR2_MATRIXO_COEF00_01 (0x40000+(0x140e<<2))//0x45038
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxo_coef00        // signed ,    RW, default = 218  
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxo_coef01        // signed ,    RW, default = 732  

#define HDR2_MATRIXO_COEF02_10 (0x40000+(0x140f<<2))//0x4503c
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxo_coef02        // signed ,    RW, default = 74  
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxo_coef10        // signed ,    RW, default = -117  

#define HDR2_MATRIXO_COEF11_12 (0x40000+(0x1410<<2))//0x45040
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxo_coef11        // signed ,    RW, default = -395  
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxo_coef12        // signed ,    RW, default = 512  

#define HDR2_MATRIXO_COEF20_21 (0x40000+(0x1411<<2))//0x45044
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxo_coef20        // signed ,    RW, default = 512  
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxo_coef21        // signed ,    RW, default = -465  

#define HDR2_MATRIXO_COEF22 (0x40000+(0x1412<<2))//0x45048
//Bit 31:13        reserved                           
//Bit 12: 0        reg_mtrxo_coef22        // signed ,    RW, default = -47  

#define HDR2_MATRIXO_COEF30_31 (0x40000+(0x1413<<2))//0x4504c
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxo_coef30        // signed ,    RW, default = 0  
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxo_coef31        // signed ,    RW, default = 0  

#define HDR2_MATRIXO_COEF32_40 (0x40000+(0x1414<<2))//0x45050
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxo_coef32        // signed ,    RW, default = 0  
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxo_coef40        // signed ,    RW, default = 0  

#define HDR2_MATRIXO_COEF41_42 (0x40000+(0x1415<<2))//0x45054
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxo_coef41        // signed ,    RW, default = 0  
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxo_coef42        // signed ,    RW, default = 0  

#define HDR2_MATRIXO_OFFSET0_1 (0x40000+(0x1416<<2))//0x45058
//Bit 31:27        reserved                           
//Bit 26:16        reg_mtrxo_offst_oup0     // signed ,    RW, default = 0  reg_mtrxo_offst_oup_0
//Bit 15:11        reserved                           
//Bit 10: 0        reg_mtrxo_offst_oup1     // signed ,    RW, default = 512  reg_mtrxo_offst_oup_1

#define HDR2_MATRIXO_OFFSET2 (0x40000+(0x1417<<2))//0x4505c
//Bit 31:11        reserved                           
//Bit 10: 0        reg_mtrxo_offst_oup2     // signed ,    RW, default = 512  reg_mtrxo_offst_oup_2

#define HDR2_MATRIXO_PRE_OFFSET0_1 (0x40000+(0x1418<<2))//0x45060
//Bit 31:27        reserved                           
//Bit 26:16        reg_mtrxo_offst_inp0     // signed ,    RW, default = 0  reg_mtrxo_offst_inp_0
//Bit 15:11        reserved                           
//Bit 10: 0        reg_mtrxo_offst_inp1     // signed ,    RW, default = 0  reg_mtrxo_offst_inp_1

#define HDR2_MATRIXO_PRE_OFFSET2 (0x40000+(0x1419<<2))//0x45064
//Bit 31:11        reserved                           
//Bit 10: 0        reg_mtrxo_offst_inp2     // signed ,    RW, default = 0  reg_mtrxo_offst_inp_2

#define HDR2_MATRIXI_CLIP (0x40000+(0x141a<<2))//0x45068
//Bit 31:20        reserved                           
//Bit 19: 8        reg_mtrxi_comp_thrd       // unsigned ,    RW, default = 0  
//Bit  7: 5        reg_mtrxi_rs              // signed ,    RW, default = 0  of the coef: -2: norm to 256; -1: norm to 512; 0: normalized to 1024 as 1; 1: norm to 2048; 2: norm to 4096;
//Bit  4: 3        reg_mtrxi_clmod           // unsigned ,    RW, default = 0  luma mode of BT2020: 0: non-constant luma or none BT2020;  1: BT2020 CL Y'R'B'=>Y'Cb'Cr' (B'/R'-Y') sign; 2: BT2020 CL: Y'Cb'Cr' =>Y;R'B', Cb'Cr' sign; 3: BT2020 CL: Y'Cb'Cr' =>Y;R'B', Cb'Cr'- 2^(BL-1) sign;
//Bit  2: 0        reserved                           

#define HDR2_MATRIXO_CLIP (0x40000+(0x141b<<2))//0x4506c
//Bit 31:20        reserved                           
//Bit 19: 8        reg_mtrxo_comp_thrd       // unsigned ,    RW, default = 0  
//Bit  7: 5        reg_mtrxo_rs              // signed ,    RW, default = 0  of the coef: -2: norm to 256; -1: norm to 512; 0: normalized to 1024 as 1; 1: norm to 2048; 2: norm to 4096;
//Bit  4: 3        reg_mtrxo_clmod           // unsigned ,    RW, default = 0  luma mode of BT2020: 0: non-constant luma or none BT2020;  1: BT2020 CL Y'R'B'=>Y'Cb'Cr' (B'/R'-Y') sign; 2: BT2020 CL: Y'Cb'Cr' =>Y;R'B', Cb'Cr' sign; 3: BT2020 CL: Y'Cb'Cr' =>Y;R'B', Cb'Cr'- 2^(BL-1) sign;
//Bit  2: 0        reserved                           

#define HDR2_CGAIN_OFFT (0x40000+(0x141c<<2))//0x45070
//Bit 31:27        reserved                           
//Bit 26:16        reg_cgain_oft2            // unsigned ,    RW, default = 512  hw reg
//Bit 15:11        reserved                           
//Bit 10: 0        reg_cgain_oft1            // unsigned ,    RW, default = 512  hw reg

#define HDR2_HIST_RD (0x40000+(0x141d<<2))//0x45074
//Bit 31:24        reserved                           
//Bit 23: 0        reg_hist_status           // unsigned ,    RW, default = 0  

#define EOTF_LUT_ADDR_PORT (0x40000+(0x141e<<2))//0x45078

#define EOTF_LUT_DATA_PORT (0x40000+(0x141f<<2))//0x4507c

#define OETF_LUT_ADDR_PORT (0x40000+(0x1420<<2))//0x45080

#define OETF_LUT_DATA_PORT (0x40000+(0x1421<<2))//0x45084

#define CGAIN_LUT_ADDR_PORT (0x40000+(0x1422<<2))//0x45088

#define CGAIN_LUT_DATA_PORT (0x40000+(0x1423<<2))//0x4508c

#define HDR2_CGAIN_COEF0 (0x40000+(0x1424<<2))//0x45090
//Bit 31:28        reserved                           
//Bit 27:16        reg_coef1                 // unsigned ,    RW, default = 2376  reg_c_gain_lim_coef1
//Bit 15:12        reserved                           
//Bit 11: 0        reg_coef0                 // unsigned ,    RW, default = 920  reg_c_gain_lim_coef0

#define HDR2_CGAIN_COEF1 (0x40000+(0x1425<<2))//0x45094
//Bit 31           reg_sel_opt               // unsigned ,    RW, default = 1  
//Bit 30:28        reserved                           
//Bit 27:16        reg_maxrgb                // unsigned ,    RW, default = 1023  max rgb in nolinear domain
//Bit 15:12        reserved                           
//Bit 11: 0        reg_c_gain_lim_coef_2     // unsigned ,    RW, default = 208  reg_c_gain_lim_coef2

#define OGAIN_LUT_ADDR_PORT (0x40000+(0x1426<<2))//0x45098

#define OGAIN_LUT_DATA_PORT (0x40000+(0x1427<<2))//0x4509c

#define HDR2_ADPS_CTRL (0x40000+(0x1428<<2))//0x450a0
//Bit 31:24        reserved                           
//Bit 23:20        reg_adpscl1_sft           // unsigned ,    RW, default = 5  adpscl shift
//Bit 19:18        reserved                           
//Bit 17           reg_ogain_blend_mode      // unsigned ,    RW, default = 0  adpscl and adpscl1 result blending or not
//Bit 16           reg_adpscl_sel_opt        // unsigned ,    RW, default = 1  hw reg
//Bit 15:14        reserved                           
//Bit 13: 8        reg_adpscl_max            // unsigned ,    RW, default = 24  
//Bit  7           reg_adpscl_clip_en        // unsigned ,    RW, default = 0  clip enable
//Bit  6           reg_adpscl_bypass2        // unsigned ,    RW, default = 0  enable of adaptive scaling on linear RGB based on Ys, [0] for R, [1] for G and [2] for B;
//Bit  5           reg_adpscl_bypass1        // unsigned ,    RW, default = 0  enable of adaptive scaling on linear RGB based on Ys, [0] for R, [1] for G and [2] for B;
//Bit  4           reg_adpscl_bypass0        // unsigned ,    RW, default = 0  enable of adaptive scaling on linear RGB based on Ys, [0] for R, [1] for G and [2] for B;
//Bit  3: 2        reg_adpscl1_mode          // unsigned ,    RW, default = 1  0, nolinear input, 1, max linear, 2, adpscl mode
//Bit  1: 0        reg_adpscl_mode           // unsigned ,    RW, default = 1  0, nolinear input, 1, max linear, 2, adpscl mode

#define HDR2_ADPS_ALPHA0 (0x40000+(0x1429<<2))//0x450a4
//Bit 31:30        reserved                           
//Bit 29:16        reg_adpscl_alpha1        // unsigned ,    RW, default = 1024  gain(contrast) to linear RGB channel with 1/12 factor, normalized to 1024 as "1" , [0] for R, [1] for G and [2] for B;
//Bit 15:14        reserved                           
//Bit 13: 0        reg_adpscl_alpha0        // unsigned ,    RW, default = 1024  gain(contrast) to linear RGB channel with 1/12 factor, normalized to 1024 as "1" , [0] for R, [1] for G and [2] for B;

#define HDR2_ADPS_ALPHA1 (0x40000+(0x142a<<2))//0x450a8
//Bit 31:28        reg_adpscl_shift0         // unsigned ,    RW, default = 12  
//Bit 27:25        reserved                           
//Bit 24:20        reg_adpscl_shift1         // unsigned ,    RW, default = 14  
//Bit 19:16        reg_adpscl_shift2         // unsigned ,    RW, default = 12  hw no use
//Bit 15:14        reserved                           
//Bit 13: 0        reg_adpscl_alpha2        // unsigned ,    RW, default = 1024  gain(contrast) to linear RGB channel with 1/12 factor, normalized to 1024 as "1" , [0] for R, [1] for G and [2] for B;

#define HDR2_ADPS_BETA0 (0x40000+(0x142b<<2))//0x450ac
//Bit 31:21        reserved                           
//Bit 20: 0        reg_adpscl_beta0         // unsigned ,    RW, default = 0  offset(brightness) to linear RGB channel,  [0] for R, [1] for G and [2] for B;

#define HDR2_ADPS_BETA1 (0x40000+(0x142c<<2))//0x450b0
//Bit 31:21        reserved                           
//Bit 20: 0        reg_adpscl_beta1         // unsigned ,    RW, default = 0  offset(brightness) to linear RGB channel,  [0] for R, [1] for G and [2] for B;

#define HDR2_ADPS_BETA2 (0x40000+(0x142d<<2))//0x450b4
//Bit 31:21        reserved                           
//Bit 20: 0        reg_adpscl_beta2         // unsigned ,    RW, default = 0  offset(brightness) to linear RGB channel,  [0] for R, [1] for G and [2] for B;

#define HDR2_ADPS_COEF0 (0x40000+(0x142e<<2))//0x450b8
//Bit 31:28        reserved                           
//Bit 27:16        reg_adpscl_ys_coef1      // unsigned ,    RW, default = 1024  coef to calculate the Ys, normalized to 2048 as "1", leave one bit margin;
//Bit 15:12        reserved                           
//Bit 11: 0        reg_adpscl_ys_coef0      // unsigned ,    RW, default = 1024  coef to calculate the Ys, normalized to 2048 as "1", leave one bit margin;

#define HDR2_ADPS_COEF1 (0x40000+(0x142f<<2))//0x450bc
//Bit 31:12        reserved                           
//Bit 11: 0        reg_adpscl_ys_coef2      // unsigned ,    RW, default = 1024  coef to calculate the Ys, normalized to 2048 as "1", leave one bit margin;

#define HDR2_GMUT_CTRL (0x40000+(0x1430<<2))//0x450c0
//Bit 31: 5        reserved                           
//Bit  4           reg_new_mode              // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_gmut_shift            // unsigned ,    RW, default = 8  mult shift  means, if shift = 14, coef = 0x4000; gain = 1

#define HDR2_GMUT_COEF0 (0x40000+(0x1431<<2))//0x450c4
//Bit 31:16        reg_gmut_coef01         // signed ,    RW, default = -150  
//Bit 15: 0        reg_gmut_coef00         // signed ,    RW, default = 425  

#define HDR2_GMUT_COEF1 (0x40000+(0x1432<<2))//0x450c8
//Bit 31:16        reg_gmut_coef10         // signed ,    RW, default = -31  
//Bit 15: 0        reg_gmut_coef02         // signed ,    RW, default = -18  

#define HDR2_GMUT_COEF2 (0x40000+(0x1433<<2))//0x450cc
//Bit 31:16        reg_gmut_coef12         // signed ,    RW, default = -2  
//Bit 15: 0        reg_gmut_coef11         // signed ,    RW, default = 290  

#define HDR2_GMUT_COEF3 (0x40000+(0x1434<<2))//0x450d0
//Bit 31:16        reg_gmut_coef21         // signed ,    RW, default = -25  
//Bit 15: 0        reg_gmut_coef20         // signed ,    RW, default = -5  

#define HDR2_GMUT_COEF4 (0x40000+(0x1435<<2))//0x450d4
//Bit 31:16        reserved                           
//Bit 15: 0        reg_gmut_coef_2_2         // signed ,    RW, default = 286  

#define HDR2_PIPE_CTRL1 (0x40000+(0x1436<<2))//0x450d8
//Bit 31:24        reg_vblank_num_oetf       // unsigned ,    RW, default = 4  
//Bit 23:16        reg_hblank_num_oetf       // unsigned ,    RW, default = 4  
//Bit 15: 8        reg_vblank_num_eotf       // unsigned ,    RW, default = 10  
//Bit  7: 0        reg_hblank_num_eotf       // unsigned ,    RW, default = 10  

#define HDR2_PIPE_CTRL2 (0x40000+(0x1437<<2))//0x450dc
//Bit 31:24        reg_vblank_num_cgain      // unsigned ,    RW, default = 10  
//Bit 23:16        reg_hblank_num_cgain      // unsigned ,    RW, default = 10  
//Bit 15: 8        reg_vblank_num_gmut       // unsigned ,    RW, default = 17  
//Bit  7: 0        reg_hblank_num_gmut       // unsigned ,    RW, default = 17  

#define HDR2_PIPE_CTRL3 (0x40000+(0x1438<<2))//0x450e0
//Bit 31:24        reg_vblank_num_adps       // unsigned ,    RW, default = 22  
//Bit 23:16        reg_hblank_num_adps       // unsigned ,    RW, default = 22  
//Bit 15: 8        reg_vblank_num_uv         // unsigned ,    RW, default = 4  
//Bit  7: 0        reg_hblank_num_uv         // unsigned ,    RW, default = 4  

#define HDR2_PROC_WIN1 (0x40000+(0x1439<<2))//0x450e4
//Bit 31           reg_proc_win_gmut_en      // unsigned ,    RW, default = 0  hw reg
//Bit 30           reg_proc_win_adps_en      // unsigned ,    RW, default = 0  hw reg
//Bit 29           reg_proc_win_cgain_en     // unsigned ,    RW, default = 0  hw reg
//Bit 28:16        reg_proc_x_ed             // unsigned ,    RW, default = 99  
//Bit 15:13        reserved                           
//Bit 12: 0        reg_proc_x_st             // unsigned ,    RW, default = 0  

#define HDR2_PROC_WIN2 (0x40000+(0x143a<<2))//0x450e8
//Bit 31:30        reserved                           
//Bit 29           reg_proc_win_aicr_en      // unsigned ,    RW, default = 1  hw reg
//Bit 28:16        reg_proc_y_ed             // unsigned ,    RW, default = 99  
//Bit 15:13        reserved                           
//Bit 12: 0        reg_proc_y_st             // unsigned ,    RW, default = 0  

#define HDR2_MATRIXI_EN_CTRL (0x40000+(0x143b<<2))//0x450ec
//Bit 31: 8        reserved                           
//Bit  7: 0        reg_matrixi_en_ctrl       // unsigned ,    RW, default = 0  

#define HDR2_MATRIXO_EN_CTRL (0x40000+(0x143c<<2))//0x450f0
//Bit 31: 8        reserved                           
//Bit  7: 0        reg_matrixo_en_ctrl       // unsigned ,    RW, default = 0  

#define HDR2_HIST_CTRL (0x40000+(0x143d<<2))//0x450f4
//Bit 31:25        reserved                           
//Bit 24:17        reg_vcbus_rd_idx          // unsigned ,    RW, default = 0  hw reg
//Bit 16           reg_hist_enable           // unsigned ,    RW, default = 0  hw reg    
//Bit 15: 8        reg_gclk_ctrl0            // unsigned ,    RW, default = 20  hw reg   
//Bit  7: 6        reserved                           
//Bit  5           reg_piecewise_mode        // unsigned ,    RW, default = 0  hist statistic mode select; 1 for piecewise,0 for equal step
//Bit  4           reg_hist_win_mode         // unsigned ,    RW, default = 1  1 for doing hist statistic processing inside window; 0 for outside window
//Bit  3           reg_maxRGB_rshift         // unsigned ,    RW, default = 0  o domain data (mainly sdr) shift
//Bit  2: 0        reg_maxRGB_sel            // unsigned ,    RW, default = 0  hist statistic data select 0: non-linear MAX(R',G',B') before EOTF 2: non-linear Y' before EOTF 3:non-linear sat before EOTF; 4:   linear MAX(R,G,B) after EOTF, default 5~7:  linear Y after EOTF

#define HDR2_HIST_H_START_END (0x40000+(0x143e<<2))//0x450f8
//Bit 31:29        reserved                           
//Bit 28:16        reg_hist_proc_x_st        // unsigned ,    RW, default = 0  hist proc start x
//Bit 15:13        reserved                           
//Bit 12: 0        reg_hist_proc_x_ed        // unsigned ,    RW, default = 99  hist proc end x

#define HDR2_HIST_V_START_END (0x40000+(0x143f<<2))//0x450fc
//Bit 31:29        reserved                           
//Bit 28:16        reg_hist_proc_y_st        // unsigned ,    RW, default = 0  hist proc start y
//Bit 15:13        reserved                           
//Bit 12: 0        reg_hist_proc_y_ed        // unsigned ,    RW, default = 99  hist proc end y

#define HDR2_GMUT_COMP0 (0x40000+(0x1440<<2))//0x45100
//Bit 31           reg_hdr2_gm_comp_en       // unsigned ,    RW, default = 1  
//Bit 30:28        reserved                           
//Bit 27: 8        reg_hdr_comp_ofst_r       // unsigned ,    RW, default = 85900  
//Bit  7: 0        reserved                           

#define HDR2_GMUT_COMP1 (0x40000+(0x1441<<2))//0x45104
//Bit 31:28        reserved                           
//Bit 27: 8        reg_hdr_comp_ofst_g       // unsigned ,    RW, default = 85900  
//Bit  7: 0        reserved                           

#define HDR2_GMUT_COMP2 (0x40000+(0x1442<<2))//0x45108
//Bit 31:28        reserved                           
//Bit 27: 8        reg_hdr_comp_ofst_b       // unsigned ,    RW, default = 85900  
//Bit  7: 0        reserved                           

#define HDR2_GMUT_COMP3 (0x40000+(0x1443<<2))//0x4510c
//Bit 31:28        reserved                           
//Bit 27: 8        reg_hdr_comp_min_r        // unsigned ,    RW, default = 510025  
//Bit  7: 0        reserved                           

#define HDR2_GMUT_COMP4 (0x40000+(0x1444<<2))//0x45110
//Bit 31:28        reserved                           
//Bit 27: 8        reg_hdr_comp_min_g        // unsigned ,    RW, default = 472965  
//Bit  7: 0        reserved                           

#define HDR2_GMUT_COMP5 (0x40000+(0x1445<<2))//0x45114
//Bit 31:28        reserved                           
//Bit 27: 8        reg_hdr_comp_min_b        // unsigned ,    RW, default = 467019  
//Bit  7: 0        reserved                           

#define HDR2_GMUT_COMP6 (0x40000+(0x1446<<2))//0x45118
//Bit 31:30        reserved                           
//Bit 29: 8        reg_hdr_comp_rat_r        // unsigned ,    RW, default = 152108  
//Bit  7: 0        reserved                           

#define HDR2_GMUT_COMP7 (0x40000+(0x1447<<2))//0x4511c
//Bit 31:30        reserved                           
//Bit 29: 8        reg_hdr_comp_rat_g        // unsigned ,    RW, default = 472965  
//Bit  7: 0        reserved                           

#define HDR2_GMUT_COMP8 (0x40000+(0x1448<<2))//0x45120
//Bit 31:30        reserved                           
//Bit 29: 8        reg_hdr_comp_rat_b        // unsigned ,    RW, default = 467019  
//Bit  7: 0        reserved                           


#define VPU_VADJ1_MISC (0x40000+(0x1640<<2))//0x45900
//Bit 31: 6        reserved                           
//Bit  5: 4        reg_vadj_gclk_ctrl        // unsigned ,    RW, default = 0  hw reg
//Bit     3        reserved                           
//Bit     2        reg_vadj_en_sync_mode     // unsigned ,    RW, default = 0  hw reg
//Bit     1        reg_vadj_minus_black_en   // unsigned ,    RW, default = 0  hw reg
//Bit     0        reg_vadj_enable           // unsigned ,    RW, default = 0  hw reg

#define VPU_VADJ1_BLACK_VAL (0x40000+(0x1641<<2))//0x45904
//Bit 31:27        reserved                           
//Bit 26:16        reg_vadj_chrm_ofset       // signed ,    RW, default = 512  for DC level subtraction before UV hue and saturation adjustment, u10 precision mode, set to 512; u12 precision mode set to 0
//Bit 15:11        reserved                           
//Bit 10: 0        reg_vadj_blk_ofset        // signed ,    RW, default = 64  for black level subtraction before contrast gain, u10 precision mode, set to 64; u12 precision mode set to (-2048+256)

#define VPU_VADJ1_Y (0x40000+(0x1642<<2))//0x45908
//Bit 31:19        reserved                           
//Bit 18: 8        reg_vadj_bri              // signed ,    RW, default = 0  brightness, signed value,[-1024,+1023]
//Bit  7: 0        reg_vadj_con              // unsigned ,    RW, default = 128  contrast, unsigned value, [0,2)

#define VPU_VADJ1_MA_MB (0x40000+(0x1643<<2))//0x4590c
//Bit 31:26        reserved                           
//Bit 25:16        reg_vadj_mabcd_0          // signed ,    RW, default = 256  ma/mb/mc/md for hue/sat adjustment, [-2,2)
//Bit 15:10        reserved                           
//Bit  9: 0        reg_vadj_mabcd_1          // signed ,    RW, default = 0  for hue/sat adjustment, [-2,2)

#define VPU_VADJ1_MC_MD (0x40000+(0x1644<<2))//0x45910
//Bit 31:26        reserved                           
//Bit 25:16        reg_vadj_mabcd_2          // signed ,    RW, default = 0  for hue/sat adjustment, [-2,2)
//Bit 15:10        reserved                           
//Bit  9: 0        reg_vadj_mabcd_3          // signed ,    RW, default = 256  for hue/sat adjustment, [-2,2)

#define VPU_VADJ1_CURV_0 (0x40000+(0x1645<<2))//0x45914
//Bit 31:24        reg_vadj_softcon_curv0_2  // unsigned ,    RW, default = 128  ci0,c=ci>>cs , c= (2^18)/b
//Bit 23:12        reg_vadj_softcon_curv0_1  // unsigned ,    RW, default = 64  b0, curve 0 c parameters
//Bit 11: 0        reg_vadj_softcon_curv0_0  // signed ,    RW, default = 0  a0,curve 0 a parameters

#define VPU_VADJ1_CURV_1 (0x40000+(0x1646<<2))//0x45918
//Bit 31:12        reserved                           
//Bit 11: 3        reg_vadj_softcon_curv0_4  // signed ,    RW, default = -255  g0, curve 0 g parameters for amplitude
//Bit  2: 0        reg_vadj_softcon_curv0_3  // unsigned ,    RW, default = 5  cs0,will be set by software (provide equation)

#define VPU_VADJ1_CURV_2 (0x40000+(0x1647<<2))//0x4591c
//Bit 31:24        reg_vadj_softcon_curv1_2  // unsigned ,    RW, default = 137  ci0,c=ci>>cs , c= (2^18)/b
//Bit 23:12        reg_vadj_softcon_curv1_1  // unsigned ,    RW, default = 480  b0, curve 1 c parameters
//Bit 11: 0        reg_vadj_softcon_curv1_0  // signed ,    RW, default = 576  a0,curve 1 a parameters

#define VPU_VADJ1_CURV_3 (0x40000+(0x1648<<2))//0x45920
//Bit 31:12        reserved                           
//Bit 11: 3        reg_vadj_softcon_curv1_4  // signed ,    RW, default = 255  g0, curve 0 g parameters for amplitude
//Bit  2: 0        reg_vadj_softcon_curv1_3  // unsigned ,    RW, default = 2  cs0,will be set by software (provide equation)

#define VPU_RGB_CTRST (0x40000+(0x1649<<2))//0x45924
//Bit 31:30        reg_rgb_gclk_ctrl         // unsigned ,    RW, default = 0  hw reg
//Bit 29:28        reserved                           
//Bit 27:16        reg_rgb_ctrst             // unsigned ,    RW, default = 1024  
//Bit 15:13        reserved                           
//Bit 12: 2        reg_rgb_ctrst_blklvl      // signed ,    RW, default = 64  
//Bit     1        reg_rgbbst_en             // unsigned ,    RW, default = 0  
//Bit     0        reg_rgb_ctrst_prt         // unsigned ,    RW, default = 1  

#define VPU_RGB_BRGHT (0x40000+(0x164a<<2))//0x45928
//Bit 31:15        reserved                           
//Bit 14: 4        reg_rgb_brght             // signed ,    RW, default = 0  
//Bit  3: 2        reserved                           
//Bit     1        reg_rgb_brght_prt         // unsigned ,    RW, default = 1  
//Bit     0        reg_rgbbst_dlut_x2        // unsigned ,    RW, default = 0  

#define VPU_RGB_DLUT_0_3 (0x40000+(0x164b<<2))//0x4592c
//Bit 31:24        reg_rgb_dlut_0            // unsigned ,    RW, default = 255  
//Bit 23:16        reg_rgb_dlut_1            // unsigned ,    RW, default = 205  
//Bit 15: 8        reg_rgb_dlut_2            // unsigned ,    RW, default = 171  
//Bit  7: 0        reg_rgb_dlut_3            // unsigned ,    RW, default = 147  

#define VPU_RGB_DLUT_4_7 (0x40000+(0x164c<<2))//0x45930
//Bit 31:24        reg_rgb_dlut_4            // unsigned ,    RW, default = 128  
//Bit 23:16        reg_rgb_dlut_5            // unsigned ,    RW, default = 113  
//Bit 15: 8        reg_rgb_dlut_6            // unsigned ,    RW, default = 102  
//Bit  7: 0        reg_rgb_dlut_7            // unsigned ,    RW, default = 93  

#define VPU_RGB_DLUT_8_11 (0x40000+(0x164d<<2))//0x45934
//Bit 31:24        reg_rgb_dlut_8            // unsigned ,    RW, default = 85  
//Bit 23:16        reg_rgb_dlut_9            // unsigned ,    RW, default = 78  
//Bit 15: 8        reg_rgb_dlut_10           // unsigned ,    RW, default = 73  
//Bit  7: 0        reg_rgb_dlut_11           // unsigned ,    RW, default = 68  

#define VPU_MATRIX_SAT (0x40000+(0x164e<<2))//0x45938


#define VPU_VADJ1_CSC_COEF_00_01 (0x40000+(0x1650<<2))//0x45940
//Bit 31:29        reserved                           
//Bit 28:16        reg_csc_coef_0_0          // signed ,    RW, default = 256  
//Bit 15:13        reserved                           
//Bit 12: 0        reg_csc_coef_0_1          // signed ,    RW, default = 0  

#define VPU_VADJ1_CSC_COEF_02_10 (0x40000+(0x1651<<2))//0x45944
//Bit 31:29        reserved                           
//Bit 28:16        reg_csc_coef_0_2          // signed ,    RW, default = 394  
//Bit 15:13        reserved                           
//Bit 12: 0        reg_csc_coef_1_0          // signed ,    RW, default = 256  

#define VPU_VADJ1_CSC_COEF_11_12 (0x40000+(0x1652<<2))//0x45948
//Bit 31:29        reserved                           
//Bit 28:16        reg_csc_coef_1_1          // signed ,    RW, default = -47  
//Bit 15:13        reserved                           
//Bit 12: 0        reg_csc_coef_1_2          // signed ,    RW, default = -118  

#define VPU_VADJ1_CSC_COEF_20_21 (0x40000+(0x1653<<2))//0x4594c
//Bit 31:29        reserved                           
//Bit 28:16        reg_csc_coef_2_0          // signed ,    RW, default = 256  
//Bit 15:13        reserved                           
//Bit 12: 0        reg_csc_coef_2_1          // signed ,    RW, default = 465  

#define VPU_VADJ1_CSC_COEF_22 (0x40000+(0x1654<<2))//0x45950
//Bit 31:19        reserved                           
//Bit 18:16        reg_csc_3x3mtrx_rs        // unsigned ,    RW, default = 0  of the coef: -2: norma 256 -1:normal 512  0:normal 1024  1:normal 2048  2:normal 4096
//Bit 15:13        reserved                           
//Bit 12: 0        reg_csc_coef_2_2          // signed ,    RW, default = 0  

#define VPU_VADJ1_CSC_OFFST_INP_0_1 (0x40000+(0x1655<<2))//0x45954
//Bit 31:27        reserved                           
//Bit 26:16        reg_csc_pre_ofst_0        // signed ,    RW, default = 0  reg_csc_pre_ofst0
//Bit 15:11        reserved                           
//Bit 10: 0        reg_csc_pre_ofst_1        // signed ,    RW, default = -512  reg_csc_pre_ofst1

#define VPU_VADJ1_CSC_OFFST_INP_2 (0x40000+(0x1656<<2))//0x45958
//Bit 31:11        reserved                           
//Bit 10: 0        reg_csc_pre_ofst_2        // signed ,    RW, default = -512  reg_csc_pre_ofst2

#define VPU_VADJ1_CSC_OFFST_OUP_0_1 (0x40000+(0x1657<<2))//0x4595c
//Bit 31:27        reserved                           
//Bit 26:16        reg_csc_pst_ofst_0        // signed ,    RW, default = 0  reg_csc_pst_ofst0
//Bit 15:11        reserved                           
//Bit 10: 0        reg_csc_pst_ofst_1        // signed ,    RW, default = 0  reg_csc_pst_ofst1

#define VPU_VADJ1_CSC_OFFST_OUP_2 (0x40000+(0x1658<<2))//0x45960
//Bit 31:11        reserved                           
//Bit 10: 0        reg_csc_pst_ofst_2        // signed ,    RW, default = 0  reg_csc_pst_ofst2


#define VPP_GAINOFF_CTRL0 (0x40000+(0x1680<<2))//0x45a00
//Bit 31        reg_gainoff_en              //unsigned, RW, default = 0
//Bit 30        reg_gainoff_enable_sel      //unsigned, RW, default = 1
//Bit 29:27     reserved
//Bit 26:16     reg_gainoff_gain_0          //unsigned, RW, default = 1024
//Bit 15:11     reserved
//Bit 10:0      reg_gainoff_gain_1          //unsigned, RW, default = 1024

#define VPP_GAINOFF_CTRL1 (0x40000+(0x1681<<2))//0x45a04
//Bit 31:27     reserved
//Bit 26:16     reg_gainoff_gain_2          //unsigned, RW, default = 1024
//Bit 15:13     reserved
//Bit 12:0      reg_gainoff_offset_0        //unsigned, RW, default = 0

#define VPP_GAINOFF_CTRL2 (0x40000+(0x1682<<2))//0x45a08
//Bit 31:29     reserved
//Bit 28:16     reg_gainoff_offset_1        //unsigned, RW, default = 0
//Bit 15:13     reserved
//Bit 12:0      reg_gainoff_offset_2        //unsigned, RW, default = 0

#define VPP_GAINOFF_CTRL3 (0x40000+(0x1683<<2))//0x45a0c
//Bit 31:29     reserved
//Bit 28:16     reg_gainoff_pre_offset_0    //unsigned, RW, default = 0
//Bit 15:13     reserved
//Bit 12:0      reg_gainoff_pre_offset_1    //unsigned, RW, default = 0

#define VPP_GAINOFF_CTRL4 (0x40000+(0x1684<<2))//0x45a10
//Bit 31:13     reserved
//Bit 12:0      reg_gainoff_pre_offset_2    //unsigned, RW, default = 0

#define VPP_GAINOFF_GCLK_CTRL (0x40000+(0x1685<<2))//0x45a14
//Bit 31:2      reserved
//Bit 1:0       reg_gainoff_gclk_ctrl       //unsigned, RW, default = 0


#define VPP_CHROMA_ADDR_PORT (0x40000+(0x16a0<<2))//0x45a80

#define VPP_CHROMA_DATA_PORT (0x40000+(0x16a1<<2))//0x45a84


#define VPU_PROC_TOP_CTRL (0x40000+(0x1700<<2))//0x45c00
//Bit 31:21     reserved
//Bit 20        reg_crop_en                 //unsigned, RW, default = 0
//Bit 19:9      reserved
//Bit 8         reg_sharp_in_inp_422        //unsigned, RW, default = 0
//Bit 7:0       reg_sharp_debug_mode        //unsigned, RW, default = 0

#define VPU_PROC_BLD_EN (0x40000+(0x1701<<2))//0x45c04
//Bit 31:1      reserved
//Bit 0         reg_blend_din_en            //unsigned, RW, default = 1

#define VPU_PROC_BLD0_CTRL (0x40000+(0x1702<<2))//0x45c08
//Bit 31:5      reserved
//Bit 4         reg_blend0_premult_en       //unsigned, RW, default = 1          
//Bit 3:0       reg_blend0_din_reoder       //unsigned, RW, default = 0

#define VPU_PROC_BLD1_CTRL (0x40000+(0x1703<<2))//0x45c0c
//Bit 31:5      reserved
//Bit 4         reg_blend1_premult_en       //unsigned, RW, default = 0          
//Bit 3:0       reg_blend1_din_reoder       //unsigned, RW, default = 4

#define VPU_PROC_BLD2_CTRL (0x40000+(0x1704<<2))//0x45c10
//Bit 31:5      reserved
//Bit 4         reg_blend2_premult_en       //unsigned, RW, default = 0          
//Bit 3:0       reg_blend2_din_reoder       //unsigned, RW, default = 4

#define VPU_PROC_BLD3_CTRL (0x40000+(0x1705<<2))//0x45c14
//Bit 31:5      reserved
//Bit 4         reg_blend3_premult_en       //unsigned, RW, default = 0          
//Bit 3:0       reg_blend3_din_reoder       //unsigned, RW, default = 4

#define VPU_PROC_BLD_SRC0_HSCOPE (0x40000+(0x1706<<2))//0x45c18
//Bit 31:29     reserved
//Bit 28:16     reg_bld_src0_h_end          //unsigned, RW, default = 2559
//Bit 15:13     reserved                    
//Bit 12:0      reg_bld_src0_h_start        //unsigned, RW, default = 0

#define VPU_PROC_BLD_SRC0_VSCOPE (0x40000+(0x1707<<2))//0x45c1c
//Bit 31:29     reserved
//Bit 28:16     reg_bld_src0_v_end          //unsigned, RW, default = 1439
//Bit 15:13     reserved                    
//Bit 12:0      reg_bld_src0_v_start        //unsigned, RW, default = 0

#define VPU_PROC_BLD_SRC1_HSCOPE (0x40000+(0x1708<<2))//0x45c20
//Bit 31:29     reserved
//Bit 28:16     reg_bld_src1_h_end          //unsigned, RW, default = 2559
//Bit 15:13     reserved                    
//Bit 12:0      reg_bld_src1_h_start        //unsigned, RW, default = 0

#define VPU_PROC_BLD_SRC1_VSCOPE (0x40000+(0x1709<<2))//0x45c24
//Bit 31:29     reserved
//Bit 28:16     reg_bld_src1_v_end          //unsigned, RW, default = 1439
//Bit 15:13     reserved                    
//Bit 12:0      reg_bld_src1_v_start        //unsigned, RW, default = 0

#define VPU_PROC_BLD_SRC2_HSCOPE (0x40000+(0x170a<<2))//0x45c28
//Bit 31:29     reserved
//Bit 28:16     reg_bld_src2_h_end          //unsigned, RW, default = 2559
//Bit 15:13     reserved                    
//Bit 12:0      reg_bld_src2_h_start        //unsigned, RW, default = 0

#define VPU_PROC_BLD_SRC2_VSCOPE (0x40000+(0x170b<<2))//0x45c2c
//Bit 31:29     reserved
//Bit 28:16     reg_bld_src2_v_end          //unsigned, RW, default = 1439
//Bit 15:13     reserved                    
//Bit 12:0      reg_bld_src2_v_start        //unsigned, RW, default = 0

#define VPU_PROC_BLD_SRC3_HSCOPE (0x40000+(0x170c<<2))//0x45c30
//Bit 31:29     reserved
//Bit 28:16     reg_bld_src3_h_end          //unsigned, RW, default = 2559
//Bit 15:13     reserved                    
//Bit 12:0      reg_bld_src3_h_start        //unsigned, RW, default = 0

#define VPU_PROC_BLD_SRC3_VSCOPE (0x40000+(0x170d<<2))//0x45c34
//Bit 31:29     reserved
//Bit 28:16     reg_bld_src3_v_end          //unsigned, RW, default = 1439
//Bit 15:13     reserved                    
//Bit 12:0      reg_bld_src3_v_start        //unsigned, RW, default = 0

#define VPU_PROC_BLD_VIDEO_ALPHA (0x40000+(0x170e<<2))//0x45c38
//Bit 31:9      reserved
//Bit 8 :0      reg_bld_video_alpha         //unsigned, RW, default = 9'h100 

#define VPU_PROC_VADJ_POS (0x40000+(0x1713<<2))//0x45c4c
//Bit 31:1      reserved
//Bit 0         reg_vadj_aft_bld            //unsigned, RW, default = 1

#define VPU_PROC_ERR_CLR (0x40000+(0x1714<<2))//0x45c50
//Bit 31:9      reserved
//Bit 8:0       pls_err_clr                 //unsigned, W1T, default = 0

#define VPU_PROC_BLD_VLD_CLR (0x40000+(0x1715<<2))//0x45c54
//Bit 31:4      reserved
//Bit 3:0       pls_bld_vld_clr             //unsigned, W1T, default = 0

#define VPU_PROC_DE_ERR (0x40000+(0x1716<<2))//0x45c58
//Bit 31:9      reserved
//Bit 8:0       ro_de_err                   //unsigned, RO, default = 0

#define VPU_PROC_BLD_DIN_RO (0x40000+(0x1717<<2))//0x45c5c
//Bit 31:4      reserved
//Bit 3         ro_bld_din0_vld             //unsigned, RO, default = 0
//Bit 2         ro_bld_din1_err             //unsigned, RO, default = 0
//Bit 1         ro_bld_din2_err             //unsigned, RO, default = 0
//Bit 0         ro_bld_din3_err             //unsigned, RO, default = 0

#define VPU_PROC_HBLANK_NUM (0x40000+(0x1718<<2))//0x45c60
//Bit 31:13     reserved
//Bit 12:0      reg_hblank_num              //unsigned, RW, default = 0

#define VPU_PROC_DUMPPY_DATA (0x40000+(0x1719<<2))//0x45c64
//Bit 31:30     reserved
//Bit 29:0      reg_dummy_data              //unsigned, RW, default = 0

#define VPU_PROC_DE_DLY_ERR (0x40000+(0x171a<<2))//0x45c68
//Bit 31:8      reserved
//Bit 7:0       ro_de_dly_err               //unsigned, RO, default = 0

#define VPU_PROC_DE_DLY_ERR_CLR (0x40000+(0x171b<<2))//0x45c6c
//Bit 31:8      reserved
//Bit 7:0       pls_de_dly_err_clr          //unsigned, W1T, default = 0

#define VPU_PROC_CM_RAM_RST (0x40000+(0x171d<<2))//0x45c74
//Bit 31:5      reserved
//Bit 4         reg_cm_int_phs_mask         //unsigned, RW,  default = 1
//Bit 3:1       reserved
//Bit 0         pls_cm_ram_rst              //unsigned, W1T, default = 0


#define VPP_BLEND_CTRL (0x40000+(0x1720<<2))//0x45c80
//Bit 31:28  reserved              
//Bit 27:20  reg_hold_lines               //unsigned ,RW, default = 0  
//Bit 19:7   reserved            
//Bit 6 :4   reg_phs_enable               //unsigned ,RW, default = 0         
//Bit 3 :2   reserved            
//Bit 1 :0   reg_gclk_ctrl                //unsigned ,RW, default = 0

#define VPP_BLEND_BLEND_DUMMY_DATA (0x40000+(0x1721<<2))//0x45c84
//Bit 31:24  reserved
//Bit 23:16  reg_blend0_dummy_data_y      //unsigned ,RW, default = 8'h00
//Bit 15:8   reg_blend0_dummy_data_cb     //unsigned ,RW, default = 8'h80
//Bit 7 :0   reg_blend0_dummy_data_cr     //unsigned ,RW, default = 8'h80

#define VPP_BLEND_DUMMY_ALPHA (0x40000+(0x1722<<2))//0x45c88
//Bit 31:29  reserved
//Bit 28:20  reg_blend2_dummy_alpha       //unsigned ,RW, default = 9'h0
//Bit 19:11  reg_blend1_dummy_alpha       //unsigned ,RW, default = 9'h0
//Bit 10:9   reserved
//Bit 8 :0   reg_blend0_dummy_alpha       //unsigned ,RW, default = 9'h0

#define VPP_BLEND_RO_CURRENT_XY (0x40000+(0x1723<<2))//0x45c8c
//Bit 31:0   ro_blend_current_xy          //unsigned ,RO, default = 32'h0


#define FOSD_WIN_FONT_INFO_0 (0x40000+(0x1800<<2))//0x46000
//Bit 31            reserved                           
//Bit 30           reg_win_en_0              // unsigned ,    RW, default = 0  window enable
//Bit 29:16        reg_win_baddr_0           // unsigned ,    RW, default = 0  each window (font_idx + color_idx) start addr in sram
//Bit 15: 8        reg_win_blend_alpha_0     // unsigned ,    RW, default = 0  blending alpha of font in window
//Bit  7: 6        reg_font_rot_mode_0       // unsigned ,    RW, default = 0  rotate mode, 0: 0, 1:90, 2:180, 3:270
//Bit  5           reg_font_vflip_en_0       // unsigned ,    RW, default = 0  ver flip
//Bit  4           reg_font_hmirr_en_0       // unsigned ,    RW, default = 0  hor flip
//Bit  3: 2        reg_win_rot_mode_0        // unsigned ,    RW, default = 0  rotate mode, 0: 0, 1:90, 2:180, 3:270, should be the same as reg_font_rot_mode[i]
//Bit  1           reg_win_vflip_en_0        // unsigned ,    RW, default = 0  ver flip, should be the same as reg_font_vflip_en[i]
//Bit  0           reg_win_hmirr_en_0        // unsigned ,    RW, default = 0  hor flip, should be the same as reg_font_hmirr_en[i]

#define FOSD_WIN_LT_XY_0 (0x40000+(0x1801<<2))//0x46004
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_lt_x_0            // unsigned ,    RW, default = 0  top-left x coordinate of window
//Bit 12: 0        reg_osd_lt_y_0            // unsigned ,    RW, default = 0  top-left y coordinate of window

#define FOSD_WIN_RB_XY_0 (0x40000+(0x1802<<2))//0x46008
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_rb_x_0            // unsigned ,    RW, default = 2  right-bottom x coordinate of window
//Bit 12: 0        reg_osd_rb_y_0            // unsigned ,    RW, default = 2  right-bottom y coordinate of window

#define FOSD_WIN_WH_0 (0x40000+(0x1803<<2))//0x4600c
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_h_num_0           // unsigned ,    RW, default = 1  horizontal font num in window
//Bit 12: 0        reg_osd_v_num_0           // unsigned ,    RW, default = 1  vertical font num in window

#define FOSD_WIN_FONT_INFO_1 (0x40000+(0x1804<<2))//0x46010
//Bit 31            reserved                           
//Bit 30           reg_win_en_1              // unsigned ,    RW, default = 0  window enable
//Bit 29:16        reg_win_baddr_1           // unsigned ,    RW, default = 0  each window (font_idx + color_idx) start addr in sram
//Bit 15: 8        reg_win_blend_alpha_1     // unsigned ,    RW, default = 0  blending alpha of font in window
//Bit  7: 6        reg_font_rot_mode_1       // unsigned ,    RW, default = 0  rotate mode, 0: 0, 1:90, 2:180, 3:270
//Bit  5           reg_font_vflip_en_1       // unsigned ,    RW, default = 0  ver flip
//Bit  4           reg_font_hmirr_en_1       // unsigned ,    RW, default = 0  hor flip
//Bit  3: 2        reg_win_rot_mode_1        // unsigned ,    RW, default = 0  rotate mode, 0: 0, 1:90, 2:180, 3:270, should be the same as reg_font_rot_mode[i]
//Bit  1           reg_win_vflip_en_1        // unsigned ,    RW, default = 0  ver flip, should be the same as reg_font_vflip_en[i]
//Bit  0           reg_win_hmirr_en_1        // unsigned ,    RW, default = 0  hor flip, should be the same as reg_font_hmirr_en[i]

#define FOSD_WIN_LT_XY_1 (0x40000+(0x1805<<2))//0x46014
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_lt_x_1            // unsigned ,    RW, default = 0  top-left x coordinate of window
//Bit 12: 0        reg_osd_lt_y_1            // unsigned ,    RW, default = 0  top-left y coordinate of window

#define FOSD_WIN_RB_XY_1 (0x40000+(0x1806<<2))//0x46018
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_rb_x_1            // unsigned ,    RW, default = 2  right-bottom x coordinate of window
//Bit 12: 0        reg_osd_rb_y_1            // unsigned ,    RW, default = 2  right-bottom y coordinate of window

#define FOSD_WIN_WH_1 (0x40000+(0x1807<<2))//0x4601c
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_h_num_1           // unsigned ,    RW, default = 1  horizontal font num in window
//Bit 12: 0        reg_osd_v_num_1           // unsigned ,    RW, default = 1  vertical font num in window

#define FOSD_WIN_FONT_INFO_2 (0x40000+(0x1808<<2))//0x46020
//Bit 31            reserved                           
//Bit 30           reg_win_en_2              // unsigned ,    RW, default = 0  window enable
//Bit 29:16        reg_win_baddr_2           // unsigned ,    RW, default = 0  each window (font_idx + color_idx) start addr in sram
//Bit 15: 8        reg_win_blend_alpha_2     // unsigned ,    RW, default = 0  blending alpha of font in window
//Bit  7: 6        reg_font_rot_mode_2       // unsigned ,    RW, default = 0  rotate mode, 0: 0, 1:90, 2:180, 3:270
//Bit  5           reg_font_vflip_en_2       // unsigned ,    RW, default = 0  ver flip
//Bit  4           reg_font_hmirr_en_2       // unsigned ,    RW, default = 0  hor flip
//Bit  3: 2        reg_win_rot_mode_2        // unsigned ,    RW, default = 0  rotate mode, 0: 0, 1:90, 2:180, 3:270, should be the same as reg_font_rot_mode[i]
//Bit  1           reg_win_vflip_en_2        // unsigned ,    RW, default = 0  ver flip, should be the same as reg_font_vflip_en[i]
//Bit  0           reg_win_hmirr_en_2        // unsigned ,    RW, default = 0  hor flip, should be the same as reg_font_hmirr_en[i]

#define FOSD_WIN_LT_XY_2 (0x40000+(0x1809<<2))//0x46024
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_lt_x_2            // unsigned ,    RW, default = 0  top-left x coordinate of window
//Bit 12: 0        reg_osd_lt_y_2            // unsigned ,    RW, default = 0  top-left y coordinate of window

#define FOSD_WIN_RB_XY_2 (0x40000+(0x180a<<2))//0x46028
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_rb_x_2            // unsigned ,    RW, default = 2  right-bottom x coordinate of window
//Bit 12: 0        reg_osd_rb_y_2            // unsigned ,    RW, default = 2  right-bottom y coordinate of window

#define FOSD_WIN_WH_2 (0x40000+(0x180b<<2))//0x4602c
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_h_num_2           // unsigned ,    RW, default = 1  horizontal font num in window
//Bit 12: 0        reg_osd_v_num_2           // unsigned ,    RW, default = 1  vertical font num in window

#define FOSD_WIN_FONT_INFO_3 (0x40000+(0x180c<<2))//0x46030
//Bit 31            reserved                           
//Bit 30           reg_win_en_3              // unsigned ,    RW, default = 0  window enable
//Bit 29:16        reg_win_baddr_3           // unsigned ,    RW, default = 0  each window (font_idx + color_idx) start addr in sram
//Bit 15: 8        reg_win_blend_alpha_3     // unsigned ,    RW, default = 0  blending alpha of font in window
//Bit  7: 6        reg_font_rot_mode_3       // unsigned ,    RW, default = 0  rotate mode, 0: 0, 1:90, 2:180, 3:270
//Bit  5           reg_font_vflip_en_3       // unsigned ,    RW, default = 0  ver flip
//Bit  4           reg_font_hmirr_en_3       // unsigned ,    RW, default = 0  hor flip
//Bit  3: 2        reg_win_rot_mode_3        // unsigned ,    RW, default = 0  rotate mode, 0: 0, 1:90, 2:180, 3:270, should be the same as reg_font_rot_mode[i]
//Bit  1           reg_win_vflip_en_3        // unsigned ,    RW, default = 0  ver flip, should be the same as reg_font_vflip_en[i]
//Bit  0           reg_win_hmirr_en_3        // unsigned ,    RW, default = 0  hor flip, should be the same as reg_font_hmirr_en[i]

#define FOSD_WIN_LT_XY_3 (0x40000+(0x180d<<2))//0x46034
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_lt_x_3            // unsigned ,    RW, default = 0  top-left x coordinate of window
//Bit 12: 0        reg_osd_lt_y_3            // unsigned ,    RW, default = 0  top-left y coordinate of window

#define FOSD_WIN_RB_XY_3 (0x40000+(0x180e<<2))//0x46038
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_rb_x_3            // unsigned ,    RW, default = 2  right-bottom x coordinate of window
//Bit 12: 0        reg_osd_rb_y_3            // unsigned ,    RW, default = 2  right-bottom y coordinate of window

#define FOSD_WIN_WH_3 (0x40000+(0x180f<<2))//0x4603c
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_h_num_3           // unsigned ,    RW, default = 1  horizontal font num in window
//Bit 12: 0        reg_osd_v_num_3           // unsigned ,    RW, default = 1  vertical font num in window

#define FOSD_WIN_FONT_INFO_4 (0x40000+(0x1810<<2))//0x46040
//Bit 31            reserved                           
//Bit 30           reg_win_en_4              // unsigned ,    RW, default = 0  window enable
//Bit 29:16        reg_win_baddr_4           // unsigned ,    RW, default = 0  each window (font_idx + color_idx) start addr in sram
//Bit 15: 8        reg_win_blend_alpha_4     // unsigned ,    RW, default = 0  blending alpha of font in window
//Bit  7: 6        reg_font_rot_mode_4       // unsigned ,    RW, default = 0  rotate mode, 0: 0, 1:90, 2:180, 3:270
//Bit  5           reg_font_vflip_en_4       // unsigned ,    RW, default = 0  ver flip
//Bit  4           reg_font_hmirr_en_4       // unsigned ,    RW, default = 0  hor flip
//Bit  3: 2        reg_win_rot_mode_4        // unsigned ,    RW, default = 0  rotate mode, 0: 0, 1:90, 2:180, 3:270, should be the same as reg_font_rot_mode[i]
//Bit  1           reg_win_vflip_en_4        // unsigned ,    RW, default = 0  ver flip, should be the same as reg_font_vflip_en[i]
//Bit  0           reg_win_hmirr_en_4        // unsigned ,    RW, default = 0  hor flip, should be the same as reg_font_hmirr_en[i]

#define FOSD_WIN_LT_XY_4 (0x40000+(0x1811<<2))//0x46044
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_lt_x_4            // unsigned ,    RW, default = 0  top-left x coordinate of window
//Bit 12: 0        reg_osd_lt_y_4            // unsigned ,    RW, default = 0  top-left y coordinate of window

#define FOSD_WIN_RB_XY_4 (0x40000+(0x1812<<2))//0x46048
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_rb_x_4            // unsigned ,    RW, default = 2  right-bottom x coordinate of window
//Bit 12: 0        reg_osd_rb_y_4            // unsigned ,    RW, default = 2  right-bottom y coordinate of window

#define FOSD_WIN_WH_4 (0x40000+(0x1813<<2))//0x4604c
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_h_num_4           // unsigned ,    RW, default = 1  horizontal font num in window
//Bit 12: 0        reg_osd_v_num_4           // unsigned ,    RW, default = 1  vertical font num in window

#define FOSD_WIN_FONT_INFO_5 (0x40000+(0x1814<<2))//0x46050
//Bit 31            reserved                           
//Bit 30           reg_win_en_5              // unsigned ,    RW, default = 0  window enable
//Bit 29:16        reg_win_baddr_5           // unsigned ,    RW, default = 0  each window (font_idx + color_idx) start addr in sram
//Bit 15: 8        reg_win_blend_alpha_5     // unsigned ,    RW, default = 0  blending alpha of font in window
//Bit  7: 6        reg_font_rot_mode_5       // unsigned ,    RW, default = 0  rotate mode, 0: 0, 1:90, 2:180, 3:270
//Bit  5           reg_font_vflip_en_5       // unsigned ,    RW, default = 0  ver flip
//Bit  4           reg_font_hmirr_en_5       // unsigned ,    RW, default = 0  hor flip
//Bit  3: 2        reg_win_rot_mode_5        // unsigned ,    RW, default = 0  rotate mode, 0: 0, 1:90, 2:180, 3:270, should be the same as reg_font_rot_mode[i]
//Bit  1           reg_win_vflip_en_5        // unsigned ,    RW, default = 0  ver flip, should be the same as reg_font_vflip_en[i]
//Bit  0           reg_win_hmirr_en_5        // unsigned ,    RW, default = 0  hor flip, should be the same as reg_font_hmirr_en[i]

#define FOSD_WIN_LT_XY_5 (0x40000+(0x1815<<2))//0x46054
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_lt_x_5            // unsigned ,    RW, default = 0  top-left x coordinate of window
//Bit 12: 0        reg_osd_lt_y_5            // unsigned ,    RW, default = 0  top-left y coordinate of window

#define FOSD_WIN_RB_XY_5 (0x40000+(0x1816<<2))//0x46058
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_rb_x_5            // unsigned ,    RW, default = 2  right-bottom x coordinate of window
//Bit 12: 0        reg_osd_rb_y_5            // unsigned ,    RW, default = 2  right-bottom y coordinate of window

#define FOSD_WIN_WH_5 (0x40000+(0x1817<<2))//0x4605c
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_h_num_5           // unsigned ,    RW, default = 1  horizontal font num in window
//Bit 12: 0        reg_osd_v_num_5           // unsigned ,    RW, default = 1  vertical font num in window

#define FOSD_WIN_FONT_INFO_6 (0x40000+(0x1818<<2))//0x46060
//Bit 31            reserved                           
//Bit 30           reg_win_en_6              // unsigned ,    RW, default = 0  window enable
//Bit 29:16        reg_win_baddr_6           // unsigned ,    RW, default = 0  each window (font_idx + color_idx) start addr in sram
//Bit 15: 8        reg_win_blend_alpha_6     // unsigned ,    RW, default = 0  blending alpha of font in window
//Bit  7: 6        reg_font_rot_mode_6       // unsigned ,    RW, default = 0  rotate mode, 0: 0, 1:90, 2:180, 3:270
//Bit  5           reg_font_vflip_en_6       // unsigned ,    RW, default = 0  ver flip
//Bit  4           reg_font_hmirr_en_6       // unsigned ,    RW, default = 0  hor flip
//Bit  3: 2        reg_win_rot_mode_6        // unsigned ,    RW, default = 0  rotate mode, 0: 0, 1:90, 2:180, 3:270, should be the same as reg_font_rot_mode[i]
//Bit  1           reg_win_vflip_en_6        // unsigned ,    RW, default = 0  ver flip, should be the same as reg_font_vflip_en[i]
//Bit  0           reg_win_hmirr_en_6        // unsigned ,    RW, default = 0  hor flip, should be the same as reg_font_hmirr_en[i]

#define FOSD_WIN_LT_XY_6 (0x40000+(0x1819<<2))//0x46064
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_lt_x_6            // unsigned ,    RW, default = 0  top-left x coordinate of window
//Bit 12: 0        reg_osd_lt_y_6            // unsigned ,    RW, default = 0  top-left y coordinate of window

#define FOSD_WIN_RB_XY_6 (0x40000+(0x181a<<2))//0x46068
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_rb_x_6            // unsigned ,    RW, default = 2  right-bottom x coordinate of window
//Bit 12: 0        reg_osd_rb_y_6            // unsigned ,    RW, default = 2  right-bottom y coordinate of window

#define FOSD_WIN_WH_6 (0x40000+(0x181b<<2))//0x4606c
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_h_num_6           // unsigned ,    RW, default = 1  horizontal font num in window
//Bit 12: 0        reg_osd_v_num_6           // unsigned ,    RW, default = 1  vertical font num in window

#define FOSD_WIN_FONT_INFO_7 (0x40000+(0x181c<<2))//0x46070
//Bit 31            reserved                           
//Bit 30           reg_win_en_7              // unsigned ,    RW, default = 0  window enable
//Bit 29:16        reg_win_baddr_7           // unsigned ,    RW, default = 0  each window (font_idx + color_idx) start addr in sram
//Bit 15: 8        reg_win_blend_alpha_7     // unsigned ,    RW, default = 0  blending alpha of font in window
//Bit  7: 6        reg_font_rot_mode_7       // unsigned ,    RW, default = 0  rotate mode, 0: 0, 1:90, 2:180, 3:270
//Bit  5           reg_font_vflip_en_7       // unsigned ,    RW, default = 0  ver flip
//Bit  4           reg_font_hmirr_en_7       // unsigned ,    RW, default = 0  hor flip
//Bit  3: 2        reg_win_rot_mode_7        // unsigned ,    RW, default = 0  rotate mode, 0: 0, 1:90, 2:180, 3:270, should be the same as reg_font_rot_mode[i]
//Bit  1           reg_win_vflip_en_7        // unsigned ,    RW, default = 0  ver flip, should be the same as reg_font_vflip_en[i]
//Bit  0           reg_win_hmirr_en_7        // unsigned ,    RW, default = 0  hor flip, should be the same as reg_font_hmirr_en[i]

#define FOSD_WIN_LT_XY_7 (0x40000+(0x181d<<2))//0x46074
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_lt_x_7            // unsigned ,    RW, default = 0  top-left x coordinate of window
//Bit 12: 0        reg_osd_lt_y_7            // unsigned ,    RW, default = 0  top-left y coordinate of window

#define FOSD_WIN_RB_XY_7 (0x40000+(0x181e<<2))//0x46078
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_rb_x_7            // unsigned ,    RW, default = 2  right-bottom x coordinate of window
//Bit 12: 0        reg_osd_rb_y_7            // unsigned ,    RW, default = 2  right-bottom y coordinate of window

#define FOSD_WIN_WH_7 (0x40000+(0x181f<<2))//0x4607c
//Bit 31:26        reserved                           
//Bit 25:13        reg_osd_h_num_7           // unsigned ,    RW, default = 1  horizontal font num in window
//Bit 12: 0        reg_osd_v_num_7           // unsigned ,    RW, default = 1  vertical font num in window

#define FOSD_FONT_1BIT_ADDR (0x40000+(0x1820<<2))//0x46080
//Bit 31:14        reserved                           
//Bit 13: 0        reg_1bit_font_baddr       // unsigned ,    RW, default = 0  1 bit font start addr in sram

#define FOSD_FONT_2BIT_ADDR (0x40000+(0x1821<<2))//0x46084
//Bit 31:14        reserved                           
//Bit 13: 0        reg_2bit_font_baddr       // unsigned ,    RW, default = 0  2 bit font start addr in sram

#define FOSD_FONT_4BIT_ADDR (0x40000+(0x1822<<2))//0x46088
//Bit 31:14        reserved                           
//Bit 13: 0        reg_4bit_font_baddr       // unsigned ,    RW, default = 0  4 bit font start addr in sram

#define FOSD_FONT_IDX_TH (0x40000+(0x1823<<2))//0x4608c
//Bit 31:24        reserved                           
//Bit 23:12        reg_font_2bit_idx_th      // unsigned ,    RW, default = 0  indicate how many 1bit font num in sram
//Bit 11: 0        reg_font_4bit_idx_th      // unsigned ,    RW, default = 0  indicate how many 1bit + 2bit font num in sram

#define FOSD_ELLIP_CENTERXY_0 (0x40000+(0x1824<<2))//0x46090
//Bit 31:26        reserved                           
//Bit 25:13        reg_ellip_x_0             // unsigned ,    RW, default = 0  the center x coordinate of ellipse
//Bit 12: 0        reg_ellip_y_0             // unsigned ,    RW, default = 0  the center y coordinate of ellipse

#define FOSD_ELLIP_INFO_0 (0x40000+(0x1825<<2))//0x46094
//Bit 31:26        reserved                           
//Bit 25           reg_each_ellip_en_0       // unsigned ,    RW, default = 0  the thick of ellipse
//Bit 24           reg_ellip_mode_0          // unsigned ,    RW, default = 0  the mode of ellipse 0:no fill; 1:fill
//Bit 23:20        reg_ellip_thick_0         // unsigned ,    RW, default = 1  the thick of ellipse
//Bit 19:10        reg_ellip_w_0             // unsigned ,    RW, default = 1  the hor radius of ellipse
//Bit  9: 0        reg_ellip_h_0             // unsigned ,    RW, default = 1  the ver radius of ellipse

#define FOSD_ELLIP_RGBA_0 (0x40000+(0x1826<<2))//0x46098
//Bit 31:24        reg_ellip_alpha_0         // unsigned ,    RW, default = 128  blending alpha of ellipse
//Bit 23:16        reg_ellip_r_0             // unsigned ,    RW, default = 128  R value of ellipse
//Bit 15: 8        reg_ellip_g_0             // unsigned ,    RW, default = 128  G value of ellipse
//Bit  7: 0        reg_ellip_b_0             // unsigned ,    RW, default = 128  B value of ellipse

#define FOSD_ELLIP_OUT_A2_0 (0x40000+(0x1827<<2))//0x4609c
//Bit 31:20        reserved                           
//Bit 19: 0        reg_ellip_outer_a2_0      // unsigned ,    RW, default = 1  reg_ellip_w * reg_ellip_w, should be calculated by firmware

#define FOSD_ELLIP_IN_A2_0 (0x40000+(0x1828<<2))//0x460a0
//Bit 31:20        reserved                           
//Bit 19: 0        reg_ellip_inner_a2_0      // unsigned ,    RW, default = 1  (reg_ellip_w - reg_ellip_thick) * (reg_ellip_w - reg_ellip_thick), should be calculated by firmware

#define FOSD_ELLIP_OUT_B2_0 (0x40000+(0x1829<<2))//0x460a4
//Bit 31:20        reserved                           
//Bit 19: 0        reg_ellip_outer_b2_0      // unsigned ,    RW, default = 1  reg_ellip_h * reg_ellip_h, should be calculated by firmware

#define FOSD_ELLIP_IN_B2_0 (0x40000+(0x182a<<2))//0x460a8
//Bit 31:20        reserved                           
//Bit 19: 0        reg_ellip_inner_b2_0      // unsigned ,    RW, default = 1  (reg_ellip_h - reg_ellip_thick) * (reg_ellip_h - reg_ellip_thick), should be calculated by firmware

#define FOSD_ELLIP_OUT_A2B2_L_0 (0x40000+(0x182b<<2))//0x460ac
//Bit 31: 0        reg_ellip_outer_a2b2_low_0 // unsigned ,    RW, default = 1  low 32 bit of reg_ellip_outer_a2 * reg_ellip_outer_b2, should be calculated by firmware

#define FOSD_ELLIP_IN_A2B2_L_0 (0x40000+(0x182c<<2))//0x460b0
//Bit 31: 0        reg_ellip_inner_a2b2_low_0 // unsigned ,    RW, default = 1  low 32 bit of reg_ellip_inner_a2 * reg_ellip_inner_b2, should be calculated by firmware

#define FOSD_ELLIP_OUTIN_A2B2_H_0 (0x40000+(0x182d<<2))//0x460b4
//Bit 31:16        reserved                           
//Bit 15: 8        reg_ellip_inner_a2b2_high_0 // unsigned ,    RW, default = 1  high 8 bit of reg_ellip_inner_a2 * reg_ellip_inner_b2, should be calculated by firmware
//Bit  7: 0        reg_ellip_outer_a2b2_high_0 // unsigned ,    RW, default = 1  high 8 bit of reg_ellip_outer_a2 * reg_ellip_outer_b2, should be calculated by firmware

#define FOSD_ELLIP_CENTERXY_1 (0x40000+(0x182e<<2))//0x460b8
//Bit 31:26        reserved                           
//Bit 25:13        reg_ellip_x_1             // unsigned ,    RW, default = 0  the center x coordinate of ellipse
//Bit 12: 0        reg_ellip_y_1             // unsigned ,    RW, default = 0  the center y coordinate of ellipse

#define FOSD_ELLIP_INFO_1 (0x40000+(0x182f<<2))//0x460bc
//Bit 31:26        reserved                           
//Bit 25           reg_each_ellip_en_1       // unsigned ,    RW, default = 0  the thick of ellipse
//Bit 24           reg_ellip_mode_1          // unsigned ,    RW, default = 0  the mode of ellipse 0:no fill; 1:fill
//Bit 23:20        reg_ellip_thick_1         // unsigned ,    RW, default = 1  the thick of ellipse
//Bit 19:10        reg_ellip_w_1             // unsigned ,    RW, default = 1  the hor radius of ellipse
//Bit  9: 0        reg_ellip_h_1             // unsigned ,    RW, default = 1  the ver radius of ellipse

#define FOSD_ELLIP_RGBA_1 (0x40000+(0x1830<<2))//0x460c0
//Bit 31:24        reg_ellip_alpha_1         // unsigned ,    RW, default = 128  blending alpha of ellipse
//Bit 23:16        reg_ellip_r_1             // unsigned ,    RW, default = 128  R value of ellipse
//Bit 15: 8        reg_ellip_g_1             // unsigned ,    RW, default = 128  G value of ellipse
//Bit  7: 0        reg_ellip_b_1             // unsigned ,    RW, default = 128  B value of ellipse

#define FOSD_ELLIP_OUT_A2_1 (0x40000+(0x1831<<2))//0x460c4
//Bit 31:20        reserved                           
//Bit 19: 0        reg_ellip_outer_a2_1      // unsigned ,    RW, default = 1  reg_ellip_w * reg_ellip_w, should be calculated by firmware

#define FOSD_ELLIP_IN_A2_1 (0x40000+(0x1832<<2))//0x460c8
//Bit 31:20        reserved                           
//Bit 19: 0        reg_ellip_inner_a2_1      // unsigned ,    RW, default = 1  (reg_ellip_w - reg_ellip_thick) * (reg_ellip_w - reg_ellip_thick), should be calculated by firmware

#define FOSD_ELLIP_OUT_B2_1 (0x40000+(0x1833<<2))//0x460cc
//Bit 31:20        reserved                           
//Bit 19: 0        reg_ellip_outer_b2_1      // unsigned ,    RW, default = 1  reg_ellip_h * reg_ellip_h, should be calculated by firmware

#define FOSD_ELLIP_IN_B2_1 (0x40000+(0x1834<<2))//0x460d0
//Bit 31:20        reserved                           
//Bit 19: 0        reg_ellip_inner_b2_1      // unsigned ,    RW, default = 1  (reg_ellip_h - reg_ellip_thick) * (reg_ellip_h - reg_ellip_thick), should be calculated by firmware

#define FOSD_ELLIP_OUT_A2B2_L_1 (0x40000+(0x1835<<2))//0x460d4
//Bit 31: 0        reg_ellip_outer_a2b2_low_1 // unsigned ,    RW, default = 1  low 32 bit of reg_ellip_outer_a2 * reg_ellip_outer_b2, should be calculated by firmware

#define FOSD_ELLIP_IN_A2B2_L_1 (0x40000+(0x1836<<2))//0x460d8
//Bit 31: 0        reg_ellip_inner_a2b2_low_1 // unsigned ,    RW, default = 1  low 32 bit of reg_ellip_inner_a2 * reg_ellip_inner_b2, should be calculated by firmware

#define FOSD_ELLIP_OUTIN_A2B2_H_1 (0x40000+(0x1837<<2))//0x460dc
//Bit 31:16        reserved                           
//Bit 15: 8        reg_ellip_inner_a2b2_high_1 // unsigned ,    RW, default = 1  high 8 bit of reg_ellip_inner_a2 * reg_ellip_inner_b2, should be calculated by firmware
//Bit  7: 0        reg_ellip_outer_a2b2_high_1 // unsigned ,    RW, default = 1  high 8 bit of reg_ellip_outer_a2 * reg_ellip_outer_b2, should be calculated by firmware

#define FOSD_RECT_XY_0 (0x40000+(0x1838<<2))//0x460e0
//Bit 31:26        reserved                           
//Bit 25:13        reg_rect_x_0              // unsigned ,    RW, default = 0  top-left x coordinate of rectangle
//Bit 12: 0        reg_rect_y_0              // unsigned ,    RW, default = 0  top-left y coordinate of rectangle

#define FOSD_RECT_WH_0 (0x40000+(0x1839<<2))//0x460e4
//Bit 31:26        reserved                           
//Bit 25:13        reg_rect_w_0              // unsigned ,    RW, default = 2  the width of rectangle
//Bit 12: 0        reg_rect_h_0              // unsigned ,    RW, default = 2  the height of rectangle

#define FOSD_MODE_INFO_1_0 (0x40000+(0x183a<<2))//0x460e8
//Bit 31:12        reserved                           
//Bit 11           reg_each_rect_en_0        // unsigned ,    RW, default = 0  each rectangular enable
//Bit 10: 8        reg_rect_grad_mode_0      // unsigned ,    RW, default = 0  0: hor:color1->color2; 1: hor:color1->color2->color1; 2: ver:color1->color2; 3: ver:color1->color2->color1; 4: center->round; 5:no color grad
//Bit  7           reg_rect_mode_0           // unsigned ,    RW, default = 0  0:no fill rect; 1: fill rect
//Bit  6: 3        reg_rect_thick_0          // unsigned ,    RW, default = 4  thick of rectangle to get a outlined rectangle
//Bit  2           reg_rect_comp_r_0         // unsigned ,    RW, default = 0  0: add+ 1: minus-
//Bit  1           reg_rect_comp_g_0         // unsigned ,    RW, default = 0  0: add+ 1: minus-
//Bit  0           reg_rect_comp_b_0         // unsigned ,    RW, default = 0  0: add+ 1: minus-

#define FOSD_RECT_DELTA_0 (0x40000+(0x183b<<2))//0x460ec
//Bit 31:24        reserved                           
//Bit 23:16        reg_rect_delta_r_0        // unsigned ,    RW, default = 0  R delta value to change on the base of fore change
//Bit 15: 8        reg_rect_delta_g_0        // unsigned ,    RW, default = 0  G delta value to change on the base of fore change
//Bit  7: 0        reg_rect_delta_b_0        // unsigned ,    RW, default = 0  B delta value to change on the base of fore change

#define FOSD_RECT_STEP_0 (0x40000+(0x183c<<2))//0x460f0
//Bit 31:24        reserved                           
//Bit 23:16        reg_rect_step_r_0         // unsigned ,    RW, default = 0  how much pixels to change R
//Bit 15: 8        reg_rect_step_g_0         // unsigned ,    RW, default = 0  how much pixels to change G
//Bit  7: 0        reg_rect_step_b_0         // unsigned ,    RW, default = 0  how much pixels to change B

#define FOSD_RECT_RGBA_0 (0x40000+(0x183d<<2))//0x460f4
//Bit 31:24        reg_rect_alpha_0          // unsigned ,    RW, default = 0  blending alpha of rectangle
//Bit 23:16        reg_rect_base_r_0         // unsigned ,    RW, default = 255  basic R of rectangle
//Bit 15: 8        reg_rect_base_g_0         // unsigned ,    RW, default = 128  basic G of rectangle
//Bit  7: 0        reg_rect_base_b_0         // unsigned ,    RW, default = 128  basic B of rectangle

#define FOSD_RECT_XY_1 (0x40000+(0x183e<<2))//0x460f8
//Bit 31:26        reserved                           
//Bit 25:13        reg_rect_x_1              // unsigned ,    RW, default = 0  top-left x coordinate of rectangle
//Bit 12: 0        reg_rect_y_1              // unsigned ,    RW, default = 0  top-left y coordinate of rectangle

#define FOSD_RECT_WH_1 (0x40000+(0x183f<<2))//0x460fc
//Bit 31:26        reserved                           
//Bit 25:13        reg_rect_w_1              // unsigned ,    RW, default = 2  the width of rectangle
//Bit 12: 0        reg_rect_h_1              // unsigned ,    RW, default = 2  the height of rectangle

#define FOSD_MODE_INFO_1_1 (0x40000+(0x1840<<2))//0x46100
//Bit 31:12        reserved                           
//Bit 11           reg_each_rect_en_1        // unsigned ,    RW, default = 0  each rectangular enable
//Bit 10: 8        reg_rect_grad_mode_1      // unsigned ,    RW, default = 0  0: hor:color1->color2; 1: hor:color1->color2->color1; 2: ver:color1->color2; 3: ver:color1->color2->color1; 4: center->round; 5:no color grad
//Bit  7           reg_rect_mode_1           // unsigned ,    RW, default = 0  0:no fill rect; 1: fill rect
//Bit  6: 3        reg_rect_thick_1          // unsigned ,    RW, default = 4  thick of rectangle to get a outlined rectangle
//Bit  2           reg_rect_comp_r_1         // unsigned ,    RW, default = 0  0: add+ 1: minus-
//Bit  1           reg_rect_comp_g_1         // unsigned ,    RW, default = 0  0: add+ 1: minus-
//Bit  0           reg_rect_comp_b_1         // unsigned ,    RW, default = 0  0: add+ 1: minus-

#define FOSD_RECT_DELTA_1 (0x40000+(0x1841<<2))//0x46104
//Bit 31:24        reserved                           
//Bit 23:16        reg_rect_delta_r_1        // unsigned ,    RW, default = 0  R delta value to change on the base of fore change
//Bit 15: 8        reg_rect_delta_g_1        // unsigned ,    RW, default = 0  G delta value to change on the base of fore change
//Bit  7: 0        reg_rect_delta_b_1        // unsigned ,    RW, default = 0  B delta value to change on the base of fore change

#define FOSD_RECT_STEP_1 (0x40000+(0x1842<<2))//0x46108
//Bit 31:24        reserved                           
//Bit 23:16        reg_rect_step_r_1         // unsigned ,    RW, default = 0  how much pixels to change R
//Bit 15: 8        reg_rect_step_g_1         // unsigned ,    RW, default = 0  how much pixels to change G
//Bit  7: 0        reg_rect_step_b_1         // unsigned ,    RW, default = 0  how much pixels to change B

#define FOSD_RECT_RGBA_1 (0x40000+(0x1843<<2))//0x4610c
//Bit 31:24        reg_rect_alpha_1          // unsigned ,    RW, default = 0  blending alpha of rectangle
//Bit 23:16        reg_rect_base_r_1         // unsigned ,    RW, default = 255  basic R of rectangle
//Bit 15: 8        reg_rect_base_g_1         // unsigned ,    RW, default = 128  basic G of rectangle
//Bit  7: 0        reg_rect_base_b_1         // unsigned ,    RW, default = 128  basic B of rectangle

#define FOSD_RECT_XY_2 (0x40000+(0x1844<<2))//0x46110
//Bit 31:26        reserved                           
//Bit 25:13        reg_rect_x_2              // unsigned ,    RW, default = 0  top-left x coordinate of rectangle
//Bit 12: 0        reg_rect_y_2              // unsigned ,    RW, default = 0  top-left y coordinate of rectangle

#define FOSD_RECT_WH_2 (0x40000+(0x1845<<2))//0x46114
//Bit 31:26        reserved                           
//Bit 25:13        reg_rect_w_2              // unsigned ,    RW, default = 2  the width of rectangle
//Bit 12: 0        reg_rect_h_2              // unsigned ,    RW, default = 2  the height of rectangle

#define FOSD_MODE_INFO_1_2 (0x40000+(0x1846<<2))//0x46118
//Bit 31:12        reserved                           
//Bit 11           reg_each_rect_en_2        // unsigned ,    RW, default = 0  each rectangular enable
//Bit 10: 8        reg_rect_grad_mode_2      // unsigned ,    RW, default = 0  0: hor:color1->color2; 1: hor:color1->color2->color1; 2: ver:color1->color2; 3: ver:color1->color2->color1; 4: center->round; 5:no color grad
//Bit  7           reg_rect_mode_2           // unsigned ,    RW, default = 0  0:no fill rect; 1: fill rect
//Bit  6: 3        reg_rect_thick_2          // unsigned ,    RW, default = 4  thick of rectangle to get a outlined rectangle
//Bit  2           reg_rect_comp_r_2         // unsigned ,    RW, default = 0  0: add+ 1: minus-
//Bit  1           reg_rect_comp_g_2         // unsigned ,    RW, default = 0  0: add+ 1: minus-
//Bit  0           reg_rect_comp_b_2         // unsigned ,    RW, default = 0  0: add+ 1: minus-

#define FOSD_RECT_DELTA_2 (0x40000+(0x1847<<2))//0x4611c
//Bit 31:24        reserved                           
//Bit 23:16        reg_rect_delta_r_2        // unsigned ,    RW, default = 0  R delta value to change on the base of fore change
//Bit 15: 8        reg_rect_delta_g_2        // unsigned ,    RW, default = 0  G delta value to change on the base of fore change
//Bit  7: 0        reg_rect_delta_b_2        // unsigned ,    RW, default = 0  B delta value to change on the base of fore change

#define FOSD_RECT_STEP_2 (0x40000+(0x1848<<2))//0x46120
//Bit 31:24        reserved                           
//Bit 23:16        reg_rect_step_r_2         // unsigned ,    RW, default = 0  how much pixels to change R
//Bit 15: 8        reg_rect_step_g_2         // unsigned ,    RW, default = 0  how much pixels to change G
//Bit  7: 0        reg_rect_step_b_2         // unsigned ,    RW, default = 0  how much pixels to change B

#define FOSD_RECT_RGBA_2 (0x40000+(0x1849<<2))//0x46124
//Bit 31:24        reg_rect_alpha_2          // unsigned ,    RW, default = 0  blending alpha of rectangle
//Bit 23:16        reg_rect_base_r_2         // unsigned ,    RW, default = 255  basic R of rectangle
//Bit 15: 8        reg_rect_base_g_2         // unsigned ,    RW, default = 128  basic G of rectangle
//Bit  7: 0        reg_rect_base_b_2         // unsigned ,    RW, default = 128  basic B of rectangle

#define FOSD_RECT_XY_3 (0x40000+(0x184a<<2))//0x46128
//Bit 31:26        reserved                           
//Bit 25:13        reg_rect_x_3              // unsigned ,    RW, default = 0  top-left x coordinate of rectangle
//Bit 12: 0        reg_rect_y_3              // unsigned ,    RW, default = 0  top-left y coordinate of rectangle

#define FOSD_RECT_WH_3 (0x40000+(0x184b<<2))//0x4612c
//Bit 31:26        reserved                           
//Bit 25:13        reg_rect_w_3              // unsigned ,    RW, default = 2  the width of rectangle
//Bit 12: 0        reg_rect_h_3              // unsigned ,    RW, default = 2  the height of rectangle

#define FOSD_MODE_INFO_1_3 (0x40000+(0x184c<<2))//0x46130
//Bit 31:12        reserved                           
//Bit 11           reg_each_rect_en_3        // unsigned ,    RW, default = 0  each rectangular enable
//Bit 10: 8        reg_rect_grad_mode_3      // unsigned ,    RW, default = 0  0: hor:color1->color2; 1: hor:color1->color2->color1; 2: ver:color1->color2; 3: ver:color1->color2->color1; 4: center->round; 5:no color grad
//Bit  7           reg_rect_mode_3           // unsigned ,    RW, default = 0  0:no fill rect; 1: fill rect
//Bit  6: 3        reg_rect_thick_3          // unsigned ,    RW, default = 4  thick of rectangle to get a outlined rectangle
//Bit  2           reg_rect_comp_r_3         // unsigned ,    RW, default = 0  0: add+ 1: minus-
//Bit  1           reg_rect_comp_g_3         // unsigned ,    RW, default = 0  0: add+ 1: minus-
//Bit  0           reg_rect_comp_b_3         // unsigned ,    RW, default = 0  0: add+ 1: minus-

#define FOSD_RECT_DELTA_3 (0x40000+(0x184d<<2))//0x46134
//Bit 31:24        reserved                           
//Bit 23:16        reg_rect_delta_r_3        // unsigned ,    RW, default = 0  R delta value to change on the base of fore change
//Bit 15: 8        reg_rect_delta_g_3        // unsigned ,    RW, default = 0  G delta value to change on the base of fore change
//Bit  7: 0        reg_rect_delta_b_3        // unsigned ,    RW, default = 0  B delta value to change on the base of fore change

#define FOSD_RECT_STEP_3 (0x40000+(0x184e<<2))//0x46138
//Bit 31:24        reserved                           
//Bit 23:16        reg_rect_step_r_3         // unsigned ,    RW, default = 0  how much pixels to change R
//Bit 15: 8        reg_rect_step_g_3         // unsigned ,    RW, default = 0  how much pixels to change G
//Bit  7: 0        reg_rect_step_b_3         // unsigned ,    RW, default = 0  how much pixels to change B

#define FOSD_RECT_RGBA_3 (0x40000+(0x184f<<2))//0x4613c
//Bit 31:24        reg_rect_alpha_3          // unsigned ,    RW, default = 0  blending alpha of rectangle
//Bit 23:16        reg_rect_base_r_3         // unsigned ,    RW, default = 255  basic R of rectangle
//Bit 15: 8        reg_rect_base_g_3         // unsigned ,    RW, default = 128  basic G of rectangle
//Bit  7: 0        reg_rect_base_b_3         // unsigned ,    RW, default = 128  basic B of rectangle

#define FOSD_RECT_XY_4 (0x40000+(0x1850<<2))//0x46140
//Bit 31:26        reserved                           
//Bit 25:13        reg_rect_x_4              // unsigned ,    RW, default = 0  top-left x coordinate of rectangle
//Bit 12: 0        reg_rect_y_4              // unsigned ,    RW, default = 0  top-left y coordinate of rectangle

#define FOSD_RECT_WH_4 (0x40000+(0x1851<<2))//0x46144
//Bit 31:26        reserved                           
//Bit 25:13        reg_rect_w_4              // unsigned ,    RW, default = 2  the width of rectangle
//Bit 12: 0        reg_rect_h_4              // unsigned ,    RW, default = 2  the height of rectangle

#define FOSD_MODE_INFO_1_4 (0x40000+(0x1852<<2))//0x46148
//Bit 31:12        reserved                           
//Bit 11           reg_each_rect_en_4        // unsigned ,    RW, default = 0  each rectangular enable
//Bit 10: 8        reg_rect_grad_mode_4      // unsigned ,    RW, default = 0  0: hor:color1->color2; 1: hor:color1->color2->color1; 2: ver:color1->color2; 3: ver:color1->color2->color1; 4: center->round; 5:no color grad
//Bit  7           reg_rect_mode_4           // unsigned ,    RW, default = 0  0:no fill rect; 1: fill rect
//Bit  6: 3        reg_rect_thick_4          // unsigned ,    RW, default = 4  thick of rectangle to get a outlined rectangle
//Bit  2           reg_rect_comp_r_4         // unsigned ,    RW, default = 0  0: add+ 1: minus-
//Bit  1           reg_rect_comp_g_4         // unsigned ,    RW, default = 0  0: add+ 1: minus-
//Bit  0           reg_rect_comp_b_4         // unsigned ,    RW, default = 0  0: add+ 1: minus-

#define FOSD_RECT_DELTA_4 (0x40000+(0x1853<<2))//0x4614c
//Bit 31:24        reserved                           
//Bit 23:16        reg_rect_delta_r_4        // unsigned ,    RW, default = 0  R delta value to change on the base of fore change
//Bit 15: 8        reg_rect_delta_g_4        // unsigned ,    RW, default = 0  G delta value to change on the base of fore change
//Bit  7: 0        reg_rect_delta_b_4        // unsigned ,    RW, default = 0  B delta value to change on the base of fore change

#define FOSD_RECT_STEP_4 (0x40000+(0x1854<<2))//0x46150
//Bit 31:24        reserved                           
//Bit 23:16        reg_rect_step_r_4         // unsigned ,    RW, default = 0  how much pixels to change R
//Bit 15: 8        reg_rect_step_g_4         // unsigned ,    RW, default = 0  how much pixels to change G
//Bit  7: 0        reg_rect_step_b_4         // unsigned ,    RW, default = 0  how much pixels to change B

#define FOSD_RECT_RGBA_4 (0x40000+(0x1855<<2))//0x46154
//Bit 31:24        reg_rect_alpha_4          // unsigned ,    RW, default = 0  blending alpha of rectangle
//Bit 23:16        reg_rect_base_r_4         // unsigned ,    RW, default = 255  basic R of rectangle
//Bit 15: 8        reg_rect_base_g_4         // unsigned ,    RW, default = 128  basic G of rectangle
//Bit  7: 0        reg_rect_base_b_4         // unsigned ,    RW, default = 128  basic B of rectangle

#define FOSD_LINE_K_0 (0x40000+(0x1856<<2))//0x46158
//Bit 31: 0        reg_line_grad_0           // unsigned ,    RW, default = 1  line gradient, dx=(reg_line_end_x - reg_line_start_x), dy=(reg_line_end_y - reg_line_start_y), if abs(dx) > abs(dy), reg_line_grad=((abs(dy)<<16) / dx), else if abs(dx) < abs(dy), reg_line_grad=((abs(dx)<<16) / dy), should be calculated by firmware

#define FOSD_LINE_START_XY_0 (0x40000+(0x1857<<2))//0x4615c
//Bit 31            reserved                           
//Bit 30:18        reg_line_start_x_0        // unsigned ,    RW, default = 0  start x coordinate of line
//Bit 17: 5        reg_line_start_y_0        // unsigned ,    RW, default = 0  start y coordinate of line
//Bit  4           reg_each_line_en_0        // unsigned ,    RW, default = 0  each line enable
//Bit  3: 0        reg_line_width_0          // unsigned ,    RW, default = 4  thick of line

#define FOSD_LINE_END_XY_0 (0x40000+(0x1858<<2))//0x46160
//Bit 31:26        reserved                           
//Bit 25:13        reg_line_end_x_0          // unsigned ,    RW, default = 64  end x coordinate of line
//Bit 12: 0        reg_line_end_y_0          // unsigned ,    RW, default = 64  end y coordinate of line

#define FOSD_LINE_RGBA_0 (0x40000+(0x1859<<2))//0x46164
//Bit 31:24        reg_line_alpha_0          // unsigned ,    RW, default = 128  blending alpha of line
//Bit 23:16        reg_line_r_0              // unsigned ,    RW, default = 128  R value of line
//Bit 15: 8        reg_line_g_0              // unsigned ,    RW, default = 128  G value of line
//Bit  7: 0        reg_line_b_0              // unsigned ,    RW, default = 128  B value of line

#define FOSD_LINE_K_1 (0x40000+(0x185a<<2))//0x46168
//Bit 31: 0        reg_line_grad_1           // unsigned ,    RW, default = 1  line gradient, dx=(reg_line_end_x - reg_line_start_x), dy=(reg_line_end_y - reg_line_start_y), if abs(dx) > abs(dy), reg_line_grad=((abs(dy)<<16) / dx), else if abs(dx) < abs(dy), reg_line_grad=((abs(dx)<<16) / dy), should be calculated by firmware

#define FOSD_LINE_START_XY_1 (0x40000+(0x185b<<2))//0x4616c
//Bit 31            reserved                           
//Bit 30:18        reg_line_start_x_1        // unsigned ,    RW, default = 0  start x coordinate of line
//Bit 17: 5        reg_line_start_y_1        // unsigned ,    RW, default = 0  start y coordinate of line
//Bit  4           reg_each_line_en_1        // unsigned ,    RW, default = 0  each line enable
//Bit  3: 0        reg_line_width_1          // unsigned ,    RW, default = 4  thick of line

#define FOSD_LINE_END_XY_1 (0x40000+(0x185c<<2))//0x46170
//Bit 31:26        reserved                           
//Bit 25:13        reg_line_end_x_1          // unsigned ,    RW, default = 64  end x coordinate of line
//Bit 12: 0        reg_line_end_y_1          // unsigned ,    RW, default = 64  end y coordinate of line

#define FOSD_LINE_RGBA_1 (0x40000+(0x185d<<2))//0x46174
//Bit 31:24        reg_line_alpha_1          // unsigned ,    RW, default = 128  blending alpha of line
//Bit 23:16        reg_line_r_1              // unsigned ,    RW, default = 128  R value of line
//Bit 15: 8        reg_line_g_1              // unsigned ,    RW, default = 128  G value of line
//Bit  7: 0        reg_line_b_1              // unsigned ,    RW, default = 128  B value of line

#define FOSD_LINE_K_2 (0x40000+(0x185e<<2))//0x46178
//Bit 31: 0        reg_line_grad_2           // unsigned ,    RW, default = 1  line gradient, dx=(reg_line_end_x - reg_line_start_x), dy=(reg_line_end_y - reg_line_start_y), if abs(dx) > abs(dy), reg_line_grad=((abs(dy)<<16) / dx), else if abs(dx) < abs(dy), reg_line_grad=((abs(dx)<<16) / dy), should be calculated by firmware

#define FOSD_LINE_START_XY_2 (0x40000+(0x185f<<2))//0x4617c
//Bit 31            reserved                           
//Bit 30:18        reg_line_start_x_2        // unsigned ,    RW, default = 0  start x coordinate of line
//Bit 17: 5        reg_line_start_y_2        // unsigned ,    RW, default = 0  start y coordinate of line
//Bit  4           reg_each_line_en_2        // unsigned ,    RW, default = 0  each line enable
//Bit  3: 0        reg_line_width_2          // unsigned ,    RW, default = 4  thick of line

#define FOSD_LINE_END_XY_2 (0x40000+(0x1860<<2))//0x46180
//Bit 31:26        reserved                           
//Bit 25:13        reg_line_end_x_2          // unsigned ,    RW, default = 64  end x coordinate of line
//Bit 12: 0        reg_line_end_y_2          // unsigned ,    RW, default = 64  end y coordinate of line

#define FOSD_LINE_RGBA_2 (0x40000+(0x1861<<2))//0x46184
//Bit 31:24        reg_line_alpha_2          // unsigned ,    RW, default = 128  blending alpha of line
//Bit 23:16        reg_line_r_2              // unsigned ,    RW, default = 128  R value of line
//Bit 15: 8        reg_line_g_2              // unsigned ,    RW, default = 128  G value of line
//Bit  7: 0        reg_line_b_2              // unsigned ,    RW, default = 128  B value of line

#define FOSD_LINE_K_3 (0x40000+(0x1862<<2))//0x46188
//Bit 31: 0        reg_line_grad_3           // unsigned ,    RW, default = 1  line gradient, dx=(reg_line_end_x - reg_line_start_x), dy=(reg_line_end_y - reg_line_start_y), if abs(dx) > abs(dy), reg_line_grad=((abs(dy)<<16) / dx), else if abs(dx) < abs(dy), reg_line_grad=((abs(dx)<<16) / dy), should be calculated by firmware

#define FOSD_LINE_START_XY_3 (0x40000+(0x1863<<2))//0x4618c
//Bit 31            reserved                           
//Bit 30:18        reg_line_start_x_3        // unsigned ,    RW, default = 0  start x coordinate of line
//Bit 17: 5        reg_line_start_y_3        // unsigned ,    RW, default = 0  start y coordinate of line
//Bit  4           reg_each_line_en_3        // unsigned ,    RW, default = 0  each line enable
//Bit  3: 0        reg_line_width_3          // unsigned ,    RW, default = 4  thick of line

#define FOSD_LINE_END_XY_3 (0x40000+(0x1864<<2))//0x46190
//Bit 31:26        reserved                           
//Bit 25:13        reg_line_end_x_3          // unsigned ,    RW, default = 64  end x coordinate of line
//Bit 12: 0        reg_line_end_y_3          // unsigned ,    RW, default = 64  end y coordinate of line

#define FOSD_LINE_RGBA_3 (0x40000+(0x1865<<2))//0x46194
//Bit 31:24        reg_line_alpha_3          // unsigned ,    RW, default = 128  blending alpha of line
//Bit 23:16        reg_line_r_3              // unsigned ,    RW, default = 128  R value of line
//Bit 15: 8        reg_line_g_3              // unsigned ,    RW, default = 128  G value of line
//Bit  7: 0        reg_line_b_3              // unsigned ,    RW, default = 128  B value of line

#define FOSD_LINE_K_4 (0x40000+(0x1866<<2))//0x46198
//Bit 31: 0        reg_line_grad_4           // unsigned ,    RW, default = 1  line gradient, dx=(reg_line_end_x - reg_line_start_x), dy=(reg_line_end_y - reg_line_start_y), if abs(dx) > abs(dy), reg_line_grad=((abs(dy)<<16) / dx), else if abs(dx) < abs(dy), reg_line_grad=((abs(dx)<<16) / dy), should be calculated by firmware

#define FOSD_LINE_START_XY_4 (0x40000+(0x1867<<2))//0x4619c
//Bit 31            reserved                           
//Bit 30:18        reg_line_start_x_4        // unsigned ,    RW, default = 0  start x coordinate of line
//Bit 17: 5        reg_line_start_y_4        // unsigned ,    RW, default = 0  start y coordinate of line
//Bit  4           reg_each_line_en_4        // unsigned ,    RW, default = 0  each line enable
//Bit  3: 0        reg_line_width_4          // unsigned ,    RW, default = 4  thick of line

#define FOSD_LINE_END_XY_4 (0x40000+(0x1868<<2))//0x461a0
//Bit 31:26        reserved                           
//Bit 25:13        reg_line_end_x_4          // unsigned ,    RW, default = 64  end x coordinate of line
//Bit 12: 0        reg_line_end_y_4          // unsigned ,    RW, default = 64  end y coordinate of line

#define FOSD_LINE_RGBA_4 (0x40000+(0x1869<<2))//0x461a4
//Bit 31:24        reg_line_alpha_4          // unsigned ,    RW, default = 128  blending alpha of line
//Bit 23:16        reg_line_r_4              // unsigned ,    RW, default = 128  R value of line
//Bit 15: 8        reg_line_g_4              // unsigned ,    RW, default = 128  G value of line
//Bit  7: 0        reg_line_b_4              // unsigned ,    RW, default = 128  B value of line


#define FOSD_FONT_UPDATE_CRTL (0x40000+(0x1880<<2))//0x46200
//Bit 31:16     reserved 
//Bit 15:8      reg_font_update_en       //unsigned, RW,  default = 0 , 1 : update_font   0:dis    ,[win11,win10,...win0]
//Bit  7:0      reg_font_update_mode     //unsigned, RW,  default = 0 , 1 : update_win_active_area  0: update_total_win [win11,win10,...win0]

#define FOSD_FONT_GEN_STATUS (0x40000+(0x1881<<2))//0x46204
//Bit 31:27     reserved 
//Bit 26        ro_font_gen_idle              //unsigned ,    RO, default = 1, font_gen idle in  vertical  
//Bit 25:13     ro_font_gen_vcnt              //unsigned ,    RO, default = 0, font_gen vcnt
//Bit 12: 0     ro_font_gen_hcnt              //unsigned ,    RO, default = 0, font_gen hcnt 

#define FOSD_GCLK_CTRL0 (0x40000+(0x1882<<2))//0x46208
//Bit 31: 0     reg_gclk_ctrl0             //unsigned ,    RW, default = 0 

#define FOSD_GCLK_CTRL1 (0x40000+(0x1883<<2))//0x4620c
//Bit 31: 0     reg_gclk_ctrl1             //unsigned ,    RW, default = 0

#define FOSD_FONT_ROTATE_CTRL (0x40000+(0x1884<<2))//0x46210
//Bit 31: 2     reserved 
//Bit  1: 1     reg_font_rot_clr      //unsigned, RW, default = 0 , clr font rotate   
//Bit  0: 0     reg_font_rot_start    //unsigned, RW, default = 0 , font_rotate start   

#define FOSD_FONT_UPDATE_RGBA_0 (0x40000+(0x1885<<2))//0x46214
//Bit 31:24        reg_font_upd_alpha_0            // unsigned ,    RW, default = 0  blending alpha of font ,enable when reg_font_update_en[0] ==1 
//Bit 23:16        reg_font_upd_r_0                // unsigned ,    RW, default = 0  R value of font,enable when reg_font_update_en[0] ==1
//Bit 15: 8        reg_font_upd_g_0                // unsigned ,    RW, default = 0  G value of font,enable when reg_font_update_en[0] ==1
//Bit  7: 0        reg_font_upd_b_0                // unsigned ,    RW, default = 0  B value of font,enable when reg_font_update_en[0] ==1

#define FOSD_FONT_UPDATE_RGBA_1 (0x40000+(0x1886<<2))//0x46218
//Bit 31:24        reg_font_upd_alpha_1            // unsigned ,    RW, default = 0  blending alpha of font ,enable when reg_font_update_en[1] ==1 
//Bit 23:16        reg_font_upd_r_1                // unsigned ,    RW, default = 0  R value of font,enable when reg_font_update_en[1] ==1
//Bit 15: 8        reg_font_upd_g_1                // unsigned ,    RW, default = 0  G value of font,enable when reg_font_update_en[1] ==1
//Bit  7: 0        reg_font_upd_b_1                // unsigned ,    RW, default = 0  B value of font,enable when reg_font_update_en[1] ==1

#define FOSD_FONT_UPDATE_RGBA_2 (0x40000+(0x1887<<2))//0x4621c
//Bit 31:24        reg_font_upd_alpha_2            // unsigned ,    RW, default = 0  blending alpha of font ,enable when reg_font_update_en[2] ==1 
//Bit 23:16        reg_font_upd_r_2                // unsigned ,    RW, default = 0  R value of font,enable when reg_font_update_en[2] ==1
//Bit 15: 8        reg_font_upd_g_2                // unsigned ,    RW, default = 0  G value of font,enable when reg_font_update_en[2] ==1
//Bit  7: 0        reg_font_upd_b_2                // unsigned ,    RW, default = 0  B value of font,enable when reg_font_update_en[2] ==1

#define FOSD_FONT_UPDATE_RGBA_3 (0x40000+(0x1888<<2))//0x46220
//Bit 31:24        reg_font_upd_alpha_3            // unsigned ,    RW, default = 0  blending alpha of font ,enable when reg_font_update_en[3] ==1 
//Bit 23:16        reg_font_upd_r_3                // unsigned ,    RW, default = 0  R value of font,enable when reg_font_update_en[3] ==1
//Bit 15: 8        reg_font_upd_g_3                // unsigned ,    RW, default = 0  G value of font,enable when reg_font_update_en[3] ==1
//Bit  7: 0        reg_font_upd_b_3                // unsigned ,    RW, default = 0  B value of font,enable when reg_font_update_en[3] ==1

#define FOSD_FONT_UPDATE_RGBA_4 (0x40000+(0x1889<<2))//0x46224
//Bit 31:24        reg_font_upd_alpha_4            // unsigned ,    RW, default = 0  blending alpha of font ,enable when reg_font_update_en[4] ==1 
//Bit 23:16        reg_font_upd_r_4                // unsigned ,    RW, default = 0  R value of font,enable when reg_font_update_en[4] ==1
//Bit 15: 8        reg_font_upd_g_4                // unsigned ,    RW, default = 0  G value of font,enable when reg_font_update_en[4] ==1
//Bit  7: 0        reg_font_upd_b_4                // unsigned ,    RW, default = 0  B value of font,enable when reg_font_update_en[4] ==1

#define FOSD_FONT_UPDATE_RGBA_5 (0x40000+(0x188a<<2))//0x46228
//Bit 31:24        reg_font_upd_alpha_5            // unsigned ,    RW, default = 0  blending alpha of font ,enable when reg_font_update_en[5] ==1 
//Bit 23:16        reg_font_upd_r_5                // unsigned ,    RW, default = 0  R value of font,enable when reg_font_update_en[5] ==1
//Bit 15: 8        reg_font_upd_g_5                // unsigned ,    RW, default = 0  G value of font,enable when reg_font_update_en[5] ==1
//Bit  7: 0        reg_font_upd_b_5                // unsigned ,    RW, default = 0  B value of font,enable when reg_font_update_en[5] ==1

#define FOSD_FONT_UPDATE_RGBA_6 (0x40000+(0x188b<<2))//0x4622c
//Bit 31:24        reg_font_upd_alpha_6            // unsigned ,    RW, default = 0  blending alpha of font ,enable when reg_font_update_en[6] ==1 
//Bit 23:16        reg_font_upd_r_6                // unsigned ,    RW, default = 0  R value of font,enable when reg_font_update_en[6] ==1
//Bit 15: 8        reg_font_upd_g_6                // unsigned ,    RW, default = 0  G value of font,enable when reg_font_update_en[6] ==1
//Bit  7: 0        reg_font_upd_b_6                // unsigned ,    RW, default = 0  B value of font,enable when reg_font_update_en[6] ==1

#define FOSD_FONT_UPDATE_RGBA_7 (0x40000+(0x188c<<2))//0x46230
//Bit 31:24        reg_font_upd_alpha_7            // unsigned ,    RW, default = 0  blending alpha of font ,enable when reg_font_update_en[7] ==1 
//Bit 23:16        reg_font_upd_r_7                // unsigned ,    RW, default = 0  R value of font,enable when reg_font_update_en[7] ==1
//Bit 15: 8        reg_font_upd_g_7                // unsigned ,    RW, default = 0  G value of font,enable when reg_font_update_en[7] ==1
//Bit  7: 0        reg_font_upd_b_7                // unsigned ,    RW, default = 0  B value of font,enable when reg_font_update_en[7] ==1

#define FOSD_FONT_UPDATE_LT_XY_0 (0x40000+(0x188d<<2))//0x46234
//Bit 31:26        reserved                    
//Bit 25:13        reg_font_upd_lt_x_0     // unsigned ,    RW, default = 0  top-left x coordinate of window when reg_font_update_mode = 1
//Bit 12: 0        reg_font_upd_lt_y_0     // unsigned ,    RW, default = 0  top-left y coordinate of window when reg_font_update_mode = 1

#define FOSD_FONT_UPDATE_RB_XY_0 (0x40000+(0x188e<<2))//0x46238
//Bit 31:26        reserved                    
//Bit 25:13        reg_font_upd_rb_x_0     // unsigned ,    RW, default = 2  right-bottom x coordinate of window when reg_font_update_mode = 1
//Bit 12: 0        reg_font_upd_rb_y_0     // unsigned ,    RW, default = 2  right-bottom y coordinate of window when reg_font_update_mode = 1

#define FOSD_FONT_UPDATE_LT_XY_1 (0x40000+(0x188f<<2))//0x4623c
//Bit 31:26        reserved                    
//Bit 25:13        reg_font_upd_lt_x_1     // unsigned ,    RW, default = 0  top-left x coordinate of window when reg_font_update_mode = 1
//Bit 12: 0        reg_font_upd_lt_y_1     // unsigned ,    RW, default = 0  top-left y coordinate of window when reg_font_update_mode = 1

#define FOSD_FONT_UPDATE_RB_XY_1 (0x40000+(0x1890<<2))//0x46240
//Bit 31:26        reserved                    
//Bit 25:13        reg_font_upd_rb_x_1     // unsigned ,    RW, default = 2  right-bottom x coordinate of window when reg_font_update_mode = 1
//Bit 12: 0        reg_font_upd_rb_y_1     // unsigned ,    RW, default = 2  right-bottom y coordinate of window when reg_font_update_mode = 1

#define FOSD_FONT_UPDATE_LT_XY_2 (0x40000+(0x1891<<2))//0x46244
//Bit 31:26        reserved                    
//Bit 25:13        reg_font_upd_lt_x_2     // unsigned ,    RW, default = 0  top-left x coordinate of window when reg_font_update_mode = 1
//Bit 12: 0        reg_font_upd_lt_y_2     // unsigned ,    RW, default = 0  top-left y coordinate of window when reg_font_update_mode = 1

#define FOSD_FONT_UPDATE_RB_XY_2 (0x40000+(0x1892<<2))//0x46248
//Bit 31:26        reserved                    
//Bit 25:13        reg_font_upd_rb_x_2     // unsigned ,    RW, default = 2  right-bottom x coordinate of window when reg_font_update_mode = 1
//Bit 12: 0        reg_font_upd_rb_y_2     // unsigned ,    RW, default = 2  right-bottom y coordinate of window when reg_font_update_mode = 1

#define FOSD_FONT_UPDATE_LT_XY_3 (0x40000+(0x1893<<2))//0x4624c
//Bit 31:26        reserved                    
//Bit 25:13        reg_font_upd_lt_x_3     // unsigned ,    RW, default = 0  top-left x coordinate of window when reg_font_update_mode = 1
//Bit 12: 0        reg_font_upd_lt_y_3     // unsigned ,    RW, default = 0  top-left y coordinate of window when reg_font_update_mode = 1

#define FOSD_FONT_UPDATE_RB_XY_3 (0x40000+(0x1894<<2))//0x46250
//Bit 31:26        reserved                    
//Bit 25:13        reg_font_upd_rb_x_3     // unsigned ,    RW, default = 2  right-bottom x coordinate of window when reg_font_update_mode = 1
//Bit 12: 0        reg_font_upd_rb_y_3     // unsigned ,    RW, default = 2  right-bottom y coordinate of window when reg_font_update_mode = 1

#define FOSD_FONT_UPDATE_LT_XY_4 (0x40000+(0x1895<<2))//0x46254
//Bit 31:26        reserved                    
//Bit 25:13        reg_font_upd_lt_x_4     // unsigned ,    RW, default = 0  top-left x coordinate of window when reg_font_update_mode = 1
//Bit 12: 0        reg_font_upd_lt_y_4     // unsigned ,    RW, default = 0  top-left y coordinate of window when reg_font_update_mode = 1

#define FOSD_FONT_UPDATE_RB_XY_4 (0x40000+(0x1896<<2))//0x46258
//Bit 31:26        reserved                    
//Bit 25:13        reg_font_upd_rb_x_4     // unsigned ,    RW, default = 2  right-bottom x coordinate of window when reg_font_update_mode = 1
//Bit 12: 0        reg_font_upd_rb_y_4     // unsigned ,    RW, default = 2  right-bottom y coordinate of window when reg_font_update_mode = 1

#define FOSD_FONT_UPDATE_LT_XY_5 (0x40000+(0x1897<<2))//0x4625c
//Bit 31:26        reserved                    
//Bit 25:13        reg_font_upd_lt_x_5     // unsigned ,    RW, default = 0  top-left x coordinate of window when reg_font_update_mode = 1
//Bit 12: 0        reg_font_upd_lt_y_5     // unsigned ,    RW, default = 0  top-left y coordinate of window when reg_font_update_mode = 1

#define FOSD_FONT_UPDATE_RB_XY_5 (0x40000+(0x1898<<2))//0x46260
//Bit 31:26        reserved                    
//Bit 25:13        reg_font_upd_rb_x_5     // unsigned ,    RW, default = 2  right-bottom x coordinate of window when reg_font_update_mode = 1
//Bit 12: 0        reg_font_upd_rb_y_5     // unsigned ,    RW, default = 2  right-bottom y coordinate of window when reg_font_update_mode = 1

#define FOSD_FONT_UPDATE_LT_XY_6 (0x40000+(0x1899<<2))//0x46264
//Bit 31:26        reserved                    
//Bit 25:13        reg_font_upd_lt_x_6     // unsigned ,    RW, default = 0  top-left x coordinate of window when reg_font_update_mode = 1
//Bit 12: 0        reg_font_upd_lt_y_6     // unsigned ,    RW, default = 0  top-left y coordinate of window when reg_font_update_mode = 1

#define FOSD_FONT_UPDATE_RB_XY_6 (0x40000+(0x189a<<2))//0x46268
//Bit 31:26        reserved                    
//Bit 25:13        reg_font_upd_rb_x_6     // unsigned ,    RW, default = 2  right-bottom x coordinate of window when reg_font_update_mode = 1
//Bit 12: 0        reg_font_upd_rb_y_6     // unsigned ,    RW, default = 2  right-bottom y coordinate of window when reg_font_update_mode = 1

#define FOSD_FONT_UPDATE_LT_XY_7 (0x40000+(0x189b<<2))//0x4626c
//Bit 31:26        reserved                    
//Bit 25:13        reg_font_upd_lt_x_7     // unsigned ,    RW, default = 0  top-left x coordinate of window when reg_font_update_mode = 1
//Bit 12: 0        reg_font_upd_lt_y_7     // unsigned ,    RW, default = 0  top-left y coordinate of window when reg_font_update_mode = 1

#define FOSD_FONT_UPDATE_RB_XY_7 (0x40000+(0x189c<<2))//0x46270
//Bit 31:26        reserved                    
//Bit 25:13        reg_font_upd_rb_x_7     // unsigned ,    RW, default = 2  right-bottom x coordinate of window when reg_font_update_mode = 1
//Bit 12: 0        reg_font_upd_rb_y_7     // unsigned ,    RW, default = 2  right-bottom y coordinate of window when reg_font_update_mode = 1

#define FOSD_WIN_INVALID_AREA_RGBA (0x40000+(0x189d<<2))//0x46274
//Bit 31:24        reg_win_invalid_area_alpha            // unsigned ,    RW, default = 0  blending alpha of font ,enable when win_invalid_area 
//Bit 23:16        reg_win_invalid_area_r                // unsigned ,    RW, default = 0  R value of font,enable when win_invalid_area
//Bit 15: 8        reg_win_invalid_area_g                // unsigned ,    RW, default = 0  G value of font,enable when win_invalid_area
//Bit  7: 0        reg_win_invalid_area_b                // unsigned ,    RW, default = 0  B value of font,enable when win_invalid_area

#define FOSD_RECT_INVALID_AREA_RGBA (0x40000+(0x189e<<2))//0x46278
//Bit 31:24        reg_rect_invalid_area_alpha            // unsigned ,    RW, default = 0  blending alpha of font ,enable when rect_invalid_area 
//Bit 23:16        reg_rect_invalid_area_r                // unsigned ,    RW, default = 0  R value of font,enable when rect_invalid_area
//Bit 15: 8        reg_rect_invalid_area_g                // unsigned ,    RW, default = 0  G value of font,enable when rect_invalid_area
//Bit  7: 0        reg_rect_invalid_area_b                // unsigned ,    RW, default = 0  B value of font,enable when rect_invalid_area

#define FOSD_LINE_INVALID_AREA_RGBA (0x40000+(0x189f<<2))//0x4627c
//Bit 31:24        reg_line_invalid_area_alpha            // unsigned ,    RW, default = 0  blending alpha of font ,enable when line_invalid_area 
//Bit 23:16        reg_line_invalid_area_r                // unsigned ,    RW, default = 0  R value of font,enable when line_invalid_area
//Bit 15: 8        reg_line_invalid_area_g                // unsigned ,    RW, default = 0  G value of font,enable when line_invalid_area
//Bit  7: 0        reg_line_invalid_area_b                // unsigned ,    RW, default = 0  B value of font,enable when line_invalid_area

#define FOSD_RECT_ELLIP_OVLP_MODE (0x40000+(0x18a0<<2))//0x46280
//Bit 31:1      reserved 
//Bit 0         reg_rect_ellip_ovlp_mode                 //unsigned, RW,  default = 0 , 1 : [rect,ellip] 0:[ellip,rect] 

#define FOSD_WIN_IDX_OFST_0 (0x40000+(0x18a1<<2))//0x46284
//Bit 31:27        reserved                           
//Bit 26           reg_osd_win_crop_en_0             // unsigned ,    RW, default = 0 1: enable osd_win_crop 0:dis osd_win_crop 
//Bit 25:13        reg_osd_win_idx_hofst_0           // unsigned ,    RW, default = 0  horizontal start font position in window
//Bit 12: 0        reg_osd_win_idx_vofst_0           // unsigned ,    RW, default = 0  vertical start font position in window

#define FOSD_WIN_IDX_OFST_1 (0x40000+(0x18a2<<2))//0x46288
//Bit 31:27        reserved                           
//Bit 26           reg_osd_win_crop_en_1             // unsigned ,    RW, default = 0 1: enable osd_win_crop 0:dis osd_win_crop                         
//Bit 25:13        reg_osd_win_idx_hofst_1           // unsigned ,    RW, default = 0  horizontal start font position in window
//Bit 12: 0        reg_osd_win_idx_vofst_1           // unsigned ,    RW, default = 0  vertical start font position in window

#define FOSD_WIN_IDX_OFST_2 (0x40000+(0x18a3<<2))//0x4628c
//Bit 31:27        reserved                           
//Bit 26           reg_osd_win_crop_en_2             // unsigned ,    RW, default = 0 1: enable osd_win_crop 0:dis osd_win_crop 
//Bit 25:13        reg_osd_win_idx_hofst_2           // unsigned ,    RW, default = 0  horizontal start font position in window
//Bit 12: 0        reg_osd_win_idx_vofst_2           // unsigned ,    RW, default = 0  vertical start font position in window

#define FOSD_WIN_IDX_OFST_3 (0x40000+(0x18a4<<2))//0x46290
//Bit 31:27        reserved                           
//Bit 26           reg_osd_win_crop_en_3             // unsigned ,    RW, default = 0 1: enable osd_win_crop 0:dis osd_win_crop 
//Bit 25:13        reg_osd_win_idx_hofst_3           // unsigned ,    RW, default = 0  horizontal start font position in window
//Bit 12: 0        reg_osd_win_idx_vofst_3           // unsigned ,    RW, default = 0  vertical start font position in window

#define FOSD_WIN_IDX_OFST_4 (0x40000+(0x18a5<<2))//0x46294
//Bit 31:27        reserved                           
//Bit 26           reg_osd_win_crop_en_4             // unsigned ,    RW, default = 0 1: enable osd_win_crop 0:dis osd_win_crop 
//Bit 25:13        reg_osd_win_idx_hofst_4           // unsigned ,    RW, default = 0  horizontal start font position in window
//Bit 12: 0        reg_osd_win_idx_vofst_4           // unsigned ,    RW, default = 0  vertical start font position in window

#define FOSD_WIN_IDX_OFST_5 (0x40000+(0x18a6<<2))//0x46298
//Bit 31:27        reserved                           
//Bit 26           reg_osd_win_crop_en_5             // unsigned ,    RW, default = 0 1: enable osd_win_crop 0:dis osd_win_crop 
//Bit 25:13        reg_osd_win_idx_hofst_5           // unsigned ,    RW, default = 0  horizontal start font position in window
//Bit 12: 0        reg_osd_win_idx_vofst_5           // unsigned ,    RW, default = 0  vertical start font position in window

#define FOSD_WIN_IDX_OFST_6 (0x40000+(0x18a7<<2))//0x4629c
//Bit 31:27        reserved                           
//Bit 26           reg_osd_win_crop_en_6             // unsigned ,    RW, default = 0 1: enable osd_win_crop 0:dis osd_win_crop 
//Bit 25:13        reg_osd_win_idx_hofst_6           // unsigned ,    RW, default = 0  horizontal start font position in window
//Bit 12: 0        reg_osd_win_idx_vofst_6           // unsigned ,    RW, default = 0  vertical start font position in window

#define FOSD_WIN_IDX_OFST_7 (0x40000+(0x18a8<<2))//0x462a0
//Bit 31:27        reserved                           
//Bit 26           reg_osd_win_crop_en_7             // unsigned ,    RW, default = 0 1: enable osd_win_crop 0:dis osd_win_crop 
//Bit 25:13        reg_osd_win_idx_hofst_7           // unsigned ,    RW, default = 0  horizontal start font position in window
//Bit 12: 0        reg_osd_win_idx_vofst_7           // unsigned ,    RW, default = 0  vertical start font position in window

#define FOSD_WIN_IDX_STRIDE_0 (0x40000+(0x18a9<<2))//0x462a4
//Bit 31:13        reserved                           
//Bit 12: 0        reg_osd_win_stride_0          // unsigned ,    RW, default = 10 horizontal font stride in window 0

#define FOSD_WIN_IDX_STRIDE_1 (0x40000+(0x18aa<<2))//0x462a8
//Bit 31:13        reserved                                                                                          
//Bit 12: 0        reg_osd_win_stride_1          // unsigned ,    RW, default = 10 horizontal font stride in window 1

#define FOSD_WIN_IDX_STRIDE_2 (0x40000+(0x18ab<<2))//0x462ac
//Bit 31:13        reserved                                                                                          
//Bit 12: 0        reg_osd_win_stride_2          // unsigned ,    RW, default = 10 horizontal font stride in window 2

#define FOSD_WIN_IDX_STRIDE_3 (0x40000+(0x18ac<<2))//0x462b0
//Bit 31:13        reserved                                                                                          
//Bit 12: 0        reg_osd_win_stride_3          // unsigned ,    RW, default = 10 horizontal font stride in window 3

#define FOSD_WIN_IDX_STRIDE_4 (0x40000+(0x18ad<<2))//0x462b4
//Bit 31:13        reserved                                                                                          
//Bit 12: 0        reg_osd_win_stride_4          // unsigned ,    RW, default = 10 horizontal font stride in window 4

#define FOSD_WIN_IDX_STRIDE_5 (0x40000+(0x18ae<<2))//0x462b8
//Bit 31:13        reserved                                                                                          
//Bit 12: 0        reg_osd_win_stride_5          // unsigned ,    RW, default = 10 horizontal font stride in window 5

#define FOSD_WIN_IDX_STRIDE_6 (0x40000+(0x18af<<2))//0x462bc
//Bit 31:13        reserved                                                                                          
//Bit 12: 0        reg_osd_win_stride_6          // unsigned ,    RW, default = 10 horizontal font stride in window 6

#define FOSD_WIN_IDX_STRIDE_7 (0x40000+(0x18b0<<2))//0x462c0
//Bit 31:13        reserved                                                                                          
//Bit 12: 0        reg_osd_win_stride_7          // unsigned ,    RW, default = 10 horizontal font stride in window 7

#define FOSD_RAM_UPD_STATUS (0x40000+(0x18b1<<2))//0x462c4
//Bit 31:16         ro_upd_fifo_fault_cnt                      //unsigned ,    RO, default = 0, 
//Bit 15: 0         ro_share_ram_access_fault_cnt              //unsigned ,    RO, default = 0,   

#define FOSD_RAM_UPD_STATUS_CTRL (0x40000+(0x18b2<<2))//0x462c8
//Bit 31:0          reg_ram_cnt_sw_rst                        //unsigned ,    RW, default = 0,FOSD_RAM_UPD_STATUS clr  

#define FOSD_RAM_UPD_CRTL (0x40000+(0x18b3<<2))//0x462cc
//Bit 31:2      reserved 
//Bit 1         reg_upd_fifo_mode        //unsigned, RW,  default = 0 , 1 :color_ram  0: share_ram     
//Bit 0         reg_upd_fifo_en          //unsigned, RW,  default = 1 , 1 :enable  0: disable     

#define FOSD_FONT_RRDY_NO_SRDY_CNT (0x40000+(0x18b4<<2))//0x462d0
//Bit 31:0      ro_font_rrdynosrdy_cnt   //unsigned, RO,  default = 0 

#define FOSD_RECT_RRDY_NO_SRDY_CNT (0x40000+(0x18b5<<2))//0x462d4
//Bit 31:0      ro_rect_rrdynosrdy_cnt   //unsigned, RO,  default = 0 

#define FOSD_LINE_RRDY_NO_SRDY_CNT (0x40000+(0x18b6<<2))//0x462d8
//Bit 31:0      ro_line_rrdynosrdy_cnt   //unsigned, RO,  default = 0 

#define FOSD_FRM_CTRL (0x40000+(0x18b7<<2))//0x462dc
//Bit 31:1      reserved 
//Bit 0         reg_hw_osd_en               //unsigned, RW,  default = 1 , 1 :enable  0: disable   

#define FOSD_FONT_REPACK_CTRL (0x40000+(0x18b8<<2))//0x462e0
//Bit 31: 1     reserved 
//Bit  0: 0     reg_font_repack_start    //unsigned, RW, default = 0 , font_repack start   

#define FOSD_FONT_REPACK_GAP (0x40000+(0x18b9<<2))//0x462e4
//Bit 31: 4     reserved 
//Bit  3: 0     reg_font_gap             //unsigned, RW, default = 1 , font_repack font_gap   


#define FOSD_SHARE_RAM_ADDR (0x40000+(0x18dc<<2))//0x46370
//Bit 31:14     reserved 
//Bit 13: 0     reg_share_ram_addr    //unsigned ,    RW, default = 0, share ram wr & rd addr

#define FOSD_SHARE_RAM_DATA (0x40000+(0x18dd<<2))//0x46374
//Bit 31:24     reserved 
//Bit 23: 0     reg_share_ram_data    //unsigned ,    RW, default = 0, share ram wdata & rdata

#define FOSD_COLOR_RAM_ADDR (0x40000+(0x18de<<2))//0x46378
//Bit 31: 6     reserved 
//Bit  5: 0     reg_share_ram_addr    //unsigned ,    RW, default = 0, color ram wr & rd addr

#define FOSD_COLOR_RAM_DATA (0x40000+(0x18df<<2))//0x4637c
//Bit 31:25     reserved 
//Bit 24: 0     reg_color_ram_data    //unsigned ,    RW, default = 0, color ram wdata & rdata {alpha[0:0],R[7:0],G[7:0],B[7:0]}


#define FOSD_MATRIX_COEF00_01 (0x40000+(0x18e0<<2))//0x46380
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef00        // signed ,    RW, default = 1023  reg_mtrxi_coef_00
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef01        // signed ,    RW, default = 0  reg_mtrxi_coef_01

#define FOSD_MATRIX_COEF02_10 (0x40000+(0x18e1<<2))//0x46384
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef02        // signed ,    RW, default = 1510  reg_mtrxi_coef_02
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef10        // signed ,    RW, default = 1023  reg_mtrxi_coef_10

#define FOSD_MATRIX_COEF11_12 (0x40000+(0x18e2<<2))//0x46388
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef11        // signed ,    RW, default = -168  reg_mtrxi_coef_11
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef12        // signed ,    RW, default = -585  reg_mtrxi_coef_12

#define FOSD_MATRIX_COEF20_21 (0x40000+(0x18e3<<2))//0x4638c
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef20        // signed ,    RW, default = 1023  reg_mtrxi_coef_20
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef21        // signed ,    RW, default = 1926  reg_mtrxi_coef_21

#define FOSD_MATRIX_COEF22 (0x40000+(0x18e4<<2))//0x46390
//Bit 31:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef22        // signed ,    RW, default = 0  reg_mtrxi_coef_22

#define FOSD_MATRIX_COEF30_31 (0x40000+(0x18e5<<2))//0x46394
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef30        // signed ,    RW, default = 0  reg_mtrxi_coef_30
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef31        // signed ,    RW, default = 0  reg_mtrxi_coef_31

#define FOSD_MATRIX_COEF32_40 (0x40000+(0x18e6<<2))//0x46398
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef32        // signed ,    RW, default = 0  reg_mtrxi_coef_32
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef40        // signed ,    RW, default = 0  reg_mtrxi_coef_40

#define FOSD_MATRIX_COEF41_42 (0x40000+(0x18e7<<2))//0x4639c
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef41        // signed ,    RW, default = 0  reg_mtrxi_coef_41
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef42        // signed ,    RW, default = 0  reg_mtrxi_coef_42

#define FOSD_MATRIX_CLIP (0x40000+(0x18e8<<2))//0x463a0
//Bit 31:20        reserved                           
//Bit 19: 8        reg_mtrx_comp_thrd      // signed ,    RW, default = 0  reg_mtrx_comp_thrd 
//Bit 7 : 5        reg_mtrx_rs             // signed ,    RW, default = 0  reg_mtrx_rs        
//Bit 4 : 3        reg_mtrx_clmod          // signed ,    RW, default = 0  reg_mtrx_clmod     
//Bit 2 : 0        reserved                           

#define FOSD_MATRIX_OFFSET0_1 (0x40000+(0x18e9<<2))//0x463a4
//Bit 31:27        reserved                           
//Bit 26:16        reg_mtrxi_offst_oup0    // signed ,    RW, default = 0  reg_mtrxi_offst_oup_0
//Bit 15:11        reserved                           
//Bit 10: 0        reg_mtrxi_offst_oup1    // signed ,    RW, default = 0  reg_mtrxi_offst_oup_1

#define FOSD_MATRIX_OFFSET2 (0x40000+(0x18ea<<2))//0x463a8
//Bit 31:11        reserved                           
//Bit 10: 0        reg_mtrxi_offst_oup2    // signed ,    RW, default = 0  reg_mtrxi_offst_oup_2

#define FOSD_MATRIX_PRE_OFFSET0_1 (0x40000+(0x18eb<<2))//0x463ac
//Bit 31:27        reserved                           
//Bit 26:16        reg_mtrxi_offst_inp0    // signed ,    RW, default = 0  
//Bit 15:11        reserved                           
//Bit 10: 0        reg_mtrxi_offst_inp1    // signed ,    RW, default = -512  

#define FOSD_MATRIX_PRE_OFFSET2 (0x40000+(0x18ec<<2))//0x463b0
//Bit 31:11        reserved                           
//Bit 10: 0        reg_mtrxi_offst_inp2    // signed ,    RW, default = -512  

#define FOSD_MATRIX_EN_CTRL (0x40000+(0x18ed<<2))//0x463b4
//Bit 31: 8        reserved                           
//Bit  7: 0        reg_MATRIX_en_ctrl     // unsigned ,    RW, default = 0  


#define RECT_MATRIX_COEF00_01 (0x40000+(0x18f0<<2))//0x463c0
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef00        // signed ,    RW, default = 1023  reg_mtrxi_coef_00
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef01        // signed ,    RW, default = 0  reg_mtrxi_coef_01

#define RECT_MATRIX_COEF02_10 (0x40000+(0x18f1<<2))//0x463c4
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef02        // signed ,    RW, default = 1510  reg_mtrxi_coef_02
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef10        // signed ,    RW, default = 1023  reg_mtrxi_coef_10

#define RECT_MATRIX_COEF11_12 (0x40000+(0x18f2<<2))//0x463c8
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef11        // signed ,    RW, default = -168  reg_mtrxi_coef_11
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef12        // signed ,    RW, default = -585  reg_mtrxi_coef_12

#define RECT_MATRIX_COEF20_21 (0x40000+(0x18f3<<2))//0x463cc
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef20        // signed ,    RW, default = 1023  reg_mtrxi_coef_20
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef21        // signed ,    RW, default = 1926  reg_mtrxi_coef_21

#define RECT_MATRIX_COEF22 (0x40000+(0x18f4<<2))//0x463d0
//Bit 31:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef22        // signed ,    RW, default = 0  reg_mtrxi_coef_22

#define RECT_MATRIX_COEF30_31 (0x40000+(0x18f5<<2))//0x463d4
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef30        // signed ,    RW, default = 0  reg_mtrxi_coef_30
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef31        // signed ,    RW, default = 0  reg_mtrxi_coef_31

#define RECT_MATRIX_COEF32_40 (0x40000+(0x18f6<<2))//0x463d8
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef32        // signed ,    RW, default = 0  reg_mtrxi_coef_32
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef40        // signed ,    RW, default = 0  reg_mtrxi_coef_40

#define RECT_MATRIX_COEF41_42 (0x40000+(0x18f7<<2))//0x463dc
//Bit 31:29        reserved                           
//Bit 28:16        reg_mtrxi_coef41        // signed ,    RW, default = 0  reg_mtrxi_coef_41
//Bit 15:13        reserved                           
//Bit 12: 0        reg_mtrxi_coef42        // signed ,    RW, default = 0  reg_mtrxi_coef_42

#define RECT_MATRIX_CLIP (0x40000+(0x18f8<<2))//0x463e0
//Bit 31:20        reserved                           
//Bit 19: 8        reg_mtrx_comp_thrd      // signed ,    RW, default = 0  reg_mtrx_comp_thrd 
//Bit 7 : 5        reg_mtrx_rs             // signed ,    RW, default = 0  reg_mtrx_rs        
//Bit 4 : 3        reg_mtrx_clmod          // signed ,    RW, default = 0  reg_mtrx_clmod     
//Bit 2 : 0        reserved                           

#define RECT_MATRIX_OFFSET0_1 (0x40000+(0x18f9<<2))//0x463e4
//Bit 31:27        reserved                           
//Bit 26:16        reg_mtrxi_offst_oup0    // signed ,    RW, default = 0  reg_mtrxi_offst_oup_0
//Bit 15:11        reserved                           
//Bit 10: 0        reg_mtrxi_offst_oup1    // signed ,    RW, default = 0  reg_mtrxi_offst_oup_1

#define RECT_MATRIX_OFFSET2 (0x40000+(0x18fa<<2))//0x463e8
//Bit 31:11        reserved                           
//Bit 10: 0        reg_mtrxi_offst_oup2    // signed ,    RW, default = 0  reg_mtrxi_offst_oup_2

#define RECT_MATRIX_PRE_OFFSET0_1 (0x40000+(0x18fb<<2))//0x463ec
//Bit 31:27        reserved                           
//Bit 26:16        reg_mtrxi_offst_inp0    // signed ,    RW, default = 0  
//Bit 15:11        reserved                           
//Bit 10: 0        reg_mtrxi_offst_inp1    // signed ,    RW, default = -512  

#define RECT_MATRIX_PRE_OFFSET2 (0x40000+(0x18fc<<2))//0x463f0
//Bit 31:11        reserved                           
//Bit 10: 0        reg_mtrxi_offst_inp2    // signed ,    RW, default = -512  

#define RECT_MATRIX_EN_CTRL (0x40000+(0x18fd<<2))//0x463f4
//Bit 31: 8        reserved                           
//Bit  7: 0        reg_MATRIX_en_ctrl     // unsigned ,    RW, default = 0  


#define ENHANCE_SHARP_MISC (0x40000+(0x1a00<<2))//0x46800
//Bit 31: 9        reserved                           
//Bit  8           reg_inp_hold_en           // unsigned ,    RW, default = 0  
//Bit  7: 0        reg_sr_hblank             // unsigned ,    RW, default = 4  

#define ENHANCE_SHARP_GCLK (0x40000+(0x1a01<<2))//0x46804
//Bit 31:0         reg_gclk_ctrl0            // unsigned ,    RW, default = 0   

#define ENHANCE_SHARP_EN (0x40000+(0x1a02<<2))//0x46808
//Bit 31:22        reserved                           
//Bit 21:20        reg_sharp_lpf_mode        // unsigned ,    RW, default = 2  0: 1:2:1   1: 1:1:1   2: 2:2:2
//Bit 19            reserved                           
//Bit 18           reg_sharp_luma_lpf_en     // unsigned ,    RW, default = 1  enable luma do 3x3 lpf
//Bit 17           reg_sharp_high_freq_mode  // unsigned ,    RW, default = 1  high freq mode, 0:{{-1, -1, -1}, {-1,  8, -1}, {-1, -1, -1}} 1:abs
//Bit 16           reg_sharp_high_freq_lpf_en // unsigned ,    RW, default = 1  enable high freq mode do 121 lpf in horizontal
//Bit 15: 4        reg_sharp_high_freq_coring_th // unsigned ,    RW, default = 0  high frequence coring threshold
//Bit  3: 1        reserved                           
//Bit  0           reg_sharp_ampl_mode       // unsigned ,    RW, default = 1  0: amplitude*edge; 1: min(amplitude, edge)

#define ENHANCE_SHARP_LUMA_LOW_TH (0x40000+(0x1a03<<2))//0x4680c
//Bit 31:28        reserved                           
//Bit 27:16        reg_sharp_luma_low_th     // unsigned ,    RW, default = 128  lowest threshold of luma
//Bit 15:10        reserved                           
//Bit  9: 0        reg_sharp_luma_low_slope  // unsigned ,    RW, default = 64  the low slope of curve

#define ENHANCE_SHARP_LUMA_HIGH_TH (0x40000+(0x1a04<<2))//0x46810
//Bit 31:20        reg_sharp_luma_high_th    // unsigned ,    RW, default = 768  highest threshold to calculate amplitude
//Bit 19:18        reserved                           
//Bit 17: 8        reg_sharp_luma_high_slope // unsigned ,    RW, default = 6  the high slope of curve
//Bit  7: 6        reserved                           
//Bit  5: 0        reg_sharp_final_strength  // signed ,    RW, default = 8  sharp strength  normal [-8 31]

#define ENHANCE_SHARP_CLIP (0x40000+(0x1a05<<2))//0x46814
//Bit 31:27        reserved                           
//Bit 26:16        reg_sharp_clip_max        // signed ,    RW, default = 1023  max threshold
//Bit 15:11        reserved                           
//Bit 10: 0        reg_sharp_clip_min        // signed ,    RW, default = -1024  min threshold

#define ENHANCE_SHARP_EDGE_LOW_TH (0x40000+(0x1a06<<2))//0x46818
//Bit 31:28        reserved                           
//Bit 27:16        reg_sharp_edge_low_th     // unsigned ,    RW, default = 10  lowest threshold of luma
//Bit 15:10        reserved                           
//Bit  9: 0        reg_sharp_edge_low_slope  // unsigned ,    RW, default = 256  the low slope of curve

#define ENHANCE_SHARP_EDGE_HIGH_TH (0x40000+(0x1a07<<2))//0x4681c
//Bit 31:28        reserved                           
//Bit 27:16        reg_sharp_edge_high_th    // unsigned ,    RW, default = 250  highest threshold to calculate amplitude
//Bit 15:10        reserved                           
//Bit  9: 0        reg_sharp_edge_high_slope // unsigned ,    RW, default = 16  the high slope of curve


#define ENHANCE_CONTRAST_MISC (0x40000+(0x2000<<2))//0x48000
//Bit 31: 9        reserved    
//Bit  8           reg_lc_set_curve_mode     // unsigned ,    RW, default = 0  0: use lc iir set lut  1: use cbus set lut
//Bit  7: 0        reg_contrast_hblank       // unsigned ,    RW, default = 4           

#define ENHANCE_CONTRAST_GCLK (0x40000+(0x2001<<2))//0x48004
//Bit 31: 0        reg_gclk_ctrl             // unsigned ,    RW, default = 0  

#define ENHANCE_EN_MODE (0x40000+(0x2002<<2))//0x48008
//Bit 31: 5        reserved                           
//Bit  4           reg_dnlp_en               // unsigned ,    RW, default = 0  
//Bit  3: 2        reserved                           
//Bit  1           reg_lc_en                 // unsigned ,    RW, default = 0  enable signal for local contrast enhancement, 1-enable; 0 disable
//Bit  0           reg_lc_blkblend_mode      // unsigned ,    RW, default = 1  use bilinear interpolation between blocks, 0: no interpolation 1: blender enabled

#define ENHANCE_COLOR_PRCT_LUT_0 (0x40000+(0x2003<<2))//0x4800c
//Bit 31:28        reg_contrast_color_prct_lut_7 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_6 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_5 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_4 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_3 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_2 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_1 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_0 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_1 (0x40000+(0x2004<<2))//0x48010
//Bit 31:28        reg_contrast_color_prct_lut_15 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_14 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_13 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_12 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_11 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_10 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_9 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_8 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_2 (0x40000+(0x2005<<2))//0x48014
//Bit 31:28        reg_contrast_color_prct_lut_23 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_22 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_21 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_20 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_19 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_18 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_17 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_16 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_3 (0x40000+(0x2006<<2))//0x48018
//Bit 31:28        reg_contrast_color_prct_lut_31 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_30 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_29 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_28 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_27 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_26 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_25 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_24 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_4 (0x40000+(0x2007<<2))//0x4801c
//Bit 31:28        reg_contrast_color_prct_lut_39 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_38 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_37 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_36 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_35 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_34 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_33 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_32 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_5 (0x40000+(0x2008<<2))//0x48020
//Bit 31:28        reg_contrast_color_prct_lut_47 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_46 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_45 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_44 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_43 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_42 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_41 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_40 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_6 (0x40000+(0x2009<<2))//0x48024
//Bit 31:28        reg_contrast_color_prct_lut_55 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_54 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_53 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_52 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_51 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_50 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_49 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_48 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_7 (0x40000+(0x200a<<2))//0x48028
//Bit 31:28        reg_contrast_color_prct_lut_63 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_62 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_61 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_60 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_59 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_58 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_57 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_56 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_8 (0x40000+(0x200b<<2))//0x4802c
//Bit 31:28        reg_contrast_color_prct_lut_71 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_70 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_69 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_68 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_67 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_66 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_65 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_64 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_9 (0x40000+(0x200c<<2))//0x48030
//Bit 31:28        reg_contrast_color_prct_lut_79 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_78 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_77 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_76 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_75 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_74 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_73 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_72 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_10 (0x40000+(0x200d<<2))//0x48034
//Bit 31:28        reg_contrast_color_prct_lut_87 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_86 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_85 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_84 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_83 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_82 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_81 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_80 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_11 (0x40000+(0x200e<<2))//0x48038
//Bit 31:28        reg_contrast_color_prct_lut_95 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_94 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_93 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_92 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_91 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_90 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_89 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_88 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_12 (0x40000+(0x200f<<2))//0x4803c
//Bit 31:28        reg_contrast_color_prct_lut_103 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_102 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_101 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_100 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_99 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_98 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_97 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_96 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_13 (0x40000+(0x2010<<2))//0x48040
//Bit 31:28        reg_contrast_color_prct_lut_111 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_110 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_109 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_108 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_107 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_106 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_105 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_104 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_14 (0x40000+(0x2011<<2))//0x48044
//Bit 31:28        reg_contrast_color_prct_lut_119 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_118 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_117 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_116 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_115 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_114 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_113 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_112 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_15 (0x40000+(0x2012<<2))//0x48048
//Bit 31:28        reg_contrast_color_prct_lut_127 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_126 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_125 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_124 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_123 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_122 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_121 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_120 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_16 (0x40000+(0x2013<<2))//0x4804c
//Bit 31:28        reg_contrast_color_prct_lut_135 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_134 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_133 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_132 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_131 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_130 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_129 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_128 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_17 (0x40000+(0x2014<<2))//0x48050
//Bit 31:28        reg_contrast_color_prct_lut_143 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_142 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_141 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_140 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_139 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_138 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_137 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_136 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_18 (0x40000+(0x2015<<2))//0x48054
//Bit 31:28        reg_contrast_color_prct_lut_151 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_150 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_149 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_148 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_147 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_146 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_145 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_144 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_19 (0x40000+(0x2016<<2))//0x48058
//Bit 31:28        reg_contrast_color_prct_lut_159 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_158 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_157 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_156 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_155 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_154 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_153 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_152 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_20 (0x40000+(0x2017<<2))//0x4805c
//Bit 31:28        reg_contrast_color_prct_lut_167 // unsigned ,    RW, default = 15  
//Bit 27:24        reg_contrast_color_prct_lut_166 // unsigned ,    RW, default = 12  
//Bit 23:20        reg_contrast_color_prct_lut_165 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_164 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_163 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_162 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_161 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_160 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_21 (0x40000+(0x2018<<2))//0x48060
//Bit 31:28        reg_contrast_color_prct_lut_175 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_174 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_173 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_172 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_171 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_170 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_169 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_168 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_22 (0x40000+(0x2019<<2))//0x48064
//Bit 31:28        reg_contrast_color_prct_lut_183 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_182 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_181 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_180 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_179 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_178 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_177 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_176 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_23 (0x40000+(0x201a<<2))//0x48068
//Bit 31:28        reg_contrast_color_prct_lut_191 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_190 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_189 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_188 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_187 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_186 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_185 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_184 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_24 (0x40000+(0x201b<<2))//0x4806c
//Bit 31:28        reg_contrast_color_prct_lut_199 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_198 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_197 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_196 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_195 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_194 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_193 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_192 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_25 (0x40000+(0x201c<<2))//0x48070
//Bit 31:28        reg_contrast_color_prct_lut_207 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_206 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_205 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_204 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_203 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_202 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_201 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_200 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_26 (0x40000+(0x201d<<2))//0x48074
//Bit 31:28        reg_contrast_color_prct_lut_215 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_214 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_213 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_212 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_211 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_210 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_209 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_208 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_27 (0x40000+(0x201e<<2))//0x48078
//Bit 31:28        reg_contrast_color_prct_lut_223 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_222 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_221 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_220 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_219 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_218 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_217 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_216 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_28 (0x40000+(0x201f<<2))//0x4807c
//Bit 31:28        reg_contrast_color_prct_lut_231 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_230 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_229 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_228 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_227 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_226 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_225 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_224 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_29 (0x40000+(0x2020<<2))//0x48080
//Bit 31:28        reg_contrast_color_prct_lut_239 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_238 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_237 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_236 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_235 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_234 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_233 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_232 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_30 (0x40000+(0x2021<<2))//0x48084
//Bit 31:28        reg_contrast_color_prct_lut_247 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_246 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_245 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_244 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_243 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_242 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_241 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_240 // unsigned ,    RW, default = 0  

#define ENHANCE_COLOR_PRCT_LUT_31 (0x40000+(0x2022<<2))//0x48088
//Bit 31:28        reg_contrast_color_prct_lut_255 // unsigned ,    RW, default = 0  
//Bit 27:24        reg_contrast_color_prct_lut_254 // unsigned ,    RW, default = 0  
//Bit 23:20        reg_contrast_color_prct_lut_253 // unsigned ,    RW, default = 0  
//Bit 19:16        reg_contrast_color_prct_lut_252 // unsigned ,    RW, default = 0  
//Bit 15:12        reg_contrast_color_prct_lut_251 // unsigned ,    RW, default = 0  
//Bit 11: 8        reg_contrast_color_prct_lut_250 // unsigned ,    RW, default = 0  
//Bit  7: 4        reg_contrast_color_prct_lut_249 // unsigned ,    RW, default = 0  
//Bit  3: 0        reg_contrast_color_prct_lut_248 // unsigned ,    RW, default = 0  

#define ENHANCE_DNLP_YGRID_0 (0x40000+(0x2023<<2))//0x4808c
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_1          // unsigned ,    RW, default = 16  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_0          // unsigned ,    RW, default = 8  

#define ENHANCE_DNLP_YGRID_1 (0x40000+(0x2024<<2))//0x48090
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_3          // unsigned ,    RW, default = 32  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_2          // unsigned ,    RW, default = 24  

#define ENHANCE_DNLP_YGRID_2 (0x40000+(0x2025<<2))//0x48094
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_5          // unsigned ,    RW, default = 48  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_4          // unsigned ,    RW, default = 40  

#define ENHANCE_DNLP_YGRID_3 (0x40000+(0x2026<<2))//0x48098
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_7          // unsigned ,    RW, default = 64  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_6          // unsigned ,    RW, default = 56  

#define ENHANCE_DNLP_YGRID_4 (0x40000+(0x2027<<2))//0x4809c
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_9          // unsigned ,    RW, default = 80  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_8          // unsigned ,    RW, default = 72  

#define ENHANCE_DNLP_YGRID_5 (0x40000+(0x2028<<2))//0x480a0
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_11         // unsigned ,    RW, default = 104  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_10         // unsigned ,    RW, default = 92  

#define ENHANCE_DNLP_YGRID_6 (0x40000+(0x2029<<2))//0x480a4
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_13         // unsigned ,    RW, default = 128  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_12         // unsigned ,    RW, default = 116  

#define ENHANCE_DNLP_YGRID_7 (0x40000+(0x202a<<2))//0x480a8
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_15         // unsigned ,    RW, default = 160  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_14         // unsigned ,    RW, default = 144  

#define ENHANCE_DNLP_YGRID_8 (0x40000+(0x202b<<2))//0x480ac
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_17         // unsigned ,    RW, default = 192  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_16         // unsigned ,    RW, default = 176  

#define ENHANCE_DNLP_YGRID_9 (0x40000+(0x202c<<2))//0x480b0
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_19         // unsigned ,    RW, default = 224  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_18         // unsigned ,    RW, default = 208  

#define ENHANCE_DNLP_YGRID_10 (0x40000+(0x202d<<2))//0x480b4
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_21         // unsigned ,    RW, default = 256  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_20         // unsigned ,    RW, default = 240  

#define ENHANCE_DNLP_YGRID_11 (0x40000+(0x202e<<2))//0x480b8
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_23         // unsigned ,    RW, default = 300  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_22         // unsigned ,    RW, default = 276  

#define ENHANCE_DNLP_YGRID_12 (0x40000+(0x202f<<2))//0x480bc
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_25         // unsigned ,    RW, default = 340  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_24         // unsigned ,    RW, default = 320  

#define ENHANCE_DNLP_YGRID_13 (0x40000+(0x2030<<2))//0x480c0
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_27         // unsigned ,    RW, default = 384  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_26         // unsigned ,    RW, default = 364  

#define ENHANCE_DNLP_YGRID_14 (0x40000+(0x2031<<2))//0x480c4
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_29         // unsigned ,    RW, default = 448  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_28         // unsigned ,    RW, default = 416  

#define ENHANCE_DNLP_YGRID_15 (0x40000+(0x2032<<2))//0x480c8
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_31         // unsigned ,    RW, default = 512  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_30         // unsigned ,    RW, default = 480  

#define ENHANCE_DNLP_YGRID_16 (0x40000+(0x2033<<2))//0x480cc
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_33         // unsigned ,    RW, default = 576  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_32         // unsigned ,    RW, default = 544  

#define ENHANCE_DNLP_YGRID_17 (0x40000+(0x2034<<2))//0x480d0
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_35         // unsigned ,    RW, default = 640  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_34         // unsigned ,    RW, default = 608  

#define ENHANCE_DNLP_YGRID_18 (0x40000+(0x2035<<2))//0x480d4
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_37         // unsigned ,    RW, default = 688  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_36         // unsigned ,    RW, default = 664  

#define ENHANCE_DNLP_YGRID_19 (0x40000+(0x2036<<2))//0x480d8
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_39         // unsigned ,    RW, default = 736  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_38         // unsigned ,    RW, default = 712  

#define ENHANCE_DNLP_YGRID_20 (0x40000+(0x2037<<2))//0x480dc
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_41         // unsigned ,    RW, default = 768  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_40         // unsigned ,    RW, default = 752  

#define ENHANCE_DNLP_YGRID_21 (0x40000+(0x2038<<2))//0x480e0
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_43         // unsigned ,    RW, default = 800  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_42         // unsigned ,    RW, default = 784  

#define ENHANCE_DNLP_YGRID_22 (0x40000+(0x2039<<2))//0x480e4
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_45         // unsigned ,    RW, default = 824  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_44         // unsigned ,    RW, default = 812  

#define ENHANCE_DNLP_YGRID_23 (0x40000+(0x203a<<2))//0x480e8
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_47         // unsigned ,    RW, default = 848  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_46         // unsigned ,    RW, default = 840  

#define ENHANCE_DNLP_YGRID_24 (0x40000+(0x203b<<2))//0x480ec
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_49         // unsigned ,    RW, default = 860  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_48         // unsigned ,    RW, default = 856  

#define ENHANCE_DNLP_YGRID_25 (0x40000+(0x203c<<2))//0x480f0
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_51         // unsigned ,    RW, default = 872  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_50         // unsigned ,    RW, default = 864  

#define ENHANCE_DNLP_YGRID_26 (0x40000+(0x203d<<2))//0x480f4
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_53         // unsigned ,    RW, default = 888  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_52         // unsigned ,    RW, default = 880  

#define ENHANCE_DNLP_YGRID_27 (0x40000+(0x203e<<2))//0x480f8
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_55         // unsigned ,    RW, default = 904  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_54         // unsigned ,    RW, default = 896  

#define ENHANCE_DNLP_YGRID_28 (0x40000+(0x203f<<2))//0x480fc
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_57         // unsigned ,    RW, default = 928  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_56         // unsigned ,    RW, default = 912  

#define ENHANCE_DNLP_YGRID_29 (0x40000+(0x2040<<2))//0x48100
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_59         // unsigned ,    RW, default = 960  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_58         // unsigned ,    RW, default = 944  

#define ENHANCE_DNLP_YGRID_30 (0x40000+(0x2041<<2))//0x48104
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_61         // unsigned ,    RW, default = 992  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_60         // unsigned ,    RW, default = 976  

#define ENHANCE_DNLP_YGRID_31 (0x40000+(0x2042<<2))//0x48108
//Bit 31:22        reserved                           
//Bit 21:12        reg_dnlp_ygrid_63         // unsigned ,    RW, default = 1020  
//Bit 11:10        reserved                           
//Bit  9: 0        reg_dnlp_ygrid_62         // unsigned ,    RW, default = 1008  

#define ENHANCE_DNLP_SATPRT_DIV_M (0x40000+(0x2043<<2))//0x4810c
//Bit 31:24        reserved                           
//Bit 23:16        reg_dnlp_satprt_div_m_2   // unsigned ,    RW, default = 128  1/m, normalized to 128 as 1, default=1
//Bit 15: 8        reg_dnlp_satprt_div_m_1   // unsigned ,    RW, default = 128  1/m, normalized to 128 as 1, default=1
//Bit  7: 0        reg_dnlp_satprt_div_m_0   // unsigned ,    RW, default = 128  1/m, normalized to 128 as 1, default=1

#define ENHANCE_DNLP_SATPRT_LMT_RGB (0x40000+(0x2044<<2))//0x48110
//Bit 31:24        reserved                           
//Bit 23:12        reg_dnlp_satprt_lmt_rgb_2 // unsigned ,    RW, default = 1023  limit of the rgb channel, for limited range RGB, set to 960, otherwise set to 1023;
//Bit 11: 0        reg_dnlp_satprt_lmt_rgb_1 // unsigned ,    RW, default = 1023  limit of the rgb channel, for limited range RGB, set to 960, otherwise set to 1023;

#define ENHANCE_DNLP_SATPRT_LMT_RGB_F (0x40000+(0x2045<<2))//0x48114
//Bit 31:12        reserved                           
//Bit 11: 0        reg_dnlp_satprt_lmt_rgb_0 // unsigned ,    RW, default = 1023  limit of the rgb channel, for limited range RGB, set to 960, otherwise set to 1023;

#define ENHANCE_DNLP_SATPRT_MODE (0x40000+(0x2046<<2))//0x48118
//Bit 31: 9        reserved                           
//Bit  8           reg_dnlp_satprt_is_lmt    // unsigned ,    RW, default = 1  
//Bit  7: 6        reserved                           
//Bit  5: 4        reg_dnlp_satprt_csc_mode  // unsigned ,    RW, default = 1  
//Bit  3: 1        reserved                           
//Bit  0           reg_dnlp_satprt_enable    // unsigned ,    RW, default = 1  

#define ENHANCE_DNLP_SATPRT_SAT_RATE_CORE (0x40000+(0x2047<<2))//0x4811c
//Bit 31:16        reserved                           
//Bit 15: 8        reg_dnlp_satprt_sat_rate  // unsigned ,    RW, default = 64  
//Bit  7: 0        reg_dnlp_satprt_sat_core  // unsigned ,    RW, default = 80  

#define ENHANCE_DNLP_COLOR_PRT_GAIN (0x40000+(0x2048<<2))//0x48120
//Bit 31: 8        reserved                           
//Bit  7: 0        reg_dnlp_color_prt_gain   // unsigned ,    RW, default = 64  gain to de-boost of dnlp_dlt base on color region, norm to 64 as 1.0. set to 0 as disable

#define ENHANCE_LC_SATUR_LUT_0 (0x40000+(0x2049<<2))//0x48124
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_1        // unsigned ,    RW, default = 128  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_0        // unsigned ,    RW, default = 64  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_1 (0x40000+(0x204a<<2))//0x48128
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_3        // unsigned ,    RW, default = 256  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_2        // unsigned ,    RW, default = 192  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_2 (0x40000+(0x204b<<2))//0x4812c
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_5        // unsigned ,    RW, default = 384  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_4        // unsigned ,    RW, default = 320  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_3 (0x40000+(0x204c<<2))//0x48130
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_7        // unsigned ,    RW, default = 512  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_6        // unsigned ,    RW, default = 448  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_4 (0x40000+(0x204d<<2))//0x48134
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_9        // unsigned ,    RW, default = 640  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_8        // unsigned ,    RW, default = 576  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_5 (0x40000+(0x204e<<2))//0x48138
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_11       // unsigned ,    RW, default = 768  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_10       // unsigned ,    RW, default = 704  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_6 (0x40000+(0x204f<<2))//0x4813c
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_13       // unsigned ,    RW, default = 896  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_12       // unsigned ,    RW, default = 832  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_7 (0x40000+(0x2050<<2))//0x48140
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_15       // unsigned ,    RW, default = 1024  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_14       // unsigned ,    RW, default = 960  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_8 (0x40000+(0x2051<<2))//0x48144
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_17       // unsigned ,    RW, default = 1152  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_16       // unsigned ,    RW, default = 1088  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_9 (0x40000+(0x2052<<2))//0x48148
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_19       // unsigned ,    RW, default = 1280  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_18       // unsigned ,    RW, default = 1216  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_10 (0x40000+(0x2053<<2))//0x4814c
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_21       // unsigned ,    RW, default = 1408  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_20       // unsigned ,    RW, default = 1344  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_11 (0x40000+(0x2054<<2))//0x48150
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_23       // unsigned ,    RW, default = 1536  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_22       // unsigned ,    RW, default = 1472  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_12 (0x40000+(0x2055<<2))//0x48154
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_25       // unsigned ,    RW, default = 1664  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_24       // unsigned ,    RW, default = 1600  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_13 (0x40000+(0x2056<<2))//0x48158
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_27       // unsigned ,    RW, default = 1792  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_26       // unsigned ,    RW, default = 1728  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_14 (0x40000+(0x2057<<2))//0x4815c
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_29       // unsigned ,    RW, default = 1920  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_28       // unsigned ,    RW, default = 1856  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_15 (0x40000+(0x2058<<2))//0x48160
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_31       // unsigned ,    RW, default = 2048  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_30       // unsigned ,    RW, default = 1984  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_16 (0x40000+(0x2059<<2))//0x48164
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_33       // unsigned ,    RW, default = 2176  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_32       // unsigned ,    RW, default = 2112  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_17 (0x40000+(0x205a<<2))//0x48168
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_35       // unsigned ,    RW, default = 2304  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_34       // unsigned ,    RW, default = 2240  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_18 (0x40000+(0x205b<<2))//0x4816c
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_37       // unsigned ,    RW, default = 2432  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_36       // unsigned ,    RW, default = 2368  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_19 (0x40000+(0x205c<<2))//0x48170
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_39       // unsigned ,    RW, default = 2560  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_38       // unsigned ,    RW, default = 2496  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_20 (0x40000+(0x205d<<2))//0x48174
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_41       // unsigned ,    RW, default = 2688  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_40       // unsigned ,    RW, default = 2624  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_21 (0x40000+(0x205e<<2))//0x48178
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_43       // unsigned ,    RW, default = 2816  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_42       // unsigned ,    RW, default = 2752  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_22 (0x40000+(0x205f<<2))//0x4817c
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_45       // unsigned ,    RW, default = 2944  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_44       // unsigned ,    RW, default = 2880  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_23 (0x40000+(0x2060<<2))//0x48180
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_47       // unsigned ,    RW, default = 3072  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_46       // unsigned ,    RW, default = 3008  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_24 (0x40000+(0x2061<<2))//0x48184
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_49       // unsigned ,    RW, default = 3200  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_48       // unsigned ,    RW, default = 3136  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_25 (0x40000+(0x2062<<2))//0x48188
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_51       // unsigned ,    RW, default = 3328  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_50       // unsigned ,    RW, default = 3264  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_26 (0x40000+(0x2063<<2))//0x4818c
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_53       // unsigned ,    RW, default = 3456  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_52       // unsigned ,    RW, default = 3392  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_27 (0x40000+(0x2064<<2))//0x48190
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_55       // unsigned ,    RW, default = 3584  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_54       // unsigned ,    RW, default = 3520  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_28 (0x40000+(0x2065<<2))//0x48194
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_57       // unsigned ,    RW, default = 3712  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_56       // unsigned ,    RW, default = 3648  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_29 (0x40000+(0x2066<<2))//0x48198
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_59       // unsigned ,    RW, default = 3840  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_58       // unsigned ,    RW, default = 3776  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_30 (0x40000+(0x2067<<2))//0x4819c
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_satur_lut_61       // unsigned ,    RW, default = 3968  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4
//Bit 11: 0        reg_lc_satur_lut_60       // unsigned ,    RW, default = 3904  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_SATUR_LUT_F (0x40000+(0x2068<<2))//0x481a0
//Bit 31:12        reserved                           
//Bit 11: 0        reg_lc_satur_lut_62       // unsigned ,    RW, default = 4032  saturation protection curve, normalized to 4096 as 1, default = y=x^1.4

#define ENHANCE_LC_BLK_HIDX_0 (0x40000+(0x2069<<2))//0x481a4
//Bit 31:30        reserved                           
//Bit 29:16        reg_lc_blk_hidx_1         // unsigned ,    RW, default = 214  block boundary x-index
//Bit 15:14        reserved                           
//Bit 13: 0        reg_lc_blk_hidx_0         // unsigned ,    RW, default = 0  block boundary x-index

#define ENHANCE_LC_BLK_HIDX_1 (0x40000+(0x206a<<2))//0x481a8
//Bit 31:30        reserved                           
//Bit 29:16        reg_lc_blk_hidx_3         // unsigned ,    RW, default = 642  block boundary x-index
//Bit 15:14        reserved                           
//Bit 13: 0        reg_lc_blk_hidx_2         // unsigned ,    RW, default = 428  block boundary x-index

#define ENHANCE_LC_BLK_HIDX_2 (0x40000+(0x206b<<2))//0x481ac
//Bit 31:30        reserved                           
//Bit 29:16        reg_lc_blk_hidx_5         // unsigned ,    RW, default = 1070  block boundary x-index
//Bit 15:14        reserved                           
//Bit 13: 0        reg_lc_blk_hidx_4         // unsigned ,    RW, default = 856  block boundary x-index

#define ENHANCE_LC_BLK_HIDX_3 (0x40000+(0x206c<<2))//0x481b0
//Bit 31:30        reserved                           
//Bit 29:16        reg_lc_blk_hidx_7         // unsigned ,    RW, default = 1498  block boundary x-index
//Bit 15:14        reserved                           
//Bit 13: 0        reg_lc_blk_hidx_6         // unsigned ,    RW, default = 1284  block boundary x-index

#define ENHANCE_LC_BLK_HIDX_4 (0x40000+(0x206d<<2))//0x481b4
//Bit 31:30        reserved                           
//Bit 29:16        reg_lc_blk_hidx_9         // unsigned ,    RW, default = 1926  block boundary x-index
//Bit 15:14        reserved                           
//Bit 13: 0        reg_lc_blk_hidx_8         // unsigned ,    RW, default = 1712  block boundary x-index

#define ENHANCE_LC_BLK_HIDX_5 (0x40000+(0x206e<<2))//0x481b8
//Bit 31:30        reserved                           
//Bit 29:16        reg_lc_blk_hidx_11        // unsigned ,    RW, default = 2354  block boundary x-index
//Bit 15:14        reserved                           
//Bit 13: 0        reg_lc_blk_hidx_10        // unsigned ,    RW, default = 2140  block boundary x-index

#define ENHANCE_LC_BLK_HIDX_F (0x40000+(0x206f<<2))//0x481bc
//Bit 31:14        reserved                           
//Bit 13: 0        reg_lc_blk_hidx_12        // unsigned ,    RW, default = 2560  block boundary x-index

#define ENHANCE_LC_BLK_VIDX_0 (0x40000+(0x2070<<2))//0x481c0
//Bit 31:30        reserved                           
//Bit 29:16        reg_lc_blk_vidx_1         // unsigned ,    RW, default = 180  block boundary y-index
//Bit 15:14        reserved                           
//Bit 13: 0        reg_lc_blk_vidx_0         // unsigned ,    RW, default = 0  block boundary y-index

#define ENHANCE_LC_BLK_VIDX_1 (0x40000+(0x2071<<2))//0x481c4
//Bit 31:30        reserved                           
//Bit 29:16        reg_lc_blk_vidx_3         // unsigned ,    RW, default = 540  block boundary y-index
//Bit 15:14        reserved                           
//Bit 13: 0        reg_lc_blk_vidx_2         // unsigned ,    RW, default = 360  block boundary y-index

#define ENHANCE_LC_BLK_VIDX_2 (0x40000+(0x2072<<2))//0x481c8
//Bit 31:30        reserved                           
//Bit 29:16        reg_lc_blk_vidx_5         // unsigned ,    RW, default = 900  block boundary y-index
//Bit 15:14        reserved                           
//Bit 13: 0        reg_lc_blk_vidx_4         // unsigned ,    RW, default = 720  block boundary y-index

#define ENHANCE_LC_BLK_VIDX_3 (0x40000+(0x2073<<2))//0x481cc
//Bit 31:30        reserved                           
//Bit 29:16        reg_lc_blk_vidx_7         // unsigned ,    RW, default = 1260  block boundary y-index
//Bit 15:14        reserved                           
//Bit 13: 0        reg_lc_blk_vidx_6         // unsigned ,    RW, default = 1080  block boundary y-index

#define ENHANCE_LC_BLK_VIDX_F (0x40000+(0x2074<<2))//0x481d0
//Bit 31:14        reserved                           
//Bit 13: 0        reg_lc_blk_vidx_8         // unsigned ,    RW, default = 1440  block boundary y-index

#define ENHANCE_LC_BLK_HV_NUM (0x40000+(0x2075<<2))//0x481d4
//Bit 31:16        reserved                           
//Bit 15: 8        reg_lc_blk_hnum           // unsigned ,    RW, default = 12  lc processing region number of V, maximum to (STA_LEN_V-1)   (0~16)
//Bit  7: 0        reg_lc_blk_vnum           // unsigned ,    RW, default = 8  lc processing region number of H, maximum to (STA_LEN_H-1)   (0~24)

#define ENHANCE_LC_YUV2RGB_OFSET (0x40000+(0x2076<<2))//0x481d8
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_yuv2rgb_ofset_1    // unsigned ,    RW, default = 512  yuv2rgb pre-offset to yuv, 1<<(10 - 1)
//Bit 11: 0        reg_lc_yuv2rgb_ofset_0    // unsigned ,    RW, default = 0  yuv2rgb pre-offset to yuv, 1<<(10 - 4)

#define ENHANCE_LC_YUV2RGB_CLIP (0x40000+(0x2077<<2))//0x481dc
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_yuv2rgb_clip_1     // unsigned ,    RW, default = 1023  yuv2rgb converted YUV clipping range, (1<<(10 )) -1
//Bit 11: 0        reg_lc_yuv2rgb_clip_0     // unsigned ,    RW, default = 0  yuv2rgb converted YUV clipping range

#define ENHANCE_LC_RGB2YUV_OFSET (0x40000+(0x2078<<2))//0x481e0
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_rgb2yuv_ofset_1    // unsigned ,    RW, default = 512  yuv2rgb pre-offset to yuv, 1<<(10 -1)
//Bit 11: 0        reg_lc_rgb2yuv_ofset_0    // unsigned ,    RW, default = 0  yuv2rgb pre-offset to yuv, 1<<(10 - 4)

#define ENHANCE_LC_RGB2YUV_CLIP (0x40000+(0x2079<<2))//0x481e4
//Bit 31:24        reserved                           
//Bit 23:12        reg_lc_rgb2yuv_clip_1     // unsigned ,    RW, default = 1023  rgb2yuv converted YUV clipping range, 1, (1<<(10 )) -1
//Bit 11: 0        reg_lc_rgb2yuv_clip_0     // unsigned ,    RW, default = 0  rgb2yuv converted YUV clipping range

#define ENHANCE_YUV_RGB_RS (0x40000+(0x207a<<2))//0x481e8
//Bit 31: 6        reserved                           
//Bit  5: 4        reg_lc_yuv2rgb_rs         // unsigned ,    RW, default = 0  matrix normalization right shift extra bits, norm= (1<<(8+rs))
//Bit  3: 2        reserved                           
//Bit  1: 0        reg_lc_rgb2yuv_rs         // unsigned ,    RW, default = 0  matrix normalization right shift extra bits, norm= (1<<(10+rs))

#define ENHANCE_YUV2RGB_MAT_0 (0x40000+(0x207b<<2))//0x481ec
//Bit 31:29        reserved                           
//Bit 28:16        reg_lc_yuv2rgb_mat3x3_1   // signed ,    RW, default = 0  yuv2rgb 3x3 matrix
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lc_yuv2rgb_mat3x3_0   // signed ,    RW, default = 1024  yuv2rgb 3x3 matrix

#define ENHANCE_YUV2RGB_MAT_1 (0x40000+(0x207c<<2))//0x481f0
//Bit 31:29        reserved                           
//Bit 28:16        reg_lc_yuv2rgb_mat3x3_3   // signed ,    RW, default = 1024  yuv2rgb 3x3 matrix
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lc_yuv2rgb_mat3x3_2   // signed ,    RW, default = 1577  yuv2rgb 3x3 matrix

#define ENHANCE_YUV2RGB_MAT_2 (0x40000+(0x207d<<2))//0x481f4
//Bit 31:29        reserved                           
//Bit 28:16        reg_lc_yuv2rgb_mat3x3_5   // signed ,    RW, default = -470  yuv2rgb 3x3 matrix
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lc_yuv2rgb_mat3x3_4   // signed ,    RW, default = -187  yuv2rgb 3x3 matrix

#define ENHANCE_YUV2RGB_MAT_3 (0x40000+(0x207e<<2))//0x481f8
//Bit 31:29        reserved                           
//Bit 28:16        reg_lc_yuv2rgb_mat3x3_7   // signed ,    RW, default = 1860  yuv2rgb 3x3 matrix
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lc_yuv2rgb_mat3x3_6   // signed ,    RW, default = 1024  yuv2rgb 3x3 matrix

#define ENHANCE_YUV2RGB_MAT_F (0x40000+(0x207f<<2))//0x481fc
//Bit 31:13        reserved                           
//Bit 12: 0        reg_lc_yuv2rgb_mat3x3_8   // signed ,    RW, default = 0  yuv2rgb 3x3 matrix

#define ENHANCE_RGB2YUV_MAT_0 (0x40000+(0x2080<<2))//0x48200
//Bit 31:29        reserved                           
//Bit 28:16        reg_lc_rgb2yuv_mat3x3_1   // signed ,    RW, default = 732  rgb2yuv 3x3 matrix
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lc_rgb2yuv_mat3x3_0   // signed ,    RW, default = 218  rgb2yuv 3x3 matrix

#define ENHANCE_RGB2YUV_MAT_1 (0x40000+(0x2081<<2))//0x48204
//Bit 31:29        reserved                           
//Bit 28:16        reg_lc_rgb2yuv_mat3x3_3   // signed ,    RW, default = -117  rgb2yuv 3x3 matrix
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lc_rgb2yuv_mat3x3_2   // signed ,    RW, default = 74  rgb2yuv 3x3 matrix

#define ENHANCE_RGB2YUV_MAT_2 (0x40000+(0x2082<<2))//0x48208
//Bit 31:29        reserved                           
//Bit 28:16        reg_lc_rgb2yuv_mat3x3_5   // signed ,    RW, default = 512  rgb2yuv 3x3 matrix
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lc_rgb2yuv_mat3x3_4   // signed ,    RW, default = -395  rgb2yuv 3x3 matrix

#define ENHANCE_RGB2YUV_MAT_3 (0x40000+(0x2083<<2))//0x4820c
//Bit 31:29        reserved                           
//Bit 28:16        reg_lc_rgb2yuv_mat3x3_7   // signed ,    RW, default = -465  rgb2yuv 3x3 matrix
//Bit 15:13        reserved                           
//Bit 12: 0        reg_lc_rgb2yuv_mat3x3_6   // signed ,    RW, default = 512  rgb2yuv 3x3 matrix

#define ENHANCE_RGB2YUV_MAT_F (0x40000+(0x2084<<2))//0x48210
//Bit 31:13        reserved                           
//Bit 12: 0        reg_lc_rgb2yuv_mat3x3_8   // signed ,    RW, default = -48  rgb2yuv 3x3 matrix

#define ENHANCE_LC_COLOR_PRCT_GAIN (0x40000+(0x2085<<2))//0x48214
//Bit 31:24        reserved                           
//Bit 23:16        reg_lc_colr_prct_gain_2   // unsigned ,    RW, default = 64  gain to de-boost of lc_dlt (y/u/v) base on color region, norm to 64 as 1.0, set to 0 as disable
//Bit 15: 8        reg_lc_colr_prct_gain_1   // unsigned ,    RW, default = 64  gain to de-boost of lc_dlt (y/u/v) base on color region, norm to 64 as 1.0, set to 0 as disable
//Bit  7: 0        reg_lc_colr_prct_gain_0   // unsigned ,    RW, default = 64  gain to de-boost of lc_dlt (y/u/v) base on color region, norm to 64 as 1.0, set to 0 as disable

#define ENHANCE_CC_EN (0x40000+(0x2086<<2))//0x48218
//Bit 31: 1        reserved                           
//Bit  0           reg_cc_en                 // unsigned ,    RW, default = 1  sr color compensation enable

#define ENHANCE_CC_YUV2RGB_OFSET (0x40000+(0x2087<<2))//0x4821c
//Bit 31:24        reserved                           
//Bit 23:12        reg_cc_yuv2rgb_ofset_0    // unsigned ,    RW, default = 0  yuv2rgb pre-offset to yuv,
//Bit 11: 0        reg_cc_yuv2rgb_ofset_1    // unsigned ,    RW, default = 512  yuv2rgb pre-offset to yuv,

#define ENHANCE_CC_YUV2RGB_MAT_0 (0x40000+(0x2088<<2))//0x48220
//Bit 31:29        reserved                           
//Bit 28:16        reg_cc_yuv2rgb_mat3x3_1   // signed ,    RW, default = 0  yuv2rgb 3x3 matrix
//Bit 15:13        reserved                           
//Bit 12: 0        reg_cc_yuv2rgb_mat3x3_0   // signed ,    RW, default = 1192  yuv2rgb 3x3 matrix

#define ENHANCE_CC_YUV2RGB_MAT_1 (0x40000+(0x2089<<2))//0x48224
//Bit 31:29        reserved                           
//Bit 28:16        reg_cc_yuv2rgb_mat3x3_3   // signed ,    RW, default = 1192  yuv2rgb 3x3 matrix
//Bit 15:13        reserved                           
//Bit 12: 0        reg_cc_yuv2rgb_mat3x3_2   // signed ,    RW, default = 1836  yuv2rgb 3x3 matrix

#define ENHANCE_CC_YUV2RGB_MAT_2 (0x40000+(0x208a<<2))//0x48228
//Bit 31:29        reserved                           
//Bit 28:16        reg_cc_yuv2rgb_mat3x3_5   // signed ,    RW, default = -547  yuv2rgb 3x3 matrix
//Bit 15:13        reserved                           
//Bit 12: 0        reg_cc_yuv2rgb_mat3x3_4   // signed ,    RW, default = -218  yuv2rgb 3x3 matrix

#define ENHANCE_CC_YUV2RGB_MAT_3 (0x40000+(0x208b<<2))//0x4822c
//Bit 31:29        reserved                           
//Bit 28:16        reg_cc_yuv2rgb_mat3x3_7   // signed ,    RW, default = 2166  yuv2rgb 3x3 matrix
//Bit 15:13        reserved                           
//Bit 12: 0        reg_cc_yuv2rgb_mat3x3_6   // signed ,    RW, default = 1192  yuv2rgb 3x3 matrix

#define ENHANCE_CC_YUV2RGB_MAT_F (0x40000+(0x208c<<2))//0x48230
//Bit 31:13        reserved                           
//Bit 12: 0        reg_cc_yuv2rgb_mat3x3_8   // signed ,    RW, default = 0  yuv2rgb 3x3 matrix

#define ENHANCE_CC_YUV2RGB_CLIP_RS (0x40000+(0x208d<<2))//0x48234
//Bit 31:28        reserved                           
//Bit 27:16        reg_cc_yuv2rgb_clip_0     // unsigned ,    RW, default = 0  yuv2rgb converted RGB clipping range
//Bit 15: 4        reg_cc_yuv2rgb_clip_1     // unsigned ,    RW, default = 1023  yuv2rgb converted RGB clipping range
//Bit  3: 2        reserved                           
//Bit  1: 0        reg_cc_yuv2rgb_rs         // unsigned ,    RW, default = 0  matrix normalization right shift extra bits, norm= (1<<(10+rs))

#define ENHANCE_CC_C_GAIN (0x40000+(0x208e<<2))//0x48238
//Bit 31:16        reserved                           
//Bit 15: 8        reg_cc_low_c_gain         // unsigned ,    RW, default = 0  64 normalize as "1", dft=0
//Bit  7: 0        reg_cc_high_c_gain        // unsigned ,    RW, default = 32  64 normalize as "1", dft=32

#define ENHANCE_CC_ADP_YWT_THD_GAIN (0x40000+(0x208f<<2))//0x4823c
//Bit 31:24        reg_cc_adp_ywt_th1        // unsigned ,    RW, default = 220  th1 of adaptive luma weighting, dft = 220
//Bit 23:16        reg_cc_adp_ywt_th0        // unsigned ,    RW, default = 32  th0 of adaptive luma weighting, dft = 32
//Bit 15: 8        reg_cc_adp_ywt_gain1      // unsigned ,    RW, default = 64  gain1 of adaptive luma weighting, 16 normalize as "1", dft = 64
//Bit  7: 0        reg_cc_adp_ywt_gain0      // unsigned ,    RW, default = 32  gain0 of adaptive luma weighting, 16 normalize as "1", dft = 32

#define ENHANCE_CC_ADP_CWT_THD_GAIN (0x40000+(0x2090<<2))//0x48240
//Bit 31:24        reg_cc_adp_cwt_th1        // unsigned ,    RW, default = 220  th1 of adaptive chroma weighting, dft = 220
//Bit 23:16        reg_cc_adp_cwt_th0        // unsigned ,    RW, default = 32  th0 of adaptive chroma weighting, dft = 32
//Bit 15: 8        reg_cc_adp_cwt_gain1      // unsigned ,    RW, default = 64  gain1 of adaptive chroma weighting, 16 normalize as "1", dft = 64
//Bit  7: 0        reg_cc_adp_cwt_gain0      // unsigned ,    RW, default = 32  gain0 of adaptive chroma weighting, 16 normalize as "1", dft = 32

#define ENHANCE_LC_MAP_RAM_CTRL (0x40000+(0x2091<<2))//0x48244
//Bit 31:2         reserved
//Bit 1            reg_lc_hw_debug_mode      //unsigned , RW, default = 0,  1 to allow cbus read iir lut for debug, cur frame data process down
//Bit 0            reg_lc_cbus2ram_en        //unsigned , RW, default = 0,  1 to allow Cbus to access the LC RAMs       

#define ENHANCE_LC_MAP_RAM_ADDR (0x40000+(0x2092<<2))//0x48248
//Bit 31           reg_lc_ram_pre_read       //unsigned , WO, default = 0,  for RAM read, write 1 for pre-read from RAM with address: {reg_lc_ram_vidx,reg_lc_ram_hidx}
//Bit 30:7         reserved
//Bit 6:4          reg_lc_ram_vidx           //unsigned , RW, default = 0,  RAM index for vert
//Bit 3:0          reg_lc_ram_hidx           //unsigned , RW, default = 0,  RAM index for horz

#define ENHANCE_LC_MAP_RAM_DATA (0x40000+(0x2093<<2))//0x4824c
//Bit 31:30        reserved
//Bit 29:0         reg_lc_ram_data           //unsigned , RW, default = 0,  RAM data for write and read


#define GAMUT_MAP0_RP_CTRL (0x40000+(0x2200<<2))//0x48800
//Bit 31: 4        reserved                           
//Bit  3           reg_matrix_en             // unsigned ,    RW, default = 0  gamut matrix0 enable
//Bit  2           reg_eotf_en               // unsigned ,    RW, default = 0  eotf1 enable
//Bit  1           reg_oetf_en               // unsigned ,    RW, default = 0  oetf1 enable
//Bit  0           reg_ootf_gain_en          // unsigned ,    RW, default = 0  ootf enable

#define GAMUT_MAP0_MATRIX_COEF0 (0x40000+(0x2201<<2))//0x48804
//Bit 31:29        reserved                           
//Bit 28:16        reg_matrix_coef_0_0       // signed ,    RW, default = 425  of the csc matrix,
//Bit 15:13        reserved                           
//Bit 12: 0        reg_matrix_coef_0_1       // signed ,    RW, default = -150  of the csc matrix,

#define GAMUT_MAP0_MATRIX_COEF1 (0x40000+(0x2202<<2))//0x48808
//Bit 31:29        reserved                           
//Bit 28:16        reg_matrix_coef_0_2       // signed ,    RW, default = -18  of the csc matrix,
//Bit 15:13        reserved                           
//Bit 12: 0        reg_matrix_coef_1_0       // signed ,    RW, default = -31  of the csc matrix,

#define GAMUT_MAP0_MATRIX_COEF2 (0x40000+(0x2203<<2))//0x4880c
//Bit 31:29        reserved                           
//Bit 28:16        reg_matrix_coef_1_1       // signed ,    RW, default = 290  of the csc matrix,
//Bit 15:13        reserved                           
//Bit 12: 0        reg_matrix_coef_1_2       // signed ,    RW, default = -2  of the csc matrix,

#define GAMUT_MAP0_MATRIX_COEF3 (0x40000+(0x2204<<2))//0x48810
//Bit 31:29        reserved                           
//Bit 28:16        reg_matrix_coef_2_0       // signed ,    RW, default = -5  of the csc matrix,
//Bit 15:13        reserved                           
//Bit 12: 0        reg_matrix_coef_2_1       // signed ,    RW, default = -25  of the csc matrix,

#define GAMUT_MAP0_MATRIX_COEF4 (0x40000+(0x2205<<2))//0x48814
//Bit 31:19        reserved                           
//Bit 18:16        reg_matrix_3x3mtrx_rs     // unsigned ,    RW, default = 0  of the coef: 0: 4.8, 1: 3.9, 2: 2.10, 3. 1.11
//Bit 15:13        reserved                           
//Bit 12: 0        reg_matrix_coef_2_2       // signed ,    RW, default = 286  of the csc matrix,

#define GAMUT_MAP0_MATRIX_OFST_0 (0x40000+(0x2206<<2))//0x48818
//Bit 31:16        reg_matrix_ofst_oup_0     // signed ,    RW, default = 0  after the conversion
//Bit 15: 0        reg_matrix_ofst_inp_0     // signed ,    RW, default = 0  before input to the matrix

#define GAMUT_MAP0_MATRIX_OFST_1 (0x40000+(0x2207<<2))//0x4881c
//Bit 31:16        reg_matrix_ofst_oup_1     // signed ,    RW, default = 0  after the conversion
//Bit 15: 0        reg_matrix_ofst_inp_1     // signed ,    RW, default = 0  before input to the matrix

#define GAMUT_MAP0_MATRIX_OFST_2 (0x40000+(0x2208<<2))//0x48820
//Bit 31:16        reg_matrix_ofst_oup_2     // signed ,    RW, default = 0  after the conversion
//Bit 15: 0        reg_matrix_ofst_inp_2     // signed ,    RW, default = 0  before input to the matrix

#define GAMUT_MAP0_EOTF_OFST (0x40000+(0x2209<<2))//0x48824
//Bit 31:29        reserved                           
//Bit 28:16        reg_eotf_pre_ofst         // signed ,    RW, default = 0  offset before eotf
//Bit 15: 0        reg_eotf_pst_ofst         // signed ,    RW, default = 0  offset aftere eotf

#define GAMUT_MAP0_OETF_OFST (0x40000+(0x220a<<2))//0x48828
//Bit 31:16        reg_oetf_pre_ofst         // signed ,    RW, default = 0  offset before oetf
//Bit 15:13        reserved                           
//Bit 12: 0        reg_oetf_pst_ofst         // signed ,    RW, default = 0  offset after oeft

#define GAMUT_OOTF_COEF_0 (0x40000+(0x220b<<2))//0x4882c
//Bit 31:28        reserved                           
//Bit 27:16        reg_ootf_ys_coef_1        // unsigned ,    RW, default = 2777  coef to calculate the Ys, normalized to 2048 as "1", leave one bit margin;
//Bit 15:12        reserved                           
//Bit 11: 0        reg_ootf_ys_coef_0        // unsigned ,    RW, default = 1076  coef to calculate the Ys, normalized to 2048 as "1", leave one bit margin;

#define GAMUT_OOTF_COEF_1 (0x40000+(0x220c<<2))//0x48830
//Bit 31:12        reserved                           
//Bit 11: 0        reg_ootf_ys_coef_2        // unsigned ,    RW, default = 243  coef to calculate the Ys, normalized to 2048 as "1", leave one bit margin;

#define GAMUT_OOTF_CTRL (0x40000+(0x220d<<2))//0x48834
//Bit 31:16        reserved                           
//Bit 15:12        reg_ootf_ys_shift         // unsigned ,    RW, default = 12  ys mult shift
//Bit 11:10        reserved                           
//Bit  9: 8        reg_ootf_data_mode        // unsigned ,    RW, default = 0  0, nolinear input, 1, max linear, 2, r
//Bit  7: 5        reserved                           
//Bit  4: 0        reg_ootf_gain_shift       // unsigned ,    RW, default = 8  ys mult shift

#define GAMUT_OOTF_FINAL_ALPHA_0 (0x40000+(0x220e<<2))//0x48838
//Bit 31:30        reserved                           
//Bit 29:16        reg_ootf_final_alpha_1    // unsigned ,    RW, default = 1024  gain(contrast) to linear RGB channel with 1/12 factor, normalized to 1024 as "1" , [0] for R, [1] for G and [2] for B;
//Bit 15:14        reserved                           
//Bit 13: 0        reg_ootf_final_alpha_0    // unsigned ,    RW, default = 1024  gain(contrast) to linear RGB channel with 1/12 factor, normalized to 1024 as "1" , [0] for R, [1] for G and [2] for B;

#define GAMUT_OOTF_FINAL_ALPHA_1 (0x40000+(0x220f<<2))//0x4883c
//Bit 31:16        reserved                           
//Bit 15:14        reserved                           
//Bit 13: 0        reg_ootf_final_alpha_2    // unsigned ,    RW, default = 1024  gain(contrast) to linear RGB channel with 1/12 factor, normalized to 1024 as "1" , [0] for R, [1] for G and [2] for B;

#define GAMUT_OOTF_BETA_OFST_0 (0x40000+(0x2210<<2))//0x48840
//Bit 31:25        reserved                           
//Bit 24: 0        reg_adpscl_beta_ofst_0    // signed ,    RW, default = 0  coef to calculate the Ys, normalized to 2048 as "1", leave one bit margin;

#define GAMUT_OOTF_BETA_OFST_1 (0x40000+(0x2211<<2))//0x48844
//Bit 31:25        reserved                           
//Bit 24: 0        reg_adpscl_beta_ofst_1    // signed ,    RW, default = 1  coef to calculate the Ys, normalized to 2048 as "1", leave one bit margin;

#define GAMUT_OOTF_BETA_OFST_2 (0x40000+(0x2212<<2))//0x48848
//Bit 31:25        reserved                           
//Bit 24: 0        reg_adpscl_beta_ofst_2    // signed ,    RW, default = 2  coef to calculate the Ys, normalized to 2048 as "1", leave one bit margin;


#define GAMUT_MAP1_RP_CTRL (0x40000+(0x2220<<2))//0x48880
//Bit 31: 4        reserved                           
//Bit  3           reg_matrix_en             // unsigned ,    RW, default = 0  gamut matrix1 enable
//Bit  2           reg_eotf_en               // unsigned ,    RW, default = 0  eotf1 enable
//Bit  1           reg_oetf_en               // unsigned ,    RW, default = 0  oetf1 enable
//Bit  0           reserved

#define GAMUT_MAP1_MATRIX_COEF0 (0x40000+(0x2221<<2))//0x48884
//Bit 31:29        reserved                           
//Bit 28:16        reg_matrix_coef_0_0       // signed ,    RW, default = 425  of the csc matrix,
//Bit 15:13        reserved                           
//Bit 12: 0        reg_matrix_coef_0_1       // signed ,    RW, default = -150  of the csc matrix,

#define GAMUT_MAP1_MATRIX_COEF1 (0x40000+(0x2222<<2))//0x48888
//Bit 31:29        reserved                           
//Bit 28:16        reg_matrix_coef_0_2       // signed ,    RW, default = -18  of the csc matrix,
//Bit 15:13        reserved                           
//Bit 12: 0        reg_matrix_coef_1_0       // signed ,    RW, default = -31  of the csc matrix,

#define GAMUT_MAP1_MATRIX_COEF2 (0x40000+(0x2223<<2))//0x4888c
//Bit 31:29        reserved                           
//Bit 28:16        reg_matrix_coef_1_1       // signed ,    RW, default = 290  of the csc matrix,
//Bit 15:13        reserved                           
//Bit 12: 0        reg_matrix_coef_1_2       // signed ,    RW, default = -2  of the csc matrix,

#define GAMUT_MAP1_MATRIX_COEF3 (0x40000+(0x2224<<2))//0x48890
//Bit 31:29        reserved                           
//Bit 28:16        reg_matrix_coef_2_0       // signed ,    RW, default = -5  of the csc matrix,
//Bit 15:13        reserved                           
//Bit 12: 0        reg_matrix_coef_2_1       // signed ,    RW, default = -25  of the csc matrix,

#define GAMUT_MAP1_MATRIX_COEF4 (0x40000+(0x2225<<2))//0x48894
//Bit 31:19        reserved                           
//Bit 18:16        reg_matrix_3x3mtrx_rs     // unsigned ,    RW, default = 0  of the coef: 0: 4.8, 1: 3.9, 2: 2.10, 3. 1.11
//Bit 15:13        reserved                           
//Bit 12: 0        reg_matrix_coef_2_2       // signed ,    RW, default = 286  of the csc matrix,

#define GAMUT_MAP1_MATRIX_OFST_0 (0x40000+(0x2226<<2))//0x48898
//Bit 31:16        reg_matrix_ofst_oup_0     // signed ,    RW, default = 0  after the conversion
//Bit 15: 0        reg_matrix_ofst_inp_0     // signed ,    RW, default = 0  before input to the matrix

#define GAMUT_MAP1_MATRIX_OFST_1 (0x40000+(0x2227<<2))//0x4889c
//Bit 31:16        reg_matrix_ofst_oup_1     // signed ,    RW, default = 0  after the conversion
//Bit 15: 0        reg_matrix_ofst_inp_1     // signed ,    RW, default = 0  before input to the matrix

#define GAMUT_MAP1_MATRIX_OFST_2 (0x40000+(0x2228<<2))//0x488a0
//Bit 31:16        reg_matrix_ofst_oup_2     // signed ,    RW, default = 0  after the conversion
//Bit 15: 0        reg_matrix_ofst_inp_2     // signed ,    RW, default = 0  before input to the matrix

#define GAMUT_MAP1_EOTF_OFST (0x40000+(0x2229<<2))//0x488a4
//Bit 31:29        reserved                           
//Bit 28:16        reg_eotf_pre_ofst         // signed ,    RW, default = 0  offset before eotf
//Bit 15: 0        reg_eotf_pst_ofst         // signed ,    RW, default = 0  offset aftere eotf

#define GAMUT_MAP1_OETF_OFST (0x40000+(0x222a<<2))//0x488a8
//Bit 31:16        reg_oetf_pre_ofst         // signed ,    RW, default = 0  offset before oetf
//Bit 15:13        reserved                           
//Bit 12: 0        reg_oetf_pst_ofst         // signed ,    RW, default = 0  offset after oeft

#define GAMUT_MAP1_3DLUT_EN (0x40000+(0x222b<<2))//0x488ac
//Bit 31: 1        reserved                           
//Bit  0           reg_3dlut_en               // unsigned ,    RW, default = 0 

// Bit 23:16    Y
// Bit 15:8     CB
// Bit 7:0      CR 

#define MAP0_GAMUT_EOTF_LUT_ADDR_PORT (0x40000+(0x2240<<2))//0x48900
//Bit 31:0  reg_eotf_lut_addr          //unsigned ,RW, default = 0 

#define MAP0_GAMUT_EOTF_LUT_DATA_PORT (0x40000+(0x2241<<2))//0x48904
//Bit 31:0  reg_eotf_lut_data          //unsigned ,RW, default = 0

#define MAP0_GAMUT_OETF_LUT_ADDR_PORT (0x40000+(0x2242<<2))//0x48908
//Bit 31:0  reg_oetf_lut_addr          //unsigned ,RW, default = 0

#define MAP0_GAMUT_OETF_LUT_DATA_PORT (0x40000+(0x2243<<2))//0x4890c
//Bit 31:0  reg_oetf_lut_data          //unsigned ,RW, default = 0

#define GAMUT_OOTF_LUT_ADDR_PORT (0x40000+(0x2244<<2))//0x48910
//Bit 31:0  reg_ootf_lut_addr          //unsigned ,RW, default = 0

#define GAMUT_OOTF_LUT_DATA_PORT (0x40000+(0x2245<<2))//0x48914
//Bit 31:0  reg_ootf_lut_data          //unsigned ,RW, default = 0

#define MAP1_GAMUT_EOTF_LUT_ADDR_PORT (0x40000+(0x2246<<2))//0x48918
//Bit 31:0  reg_eotf_lut_addr          //unsigned ,RW, default = 0 

#define MAP1_GAMUT_EOTF_LUT_DATA_PORT (0x40000+(0x2247<<2))//0x4891c
//Bit 31:0  reg_eotf_lut_data          //unsigned ,RW, default = 0

#define MAP1_GAMUT_OETF_LUT_ADDR_PORT (0x40000+(0x2248<<2))//0x48920
//Bit 31:0  reg_oetf_lut_addr          //unsigned ,RW, default = 0

#define MAP1_GAMUT_OETF_LUT_DATA_PORT (0x40000+(0x2249<<2))//0x48924
//Bit 31:0  reg_oetf_lut_data          //unsigned ,RW, default = 0

#define VPP_LUT3D_CTRL (0x40000+(0x224a<<2))//0x48928
//Bit 31:22 reserved  
//Bit 21:16 reg_lut3d_mode             //unsigned ,RW, default = 0 
//Bit 15:10 reserved 
//Bit  9:8  reg_lut3d_gclk_ctrl        //unsigned ,RW, default = 0 
//Bit    7  reserved 
//Bit  6:4  reg_lut3d_extnd_en         //unsigned ,RW, default = 0 
//Bit    3  reserved 
//Bit    2  reg_lut3d_en_sync_dis      //unsigned ,RW, default = 0 
//Bit    1  reserved 
//Bit    0  reg_lut3d_en               //unsigned ,RW, default = 0 

#define VPP_LUT3D_CBUS2RAM_CTRL (0x40000+(0x224b<<2))//0x4892c
//Bit 31:1  reserved 
//Bit    0  reg_lut3d_ram_cbus_en      //unsigned ,RW, default = 0 

#define VPP_LUT3D_RAM_ADDR (0x40000+(0x224c<<2))//0x48930

#define VPP_LUT3D_RAM_DATA (0x40000+(0x224d<<2))//0x48934


#define LCD_GAMMA_PROBE_CTRL (0x40000+(0x2401<<2))//0x49004
//Bit 31:2      reserved
//Bit 1:0       reg_gamma_probe_ctrl        //unsigned, RW, default = 0

#define LCD_GAMMA_PROBE_HL_COLOR (0x40000+(0x2402<<2))//0x49008
//Bit 31:16     reserved
//Bit 15:0      reg_gamma_probe_hl_color    //unsigned, RW, default = 0

#define LCD_GAMMA_PROBE_POS_X (0x40000+(0x2403<<2))//0x4900c
//Bit 31:13     reserved
//Bit 12:0      reg_gamma_probe_pos_x       //unsigned, RW, default = 0

#define LCD_GAMMA_PROBE_POS_Y (0x40000+(0x2404<<2))//0x49010
//Bit 31:13     reserved
//Bit 12:0      reg_gamma_probe_pos_y       //unsigned, RW, default = 0

#define LCD_LDC_AXI_UGT (0x40000+(0x2405<<2))//0x49014
//Bit 31:9      reserved
//Bit 8         reg_gamma_vcom              //unsigned, RW, default = 0
//Bit 7:1       reserved
//Bit 0         reg_gamma_disable_10b       //unsigned, RW, default = 0

#define LCD_GAMMA_PROBE_COLOR_L (0x40000+(0x2406<<2))//0x49018
//Bit 31:16     reserved
//Bit 15:0      ro_gamma_probe_color_l      //unsigned, RO, default = 0

#define LCD_GAMMA_PROBE_COLOR_H (0x40000+(0x2407<<2))//0x4901c
//Bit 31:16     reserved
//Bit 15:0      ro_gamma_probe_color_h      //unsigned, RO, default = 0

#define LDC_GAMMA_POS (0x40000+(0x2408<<2))//0x49020
//Bit 31:1      reserved
//Bit 0         reg_gamma_switch            //unsigned, RW, default = 0


#define LCD_GAMMA_CNTL_PORT0 (0x40000+(0x2480<<2))//0x49200
//Bit 31:0      reg_gamma_cntl              //unsigned, RW, default = 32'hf0000000;

#define LCD_GAMMA_ADDR_PORT0 (0x40000+(0x2481<<2))//0x49204
//Bit 31:10     reserved                    
//Bit 9 :0      reg_haddr                   //unsigned, RW, default = 10'h200;

#define LCD_GAMMA_DATA_PORT0 (0x40000+(0x2482<<2))//0x49208
//Bit 31:30     reserved
//Bit 29:0      reg_bin_data                //unsigned, RW, default = 0;


#define LDC_REG_BLOCK_NUM (0x40000+(0x3000<<2))//0x4c000
//Bit 31:25        reserved                           
//Bit 24           reg_ldc_en                // unsigned ,    RW, default = 0  enable ldc
//Bit 23:16        reg_ldc_seg_col           // unsigned ,    RW, default = 5  the number of col segments for calculate backlight
//Bit 15: 8        reg_ldc_seg_row           // unsigned ,    RW, default = 9  the number of row segments for calculate backlight
//Bit  7: 4        reg_ldc_blk_xnum          // unsigned ,    RW, default = 7  the number of col blocks for calculate intensity, 1<<blk_xnum
//Bit  3: 0        reg_ldc_blk_ynum          // unsigned ,    RW, default = 6  the number of row blocks for calculate intensity, 1<<blk_ynum

#define LDC_REG_SEG_Y_BOUNDARY_0 (0x40000+(0x3001<<2))//0x4c004
//Bit 31:26        reserved                           
//Bit 25:13        reg_ldc_seg_y_boundary_0  // unsigned ,    RW, default = 120  the segment row boundary
//Bit 12: 0        reg_ldc_seg_y_boundary_1  // unsigned ,    RW, default = 240  the segment row boundary

#define LDC_REG_SEG_Y_BOUNDARY_1 (0x40000+(0x3002<<2))//0x4c008
//Bit 31:26        reserved                           
//Bit 25:13        reg_ldc_seg_y_boundary_2  // unsigned ,    RW, default = 360  the segment row boundary
//Bit 12: 0        reg_ldc_seg_y_boundary_3  // unsigned ,    RW, default = 480  the segment row boundary

#define LDC_REG_SEG_Y_BOUNDARY_2 (0x40000+(0x3003<<2))//0x4c00c
//Bit 31:26        reserved                           
//Bit 25:13        reg_ldc_seg_y_boundary_4  // unsigned ,    RW, default = 600  the segment row boundary
//Bit 12: 0        reg_ldc_seg_y_boundary_5  // unsigned ,    RW, default = 720  the segment row boundary

#define LDC_REG_SEG_Y_BOUNDARY_3 (0x40000+(0x3004<<2))//0x4c010
//Bit 31:26        reserved                           
//Bit 25:13        reg_ldc_seg_y_boundary_6  // unsigned ,    RW, default = 840  the segment row boundary
//Bit 12: 0        reg_ldc_seg_y_boundary_7  // unsigned ,    RW, default = 960  the segment row boundary

#define LDC_REG_SEG_Y_BOUNDARY_4 (0x40000+(0x3005<<2))//0x4c014
//Bit 31:26        reserved                           
//Bit 25:13        reg_ldc_seg_y_boundary_8  // unsigned ,    RW, default = 1080  the segment row boundary
//Bit 12: 0        reg_ldc_seg_y_boundary_9  // unsigned ,    RW, default = 0  the segment row boundary

#define LDC_REG_SEG_Y_BOUNDARY_5 (0x40000+(0x3006<<2))//0x4c018
//Bit 31:26        reserved                           
//Bit 25:13        reg_ldc_seg_y_boundary_10 // unsigned ,    RW, default = 0  the segment row boundary
//Bit 12: 0        reg_ldc_seg_y_boundary_11 // unsigned ,    RW, default = 0  the segment row boundary

#define LDC_REG_SEG_Y_BOUNDARY_6 (0x40000+(0x3007<<2))//0x4c01c
//Bit 31:26        reserved                           
//Bit 25:13        reg_ldc_seg_y_boundary_12 // unsigned ,    RW, default = 0  the segment row boundary
//Bit 12: 0        reg_ldc_seg_y_boundary_13 // unsigned ,    RW, default = 0  the segment row boundary

#define LDC_REG_SEG_Y_BOUNDARY_7 (0x40000+(0x3008<<2))//0x4c020
//Bit 31:26        reserved                           
//Bit 25:13        reg_ldc_seg_y_boundary_14 // unsigned ,    RW, default = 0  the segment row boundary
//Bit 12: 0        reg_ldc_seg_y_boundary_15 // unsigned ,    RW, default = 0  the segment row boundary

#define LDC_REG_SEG_Y_BOUNDARY_8 (0x40000+(0x3009<<2))//0x4c024
//Bit 31:26        reserved                           
//Bit 25:13        reg_ldc_seg_y_boundary_16 // unsigned ,    RW, default = 0  the segment row boundary
//Bit 12: 0        reg_ldc_seg_y_boundary_17 // unsigned ,    RW, default = 0  the segment row boundary

#define LDC_REG_SEG_Y_BOUNDARY_9 (0x40000+(0x300a<<2))//0x4c028
//Bit 31:26        reserved                           
//Bit 25:13        reg_ldc_seg_y_boundary_18 // unsigned ,    RW, default = 0  the segment row boundary
//Bit 12: 0        reg_ldc_seg_y_boundary_19 // unsigned ,    RW, default = 0  the segment row boundary

#define LDC_REG_SEG_Y_BOUNDARY_10 (0x40000+(0x300b<<2))//0x4c02c
//Bit 31:26        reserved                           
//Bit 25:13        reg_ldc_seg_y_boundary_20 // unsigned ,    RW, default = 0  the segment row boundary
//Bit 12: 0        reg_ldc_seg_y_boundary_21 // unsigned ,    RW, default = 0  the segment row boundary

#define LDC_REG_SEG_Y_BOUNDARY_11 (0x40000+(0x300c<<2))//0x4c030
//Bit 31:26        reserved                           
//Bit 25:13        reg_ldc_seg_y_boundary_22 // unsigned ,    RW, default = 0  the segment row boundary
//Bit 12: 0        reg_ldc_seg_y_boundary_23 // unsigned ,    RW, default = 0  the segment row boundary

#define LDC_REG_SEG_X_BOUNDARY_0 (0x40000+(0x3021<<2))//0x4c084
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_0  // unsigned ,    RW, default = 384  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_1  // unsigned ,    RW, default = 768  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_1 (0x40000+(0x3022<<2))//0x4c088
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_2  // unsigned ,    RW, default = 1152  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_3  // unsigned ,    RW, default = 1536  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_2 (0x40000+(0x3023<<2))//0x4c08c
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_4  // unsigned ,    RW, default = 1920  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_5  // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_3 (0x40000+(0x3024<<2))//0x4c090
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_6  // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_7  // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_4 (0x40000+(0x3025<<2))//0x4c094
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_8  // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_9  // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_5 (0x40000+(0x3026<<2))//0x4c098
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_10 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_11 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_6 (0x40000+(0x3027<<2))//0x4c09c
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_12 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_13 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_7 (0x40000+(0x3028<<2))//0x4c0a0
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_14 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_15 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_8 (0x40000+(0x3029<<2))//0x4c0a4
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_16 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_17 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_9 (0x40000+(0x302a<<2))//0x4c0a8
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_18 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_19 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_10 (0x40000+(0x302b<<2))//0x4c0ac
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_20 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_21 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_11 (0x40000+(0x302c<<2))//0x4c0b0
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_22 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_23 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_12 (0x40000+(0x302d<<2))//0x4c0b4
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_24 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_25 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_13 (0x40000+(0x302e<<2))//0x4c0b8
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_26 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_27 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_14 (0x40000+(0x302f<<2))//0x4c0bc
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_28 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_29 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_15 (0x40000+(0x3030<<2))//0x4c0c0
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_30 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_31 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_16 (0x40000+(0x3031<<2))//0x4c0c4
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_32 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_33 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_17 (0x40000+(0x3032<<2))//0x4c0c8
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_34 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_35 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_18 (0x40000+(0x3033<<2))//0x4c0cc
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_36 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_37 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_19 (0x40000+(0x3034<<2))//0x4c0d0
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_38 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_39 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_20 (0x40000+(0x3035<<2))//0x4c0d4
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_40 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_41 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_21 (0x40000+(0x3036<<2))//0x4c0d8
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_42 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_43 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_22 (0x40000+(0x3037<<2))//0x4c0dc
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_44 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_45 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_SEG_X_BOUNDARY_23 (0x40000+(0x3038<<2))//0x4c0e0
//Bit 31:28        reserved                           
//Bit 27:14        reg_ldc_seg_x_boundary_46 // unsigned ,    RW, default = 0  the segment col boundary
//Bit 13: 0        reg_ldc_seg_x_boundary_47 // unsigned ,    RW, default = 0  the segment col boundary

#define LDC_REG_PANEL_SIZE (0x40000+(0x3051<<2))//0x4c144
//Bit 31:16        reg_ldc_panel_width       // unsigned ,    RW, default = 1920  the panel width
//Bit 15: 0        reg_ldc_panel_height      // unsigned ,    RW, default = 1080  the panel height

#define LDC_REG_DOWNSAMPLE (0x40000+(0x3052<<2))//0x4c148
//Bit 31            reserved                           
//Bit 30:28        reg_ldc_hist_stat_mode    // unsigned ,    RW, default = 0  0:max(r,g,b), 1:R, 2:G, 3:B, 4:Y,
//Bit 27:24        reg_ldc_h_downsample      // unsigned ,    RW, default = 1  horizontal downsample scale, 0: disable , 1:/4,  2:/8, 3:/16, 4:/32
//Bit 23           reg_ldc_ds_filter_mode    // unsigned ,    RW, default = 1  horizontal downsample filter mode, 0: avg 1: max
//Bit 22:15        reg_ldc_y_gain            // unsigned ,    RW, default = 8'h40  the gain for input pixel value, 0x40 ==1
//Bit 14:12        reg_ldc_hist_mode         // unsigned ,    RW, default = 5  the mode for hist statistic value to calculate initial backlight,
//Bit 11           reg_ldc_hist_blend_mode   // unsigned ,    RW, default = 1  the blend mode to calculate initial backlight when hist mode=3, 0: max + avg, 1:max95 + avg
//Bit 10: 4        reg_ldc_hist_blend_alpha  // unsigned ,    RW, default = 7'h60  the coef for blend when hist mode=3, 128 means "1"
//Bit  3: 0        reg_ldc_hist_adap_blend_max_gain // unsigned ,    RW, default = 13  the gain for blend max value when hist mode=4

#define LDC_REG_HIST_OVERLAP (0x40000+(0x3053<<2))//0x4c14c
//Bit 31:28        reserved                           
//Bit 27:18        reg_ldc_seg_x_overlap     // unsigned ,    RW, default = 0  the overlap pixel num for each segment in col
//Bit 17: 8        reg_ldc_seg_y_overlap     // unsigned ,    RW, default = 0  the overlap pixel num for each segment in row
//Bit  7: 0        reg_ldc_max95_ratio       // unsigned ,    RW, default = 10  max95 percentile  //10

#define LDC_RO_GLOBAL_HIST_0 (0x40000+(0x3054<<2))//0x4c150
//Bit 31: 0        ro_ldc_glb_hist_0         // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_1 (0x40000+(0x3055<<2))//0x4c154
//Bit 31: 0        ro_ldc_glb_hist_1         // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_2 (0x40000+(0x3056<<2))//0x4c158
//Bit 31: 0        ro_ldc_glb_hist_2         // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_3 (0x40000+(0x3057<<2))//0x4c15c
//Bit 31: 0        ro_ldc_glb_hist_3         // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_4 (0x40000+(0x3058<<2))//0x4c160
//Bit 31: 0        ro_ldc_glb_hist_4         // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_5 (0x40000+(0x3059<<2))//0x4c164
//Bit 31: 0        ro_ldc_glb_hist_5         // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_6 (0x40000+(0x305a<<2))//0x4c168
//Bit 31: 0        ro_ldc_glb_hist_6         // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_7 (0x40000+(0x305b<<2))//0x4c16c
//Bit 31: 0        ro_ldc_glb_hist_7         // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_8 (0x40000+(0x305c<<2))//0x4c170
//Bit 31: 0        ro_ldc_glb_hist_8         // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_9 (0x40000+(0x305d<<2))//0x4c174
//Bit 31: 0        ro_ldc_glb_hist_9         // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_10 (0x40000+(0x305e<<2))//0x4c178
//Bit 31: 0        ro_ldc_glb_hist_10        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_11 (0x40000+(0x305f<<2))//0x4c17c
//Bit 31: 0        ro_ldc_glb_hist_11        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_12 (0x40000+(0x3060<<2))//0x4c180
//Bit 31: 0        ro_ldc_glb_hist_12        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_13 (0x40000+(0x3061<<2))//0x4c184
//Bit 31: 0        ro_ldc_glb_hist_13        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_14 (0x40000+(0x3062<<2))//0x4c188
//Bit 31: 0        ro_ldc_glb_hist_14        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_15 (0x40000+(0x3063<<2))//0x4c18c
//Bit 31: 0        ro_ldc_glb_hist_15        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_16 (0x40000+(0x3064<<2))//0x4c190
//Bit 31: 0        ro_ldc_glb_hist_16        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_17 (0x40000+(0x3065<<2))//0x4c194
//Bit 31: 0        ro_ldc_glb_hist_17        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_18 (0x40000+(0x3066<<2))//0x4c198
//Bit 31: 0        ro_ldc_glb_hist_18        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_19 (0x40000+(0x3067<<2))//0x4c19c
//Bit 31: 0        ro_ldc_glb_hist_19        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_20 (0x40000+(0x3068<<2))//0x4c1a0
//Bit 31: 0        ro_ldc_glb_hist_20        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_21 (0x40000+(0x3069<<2))//0x4c1a4
//Bit 31: 0        ro_ldc_glb_hist_21        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_22 (0x40000+(0x306a<<2))//0x4c1a8
//Bit 31: 0        ro_ldc_glb_hist_22        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_23 (0x40000+(0x306b<<2))//0x4c1ac
//Bit 31: 0        ro_ldc_glb_hist_23        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_24 (0x40000+(0x306c<<2))//0x4c1b0
//Bit 31: 0        ro_ldc_glb_hist_24        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_25 (0x40000+(0x306d<<2))//0x4c1b4
//Bit 31: 0        ro_ldc_glb_hist_25        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_26 (0x40000+(0x306e<<2))//0x4c1b8
//Bit 31: 0        ro_ldc_glb_hist_26        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_27 (0x40000+(0x306f<<2))//0x4c1bc
//Bit 31: 0        ro_ldc_glb_hist_27        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_28 (0x40000+(0x3070<<2))//0x4c1c0
//Bit 31: 0        ro_ldc_glb_hist_28        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_29 (0x40000+(0x3071<<2))//0x4c1c4
//Bit 31: 0        ro_ldc_glb_hist_29        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_30 (0x40000+(0x3072<<2))//0x4c1c8
//Bit 31: 0        ro_ldc_glb_hist_30        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_31 (0x40000+(0x3073<<2))//0x4c1cc
//Bit 31: 0        ro_ldc_glb_hist_31        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_32 (0x40000+(0x3074<<2))//0x4c1d0
//Bit 31: 0        ro_ldc_glb_hist_32        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_33 (0x40000+(0x3075<<2))//0x4c1d4
//Bit 31: 0        ro_ldc_glb_hist_33        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_34 (0x40000+(0x3076<<2))//0x4c1d8
//Bit 31: 0        ro_ldc_glb_hist_34        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_35 (0x40000+(0x3077<<2))//0x4c1dc
//Bit 31: 0        ro_ldc_glb_hist_35        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_36 (0x40000+(0x3078<<2))//0x4c1e0
//Bit 31: 0        ro_ldc_glb_hist_36        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_37 (0x40000+(0x3079<<2))//0x4c1e4
//Bit 31: 0        ro_ldc_glb_hist_37        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_38 (0x40000+(0x307a<<2))//0x4c1e8
//Bit 31: 0        ro_ldc_glb_hist_38        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_39 (0x40000+(0x307b<<2))//0x4c1ec
//Bit 31: 0        ro_ldc_glb_hist_39        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_40 (0x40000+(0x307c<<2))//0x4c1f0
//Bit 31: 0        ro_ldc_glb_hist_40        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_41 (0x40000+(0x307d<<2))//0x4c1f4
//Bit 31: 0        ro_ldc_glb_hist_41        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_42 (0x40000+(0x307e<<2))//0x4c1f8
//Bit 31: 0        ro_ldc_glb_hist_42        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_43 (0x40000+(0x307f<<2))//0x4c1fc
//Bit 31: 0        ro_ldc_glb_hist_43        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_44 (0x40000+(0x3080<<2))//0x4c200
//Bit 31: 0        ro_ldc_glb_hist_44        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_45 (0x40000+(0x3081<<2))//0x4c204
//Bit 31: 0        ro_ldc_glb_hist_45        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_46 (0x40000+(0x3082<<2))//0x4c208
//Bit 31: 0        ro_ldc_glb_hist_46        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_47 (0x40000+(0x3083<<2))//0x4c20c
//Bit 31: 0        ro_ldc_glb_hist_47        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_48 (0x40000+(0x3084<<2))//0x4c210
//Bit 31: 0        ro_ldc_glb_hist_48        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_49 (0x40000+(0x3085<<2))//0x4c214
//Bit 31: 0        ro_ldc_glb_hist_49        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_50 (0x40000+(0x3086<<2))//0x4c218
//Bit 31: 0        ro_ldc_glb_hist_50        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_51 (0x40000+(0x3087<<2))//0x4c21c
//Bit 31: 0        ro_ldc_glb_hist_51        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_52 (0x40000+(0x3088<<2))//0x4c220
//Bit 31: 0        ro_ldc_glb_hist_52        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_53 (0x40000+(0x3089<<2))//0x4c224
//Bit 31: 0        ro_ldc_glb_hist_53        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_54 (0x40000+(0x308a<<2))//0x4c228
//Bit 31: 0        ro_ldc_glb_hist_54        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_55 (0x40000+(0x308b<<2))//0x4c22c
//Bit 31: 0        ro_ldc_glb_hist_55        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_56 (0x40000+(0x308c<<2))//0x4c230
//Bit 31: 0        ro_ldc_glb_hist_56        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_57 (0x40000+(0x308d<<2))//0x4c234
//Bit 31: 0        ro_ldc_glb_hist_57        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_58 (0x40000+(0x308e<<2))//0x4c238
//Bit 31: 0        ro_ldc_glb_hist_58        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_59 (0x40000+(0x308f<<2))//0x4c23c
//Bit 31: 0        ro_ldc_glb_hist_59        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_60 (0x40000+(0x3090<<2))//0x4c240
//Bit 31: 0        ro_ldc_glb_hist_60        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_61 (0x40000+(0x3091<<2))//0x4c244
//Bit 31: 0        ro_ldc_glb_hist_61        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_62 (0x40000+(0x3092<<2))//0x4c248
//Bit 31: 0        ro_ldc_glb_hist_62        // unsigned ,    RO, default = 0  

#define LDC_RO_GLOBAL_HIST_63 (0x40000+(0x3093<<2))//0x4c24c
//Bit 31: 0        ro_ldc_glb_hist_63        // unsigned ,    RO, default = 0  

#define LDC_REG_BLEND_DIFF_TH (0x40000+(0x3094<<2))//0x4c250
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_hist_adap_blend_diff_th1 // unsigned ,    RW, default = 256  the threshold between max95 and average when hist mode=4, default= 4<<6
//Bit 11: 0        reg_ldc_hist_adap_blend_diff_th2 // unsigned ,    RW, default = 640  the threshold between max95 and average when hist mode=4, default= 10<<6

#define LDC_REG_CURVE_COEF (0x40000+(0x3095<<2))//0x4c254
//Bit 31:26        reserved                           
//Bit 25:18        reg_ldc_hist_adap_blend_gain_0 // unsigned ,    RW, default = 8'h70  the blend gain0 in curve when hist mode=4
//Bit 17:10        reg_ldc_hist_adap_blend_gain_1 // unsigned ,    RW, default = 8'h40  the blend gain1 in curve when hist mode=4
//Bit  9: 4        reg_ldc_hist_adap_blend_th0 // unsigned ,    RW, default = 2  the blend threshold base in curve when hist mode=4
//Bit  3: 0        reg_ldc_hist_adap_blend_thn // unsigned ,    RW, default = 4  the blend threshold increase step in curve when hist mode=4, th0+1<<thn

#define LDC_REG_INIT_BL (0x40000+(0x3096<<2))//0x4c258
//Bit 31:28        reserved                           
//Bit 27:16        reg_ldc_init_bl_min       // unsigned ,    RW, default = 0  the minimum value of init_bl
//Bit 15: 4        reg_ldc_init_bl_max       // unsigned ,    RW, default = 12'hfff  the maximum value of init_bl
//Bit  3           reg_ldc_init_bl_limit_mode // unsigned ,    RW, default = 1  0: use curve; 1: use reg
//Bit  2           reg_ldc_pre_dimming_curve_en // unsigned ,    RW, default = 0  
//Bit  1           reg_ldc_glb_dimming_bl_en // unsigned ,    RW, default = 0  
//Bit  0           reg_ldc_post_dimming_curve_en // unsigned ,    RW, default = 0  

#define LDC_REG_INIT_BL_MIN_CURVE_0 (0x40000+(0x3097<<2))//0x4c25c
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_init_bl_min_curve_0 // unsigned ,    RW, default = 0  
//Bit 11: 0        reg_ldc_init_bl_min_curve_1 // unsigned ,    RW, default = 0  

#define LDC_REG_INIT_BL_MIN_CURVE_1 (0x40000+(0x3098<<2))//0x4c260
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_init_bl_min_curve_2 // unsigned ,    RW, default = 0  
//Bit 11: 0        reg_ldc_init_bl_min_curve_3 // unsigned ,    RW, default = 0  

#define LDC_REG_INIT_BL_MIN_CURVE_2 (0x40000+(0x3099<<2))//0x4c264
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_init_bl_min_curve_4 // unsigned ,    RW, default = 0  
//Bit 11: 0        reg_ldc_init_bl_min_curve_5 // unsigned ,    RW, default = 0  

#define LDC_REG_INIT_BL_MIN_CURVE_3 (0x40000+(0x309a<<2))//0x4c268
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_init_bl_min_curve_6 // unsigned ,    RW, default = 0  
//Bit 11: 0        reg_ldc_init_bl_min_curve_7 // unsigned ,    RW, default = 0  

#define LDC_REG_INIT_BL_MIN_CURVE_4 (0x40000+(0x309b<<2))//0x4c26c
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_init_bl_min_curve_8 // unsigned ,    RW, default = 0  
//Bit 11: 0        reg_ldc_init_bl_min_curve_9 // unsigned ,    RW, default = 0  

#define LDC_REG_INIT_BL_MIN_CURVE_5 (0x40000+(0x309c<<2))//0x4c270
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_init_bl_min_curve_10 // unsigned ,    RW, default = 0  
//Bit 11: 0        reg_ldc_init_bl_min_curve_11 // unsigned ,    RW, default = 0  

#define LDC_REG_INIT_BL_MIN_CURVE_6 (0x40000+(0x309d<<2))//0x4c274
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_init_bl_min_curve_12 // unsigned ,    RW, default = 0  
//Bit 11: 0        reg_ldc_init_bl_min_curve_13 // unsigned ,    RW, default = 0  

#define LDC_REG_INIT_BL_MIN_CURVE_7 (0x40000+(0x309e<<2))//0x4c278
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_init_bl_min_curve_14 // unsigned ,    RW, default = 0  
//Bit 11: 0        reg_ldc_init_bl_min_curve_15 // unsigned ,    RW, default = 0  

#define LDC_REG_INIT_BL_MIN_CURVE (0x40000+(0x309f<<2))//0x4c27c
//Bit 31:12        reserved                           
//Bit 11: 0        reg_ldc_init_bl_min_curve_16 // unsigned ,    RW, default = 0  

#define LDC_REG_INIT_BL_MAX_CURVE_0 (0x40000+(0x30a0<<2))//0x4c280
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_init_bl_max_curve_0 // unsigned ,    RW, default = 4095  
//Bit 11: 0        reg_ldc_init_bl_max_curve_1 // unsigned ,    RW, default = 4095  

#define LDC_REG_INIT_BL_MAX_CURVE_1 (0x40000+(0x30a1<<2))//0x4c284
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_init_bl_max_curve_2 // unsigned ,    RW, default = 4095  
//Bit 11: 0        reg_ldc_init_bl_max_curve_3 // unsigned ,    RW, default = 4095  

#define LDC_REG_INIT_BL_MAX_CURVE_2 (0x40000+(0x30a2<<2))//0x4c288
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_init_bl_max_curve_4 // unsigned ,    RW, default = 4095  
//Bit 11: 0        reg_ldc_init_bl_max_curve_5 // unsigned ,    RW, default = 4095  

#define LDC_REG_INIT_BL_MAX_CURVE_3 (0x40000+(0x30a3<<2))//0x4c28c
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_init_bl_max_curve_6 // unsigned ,    RW, default = 4095  
//Bit 11: 0        reg_ldc_init_bl_max_curve_7 // unsigned ,    RW, default = 4095  

#define LDC_REG_INIT_BL_MAX_CURVE_4 (0x40000+(0x30a4<<2))//0x4c290
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_init_bl_max_curve_8 // unsigned ,    RW, default = 4095  
//Bit 11: 0        reg_ldc_init_bl_max_curve_9 // unsigned ,    RW, default = 4095  

#define LDC_REG_INIT_BL_MAX_CURVE_5 (0x40000+(0x30a5<<2))//0x4c294
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_init_bl_max_curve_10 // unsigned ,    RW, default = 4095  
//Bit 11: 0        reg_ldc_init_bl_max_curve_11 // unsigned ,    RW, default = 4095  

#define LDC_REG_INIT_BL_MAX_CURVE_6 (0x40000+(0x30a6<<2))//0x4c298
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_init_bl_max_curve_12 // unsigned ,    RW, default = 4095  
//Bit 11: 0        reg_ldc_init_bl_max_curve_13 // unsigned ,    RW, default = 4095  

#define LDC_REG_INIT_BL_MAX_CURVE_7 (0x40000+(0x30a7<<2))//0x4c29c
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_init_bl_max_curve_14 // unsigned ,    RW, default = 4095  
//Bit 11: 0        reg_ldc_init_bl_max_curve_15 // unsigned ,    RW, default = 4095  

#define LDC_REG_INIT_BL_MAX_CURVE (0x40000+(0x30a8<<2))//0x4c2a0
//Bit 31:12        reserved                           
//Bit 11: 0        reg_ldc_init_bl_max_curve_16 // unsigned ,    RW, default = 4095  

#define LDC_REG_PRE_DIMMING_CURVE_0 (0x40000+(0x30a9<<2))//0x4c2a4
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_pre_dimming_curve_0 // unsigned ,    RW, default = 0  
//Bit 11: 0        reg_ldc_pre_dimming_curve_1 // unsigned ,    RW, default = 256  

#define LDC_REG_PRE_DIMMING_CURVE_1 (0x40000+(0x30aa<<2))//0x4c2a8
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_pre_dimming_curve_2 // unsigned ,    RW, default = 512  
//Bit 11: 0        reg_ldc_pre_dimming_curve_3 // unsigned ,    RW, default = 768  

#define LDC_REG_PRE_DIMMING_CURVE_2 (0x40000+(0x30ab<<2))//0x4c2ac
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_pre_dimming_curve_4 // unsigned ,    RW, default = 1024  
//Bit 11: 0        reg_ldc_pre_dimming_curve_5 // unsigned ,    RW, default = 1280  

#define LDC_REG_PRE_DIMMING_CURVE_3 (0x40000+(0x30ac<<2))//0x4c2b0
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_pre_dimming_curve_6 // unsigned ,    RW, default = 1536  
//Bit 11: 0        reg_ldc_pre_dimming_curve_7 // unsigned ,    RW, default = 1792  

#define LDC_REG_PRE_DIMMING_CURVE_4 (0x40000+(0x30ad<<2))//0x4c2b4
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_pre_dimming_curve_8 // unsigned ,    RW, default = 2048  
//Bit 11: 0        reg_ldc_pre_dimming_curve_9 // unsigned ,    RW, default = 2304  

#define LDC_REG_PRE_DIMMING_CURVE_5 (0x40000+(0x30ae<<2))//0x4c2b8
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_pre_dimming_curve_10 // unsigned ,    RW, default = 2560  
//Bit 11: 0        reg_ldc_pre_dimming_curve_11 // unsigned ,    RW, default = 2816  

#define LDC_REG_PRE_DIMMING_CURVE_6 (0x40000+(0x30af<<2))//0x4c2bc
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_pre_dimming_curve_12 // unsigned ,    RW, default = 3072  
//Bit 11: 0        reg_ldc_pre_dimming_curve_13 // unsigned ,    RW, default = 3328  

#define LDC_REG_PRE_DIMMING_CURVE_7 (0x40000+(0x30b0<<2))//0x4c2c0
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_pre_dimming_curve_14 // unsigned ,    RW, default = 3584  
//Bit 11: 0        reg_ldc_pre_dimming_curve_15 // unsigned ,    RW, default = 3840  

#define LDC_REG_PRE_DIMMING_CURVE (0x40000+(0x30b1<<2))//0x4c2c4
//Bit 31:12        reserved                           
//Bit 11: 0        reg_ldc_pre_dimming_curve_16 // unsigned ,    RW, default = 4095  

#define LDC_REG_INIT_BL_MODE5_ALPHA01 (0x40000+(0x30b2<<2))//0x4c2c8
//Bit 31:26        reserved                           
//Bit 25:18        reg_ldc_hist_alpha1_gain_0 // unsigned ,    RW, default = 8'h40  the alpha1 blend gain0 in curve when hist mode=5
//Bit 17:10        reg_ldc_hist_alpha1_gain_1 // unsigned ,    RW, default = 8'h7F  the alpha1 blend gain1 in curve when hist mode=5
//Bit  9: 4        reg_ldc_hist_alpha1_blend_th0 // unsigned ,    RW, default = 6  the alpha1 blend threshold base in curve when hist mode=5
//Bit  3: 0        reg_ldc_hist_alpha1_blend_thn // unsigned ,    RW, default = 5  the alpha1 blend threshold increase step in curve when hist mode=5, th0+1<<thn

#define LDC_REG_INIT_BL_MODE5_ALPHA02 (0x40000+(0x30b3<<2))//0x4c2cc
//Bit 31:26        reserved                           
//Bit 25:18        reg_ldc_hist_alpha2_gain_0 // unsigned ,    RW, default = 8'h40  the alpha2 blend gain0 in curve when hist mode=5
//Bit 17:10        reg_ldc_hist_alpha2_gain_1 // unsigned ,    RW, default = 8'h70  the alpha2 blend gain1 in curve when hist mode=5
//Bit  9: 4        reg_ldc_hist_alpha2_blend_th0 // unsigned ,    RW, default = 26  the alpha2 blend threshold base in curve when hist mode=5
//Bit  3: 0        reg_ldc_hist_alpha2_blend_thn // unsigned ,    RW, default = 5  the alpha2 blend threshold increase step in curve when hist mode=5, th0+1<<thn

#define LDC_REG_INIT_BL_MODE5_BLEND (0x40000+(0x30b4<<2))//0x4c2d0
//Bit 31:29        reserved                           
//Bit 28:21        reg_ldc_hist_mode5_adap_blend_gain_0 // unsigned ,    RW, default = 8'h60  the blend gain0 in curve when hist mode=5
//Bit 20:13        reg_ldc_hist_mode5_adap_blend_gain_1 // unsigned ,    RW, default = 8'h7F  the blend gain1 in curve when hist mode=5
//Bit 12: 7        reg_ldc_hist_mode5_adap_blend_th0 // unsigned ,    RW, default = 2  the blend threshold base in curve when hist mode=5
//Bit  6: 3        reg_ldc_hist_mode5_adap_blend_thn // unsigned ,    RW, default = 5  the blend threshold increase step in curve when hist mode=5, th0+1<<thn
//Bit  2: 0        reg_ldc_hist_blend_alpha0_mode // unsigned ,    RW, default = 2  the blend alpha0 mode when hist mode=5, 0:alpha1, 1:alpha2, 2:max, 3:min, 4:avg

#define LDC_REG_SF_MODE (0x40000+(0x30b5<<2))//0x4c2d4
//Bit 31:27        reserved                           
//Bit 26:25        reg_ldc_sf_mode           // unsigned ,    RW, default = 2  the mode for backlight spatial filter, 0: no filter, 1: TSF filter , 2: new spatial filter
//Bit 24:13        reg_ldc_sf_tsf_3x3        // unsigned ,    RW, default = 12'h600  smoothness control of the spatial filter at 3x3
//Bit 12: 1        reg_ldc_sf_tsf_5x5        // unsigned ,    RW, default = 12'hC00  smoothness control of the spatial filter at 5x5
//Bit  0           reg_ldc_sf2_win_mode      // unsigned ,    RW, default = 0  0: -1~+1, 1: -2~+2

#define LDC_REG_SF_GAIN (0x40000+(0x30b6<<2))//0x4c2d8
//Bit 31:16        reserved                           
//Bit 15: 8        reg_ldc_sf_gain_up        // unsigned ,    RW, default = 8'h20  the gain for backlight up
//Bit  7: 0        reg_ldc_sf_gain_dn        // unsigned ,    RW, default = 8'h00  the gain for backlight down,256 as "1"

#define LDC_REG_BS_MODE (0x40000+(0x30b7<<2))//0x4c2dc
//Bit 31:16        reserved                           
//Bit 15:13        reg_ldc_bs_bl_mode        // unsigned ,    RW, default = 0  the boost mode, 0:no boost,1:glb boost,2:local boost,3:max,4:min,5:avg
//Bit 12: 1        reg_ldc_glb_apl           // unsigned ,    RW, default = 0  global apl for one channel
//Bit  0           reg_ldc_glb_apl_mode      // unsigned ,    RW, default = 1  0: reg_ldc_glb_apl, 1: ro_ldc_glb_apl

#define LDC_REG_APL (0x40000+(0x30b8<<2))//0x4c2e0
//Bit 31: 8        reserved                           
//Bit  7: 0        reg_ldc_bs_glb_apl_gain   // unsigned ,    RW, default = 8'h20  0x10 as "1"

#define LDC_REG_GLB_BOOST (0x40000+(0x30b9<<2))//0x4c2e4
//Bit 31:28        reserved                           
//Bit 27:16        reg_ldc_bs_dark_scene_bl_th // unsigned ,    RW, default = 12'h200  the global threshold for dark scene
//Bit 15: 8        reg_ldc_bs_gain           // unsigned ,    RW, default = 8'h20  the boost gain, default=0x20, 256 as "1"
//Bit  7: 0        reg_ldc_bs_limit_gain     // unsigned ,    RW, default = 8'h60  the boost limit gain, 0x40 as "1"

#define LDC_REG_LOCAL_BOOST (0x40000+(0x30ba<<2))//0x4c2e8
//Bit 31:28        reserved                           
//Bit 27:20        reg_ldc_bs_loc_apl_gain   // unsigned ,    RW, default = 8'h20  the local apl gain for dark scene, 0x10 as "1"
//Bit 19:12        reg_ldc_bs_loc_max_min_gain // unsigned ,    RW, default = 8'h20  the local boost gain,0x10 as "1"
//Bit 11: 0        reg_ldc_bs_loc_dark_scene_bl_th // unsigned ,    RW, default = 12'h600  the local threshold for dark scene

#define LDC_REG_GLB_DIMMING_BL_CURVE_0 (0x40000+(0x30bb<<2))//0x4c2ec
//Bit 31:24        reg_ldc_glb_apl_gain_curve_0 // unsigned ,    RW, default = 0  
//Bit 23:16        reg_ldc_glb_apl_gain_curve_1 // unsigned ,    RW, default = 16  
//Bit 15: 8        reg_ldc_glb_apl_gain_curve_2 // unsigned ,    RW, default = 32  
//Bit  7: 0        reg_ldc_glb_apl_gain_curve_3 // unsigned ,    RW, default = 48  

#define LDC_REG_GLB_DIMMING_BL_CURVE_1 (0x40000+(0x30bc<<2))//0x4c2f0
//Bit 31:24        reg_ldc_glb_apl_gain_curve_4 // unsigned ,    RW, default = 64  
//Bit 23:16        reg_ldc_glb_apl_gain_curve_5 // unsigned ,    RW, default = 80  
//Bit 15: 8        reg_ldc_glb_apl_gain_curve_6 // unsigned ,    RW, default = 96  
//Bit  7: 0        reg_ldc_glb_apl_gain_curve_7 // unsigned ,    RW, default = 112  

#define LDC_REG_GLB_DIMMING_BL_CURVE_2 (0x40000+(0x30bd<<2))//0x4c2f4
//Bit 31:24        reg_ldc_glb_apl_gain_curve_8 // unsigned ,    RW, default = 128  
//Bit 23:16        reg_ldc_glb_apl_gain_curve_9 // unsigned ,    RW, default = 144  
//Bit 15: 8        reg_ldc_glb_apl_gain_curve_10 // unsigned ,    RW, default = 160  
//Bit  7: 0        reg_ldc_glb_apl_gain_curve_11 // unsigned ,    RW, default = 176  

#define LDC_REG_GLB_DIMMING_BL_CURVE_3 (0x40000+(0x30be<<2))//0x4c2f8
//Bit 31:24        reg_ldc_glb_apl_gain_curve_12 // unsigned ,    RW, default = 192  
//Bit 23:16        reg_ldc_glb_apl_gain_curve_13 // unsigned ,    RW, default = 208  
//Bit 15: 8        reg_ldc_glb_apl_gain_curve_14 // unsigned ,    RW, default = 224  
//Bit  7: 0        reg_ldc_glb_apl_gain_curve_15 // unsigned ,    RW, default = 240  

#define LDC_REG_GLB_DIMMING_BL_CURVE (0x40000+(0x30bf<<2))//0x4c2fc
//Bit 31: 8        reserved                           
//Bit  7: 0        reg_ldc_glb_apl_gain_curve_16 // unsigned ,    RW, default = 255  

#define LDC_REG_TF (0x40000+(0x30c0<<2))//0x4c300
//Bit 31:24        reg_ldc_tf_low_alpha      // unsigned ,    RW, default = 8'h20  the low alpha for temporal filter
//Bit 23:16        reg_ldc_tf_high_alpha     // unsigned ,    RW, default = 8'h20  the high alpha for temporal filter
//Bit 15: 8        reg_ldc_tf_low_alpha_sc   // unsigned ,    RW, default = 8'h40  the low alpha for temporal filter in scene change
//Bit  7: 0        reg_ldc_tf_high_alpha_sc  // unsigned ,    RW, default = 8'h40  the high alpha for temporal filter in scene change

#define LDC_REG_TF_SC (0x40000+(0x30c1<<2))//0x4c304
//Bit 31:28        reserved                           
//Bit 27           reg_ldc_tf_en             // unsigned ,    RW, default = 0  temporal filter enable bit
//Bit 26           reg_ldc_tf_sc_flag        // unsigned ,    RW, default = 0  the flag for scene change
//Bit 25:22        reg_ldc_cmp_mask_x        // unsigned ,    RW, default = 7  the mask width from profile to calculate one block intensity
//Bit 21:18        reg_ldc_cmp_mask_y        // unsigned ,    RW, default = 7  the mask height from profile to calculate one block intensity
//Bit 17:12        reg_ldc_hist_big_change_cnt_th // unsigned ,    RW, default = 63  
//Bit 11: 0        reg_ldc_glb_apl_diff_th   // unsigned ,    RW, default = 1023  use u10

#define LDC_REG_TF_SC_BIG_TH (0x40000+(0x30c2<<2))//0x4c308
//Bit 31: 0        reg_ldc_hist_big_change_th // unsigned ,    RW, default = 31'hFFFFFFFF  UINT32

#define LDC_REG_TF_SUM_TH (0x40000+(0x30c3<<2))//0x4c30c
//Bit 31: 0        reg_ldc_hist_diff_sum_th  // unsigned ,    RW, default = 32'hFFFFFFFF  UINT32

#define LDC_REG_POST_DIMMING_CURVE_0 (0x40000+(0x30c4<<2))//0x4c310
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_post_dimming_curve_0 // unsigned ,    RW, default = 0  
//Bit 11: 0        reg_ldc_post_dimming_curve_1 // unsigned ,    RW, default = 256  

#define LDC_REG_POST_DIMMING_CURVE_1 (0x40000+(0x30c5<<2))//0x4c314
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_post_dimming_curve_2 // unsigned ,    RW, default = 512  
//Bit 11: 0        reg_ldc_post_dimming_curve_3 // unsigned ,    RW, default = 768  

#define LDC_REG_POST_DIMMING_CURVE_2 (0x40000+(0x30c6<<2))//0x4c318
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_post_dimming_curve_4 // unsigned ,    RW, default = 1024  
//Bit 11: 0        reg_ldc_post_dimming_curve_5 // unsigned ,    RW, default = 1280  

#define LDC_REG_POST_DIMMING_CURVE_3 (0x40000+(0x30c7<<2))//0x4c31c
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_post_dimming_curve_6 // unsigned ,    RW, default = 1536  
//Bit 11: 0        reg_ldc_post_dimming_curve_7 // unsigned ,    RW, default = 1792  

#define LDC_REG_POST_DIMMING_CURVE_4 (0x40000+(0x30c8<<2))//0x4c320
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_post_dimming_curve_8 // unsigned ,    RW, default = 2048  
//Bit 11: 0        reg_ldc_post_dimming_curve_9 // unsigned ,    RW, default = 2304  

#define LDC_REG_POST_DIMMING_CURVE_5 (0x40000+(0x30c9<<2))//0x4c324
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_post_dimming_curve_10 // unsigned ,    RW, default = 2560  
//Bit 11: 0        reg_ldc_post_dimming_curve_11 // unsigned ,    RW, default = 2816  

#define LDC_REG_POST_DIMMING_CURVE_6 (0x40000+(0x30ca<<2))//0x4c328
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_post_dimming_curve_12 // unsigned ,    RW, default = 3072  
//Bit 11: 0        reg_ldc_post_dimming_curve_13 // unsigned ,    RW, default = 3328  

#define LDC_REG_POST_DIMMING_CURVE_7 (0x40000+(0x30cb<<2))//0x4c32c
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_post_dimming_curve_14 // unsigned ,    RW, default = 3584  
//Bit 11: 0        reg_ldc_post_dimming_curve_15 // unsigned ,    RW, default = 3840  

#define LDC_REG_POST_DIMMING_CURVE (0x40000+(0x30cc<<2))//0x4c330
//Bit 31:12        reserved                           
//Bit 11: 0        reg_ldc_post_dimming_curve_16 // unsigned ,    RW, default = 4095  

#define LDC_REG_PROFILE_MODE (0x40000+(0x30cd<<2))//0x4c334
//Bit 31:24        reserved                           
//Bit 23: 8        reg_ldc_profile_k         // unsigned ,    RW, default = 16'h240  the profile numerator coef
//Bit  7: 0        reg_ldc_profile_bits      // unsigned ,    RW, default = 8'h18  the profile denominator coef

#define LDC_REG_BLK_FILTER (0x40000+(0x30ce<<2))//0x4c338
//Bit 31:16        reserved                           
//Bit 15: 8        reg_ldc_block_filter_A    // unsigned ,    RW, default = 56  the coef for block intensity filter
//Bit  7: 0        reg_ldc_block_filter_B    // unsigned ,    RW, default = 37  the coef for block intensity filter

#define LDC_REG_BLK_FILTER_COEF (0x40000+(0x30cf<<2))//0x4c33c
//Bit 31:24        reg_ldc_block_filter_C    // unsigned ,    RW, default = 20  the coef for block intensity filter
//Bit 23:16        reg_ldc_block_filter_D    // unsigned ,    RW, default = 10  the coef for block intensity filter
//Bit 15: 8        reg_ldc_block_filter_E    // unsigned ,    RW, default = 5  the coef for block intensity filter
//Bit  7: 0        reg_ldc_block_filter_F    // unsigned ,    RW, default = 2  the coef for block intensity filter

#define LDC_REG_BLK_INTEN_BOOST (0x40000+(0x30d0<<2))//0x4c340
//Bit 31:24        reg_ldc_block_filter_up_gain // unsigned ,    RW, default = 4  up gain
//Bit 23:16        reg_ldc_block_filter_dn_gain // unsigned ,    RW, default = 16  down gain
//Bit 15: 8        reg_ldc_block_filter_max_margin // signed ,    RW, default = 8  max threshold
//Bit  7: 0        reg_ldc_block_filter_min_margin // signed ,    RW, default = 8  min threshold

#define LDC_REG_BLK_INTEN_BOOST_EN (0x40000+(0x30d1<<2))//0x4c344
//Bit 31:29        reserved                           
//Bit 28           reg_ldc_block_intensity_boost_en // unsigned ,    RW, default = 0  block intensity filter boost enable
//Bit 27:16        reg_ldc_block_intensity_boost_dutydiff_th // unsigned ,    RW, default = 0  threshold of duty diff in boost block intensity xcross segment .
//Bit 15: 8        reg_ldc_block_filter_bl_diff_thd // unsigned ,    RW, default = 40  backlight diff threshold
//Bit  7: 5        reserved                           
//Bit  4           reg_ldc_block_intensity_check_3x3_en // unsigned ,    RW, default = 1  windown size to get min and max value, 0:5x5, 1: 3x3
//Bit  3           reg_ldc_block_intensity_boost_win_h // unsigned ,    RW, default = 0  horizontal size of boost window, 0:+-2, 1:+-3
//Bit  2           reg_ldc_block_intensity_post_filter_en // unsigned ,    RW, default = 0  enable block intensity filter post processing
//Bit  1           reg_ldc_block_intensity_post_filter_mode // unsigned ,    RW, default = 0  block intensity post processing filter coef, 0:[1 2 1; 2 4 2; 1 2 1], 1:[1 1 1; 1 8 1; 1 1 1]
//Bit  0           reg_ldc_pixel_intensity_interp_mode // unsigned ,    RW, default = 0  pixel intensity interpolation mode, 0: bilinear interpolation, 1: curve interpolation, lut

#define LDC_REG_BL_MEMORY (0x40000+(0x30d2<<2))//0x4c348
//Bit 31:16        reserved                           
//Bit 15:14        reg_ldc_post_bl_fid_sel   // unsigned ,    RW, default = 0  the option of fid, 0:input_fid, 1: output_fid, 2: power_fid
//Bit 13           reg_ldc_bl_adp_frm_en     // unsigned ,    RW, default = 0  enable input frame backlight index, 0:ro,1:reg
//Bit 12: 9        reg_ldc_bl_input_fid      // unsigned ,    RW, default = 0  the memory input frame backlight index
//Bit  8: 6        reg_ldc_bl_buf_diff       // unsigned ,    RW, default = 1  the memory diff frame
//Bit  5: 3        reg_ldc_bl_buf_num        // unsigned ,    RW, default = 4  the total memory frames
//Bit  2: 0        reg_ldc_post_bl_buf_diff  // unsigned ,    RW, default = 1  the memory diff frame of post backlight

#define LDC_REG_BL_MEMORY_RO (0x40000+(0x30d3<<2))//0x4c34c
//Bit 31:12        reserved                           
//Bit 11: 8        ro_ldc_bl_input_fid       // unsigned ,    RO, default = 0  the memory input frame backlight index
//Bit  7: 4        ro_ldc_bl_output_fid      // unsigned ,    RO, default = 3  the memory output frame backlight index
//Bit  3: 0        ro_ldc_post_bl_output_fid // unsigned ,    RO, default = 3  the memory output frame backlight index of post backlight

#define LDC_REG_FACTOR_DIV_0 (0x40000+(0x30d4<<2))//0x4c350
//Bit 31:16        reg_ldc_factor_for_div_00 // unsigned ,    RW, default = 33893  the factor for divide,33893:2160; 34952:1080
//Bit 15: 0        reg_ldc_factor_for_div_01 // unsigned ,    RW, default = 33893  the factor for divide,33893:2160; 34952:1080

#define LDC_REG_FACTOR_DIV_1 (0x40000+(0x30d5<<2))//0x4c354
//Bit 31:16        reg_ldc_factor_for_div_10 // unsigned ,    RW, default = 32896  the factor for divide
//Bit 15: 0        reg_ldc_factor_for_div_11 // unsigned ,    RW, default = 32896  the factor for divide

#define LDC_REG_BITS_DIV (0x40000+(0x30d6<<2))//0x4c358
//Bit 31:24        reg_ldc_bits_for_div_00   // unsigned ,    RW, default = 25  the bits for divide, 25:2160; 23:1080
//Bit 23:16        reg_ldc_bits_for_div_01   // unsigned ,    RW, default = 25  the bits for divide, 25:2160; 23:1080
//Bit 15: 8        reg_ldc_bits_for_div_10   // unsigned ,    RW, default = 25  the bits for divide, 25:2160; 23:1080
//Bit  7: 0        reg_ldc_bits_for_div_11   // unsigned ,    RW, default = 25  the bits for divide, 25:2160; 23:1080

#define LDC_REG_RGB_FACTOR_DIV_XY (0x40000+(0x30d7<<2))//0x4c35c
//Bit 31:16        reg_ldc_rgb_gain_div_x_factor // unsigned ,    RW, default = 1638  the factor for divide, 819:3840; 1638:1920, when seg=48x32
//Bit 15: 0        reg_ldc_rgb_gain_div_y_factor // unsigned ,    RW, default = 1941  the factor for divide, 970:2160; 1941:1080, when seg=48x32


#define LDC_REG_RGB_FORCE_VAL_0 (0x40000+(0x3100<<2))//0x4c400
//Bit 31:20        reserved                           
//Bit 19:12        reg_ldc_rgb_gain_seg_col  // unsigned ,    RW, default = 5  the number of col segments for rgb gain
//Bit 11: 4        reg_ldc_rgb_gain_seg_row  // unsigned ,    RW, default = 9  the number of row segments for rgb gain
//Bit  3            reserved                           
//Bit  2           reg_ldc_force_rgb_gain_en // unsigned ,    RW, default = 0  force gain, 0:off, 1:on
//Bit  1: 0        reg_ldc_rgb_gain_ds_mode  // unsigned ,    RW, default = 1  0:without downsample, 1:horizontal downsample, 2:vertical downsample, 3:horizontal and vertical downsample

#define LDC_REG_RGB_FORCE_VAL_1 (0x40000+(0x3101<<2))//0x4c404
//Bit 31:30        reserved                           
//Bit 29:20        reg_ldc_force_r_gain_val  // unsigned ,    RW, default = 256  
//Bit 19:10        reg_ldc_force_g_gain_val  // unsigned ,    RW, default = 256  
//Bit  9: 0        reg_ldc_force_b_gain_val  // unsigned ,    RW, default = 256  

#define LDC_REG_GLB_GAIN (0x40000+(0x3102<<2))//0x4c408
//Bit 31:12        reserved                           
//Bit 11: 0        reg_ldc_glb_gain          // unsigned ,    RW, default = 1024  the global coef for compensation gain

#define LDC_REG_MIN_GAIN_LUT_0 (0x40000+(0x3103<<2))//0x4c40c
//Bit 31:24        reg_ldc_min_gain_lut_0    // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 23:16        reg_ldc_min_gain_lut_1    // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 15: 8        reg_ldc_min_gain_lut_2    // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit  7: 0        reg_ldc_min_gain_lut_3    // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut

#define LDC_REG_MIN_GAIN_LUT_1 (0x40000+(0x3104<<2))//0x4c410
//Bit 31:24        reg_ldc_min_gain_lut_4    // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 23:16        reg_ldc_min_gain_lut_5    // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 15: 8        reg_ldc_min_gain_lut_6    // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit  7: 0        reg_ldc_min_gain_lut_7    // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut

#define LDC_REG_MIN_GAIN_LUT_2 (0x40000+(0x3105<<2))//0x4c414
//Bit 31:24        reg_ldc_min_gain_lut_8    // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 23:16        reg_ldc_min_gain_lut_9    // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 15: 8        reg_ldc_min_gain_lut_10   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit  7: 0        reg_ldc_min_gain_lut_11   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut

#define LDC_REG_MIN_GAIN_LUT_3 (0x40000+(0x3106<<2))//0x4c418
//Bit 31:24        reg_ldc_min_gain_lut_12   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 23:16        reg_ldc_min_gain_lut_13   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 15: 8        reg_ldc_min_gain_lut_14   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit  7: 0        reg_ldc_min_gain_lut_15   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut

#define LDC_REG_MIN_GAIN_LUT_4 (0x40000+(0x3107<<2))//0x4c41c
//Bit 31:24        reg_ldc_min_gain_lut_16   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 23:16        reg_ldc_min_gain_lut_17   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 15: 8        reg_ldc_min_gain_lut_18   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit  7: 0        reg_ldc_min_gain_lut_19   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut

#define LDC_REG_MIN_GAIN_LUT_5 (0x40000+(0x3108<<2))//0x4c420
//Bit 31:24        reg_ldc_min_gain_lut_20   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 23:16        reg_ldc_min_gain_lut_21   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 15: 8        reg_ldc_min_gain_lut_22   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit  7: 0        reg_ldc_min_gain_lut_23   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut

#define LDC_REG_MIN_GAIN_LUT_6 (0x40000+(0x3109<<2))//0x4c424
//Bit 31:24        reg_ldc_min_gain_lut_24   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 23:16        reg_ldc_min_gain_lut_25   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 15: 8        reg_ldc_min_gain_lut_26   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit  7: 0        reg_ldc_min_gain_lut_27   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut

#define LDC_REG_MIN_GAIN_LUT_7 (0x40000+(0x310a<<2))//0x4c428
//Bit 31:24        reg_ldc_min_gain_lut_28   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 23:16        reg_ldc_min_gain_lut_29   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 15: 8        reg_ldc_min_gain_lut_30   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit  7: 0        reg_ldc_min_gain_lut_31   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut

#define LDC_REG_MIN_GAIN_LUT_8 (0x40000+(0x310b<<2))//0x4c42c
//Bit 31:24        reg_ldc_min_gain_lut_32   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 23:16        reg_ldc_min_gain_lut_33   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 15: 8        reg_ldc_min_gain_lut_34   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit  7: 0        reg_ldc_min_gain_lut_35   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut

#define LDC_REG_MIN_GAIN_LUT_9 (0x40000+(0x310c<<2))//0x4c430
//Bit 31:24        reg_ldc_min_gain_lut_36   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 23:16        reg_ldc_min_gain_lut_37   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 15: 8        reg_ldc_min_gain_lut_38   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit  7: 0        reg_ldc_min_gain_lut_39   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut

#define LDC_REG_MIN_GAIN_LUT_10 (0x40000+(0x310d<<2))//0x4c434
//Bit 31:24        reg_ldc_min_gain_lut_40   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 23:16        reg_ldc_min_gain_lut_41   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 15: 8        reg_ldc_min_gain_lut_42   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit  7: 0        reg_ldc_min_gain_lut_43   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut

#define LDC_REG_MIN_GAIN_LUT_11 (0x40000+(0x310e<<2))//0x4c438
//Bit 31:24        reg_ldc_min_gain_lut_44   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 23:16        reg_ldc_min_gain_lut_45   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 15: 8        reg_ldc_min_gain_lut_46   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit  7: 0        reg_ldc_min_gain_lut_47   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut

#define LDC_REG_MIN_GAIN_LUT_12 (0x40000+(0x310f<<2))//0x4c43c
//Bit 31:24        reg_ldc_min_gain_lut_48   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 23:16        reg_ldc_min_gain_lut_49   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 15: 8        reg_ldc_min_gain_lut_50   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit  7: 0        reg_ldc_min_gain_lut_51   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut

#define LDC_REG_MIN_GAIN_LUT_13 (0x40000+(0x3110<<2))//0x4c440
//Bit 31:24        reg_ldc_min_gain_lut_52   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 23:16        reg_ldc_min_gain_lut_53   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 15: 8        reg_ldc_min_gain_lut_54   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit  7: 0        reg_ldc_min_gain_lut_55   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut

#define LDC_REG_MIN_GAIN_LUT_14 (0x40000+(0x3111<<2))//0x4c444
//Bit 31:24        reg_ldc_min_gain_lut_56   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 23:16        reg_ldc_min_gain_lut_57   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 15: 8        reg_ldc_min_gain_lut_58   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit  7: 0        reg_ldc_min_gain_lut_59   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut

#define LDC_REG_MIN_GAIN_LUT_15 (0x40000+(0x3112<<2))//0x4c448
//Bit 31:24        reg_ldc_min_gain_lut_60   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 23:16        reg_ldc_min_gain_lut_61   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit 15: 8        reg_ldc_min_gain_lut_62   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut
//Bit  7: 0        reg_ldc_min_gain_lut_63   // unsigned ,    RW, default = 8'h40  the coef for compensation limit min gain lut

#define LDC_REG_BLK_FILTER_TSF3 (0x40000+(0x3113<<2))//0x4c44c
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_blk_intensity_filter_tsf_0_0 // unsigned ,    RW, default = 8  smoothness control of the blk_intensity filter at 3 in the nearest case
//Bit 11: 0        reg_ldc_blk_intensity_filter_tsf_0_1 // unsigned ,    RW, default = 16  smoothness control of the blk_intensity filter at 5 in the nearest case

#define LDC_REG_BLK_FILTER_TSF5 (0x40000+(0x3114<<2))//0x4c450
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_blk_intensity_filter_tsf_1_0 // unsigned ,    RW, default = 7  smoothness control of the blk_intensity filter at 3 in the second-nearest case
//Bit 11: 0        reg_ldc_blk_intensity_filter_tsf_1_1 // unsigned ,    RW, default = 17  smoothness control of the blk_intensity filter at 5 in the second-nearest case

#define LDC_REG_DITHER (0x40000+(0x3115<<2))//0x4c454
//Bit 31:18        reserved                           
//Bit 17           reg_ldc_blk_intensity_filter_en // unsigned ,    RW, default = 1  block intensity filter enable
//Bit 16           reg_ldc_pixel_cmp_en      // unsigned ,    RW, default = 1  pixel compensation enable
//Bit 15           reg_ldc_dth_en            // unsigned ,    RW, default = 0  the enable dither
//Bit 14           reg_ldc_dth_bw            // unsigned ,    RW, default = 0  0: 2bits, 1: 4bits
//Bit 13:12        reg_ldc_blk_intensity_filter_mode // unsigned ,    RW, default = 1  0:filter off, 1:filter on with mode1, 2: filter on with mode2
//Bit 11: 0        reg_ldc_blk_intensity_th  // unsigned ,    RW, default = 0  

#define LDC_REG_DITHER_LUT_0_0 (0x40000+(0x3116<<2))//0x4c458
//Bit 31:16        reg_dither_lut1_0_0       // unsigned ,    RW, default = 0     
//Bit 15: 0        reg_dither_lut2_0_0       // unsigned ,    RW, default = 0     

#define LDC_REG_DITHER_LUT_0_1 (0x40000+(0x3117<<2))//0x4c45c
//Bit 31:16        reg_dither_lut1_0_1       // unsigned ,    RW, default = 0       
//Bit 15: 0        reg_dither_lut2_0_1       // unsigned ,    RW, default = 0       

#define LDC_REG_DITHER_LUT_0_2 (0x40000+(0x3118<<2))//0x4c460
//Bit 31:16        reg_dither_lut1_0_2       // unsigned ,    RW, default = 0       
//Bit 15: 0        reg_dither_lut2_0_2       // unsigned ,    RW, default = 0       

#define LDC_REG_DITHER_LUT_0_3 (0x40000+(0x3119<<2))//0x4c464
//Bit 31:16        reg_dither_lut1_0_3       // unsigned ,    RW, default = 0       
//Bit 15: 0        reg_dither_lut2_0_3       // unsigned ,    RW, default = 0       

#define LDC_REG_DITHER_LUT_0_4 (0x40000+(0x311a<<2))//0x4c468
//Bit 31:16        reg_dither_lut1_0_4       // unsigned ,    RW, default = 0     
//Bit 15: 0        reg_dither_lut2_0_4       // unsigned ,    RW, default = 0     

#define LDC_REG_DITHER_LUT_0_5 (0x40000+(0x311b<<2))//0x4c46c
//Bit 31:16        reg_dither_lut1_0_5       // unsigned ,    RW, default = 0     
//Bit 15: 0        reg_dither_lut2_0_5       // unsigned ,    RW, default = 0     

#define LDC_REG_DITHER_LUT_0_6 (0x40000+(0x311c<<2))//0x4c470
//Bit 31:16        reg_dither_lut1_0_6       // unsigned ,    RW, default = 0       
//Bit 15: 0        reg_dither_lut2_0_6       // unsigned ,    RW, default = 0       

#define LDC_REG_DITHER_LUT_0_7 (0x40000+(0x311d<<2))//0x4c474
//Bit 31:16        reg_dither_lut1_0_7       // unsigned ,    RW, default = 0       
//Bit 15: 0        reg_dither_lut2_0_7       // unsigned ,    RW, default = 0       

#define LDC_REG_DITHER_LUT_1_0 (0x40000+(0x311e<<2))//0x4c478
//Bit 31:16        reg_dither_lut1_1_0       // unsigned ,    RW, default = 33345 
//Bit 15: 0        reg_dither_lut2_1_0       // unsigned ,    RW, default = 33345 

#define LDC_REG_DITHER_LUT_1_1 (0x40000+(0x311f<<2))//0x4c47c
//Bit 31:16        reg_dither_lut1_1_1       // unsigned ,    RW, default = 16680    
//Bit 15: 0        reg_dither_lut2_1_1       // unsigned ,    RW, default = 16680    

#define LDC_REG_DITHER_LUT_1_2 (0x40000+(0x3120<<2))//0x4c480
//Bit 31:16        reg_dither_lut1_1_2       // unsigned ,    RW, default = 10260    
//Bit 15: 0        reg_dither_lut2_1_2       // unsigned ,    RW, default = 10260    

#define LDC_REG_DITHER_LUT_1_3 (0x40000+(0x3121<<2))//0x4c484
//Bit 31:16        reg_dither_lut1_1_3       // unsigned ,    RW, default = 5250   
//Bit 15: 0        reg_dither_lut2_1_3       // unsigned ,    RW, default = 5250   

#define LDC_REG_DITHER_LUT_1_4 (0x40000+(0x3122<<2))//0x4c488
//Bit 31:16        reg_dither_lut1_1_4       // unsigned ,    RW, default = 5250 
//Bit 15: 0        reg_dither_lut2_1_4       // unsigned ,    RW, default = 5250 

#define LDC_REG_DITHER_LUT_1_5 (0x40000+(0x3123<<2))//0x4c48c
//Bit 31:16        reg_dither_lut1_1_5       // unsigned ,    RW, default = 10305   
//Bit 15: 0        reg_dither_lut2_1_5       // unsigned ,    RW, default = 10305   

#define LDC_REG_DITHER_LUT_1_6 (0x40000+(0x3124<<2))//0x4c490
//Bit 31:16        reg_dither_lut1_1_6       // unsigned ,    RW, default = 16680     
//Bit 15: 0        reg_dither_lut2_1_6       // unsigned ,    RW, default = 16680     

#define LDC_REG_DITHER_LUT_1_7 (0x40000+(0x3125<<2))//0x4c494
//Bit 31:16        reg_dither_lut1_1_7       // unsigned ,    RW, default = 33300     
//Bit 15: 0        reg_dither_lut2_1_7       // unsigned ,    RW, default = 33300     

#define LDC_REG_DITHER_LUT_2_0 (0x40000+(0x3126<<2))//0x4c498
//Bit 31:16        reg_dither_lut1_2_0       // unsigned ,    RW, default = 38550 
//Bit 15: 0        reg_dither_lut2_2_0       // unsigned ,    RW, default = 38550 

#define LDC_REG_DITHER_LUT_2_1 (0x40000+(0x3127<<2))//0x4c49c
//Bit 31:16        reg_dither_lut1_2_1       // unsigned ,    RW, default = 26985  
//Bit 15: 0        reg_dither_lut2_2_1       // unsigned ,    RW, default = 26985  

#define LDC_REG_DITHER_LUT_2_2 (0x40000+(0x3128<<2))//0x4c4a0
//Bit 31:16        reg_dither_lut1_2_2       // unsigned ,    RW, default = 38550  
//Bit 15: 0        reg_dither_lut2_2_2       // unsigned ,    RW, default = 38550  

#define LDC_REG_DITHER_LUT_2_3 (0x40000+(0x3129<<2))//0x4c4a4
//Bit 31:16        reg_dither_lut1_2_3       // unsigned ,    RW, default = 26985  
//Bit 15: 0        reg_dither_lut2_2_3       // unsigned ,    RW, default = 26985  

#define LDC_REG_DITHER_LUT_2_4 (0x40000+(0x312a<<2))//0x4c4a8
//Bit 31:16        reg_dither_lut1_2_4       // unsigned ,    RW, default = 38505   
//Bit 15: 0        reg_dither_lut2_2_4       // unsigned ,    RW, default = 38505  

#define LDC_REG_DITHER_LUT_2_5 (0x40000+(0x312b<<2))//0x4c4ac
//Bit 31:16        reg_dither_lut1_2_5       // unsigned ,    RW, default = 50115  
//Bit 15: 0        reg_dither_lut2_2_5       // unsigned ,    RW, default = 50115  

#define LDC_REG_DITHER_LUT_2_6 (0x40000+(0x312c<<2))//0x4c4b0
//Bit 31:16        reg_dither_lut1_2_6       // unsigned ,    RW, default = 27030  
//Bit 15: 0        reg_dither_lut2_2_6       // unsigned ,    RW, default = 27030  

#define LDC_REG_DITHER_LUT_2_7 (0x40000+(0x312d<<2))//0x4c4b4
//Bit 31:16        reg_dither_lut1_2_7       // unsigned ,    RW, default = 15420  
//Bit 15: 0        reg_dither_lut2_2_7       // unsigned ,    RW, default = 15420  

#define LDC_REG_DITHER_LUT_3_0 (0x40000+(0x312e<<2))//0x4c4b8
//Bit 31:16        reg_dither_lut1_3_0       // unsigned ,    RW, default = 32190    
//Bit 15: 0        reg_dither_lut2_3_0       // unsigned ,    RW, default = 32190    

#define LDC_REG_DITHER_LUT_3_1 (0x40000+(0x312f<<2))//0x4c4bc
//Bit 31:16        reg_dither_lut1_3_1       // unsigned ,    RW, default = 48855    
//Bit 15: 0        reg_dither_lut2_3_1       // unsigned ,    RW, default = 48855    

#define LDC_REG_DITHER_LUT_3_2 (0x40000+(0x3130<<2))//0x4c4c0
//Bit 31:16        reg_dither_lut1_3_2       // unsigned ,    RW, default = 55275    
//Bit 15: 0        reg_dither_lut2_3_2       // unsigned ,    RW, default = 55275    

#define LDC_REG_DITHER_LUT_3_3 (0x40000+(0x3131<<2))//0x4c4c4
//Bit 31:16        reg_dither_lut1_3_3       // unsigned ,    RW, default = 60285    
//Bit 15: 0        reg_dither_lut2_3_3       // unsigned ,    RW, default = 60285    

#define LDC_REG_DITHER_LUT_3_4 (0x40000+(0x3132<<2))//0x4c4c8
//Bit 31:16        reg_dither_lut1_3_4       // unsigned ,    RW, default = 60285    
//Bit 15: 0        reg_dither_lut2_3_4       // unsigned ,    RW, default = 60285    

#define LDC_REG_DITHER_LUT_3_5 (0x40000+(0x3133<<2))//0x4c4cc
//Bit 31:16        reg_dither_lut1_3_5       // unsigned ,    RW, default = 55230     
//Bit 15: 0        reg_dither_lut2_3_5       // unsigned ,    RW, default = 55230    

#define LDC_REG_DITHER_LUT_3_6 (0x40000+(0x3134<<2))//0x4c4d0
//Bit 31:16        reg_dither_lut1_3_6       // unsigned ,    RW, default = 48855    
//Bit 15: 0        reg_dither_lut2_3_6       // unsigned ,    RW, default = 48855    

#define LDC_REG_DITHER_LUT_3_7 (0x40000+(0x3135<<2))//0x4c4d4
//Bit 31:16        reg_dither_lut1_3_7       // unsigned ,    RW, default = 32235     
//Bit 15: 0        reg_dither_lut2_3_7       // unsigned ,    RW, default = 32235     

#define LDC_REG_FORCE_VAL (0x40000+(0x3136<<2))//0x4c4d8
//Bit 31:16        reg_ldc_force_pix_intensity_val // unsigned ,    RW, default = 0  
//Bit 15           reg_ldc_force_pix_intensity_en // unsigned ,    RW, default = 0  
//Bit 14           reg_ldc_force_pix_gain_en // unsigned ,    RW, default = 0  
//Bit 13: 0        reg_ldc_force_pix_gain_val // unsigned ,    RW, default = 4096  

#define LDC_REG_DEBUG_PATH (0x40000+(0x3137<<2))//0x4c4dc
//Bit 31: 3        reserved                           
//Bit  2           reg_ldc_debug_grid_en     // unsigned ,    RW, default = 0  
//Bit  1           reg_ldc_debug_demo_en     // unsigned ,    RW, default = 0  
//Bit  0           reg_ldc_debug_demo_inverse // unsigned ,    RW, default = 0  

#define LDC_REG_DEMO_WINDOW_X (0x40000+(0x3138<<2))//0x4c4e0
//Bit 31:16        reg_ldc_debug_demo_wnd_0  // unsigned ,    RW, default = 480  
//Bit 15: 0        reg_ldc_debug_demo_wnd_2  // unsigned ,    RW, default = 1440  

#define LDC_REG_DEMO_WINDOW_Y (0x40000+(0x3139<<2))//0x4c4e4
//Bit 31:16        reg_ldc_debug_demo_wnd_1  // unsigned ,    RW, default = 270  
//Bit 15: 0        reg_ldc_debug_demo_wnd_3  // unsigned ,    RW, default = 810  

#define LDC_REG_INPUT_STAT_NUM (0x40000+(0x315c<<2))//0x4c570
//Bit 31:25        reserved                           
//Bit 24           reg_ambilight_stat_en     // unsigned ,    RW, default = 0  
//Bit 23:22        reg_ambilight_stat_mode   // unsigned ,    RW, default = 0  mode 0: 32 bit whole range, 1: 24 bit whole range, 2: 32 bit boundary, 3: 24 bit boundary
//Bit 21            reserved                           
//Bit 20:16        reg_ambilight_stat_y_num  // unsigned ,    RW, default = 20  
//Bit 15            reserved                           
//Bit 14: 9        reg_ambilight_stat_x_num  // unsigned ,    RW, default = 32  
//Bit  8: 5        reg_ambilight_stat_boundary_y // unsigned ,    RW, default = 4  
//Bit  4: 0        reg_ambilight_stat_boundary_x // unsigned ,    RW, default = 4  

#define LDC_REG_DEBUG_ENABLE (0x40000+(0x315d<<2))//0x4c574
//Bit 31:22        reserved                           
//Bit 21           reg_ldc_debug_input_max_rgb_en // unsigned ,    RW, default = 0  
//Bit 20           reg_ldc_debug_initial_bl_en // unsigned ,    RW, default = 0  
//Bit 19           reg_ldc_debug_sf_bl_en    // unsigned ,    RW, default = 0  
//Bit 18           reg_ldc_debug_bs_bl_en    // unsigned ,    RW, default = 0  
//Bit 17           reg_ldc_debug_tf_bl_en    // unsigned ,    RW, default = 0  
//Bit 16           reg_ldc_debug_block_intensity_en // unsigned ,    RW, default = 0  
//Bit 15           reg_ldc_debug_block_intensity_filter_en // unsigned ,    RW, default = 0  
//Bit 14           reg_ldc_debug_pixel_intensity_en // unsigned ,    RW, default = 0  
//Bit 13           reg_ldc_debug_pixel_gain_en // unsigned ,    RW, default = 0  
//Bit 12           reg_ldc_debug_pixel_cmp_en // unsigned ,    RW, default = 0  
//Bit 11           reg_ldc_debug_intensity_norm_en // unsigned ,    RW, default = 0  fix set 0 for RTL
//Bit 10: 8        reserved                           
//Bit  7: 6        reg_ldc_debug_force_seg_mode // unsigned ,    RW, default = 0  0: no force; 1: force one duty, center seg_x/y from profile bin, 2: force more region duty from reg_file.txt
//Bit  5           reg_ldc_debug_pre_mapping_bl_en // unsigned ,    RW, default = 0  
//Bit  4           reg_ldc_debug_glb_dimming_bl_en // unsigned ,    RW, default = 0  
//Bit  3           reg_ldc_debug_post_mapping__bl_en // unsigned ,    RW, default = 0  
//Bit  2           reg_ldc_debug_power_post_mapping_bl_en // unsigned ,    RW, default = 0  
//Bit  1           reg_ldc_debug_pixel_rgb_gain_en // unsigned ,    RW, default = 1  
//Bit  0           reg_ldc_debug_seg_rgb_gain_en // unsigned ,    RW, default = 1  

#define LDC_REG_FORCE_DUTY (0x40000+(0x315e<<2))//0x4c578
//Bit 31:24        reg_ldc_debug_force_seg_y_1 // unsigned ,    RW, default = 0  force duty seg_y_idx_end
//Bit 23:16        reg_ldc_debug_force_seg_y_0 // unsigned ,    RW, default = 0  force duty seg_y_idx_start
//Bit 15: 8        reg_ldc_debug_force_seg_x_1 // unsigned ,    RW, default = 0  force duty seg_x_idx_end
//Bit  7: 0        reg_ldc_debug_force_seg_x_0 // unsigned ,    RW, default = 0  force duty seg_x_idx_start

#define LDC_REG_POWER (0x40000+(0x315f<<2))//0x4c57c
//Bit 31           reg_ldc_power_sum_duty_mode     // unsigned ,    RW, default = 1  0: reg_ldc_power_sum_duty, 1: ro_ldc_power_sum_duty                   
//Bit 30           reg_ldc_power_post_dimming_curve_en // unsigned ,    RW, default = 0  0:off, 1:on
//Bit 29           reg_ldc_power_boost_en    // unsigned ,    RW, default = 0  0:off, 1:on
//Bit 28           reg_ldc_power_icur_iset_mode // unsigned ,    RW, default = 1  0:iset, 1:icur
//Bit 27           reg_ldc_power_iset_mode   // unsigned ,    RW, default = 0  0:local_iset, 1:glb_iset
//Bit 26:25        reg_ldc_power_duty_bw     // unsigned ,    RW, default = 2  0:6, 1:8, 2:10, 3:12
//Bit 24:23        reg_ldc_power_iset_bw     // unsigned ,    RW, default = 1  0:4, 1:6, 2:8, 3:10
//Bit 22:11        reg_ldc_power_icur_start  // unsigned ,    RW, default = 8  
//Bit 10: 1        reg_ldc_power_icur_step   // unsigned ,    RW, default = 8  
//Bit  0           reg_ldc_power_combo_mode  // unsigned ,    RW, default = 0  0: iset+duty, 1: duty+iset

#define LDC_REG_POWER_ISET (0x40000+(0x3160<<2))//0x4c580
//Bit 31:20        reg_ldc_power_base_icur_iset // unsigned ,    RW, default = 192  23(icur_iset_mode==0) or 192(icur_iset_mode==1)
//Bit 19:10        reg_ldc_power_max_iset    // unsigned ,    RW, default = 63  
//Bit  9: 0        reg_ldc_power_min_iset    // unsigned ,    RW, default = 23  

#define LDC_REG_POWER_POST_0 (0x40000+(0x3161<<2))//0x4c584
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_power_post_dimming_curve_0 // unsigned ,    RW, default = 0  
//Bit 11: 0        reg_ldc_power_post_dimming_curve_1 // unsigned ,    RW, default = 256  

#define LDC_REG_POWER_POST_1 (0x40000+(0x3162<<2))//0x4c588
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_power_post_dimming_curve_2 // unsigned ,    RW, default = 512  
//Bit 11: 0        reg_ldc_power_post_dimming_curve_3 // unsigned ,    RW, default = 768  

#define LDC_REG_POWER_POST_2 (0x40000+(0x3163<<2))//0x4c58c
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_power_post_dimming_curve_4 // unsigned ,    RW, default = 1024  
//Bit 11: 0        reg_ldc_power_post_dimming_curve_5 // unsigned ,    RW, default = 1280  

#define LDC_REG_POWER_POST_3 (0x40000+(0x3164<<2))//0x4c590
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_power_post_dimming_curve_6 // unsigned ,    RW, default = 1536  
//Bit 11: 0        reg_ldc_power_post_dimming_curve_7 // unsigned ,    RW, default = 1792  

#define LDC_REG_POWER_POST_4 (0x40000+(0x3165<<2))//0x4c594
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_power_post_dimming_curve_8 // unsigned ,    RW, default = 2048  
//Bit 11: 0        reg_ldc_power_post_dimming_curve_9 // unsigned ,    RW, default = 2304  

#define LDC_REG_POWER_POST_5 (0x40000+(0x3166<<2))//0x4c598
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_power_post_dimming_curve_10 // unsigned ,    RW, default = 2560  
//Bit 11: 0        reg_ldc_power_post_dimming_curve_11 // unsigned ,    RW, default = 2816  

#define LDC_REG_POWER_POST_6 (0x40000+(0x3167<<2))//0x4c59c
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_power_post_dimming_curve_12 // unsigned ,    RW, default = 3072  
//Bit 11: 0        reg_ldc_power_post_dimming_curve_13 // unsigned ,    RW, default = 3328  

#define LDC_REG_POWER_POST_7 (0x40000+(0x3168<<2))//0x4c5a0
//Bit 31:24        reserved                           
//Bit 23:12        reg_ldc_power_post_dimming_curve_14 // unsigned ,    RW, default = 3584  
//Bit 11: 0        reg_ldc_power_post_dimming_curve_15 // unsigned ,    RW, default = 3840  

#define LDC_REG_POWER_POST (0x40000+(0x3169<<2))//0x4c5a4
//Bit 31:12        reserved                           
//Bit 11: 0        reg_ldc_power_post_dimming_curve_16 // unsigned ,    RW, default = 4095  

#define LDC_REG_POWER_LIMIT (0x40000+(0x316a<<2))//0x4c5a8
//Bit 31:16        reg_ldc_power_constraint_percent // unsigned ,    RW, default = 3154  percent = 0.x~1.0, 3154/4096=0.77
//Bit 15: 0        reg_ldc_power_boost_ratio_max // unsigned ,    RW, default = 8356  u3.13, ratio = 0.x~7.99999, 16712/8192=2.04

#define LDC_REG_POWER_LIMIT_BW (0x40000+(0x316b<<2))//0x4c5ac
//Bit 31: 16       reserved                           
//Bit 15: 12       reg_ldc_power_boost_ratio_bw // unsigned ,    RW, default = 12  percent = 0.x~1.0, 3154/4096=0.77, 12bit means 1
//Bit 11: 0        reg_ldc_power_max_duty // unsigned ,    RW, default = 4095  u3.13, ratio = 0.x~7.99999, 16712/8192=2.04, 13bit means 1

#define LDC_REG_INPUT_FULL_IDX (0x40000+(0x316c<<2))//0x4c5b0
//Bit 31: 0        ro_ldc_input_ful_idx      // unsigned ,    RO, default = option->start_no  the index for input images

#define LDC_RO_GLB_HIST_SUM (0x40000+(0x316d<<2))//0x4c5b4
//Bit 31: 0        ro_ldc_glb_hist_data_sum  // unsigned ,    RO, default = 0  the global hist data sum

#define LDC_RO_GLB_HIST_CNT (0x40000+(0x316e<<2))//0x4c5b8
//Bit 31: 0        ro_ldc_glb_hist_cnt_sum   // unsigned ,    RO, default = 0  the global hist cnt sum

#define LDC_RO_GLB_APL (0x40000+(0x316f<<2))//0x4c5bc
//Bit 31:20        reserved                           
//Bit 19:10        ro_ldc_glb_apl            // unsigned ,    RO, default = 0  the global apl
//Bit  9: 0        ro_ldc_glb_apl_pre        // unsigned ,    RO, default = 0  the global apl of previous frame

#define LDC_RO_OUTPUT_IDX (0x40000+(0x3170<<2))//0x4c5c0
//Bit 31: 0        ro_ldc_ofrm_idx           // unsigned ,    RO, default = 0  the output frame index

#define LDC_RO_POWER_SUM_DUTY_0 (0x40000+(0x3171<<2))//0x4c5c4
//Bit 31:26        reserved                           
//Bit 25: 0        ro_ldc_power_sum_duty_0   // unsigned ,    RO, default = 0  the sum_duty of power_post_mapping_bl

#define LDC_RO_POWER_SUM_DUTY_1 (0x40000+(0x3172<<2))//0x4c5c8
//Bit 31:26        reserved                           
//Bit 25: 0        ro_ldc_power_sum_duty_1   // unsigned ,    RO, default = 0  the sum_duty of power_post_mapping_bl

#define LDC_RO_POWER_SUM_DUTY_2 (0x40000+(0x3173<<2))//0x4c5cc
//Bit 31:26        reserved                           
//Bit 25: 0        ro_ldc_power_sum_duty_2   // unsigned ,    RO, default = 0  the sum_duty of power_post_mapping_bl

#define LDC_RO_POWER_SUM_DUTY_3 (0x40000+(0x3174<<2))//0x4c5d0
//Bit 31:26        reserved                           
//Bit 25: 0        ro_ldc_power_sum_duty_3   // unsigned ,    RO, default = 0  the sum_duty of power_post_mapping_bl

#define LDC_REG_POWER_SUM_DUTY_0 (0x40000+(0x3175<<2))//0x4c5d4
//Bit 31:26        reserved                           
//Bit 25: 0        reg_ldc_power_sum_duty_0   // unsigned ,    RW, default = 0  the sum_duty of power_post_mapping_bl

#define LDC_REG_POWER_SUM_DUTY_1 (0x40000+(0x3176<<2))//0x4c5d8
//Bit 31:26        reserved                           
//Bit 25: 0        reg_ldc_power_sum_duty_1   // unsigned ,    RW, default = 0  the sum_duty of power_post_mapping_bl

#define LDC_REG_POWER_SUM_DUTY_2 (0x40000+(0x3177<<2))//0x4c5dc
//Bit 31:26        reserved                           
//Bit 25: 0        reg_ldc_power_sum_duty_2   // unsigned ,    RW, default = 0  the sum_duty of power_post_mapping_bl

#define LDC_REG_POWER_SUM_DUTY_3 (0x40000+(0x3178<<2))//0x4c5e0
//Bit 31:26        reserved                           
//Bit 25: 0        reg_ldc_power_sum_duty_3   // unsigned ,    RW, default = 0  the sum_duty of power_post_mapping_bl

#define LDC_REG_BLK_INTENSITY_MNT (0x40000+(0x3179<<2))//0x4c5e4
//Bit 31:18        reserved                           
//Bit 17           reg_ldc_quadrant_mode      // unsigned ,    RW, default = 0  0: all 4 quadrants, 1:4/2/2/1 quadrant symmetrically
//Bit 16           reg_ldc_profile_bin_mode   // unsigned ,    RW, default = 0  0:TV mode, 1:MNT mode
//Bit 15: 0        reg_ldc_blk_intensity_gain // unsigned ,    RW, default = 4096  12bits means 1

#define LDC_REG_DIS_X_RATIO_MNT (0x40000+(0x317a<<2))//0x4c5e8
//Bit 31:20        reserved                           
//Bit 19: 0        reg_ldc_dis_x_ratio        // unsigned ,    RW, default = 119837  2^(16+8)/(xsize*cmp_mask_x/seg_col/2)

#define LDC_REG_DIS_Y_RATIO_MNT (0x40000+(0x317b<<2))//0x4c5ec
//Bit 31:20        reserved                           
//Bit 19: 0        reg_ldc_dis_y_ratio        // unsigned ,    RW, default = 142029  2^(16+8)/(ysize*cmp_mask_y/seg_row/2)

#define LDC_REG_POWER_ALPHA (0x40000+(0x31d0<<2))//0x4c740
//Bit 31:26         reserved                           
//Bit 25: 24        reg_ldc_power_alpha_mode         // unsigned ,    RW, default = 0  
//Bit 23: 16        reg_ldc_power_alpha_value        // unsigned ,    RW, default = 255  
//Bit 15: 0         reg_ldc_power_duty_div_ratio     // unsigned ,    RW, default = 16  

#define LDC_REG_POWER_ALPHA_SUM_DUTY_0 (0x40000+(0x31d1<<2))//0x4c744
//Bit 31: 24        reg_ldc_power_alpha_vs_sum_duty_lut_0        // unsigned ,    RW, default = 0    
//Bit 23: 16        reg_ldc_power_alpha_vs_sum_duty_lut_1        // unsigned ,    RW, default = 16   
//Bit 15: 8         reg_ldc_power_alpha_vs_sum_duty_lut_2        // unsigned ,    RW, default = 32   
//Bit  7: 0         reg_ldc_power_alpha_vs_sum_duty_lut_3        // unsigned ,    RW, default = 48   

#define LDC_REG_POWER_ALPHA_SUM_DUTY_1 (0x40000+(0x31d2<<2))//0x4c748
//Bit 31: 24        reg_ldc_power_alpha_vs_sum_duty_lut_4        // unsigned ,    RW, default = 64   
//Bit 23: 16        reg_ldc_power_alpha_vs_sum_duty_lut_5        // unsigned ,    RW, default = 80   
//Bit 15: 8         reg_ldc_power_alpha_vs_sum_duty_lut_6        // unsigned ,    RW, default = 96   
//Bit  7: 0         reg_ldc_power_alpha_vs_sum_duty_lut_7        // unsigned ,    RW, default = 112  

#define LDC_REG_POWER_ALPHA_SUM_DUTY_2 (0x40000+(0x31d3<<2))//0x4c74c
//Bit 31: 24        reg_ldc_power_alpha_vs_sum_duty_lut_8        // unsigned ,    RW, default = 128  
//Bit 23: 16        reg_ldc_power_alpha_vs_sum_duty_lut_9        // unsigned ,    RW, default = 144  
//Bit 15: 8         reg_ldc_power_alpha_vs_sum_duty_lut_10       // unsigned ,    RW, default = 160  
//Bit  7: 0         reg_ldc_power_alpha_vs_sum_duty_lut_11       // unsigned ,    RW, default = 176  

#define LDC_REG_POWER_ALPHA_SUM_DUTY_3 (0x40000+(0x31d4<<2))//0x4c750
//Bit 31: 24        reg_ldc_power_alpha_vs_sum_duty_lut_12       // unsigned ,    RW, default = 192  
//Bit 23: 16        reg_ldc_power_alpha_vs_sum_duty_lut_13       // unsigned ,    RW, default = 208  
//Bit 15: 8         reg_ldc_power_alpha_vs_sum_duty_lut_14       // unsigned ,    RW, default = 224  
//Bit  7: 0         reg_ldc_power_alpha_vs_sum_duty_lut_15       // unsigned ,    RW, default = 240  

#define LDC_REG_POWER_ALPHA_SUM_DUTY (0x40000+(0x31d5<<2))//0x4c754
//Bit 31:28         reserved                           
//Bit 27: 20        reg_ldc_power_alpha_vs_sum_duty_lut_16       // unsigned ,    RW, default = 255  
//Bit 19: 12        reg_ldc_power_alpha_vs_seg_duty_lut_16       // unsigned ,    RW, default = 255  
//Bit 11: 0         reg_ldc_power_iset_ofst                      // unsigned ,    RW, default = 0  

#define LDC_REG_POWER_ALPHA_SEG_DUTY_0 (0x40000+(0x31d6<<2))//0x4c758
//Bit 31: 24        reg_ldc_power_alpha_vs_seg_duty_lut_0        // unsigned ,    RW, default = 0     
//Bit 23: 16        reg_ldc_power_alpha_vs_seg_duty_lut_1        // unsigned ,    RW, default = 16   
//Bit 15: 8         reg_ldc_power_alpha_vs_seg_duty_lut_2        // unsigned ,    RW, default = 32   
//Bit  7: 0         reg_ldc_power_alpha_vs_seg_duty_lut_3        // unsigned ,    RW, default = 48   

#define LDC_REG_POWER_ALPHA_SEG_DUTY_1 (0x40000+(0x31d7<<2))//0x4c75c
//Bit 31: 24        reg_ldc_power_alpha_vs_seg_duty_lut_4        // unsigned ,    RW, default = 64   
//Bit 23: 16        reg_ldc_power_alpha_vs_seg_duty_lut_5        // unsigned ,    RW, default = 80   
//Bit 15: 8         reg_ldc_power_alpha_vs_seg_duty_lut_6        // unsigned ,    RW, default = 96   
//Bit  7: 0         reg_ldc_power_alpha_vs_seg_duty_lut_7        // unsigned ,    RW, default = 112  

#define LDC_REG_POWER_ALPHA_SEG_DUTY_2 (0x40000+(0x31d8<<2))//0x4c760
//Bit 31: 24        reg_ldc_power_alpha_vs_seg_duty_lut_8        // unsigned ,    RW, default = 128  
//Bit 23: 16        reg_ldc_power_alpha_vs_seg_duty_lut_9        // unsigned ,    RW, default = 144  
//Bit 15: 8         reg_ldc_power_alpha_vs_seg_duty_lut_10       // unsigned ,    RW, default = 160  
//Bit  7: 0         reg_ldc_power_alpha_vs_seg_duty_lut_11       // unsigned ,    RW, default = 176  

#define LDC_REG_POWER_ALPHA_SEG_DUTY_3 (0x40000+(0x31d9<<2))//0x4c764
//Bit 31: 24        reg_ldc_power_alpha_vs_seg_duty_lut_12       // unsigned ,    RW, default = 192  
//Bit 23: 16        reg_ldc_power_alpha_vs_seg_duty_lut_13       // unsigned ,    RW, default = 208  
//Bit 15: 8         reg_ldc_power_alpha_vs_seg_duty_lut_14       // unsigned ,    RW, default = 224  
//Bit  7: 0         reg_ldc_power_alpha_vs_seg_duty_lut_15       // unsigned ,    RW, default = 240  


#define LDC_REG_HW_BIN_ADDR_BASE (0x40000+(0x3180<<2))//0x4c600
//Bit 31:0       reg_hw_bin_addr_base           // unsigned ,    RW, default = 32'h00100000           

#define LDC_REG_HW_HIST_ADDR_BASE (0x40000+(0x3181<<2))//0x4c604
//Bit 31:0       reg_hw_hist_addr_base          // unsigned ,    RW, default = 32'h0015ac00           

#define LDC_REG_HW_DUTY_INFO_ADDR_BASE (0x40000+(0x3182<<2))//0x4c608
//Bit 31:0       reg_hw_duty_info_addr_base     // unsigned ,    RW, default = 32'h0015c400           

#define LDC_REG_HW_RGB_GAIN_ADDR_BASE (0x40000+(0x3183<<2))//0x4c60c
//Bit 31:0       reg_hw_rgb_gain_addr_base      // unsigned ,    RW, default = 32'h0015d400           

#define LDC_REG_HW_POWER_ADDR_BASE (0x40000+(0x3184<<2))//0x4c610
//Bit 31:0       reg_hw_power_addr_base         // unsigned ,    RW, default = 32'h0015d800           

#define LDC_REG_HW_WMIF_WPOWER (0x40000+(0x3185<<2))//0x4c614
//Bit 31:14       reserved                           
//Bit 13:12       reg_hw_wpower_sw_rst            // unsigned ,    RW, default = 0 , 
//Bit 11:10       reg_hw_wpower_gclk_ctrl         // unsigned ,    RW, default = 0 ,  
//Bit  9          reg_hw_wpower_wrmif_clr         // unsigned ,    RW, default = 0 , 
//Bit  8: 6       reg_hw_wpower_cmd_intr_len      // unsigned ,    RW, default = 0 , 0-12,1-16,2-24,3-32,4-40,5-48,6-56,7-64,
//Bit  5: 4       reg_hw_wpower_cmd_req_size      // unsigned ,    RW, default = 3 , 00:ram16, 01:ram32, 10:ram48, 11:ram8 
//Bit  3: 2       reg_hw_wpower_burst_len         // unsigned ,    RW, default = 2 , 00-single 01-bst2 10-bst4 11-bst8 
//Bit  1          reg_hw_wpower_swap_64bit        // unsigned ,    RW, default = 0 , 
//Bit  0          reg_hw_wpower_little_endian     // unsigned ,    RW, default = 1 , 

#define LDC_RO_HW_WMIF_WPOWER (0x40000+(0x3186<<2))//0x4c618
//Bit 31:17       reserved                           
//Bit 16          ro_hw_wpower_wrmif_frm_int       // unsigned ,    RO, default = 0 , 
//Bit 15: 0       ro_hw_wpower_status              // unsigned ,    RO, default = 0 , 

#define LDC_REG_HW_WMIF_HIST (0x40000+(0x3187<<2))//0x4c61c
//Bit 31:14       reserved                           
//Bit 13:12       reg_hw_whist_sw_rst             // unsigned ,    RW, default = 0 , 
//Bit 11:10       reg_hw_whist_gclk_ctrl          // unsigned ,    RW, default = 0 ,  
//Bit  9          reg_hw_whist_wrmif_clr          // unsigned ,    RW, default = 0 , 
//Bit  8: 6       reg_hw_whist_cmd_intr_len       // unsigned ,    RW, default = 0 , 0-12,1-16,2-24,3-32,4-40,5-48,6-56,7-64,
//Bit  5: 4       reg_hw_whist_cmd_req_size       // unsigned ,    RW, default = 3 , 00:ram16, 01:ram32, 10:ram48, 11:ram8 
//Bit  3: 2       reg_hw_whist_burst_len          // unsigned ,    RW, default = 3 , 00-single 01-bst2 10-bst4 11-bst8 
//Bit  1          reg_hw_whist_swap_64bit         // unsigned ,    RW, default = 0 , 
//Bit  0          reg_hw_whist_little_endian      // unsigned ,    RW, default = 1 , 

#define LDC_RO_HW_WMIF_HIST (0x40000+(0x3188<<2))//0x4c620
//Bit 31:17       reserved                           
//Bit 16          ro_hw_whist_wrmif_frm_int       // unsigned ,    RO, default = 0 , 
//Bit 15: 0       ro_hw_whist_status              // unsigned ,    RO, default = 0 , 

#define LDC_REG_HW_WMIF_TMP_WDUTY (0x40000+(0x3189<<2))//0x4c624
//Bit 31:14       reserved                           
//Bit 13:12       reg_hw_tmp_wduty_sw_rst          // unsigned ,    RW, default = 0 , 
//Bit 11:10       reg_hw_tmp_wduty_gclk_ctrl       // unsigned ,    RW, default = 0 ,  
//Bit  9          reg_hw_tmp_wduty_wrmif_clr       // unsigned ,    RW, default = 0 , 
//Bit  8: 6       reg_hw_tmp_wduty_cmd_intr_len    // unsigned ,    RW, default = 1 , 0-12,1-16,2-24,3-32,4-40,5-48,6-56,7-64,
//Bit  5: 4       reg_hw_tmp_wduty_cmd_req_size    // unsigned ,    RW, default = 0 , 00:ram16, 01:ram32, 10:ram48, 11:ram8 
//Bit  3: 2       reg_hw_tmp_wduty_burst_len       // unsigned ,    RW, default = 2 , 00-single 01-bst2 10-bst4 11-bst8 
//Bit  1          reg_hw_tmp_wduty_swap_64bit      // unsigned ,    RW, default = 0 , 
//Bit  0          reg_hw_tmp_wduty_little_endian   // unsigned ,    RW, default = 1 , 

#define LDC_RO_HW_WMIF_TMP_WDUTY (0x40000+(0x318a<<2))//0x4c628
//Bit 31:17       reserved                           
//Bit 16          ro_hw_tmp_wrmif_frm_int          // unsigned ,    RO, default = 0 , 
//Bit 15: 0       ro_hw_tmp_wduty_status           // unsigned ,    RO, default = 0 , 

#define LDC_REG_HW_RMIF_TMP_RDUTY (0x40000+(0x318b<<2))//0x4c62c
//Bit 31:14       reserved                           
//Bit 13          reg_hw_tmp_rduty_rdmif_clr       // unsigned  ,  RW, default = 0 ,  
//Bit 12:10       reg_hw_tmp_rduty_cmd_intr_len    // unsigned  ,  RW, default = 1 , 0-12,1-16,2-24,3-32,4-40,5-48,6-56,7-64,
//Bit  9: 8       reg_hw_tmp_rduty_cmd_req_size    // unsigned  ,  RW, default = 0 , 00:ram16, 01:ram32, 10:ram48, 11:ram8 
//Bit  7: 6       reg_hw_tmp_rduty_burst_len       // unsigned  ,  RW, default = 1 , 00-single 01-bst4 10-bst8 11-bst16
//Bit  5          reg_hw_tmp_rduty_swap_64bit      // unsigned  ,  RW, default = 0 , 
//Bit  4          reg_hw_tmp_rduty_little_endian   // unsigned  ,  RW, default = 1 ,  
//Bit  3: 2       reg_hw_tmp_rduty_sw_rst          // unsigned  ,  RW, default = 0 ,  
//Bit  1: 0       reg_hw_tmp_rduty_gclk_ctrl       // unsigned  ,  RW, default = 0 ,

#define LDC_RO_HW_RMIF_TMP_RDUTY (0x40000+(0x318c<<2))//0x4c630
//Bit 31:17       reserved                           
//Bit 16          ro_hw_tmp_rduty_rdmif_frm_int    // unsigned ,    RO, default = 0 , 
//Bit 15: 0       ro_hw_tmp_rduty_status           // unsigned ,    RO, default = 0 , 

#define LDC_REG_HW_RMIF_DUTY (0x40000+(0x318d<<2))//0x4c634
//Bit 31:14       reserved                           
//Bit 13          reg_hw_duty_rdmif_clr       // unsigned  ,  RW, default = 0 ,  
//Bit 12:10       reg_hw_duty_cmd_intr_len    // unsigned  ,  RW, default = 1 , 0-12,1-16,2-24,3-32,4-40,5-48,6-56,7-64,
//Bit  9: 8       reg_hw_duty_cmd_req_size    // unsigned  ,  RW, default = 0 , 00:ram16, 01:ram32, 10:ram48, 11:ram8 
//Bit  7: 6       reg_hw_duty_burst_len       // unsigned  ,  RW, default = 1 , 00-single 01-bst4 10-bst8 11-bst16
//Bit  5          reg_hw_duty_swap_64bit      // unsigned  ,  RW, default = 0 , 
//Bit  4          reg_hw_duty_little_endian   // unsigned  ,  RW, default = 1 ,  
//Bit  3: 2       reg_hw_duty_sw_rst          // unsigned  ,  RW, default = 0 ,  
//Bit  1: 0       reg_hw_duty_gclk_ctrl       // unsigned  ,  RW, default = 0 ,

#define LDC_RO_HW_RMIF_DUTY (0x40000+(0x318e<<2))//0x4c638
//Bit 31:17       reserved                           
//Bit 16          ro_hw_duty_rdmif_frm_int    // unsigned ,    RO, default = 0 , 
//Bit 15: 0       ro_hw_duty_status           // unsigned ,    RO, default = 0 , 

#define LDC_REG_HW_RMIF_FILE (0x40000+(0x318f<<2))//0x4c63c
//Bit 31:14       reserved                           
//Bit 13          reg_hw_file_rdmif_clr       // unsigned ,   RW, default = 0 ,  
//Bit 12:10       reg_hw_file_cmd_intr_len    // unsigned ,   RW, default = 1 , 0-12,1-16,2-24,3-32,4-40,5-48,6-56,7-64,
//Bit  9: 8       reg_hw_file_cmd_req_size    // unsigned ,   RW, default = 0 , 00:ram16, 01:ram32, 10:ram48, 11:ram8 
//Bit  7: 6       reg_hw_file_burst_len       // unsigned ,   RW, default = 3 , 00-single 01-bst4 10-bst8 11-bst16
//Bit  5          reg_hw_file_swap_64bit      // unsigned ,   RW, default = 0 , 
//Bit  4          reg_hw_file_little_endian   // unsigned ,   RW, default = 1 ,  
//Bit  3: 2       reg_hw_file_sw_rst          // unsigned ,   RW, default = 0 ,  
//Bit  1: 0       reg_hw_file_gclk_ctrl       // unsigned ,   RW, default = 0 ,

#define LDC_RO_HW_RMIF_FILE (0x40000+(0x3190<<2))//0x4c640
//Bit 31:17       reserved                           
//Bit 16          ro_hw_file_rdmif_frm_int    // unsigned ,    RO, default = 0 , 
//Bit 15: 0       ro_hw_file_status           // unsigned ,    RO, default = 0 , 

#define LDC_REG_HW_RMIF_GAIN (0x40000+(0x3191<<2))//0x4c644
//Bit 31:14       reserved                           
//Bit 13          reg_hw_gain_rdmif_clr       // unsigned   , RW, default = 0 ,  
//Bit 12:10       reg_hw_gain_cmd_intr_len    // unsigned   , RW, default = 1 , 0-12,1-16,2-24,3-32,4-40,5-48,6-56,7-64,
//Bit  9: 8       reg_hw_gain_cmd_req_size    // unsigned   , RW, default = 3 , 00:ram16, 01:ram32, 10:ram48, 11:ram8 
//Bit  7: 6       reg_hw_gain_burst_len       // unsigned   , RW, default = 1 , 00-single 01-bst4 10-bst8 11-bst16
//Bit  5          reg_hw_gain_swap_64bit      // unsigned   , RW, default = 0 , 
//Bit  4          reg_hw_gain_little_endian   // unsigned   , RW, default = 1 ,  
//Bit  3: 2       reg_hw_gain_sw_rst          // unsigned   , RW, default = 0 ,  
//Bit  1: 0       reg_hw_gain_gclk_ctrl       // unsigned   , RW, default = 0 ,

#define LDC_RO_HW_RMIF_GAIN (0x40000+(0x3192<<2))//0x4c648
//Bit 31:17       reserved                           
//Bit 16          ro_hw_gain_rdmif_frm_int    // unsigned ,    RO, default = 0 , 
//Bit 15: 0       ro_hw_gain_status           // unsigned ,    RO, default = 0 , 

#define LDC_REG_HW_RMIF_POWER (0x40000+(0x3193<<2))//0x4c64c
//Bit 31:14       reserved                           
//Bit 13          reg_hw_rpower_rdmif_clr       // unsigned  ,  RW, default = 0 ,  
//Bit 12:10       reg_hw_rpower_cmd_intr_len    // unsigned  ,  RW, default = 1 , 0-12,1-16,2-24,3-32,4-40,5-48,6-56,7-64,
//Bit  9: 8       reg_hw_rpower_cmd_req_size    // unsigned  ,  RW, default = 3 , 00:ram16, 01:ram32, 10:ram48, 11:ram8 
//Bit  7: 6       reg_hw_rpower_burst_len       // unsigned  ,  RW, default = 1 , 00-single 01-bst4 10-bst8 11-bst16
//Bit  5          reg_hw_rpower_swap_64bit      // unsigned  ,  RW, default = 0 , 
//Bit  4          reg_hw_rpower_little_endian   // unsigned  ,  RW, default = 1 ,  
//Bit  3: 2       reg_hw_rpower_sw_rst          // unsigned  ,  RW, default = 0 ,  
//Bit  1: 0       reg_hw_rpower_gclk_ctrl       // unsigned  ,  RW, default = 0 ,

#define LDC_RO_HW_RMIF_POWER (0x40000+(0x3194<<2))//0x4c650
//Bit 31:17       reserved                           
//Bit 16          ro_hw_rpower_rdmif_frm_int    // unsigned ,    RO, default = 0 , 
//Bit 15: 0       ro_hw_rpower_status           // unsigned ,    RO, default = 0 , 

#define LDC_REG_HW_RD_ARB_CTRL0 (0x40000+(0x3195<<2))//0x4c654
//Bit 31:23       reserved   
//Bit 22:21       reg_hw_rd_arb_gclk_ctrl          // unsigned ,RW, default = 0    
//Bit 20          reg_hw_rd_arb_arb_mode           // unsigned ,RW, default = 1    
//Bit 19:15       reg_hw_rd_arb_sel                // unsigned ,RW, default = 0    
//Bit 14:10       reg_hw_rd_arb_dc_req_en          // unsigned ,RW, default = 5'h1f    
//Bit 9 : 0       reg_hw_rd_arb_ugt_basic          // unsigned ,RW, default = 10'h155    

#define LDC_REG_HW_RD_ARB_CTRL1 (0x40000+(0x3196<<2))//0x4c658
//Bit 31:30       reserved   
//Bit 29: 0       reg_hw_rd_arb_dc_weigh_sxn       // unsigned ,RW, default = 30'hf3cf3cf    

#define LDC_REG_HW_RD_ARB_CTRL2 (0x40000+(0x3197<<2))//0x4c65c
//Bit 31:18       reserved   
//Bit 17:16       reg_hw_wr_arb_gclk_ctrl          // unsigned ,RW, default = 0  
//Bit 15: 0       reg_hw_rd_arb_req_limt_num       // unsigned ,RW, default = 16'h3f3f   

#define LDC_REG_HW_WR_ARB_CTRL0 (0x40000+(0x3198<<2))//0x4c660
//Bit 31          reserved  
//Bit 30          reg_hw_wr_arb_arb_mode          // unsigned ,RW, default = 1  
//Bit 29:27       reg_hw_wr_arb_sel               // unsigned ,RW, default = 0  
//Bit 26:24       reg_hw_wr_arb_dc_req_en         // unsigned ,RW, default = 3'h7  
//Bit 23: 6       reg_hw_wr_arb_dc_weigh_sxn      // unsigned ,RW, default = 18'hf3cf  
//Bit 5 : 0       reg_hw_wr_arb_ugt_basic         // unsigned ,RW, default = 6'h15  

#define LDC_RO_HW_ARB_CTRL0 (0x40000+(0x3199<<2))//0x4c664
//Bit 31: 2       reserved 
//Bit 1           ro_hw_rd_arb_arb_busy           // unsigned ,RO, default = 0    
//Bit 0           ro_hw_wr_arb_arb_busy           // unsigned ,RO, default = 0  

#define LDC_REG_HW_GATE_CLK_CTRL (0x40000+(0x319a<<2))//0x4c668
//Bit 31:0        reg_hw_clk_gate                 // unsigned ,    RW, default = 32'h80000000  ,bit[31]:reg clk gate

#define LDC_REG_HW_FRM_CTRL (0x40000+(0x319b<<2))//0x4c66c
//Bit 31:16       reserved  
//Bit 15          reg_hw_bypass_pre_flt_en       //unsigned ,   RW, default = 1 , 0:bypass 1:work
//Bit 14          reg_hw_bypass_post_flt_en      //unsigned ,   RW, default = 1 , 0:bypass 1:work
//Bit 13          reg_hw_cmp_cal_before_vs_en    //unsigned ,   RW, default = 0 , 0: cal time point vs  1: cal time point before vs
//Bit 12          reg_hw_hs_polarity_sel         //unsigned ,   RW, default = 0 , 1: high level available  0: low level available
//Bit 11          reg_hw_frm_rst_is_pose         //unsigned ,   RW, default = 1 , 1: frm_rst is pose, 0:frm_rst is nege
//Bit 10          reg_hw_soft_cal_power_mode     //unsigned ,   RW, default = 0 , 0: hw cal power, 1:soft cal power
//Bit 9           reg_hw_bypass_pix_intsty       //unsigned ,   RW, default = 0 , 0: work, 1:bypass
//Bit 8           reg_hw_bypass_blk_intsty       //unsigned ,   RW, default = 0 , 0: work, 1:bypass
//Bit 7           reg_hw_seg_hist_done_sel       //unsigned ,   RW, default = 0 , 0: seg hist mif done(early),  1:temporal flt din done (later)
//Bit 6           reg_hw_soft_cal_duty_mode      //unsigned ,   RW, default = 0 , 0: hw cal duty, 1:soft cal duty
//Bit 5 :4        reg_hw_amblt_ppc_mode          //unsigned ,   RW, default = 0 , 0: input=ppc, 1: input 1ppc in PPC=2,4, 2:input 2ppc in PPC=4
//Bit 3           reg_hw_vs_polarity_sel         //unsigned ,   RW, default = 1 , 1: high level available  0: low level available
//Bit 2           reg_hw_hist_vs_polarity_sel    //unsigned ,   RW, default = 1 , 1: high level available  0: low level available
//Bit 1           reg_hw_hist_source_sel         //unsigned ,   RW, default = 0 , 1: select main video to hist 0:select hist video to hist 
//Bit 0           reg_hw_is_first_frm            //unsigned ,   RW, default = 0 , 1: first frame

#define LDC_RO_MEMORY_IDX (0x40000+(0x319c<<2))//0x4c670
//Bit 31:10      reserved                           
//Bit 9 : 8      ro_hw_bl_output_fid_power       //unsigned ,    RO, default = 3
//Bit 7 : 2      ro_hw_dbg_blk_inty_cal_line     //unsigned ,    RO, default = 0 ,blk_inty cal line num when fst de arrive
//Bit 1          ro_hw_power_buf_id              //unsigned ,    RO, default = 1 ,cur power duty write index mem
//Bit 0          ro_hw_seg_hist_buf_id           //unsigned ,    RO, default = 0 ,cur seg hist write index mem

#define LDC_REG_CURSOR_CTRL (0x40000+(0x319d<<2))//0x4c674
//Bit 31         reserved                           
//Bit 30:28      reg_hw_cursor_color             //unsigned ,    RW, default = 0    
//Bit 27:15      reg_hw_cursor_pos_y             //unsigned ,    RW, default = 0    
//Bit 14: 1      reg_hw_cursor_pos_x             //unsigned ,    RW, default = 0    
//Bit 0          reg_hw_cursor_en                //unsigned ,    RW, default = 0    

#define LDC_RO_CURSOR_DOT_YUV (0x40000+(0x319e<<2))//0x4c678
//Bit 31:30      reserved                           
//Bit 29:20      ro_hw_cursor_dot_v              //unsigned ,    RO, default = 0 ,  cursur dot read back v                  
//Bit 19:10      ro_hw_cursor_dot_u              //unsigned ,    RO, default = 0 ,  cursur dot read back u                  
//Bit 9 :0       ro_hw_cursor_dot_y              //unsigned ,    RO, default = 0 ,  cursur dot read back y                  

#define LDC_REG_INTR_CTRL (0x40000+(0x319f<<2))//0x4c67c
//Bit 31:30      reg_hw_bcon_intr_sel             //unsigned ,    RW, default = 0    
//Bit 29:16      reg_hw_bcon_intr_num             //unsigned ,    RW, default = 1    
//Bit 15         reg_hw_bcon_duty_dly_mode        //unsigned ,    RW, default = 0 , 0: 0-delay, 1: 1-delay                 
//Bit 14         reg_hw_bcon_duty_or_power_sel    //unsigned ,    RW, default = 0 , 1: duty   , 0: power                     
//Bit 13: 0      reg_hw_spi_intr_num              //unsigned ,    RW, default = 1    

#define LDC_CMP_GAIN0_ADDR_PORT (0x40000+(0x31a0<<2))//0x4c680
//Bit 31: 8      reserved                           
//Bit 7 : 0      reg_hw_gain_lut_addr0            //unsigned ,    RW, default = 0    

#define LDC_CMP_GAIN0_DATA_PORT (0x40000+(0x31a1<<2))//0x4c684
//Bit 31: 24     reserved                           
//Bit 23: 0      reg_hw_gain_lut_data0            //unsigned ,    RW, default = 0    

#define LDC_CMP_GAIN1_ADDR_PORT (0x40000+(0x31a2<<2))//0x4c688
//Bit 31: 8      reserved                           
//Bit 7 : 0      reg_hw_gain_lut_addr1            //unsigned ,    RW, default = 0    

#define LDC_CMP_GAIN1_DATA_PORT (0x40000+(0x31a3<<2))//0x4c68c
//Bit 31: 24     reserved                           
//Bit 23: 0      reg_hw_gain_lut_data1            //unsigned ,    RW, default = 0    

#define LDC_PROFILE_ADDR_PORT (0x40000+(0x31a4<<2))//0x4c690
//Bit 31:11      reserved                           
//Bit 10: 0      reg_hw_profile_lut_addr         //unsigned ,    RW, default = 0    

#define LDC_PROFILE_DATA_PORT (0x40000+(0x31a5<<2))//0x4c694
//Bit 31: 0      reg_hw_profile_lut_data         //unsigned ,    RW, default = 0 

#define LDC_RGB_GAIN_ADDR_PORT (0x40000+(0x31a6<<2))//0x4c698
//Bit 31:11      reserved                           
//Bit 10: 0      reg_hw_rgb_gain_lut_addr            //unsigned ,    RW, default = 0    

#define LDC_RGB_GAIN_DATA_PORT (0x40000+(0x31a7<<2))//0x4c69c
//Bit 31: 30     reserved                           
//Bit 29: 0      reg_hw_rgb_gain_lut_data            //unsigned ,    RW, default = 0    

#define LDC_RGB_HW_CAL_CTRL0 (0x40000+(0x31a8<<2))//0x4c6a0
//Bit 31:29     reserved                           
//Bit 28:16     reg_hw_pre_cal_line_num            //unsigned ,    RW, default = 40    
//Bit 15:14     reserved                           
//Bit 13: 0     reg_hw_wr_duty_done_delay_num      //unsigned ,    RW, default = 150    

#define LDC_RGB_HW_CAL_CTRL1 (0x40000+(0x31a9<<2))//0x4c6a4
//Bit 31:24     reserved                           
//Bit 23        reg_hw_abn_rst_det_en      //unsigned ,    RW, default = 0  ,1:open 0:close
//Bit 22:21     reg_hw_ldc_en              //unsigned ,    RW, default = 3  , 
//Bit 20:11     reg_hw_power_cal_line_num  //unsigned ,    RW, default = 20 , 
//Bit 10: 9     reg_hw_frm_rst_blk_mode    //unsigned ,    RW, default = 0 , 0:auto, 1:cfg set reg   
//Bit 8 : 3     reg_hw_rdmif_duty_num      //unsigned ,    RW, default = 5 , pre rd duty line num                   
//Bit 2         reserved                           
//Bit 1 : 0     reg_hw_cmp_cal_point       //unsigned ,    RW, default = 0 , 0: frm_blk 1:frm_duty 2:frm_rst 

#define LDC_RGB_HW_SOFT_RST_CTRL (0x40000+(0x31aa<<2))//0x4c6a8
//Bit 31: 1     reserved                           
//Bit 0         reg_hw_ldc_soft_rst        //unsigned ,    RW, default = 0 

#define LDC_RGB_HW_FORCE_WINDOW_X (0x40000+(0x31ab<<2))//0x4c6ac
//Bit 31:30     reserved                           
//Bit 29:16     reg_hw_ldc_force_wnd_rit       //unsigned ,    RW, default = 959 : 0~frm_hsize-1
//Bit 15:14     reserved                           
//Bit 13: 0     reg_hw_ldc_force_wnd_lft       //unsigned ,    RW, default = 0

#define LDC_RGB_HW_FORCE_WINDOW_Y (0x40000+(0x31ac<<2))//0x4c6b0
//Bit 31:30     reserved                           
//Bit 29:16     reg_hw_ldc_force_wnd_bot      //unsigned ,    RW, default = 1079 : 0~frm_vsize-1
//Bit 15:14     reserved                           
//Bit 13: 0     reg_hw_ldc_force_wnd_top       //unsigned ,    RW, default = 0

#define LDC_RGB_HW_FORCE_WINDOW_DATA (0x40000+(0x31ad<<2))//0x4c6b4
//Bit 31        reserved                           
//Bit 30        reg_hw_ldc_force_data_en  //unsigned ,    RW, default = 0
//Bit 29:20     reg_hw_ldc_force_b        //unsigned ,    RW, default = 256
//Bit 19:10     reg_hw_ldc_force_g        //unsigned ,    RW, default = 256
//Bit  9: 0     reg_hw_ldc_force_r        //unsigned ,    RW, default = 256

#define LDC_RGB_HW_DEMO_PNT_WINDOW_X (0x40000+(0x31ae<<2))//0x4c6b8
//Bit 31:30     reserved                           
//Bit 29:16     reg_hw_ldc_debug_pnt_wnd_rit       //unsigned ,    RW, default = 959 : 0~frm_hsize-1
//Bit 15:14     reserved                           
//Bit 13: 0     reg_hw_ldc_debug_pnt_wnd_lft       //unsigned ,    RW, default = 0

#define LDC_RGB_HW_DEMO_PNT_WINDOW_Y (0x40000+(0x31af<<2))//0x4c6bc
//Bit 31:30     reserved                           
//Bit 29:16     reg_hw_ldc_debug_pnt_wnd_bot       //unsigned ,    RW, default = 1079 : 0~frm_vsize-1
//Bit 15:14     reserved                           
//Bit 13: 0     reg_hw_ldc_debug_pnt_wnd_top       //unsigned ,    RW, default = 0

#define LDC_RGB_HW_DBG_PNT_CTRL (0x40000+(0x31b0<<2))//0x4c6c0
//Bit 31: 9     reserved
//Bit  8: 5     reg_hw_sft_num                     //unsigned ,    RW, default = 4 
//Bit  4        reg_hw_ldc_debug_pnt_inverse       //unsigned ,    RW, default = 0 
//Bit  3        reg_hw_ldc_debug_pnt_en            //unsigned ,    RW, default = 0 
//Bit  2        reg_hw_dbg_sel_rgb_gain            //unsigned ,    RW, default = 0 
//Bit  1        reg_hw_dbg_sel_cmp_gain            //unsigned ,    RW, default = 0 
//Bit  0        reg_hw_dbg_sel_pix_inty            //unsigned ,    RW, default = 0 


#define LCD_HIST_REG_HW_PANEL_SIZE (0x40000+(0x31c0<<2))//0x4c700
//Bit 31:30      reserved
//Bit 29:16      reg_hw_lcd_panel_width        // unsigned ,    RW, default = 1920  the panel width
//Bit 15:14      reserved
//Bit 13: 0      reg_hw_lcd_panel_height       // unsigned ,    RW, default = 1080  the panel height

#define LCD_HIST_REG_HW_CTRL (0x40000+(0x31c1<<2))//0x4c704
//Bit 31:19      reserved
//Bit 18:17      reg_hw_lcd_hist_point_sel     // unsigned ,    RW, default = 0  hist sel:ldc in, 1:ldc out 2:gamma in 3:gamma out
//Bit 16:14      reg_hw_lcd_clk_gate           // unsigned ,    RW, default = 1  clk gate
//Bit 13         reg_hw_lcd_vs_polarity_sel    // unsigned ,    RW, default = 0  vs polarity sel
//Bit 12:11      reg_hw_lcd_drop_point_sel     // unsigned ,    RW, default = 0  drop_point sel odd or even
//Bit 10: 8      reg_hw_lcd_hist_data_sel      // unsigned ,    RW, default = 0  hist data sel MAX(RGB) R G B Y  
//Bit 7 : 0      reg_hw_lcd_y_gain             // unsigned ,    RW, default = 64 the gain for input pixel value,

#define LCD_HIST_RO_HW_CLB_APL (0x40000+(0x31c2<<2))//0x4c708
//Bit 31:12      reserved
//Bit 11:0       ro_hw_lcd_glb_apl             // unsigned ,    RO, default = 0  

#define LCD_HIST_RO_HW_CLB_SUM (0x40000+(0x31c3<<2))//0x4c70c
//Bit 31:23      reserved
//Bit 22:0       ro_hw_lcd_glb_hist_cnt        // unsigned ,    RO, default = 0  

#define LCD_HIST_RO_HW_CLB_CNT (0x40000+(0x31c4<<2))//0x4c710
//Bit 31:29      reserved
//Bit 28:0       ro_hw_lcd_glb_hist_sum        // unsigned ,    RO, default = 0  

#define LCD_HIST_REG_HW_ADDR_PORT (0x40000+(0x31c5<<2))//0x4c714
//Bit 31: 6      reserved                           
//Bit 5 : 0      reg_hw_lcd_glb_hist_addr      //unsigned ,    RW, default = 0    

#define LDC_HIST_REG_HW_DATA_PORT (0x40000+(0x31c6<<2))//0x4c718
//Bit 31: 22     reserved                           
//Bit 21: 0      reg_hw_lcd_glb_hist_data      //unsigned ,    RW, default = 0    


#define VPU_TOP_OSD_MENU (0x40000+(0x3a00<<2))//0x4e800
//Bit 31:4      reserved
//Bit 3         reg_nemu                    //unsigned, RW, default = 0
//Bit 3         reg_top                     //unsigned, RW, default = 0
//Bit 2         reg_bottom                  //unsigned, RW, default = 0
//Bit 1         reg_left                    //unsigned, RW, default = 0
//Bit 0         reg_right                   //unsigned, RW, default = 0

#define CM_ENH_CTL_REG              0x208
#define SAT_BYYB_NODE_REG0          0x200
#define SAT_BYYB_NODE_REG1          0x201
#define SAT_BYYB_NODE_REG2          0x202
#define XVYCC_YSCP_REG              0x21c
#define XVYCC_USCP_REG              0x21d
#define XVYCC_VSCP_REG              0x21e
#define FRM_SIZE_REG                0x205
#define LUMA_ADJ0_REG               0x21f
#endif

