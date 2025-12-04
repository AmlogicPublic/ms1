#include "set_vpu_cm.h"

////=========================================================////
////  G9TV VPP NEW FUNCTION 
////  add by minjie li in 201303
////=========================================================////
//// cm2
int set_chroma_regs (MNT_CM_t *cm)
{
    uint16_t hsize       = cm->hsize      ;
    uint16_t vsize       = cm->vsize      ;
    char     cm2_en      = cm->cm2_en     ;
    char     hue_adj_en  = cm->hue_adj_en ;
    char     sat_adj_en  = cm->sat_adj_en ;
    char     luma_adj_en = cm->luma_adj_en;
    char     xvycc_mode  = cm->xvycc_mode ;

    int i;
    int j,k;
    int8_t cm2_cfg_reg[700];
    int8_t tmp[64];

	int16_t reg_cm2_xvycc_luma_min  ;         // s12 [-2048,2047]
	int16_t reg_cm2_xvycc_luma_max  ;         // s12 [-2048,2047]
    int16_t reg_cm2_xvycc_chrm_u_min;         // s12 [-2048,2047]
	int16_t reg_cm2_xvycc_chrm_u_max;         // s12 [-2048,2047]
	int16_t reg_cm2_xvycc_chrm_v_min;         // s12 [-2048,2047]
	int16_t reg_cm2_xvycc_chrm_v_max;         // s12 [-2048,2047]
    int16_t reg_cm_luma_blacklevel = 64;
    int16_t reg_cm_luma_contrast   = 0x400;


    static int8_t reg_CM2_Adj_Luma_via_Hue[32]=  // 8bits-signed (-128~127) 
        {0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  6, 10, 10, 14, 16, 14, 12, 10,  8, 4, 0, 0, 0, 0, 0, 0};                                                 
    // parameters reg_CM2_Adj_Sat_via_HS[32] 8bits-signed
    static int8_t reg_CM2_Adj_Sat_via_HS[3][32]=  // 8bits-signed (-128~127) 
        {{ 8,  8, 8, 8, 8, 8, 8, 8,  0, 0, 0, 0, 0, 0, 0, 8,  18,  18, 18, 18, 18,  18, 18, 18,  14, 8,  8,   8,  8,  8, 8, 8},  // Satth0
    {20, 20,20,20,20,20,20,20,  0, 0, 0, 0, 0, 0, 0,10,  20, 30,  40, 40, 40,  36, 20, 20,  10, 20, 20, 20, 20, 20,20, 2},  // Satth1
         { 0,  0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0,-1,  -1,  4,   4,  4,  4,  24, 20, 20,  10, 10, 10, 10, 10, 10,10, 10}};// Satth2


    static int8_t reg_CM2_Adj_Satgain_via_Y[5][32]= // 8bits-signed (-128~127) Normalized to 128
        {{0, 0, 0, 0, 0, 0, 0, 0,   20, 20, 20, 20, 20, 20, 20, 20, 18,10, 8, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},  // Luma=0
    {0, 0, 0, 0, 0, 0, 0, 0,   20, 20, 20, 20, 20, 20, 20, 20, 18,10, 8, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},  // Luma=1/4
    {0, 0, 0, 0, 0, 0, 0, 0,   10, 10, 10, 10, 10, 10, 10, 10, 10, 6, 4, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},  // Luma=2/4
    {0, 0, 0, 0, 0, 0, 0, 0,    8,  8,  8,  8,  8,  8,  8,  8,  6, 4, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},  // Luma=3/4
         {0, 0, 0, 0, 0, 0, 0, 0,    8,  8,  8,  8,  8,  8,  8,  8,  4, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0}}; // Luma=4/4


    static int8_t reg_CM2_Adj_SatGLBgain_via_Y[9]= {15, 18, 20, 28, 30, 30, 28, 18,  15};        // global gain for all hue/sat
    
    // parameters reg_CM2_Adj_Hue_via_H[32] 8bits-signed
    static int8_t reg_CM2_Adj_Hue_via_H[32]=  // 8bits-signed (-128~127) 
        {0, 10, 18, 10, 0, -10, -18, -8,   0,  0, 0, 0, 0, 0, 0, 0,   0, 10, 20, 10, -10,-20,-10,-8,  0, 0, 0, 0,  0, 0, 0, 0};
    
    // parameters reg_CM2_Adj_Hue_via_S[5][32] 8bits-signed
    static int8_t reg_CM2_Adj_Hue_via_S[5][32]=  // 8bits-signed (-128~127) 
        {{0,  0, 0, 0, 0, 0, 0, -4,  -8, -12, -16,-10,-4, 2, 4, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},  // Sat= 0
    {0,  0, 0, 0, 0, 0, 0, -3,  -6, -12, -12,-8, -4, 2, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},  // Sat= 1/4
    {0,  0, 0, 0, 0, 0, 0, -2,  -4, -10, -10,-6, -4, 1, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},  // Sat= 2/4
    {0,  0, 0, 0, 0, 0, 0, -1,  -1,  -1,   0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},  // Sat= 3/4
        {0,  0, 0, 0, 0, 0, 0,  0,  0, 0, 0,   0, 0,  0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0}};    // Sat= 4/4

    
    // parameters reg_CM2_Adj_Hue_via_Y[5][32] 8bits-signed
    static int8_t reg_CM2_Adj_Hue_via_Y[5][32]=  // 8bits-signed (-128~127) 
        {{0, 0,   0,   0, 0, 0, 0, 0,   0,   0,   0,   0,  0,  0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,   0,  0,  0,  0,  0,  0,  0,  0},  // Y= 0
    {0, 0,   0,   0, 0, 0, 0, 0,   0,   0,   0,   0,  0,  0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,   0,  0,  0,  0,  0,  0,  0,  0},  // Y= 1/4
    {0, 0,   0,   0, 0, 0, 0, 0,  -2,  -8, -10, -10, -8, -6, 2, 0,  0, 0, 0, 0, 0, 0, 0, 0,  10, 10, 10,  0,-10,-18,-10, -4},  // Y= 2/4
    {0, 0,   0,   0, 0, 0, 0, 0,  -4,  -8, -10, -10, -8, -6, 2, 0,  0, 0, 0, 0, 0, 0, 0, 0,  20, 20, 10,  0,-10,-18,-10, -4},  // Y= 3/4
        {0, 0,   0,   0, 0, 0, 0, 0,  -1,  -8,  -4,  -2, -2, -1, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  20, 20, 10,  0,-10,-18,-10, -4}}; // Y= 4/4

    Wr(VPP_CHROMA_ADDR_PORT, CM_ENH_CTL_REG);
    Wr(VPP_CHROMA_DATA_PORT, ( ((hue_adj_en &0x1)<<4) |
                               ((sat_adj_en &0x1)<<3) |
                               ((luma_adj_en&0x1)<<2) |
                               ((cm2_en     &0x1)   ) ) );

    if ( cm2_en ) {
        for ( i=0; i<32 ; i=i+1 ) {
            cm2_cfg_reg[0   +i]  = reg_CM2_Adj_Luma_via_Hue[i];
            cm2_cfg_reg[32*1+i]  = reg_CM2_Adj_Sat_via_HS[0][i];
            cm2_cfg_reg[32*2+i]  = reg_CM2_Adj_Sat_via_HS[1][i];
            cm2_cfg_reg[32*3+i]  = reg_CM2_Adj_Sat_via_HS[2][i];
            cm2_cfg_reg[32*4+i]  = reg_CM2_Adj_Hue_via_H[i];
            cm2_cfg_reg[32*5+i]  = reg_CM2_Adj_Hue_via_Y[0][i];
            cm2_cfg_reg[32*6+i]  = reg_CM2_Adj_Hue_via_Y[1][i];
            cm2_cfg_reg[32*7+i]  = reg_CM2_Adj_Hue_via_Y[2][i];
            cm2_cfg_reg[32*8+i]  = reg_CM2_Adj_Hue_via_Y[3][i];
            cm2_cfg_reg[32*9+i]  = reg_CM2_Adj_Hue_via_Y[4][i];
            cm2_cfg_reg[32*10+i] = reg_CM2_Adj_Hue_via_S[0][i];
            cm2_cfg_reg[32*11+i] = reg_CM2_Adj_Hue_via_S[1][i];
            cm2_cfg_reg[32*12+i] = reg_CM2_Adj_Hue_via_S[2][i];
            cm2_cfg_reg[32*13+i] = reg_CM2_Adj_Hue_via_S[3][i];
            cm2_cfg_reg[32*14+i] = reg_CM2_Adj_Hue_via_S[4][i];
            cm2_cfg_reg[32*15+i] = reg_CM2_Adj_Satgain_via_Y[0][i];
            cm2_cfg_reg[32*16+i] = reg_CM2_Adj_Satgain_via_Y[1][i];
            cm2_cfg_reg[32*17+i] = reg_CM2_Adj_Satgain_via_Y[2][i];
            cm2_cfg_reg[32*18+i] = reg_CM2_Adj_Satgain_via_Y[3][i];
            cm2_cfg_reg[32*19+i] = reg_CM2_Adj_Satgain_via_Y[4][i];
        }
        for ( j = 0; j<32; j++) {
            Wr(VPP_CHROMA_ADDR_PORT, 256+j*8);
            for ( i = 0; i<20; i++) {
                tmp[i] = cm2_cfg_reg[j+32*i];
            }
            for ( k = 0; k<5; k++) {
                Wr(VPP_CHROMA_DATA_PORT,( ((tmp[k*4+3]&0xff)<<24) |
                                          ((tmp[k*4+2]&0xff)<<16) |
                                          ((tmp[k*4+1]&0xff)<< 8) |
                                          ((tmp[k*4+0]&0xff)    ) ) );
            }
        }

        Wr(VPP_CHROMA_ADDR_PORT, SAT_BYYB_NODE_REG0);
        Wr(VPP_CHROMA_DATA_PORT, ( ((reg_CM2_Adj_SatGLBgain_via_Y[3]&0xff)<<24) |
                                   ((reg_CM2_Adj_SatGLBgain_via_Y[2]&0xff)<<16) |
                                   ((reg_CM2_Adj_SatGLBgain_via_Y[1]&0xff)<< 8) |
                                   ((reg_CM2_Adj_SatGLBgain_via_Y[0]&0xff)    ) ) );

        Wr(VPP_CHROMA_ADDR_PORT, SAT_BYYB_NODE_REG1);
        Wr(VPP_CHROMA_DATA_PORT, ( ((reg_CM2_Adj_SatGLBgain_via_Y[7]&0xff)<<24) |
                                   ((reg_CM2_Adj_SatGLBgain_via_Y[6]&0xff)<<16) |
                                   ((reg_CM2_Adj_SatGLBgain_via_Y[5]&0xff)<< 8) |
                                   ((reg_CM2_Adj_SatGLBgain_via_Y[4]&0xff)    ) ) );

        Wr(VPP_CHROMA_ADDR_PORT, SAT_BYYB_NODE_REG2);
        Wr(VPP_CHROMA_DATA_PORT,  ((reg_CM2_Adj_SatGLBgain_via_Y[8]&0xff)) );

	    reg_cm2_xvycc_luma_min  = xvycc_mode ?  0     :     0;  // s12 [0,2047]
	    reg_cm2_xvycc_luma_max  = xvycc_mode ?  0xfff : 0x3ff;  // s12 [0,4095]
        reg_cm2_xvycc_chrm_u_min= xvycc_mode ?  0     :     0;  // s12 [0,2047]
	    reg_cm2_xvycc_chrm_u_max= xvycc_mode ?  0xfff : 0x3ff;  // s12 [0,4095]
	    reg_cm2_xvycc_chrm_v_min= xvycc_mode ?  0     :     0;  // s12 [0,2047]
	    reg_cm2_xvycc_chrm_v_max= xvycc_mode ?  0xfff : 0x3ff;  // s12 [0,2047]

        Wr(VPP_CHROMA_ADDR_PORT, XVYCC_YSCP_REG);
        Wr(VPP_CHROMA_DATA_PORT, ( ((reg_cm2_xvycc_luma_max&0xfff)<<16) |
                                   ((reg_cm2_xvycc_luma_min&0xfff)    ) ) );
        Wr(VPP_CHROMA_ADDR_PORT, XVYCC_USCP_REG);
        Wr(VPP_CHROMA_DATA_PORT, ( ((reg_cm2_xvycc_chrm_u_max&0xfff)<<16) |
                                   ((reg_cm2_xvycc_chrm_u_min&0xfff)    ) ) );
        Wr(VPP_CHROMA_ADDR_PORT, XVYCC_VSCP_REG);
        Wr(VPP_CHROMA_DATA_PORT, ( ((reg_cm2_xvycc_chrm_v_max&0xfff)<<16) |
                                   ((reg_cm2_xvycc_chrm_v_min&0xfff)    ) ) );

        Wr(VPP_CHROMA_ADDR_PORT, FRM_SIZE_REG);
        Wr(VPP_CHROMA_DATA_PORT, ( ((vsize&0x1fff)<<16) |
                                   ((hsize&0x1fff)    ) ) );

        Wr(VPP_CHROMA_ADDR_PORT, LUMA_ADJ0_REG);
        Wr(VPP_CHROMA_DATA_PORT, (((reg_cm_luma_blacklevel&0xfff)<<12) |
                                  ((reg_cm_luma_contrast&0xfff)      ) ) );

    }
    return 0;
}

