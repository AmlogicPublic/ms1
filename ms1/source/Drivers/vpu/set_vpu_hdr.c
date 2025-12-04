#include "set_vpu_hdr.h"

int32_t s2h_eotf_lut[148] = {
1032192, 163840, 199044, 219570, 234611, 247036, 255751, 264272, 270593, 
277939 , 282430, 292441, 299792, 307054, 313394, 318215, 323651, 328697, 
335722 , 344064, 348910, 354448, 360570, 364052, 367901, 372123, 376722, 
379268 , 384835, 391196, 395793, 399793, 404214, 409063, 411973, 414834, 
417919 , 421229, 424767, 427260, 429261, 431380, 433618, 435975, 438453, 
441054 , 443072, 444496, 445981, 447530, 449142, 450817, 452556, 454360, 
456229 , 458163, 459457, 460489, 461555, 462654, 463787, 464953, 466153, 
467387 , 468655, 469957, 471294, 472665, 474071, 475324, 476062, 476817, 
477590 , 478381, 479190, 480016, 480861, 481723, 482603, 483502, 484418, 
485353 , 486306, 487278, 488267, 489276, 490302, 491348, 491966, 492507, 
493057 , 493618, 494187, 494766, 495354, 495952, 496559, 497176, 497803, 
498439 , 499084, 499740, 500405, 501079, 501764, 502458, 503161, 503875, 
504598 , 505332, 506075, 506828, 507590, 508133, 508525, 508921, 509322, 
509728 , 510140, 510556, 510977, 511404, 511835, 512272, 512713, 513160, 
513611 , 514068, 514530, 514997, 515469, 515946, 516429, 516916, 517409, 
517907 , 518410, 518918, 519432, 519950, 520474, 521003, 521537, 522077, 
522622 , 523172, 523727, 524287
};

int16_t s2h_oetf_lut[149] = {
0, 3, 5, 8, 12, 19, 28, 41, 60, 67, 74, 80, 85, 96, 105, 113, 120, 134, 146, 157, 167, 184, 200, 214, 227, 250, 270, 288, 304, 332, 357, 380, 400, 435, 465, 492, 517, 559, 595, 628, 656, 706, 749, 787, 820, 850, 878, 903, 927, 949, 970, 989, 1008, 1042, 1073, 1102, 1129, 1154, 1177, 1199, 1219, 1258, 1292, 1324, 1354, 1381, 1407, 1431, 1453, 1495, 1533, 1568, 1600, 1630, 1657, 1683, 1708, 1753, 1794, 1831, 1865, 1897, 1926, 1954, 1980, 2028, 2071, 2110, 2146, 2179, 2210, 2239, 2267, 2317, 2361, 2402, 2440, 2474, 2506, 2536, 2564, 2616, 2662, 2704, 2742, 2778, 2810, 2841, 2870, 2922, 2969, 3011, 3050, 3086, 3119, 3150, 3179, 3231, 3278, 3321, 3360, 3396, 3429, 3459, 3488, 3540, 3587, 3629, 3668, 3703, 3736, 3766, 3795, 3821, 3846, 3870, 3892, 3913, 3934, 3953, 3971, 3989, 4006, 4022, 4038, 4053, 4068, 4082, 4095
};

int16_t s2h_ogain_lut0[149] = {
0x400,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x001,0x001,0x001,0x001,0x001,0x001,0x001,
0x002,0x002,0x002,0x002,0x002,0x003,0x003,0x003,0x003,
0x004,0x004,0x004,0x005,0x005,0x006,0x006,0x007,0x007,
0x008,0x009,0x00a,0x00a,0x00b,0x00c,0x00d,0x00f,0x010,
0x012,0x013,0x015,0x016,0x018,0x01b,0x01e,0x021,0x024,
0x027,0x02a,0x02d,0x030,0x035,0x03b,0x041,0x047,0x04d,
0x052,0x058,0x05e,0x069,0x075,0x080,0x08c,0x097,0x0a2,
0x0ad,0x0b8,0x0ce,0x0e4,0x0f9,0x10e,0x123,0x138,0x14c,
0x161,0x189,0x1af,0x1d5,0x1fb,0x21f,0x242,0x264,0x286,
0x2c7,0x304,0x33e,0x374,0x3a8,0x3d8,0x406,0x430,0x47c,
0x4be,0x4f5,0x522,0x546,0x561,0x574,0x580,0x585,0x583,
0x57b,0x56e,0x55c,0x546,0x52d,0x510,0x4f1,0x4d0,0x4ad,
0x48a,0x466,0x443,0x421,0x400

};


int16_t s2h_cgain_lut[65] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023    
};


int16_t h2s_ogain_lut0[149] = {
1130, 1130, 1141, 1143, 1146, 1148, 1148, 1148, 1148,
1148, 1148, 1148, 1148, 1148, 1148, 1148, 1148, 1148, 1148,
1148, 1148, 1148, 1148, 1148, 1148, 1148, 1148, 1148, 1148,
1148, 1148, 1148, 1148, 1148, 1148, 1148, 1164, 1259, 1325,
1371, 1406, 1454, 1486, 1509, 1529, 1540, 1553, 1561, 1567,
1574, 1581, 1585, 1588, 1582, 1569, 1556, 1547, 1541, 1533,
1526, 1521, 1513, 1506, 1500, 1494, 1491, 1488, 1485, 1483,
1479, 1475, 1474, 1470, 1470, 1468, 1469, 1466, 1467, 1464,
1465, 1463, 1461, 1462, 1463, 1461, 1460, 1417, 1347, 1291,
1242, 1199, 1164, 1134, 1078, 1030, 988 , 957 , 929 , 904 ,
882 , 860 , 797 , 747 , 704 , 669 , 650 , 650 , 650 , 650 ,
650 , 650 , 650 , 650 , 650 , 650 , 650 , 650 , 650 , 650 ,
650 , 650 , 650 , 650 , 650 , 650 , 650 , 650 , 650 , 650 ,
650 , 650 , 650 , 650 , 650 , 650 , 650 , 650 , 650 , 650 ,
650 , 650 , 650 , 650 , 650 , 650 , 650 , 650 , 650 , 650 
};



int32_t h2s_eotf_lut[148] = {
59392 , 66560 , 94208 , 110592, 121984, 132160, 138816, 146496,
151264, 156128, 161472, 168768, 175968, 182240, 186960, 192272, 197400, 200680,
208240, 215102, 220346, 226400, 231349, 235271, 239686, 244625, 247940, 250984,
258030, 264312, 269323, 275208, 280295, 284260, 288817, 294028, 297434, 300794,
304586, 308851, 312465, 315139, 318120, 321437, 325119, 328439, 330693, 333181,
335922, 338938, 342251, 344974, 346965, 349143, 351524, 354124, 356960, 360050,
361931, 363762, 365751, 367912, 370258, 372802, 375559, 377689, 379306, 381056,
382948, 384994, 387204, 389591, 392167, 394081, 395581, 397197, 398940, 400818,
402840, 405018, 407363, 409743, 411100, 412561, 414132, 415820, 417636, 419588,
421685, 423939, 426172, 427472, 428869, 430370, 431980, 433710, 435567, 437561,
439701, 441999, 443416, 444740, 446160, 447685, 449321, 451078, 452962, 454986,
457157, 459120, 460370, 461713, 463154, 464701, 466363, 468146, 470061, 472118,
474326, 475917, 477191, 478560, 480031, 481611, 483309, 485135, 487098, 489208,
491477, 492719, 494032, 495444, 496965, 498601, 500363, 502261, 504304, 506506,
508391, 509670, 511049, 512536, 514140, 515870, 517739, 519756, 521934, 524287
};

int16_t h2s_oetf_lut[149] = {
0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, //18
3,  3,  3,  3,  4,  4,  4,  5,  5,  6,  6,  7,  7,  8,  9,  10, 10, 11, 12, 13, //20
14, 16, 17, 18, 20, 21, 23, 24, 25, 26, 28, 29, 30, 31, 32, 33, 35, 36, 38, 39,
41, 42, 43, 46, 48, 50, 52, 54, 56, 58, 60, 63, 66, 69, 72, 74, 77, 79, 82, 86,
91,  95,  99,  102, 106, 109, 112, 119, 124, 130, 135, 140, 145, 150, 154, 163, 171, 178, 185, 192,
199, 205, 211, 223, 234, 244, 254, 264, 273, 281, 290, 306, 314, 335, 349, 362, 374, 386, 397, 419,
440, 459, 478, 496, 513, 529, 545, 575, 603, 630, 655, 679, 703, 725, 747, 768, 788, 807, 827, 845,
863, 881, 898, 915, 931, 947, 963, 979, 994, 1009, 1023
};

int16_t h2s_cgain_lut[65] = {
1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023,
1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023,
1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023,
1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1014, 998 ,
982 ,  966,  950,  934,  918,  902,  886,  870,  854,  838,
822 ,  806,  790,  774,  758,  742,  726,  710,  694,  678,
662 ,  646,  630,  614
};


AML_VM_REG aml_vm_reg;

void set_hdr2_v0(MNT_HDR_t *mnt_vpu_hdr)
{

char hdr2_sel      = mnt_vpu_hdr->hdr2_sel     ;
char hdr2_top_en   = mnt_vpu_hdr->hdr2_top_en  ;
char hdr2_only_mat = mnt_vpu_hdr->hdr2_only_mat;
char hdr2_fmt_cfg  = mnt_vpu_hdr->hdr2_fmt_cfg ;
char in_fmt        = mnt_vpu_hdr->in_fmt       ;
char rgb_out_en    = mnt_vpu_hdr->rgb_out_en   ;

int32_t eotf_lut[148];//20bit
int16_t oetf_lut[149];//10bit
int16_t ogain_lut[149];//16bit
int16_t cgain_lut[65];//12bit
int i=0;
char only_matrix=0;

int16_t rgb2ycbcr_ncl2020[15] =   {230,594,52,-125,-323,448,448,-412,-36,0,0,0,0,0,0 };
int16_t yrb2ycbcr_cl2020[15]  =   {876,0,0,-566,0,566,-902,902,0,-462,0,462,-521,521,0 };
int16_t ycbcr2rgb_ncl2020[15] =   {1197,0,1726,1197,-193,-669,1197,2202,0,0,0,0,0,0,0 };
int16_t ycbcr2yrb_cl2020[15]  =   {1197,0,0,1197,0,1163,1197,1851,0,1197,0,2011,1197,2271,0 };


char cgain_mode   = 1;//   
char gmut_mode    = 1;//   
char eo_enable    = 1;//   
char oe_enable    = 1;//   
char ogain_enable = 1;//   
char cgain_enable = 1;//


  printf("hdr2_fmt    =%x\n",hdr2_fmt_cfg);
  printf("hdr2_top_en =%x\n",hdr2_top_en );

  only_matrix = hdr2_only_mat;
  if(hdr2_top_en){
    only_matrix = 0;
    eo_enable    = 1;   
    oe_enable    = 1;   
    ogain_enable = 1;   
    cgain_enable = 0; 

    if(hdr2_fmt_cfg==0){ 

        for(i=0;i<149;i++){
            oetf_lut[i]  = s2h_oetf_lut[i]>>2;
            ogain_lut[i] = s2h_ogain_lut0[i];
            if(i<148) eotf_lut[i] = s2h_eotf_lut[i];
            if(i<65)  cgain_lut[i] = s2h_cgain_lut[i];
        }
        
         gmut_mode    = 1;  
         cgain_mode = 1;
    }
    else if(hdr2_fmt_cfg == 1){

        for(i=0;i<149;i++){
            oetf_lut[i]   = h2s_oetf_lut[i];
            ogain_lut[i]  = h2s_ogain_lut0[i];
            if(i<148) eotf_lut[i] = h2s_eotf_lut[i];
            if(i<65)  cgain_lut[i] = h2s_cgain_lut[i];
        }

         gmut_mode    = 1; 
         cgain_mode = 0;
    }


    Wr(HDR2_CTRL,  0            <<24 |
                   rgb_out_en   <<17 | 
                   0            <<16 |
                   hdr2_top_en  <<13 |
                   cgain_mode   <<12 |
                   gmut_mode    << 6 |
                   in_fmt       << 4 |
                   eo_enable    << 3 |
                   oe_enable    << 2 |
                   ogain_enable << 1 |
                   cgain_enable << 0 );

    set_hdr2_mtrix(0,hdr2_sel,!in_fmt,in_fmt,rgb_out_en);

    Wr( EOTF_LUT_ADDR_PORT , 0x0);
    for(i=0;i<148;i++){
            Wr(EOTF_LUT_DATA_PORT,eotf_lut[i]);
    }
    
    Wr( OETF_LUT_ADDR_PORT , 0x0);
    for(i=0;i<74;i++){
        Wr(OETF_LUT_DATA_PORT,((oetf_lut[i*2+1])<<16)+(oetf_lut[i*2]));
    }
    Wr(OETF_LUT_DATA_PORT,oetf_lut[148]);

    Wr( OGAIN_LUT_ADDR_PORT , 0x0);
    for(i=0;i<74;i++){
        Wr(OGAIN_LUT_DATA_PORT,(ogain_lut[i*2+1]<<16)+ogain_lut[i*2]);
    }
    Wr(OGAIN_LUT_DATA_PORT,ogain_lut[148]);

    Wr( CGAIN_LUT_ADDR_PORT , 0x0);
    for(i=0;i<32;i++){
        Wr(CGAIN_LUT_DATA_PORT,(cgain_lut[i*2+1]<<16)+cgain_lut[i*2]);
    }
    Wr(CGAIN_LUT_DATA_PORT,cgain_lut[64]);

    if(hdr2_fmt_cfg == 0) {
        sdr2hdr_regs_set(&aml_vm_reg);
    }
    else if(hdr2_fmt_cfg == 1) {
        hdr2sdr_regs_set(&aml_vm_reg);
    }
    hdr2_reg_sys_wr(&aml_vm_reg);

  }  // if hdr2_top_en
  else if(hdr2_only_mat){ //hdr2 used as matrix
      set_hdr2_mtrix(1,hdr2_sel,!in_fmt,in_fmt,0);
  }

    Wr_reg_bits(HDR2_CTRL, only_matrix,16,1);
    Wr_reg_bits(HDR2_CTRL, hdr2_top_en,13,1);

}



void set_hdr2_mtrix(
         char only_matrix   , // hdr2 used as matrix
         char mat_sel       , //
         char rgb2yuv_in    , // in  matrix: 1:rgb to yuv 0 :yuv to rgb
         char hdr2_fmt      , // in  matrix: yuv to rgb
         char out_matrix_fmt  // out matrix: rgb to yuv
)
{

    ///// matrix coef 
    int16_t rgb2yuvpre[3]   = {0, 0, 0};
    int16_t rgb2yuvpos[3]   = {64, 512, 512};
    int16_t yuv2rgbpre[3]   = {-64, -512, -512};
    int16_t yuv2rgbpos[3]   = {0, 0, 0};
    
    //matrix coef BT709
    int16_t yuv2rgbmat[15]        =   {1197, 0, 0, 1197, 1851, 0, 1197, 0, 1163, 1197, 2271, 0, 1197, 0, 2011 };
    int16_t rgb2ycbcr[15]         =   {230,594,52,-125,-323,448,448,-412,-36,0,0,0,0,0,0};
    int16_t rgb2ycbcr_ncl2020[15] =   {230,594,52,-125,-323,448,448,-412,-36,0,0,0,0,0,0 };
    int16_t rgb2ycbcr_709[15]     =   {186, 627, 63, -103, -345, 448, 448, -407, -41, 0, 0, 0, 0, 0, 0 };
    int16_t ycbcr2rgb[15]         =   {1197,0,1726,1197,-193,-669,1197,2202,0,0,0,0,0,0,0};
    int16_t yrb2ycbcr_cl2020[15]  =   {876,0,0,-566,0,566,-902,902,0,-462,0,462,-521,521,0 };
    int16_t ycbcr2rgb_ncl2020[15] =   {1197,0,1726,1197,-193,-669,1197,2202,0,0,0,0,0,0,0 };
    int16_t ycbcr2yrb_cl2020[15]  =   {1197,0,0,1197,0,1163,1197,1851,0,1197,0,2011,1197,2271,0 };

    int i;
    char mat_conv_en;
    int16_t pre_offset[3],post_offset[3];
    int16_t mat_coef[15];
    int8_t clip_enable[3]   = {0,0,0};
    int8_t hilt_color[3]    = {0,0,0};
    char yuv2rgb_in=!rgb2yuv_in;
 
   if( yuv2rgb_in )
   {
     mat_conv_en     = 1;
     for(i = 0;i<3;i++){
     pre_offset[i]          = yuv2rgbpre[i];
     post_offset[i]         = yuv2rgbpos[i];
     }

     for(i = 0;i<15;i++){
     //mat_coef[i] = ycbcr2rgb[i];
     mat_coef[i] = only_matrix ? ycbcr2rgb[i] : 
                   ((hdr2_fmt==2) ? yuv2rgbmat[i] : ycbcr2rgb_ncl2020[i]);
     }

   }
   else if(rgb2yuv_in)
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

   Wr(HDR2_MATRIXI_COEF00_01, (mat_coef[0*3+0]<<16)|(mat_coef[0*3+1]&0x1FFF)  );
   Wr(HDR2_MATRIXI_COEF02_10, (mat_coef[0*3+2]<<16)|(mat_coef[1*3+0]&0x1FFF)  );
   Wr(HDR2_MATRIXI_COEF11_12, (mat_coef[1*3+1]<<16)|(mat_coef[1*3+2]&0x1FFF)  );
   Wr(HDR2_MATRIXI_COEF20_21, (mat_coef[2*3+0]<<16)|(mat_coef[2*3+1]&0x1FFF)  );
   Wr(HDR2_MATRIXI_COEF22   ,  mat_coef[2*3+2]);
   Wr(HDR2_MATRIXI_OFFSET0_1    , (post_offset[0]<<16)|(post_offset[1]&0xFFF) );
   Wr(HDR2_MATRIXI_OFFSET2      ,  post_offset[2]  );
   Wr(HDR2_MATRIXI_PRE_OFFSET0_1, (pre_offset[0]<<16)|(pre_offset[1]&0xFFF) );
   Wr(HDR2_MATRIXI_PRE_OFFSET2  ,  pre_offset[2] );
   Wr(HDR2_MATRIXI_EN_CTRL,mat_conv_en);

   if(!only_matrix){
       if( !out_matrix_fmt )
       {
         mat_conv_en     = 1;
         for(i = 0;i<3;i++){
         pre_offset[i]          = rgb2yuvpre[i];
         post_offset[i]         = rgb2yuvpos[i];
         }
       
         for(i = 0;i<15;i++){
         mat_coef[i] = (hdr2_fmt==0) ? rgb2ycbcr_ncl2020[i] : rgb2ycbcr_709[i];
         }
       }
       else if(out_matrix_fmt)
       {
         mat_conv_en     = 0;
       } 
       
       Wr(HDR2_MATRIXO_COEF00_01, (mat_coef[0*3+0]<<16)|(mat_coef[0*3+1]&0x1FFF)  );
       Wr(HDR2_MATRIXO_COEF02_10, (mat_coef[0*3+2]<<16)|(mat_coef[1*3+0]&0x1FFF)  );
       Wr(HDR2_MATRIXO_COEF11_12, (mat_coef[1*3+1]<<16)|(mat_coef[1*3+2]&0x1FFF)  );
       Wr(HDR2_MATRIXO_COEF20_21, (mat_coef[2*3+0]<<16)|(mat_coef[2*3+1]&0x1FFF)  );
       Wr(HDR2_MATRIXO_COEF22   ,  mat_coef[2*3+2]);
       Wr(HDR2_MATRIXO_OFFSET0_1    , (post_offset[0]<<16)|(post_offset[1]&0xFFF) );
       Wr(HDR2_MATRIXO_OFFSET2      ,  post_offset[2]  );
       Wr(HDR2_MATRIXO_PRE_OFFSET0_1, (pre_offset[0]<<16)|(pre_offset[1]&0xFFF) );
       Wr(HDR2_MATRIXO_PRE_OFFSET2  ,  pre_offset[2] );
       Wr(HDR2_CGAIN_OFFT, (post_offset[2]<<16) | post_offset[1]);
       Wr(HDR2_MATRIXO_EN_CTRL,mat_conv_en);
   }

   Wr_reg_bits(HDR2_CTRL,only_matrix, 16,1);// hdr2 used as matrix

}

void  read_hdr_hist(int64_t hdr_hist[128], uint32_t hist_rd_reg)
{
    int  hist_cnt; 
    
    for(hist_cnt = 0; hist_cnt < 128; hist_cnt ++) {
        hdr_hist[hist_cnt] = Rd(hist_rd_reg);
        printf("hdr_hist[%0d]: %x\n", hist_cnt, hdr_hist[hist_cnt]);
    }
}

void  sdr2hdr_regs_set(AML_VM_REG *aml_vm_reg)
{
aml_vm_reg->reg_adpscl_shift1                     = 13  ;
aml_vm_reg->reg_adpscl_shift0                     = 12  ;
aml_vm_reg->reg_cgain_oft[0]                      = 0   ;
aml_vm_reg->reg_cgain_oft[1]                      = 512 ;
aml_vm_reg->reg_cgain_oft[2]                      = 512 ;
aml_vm_reg->reg_c_gain_lim_coef[0]                = 920 ;
aml_vm_reg->reg_c_gain_lim_coef[1]                = 2376;
aml_vm_reg->reg_c_gain_lim_coef[2]                = 208 ;
aml_vm_reg->reg_sel_opt                           = 1   ;
aml_vm_reg->reg_c_gain_lim_maxrgb                 = 1024;
aml_vm_reg->reg_adpscl1_sft                       = 5   ;
aml_vm_reg->reg_ogain_blend                       = 0   ;
aml_vm_reg->reg_adpscl_sel_opt                    = 1   ;
aml_vm_reg->reg_adpscl_max                        = 63  ;
aml_vm_reg->reg_adpscl_clip_en                    = 0   ;
aml_vm_reg->reg_adpscl_bypass[0]                  = 0   ;
aml_vm_reg->reg_adpscl_bypass[1]                  = 0   ;
aml_vm_reg->reg_adpscl_bypass[2]                  = 0   ;
aml_vm_reg->reg_adpscl1_mode                      = 1   ;
aml_vm_reg->reg_adpscl_mode                       = 1   ;
aml_vm_reg->reg_adpscl_alpha[0]                   = 4096;
aml_vm_reg->reg_adpscl_alpha[1]                   = 4096;
aml_vm_reg->reg_adpscl_alpha[2]                   = 4096;
aml_vm_reg->reg_adpscl_shift2[0]                  = 10  ;
aml_vm_reg->reg_adpscl_shift2[1]                  = 7   ;
aml_vm_reg->reg_adpscl_shift2[2]                  = 12  ;
aml_vm_reg->reg_adpscl_beta_s[0]                  = 0   ;
aml_vm_reg->reg_adpscl_beta_s[1]                  = 0   ;
aml_vm_reg->reg_adpscl_beta_s[2]                  = 0   ;
aml_vm_reg->reg_adpscl_beta[0]                    = 1032192;
aml_vm_reg->reg_adpscl_beta[1]                    = 1032192;
aml_vm_reg->reg_adpscl_beta[2]                    = 1032192;
aml_vm_reg->reg_adpscl_ys_coef[0]                 = 1024;
aml_vm_reg->reg_adpscl_ys_coef[1]                 = 1024;
aml_vm_reg->reg_adpscl_ys_coef[2]                 = 1024;
aml_vm_reg->reg_new_mode                          = 0   ;
aml_vm_reg->reg_gmut_shift                        = 12  ;
aml_vm_reg->reg_gmut_coef[0][0]                   = 1285;
aml_vm_reg->reg_gmut_coef[0][1]                   = 674 ;
aml_vm_reg->reg_gmut_coef[0][2]                   = 89  ;
aml_vm_reg->reg_gmut_coef[1][0]                   = 142 ;
aml_vm_reg->reg_gmut_coef[1][1]                   = 1883;
aml_vm_reg->reg_gmut_coef[1][2]                   = 23  ;
aml_vm_reg->reg_gmut_coef[2][0]                   = 34  ;
aml_vm_reg->reg_gmut_coef[2][1]                   = 180 ;
aml_vm_reg->reg_gmut_coef[2][2]                   = 1834;
aml_vm_reg->reg_matrixi_en_ctrl                   = 1   ;//cmodel not need,but rtl need
aml_vm_reg->reg_matrixo_en_ctrl                   = 1   ;//
aml_vm_reg->reg_vcbus_rd_idx                      = 0   ;
aml_vm_reg->reg_gclk_ctrl0                        = 0   ;
aml_vm_reg->reg_piecewise_mode                    = 0   ;
aml_vm_reg->reg_hist_win_mode                     = 1   ;
aml_vm_reg->reg_maxRGB_rshift                     = 0   ;
aml_vm_reg->reg_maxRGB_sel                        = 0   ;
aml_vm_reg->reg_hdr2_gm_comp_en                   = 0;
aml_vm_reg->reg_hdr_comp_ofst_r                   = 85900 ;
aml_vm_reg->reg_hdr_comp_ofst_g                   = 85900 ;
aml_vm_reg->reg_hdr_comp_ofst_b                   = 85900 ;
aml_vm_reg->reg_hdr_comp_min_r                    = 510025;
aml_vm_reg->reg_hdr_comp_min_g                    = 472965;
aml_vm_reg->reg_hdr_comp_min_b                    = 467019;
aml_vm_reg->reg_hdr_comp_rat_r                    = 152108;
aml_vm_reg->reg_hdr_comp_rat_g                    = 472965;
aml_vm_reg->reg_hdr_comp_rat_b                    = 467019;
}


void hdr2sdr_regs_set(AML_VM_REG *aml_vm_reg)
{
aml_vm_reg->reg_adpscl_shift1                     = 6   ;
aml_vm_reg->reg_adpscl_shift0                     = 10  ;
aml_vm_reg->reg_cgain_oft[0]                      = 0   ;
aml_vm_reg->reg_cgain_oft[1]                      = 512 ;
aml_vm_reg->reg_cgain_oft[2]                      = 512 ;
aml_vm_reg->reg_c_gain_lim_coef[0]                = 920 ;
aml_vm_reg->reg_c_gain_lim_coef[1]                = 2376;
aml_vm_reg->reg_c_gain_lim_coef[2]                = 208 ;
aml_vm_reg->reg_sel_opt                           = 1   ;
aml_vm_reg->reg_c_gain_lim_maxrgb                 = 1023;
aml_vm_reg->reg_adpscl1_sft                       = 5   ;
aml_vm_reg->reg_ogain_blend                       = 0   ;
aml_vm_reg->reg_adpscl_sel_opt                    = 1   ;
aml_vm_reg->reg_adpscl_max                        = 63  ;
aml_vm_reg->reg_adpscl_clip_en                    = 0   ;
aml_vm_reg->reg_adpscl_bypass[0]                  = 0   ;
aml_vm_reg->reg_adpscl_bypass[1]                  = 0   ;
aml_vm_reg->reg_adpscl_bypass[2]                  = 0   ;
aml_vm_reg->reg_adpscl1_mode                      = 1   ;
aml_vm_reg->reg_adpscl_mode                       = 0   ;
aml_vm_reg->reg_adpscl_alpha[0]                   = 1024;
aml_vm_reg->reg_adpscl_alpha[1]                   = 1024;
aml_vm_reg->reg_adpscl_alpha[2]                   = 1024;
aml_vm_reg->reg_adpscl_shift2[0]                  = 10  ;
aml_vm_reg->reg_adpscl_shift2[1]                  = 6   ;
aml_vm_reg->reg_adpscl_shift2[2]                  = 10  ;
aml_vm_reg->reg_adpscl_beta_s[0]                  = 0   ;
aml_vm_reg->reg_adpscl_beta_s[1]                  = 0   ;
aml_vm_reg->reg_adpscl_beta_s[2]                  = 0   ;
aml_vm_reg->reg_adpscl_beta[0]                    = 0   ;
aml_vm_reg->reg_adpscl_beta[1]                    = 0   ;
aml_vm_reg->reg_adpscl_beta[2]                    = 0   ;
aml_vm_reg->reg_adpscl_ys_coef[0]                 = 269 ;
aml_vm_reg->reg_adpscl_ys_coef[1]                 = 694 ;
aml_vm_reg->reg_adpscl_ys_coef[2]                 = 61  ;
aml_vm_reg->reg_new_mode                          = 0   ;
aml_vm_reg->reg_gmut_shift                        = 8   ;
aml_vm_reg->reg_gmut_coef[0][0]                   = 425 ;
aml_vm_reg->reg_gmut_coef[0][1]                   = -150;
aml_vm_reg->reg_gmut_coef[0][2]                   = -18 ;
aml_vm_reg->reg_gmut_coef[1][0]                   = -31 ;
aml_vm_reg->reg_gmut_coef[1][1]                   = 290 ;
aml_vm_reg->reg_gmut_coef[1][2]                   = -2  ;
aml_vm_reg->reg_gmut_coef[2][0]                   = -5  ;
aml_vm_reg->reg_gmut_coef[2][1]                   = -25 ;
aml_vm_reg->reg_gmut_coef[2][2]                   = 286 ;
aml_vm_reg->reg_matrixi_en_ctrl                   = 1   ;
aml_vm_reg->reg_matrixo_en_ctrl                   = 1   ;
aml_vm_reg->reg_vcbus_rd_idx                      = 0   ;
aml_vm_reg->reg_gclk_ctrl0                        = 0   ;
aml_vm_reg->reg_piecewise_mode                    = 0   ;
aml_vm_reg->reg_hist_win_mode                     = 1   ;
aml_vm_reg->reg_maxRGB_rshift                     = 0   ;
aml_vm_reg->reg_maxRGB_sel                        = 0   ;
aml_vm_reg->reg_hdr2_gm_comp_en                   = 1;
aml_vm_reg->reg_hdr_comp_ofst_r                   = 85900 ;
aml_vm_reg->reg_hdr_comp_ofst_g                   = 85900 ;
aml_vm_reg->reg_hdr_comp_ofst_b                   = 85900 ;
aml_vm_reg->reg_hdr_comp_min_r                    = 510025;
aml_vm_reg->reg_hdr_comp_min_g                    = 472965;
aml_vm_reg->reg_hdr_comp_min_b                    = 467019;
aml_vm_reg->reg_hdr_comp_rat_r                    = 152108;
aml_vm_reg->reg_hdr_comp_rat_g                    = 472965;
aml_vm_reg->reg_hdr_comp_rat_b                    = 467019;
}


void hdr2_reg_sys_wr(AML_VM_REG *aml_vm_reg)
{

uint32_t reg_data;

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_c_gain_lim_coef[1]         & ((1L<<(27-16+1))-1)) << 16 ;
    reg_data |= (aml_vm_reg->reg_c_gain_lim_coef[0]         & ((1L<<(11-0+1))-1)) << 0 ;

    Wr(HDR2_CGAIN_COEF0, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_sel_opt                    & ((1L<<(31-31+1))-1)) << 31 ;
    reg_data |= (aml_vm_reg->reg_c_gain_lim_maxrgb          & ((1L<<(27-16+1))-1)) << 16 ;
    reg_data |= (aml_vm_reg->reg_c_gain_lim_coef[2]         & ((1L<<(11-0+1))-1)) << 0 ;

    Wr(HDR2_CGAIN_COEF1, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_adpscl1_sft                & ((1L<<(23-20+1))-1)) << 20 ;
    reg_data |= (aml_vm_reg->reg_ogain_blend                & ((1L<<(17-17+1))-1)) << 17 ;
    reg_data |= (aml_vm_reg->reg_adpscl_sel_opt             & ((1L<<(16-16+1))-1)) << 16 ;
    reg_data |= (aml_vm_reg->reg_adpscl_max                 & ((1L<<(13-8+1))-1)) << 8 ;
    reg_data |= (aml_vm_reg->reg_adpscl_clip_en             & ((1L<<(7-7+1))-1)) << 7 ;
    reg_data |= (aml_vm_reg->reg_adpscl_bypass[2]           & ((1L<<(6-6+1))-1)) << 6 ;
    reg_data |= (aml_vm_reg->reg_adpscl_bypass[1]           & ((1L<<(5-5+1))-1)) << 5 ;
    reg_data |= (aml_vm_reg->reg_adpscl_bypass[0]           & ((1L<<(4-4+1))-1)) << 4 ;
    reg_data |= (aml_vm_reg->reg_adpscl1_mode               & ((1L<<(3-2+1))-1)) << 2 ;
    reg_data |= (aml_vm_reg->reg_adpscl_mode                & ((1L<<(1-0+1))-1)) << 0 ;

    Wr(HDR2_ADPS_CTRL, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_adpscl_alpha[1]            & ((1L<<(29-16+1))-1)) << 16 ;
    reg_data |= (aml_vm_reg->reg_adpscl_alpha[0]            & ((1L<<(13-0+1))-1)) << 0 ;

    Wr(HDR2_ADPS_ALPHA0, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_adpscl_shift0              & ((1L<<(31-28+1))-1)) << 28 ;
    reg_data |= (aml_vm_reg->reg_adpscl_shift1              & ((1L<<(24-20+1))-1)) << 20 ;
    reg_data |= (aml_vm_reg->reg_adpscl_shift2[2]           & ((1L<<(19-16+1))-1)) << 16 ;
    reg_data |= (aml_vm_reg->reg_adpscl_alpha[2]            & ((1L<<(13-0+1))-1))  << 0 ;

    Wr(HDR2_ADPS_ALPHA1, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_adpscl_beta_s[0]           & ((1L<<(20-20+1))-1)) << 20 ;
    reg_data |= (aml_vm_reg->reg_adpscl_beta[0]             & ((1L<<(19-0+1))-1)) << 0 ;

    Wr(HDR2_ADPS_BETA0, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_adpscl_beta_s[1]           & ((1L<<(20-20+1))-1)) << 20 ;
    reg_data |= (aml_vm_reg->reg_adpscl_beta[1]             & ((1L<<(19-0+1))-1)) << 0 ;

    Wr(HDR2_ADPS_BETA1, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_adpscl_beta_s[2]           & ((1L<<(20-20+1))-1)) << 20 ;
    reg_data |= (aml_vm_reg->reg_adpscl_beta[2]             & ((1L<<(19-0+1))-1)) << 0 ;

    Wr(HDR2_ADPS_BETA2, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_adpscl_ys_coef[1]          & ((1L<<(27-16+1))-1)) << 16 ;
    reg_data |= (aml_vm_reg->reg_adpscl_ys_coef[0]          & ((1L<<(11-0+1))-1)) << 0 ;

    Wr(HDR2_ADPS_COEF0, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_adpscl_ys_coef[2]          & ((1L<<(11-0+1))-1)) << 0 ;

    Wr(HDR2_ADPS_COEF1, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_new_mode                   & ((1L<<(4-4+1))-1)) << 4 ;
    reg_data |= (aml_vm_reg->reg_gmut_shift                 & ((1L<<(3-0+1))-1)) << 0 ;

    Wr(HDR2_GMUT_CTRL, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_gmut_coef[0][1]            & ((1L<<(31-16+1))-1)) << 16 ;
    reg_data |= (aml_vm_reg->reg_gmut_coef[0][0]            & ((1L<<(15-0+1))-1)) << 0 ;

    Wr(HDR2_GMUT_COEF0, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_gmut_coef[1][0]            & ((1L<<(31-16+1))-1)) << 16 ;
    reg_data |= (aml_vm_reg->reg_gmut_coef[0][2]            & ((1L<<(15-0+1))-1)) << 0 ;

    Wr(HDR2_GMUT_COEF1, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_gmut_coef[1][2]            & ((1L<<(31-16+1))-1)) << 16 ;
    reg_data |= (aml_vm_reg->reg_gmut_coef[1][1]            & ((1L<<(15-0+1))-1)) << 0 ;

    Wr(HDR2_GMUT_COEF2, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_gmut_coef[2][1]            & ((1L<<(31-16+1))-1)) << 16 ;
    reg_data |= (aml_vm_reg->reg_gmut_coef[2][0]            & ((1L<<(15-0+1))-1)) << 0 ;

    Wr(HDR2_GMUT_COEF3, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_gmut_coef[2][2]            & ((1L<<(15-0+1))-1)) << 0 ;

    Wr(HDR2_GMUT_COEF4, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_hdr2_gm_comp_en            &0x1     )<<31; 
    reg_data |= (aml_vm_reg->reg_hdr_comp_ofst_r            &0xfffff )<<8 ;

    Wr(HDR2_GMUT_COMP0, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_hdr_comp_ofst_g            &0xfffff )<<8 ;

    Wr(HDR2_GMUT_COMP1, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_hdr_comp_ofst_b            &0xfffff )<<8 ;

    Wr(HDR2_GMUT_COMP2, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_hdr_comp_min_r             &0xfffff )<<8 ;

    Wr(HDR2_GMUT_COMP3, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_hdr_comp_min_g             &0xfffff )<<8 ;

    Wr(HDR2_GMUT_COMP4, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_hdr_comp_min_b             &0xfffff )<<8 ;

    Wr(HDR2_GMUT_COMP5, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_hdr_comp_rat_r             &0x3fffff )<<8 ;

    Wr(HDR2_GMUT_COMP6, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_hdr_comp_rat_g             &0x3fffff )<<8 ;

    Wr(HDR2_GMUT_COMP7, reg_data);

    reg_data = 0;
    reg_data |= (aml_vm_reg->reg_hdr_comp_rat_b             &0x3fffff )<<8 ;

    Wr(HDR2_GMUT_COMP8, reg_data);
}
