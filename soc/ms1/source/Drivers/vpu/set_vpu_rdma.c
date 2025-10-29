#include "set_vpu_rdma.h"
#include <string.h>

VPU_RDMA_BUF_t g_vpu_rdma_wbuf; // for reg wr
VPU_RDMA_BUF_t g_vpu_rdma_rbuf; // for reg rd

// chan_idx    : max 16 channels
// chan_buf_idx: max 16 buf each channel
void get_vpu_rdma_buf_info(uint64_t *ddr_st_addr, uint64_t *ddr_end_addr, uint32_t *ddr_buf_len, VPU_RDMA_BUF_t *vpu_rdma_buf, uint32_t chan_idx, uint32_t chan_buf_idx) {
  *ddr_st_addr = vpu_rdma_buf->buf[chan_buf_idx][chan_idx].rdma_buf_st_addr;
  *ddr_end_addr = vpu_rdma_buf->buf[chan_buf_idx][chan_idx].rdma_buf_end_addr;
  *ddr_buf_len = vpu_rdma_buf->buf[chan_buf_idx][chan_idx].rdma_buf_length;
  printf("get_vpu_rdma_buf_info: chan_idx %0d buf_idx %0d len %d\n", chan_idx, chan_buf_idx, vpu_rdma_buf->buf[chan_idx][chan_buf_idx].rdma_buf_length);
}

void vpu_rdma_buf_init(VPU_RDMA_BUF_t *vpu_rdma_buf, uint32_t chan_idx, uint32_t chan_buf_idx, uint64_t chan_st_addr, uint32_t is_ddr_wr) {
  vpu_rdma_buf->cur_chan_idx = chan_idx;
  vpu_rdma_buf->cur_buf_idx = chan_buf_idx;
  vpu_rdma_buf->is_ddr_wr = is_ddr_wr;

  printf("vpu_rdma_buf_init(): chan%0d buf%0d\n", chan_idx, chan_buf_idx);
  // vpu_rdma_buf->buf[chan_idx][chan_buf_idx].rdma_buf_step = auto_buf_step;
  if (chan_buf_idx) {
    // vpu_rdma_buf->buf[chan_idx][chan_buf_idx].rdma_buf_st_addr += vpu_rdma_buf->buf[chan_idx][chan_buf_idx-1].rdma_buf_step;
    vpu_rdma_buf->buf[chan_idx][chan_buf_idx].rdma_buf_end_addr = vpu_rdma_buf->buf[chan_idx][chan_buf_idx].rdma_buf_st_addr;
  } else {
    vpu_rdma_buf->buf[chan_idx][chan_buf_idx].rdma_buf_st_addr = chan_st_addr;
    vpu_rdma_buf->buf[chan_idx][chan_buf_idx].rdma_buf_end_addr = chan_st_addr;
  }
  vpu_rdma_buf->buf[chan_idx][chan_buf_idx].rdma_buf_length = 0;
}

void vpu_rdma_wr_ddr(VPU_RDMA_BUF_t *vpu_rdma_buf, uint64_t reg_addr, uint64_t reg_data) {
  uint64_t wdata;
  uint32_t chan_idx = vpu_rdma_buf->cur_chan_idx;
  uint32_t buf_idx = vpu_rdma_buf->cur_buf_idx;

  reg_addr = reg_addr >= VPU_APB_BASE ? reg_addr - VPU_APB_BASE : reg_addr;
  reg_addr = reg_addr >> 2;

  wdata = 0ULL << 48 |     // 48--63
          reg_data << 32 | // 32--47
          reg_addr << 0;   // 0 --31

  Wr_ddr(vpu_rdma_buf->buf[chan_idx][buf_idx].rdma_buf_end_addr, wdata);

  printf("vpu_rdma_wr_ddr(): adress=%llx,data=%llx\n", vpu_rdma_buf->buf[chan_idx][buf_idx].rdma_buf_end_addr, wdata);

  vpu_rdma_buf->buf[chan_idx][buf_idx].rdma_buf_end_addr = vpu_rdma_buf->buf[chan_idx][buf_idx].rdma_buf_end_addr + 8;
  vpu_rdma_buf->buf[chan_idx][buf_idx].rdma_buf_length = vpu_rdma_buf->buf[chan_idx][buf_idx].rdma_buf_length + 1;
}

void vout_rdma_wr_ddr(VPU_RDMA_BUF_t *vpu_rdma_buf, uint64_t reg_addr, uint64_t reg_data) {
  uint64_t wdata;
  uint32_t chan_idx = vpu_rdma_buf->cur_chan_idx;
  uint32_t buf_idx = vpu_rdma_buf->cur_buf_idx;

  reg_addr = reg_addr >= VPU_APB_BASE ? reg_addr - VPU_APB_BASE : reg_addr;
  reg_addr = reg_addr >> 2;

  wdata = 4ULL << 48 |     // 48--63
          reg_data << 32 | // 32--47
          reg_addr << 0;   // 0 --31

  Wr_ddr(vpu_rdma_buf->buf[chan_idx][buf_idx].rdma_buf_end_addr, wdata);

  printf("vpu_rdma_wr_ddr(): adress=%llx,data=%llx\n", vpu_rdma_buf->buf[chan_idx][buf_idx].rdma_buf_end_addr, wdata);

  vpu_rdma_buf->buf[chan_idx][buf_idx].rdma_buf_end_addr = vpu_rdma_buf->buf[chan_idx][buf_idx].rdma_buf_end_addr + 8;
  vpu_rdma_buf->buf[chan_idx][buf_idx].rdma_buf_length = vpu_rdma_buf->buf[chan_idx][buf_idx].rdma_buf_length + 1;
}

void vpu_rdma_wr_ddr1(uint64_t reg_addr, uint64_t reg_data, uint32_t is_reg_write) {
  uint64_t wdata;
  uint32_t wchan_idx = g_vpu_rdma_wbuf.cur_chan_idx;
  uint32_t rchan_idx = g_vpu_rdma_rbuf.cur_chan_idx;
  uint32_t wbuf_idx = g_vpu_rdma_wbuf.cur_buf_idx;
  uint32_t rbuf_idx = g_vpu_rdma_rbuf.cur_buf_idx;
  uint32_t reg_mod_ofst;

  // printf("reg_addr 0x%x data 0x%x\n", reg_addr, reg_data);
  if ((reg_addr & 0xFFF00000) != VPU_APB_BASE) {
    return;
  }
  reg_addr = reg_addr >= VPU_APB_BASE ? reg_addr - VPU_APB_BASE : reg_addr;
  reg_addr = reg_addr >> 2;

  reg_mod_ofst = (reg_addr >> 8) & 0xFF;
  printf("%llx reg_mod_ofst %x\n", reg_addr, reg_mod_ofst);
  if (!((reg_mod_ofst >= 0x60 && reg_mod_ofst <= 0x78) ||                         // osd_proc
        reg_mod_ofst == 0x1d || reg_mod_ofst == 0x25 || reg_mod_ofst == 0x26 ||   // vpp post
        reg_mod_ofst == 0x2c || reg_mod_ofst == 0x3e || reg_mod_ofst == 0x31 ||   // vpp_post
        (reg_mod_ofst >= 0x50 && reg_mod_ofst <= 0x53) || reg_mod_ofst == 0x5a || // vd_proc
        (reg_mod_ofst >= 0x28 && reg_mod_ofst <= 0x2b) || reg_mod_ofst == 0x2e || // vd_proc
        reg_mod_ofst == 0x32 || reg_mod_ofst == 0x38 || reg_mod_ofst == 0x3f ||   // vd_proc
        reg_mod_ofst == 0x40 || reg_mod_ofst == 0x48 || reg_mod_ofst == 0x49 ||   // vd_proc
        reg_mod_ofst == 0x4c || reg_mod_ofst == 0x4d                              // vd_proc
        ))
    return;

  wdata = 0ULL << 48 |                    // 48--63
          (reg_addr & 0xFFFFFFFF) << 32 | // 32--47
          (reg_data & 0xFFFFFFFF) << 0;   // 0--31
  if (is_reg_write) {
    printf("wchan/buf_idx %0d %0d  addrh %x addrl %x wdatah %x wdatal %x\n", wchan_idx, wbuf_idx, (uint32_t)(g_vpu_rdma_wbuf.buf[wchan_idx][wbuf_idx].rdma_buf_end_addr >> 32), (uint32_t)(g_vpu_rdma_wbuf.buf[wchan_idx][wbuf_idx].rdma_buf_end_addr & 0xFFFFFFFF), (uint32_t)(wdata >> 32), (uint32_t)(wdata & 0xFFFFFFFF));
    Wr_ddr(g_vpu_rdma_wbuf.buf[wchan_idx][wbuf_idx].rdma_buf_end_addr, wdata);

    g_vpu_rdma_wbuf.buf[wchan_idx][wbuf_idx].rdma_buf_end_addr = g_vpu_rdma_wbuf.buf[wchan_idx][wbuf_idx].rdma_buf_end_addr + 8;
    g_vpu_rdma_wbuf.buf[wchan_idx][wbuf_idx].rdma_buf_length = g_vpu_rdma_wbuf.buf[wchan_idx][wbuf_idx].rdma_buf_length + 1;
  } else {
    Wr_ddr(g_vpu_rdma_rbuf.buf[rchan_idx][rbuf_idx].rdma_buf_end_addr, wdata);

    g_vpu_rdma_rbuf.buf[rchan_idx][rbuf_idx].rdma_buf_end_addr = g_vpu_rdma_rbuf.buf[rchan_idx][rbuf_idx].rdma_buf_end_addr + 8;
    g_vpu_rdma_rbuf.buf[rchan_idx][rbuf_idx].rdma_buf_length = g_vpu_rdma_rbuf.buf[rchan_idx][rbuf_idx].rdma_buf_length + 1;
  }
}

///////////////////////////////////////////////
//            RDMA_CONFIG
///////////////////////////////////////////////
void init_vpu_rdma_params(MNT_RDMA_t *rdma_test_vec) {
  memset((void *)rdma_test_vec, 0, sizeof(MNT_RDMA_t));
}

void vpu_rdma_init(MNT_RDMA_t *rdma_test_vec, VPU_RDMA_BUF_t *vpu_rdma_buf, uint8_t cbus_auto_write,
                   uint32_t rdma_chn_idx, uint32_t int_src_idx) {
  uint64_t *ddr_st_addr, *ddr_end_addr;
  uint32_t *ddr_buf_len;

  uint32_t chan_idx = vpu_rdma_buf->cur_chan_idx;
  uint32_t buf_idx = vpu_rdma_buf->cur_buf_idx;
  printf("vpu_rdma_init : rdma_test_vec->ahb_start_addr[rdma_chn_idx] = %llx ...\n", rdma_test_vec->ahb_start_addr[rdma_chn_idx]);

  // if(rdma_chn_idx==0)
  //     rdma_test_vec->rdma_start_auto                   = 0;//manual load
  // else
  //     rdma_test_vec->rdma_start_auto                   = 1;//auto   load

  // rdma_test_vec->rdma_start_auto                            = buf_auto_incr_mode;

  rdma_test_vec->rdma_auto_src_sel[rdma_chn_idx] = int_src_idx;

  printf("vpu_rdma_init : rdma_chn_idx     = %d\n", rdma_chn_idx);
  printf("vpu_rdma_init : rdma_auto_src_sel= %d\n", rdma_test_vec->rdma_auto_src_sel[rdma_chn_idx]);

  rdma_test_vec->ahb_start_addr[rdma_chn_idx] = vpu_rdma_buf->buf[rdma_chn_idx][0].rdma_buf_st_addr;
  rdma_test_vec->ahb_end_addr[rdma_chn_idx] = vpu_rdma_buf->buf[rdma_chn_idx][0].rdma_buf_end_addr;

  // rdma_test_vec->cbus_auto_write[rdma_chn_idx]              = 1;
  rdma_test_vec->cbus_write[rdma_chn_idx] = cbus_auto_write;

  // for(int buf = 0; buf < VPU_RDMA_CHAN_BUF_NUM; buf++){
  //     rdma_test_vec->buf_auto_incr_lenth[rdma_chn_idx][buf] = vpu_rdma_buf->buf[rdma_chn_idx][buf].rdma_buf_length * 8;
  //     printf("vpu_rdma_init(): chan%0d buf%0d %d\n", rdma_chn_idx, buf, rdma_test_vec->buf_auto_incr_lenth[rdma_chn_idx][buf]);
  // }

  // rdma_test_vec->buf_auto_incr_mode[rdma_chn_idx]           = buf_auto_incr_mode;
  // rdma_test_vec->buf_auto_incr_step[rdma_chn_idx]           = buf_auto_incr_step;
  // rdma_test_vec->buf_auto_incr_idx[rdma_chn_idx]            = buf_idx;
}

void rdma_config_v1(MNT_RDMA_t *rdma_test_vec, uint32_t rdma_chn_idx) {
  printf("rdma_chn_idx: %d\n", rdma_chn_idx);

  // init rdma
  Wr(RDMA_CTRL,
     0 << 24 |     // [31:24] ctrl_clr_rdma_done_int
         0 << 23 | // [   23] ctrl_clr_err
         0 << 8 |  // [22: 8] Rsrv
         0 << 7 |  // [    7] ctrl_axi_wr_urgent
         0 << 6 |  // [    6] ctrl_axi_rd_urgent
         1 << 4 |  // [ 5: 4] ctrl_ahb_wr_burst_size. 0=1; 1=4; 2=8; 3=16.//TODO//rdma_test_vec->ahb_wr_burst_size
         1 << 2 |  // [ 3: 2] ctrl_ahb_rd_burst_size. 0=1; 1=4; 2=8; 3=16.//rdma_test_vec->ahb_rd_burst_size
         0 << 1 |  // [    1] ctrl_sw_reset.
         0 << 0    // [    0] ctrl_free_clk_enable.
  );

  Wr(RDMA_ACCESS_MAN, rdma_test_vec->cbus_buf_sel_man << 3 |                 // [    3] ctrl_buf_sel. 0=DDR; 1=SRAM.
                          rdma_test_vec->cbus_write[rdma_chn_idx] << 2 |     // [    2] ctrl_cbus_write_man. 1=Register write; 0=Register read.
                          rdma_test_vec->cbus_addr_incr[rdma_chn_idx] << 1 | // [    1] ctrl_cbus_addr_incr_man. 1=Incremental register access; 0=Non-incremental.
                          0 << 0                                             // [    0] ctrl_start_man pulse.
  );
  Wr(RDMA_ACCESS_AUTO, rdma_test_vec->cbus_write[1] << 5 |
                           rdma_test_vec->cbus_write[2] << 6 |
                           rdma_test_vec->cbus_write[3] << 7 |
                           rdma_test_vec->cbus_addr_incr[1] << 1 |
                           rdma_test_vec->cbus_addr_incr[2] << 2 |
                           rdma_test_vec->cbus_addr_incr[3] << 3);
  Wr(RDMA_ACCESS_AUTO2, rdma_test_vec->cbus_write[4] << 4 |
                            rdma_test_vec->cbus_write[5] << 5 |
                            rdma_test_vec->cbus_write[6] << 6 |
                            rdma_test_vec->cbus_write[7] << 7 |
                            rdma_test_vec->cbus_addr_incr[4] << 0 |
                            rdma_test_vec->cbus_addr_incr[5] << 1 |
                            rdma_test_vec->cbus_addr_incr[6] << 2 |
                            rdma_test_vec->cbus_addr_incr[7] << 3);
  // Wr(RDMA_ACCESS_AUTO3, rdma_test_vec->cbus_write[8]     << 8  |
  //                           rdma_test_vec->cbus_write[9]     << 9  |
  //                           rdma_test_vec->cbus_write[10]    << 10 |
  //                           rdma_test_vec->cbus_write[11]    << 11 |
  //                           rdma_test_vec->cbus_write[12]    << 12 |
  //                           rdma_test_vec->cbus_write[13]    << 13 |
  //                           rdma_test_vec->cbus_write[14]    << 14 |
  //                           rdma_test_vec->cbus_write[15]    << 15 |
  //                           rdma_test_vec->cbus_addr_incr[8] << 0  |
  //                           rdma_test_vec->cbus_addr_incr[9] << 1  |
  //                           rdma_test_vec->cbus_addr_incr[10]<< 2  |
  //                           rdma_test_vec->cbus_addr_incr[11]<< 3  |
  //                           rdma_test_vec->cbus_addr_incr[12]<< 4  |
  //                           rdma_test_vec->cbus_addr_incr[13]<< 5  |
  //                           rdma_test_vec->cbus_addr_incr[14]<< 6  |
  //                           rdma_test_vec->cbus_addr_incr[15]<< 7
  //);

  // Wr_reg_bits(RDMA_DONE_FLAG_SEL, 1, rdma_chn_idx, 1);

  // auto buf or not?
  // Wr(RDMA_BUF_CFG_EN_MAN + rdma_chn_idx * 4, rdma_test_vec->buf_auto_incr_mode[rdma_chn_idx]);
  // if(rdma_test_vec->buf_auto_incr_mode[rdma_chn_idx]){
  //     uint32_t reg_sft_bit;
  //     uint32_t reg_buf_len;
  //     uint32_t max_data_len  = 0;

  //    for(int i = 0; i < 16; i ++) {
  //        max_data_len = rdma_test_vec->buf_auto_incr_lenth[rdma_chn_idx][i] > max_data_len ? rdma_test_vec->buf_auto_incr_lenth[rdma_chn_idx][i] : max_data_len;
  //    }
  //    reg_sft_bit = cal_reg_sft_bit(max_data_len);

  //    //Wr_reg_bits(RDMA_BUF_CTRL_MAN     + rdma_chn_idx * 4 ,       reg_sft_bit, 8, 4);
  //    //for(int i = 0; i < 16; i ++) {
  //    //    reg_buf_len  = (rdma_test_vec->buf_auto_incr_lenth[rdma_chn_idx][i] >> reg_sft_bit);
  //    //    //printf("chan%d buf %d reg_buf_len %d\n", rdma_chn_idx, i, reg_buf_len);
  //    //    Wr_reg_bits(RDMA_BUF_LENGTH_MAN_0 + rdma_chn_idx*16+(i/4)*4, reg_buf_len, (i%4)*8, 8);
  //    //}
  //}

  printf("rdma_config_v1: rdma_auto_src_sel[%0d] = %0x\n", rdma_chn_idx, rdma_test_vec->rdma_auto_src_sel[rdma_chn_idx]);

  if (rdma_chn_idx == 0) {
    printf("RDMA_ACCESS_MAN ..2.\n");

    Wr(RDMA_AHB_START_ADDR_MAN, (rdma_test_vec->ahb_start_addr[rdma_chn_idx]) & 0xffffffff);
    Wr(RDMA_AHB_START_ADDR_MAN_MSB, (rdma_test_vec->ahb_start_addr[rdma_chn_idx] >> 32) & 0xffffffff);

    Wr(RDMA_AHB_END_ADDR_MAN, (rdma_test_vec->ahb_end_addr[rdma_chn_idx]) & 0xffffffff);
    Wr(RDMA_AHB_END_ADDR_MAN_MSB, (rdma_test_vec->ahb_end_addr[rdma_chn_idx] >> 32) & 0xffffffff);

    // Wr_reg_bits(RDMA_BUF_CTRL_MAN  + 0, rdma_test_vec->buf_auto_incr_step[rdma_chn_idx],16,16);

  } else {
    printf("RDMA_ACCESS_AUTO ..2.\n");
    Wr(RDMA_AUTO_SRC1_SEL + ((rdma_chn_idx - 1) * 5 * 4), rdma_test_vec->rdma_auto_src_sel[rdma_chn_idx]);

    Wr(RDMA_AHB_START_ADDR_1 + ((rdma_chn_idx - 1) * 5 * 4), rdma_test_vec->ahb_start_addr[rdma_chn_idx]);

    Wr(RDMA_AHB_END_ADDR_1 + ((rdma_chn_idx - 1) * 5 * 4), rdma_test_vec->ahb_end_addr[rdma_chn_idx]);
    // Wr_reg_bits(RDMA_BUF_CTRL_MAN  + rdma_chn_idx * 4 ,rdma_test_vec->buf_auto_incr_step[rdma_chn_idx],16,16);
  }
}

uint32_t cal_reg_sft_bit(uint32_t buf_bytes_len) {
  uint32_t reg_sft_bit;

  if (buf_bytes_len < 256) {
    reg_sft_bit = 0;
  } else if (buf_bytes_len < 256 * 2) {
    reg_sft_bit = 1;
  } else if (buf_bytes_len < 256 * 4) {
    reg_sft_bit = 2;
  } else if (buf_bytes_len < 256 * 8) {
    reg_sft_bit = 3;
  } else if (buf_bytes_len < 256 * 16) {
    reg_sft_bit = 4;
  } else if (buf_bytes_len < 256 * 32) {
    reg_sft_bit = 5;
  } else if (buf_bytes_len < 256 * 64) {
    reg_sft_bit = 6;
  } else if (buf_bytes_len < 256 * 128) {
    reg_sft_bit = 7;
  }
  return reg_sft_bit;
}

// void cfg_rdma_auto_src_sel(uint32_t chan_idx, MNT_RDMA_t *rdma_test_vec)
//{
//     Wr(RDMA_AUTO_SRC1_SEL + (chan_idx - 1) * 5 * 4, rdma_test_vec->rdma_auto_src_sel[chan_idx]);
// }

void start_rdma_chan0() {
  printf("start_rdma_chan0()!\n");
  Wr(RDMA_ACCESS_MAN, Rd(RDMA_ACCESS_MAN) | 1);
}

// void cfg_rdma_auto_buf_len(MNT_RDMA_t *rdma_test_vec,  uint32_t chan_idx, uint32_t buf_idx)
//{
//     uint32_t  reg_sft_bit;
//     uint32_t  reg_buf_len;
//
//     reg_sft_bit = cal_reg_sft_bit(rdma_test_vec->buf_auto_incr_lenth[chan_idx][buf_idx]);
//     reg_buf_len = rdma_test_vec->buf_auto_incr_lenth[chan_idx][buf_idx] >> reg_sft_bit;
//
//     //Wr_reg_bits(RDMA_BUF_CTRL_MAN     + chan_idx * 4 ,       reg_sft_bit, 8, 4);
//     //Wr_reg_bits(RDMA_BUF_LENGTH_MAN_0 + chan_idx * 16 + (buf_idx/4) * 4, reg_buf_len, (buf_idx % 4) * 8, 8);
//     printf("chan%d buf %d reg_sft_bit %d\n", chan_idx, buf_idx, reg_sft_bit);
//     printf("chan%d buf %d reg_buf_len %d\n", chan_idx, buf_idx, reg_buf_len);
//
// }

// uint32_t get_rdma_proc_buf_idx(uint32_t rdma_chan_idxe)
//{
//     uint32_t  reg_rdma_buf_ro      = RDMA_BUF_RO_MAN + 4 * rdma_chan_idx;
//     return (Rd(reg_rdma_buf_ro) & 0xF);
// }

uint32_t get_rdma_proc_chan_idx() {
  uint32_t rdma_chan_idx;
  rdma_chan_idx = (Rd(RDMA_STATUS) >> 20) & 0xF;
  return rdma_chan_idx;
}

int check_rdma_status() {
  uint32_t data32;
  uint32_t flag = 0;

  // Check rmda status
  data32 = Rd(RDMA_STATUS1);
  // printf("data32 = %x\n",data32);
  if (data32 != 0x010) {
    printf("Error: Wrong RDMA_STATUS!\n");
  }
  /*
      data32  = Rd(RDMA_STATUS1);
      if (data32 != 0x00000010) {
          printf("Error: Wrong RDMA_STATUS1!\n");//diff with s5
      }
  */
  data32 = Rd(RDMA_STATUS2);
  if ((data32 & 0xffff00ff) != (32 << 0)) {
    printf("Error: Wrong RDMA_STATUS2!\n");
  }
  data32 = Rd(RDMA_STATUS3);
  if (data32 != 0) {
    printf("Error: Wrong RDMA_STATUS3!\n");
  }

  // Clear level signal rdma_int
  Wr_reg_bits(RDMA_CTRL, 0x1, 16, 16); // reg update in t3x
  /*
     // Check rmda_done flag is cleared
     data32  = Rd(RDMA_STATUS1);
     if (data32 != 0) {
         flag = 1;
         printf("Error: RDMA Done flag is not cleared!\n");
     }
 */
  return flag;
}

int set_rdma_int_mode(uint32_t is_pulse) {
  Wr_reg_bits(RDMA_CTRL, is_pulse, 0, 1);
  return 0;
}
