#include "cfg_mnt_pps.h"

// uint32_t my_clz(uint32_t x)
//{
//     uint32_t n = 0;
//
//     if((x&0xffff0000)==0) {
//         x <<= 16;
//         n  += 16;
//     }
//     if((x&0xff000000)==0) {
//         x <<= 8;
//         n  += 8;
//     }
//     if((x&0xf0000000)==0) {
//         x <<= 4;
//         n  += 4;
//     }
//     if((x&0xc0000000)==0) {
//         x <<= 2;
//         n  += 2;
//     }
//     if((x&0x80000000)==0) {
//         n  += 1;
//     }
//
//     return n;
// }
//
// uint32_t div_u64(uint64_t dividend, uint32_t divisor)
//{
//     if (divisor==0) {
//         return 0;
//     }
//
//     uint32_t dividend_high = (uint32_t)(dividend>>32);
//     uint32_t dividend_low  = (uint32_t) dividend;
//     uint32_t divisor_high  =            divisor >>16;
//     uint32_t divisor_low   =            divisor &0xffff;
//
//     if(dividend_high>=divisor) {
//         return UINT32_MAX;
//     }
//
//     uint32_t shift = my_clz(divisor);
//     divisor <<= shift;
//
//     uint32_t scaled_dividend_high = (dividend_high <<     shift )
//                                   | (dividend_low  >> (32-shift));
//     uint32_t scaled_dividend_low  = (dividend_low  <<     shift );
//
//     uint32_t quotient_high, quotient_low, remainder, intermediate;
//
//     quotient_high = scaled_dividend_high / divisor_high;
//     remainder     = scaled_dividend_high % divisor_high;
//
//     intermediate  = remainder*UINT16_MAX + (scaled_dividend_low>>16);
//     while(quotient_high>=UINT16_MAX || quotient_high*divisor_low>intermediate) {
//         quotient_high--;
//         remainder += divisor_high;
//         if(remainder>=UINT16_MAX) break;
//         intermediate = remainder*UINT16_MAX + (scaled_dividend_low>>16);
//     }
//
//     quotient_low  = intermediate / divisor_high;
//     remainder     = intermediate % divisor_high;
//
//     intermediate  = remainder*UINT16_MAX + (scaled_dividend_low&0xffff);
//     while(quotient_low>=UINT16_MAX || quotient_low*divisor_low>intermediate) {
//         quotient_low--;
//         remainder += divisor_high;
//         if(remainder>=UINT16_MAX) break;
//         intermediate  = remainder*UINT16_MAX + (scaled_dividend_low&0xffff);
//     }
//
//     return (quotient_high<<16) | quotient_low;
// }

void cfg_mnt_pps(uint32_t index, MNT_PPS_t *pps) {
  uint32_t pps_hsize_in = pps->src_hsize;
  uint32_t pps_vsize_in = pps->src_vsize;
  uint32_t pps_hsize_out = pps->dst_hsize;
  uint32_t pps_vsize_out = pps->dst_vsize;

  char posthsc_en = pps->posthsc_en; // 1 bits postscaler en
  char postvsc_en = pps->postvsc_en; // 1 bits postscaler en

  int hsc_integer_part = 0x1;
  int hsc_fraction_part = 0x000000;
  int vsc_integer_part = 0x1;
  int vsc_fraction_part = 0x000000;

  int hsc_ini_integer = 0;
  int hsc_ini_phase = 0;
  int vsc_ini_integer = 0;
  int vsc_ini_phase = 0;

  int posthsc_tap_num = 4;
  int postvsc_tap_num = 2;

  int pps_en = (posthsc_en == 0) &&
                       (postvsc_en == 0)
                   ? 0
                   : 1;

  if (pps_hsize_in < pps_hsize_out) {
    hsc_ini_integer = 0x1f;
    hsc_ini_phase = (((int64_t)pps_hsize_in << 16) / pps_hsize_out + (1 << 16)) / 2;
  } else {
    hsc_ini_integer = ((((int64_t)pps_hsize_in << 16) / pps_hsize_out - (1 << 16)) / 2) >> 16;
    hsc_ini_phase = (((int64_t)pps_hsize_in << 16) / pps_hsize_out - (1 << 16)) / 2 - (hsc_ini_integer << 16);
  }
  if (pps_vsize_in < pps_vsize_out) {
    vsc_ini_integer = 0x1f;
    vsc_ini_phase = (((int64_t)pps_vsize_in << 16) / pps_vsize_out + (1 << 16)) / 2;
  } else {
    vsc_ini_integer = ((((int64_t)pps_vsize_in << 16) / pps_vsize_out - (1 << 16)) / 2) >> 16;
    vsc_ini_phase = (((int64_t)pps_vsize_in << 16) / pps_vsize_out - (1 << 16)) / 2 - (vsc_ini_integer << 16);
  }
  // postscale phase step
  if (pps_hsize_out == 0 || pps_vsize_out == 0) {
    printf("[DRI-ERROR] isp_disp.c Zore overflow!!!");
  } else {

    hsc_integer_part = pps_hsize_in / pps_hsize_out;
    hsc_fraction_part = ((pps_hsize_in << 20) / pps_hsize_out) << (4 - (hsc_integer_part << 24));
    // hsc_fraction_part   = div_u64((pps_hsize_in<<24), pps_hsize_out) - (hsc_integer_part<<24);

    vsc_integer_part = pps_vsize_in / pps_vsize_out;
    vsc_fraction_part = ((pps_vsize_in << 20) / pps_vsize_out) << (4 - (vsc_integer_part << 24));
    // vsc_fraction_part   = div_u64((pps_vsize_in<<24), pps_vsize_out) - (vsc_integer_part<<24);
  }

  cfg_mnt_pps_coef(index, VOUT_APB_BASE_ADDR + (0x2b0 << 2), VOUT_APB_BASE_ADDR + (0x2b1 << 2));

  Wr_reg_bits(VOUT_APB_BASE_ADDR + (0x2a1 << 2), vsc_integer_part, 24, 4);
  Wr_reg_bits(VOUT_APB_BASE_ADDR + (0x2a1 << 2), vsc_fraction_part, 0, 24);
  Wr_reg_bits(VOUT_APB_BASE_ADDR + (0x2a2 << 2), hsc_integer_part, 24, 4);
  Wr_reg_bits(VOUT_APB_BASE_ADDR + (0x2a2 << 2), hsc_fraction_part, 0, 24);

  Wr(VOUT_APB_BASE_ADDR + (0x2a3 << 2), ((vsc_integer_part & 0x1f) << 16) |
                                            ((vsc_ini_phase & 0xffff) << 0));

  Wr(VOUT_APB_BASE_ADDR + (0x2a4 << 2), ((hsc_integer_part & 0x1f) << 16) |
                                            ((hsc_ini_phase & 0xffff) << 0));

  Wr(VOUT_APB_BASE_ADDR + (0x2a0 << 2), ((7 & 0xf) << 16) |     // 9:s11_coef 7:s9 coef
                                            ((7 & 0xf) << 12) | // 9:s11_coef 7:s9 coef
                                            ((postvsc_en & 0x1) << 9) |
                                            ((posthsc_en & 0x1) << 8) |
                                            ((posthsc_tap_num & 0xf) << 4) |
                                            ((postvsc_tap_num & 0xf) << 0));

  Wr_reg_bits(VOUT_APB_BASE_ADDR + (0x2a5 << 2), pps_hsize_out, 16, 13);
  Wr_reg_bits(VOUT_APB_BASE_ADDR + (0x2a5 << 2), pps_vsize_out, 0, 13);

  Wr_reg_bits(VOUT_APB_BASE_ADDR + (0x2a6 << 2), pps_en, 0, 1);

  // if(pps->pps_dbg_en!=0) mnt_pps_reg_debug();
}

void cfg_mnt_pps_coef(uint32_t index, uint32_t pps_scale_coef_idx, uint32_t pps_scale_coef) {
  int i;
  int16_t pps_lut_tap2[33][2] = {{128, 0},
                                 {127, 1},
                                 {126, 2},
                                 {124, 4},
                                 {123, 5},
                                 {122, 6},
                                 {120, 8},
                                 {118, 10},
                                 {116, 12},
                                 {114, 14},
                                 {112, 16},
                                 {110, 18},
                                 {108, 20},
                                 {106, 22},
                                 {104, 24},
                                 {101, 27},
                                 {99, 29},
                                 {97, 31},
                                 {94, 34},
                                 {91, 37},
                                 {89, 39},
                                 {86, 42},
                                 {84, 44},
                                 {81, 47},
                                 {78, 50},
                                 {75, 53},
                                 {73, 55},
                                 {70, 58},
                                 {68, 60},
                                 {67, 61},
                                 {66, 62},
                                 {65, 63},
                                 {64, 64}};

  int16_t pps_lut_tap4[33][4] = {{0, 128, 0, 0},
                                 {0, 127, 1, 0},
                                 {-1, 127, 2, 0},
                                 {-3, 127, 4, 0},
                                 {-4, 127, 5, 0},
                                 {-4, 126, 6, 0},
                                 {-5, 126, 8, -1},
                                 {-6, 125, 10, -1},
                                 {-6, 123, 12, -1},
                                 {-7, 122, 14, -1},
                                 {-7, 120, 16, -1},
                                 {-8, 120, 18, -2},
                                 {-8, 118, 20, -2},
                                 {-8, 116, 22, -2},
                                 {-9, 115, 24, -2},
                                 {-9, 113, 27, -3},
                                 {-9, 111, 29, -3},
                                 {-9, 109, 31, -3},
                                 {-9, 107, 34, -4},
                                 {-9, 104, 37, -4},
                                 {-9, 102, 39, -4},
                                 {-9, 100, 42, -5},
                                 {-9, 98, 44, -5},
                                 {-9, 95, 47, -5},
                                 {-9, 93, 50, -6},
                                 {-9, 90, 53, -6},
                                 {-9, 88, 55, -6},
                                 {-9, 86, 58, -7},
                                 {-9, 83, 61, -7},
                                 {-9, 80, 64, -7},
                                 {-8, 77, 66, -7},
                                 {-8, 75, 69, -8},
                                 {-8, 72, 72, -8}};

  int16_t pps_lut_tap4_s11[33][4] = {{0, 512, 0, 0},
                                     {-5, 512, 5, 0},
                                     {-10, 511, 11, 0},
                                     {-14, 510, 17, -1},
                                     {-18, 508, 23, -1},
                                     {-22, 506, 29, -1},
                                     {-25, 503, 36, -2},
                                     {-28, 500, 43, -3},
                                     {-32, 496, 51, -3},
                                     {-34, 491, 59, -4},
                                     {-37, 487, 67, -5},
                                     {-39, 482, 75, -6},
                                     {-41, 476, 84, -7},
                                     {-42, 470, 92, -8},
                                     {-44, 463, 102, -9},
                                     {-45, 456, 111, -10},
                                     {-45, 449, 120, -12},
                                     {-47, 442, 130, -13},
                                     {-47, 434, 140, -15},
                                     {-47, 425, 151, -17},
                                     {-47, 416, 161, -18},
                                     {-47, 407, 172, -20},
                                     {-47, 398, 182, -21},
                                     {-47, 389, 193, -23},
                                     {-46, 379, 204, -25},
                                     {-45, 369, 215, -27},
                                     {-44, 358, 226, -28},
                                     {-43, 348, 237, -30},
                                     {-43, 337, 249, -31},
                                     {-41, 326, 260, -33},
                                     {-40, 316, 271, -35},
                                     {-39, 305, 282, -36},
                                     {-37, 293, 293, -37}};

  //=================================== postsc coef lut ==================================
  // vscaler
  // cfg 2tap
  Wr(pps_scale_coef_idx, (0 << 17) |
                             (0 << 16) |
                             (0 << 15) |
                             (0 << 14) |
                             (1 << 9) |
                             (0 << 7) |
                             (0 << 0));

  for (i = 0; i < 33; i++) {
    Wr(pps_scale_coef, ((pps_lut_tap2[i][0] & 0x7ff) << 16) |
                           ((pps_lut_tap2[i][1] & 0x7ff) << 0));
    Wr(pps_scale_coef, ((0 & 0x7ff) << 16) |
                           ((0 & 0x7ff) << 0));
  }

  // cfg 2tap
  Wr(pps_scale_coef_idx, (0 << 17) |
                             (0 << 16) |
                             (0 << 15) |
                             (0 << 14) |
                             (1 << 9) |
                             (1 << 7) |
                             (0 << 0));
  for (i = 0; i < 33; i++) {
    Wr(pps_scale_coef, ((pps_lut_tap2[i][0] & 0x1ff) << 16) |
                           ((pps_lut_tap2[i][1] & 0x1ff) << 0));
    Wr(pps_scale_coef, ((0 & 0x1ff) << 16) |
                           ((0 & 0x1ff) << 0));
  }

  // hscaler

  Wr(pps_scale_coef_idx, (0 << 17) |
                             (0 << 16) |
                             (0 << 15) |
                             (0 << 14) |
                             (1 << 9) |
                             (2 << 7) |
                             (0 << 0));

  for (i = 0; i < 33; i++) {
    Wr(pps_scale_coef, ((pps_lut_tap4[i][0] & 0x1ff) << 16) |
                           ((pps_lut_tap4[i][1] & 0x1ff) << 0));
    Wr(pps_scale_coef, ((pps_lut_tap4[i][2] & 0x1ff) << 16) |
                           ((pps_lut_tap4[i][3] & 0x1ff) << 0));
  }

  Wr(pps_scale_coef_idx, (4 << 17) |
                             (1 << 16) |
                             (0 << 15) |
                             (0 << 14) |
                             (1 << 9) |
                             (0 << 7) |
                             (0 << 0));
  for (i = 0; i < 33; i++) {
    Wr(pps_scale_coef, ((pps_lut_tap4[i][0] & 0x1ff) << 16) |
                           ((pps_lut_tap4[i][1] & 0x1ff) << 0));
    Wr(pps_scale_coef, ((pps_lut_tap4[i][2] & 0x1ff) << 16) |
                           ((pps_lut_tap4[i][3] & 0x1ff) << 0));
  }
}

/*
void mnt_pps_reg_debug() {
    uint32_t rd_pps_scale_en             = Rd(PPS_SCALE_EN            );
    uint32_t rd_pps_vsc_start_phase_step = Rd(PPS_VSC_START_PHASE_STEP);
    uint32_t rd_pps_hsc_start_phase_step = Rd(PPS_HSC_START_PHASE_STEP);
    uint32_t rd_pps_vsc_init             = Rd(PPS_VSC_INIT            );
    uint32_t rd_pps_hsc_init             = Rd(PPS_HSC_INIT            );
    uint32_t rd_pps_scale_ctrl           = Rd(PPS_SCALE_CTRL          );
    uint32_t rd_pps_scale_coef_idx[4]    ;
    uint32_t rd_pps_scale_coef           ;

    uint16_t read_v_s9_9bit[2][33][4]    ;
    uint16_t read_h_s9_9bit[2][33][4]    ;

    int      n, i, j                     ;

    rd_pps_scale_coef_idx[0] = (0 << 17 ) | (0 << 16 ) | (0 << 15 ) | (0 << 14 ) | (1 << 9  ) | (0 << 7  ) | (0 << 0  );
    rd_pps_scale_coef_idx[1] = (0 << 17 ) | (0 << 16 ) | (0 << 15 ) | (0 << 14 ) | (1 << 9  ) | (1 << 7  ) | (0 << 0  );
    rd_pps_scale_coef_idx[2] = (0 << 17 ) | (0 << 16 ) | (0 << 15 ) | (0 << 14 ) | (1 << 9  ) | (2 << 7  ) | (0 << 0  );
    rd_pps_scale_coef_idx[3] = (4 << 17 ) | (1 << 16 ) | (0 << 15 ) | (0 << 14 ) | (1 << 9  ) | (0 << 7  ) | (0 << 0  );

    for(n=0; n<4; n++) {
        Wr(PPS_SCALE_COEF_IDX, (( 1    << 14)|   // rd_cbus_coef_en: rd for debug only
                    rd_pps_scale_coef_idx[n]));  // coef_index

        rd_pps_scale_coef = Rd(PPS_SCALE_COEF);  // empty read
        for(i=0; i<33; i++) {
            for(j=0; j<2; j++) {
                rd_pps_scale_coef = Rd(PPS_SCALE_COEF);
                if(n<2) {
                    read_v_s9_9bit[n][i][j*2+0]   = (rd_pps_scale_coef>>16)&0x1ff;
                    read_v_s9_9bit[n][i][j*2+1]   = (rd_pps_scale_coef>> 0)&0x1ff;
                } else {
                    read_h_s9_9bit[n-2][i][j*2+0] = (rd_pps_scale_coef>>16)&0x1ff;
                    read_h_s9_9bit[n-2][i][j*2+1] = (rd_pps_scale_coef>> 0)&0x1ff;
                }
            }
        }
    }

    printf("`=====================================pps_cfg_start=====================================\n");

    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_EN.reg_vsc_nor_rs_bits.set(%d)    ;\n", (rd_pps_scale_en>>28)&0xf);
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_EN.reg_hsc_nor_rs_bits.set(%d)    ;\n", (rd_pps_scale_en>>24)&0xf);
    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_EN.reg_prehsc_en.set(%d)    ;\n"      , (rd_pps_scale_en>>23)&0x1);
    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_EN.reg_prevsc_en.set(%d)    ;\n"      , (rd_pps_scale_en>>22)&0x1);
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_EN.reg_vsc_en.set(%d)    ;\n"         , (rd_pps_scale_en>>21)&0x1);
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_EN.reg_hsc_en.set(%d)    ;\n"         , (rd_pps_scale_en>>20)&0x1);
    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_EN.reg_prehsc_rate.set(%d)    ;\n"    , (rd_pps_scale_en>>18)&0x3);
    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_EN.reg_prevsc_rate.set(%d)    ;\n"    , (rd_pps_scale_en>>16)&0x3);
    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_EN.reg_prehsc_flt_num.set(%d)    ;\n" , (rd_pps_scale_en>>12)&0xf);
    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_EN.reg_prevsc_flt_num.set(%d)    ;\n" , (rd_pps_scale_en>> 8)&0xf);
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_EN.reg_hsc_tap_num.set(%d)    ;\n"    , (rd_pps_scale_en>> 4)&0xf);
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_EN.reg_vsc_tap_num.set(%d)    ;\n"    , (rd_pps_scale_en>> 0)&0xf);
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_EN.update(status)    ;\n\n\n"         );

    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_PRE_HSCALE_COEF_Y1.reg_prehsc_coef_3.set(%d)    ;\n", (rd_pps_pre_hscale_coef_y1>>16)&0xffff);
    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_PRE_HSCALE_COEF_Y1.reg_prehsc_coef_2.set(%d)    ;\n", (rd_pps_pre_hscale_coef_y1>> 0)&0xffff);
    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_PRE_HSCALE_COEF_Y1.update(status)    ;\n\n\n"       );

    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_PRE_HSCALE_COEF_Y0.reg_prehsc_coef_1.set(%d)    ;\n", (rd_pps_pre_hscale_coef_y0>>16)&0xffff);
    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_PRE_HSCALE_COEF_Y0.reg_prehsc_coef_0.set(%d)    ;\n", (rd_pps_pre_hscale_coef_y0>> 0)&0xffff);
    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_PRE_HSCALE_COEF_Y0.update(status)    ;\n\n\n"       );

    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_PRE_VSCALE_COEF_0.reg_prevsc_coef_1.set(%d)    ;\n", (rd_pps_pre_vscale_coef_0>>16)&0xffff);
    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_PRE_VSCALE_COEF_0.reg_prevsc_coef_0.set(%d)    ;\n", (rd_pps_pre_vscale_coef_0>> 0)&0xffff);
    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_PRE_VSCALE_COEF_0.update(status)    ;\n\n\n"       );

    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_PRE_VSCALE_COEF_1.reg_prevsc_coef_3.set(%d)    ;\n", (rd_pps_pre_vscale_coef_1>>16)&0xffff);
    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_PRE_VSCALE_COEF_1.reg_prevsc_coef_2.set(%d)    ;\n", (rd_pps_pre_vscale_coef_1>> 0)&0xffff);
    //printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_PRE_VSCALE_COEF_1.update(status)    ;\n\n\n"       );

    for(n=0; n<4; n++) {
        printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_COEF_IDX.reg_type_index_ext.set(%d)    ;\n" , (rd_pps_scale_coef_idx[n]>>17)&0x7 );
        printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_COEF_IDX.reg_ctype_ext_mode.set(%d)    ;\n" , (rd_pps_scale_coef_idx[n]>>16)&0x1 );
        printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_COEF_IDX.reg_index_inc.set(%d)    ;\n"      , (rd_pps_scale_coef_idx[n]>>15)&0x1 );
        printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_COEF_IDX.reg_rd_cbus_coef_en.set(%d)    ;\n", (rd_pps_scale_coef_idx[n]>>14)&0x1 );
        printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_COEF_IDX.reg_high_reso_en.set(%d)    ;\n"   , (rd_pps_scale_coef_idx[n]>> 9)&0x1 );
        printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_COEF_IDX.reg_type_index.set(%d)    ;\n"     , (rd_pps_scale_coef_idx[n]>> 7)&0x3 );
        printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_COEF_IDX.reg_coef_index.set(%d)    ;\n"     , (rd_pps_scale_coef_idx[n]>> 0)&0x7f);
        printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_COEF_IDX.update(status)    ;\n\n\n"         );

        for (i=0; i<33; i++) {
            for(j=0; j<2; j++) {
                if(n<2) {
                    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_COEF.reg_coef_data_1.set(%d)    ;\n", read_v_s9_9bit[n][i][j*2+0]);
                    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_COEF.reg_coef_data_0.set(%d)    ;\n", read_v_s9_9bit[n][i][j*2+1]);
                } else {
                    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_COEF.reg_coef_data_1.set(%d)    ;\n", read_h_s9_9bit[n-2][i][j*2+0]);
                    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_COEF.reg_coef_data_0.set(%d)    ;\n", read_h_s9_9bit[n-2][i][j*2+1]);
                }

                if(j<1) {
                    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_COEF.update(status)    ;\n"         );
                } else {
                    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_COEF.update(status)    ;\n\n\n"     );
                }
            }
        }
    }

    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_VSC_START_PHASE_STEP.reg_vf_sep_coef_en.set(%d)    ;\n"   , (rd_pps_vsc_start_phase_step>>28)&0x1     );
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_VSC_START_PHASE_STEP.reg_vsc_integer_part.set(%d)    ;\n" , (rd_pps_vsc_start_phase_step>>24)&0xf     );
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_VSC_START_PHASE_STEP.reg_vsc_fraction_part.set(%d)    ;\n", (rd_pps_vsc_start_phase_step>> 0)&0xffffff);
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_VSC_START_PHASE_STEP.update(status)    ;\n\n\n"           );

    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_HSC_START_PHASE_STEP.reg_hsc_integer_part.set(%d)    ;\n" , (rd_pps_hsc_start_phase_step>>24)&0xf     );
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_HSC_START_PHASE_STEP.reg_hsc_fraction_part.set(%d)    ;\n", (rd_pps_hsc_start_phase_step>> 0)&0xffffff);
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_HSC_START_PHASE_STEP.update(status)    ;\n\n\n"           );

    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_VSC_INIT.reg_vsc_ini_integer.set(%d)    ;\n", (rd_pps_vsc_init>>16)&0x1f  );
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_VSC_INIT.reg_vsc_ini_phase.set(%d)    ;\n"  , (rd_pps_vsc_init>> 0)&0xffff);
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_VSC_INIT.update(status)    ;\n\n\n"         );

    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_HSC_INIT.reg_hsc_ini_integer.set(%d)    ;\n", (rd_pps_hsc_init>>16)&0x1f  );
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_HSC_INIT.reg_hsc_ini_phase.set(%d)    ;\n"  , (rd_pps_hsc_init>> 0)&0xffff);
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_HSC_INIT.update(status)    ;\n\n\n"         );

    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_CTRL.reg_coef_s11_mode.set(%d)    ;\n"  , (rd_pps_scale_ctrl>>2)&0x1);
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_CTRL.reg_post_sc_mux_sel.set(%d)    ;\n", (rd_pps_scale_ctrl>>1)&0x1);
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_CTRL.reg_scale_en.set(%d)    ;\n"       , (rd_pps_scale_ctrl>>0)&0x1);
    printf("`REGMODEL.vout_pps_reg_model_C.VOUT_PPS_SCALE_CTRL.update(status)    ;\n\n\n"         );

    printf("`=====================================pps_cfg_end=======================================\n");

}
*/
