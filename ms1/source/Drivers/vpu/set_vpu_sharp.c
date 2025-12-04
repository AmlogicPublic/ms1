
#include "set_vpu_sharp.h"

void set_vpu_sharp(MNT_SHARP_t *mnt_vpu_sharp){
    
    printf("[set_vpu_sharp] start\n");

    uint16_t lc_mapping_hidx[13]={0};
    uint16_t lc_mapping_vidx[9]={0};
    uint8_t  lc_hnum = mnt_vpu_sharp->lc_mapping_hnum;
    uint8_t  lc_vnum = mnt_vpu_sharp->lc_mapping_vnum;

    uint8_t  lc_en   = (mnt_vpu_sharp->contrast_en>>2) & 0x1;
    printf("[set_vpu_sharp] lc_en0=%d\n",lc_en);

    uint8_t  dnlp_en = (mnt_vpu_sharp->contrast_en>>1) & 0x1;
    uint8_t  cc_en   = (mnt_vpu_sharp->contrast_en>>0) & 0x1;

    Wr_reg_bits(ENHANCE_EN_MODE, lc_en  , 1, 1);
    Wr_reg_bits(ENHANCE_EN_MODE, dnlp_en, 4, 1);
    Wr_reg_bits(ENHANCE_CC_EN  , cc_en  , 0, 1);

    if(mnt_vpu_sharp->lc_wr_lut_hw==0) 
      Wr_reg_bits(ENHANCE_CONTRAST_MISC, 1, 8, 1);
    else
      Wr_reg_bits(ENHANCE_CONTRAST_MISC, 0, 8, 1);
    

    if(lc_en) {
        lc_mapping_hidx[0]         = 0;
        lc_mapping_vidx[0]         = 0;
        lc_mapping_hidx[lc_hnum]   = mnt_vpu_sharp->lc_mapping_hsize;
        lc_mapping_vidx[lc_vnum]   = mnt_vpu_sharp->lc_mapping_vsize; 

        for(int i=1; i<lc_hnum; i++) {
            lc_mapping_hidx[i] = lc_mapping_hidx[i-1] + lc_mapping_hidx[lc_hnum]/lc_hnum;
        }
        for(int i=1; i<lc_vnum; i++) {
            lc_mapping_vidx[i] = lc_mapping_vidx[i-1] + lc_mapping_vidx[lc_vnum]/lc_vnum;
        }

        set_lc_mapping_lut(lc_hnum,lc_vnum,&lc_mapping_hidx[0],&lc_mapping_vidx[0]); 

        if((mnt_vpu_sharp->lc_wr_lut_en==0) || (mnt_vpu_sharp->lc_wr_lut_hw==0)) 
         update_sr_lcrams(lc_hnum, lc_vnum, mnt_vpu_sharp->curve_info, !(mnt_vpu_sharp->lc_wr_lut_en), 1);

        printf("[TEST.C] Initialize LC_MAPPING0:\n");
    } 

}


void set_lc_mapping_lut(uint8_t blk_hnum, uint8_t  blk_vnum, uint16_t blk_hidx[13], uint16_t blk_vidx[9])
{
   //int  addr_off = (0x40<<2) * (curve2_en & 0x1);//(0x2500<<2)*(slice2_en & 0x1);
   int rd_data;

   //blk_hnum/blk_vnum
   Wr_reg_bits(ENHANCE_LC_BLK_HV_NUM ,(blk_vnum&0x1f)|((blk_hnum&0x1f)<<8),0,12);
   
   //blk_hidx
   Wr(ENHANCE_LC_BLK_HIDX_0 ,blk_hidx[1] <<16 |(blk_hidx[0]));
   Wr(ENHANCE_LC_BLK_HIDX_1 ,blk_hidx[3] <<16 |(blk_hidx[2]));
   Wr(ENHANCE_LC_BLK_HIDX_2 ,blk_hidx[5] <<16 |(blk_hidx[4]));
   Wr(ENHANCE_LC_BLK_HIDX_3 ,blk_hidx[7] <<16 |(blk_hidx[6]));
   Wr(ENHANCE_LC_BLK_HIDX_4 ,blk_hidx[9] <<16 |(blk_hidx[8]));
   Wr(ENHANCE_LC_BLK_HIDX_5 ,blk_hidx[11]<<16 |(blk_hidx[10]));
   Wr_reg_bits(ENHANCE_LC_BLK_HIDX_F ,   blk_hidx[12],0,13);
     
   //blk_vidx
   Wr(ENHANCE_LC_BLK_VIDX_0   ,blk_vidx[1] <<16 |(blk_vidx[0] ));
   Wr(ENHANCE_LC_BLK_VIDX_1   ,blk_vidx[3] <<16 |(blk_vidx[2] ));
   Wr(ENHANCE_LC_BLK_VIDX_2   ,blk_vidx[5] <<16 |(blk_vidx[4] ));
   Wr(ENHANCE_LC_BLK_VIDX_3   ,blk_vidx[7] <<16 |(blk_vidx[6] ));
   Wr(ENHANCE_LC_BLK_VIDX_F   ,blk_vidx[8]);

   /*
   if(bBit12) {
       Wr_reg_bits(VPP_LC_YUV2RGB_OFSET  , 64 <<2  , 0  , 12); //reg_lc_yuv2rgb_ofset_0
       Wr_reg_bits(VPP_LC_YUV2RGB_OFSET  , 512<<2  , 12 , 12); //reg_lc_yuv2rgb_ofset_1
       Wr_reg_bits(VPP_LC_YUV2RGB_CLIP   , 0  <<2  , 0  , 12); //reg_lc_yuv2rgb_clip_0
       Wr_reg_bits(VPP_LC_YUV2RGB_CLIP   , 1023<<2 , 12 , 12); //reg_lc_yuv2rgb_clip_1
       Wr_reg_bits(VPP_LC_RGB2YUV_OFSET  , 64 <<2  , 0  , 12); //reg_lc_rgb2yuv_ofset_0
       Wr_reg_bits(VPP_LC_RGB2YUV_OFSET  , 512<<2  , 12 , 12); //reg_lc_rgb2yuv_ofset_1
       Wr_reg_bits(VPP_LC_RGB2YUV_CLIP   , 0  <<2  , 0  , 12); //reg_lc_rgb2yuv_clip_0
       Wr_reg_bits(VPP_LC_RGB2YUV_CLIP   , 1023<<2 , 12 , 12); //reg_lc_rgb2yuv_clip_1
   } else {
       //Wr_reg_bits(VPP_LC_YUV2RGB_OFSET , 64   , 0  , 12); //reg_lc_yuv2rgb_ofset_0
       //Wr_reg_bits(VPP_LC_YUV2RGB_OFSET , 512  , 12 , 12); //reg_lc_yuv2rgb_ofset_1
       //Wr_reg_bits(VPP_LC_YUV2RGB_CLIP  , 0    , 0  , 12); //reg_lc_yuv2rgb_clip_0
       //Wr_reg_bits(VPP_LC_YUV2RGB_CLIP  , 1023 , 12 , 12); //reg_lc_yuv2rgb_clip_1
       //Wr_reg_bits(VPP_LC_RGB2YUV_OFSET , 64   , 0  , 12); //reg_lc_rgb2yuv_ofset_0
       //Wr_reg_bits(VPP_LC_RGB2YUV_OFSET , 512  , 12 , 12); //reg_lc_rgb2yuv_ofset_1
       //Wr_reg_bits(VPP_LC_RGB2YUV_CLIP  , 0    , 0  , 12); //reg_lc_rgb2yuv_clip_0
       //Wr_reg_bits(VPP_LC_RGB2YUV_CLIP  , 1023 , 12 , 12); //reg_lc_rgb2yuv_clip_1
   }
   */

}


int update_sr_lcrams(uint8_t blk_hnum, uint8_t blk_vnum, int szCurveInfo[12*8*2], int bInit, int bCheck)
{
    int i;
    int dwTemp;
    int curCurveInfo[12*8*2];
    int rfalg;
    rfalg=0;
    
    //if init, use default value 
    for(i=0;i<blk_hnum*blk_vnum;i++) {
        curCurveInfo[2*i+0] = bInit ? (0|(0<<10)|(512<<20))       : szCurveInfo[2*i+0];
        curCurveInfo[2*i+1] = bInit ? (1023|(1023<<10)|(512<<20)) : szCurveInfo[2*i+1];
    }   
 
    //Fill LC Curve To Ram
    Wr(ENHANCE_LC_MAP_RAM_CTRL,1);
    Wr(ENHANCE_LC_MAP_RAM_ADDR,0);
    for(i=0;i<blk_hnum*blk_vnum;i++) {
        Wr(ENHANCE_LC_MAP_RAM_DATA,curCurveInfo[2*i+0]);
        Wr(ENHANCE_LC_MAP_RAM_DATA,curCurveInfo[2*i+1]);
    }
    Wr(ENHANCE_LC_MAP_RAM_CTRL,0);

    if(bCheck) {
        Wr(ENHANCE_LC_MAP_RAM_CTRL,1);
        Wr(ENHANCE_LC_MAP_RAM_ADDR,0|(1<<31));
        for(i=0;i<blk_hnum*blk_vnum;i++) {
            dwTemp = Rd(ENHANCE_LC_MAP_RAM_DATA);
            if(dwTemp != curCurveInfo[2*i+0]) {
                rfalg = (2*i+0) | (1<<31);
                return rfalg;
            }
            dwTemp = Rd(ENHANCE_LC_MAP_RAM_DATA);
            if(dwTemp != curCurveInfo[2*i+1]) {
                rfalg = (2*i+1) | (1<<31);
                return rfalg;
            }              
        }
        Wr(ENHANCE_LC_MAP_RAM_CTRL,0);
     }
     
     return rfalg;

}
