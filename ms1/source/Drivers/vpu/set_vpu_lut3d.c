#include "set_vpu_lut3d.h"

int vpp_set_lut3d(char enable, char bLut3DLoad, int *pLut3D, char bLut3DCheck, uint8_t lut3d_mode) 
{
    int i;
    uint32_t dwTemp, wRgb[3];
    int lut_sz  = 9; //17
    if(bLut3DLoad) {
       Wr(VPP_LUT3D_CBUS2RAM_CTRL,1);
       Wr(VPP_LUT3D_RAM_ADDR,0|(0<<31));
       for(i=0;i<lut_sz*lut_sz*lut_sz;i++) {
           //{comp0, comp1, comp2}
           Wr(VPP_LUT3D_RAM_DATA,((pLut3D[i*3+1]&0xfff)<<16)|(pLut3D[i*3+2]&0xfff));
           Wr(VPP_LUT3D_RAM_DATA, (pLut3D[i*3+0]&0xfff)); //MSB
       }
    }
    if(bLut3DCheck) {
       Wr(VPP_LUT3D_CBUS2RAM_CTRL,1);
       Wr(VPP_LUT3D_RAM_ADDR,0|(1<<31));
       //dwTemp  = Rd(VPP_LUT3D_RAM_DATA);
       for(i=0;i<lut_sz*lut_sz*lut_sz;i++) {
           dwTemp  = Rd(VPP_LUT3D_RAM_DATA);
           wRgb[2] = dwTemp & 0xfff;
           wRgb[1] =(dwTemp>>16)&0xfff;
           dwTemp  = Rd(VPP_LUT3D_RAM_DATA);
           wRgb[0] = dwTemp & 0xfff;
           //printf("\nwRgb[2]   = %d, wRgb[1]   = %d, wRgb[0]   = %d\n",wRgb[2]  , wRgb[1]  , wRgb[0]  );
           //printf("pLut3D[2] = %d, pLut3D[1] = %d, pLut3D[0] = %d\n",pLut3D[i*3+2], pLut3D[i*3+1], pLut3D[i*3+0]);
           if(wRgb[0]!=pLut3D[i*3+0]) {
               printf("Error: Lut3d check error at R[%d]\n",i); 
               return 1;
           } 
           if(wRgb[1]!=pLut3D[i*3+1]) {
               printf("Error: Lut3d check error at G[%d]\n",i); 
               return 1;
           }
           if(wRgb[2]!=pLut3D[i*3+2]) {
               printf("Error: Lut3d check error at B[%d]\n",i); 
               return 1;
           } 
       }
    }
    Wr(VPP_LUT3D_CBUS2RAM_CTRL,0);
    Wr_reg_bits(VPP_LUT3D_CTRL,7,4,3);          //reg_lut3d_extnd_en[6:4]
    if(((lut3d_mode &0x3)>3)||(((lut3d_mode>>2) &0x3)>3)|| (((lut3d_mode>>4) &0x3)>3)) {
        printf("Error: Lut3d mode error[%d]\n",lut3d_mode); 
        return 1;
    }
    Wr_reg_bits(VPP_LUT3D_CTRL,((lut3d_mode &0x3)|(((lut3d_mode>>2) &0x3)<<2)|(((lut3d_mode>>4) &0x3))<<4),16,6);      //reg_lut3d_mode[21:16]
    Wr_reg_bits(VPP_LUT3D_CTRL,enable&0x1,0,1); //reg_lut3d_en
    return 0;
}

//table: use for yuv->rgb
void vpp_lut3d_table_init(int *pLut3D, int bitdepth)
{
    int d0, d1, d2, ncmp;
    int step[3]; // steps of each input components lut-nodes
    int max_val;
    int lut_div = 3; //4
    int lut_sz  = 9; //17

    int lut9[] = {0,128,256,384,512,640,768, 896,1023};
    max_val = (1<<bitdepth) -1 ;
    // step
    for (ncmp=0; ncmp<3; ncmp++)
        step[ncmp] = (1<<(bitdepth-lut_div));

    // initialize the lut3d ad same input and output;
    for (d0=0; d0<lut_sz; d0++)
    {
        for (d1=0;d1<lut_sz; d1++)
        {
            for (d2=0;d2<lut_sz; d2++)
            { 
                pLut3D[d0*lut_sz*lut_sz*3+d1*lut_sz*3+d2*3+0] = lut9[d0];//(d0*step[0]< max_val) ? d0*step[0] : max_val; // 1st components
                pLut3D[d0*lut_sz*lut_sz*3+d1*lut_sz*3+d2*3+1] = lut9[d1];//(d1*step[1]< max_val) ? d1*step[1] : max_val; // 2nd components
                pLut3D[d0*lut_sz*lut_sz*3+d1*lut_sz*3+d2*3+2] = lut9[d2];//(d2*step[2]< max_val) ? d2*step[2] : max_val; // 3rd components   
                //ycbcr2rgbpc_nb(&pLut3D[d0*lut_sz*lut_sz*3+d1*lut_sz*3+d2*3+0],&pLut3D[d0*lut_sz*lut_sz*3+d1*lut_sz*3+d2*3+1],&pLut3D[d0*lut_sz*lut_sz*3+d1*lut_sz*3+d2*3+2],
                                //pLut3D[d0*lut_sz*lut_sz*3+d1*lut_sz*3+d2*3+0], pLut3D[d0*lut_sz*lut_sz*3+d1*lut_sz*3+d2*3+1], pLut3D[d0*lut_sz*lut_sz*3+d1*lut_sz*3+d2*3+2],
                                //bitdepth);
            }        
        }   
    }
}

void ycbcr2rgbpc_nb(int *R, int *G, int *B, int Y, int Cb, int Cr, int bitdepth)
{
    int y = 0;
    int cb = 0;
    int cr = 0;
    int r = 0;
    int g = 0;
    int b = 0;
    int norm = (1<<bitdepth)-1;

    y  = Y;
    cb = Cb - (1<<(bitdepth-1));
    cr = Cr - (1<<(bitdepth-1));

    //r = (298 * y + 408 * cr) / 256;
    //g = (298 * y - 208 * cr - 100 * cb) / 256;
    //b = (298 * y            + 516 * cb) / 256;

    r = (1197 * y + 0    * cb + 1726* cr) / 1024;
    g = (1197 * y - 193  * cb - 669 * cr) / 1024;
    b = (1197 * y + 2202 * cb + 0   * cr) / 1024;

    if(r>norm) r = norm;
    if(g>norm) g = norm;
    if(b>norm) b = norm;

    if(r<0) r = 0;
    if(g<0) g = 0;
    if(b<0) b = 0;

    *R = r;
    *G = g;
    *B = b;

   //matrix coef BT709
    //int ycbcr2rgb[15]  =  {1197, 0,1726, 1197,-193,-669,1197,2202, 0,0,0,0,0,0,0};  
    //int rgb2ycbcr[15]  =  {230,594,  52, -125,-323, 448,448, -412,-36,0,0,0,0,0,0};
} 

void set_gamut_map1(uint32_t gamut_en)
{

    uint32_t matrix_en   = (gamut_en>>2) & 0x1;
    uint32_t eotf_en     = (gamut_en>>1) & 0x1;
    uint32_t oetf_en     = (gamut_en>>0) & 0x1;

    uint32_t ofst_inp[3] = {0,0,0};
    uint32_t ofst_oup[3] = {0,0,0};

    int matrix_coef[3][3] = {{ 230,  594,  52},
                             {-125, -323, 448},
                             { 448, -412, -36}};
    int matrix_3x3mtrx_rs = 2;

    Wr(GAMUT_MAP1_RP_CTRL      , (matrix_en<<3)|
                                 (eotf_en  <<2)|
                                 (oetf_en  <<1));

    Wr(GAMUT_MAP1_MATRIX_COEF0 , ((matrix_coef[0][0]&0x1fff)<<16)|
                                 ((matrix_coef[0][1]&0x1fff)<< 0));
    Wr(GAMUT_MAP1_MATRIX_COEF1 , ((matrix_coef[0][2]&0x1fff)<<16)|
                                 ((matrix_coef[1][0]&0x1fff)<< 0));
    Wr(GAMUT_MAP1_MATRIX_COEF2 , ((matrix_coef[1][1]&0x1fff)<<16)|
                                 ((matrix_coef[1][2]&0x1fff)<< 0));
    Wr(GAMUT_MAP1_MATRIX_COEF3 , ((matrix_coef[2][0]&0x1fff)<<16)|
                                 ((matrix_coef[2][1]&0x1fff)<< 0));
    Wr(GAMUT_MAP1_MATRIX_COEF4 , ((matrix_3x3mtrx_rs&0x1fff)<<16)|
                                 ((matrix_coef[2][2]&0x1fff)<< 0));

    Wr(GAMUT_MAP1_MATRIX_OFST_0,((ofst_oup[0]&0xffff)<<16)| (ofst_inp[0]&0xffff));
    Wr(GAMUT_MAP1_MATRIX_OFST_1,((ofst_oup[1]&0xffff)<<16)| (ofst_inp[1]&0xffff));
    Wr(GAMUT_MAP1_MATRIX_OFST_2,((ofst_oup[2]&0xffff)<<16)| (ofst_inp[2]&0xffff));
    //Wr(GAMUT_MAP1_MATRIX_OFST_1, ((1<<19)>>5)<<16);
    //Wr(GAMUT_MAP1_MATRIX_OFST_2, ((1<<19)>>5)<<16);
}

void set_lut3d_gamut(MNT_LUT3D_t *mnt_vpu_lut3d)
{
    int Lut3DTable[9*9*9*3] = {};//todo

    //lut3d
    printf("lut3d_en: %d\n",mnt_vpu_lut3d->lut3d_en);
    vpp_lut3d_table_init(Lut3DTable,10); //bitdepth=10

    vpp_set_lut3d(
        mnt_vpu_lut3d->lut3d_en   ,
        mnt_vpu_lut3d->lut3d_load ,
        Lut3DTable                ,
        mnt_vpu_lut3d->lut3d_check,
        mnt_vpu_lut3d->lut3d_mode  
    );

    //gamut_map1
    set_gamut_map1(mnt_vpu_lut3d->gamut_en);
}

