#ifndef _VPU_RDMA_H_
#define _VPU_RDMA_H_

#include "vpu_include.h"

#define  VPU_APB_BASE            0xfe400000
#define  VPU_RDMA_CHAN_NUM       8 
#define  VPU_RDMA_CHAN_BUF_NUM   2  

////0,1,2..15
//#define  VPU_RDMA_CHAN_IDX(chn) (chn) 

//0x00100000---x00400000
//#define  VPU_RDMA_CHAN_ST_BADDR(chn)  (0x00100000+0x40000*chn) 
//#define  VPU_RDMA_CHAN_BUF_STEP(chn)  (2048) 

typedef struct VPU_RDMA_BUF_UNIT_TYPE{
    uint64_t rdma_buf_st_addr ;
    uint64_t rdma_buf_end_addr;
    uint32_t rdma_buf_length  ;
} VPU_RDMA_BUF_UNIT_t;

typedef struct VPU_RDMA_BUF_TYPE{
    VPU_RDMA_BUF_UNIT_t   buf[VPU_RDMA_CHAN_NUM][VPU_RDMA_CHAN_BUF_NUM];
    uint32_t              cur_chan_idx;
    uint32_t              cur_buf_idx ;
    uint32_t              is_ddr_wr   ;
} VPU_RDMA_BUF_t;


typedef struct VPU_RDMA_TYPE {
  //uint64_t    ahb_start_addr; 
  //uint64_t    ahb_end_addr;
    uint8_t     ahb_rd_burst_size;
    uint8_t     ahb_wr_burst_size;
  //uint8_t     cbus_write;
  //uint8_t     cbus_addr_incr;
  //uint8_t     rdma_chn_idx;
    uint8_t     cbus_buf_sel_man;  // 0: DDR 1: SRAM
  //uint8_t     rdma_start_auto;   // 0: start mannualy, 1: start by interrupt trigger
    uint32_t    rdma_auto_src_sel[VPU_RDMA_CHAN_NUM];  // channel to select interrupt src num,bit0:interrupt[0], bit1: interrupt[1],....
                                        // rdma_auto_src_sel[0] not used, its src0 for man
    uint64_t    ahb_start_addr[VPU_RDMA_CHAN_NUM];     // [0] is man addr, [1..15] for auto addr 
    uint64_t    ahb_end_addr[VPU_RDMA_CHAN_NUM];
    uint8_t     cbus_write[VPU_RDMA_CHAN_NUM];
    uint8_t     cbus_addr_incr[VPU_RDMA_CHAN_NUM];

    //uint8_t     buf_auto_incr_mode[VPU_RDMA_CHAN_NUM]; 
    //uint32_t    buf_auto_incr_step[VPU_RDMA_CHAN_NUM];
    //uint32_t    buf_auto_incr_idx[VPU_RDMA_CHAN_NUM];         
    //uint32_t    buf_auto_incr_lenth[VPU_RDMA_CHAN_NUM][VPU_RDMA_CHAN_BUF_NUM];
}MNT_RDMA_t;

extern VPU_RDMA_BUF_t  g_vpu_rdma_wbuf;  //for reg wr
extern VPU_RDMA_BUF_t  g_vpu_rdma_rbuf;  //for reg rd

extern void vpu_rdma_buf_init(VPU_RDMA_BUF_t* vpu_rdma_buf, uint32_t chan_idx, uint32_t chan_buf_idx, uint64_t chan_st_addr, uint32_t is_ddr_wr);
extern void vpu_rdma_wr_ddr(VPU_RDMA_BUF_t* vpu_rdma_buf, uint64_t reg_addr, uint64_t reg_data);
extern void get_vpu_rdma_buf_info(uint64_t *ddr_st_addr, uint64_t *ddr_end_addr, uint32_t *ddr_buf_len, VPU_RDMA_BUF_t *vpu_rdma_buf, uint32_t chan_idx, uint32_t chan_buf_idx);

extern void init_vpu_rdma_params(MNT_RDMA_t *rdma_test_vec);
extern void rdma_config_v1(MNT_RDMA_t *rdma_test_vec, uint32_t rdma_chn_idx);
//extern void cfg_rdma_auto_buf_len(MNT_RDMA_t *rdma_test_vec, uint32_t chan_idx, uint32_t buf_idx);
extern void vpu_rdma_init(MNT_RDMA_t *rdma_test_vec,VPU_RDMA_BUF_t* vpu_rdma_buf, uint8_t cbus_auto_write,
                   uint32_t rdma_chn_idx,uint32_t int_src_idx);

//extern void cfg_rdma_auto_src_sel(uint32_t chan_idx, MNT_RDMA_t *rdma_test_vec);
extern void vpu_rdma_wr_ddr1(uint64_t reg_addr,uint64_t reg_data, uint32_t is_reg_wr);
extern int  check_rdma_status();
extern uint32_t sel_vpu_rdma(uint32_t reg_dpss_rdma, uint32_t reg_vpu_rdma);
extern int set_rdma_int_mode(uint32_t is_pulse);
extern void start_rdma_chan0( );
extern uint32_t get_rdma_proc_chan_idx();
extern uint32_t get_rdma_proc_buf_idx(uint32_t rdma_chan_idx);
extern uint32_t cal_reg_sft_bit(uint32_t buf_bytes_len);

#ifndef VERIFICATION
    #ifndef VPU_FPGA_EN
        #define Wr_ddr(addr, data) Wr64(addr, data)
    #else
        #define Wr_ddr(addr, data) //Wr64(addr, data)    
    #endif
#else
    #define Wr_ddr(addr, data) Wr_ddr_64bit(addr, data)
#endif

#ifdef RDMA_CFG_REG
    #define Wr_vpu(addr, data)                          rdma_buf = vpu_rdma_wr(rdma_buf,addr,data,0,32)   
    #define Rd_vpu(addr)                                rdma_buf = vpu_rdma_rd(rdma_buf,addr)
    #define Wr_vpu_reg_bits(reg_addr, val, start, len)  rdma_buf = vpu_rdma_wr(rdma_buf,reg_addr,val,start,len)  
#else
    #define Wr_vpu(addr, data)                          Wr(addr, data)   
    #define Rd_vpu(addr)                                Rd(addr) 
    #define Wr_vpu_reg_bits(reg_addr, val, start, len)  Wr_reg_bits(reg_addr, val, start, len)    
#endif

#endif //_VPU_RDMA_H_
