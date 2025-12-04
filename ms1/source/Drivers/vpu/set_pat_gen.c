#include "set_pat_gen.h"

// mode 0: fix_val  1: color bar   2:white   3:dot_grid  4.cross_bar 5:xxxx
void set_pat_gen(PATGEN_TMN_t output_type, uint8_t mode){
    uint16_t hsize,vsize,colorbar_start;
    switch(output_type) {
        case PATGEN_TMN_256X144:
            hsize = 256;vsize = 144;
            Wr( VPU_TOP_TOTAL_SIZE   ,(255+100) <<16| //reg_total_hsize
                                      (143+100)     );//reg_total_vsize
            Wr( VPU_TOP_DE_PX_SCOPE  ,(255+50 )<<16 | //reg_de_px_end
                                      (0  +50 )     );//reg_de_px_bgn
            Wr( VPU_TOP_DE_LN_SCOPE_E,(143+50 )<<16 | //reg_de_ln_end
                                      (0  +50 )     );//reg_de_ln_bgn

            Wr( VPU_TOP_HS_PX_SCOPE  , 31      <<16 | //reg_hs_px_end
                                       15           );//reg_hs_px_bgn
            Wr( VPU_TOP_VS_LN_SCOPE_E, 12      <<16 | //reg_hs_ln_end
                                       10           );//reg_hs_ln_bgn
            Wr( VPU_TOP_VS_PX_SCOPE_E, 31      <<16 | //reg_vs_px_end
                                       15           );//reg_vs_px_bgn
            break;
        case PATGEN_TMN_1920X32:
            hsize = 1920;vsize = 32;
            Wr( VPU_TOP_TOTAL_SIZE   ,(1920+100) <<16| //reg_total_hsize
                                      (32  +50 )     );//reg_total_vsize
            Wr( VPU_TOP_DE_PX_SCOPE  ,(1919+50  )<<16| //reg_de_px_end
                                      (0   +50  )    );//reg_de_px_bgn
            Wr( VPU_TOP_DE_LN_SCOPE_E,(31  +50  )<<16| //reg_de_ln_end
                                      (0   +50  )    );//reg_de_ln_bgn

            Wr( VPU_TOP_HS_PX_SCOPE  , 31      <<16 | //reg_hs_px_end
                                       15           );//reg_hs_px_bgn
            Wr( VPU_TOP_VS_LN_SCOPE_E, 12      <<16 | //reg_hs_ln_end
                                       10           );//reg_hs_ln_bgn
            Wr( VPU_TOP_VS_PX_SCOPE_E, 31      <<16 | //reg_vs_px_end
                                       15           );//reg_vs_px_bgn
            break;

        case PATGEN_TMN_1920X1080:
            hsize = 1920;vsize = 1080;
            Wr( VPU_TOP_TOTAL_SIZE   ,(1920+100) <<16| //reg_total_hsize
                                      (1080+50 )     );//reg_total_vsize
            Wr( VPU_TOP_DE_PX_SCOPE  ,(1919+50  )<<16| //reg_de_px_end
                                      (0   +50  )    );//reg_de_px_bgn
            Wr( VPU_TOP_DE_LN_SCOPE_E,(1079+50  )<<16| //reg_de_ln_end
                                      (0   +50  )    );//reg_de_ln_bgn

            Wr( VPU_TOP_HS_PX_SCOPE  , 31      <<16 | //reg_hs_px_end
                                       15           );//reg_hs_px_bgn
            Wr( VPU_TOP_VS_LN_SCOPE_E, 12      <<16 | //reg_hs_ln_end
                                       10           );//reg_hs_ln_bgn
            Wr( VPU_TOP_VS_PX_SCOPE_E, 31      <<16 | //reg_vs_px_end
                                       15           );//reg_vs_px_bgn
            break;

        case PATGEN_TMN_2560X1440:
            hsize = 2560;vsize = 1440;
            Wr( VPU_TOP_TOTAL_SIZE   ,(2559+100) <<16| //reg_total_hsize
                                      (1439+100)     );//reg_total_vsize
            Wr( VPU_TOP_DE_PX_SCOPE  ,(2559+50 )<<16 | //reg_de_px_end
                                      (0   +50 )     );//reg_de_px_bgn
            Wr( VPU_TOP_DE_LN_SCOPE_E,(1439+50 )<<16 | //reg_de_ln_end
                                      (0   +50 )     );//reg_de_ln_bgn

            Wr( VPU_TOP_HS_PX_SCOPE  , 31      <<16 | //reg_hs_px_end
                                       15           );//reg_hs_px_bgn
            Wr( VPU_TOP_VS_LN_SCOPE_E, 12      <<16 | //reg_hs_ln_end
                                       10           );//reg_hs_ln_bgn
            Wr( VPU_TOP_VS_PX_SCOPE_E, 31      <<16 | //reg_vs_px_end
                                       15           );//reg_vs_px_bgn
            break;
   }

    colorbar_start = hsize==1920 ? 148 :
                     hsize==1280 ? 257 : 80;
    if(mode==0){//fix value
        Wr(VPU_TOP_BIST_MODE,  0   );
        Wr(VPU_TOP_BIST_PSTRT, colorbar_start);
        Wr(VPU_TOP_BIST_SIZE ,(hsize/8<<16) | vsize);
    }else if(mode==1){// color bar 
        Wr(VPU_TOP_BIST_MODE,  1   );
        Wr(VPU_TOP_BIST_PSTRT, colorbar_start);
        Wr(VPU_TOP_BIST_SIZE ,(hsize/8<<16) | vsize);
    }else if(mode==2){ //todo!!
    }else if(mode==3){ //todo!!
    }else if(mode==4){ //todo!!
    }else if(mode==5){ //todo!!
    }


    Wr( VPU_TOP_TIMING_CTRL  ,1 <<20         );//reg_timing_en    
    Wr( VPU_TOP_TIMING_CTRL  ,1 <<24         );//reg_dat_timing_en    
}// void set_pat_gen_timing()

