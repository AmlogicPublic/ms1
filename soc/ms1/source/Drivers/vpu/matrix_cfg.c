#include "matrix_cfg.h"

void matrix_cfg(int mat_sel,int yuv2rgb,int rgb2yuv)
{            
           
       
    ///// matrix coef 
	int32_t rgb2yuvpre[3]   = {0, 0, 0};
	int32_t rgb2yuvpos[3]   = {64, 512, 512};
	int32_t yuv2rgbpre[3]   = {-64, -512, -512};
	int32_t yuv2rgbpos[3]   = {0, 0, 0};

    //matrix coef BT709
    int32_t rgb2ycbcr[15]   =   {230,594,52,-125,-323,448,448,-412,-36,0,0,0,0,0,0};
    int32_t ycbcr2rgb[15]   =   {1197,0,1726,1197,-193,-669,1197,2202,0,0,0,0,0,0,0};
    //int ycbcr2rgb[15]   =   {1192,0,1634,1192,-401,-833,1192,2065,0,0,0,0,0,0,0};  

    //matrix coef BT2020
    //int rgb2ycbcr[15]   =   {876,0,0,-566,0,566,-902,902,0,-462,0,462,-521,521,0};
    //int ycbcr2rgb[15]   =   {1197,0,0,1170,0,1163,1170,1851,0,1170,0,2011,1170,2271,0};

    int     i;
    char    mat_conv_en;
    int32_t pre_offset[3],post_offset[3];
    int32_t mat_coef[15];
    int8_t  clip_enable[3] = {0,0,0};
    int8_t  hilt_color[3]  = {0,0,0};

    
   if( yuv2rgb )
   {
     mat_conv_en     = 1;
     for(i = 0;i<3;i++){
     pre_offset[i]          = yuv2rgbpre[i];
     post_offset[i]         = yuv2rgbpos[i];
     }

     for(i = 0;i<15;i++){
     mat_coef[i] = ycbcr2rgb[i];
     }

   }
   else if(rgb2yuv)
   {
     mat_conv_en = 1;
     for(i = 0;i<3;i++){
     pre_offset[i]          = rgb2yuvpre[i];
     post_offset[i]         = rgb2yuvpos[i];
     }
     for(i = 0;i<15;i++){
     mat_coef[i] = rgb2ycbcr[i];
     }

   }
   else
   {
     mat_conv_en = 0;
     for(i = 0;i<3;i++){
     pre_offset[i]          = 0;
     post_offset[i]         = 0;
     }
     for(i = 0;i<15;i++){
     mat_coef[i] = 0;
     }  
   }
  

    //osd1 path pq
   /*
    if(mat_sel == VPP_OSD1_PQ_MAT){
       Wr(OSD1_PQ_MATRIX_PRE_OFFSET0_1, (0x7c0<<16)|(0x600) );
       Wr(OSD1_PQ_MATRIX_OFFSET2  ,  0x600  )                 ;
       Wr(OSD1_PQ_MATRIX_COEF00_01, (0x400<<16)  );//contrast
       //Wr(OSD1_PQ_MATRIX_COEF00_01, (0x1f0<<16)  );//contrast
       Wr(OSD1_PQ_MATRIX_COEF02_10, 0x0  );
       Wr(OSD1_PQ_MATRIX_COEF11_12, (0x400<<16)  );
       Wr(OSD1_PQ_MATRIX_COEF20_21, 0x0  );
       Wr(OSD1_PQ_MATRIX_COEF22   , 0x400);        
       //Wr(OSD1_PQ_MATRIX_OFFSET0_1, (0x0400<<16)|(0x0200) )  ;//bit26:16 brigntness
       Wr(OSD1_PQ_MATRIX_OFFSET0_1, (0x0140<<16)|(0x0200) )  ;//bit26:16 brigntness
       Wr(OSD1_PQ_MATRIX_OFFSET2  , 0x0000  );
       Wr_reg_bits(OSD1_PQ_MATRIX_EN_CTRL,1,0,1);
       Wr_reg_bits(OSD1_PQ_MATRIX_CLIP,0,5,3);

   } */
}

