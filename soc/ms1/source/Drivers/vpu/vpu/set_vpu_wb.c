
#include "set_vpu_wb.h"

void set_gain_offset_regs(MNT_WB_t *mnt_vpu_wb)
{
    Wr(VPP_GAINOFF_CTRL0, (mnt_vpu_wb->gain0        <<16)|
                           mnt_vpu_wb->gain1             );
    Wr(VPP_GAINOFF_CTRL1, (mnt_vpu_wb->gain2        <<16)|
                          (mnt_vpu_wb->offset0    &0x7FF));
    Wr(VPP_GAINOFF_CTRL2, (mnt_vpu_wb->offset1      <<16)|
                          (mnt_vpu_wb->offset2    &0x7FF));
    Wr(VPP_GAINOFF_CTRL3, (mnt_vpu_wb->pre_offset0  <<16)|
                          (mnt_vpu_wb->pre_offset1&0x7FF));
    Wr(VPP_GAINOFF_CTRL4,  mnt_vpu_wb->pre_offset2       );
    Wr_reg_bits(VPP_GAINOFF_CTRL0,1,30,1);
    Wr_reg_bits(VPP_GAINOFF_CTRL0,mnt_vpu_wb->enable,31,1);
}

