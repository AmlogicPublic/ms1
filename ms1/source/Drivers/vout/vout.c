#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "Drivers/vout/register.h"
//#include "Drivers/vout/c_stimulus.h"
#include "Drivers/vout/vout.h"
#include "Drivers/vout/vout_setting.h"
#include "Drivers/vpu/cfg_mnt_pps.h"

uint32_t vout_reg_addr[] = {
0x00000000, 0x00ff003f,
0x00000001, 0x0fffffff,
0x00000003, 0xffff00ff,
0x00000005, 0xffffffff,
0x00000006, 0x003fffff,
0x00000008, 0x00ff003f,
0x00000009, 0x0fffffff,
0x0000000a, 0xffff00ff,
0x0000000b, 0xffffffff,
0x0000000c, 0x003fffff,
0x00000040, 0xffffffff,
0x00000041, 0xffffffff,
0x00000042, 0xffffffff,
0x00000043, 0xffffffff,
0x00000044, 0xffffffff,
0x00000045, 0xffffffff,
0x00000046, 0x00003ff0,
0x00000047, 0xffffffff,
0x00000048, 0xf7ffffff,
0x00000061, 0xffffffff,
0x00000062, 0xffffffff,
0x00000063, 0xffffffff,
0x00000064, 0x3fffffff,
0x00000065, 0xffffffff,
0x00000069, 0x0fffffff,
0x0000006a, 0x0fffffff,
0x0000006b, 0x000fffff,
0x0000006c, 0x013ff009,
0x0000006e, 0x00fff7ff,
0x0000006f, 0x1fffffff,
0x00000070, 0x03fff7ff,
0x00000071, 0x3fff0fff,
0x00000072, 0x00ffffff,
0x00000073, 0xffffffff,
0x00000074, 0xffffffff,
0x00000075, 0x3fffffff,
0x00000076, 0xffffffff,
0x00000077, 0x3f3f3f3f,
0x00000078, 0x003f3f3f,
0x00000079, 0x3f3f3f3f,
0x0000007a, 0x3f3f3f3f,
0x0000007b, 0x0fffffff,
0x0000007c, 0x1fffffff,
0x0000007d, 0x0fff7fff,
0x0000007e, 0x007fffff,
0x0000007f, 0xffffffff,
0x00000080, 0x0fffffff,
0x00000100, 0xffffffff,
0x00000101, 0xffffffff,
0x00000102, 0x0000ffff,
0x00000103, 0xffffffff,
0x00000104, 0xffffffff,
0x00000105, 0x3fffffff,
0x00000106, 0x3fffffff,
0x00000107, 0x3fffffff,
0x00000108, 0x0003ffff,
0x00000109, 0x8fffffff,
0x0000010a, 0x801fffff,
0x0000010b, 0xffffffff,
0x0000010c, 0xffffffff,
0x00000170, 0xffffffff,
0x00000171, 0xffffffeb,
0x00000172, 0xffffffff,
0x00000173, 0xffffffc0,
0x00000174, 0xffffffc0,
0x00000175, 0xffffffc0,
0x00000176, 0xffffffc0,
0x00000177, 0xffffffc0,
0x00000178, 0xffffffc0,
0x00000179, 0xffffffc0,
0x0000017a, 0xffffffc0,
0x0000017b, 0xffffffc0,
0x0000017c, 0xffffffc0,
0x0000017d, 0xffffffc0,
0x0000017e, 0xffffffc0,
0x0000017f, 0xffffffc0,
0x00000180, 0xffffffc0,
0x00000181, 0xffffffc0,
0x00000182, 0xffffffc0,
0x00000183, 0xffffffc0,
0x00000184, 0xffffffc0,
0x00000185, 0xffffffc0,
0x00000186, 0xffffffc0,
0x00000187, 0xffffffc0,
0x00000188, 0xffffffc0,
0x00000189, 0xffffffc0,
0x0000018a, 0xffffffc0,
0x0000018b, 0xffffffc0,
0x0000018c, 0xffffffc0,
0x0000018d, 0xffffffc0,
0x0000018e, 0xffffffc0,
0x0000018f, 0xffffffc0,
0x00000190, 0xffffffc0,
0x00000191, 0xffffffc0,
0x00000192, 0xfff80000,
0x00000193, 0xfffffff0,
0x00000194, 0xffffffc0,
0x00000195, 0xffffffc0,
0x00000196, 0xfffffff0,
0x00000197, 0xffffffc0,
0x00000198, 0xffffffc0,
0x00000199, 0xffffffc0,
0x0000019a, 0xffffffc0,
0x0000019b, 0xffffffc0,
0x0000019c, 0xffffffc0,
0x0000019d, 0xffffffc0,
0x0000019e, 0xffffffc0,
0x0000019f, 0xffffffc0,
0x000001a0, 0xffffffc0,
0x000001a1, 0xffffffc0,
0x000001a2, 0xffffffc0,
0x000001a3, 0xffffffc0,
0x000001a4, 0xffffffc0,
0x000001a5, 0xffffffc0,
0x000001a6, 0xffffffe0,
0x000001a7, 0xffffffc0,
0x000001a8, 0xffffffc0,
0x000001a9, 0xffffffc0,
0x00000200, 0xffffffff,
0x00000201, 0xffffffff,
0x00000202, 0xffffffff,
0x00000203, 0xffffffff,
0x00000204, 0xffffffff,
0x00000205, 0xffffffff,
0x00000206, 0xffffffff,
0x00000207, 0xdfff9bf7,
0x00000208, 0xffffffff,
0x00000209, 0xffffffff,
0x0000020a, 0xffffffff,
0x0000020b, 0xffffffff,
0x0000020c, 0xffffffff,
0x0000020d, 0xffffffff,
0x0000020e, 0x1fffffff,
0x0000020f, 0xffffffff,
0x00000210, 0xffffffff,
0x00000211, 0xffffffff,
0x00000212, 0xffffffff,
0x00000213, 0xffffffff,
0x00000214, 0xffffffff,
0x00000215, 0xffffffff,
0x00000216, 0xffffffff,
0x00000217, 0xffffffff,
0x00000218, 0xffffffff,
0x00000219, 0xffffffff,
0x0000021a, 0xffffffff,
0x0000021b, 0xffffffff,
0x0000021c, 0xffffffff,
0x0000021d, 0xffffffff,
0x0000021e, 0xffffffff,
0x0000021f, 0xffffffff,
0x00000220, 0xffffffff,
0x00000221, 0xffffffff,
0x00000222, 0x7fff80ff,
0x00000223, 0x3fffffff,
0x00000224, 0x0fffffff,
0x00000225, 0x3fffffff,
0x0000022d, 0x001fffff,
0x0000022e, 0x1fff1fff,
0x0000022f, 0x1fff1fff,
0x00000230, 0x1fff1fff,
0x00000231, 0x1fff1fff,
0x00000232, 0x1fff1fff,
0x00000233, 0x1fff1fff,
0x00000234, 0x1fff1fff,
0x00000235, 0x1fff1fff,
0x00000236, 0x1fff1fff,
0x00000237, 0x1fff1fff,
0x00000238, 0x1fff1fff,
0x00000239, 0x0001ffff,
0x0000023b, 0x9fff00ff,
0x0000023c, 0x00000007,
0x0000023d, 0x800fffff,
0x00000243, 0x03ffffff,
0x00000244, 0x03ffffff,
0x00000245, 0xffff7fff,
0x00000246, 0x03ffffff,
0x00000247, 0x03ffffff,
0x00000249, 0x3fffffff,
0x0000024a, 0x03ffffff,
0x0000024b, 0x03ffffff,
0x0000024e, 0x007fffff,
0x0000024f, 0x01ffffff,
0x00000250, 0xffffffff,
0x00000251, 0x07ffffff,
0x00000252, 0xffffffff,
0x00000253, 0xffff03ff,
0x00000254, 0xffffffff,
0x00000255, 0xffffffff,
0x00000256, 0xffffffff,
0x00000257, 0x03ffffff,
0x00000258, 0x03ffffff,
0x00000300, 0x00001ff3,
0x00000301, 0x3fffffff,
0x00000302, 0xffff00ff,
0x00000303, 0xff0fff7f,
0x00000304, 0x1fffffff,
0x00000305, 0xffff3fff,
0x00000306, 0x003fffff,
0x00000307, 0xffffffff,
0x00000308, 0xffffffff,
0x00000309, 0xffffffff,
0x0000030a, 0xffffffff,
0x0000030b, 0x1fffffff,
0x0000030c, 0x1fff1fff,
0x0000030d, 0xffffffff,
0x0000030e, 0xffffffff,
0x0000030f, 0xffffffff,
0x00000310, 0xffffffff,
0x00000311, 0xfff0ffff,
0x00000312, 0x1fff1fff,
0x00000313, 0x1fff1fff,
0x00000315, 0xffffffff,
0x00000316, 0x0fffffff,
0x00000317, 0x3fffffff,
0x00000318, 0x7ffff800,
0x00000319, 0x007fffff,
0x0000031a, 0xffffffff,
0x0000031b, 0xffffffff,
0x0000031c, 0xffffffff,
0x0000031d, 0xffffffff,
0x0000031e, 0x00007fff,
0x0000031f, 0x7ffc1fff,
0x00000320, 0x00007ff7,
0x00000321, 0xc03dffff,
0x00000322, 0xffffffff,
0x00000323, 0x3fffffff,
0x00000324, 0x00ffffff,
0x00000325, 0x3fffffff,
0x00000326, 0x3fffffff,
0x00000327, 0x007fffbf,
0x00000328, 0xffffffff,
0x00000329, 0xffff700f,
0x0000032a, 0x01ffffff,
0x0000032b, 0xffffffff,
0x0000032c, 0xfe3fffff,
0x0000032d, 0xffffffff,
0x0000032e, 0xffffffff,
0x0000032f, 0x00ffffff,
0x00000333, 0xffffffff,
0x00000334, 0xfc001fff,
0x00000335, 0xffffffff,
0x00000336, 0x1fffffff,
0x00000337, 0x0011ffff,
0x00000338, 0xffffffff,
0x00000339, 0xffffffff,
0x0000033a, 0xffffffff,
0x0000033b, 0xffffffff,
0x0000033c, 0xffffffff,
0x0000033d, 0xffffffff,
0x0000033e, 0xffffffff,
0x0000033f, 0xffffffff,
0x00000340, 0xffffffff,
0x00000341, 0xffffffff,
0x00000342, 0x00001fff,
0x00000343, 0x000fffff,
0x00000344, 0x000fffff,
0x00000347, 0xffffffff,
0x00000348, 0x00ffffff,
0x00000349, 0x3fffffff,
0x0000034a, 0x3fffffff,
0x0000034b, 0x3fffffff,
0x0000034c, 0x0100ffff,
0x0000034d, 0x3fffffff,
0x0000034e, 0x000003ff,
0x00000351, 0x00007fff,
0x00000352, 0x0003ffff,
0x00000353, 0x000fffff,
0x00000354, 0x001fffff,
0x00000400, 0x00007ff7,
0x00000401, 0xc03dffff,
0x00000402, 0x7ffc003f,
0x00000403, 0x8000000f,
0x00000404, 0xffffffff,
0x00000405, 0xffffffff,
0x00000406, 0xffffffff,
0x00000407, 0x1fff0000,
0x00000408, 0x1fff1fff,
0x00000409, 0x1fff1fff,
0x0000040a, 0x1fff1fff,
0x0000040b, 0x9fff1fff,
};


void vout_1920x32_free_run_cmpr_lvds(aml_lvds_cfg_t lvds_cfg)
{
    uint32_t data_tmp;
    uint32_t i, addr, data;
    uint8_t rd_setting;
    size_t len;

    Wr((VOUT_APB_BASE_ADDR+(0x8000)),0x8000);
    Wr((VOUT_APB_BASE_ADDR+(0x800c)),0x007f);

    //lvds
    //1x4
    if((lvds_cfg.pack_cntl&(1<<31)) && (lvds_cfg.pack_cntl&(1<<29))) {
        len = sizeof(vout_1920x32_free_run_4p_1x4_lvds_setting)/sizeof(vout_1920x32_free_run_4p_1x4_lvds_setting[0]);
        for (i=0;i<len/2;i=i+1)  {
            addr = vout_1920x32_free_run_4p_1x4_lvds_setting[i*2];
            data = vout_1920x32_free_run_4p_1x4_lvds_setting[i*2+1];
            // printf("vout wr test addr %x = %x\n", addr, data);
            Wr((VOUT_APB_BASE_ADDR+(addr<<2)),data);
        }
    //2x2
    } else if((lvds_cfg.pack_cntl&(1<<31)) && (lvds_cfg.pack_cntl&(1<<30))) {
        len = sizeof(vout_1920x32_free_run_4p_2x2_lvds_setting)/sizeof(vout_1920x32_free_run_4p_2x2_lvds_setting[0]);
        for (i=0;i<len/2;i=i+1)  {
            addr = vout_1920x32_free_run_4p_2x2_lvds_setting[i*2];
            data = vout_1920x32_free_run_4p_2x2_lvds_setting[i*2+1];
            // printf("vout wr test addr %x = %x\n", addr, data);
            Wr((VOUT_APB_BASE_ADDR+(addr<<2)),data);
        }
    //dual-port
    } else if(!(lvds_cfg.pack_cntl&(1<<31)) && (lvds_cfg.pack_cntl&(1<<6))) {
        len = sizeof(vout_1920x32_free_run_2plvds_setting)/sizeof(vout_1920x32_free_run_2plvds_setting[0]);
        for (i=0;i<len/2;i=i+1)  {
            addr = vout_1920x32_free_run_2plvds_setting[i*2];
            data = vout_1920x32_free_run_2plvds_setting[i*2+1];
            // printf("vout wr test addr %x = %x\n", addr, data);
            Wr((VOUT_APB_BASE_ADDR+(addr<<2)),data);
        }
    //1port
    } else {
        len = sizeof(vout_1920x32_free_run_1plvds_setting)/sizeof(vout_1920x32_free_run_1plvds_setting[0]);
        for (i=0;i<len/2;i=i+1)  {
            addr = vout_1920x32_free_run_1plvds_setting[i*2];
            data = vout_1920x32_free_run_1plvds_setting[i*2+1];
            // printf("vout wr test addr %x = %x\n", addr, data);
            Wr((VOUT_APB_BASE_ADDR+(addr<<2)),data);
        }
    }
    //pn swap
    if(lvds_cfg.pack_cntl & (1<<5)) {
        data = Rd(VOUT_APB_BASE_ADDR+(0x0109<<2));
        Wr((VOUT_APB_BASE_ADDR+(0x0109<<2)),data | (1<<5));
    }

    //vout, except lvds
    len = sizeof(vout_1920x32_free_run_cmpr_setting)/sizeof(vout_1920x32_free_run_cmpr_setting[0]);
    for (i=0;i<len/2;i=i+1)  {
      addr = vout_1920x32_free_run_cmpr_setting[i*2];
      data = vout_1920x32_free_run_cmpr_setting[i*2+1];
      // printf("vout wr test addr %x = %x\n", addr, data);
      Wr((VOUT_APB_BASE_ADDR+(addr<<2)),data);
    }
}

void vout_vga_ft_test()
{
    uint32_t i, addr, data;
    size_t len;

    Wr((VOUT_APB_BASE_ADDR+(0x8000)),0x8008);
    Wr((VOUT_APB_BASE_ADDR+(0x800c)),0x007f);

    len = sizeof(vout_vga_ft_setting)/sizeof(vout_vga_ft_setting[0]);
    for (i=0;i<len/2;i=i+1)  {
      addr = vout_vga_ft_setting[i*2];
      data = vout_vga_ft_setting[i*2+1];
     // printf("vout wr test addr %x = %x\n", addr, data);
      Wr((VOUT_APB_BASE_ADDR+(addr<<2)),data);
    }


}

void vout_test_reg()
{
     uint32_t i;
     uint32_t j;
     uint32_t data_tmp;
     uint32_t addr, width,mask;
     //set clk, reset
    Wr((VOUT_APB_BASE_ADDR+(0x8000)),0x8000);
    Wr((VOUT_APB_BASE_ADDR+(0x800c)),0x0077);
     //test all the reg wr and rd of vout
     for(i=0;i<280;i++){
       addr = vout_reg_addr[i*2+0]<<2;
       mask = vout_reg_addr[i*2+1];
       //all register write 0
       Wr(VOUT_APB_BASE_ADDR+addr,0);
       data_tmp = Rd(VOUT_APB_BASE_ADDR+addr);
       if ((data_tmp&mask) != 0) {
             printf("VOUT_ERROR vout reg test %x data %x\n", addr>>2, 0x00000000);
        }else{
             printf("RIGHT vout reg test %x data %x\n", addr>>2, 0x00000000);
        }

       //all register write 1
       Wr(VOUT_APB_BASE_ADDR+addr,0xffffffff);
       data_tmp = Rd(VOUT_APB_BASE_ADDR+addr);
       if (data_tmp != mask) {
             printf("VOUT_ERROR vout reg test %x data %x\n", addr>>2, 0xffffffff);
        }else{
             printf("RIGHT vout reg test %x data %x\n", addr>>2, 0xffffffff);
        }

        //all register write ha5a5a5a5
       Wr(VOUT_APB_BASE_ADDR+addr,0xa5a5a5a5);
       data_tmp = Rd(VOUT_APB_BASE_ADDR+addr);
       if (data_tmp != (mask&0xa5a5a5a5) ) {
             printf("VOUT_ERROR vout reg test %x data %x\n", addr>>2, (mask&0xa5a5a5a5));
        } else{
             printf("RIGHT vout reg test %x data %x\n", addr>>2, (mask&0xa5a5a5a5));
        }


        //all register write h5a5a5a5a
       Wr(VOUT_APB_BASE_ADDR+addr,0x5a5a5a5a);
       data_tmp = Rd(VOUT_APB_BASE_ADDR+addr);
       if (data_tmp != (mask&0x5a5a5a5a) ) {
             printf("VOUT_ERROR vout reg test %x data %x\n", addr>>2, (mask&0x5a5a5a5a));
        } 

    }

}

void vout_test_ram()
{
     uint32_t i;
     uint32_t j;
     uint32_t data_tmp;
     uint32_t addr, width;

     //set clk, reset
    Wr((VOUT_APB_BASE_ADDR+(0x8000)),0x8000);
    Wr((VOUT_APB_BASE_ADDR+(0x800c)),0x0077);

     for (i=0;i<3;i++) {
       Wr(VOUT_APB_BASE_ADDR+(0x601<<2),1<<(0+i));
       //ram0 289x8
       for (j=0;j<289;j++) {
         Wr(VOUT_APB_BASE_ADDR+((0x700+j)<<2),0x0);
       }
       for (j=0;j<289;j++) {
         data_tmp = Rd(VOUT_APB_BASE_ADDR+((0x700+j)<<2));
           if (data_tmp != 0) {
             printf("VOUT_ERROR vout OD RAM0 %x, test addr %x\n", i, j);
           }else{
             printf("RIGHT vout OD RAM0 %x, test addr %x \n", i, j);
           }

       }

       for (j=0;j<289;j++) {
         Wr(VOUT_APB_BASE_ADDR+((0x700+j)<<2),0xffffffff);
       }
       for (j=0;j<289;j++) {
         data_tmp = Rd(VOUT_APB_BASE_ADDR+((0x700+j)<<2));
           if (data_tmp != 0xff) {
             printf("VOUT_ERROR vout OD RAM0 %x, test addr %x \n", i, j);
           }else{
             printf("RIGHT vout OD RAM0 %x, test addr %x \n", i, j);
           }

       }
       //ram1,272x8
       Wr(VOUT_APB_BASE_ADDR+(0x601<<2),1<<((i+3)));
       for (j=0;j<272;j++) {
         Wr(VOUT_APB_BASE_ADDR+((0x700+j)<<2),0x0);
       }
       for (j=0;j<272;j++) {
         data_tmp = Rd(VOUT_APB_BASE_ADDR+((0x700+j)<<2));
           if (data_tmp != 0) {
             printf("VOUT_ERROR vout OD RAM1 %x, test addr %x\n", i, j);
           }else{
             printf("RIGHT vout OD RAM1 %x, test addr %x \n", i, j);
           }

       }

       for (j=0;j<272;j++) {
         Wr(VOUT_APB_BASE_ADDR+((0x700+j)<<2),0xffffffff);
       }
       for (j=0;j<272;j++) {
         data_tmp = Rd(VOUT_APB_BASE_ADDR+((0x700+j)<<2));
           if (data_tmp != 0xff) {
             printf("VOUT_ERROR vout OD RAM1 %x, test addr %x \n", i, j);
           }else{
             printf("RIGHT vout OD RAM1 %x, test addr %x \n", i, j);
           }

       }       

       //ram2 272x8
       Wr(VOUT_APB_BASE_ADDR+(0x601<<2),1<<(i+6));
       for (j=0;j<272;j++) {
         Wr(VOUT_APB_BASE_ADDR+((0x700+j)<<2),0x0);
       }
       for (j=0;j<272;j++) {
         data_tmp = Rd(VOUT_APB_BASE_ADDR+((0x700+j)<<2));
           if (data_tmp != 0) {
             printf("VOUT_ERROR vout OD RAM2 %x, test addr %x\n", i, j);
           }else{
             printf("RIGHT vout OD RAM2 %x, test addr %x \n", i, j);
           }

       }

       for (j=0;j<272;j++) {
         Wr(VOUT_APB_BASE_ADDR+((0x700+j)<<2),0xffffffff);
       }
       for (j=0;j<272;j++) {
         data_tmp = Rd(VOUT_APB_BASE_ADDR+((0x700+j)<<2));
           if (data_tmp != 0xff) {
             printf("VOUT_ERROR vout OD RAM2 %x, test addr %x \n", i, j);
           }else{
             printf("RIGHT vout OD RAM2 %x, test addr %x \n", i, j);
           }

       }
       //ram3 256x8
       Wr(VOUT_APB_BASE_ADDR+(0x601<<2),1<<(i+9));
       for (j=0;j<256;j++) {
         Wr(VOUT_APB_BASE_ADDR+((0x700+j)<<2),0x0);
       }
       for (j=0;j<256;j++) {
         data_tmp = Rd(VOUT_APB_BASE_ADDR+((0x700+j)<<2));
           if (data_tmp != 0) {
             printf("VOUT_ERROR vout OD RAM3 %x, test addr %x\n", i, j);
           }else{
             printf("RIGHT vout OD RAM3 %x, test addr %x \n", i, j);
           }

       }

       for (j=0;j<256;j++) {
         Wr(VOUT_APB_BASE_ADDR+((0x700+j)<<2),0xffffffff);
       }
       for (j=0;j<256;j++) {
         data_tmp = Rd(VOUT_APB_BASE_ADDR+((0x700+j)<<2));
           if (data_tmp != 0xff) {
             printf("VOUT_ERROR vout OD RAM3 %x, test addr %x \n", i, j);
           }else{
             printf("RIGHT vout OD RAM3 %x, test addr %x \n", i, j);
           }
       } 
     }
     Wr(VOUT_APB_BASE_ADDR+(0x601<<2),0);

     //dither
     for (i=0;i<1;i=i+1) {
       Wr(VOUT_APB_BASE_ADDR+(0x609<<2),1<<(i));
       for (j=0;j<512;j++) {
         Wr(VOUT_APB_BASE_ADDR+((0x700+j)<<2),0x0);
       }
       for (j=0;j<512;j++) {
         data_tmp = Rd(VOUT_APB_BASE_ADDR+((0x700+j)<<2));
           if (data_tmp != 0) {
             printf("VOUT_ERROR vout dither RAM %x, test addr %x\n", i, j);
           }else {
             printf("RIGHT vout dither RAM %x, test addr %x \n", i, j);
           }
       }
       for (j=0;j<512;j++) {
         Wr(VOUT_APB_BASE_ADDR+((0x700+j)<<2),0xffffffff);
       }
       for (j=0;j<512;j++) {
         data_tmp = Rd(VOUT_APB_BASE_ADDR+((0x700+j)<<2));
           if (data_tmp != 0xffffff) {
             printf("VOUT_ERROR vout dither RAM %x, test addr %x \n", i, j);
           } else {
             printf("RIGHT vout dither RAM %x, test addr %x \n", i, j);
           }
       }
     }
     Wr(VOUT_APB_BASE_ADDR+(0x609<<2),0);
     Wr(VOUT_APB_BASE_ADDR+(0x2a4<<2),0);//reg_ram_clk_sel = 0

     //bcon
     for (i=0;i<1;i=i+1) {
       Wr(VOUT_APB_BASE_ADDR+(0x60a<<2),1<<(i));
       for (j=0;j<1250;j++) {
         Wr(VOUT_APB_BASE_ADDR+((0x700+j)<<2),0x0);
       }
       for (j=0;j<1250;j++) {
         data_tmp = Rd(VOUT_APB_BASE_ADDR+((0x700+j)<<2));
           if (data_tmp != 0) {
             printf("VOUT_ERROR vout bcon RAM %x, test addr %x\n", i, j);
           }else {
             printf("RIGHT vout bcon RAM %x, test addr %x \n", i, j);
           }
       }
       for (j=0;j<1250;j++) {
         Wr(VOUT_APB_BASE_ADDR+((0x700+j)<<2),0xffffffff);
       }
       for (j=0;j<1250;j++) {
         data_tmp = Rd(VOUT_APB_BASE_ADDR+((0x700+j)<<2));
           if (data_tmp != 0x7ff) {
             printf("VOUT_ERROR vout bcon RAM %x, test addr %x \n", i, j);
           } else {
             printf("RIGHT vout bcon RAM %x, test addr %x \n", i, j);
           }
       }
     }
     Wr(VOUT_APB_BASE_ADDR+(0x60a<<2),0);
     }

void vout_2560x1440_cmpr_od_test()
{
    uint32_t data_tmp;
    uint32_t i, addr, data;
    uint8_t rd_setting;
    uint32_t len;

    Wr((VOUT_APB_BASE_ADDR+(0x8000)),0x8000);
    Wr((VOUT_APB_BASE_ADDR+(0x800c)),0x0077);
    //vout, except lvds
    len = sizeof(vout_2560x1440_cmpr_od_setting)/sizeof(vout_2560x1440_cmpr_od_setting[0]);
    for (i=0;i<len/2;i=i+1)  {
      addr = vout_2560x1440_cmpr_od_setting[i*2];
      data = vout_2560x1440_cmpr_od_setting[i*2+1];
     // printf("vout wr test addr %x = %x\n", addr, data);
      Wr((VOUT_APB_BASE_ADDR+(addr<<2)),data);
    }
    Wr((VOUT_APB_BASE_ADDR+(0x800c)),0x007f);
}

void vout_1080p_to_qhd_lvds(aml_lvds_cfg_t lvds_cfg,uint32_t scaler_in_h,uint32_t scaler_in_v,uint32_t scaler_o_h,uint32_t scaler_o_v,uint32_t h_start,uint32_t v_start,uint32_t h_tot_o,uint32_t v_tot_o,uint32_t in_patt_en)
{
    uint32_t data_tmp;
    uint32_t i, addr, data;
    uint8_t rd_setting;
    size_t len;

    uint32_t  reg_mute_httl_src_sel=0;
		uint32_t	reg_mute_hs_det_en=0;
		uint32_t	reg_mute_de_w1c_en=0;
		uint32_t	reg_mute_de_det_en=1;

    uint32_t	pre_top_data_ctrl0_data;
    uint32_t	pre_top_data_ctrl1_data;
    uint32_t	pre_top_data_ctrl2_data;
    uint32_t	pre_top_data_ctrl3_data;
    uint32_t	pre_top_data_ctrl8_data;
    uint32_t	pre_top_data_ctrl9_data;
    uint32_t	pre_top_data_ctrl74_data;
    uint32_t	pre_top_data_ctrl81_data;

	  uint32_t  reg_test_patt_enable=0;
		uint32_t	reg_test_patt_timing_slf_gen=0;
		uint32_t	reg_test_patt_mode=6;
		uint32_t	reg_test_patt_prede=0;
		uint32_t	reg_test_patt_vstart=16;
		uint32_t	reg_test_patt_hstart=36;

		uint32_t  reg_pre_bypass_proc=0;
		uint32_t	reg_pre_bypass_vpu_pq=1;
		uint32_t	reg_test_patt_vtotal;
		uint32_t	reg_test_patt_htotal;

    uint32_t	reg_test_patt_vend;
		uint32_t	reg_test_patt_hend;

    uint32_t	reg_pix_pll_frac_in=0x3333;
		uint32_t	reg_pix_pll_m_in=0x43;

    uint32_t	reg_pll_frac_in=0x180000;
		uint32_t	reg_pll_m_in=0xe1;

    uint32_t  reg_mute_force_freerun=0;
    uint32_t  reg_mute_force_normal=1;
    uint32_t	reg_mute_htot;
		uint32_t	reg_mute_vtot;
    uint32_t	cmpr_data;

    reg_mute_htot        = 800;
    reg_mute_vtot        = 62;
    MNT_PPS_t pps;

    cmpr_data =    (scaler_o_v << 12) | (0x280);
    addr = 0x70;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),cmpr_data);
    printf("vout wr cmpr data addr %x = %x\n", addr, cmpr_data);


    pre_top_data_ctrl8_data = (reg_mute_force_freerun << 31) |
                              (reg_mute_force_normal  << 30) |
                              (reg_mute_vtot          << 15) |
                               reg_mute_htot;
    addr = 0x208;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl8_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl8_data);



    reg_test_patt_vstart = v_start;
    reg_test_patt_hstart = h_start;
    reg_test_patt_vend   = v_start + scaler_o_v;
    reg_test_patt_hend   = h_start + scaler_o_h;

    reg_test_patt_vtotal = v_tot_o;
    reg_test_patt_htotal = h_tot_o;

    Wr((VOUT_APB_BASE_ADDR+(0x8000)),0x8000);
    Wr((VOUT_APB_BASE_ADDR+(0x800c)),0x007f);

    pre_top_data_ctrl9_data = (reg_mute_httl_src_sel << 31) |
                              (reg_mute_hs_det_en << 30)    |
                              (reg_mute_de_w1c_en << 29)    |
                              (reg_mute_de_det_en << 28)    |
                              (scaler_in_v << 14)           |
                              (scaler_in_h);
    pre_top_data_ctrl0_data = (reg_pre_bypass_proc << 31)   |
                              (reg_pre_bypass_vpu_pq << 30) |
                              (reg_test_patt_vtotal << 15)  |
                              (reg_test_patt_htotal);


    pre_top_data_ctrl1_data = (reg_test_patt_enable << 31)         |
                              (reg_test_patt_timing_slf_gen << 30) |
                              (reg_test_patt_mode << 26)           |
                              (reg_test_patt_prede << 20)          |
                              (reg_test_patt_vstart << 10)         |
                              (reg_test_patt_hstart);
    pre_top_data_ctrl2_data = (0 << 31) |
                              (0 << 30) |
                              (reg_test_patt_vend << 15) |
                              (reg_test_patt_hend);
    pre_top_data_ctrl3_data = (in_patt_en << 31) |
                              (in_patt_en << 30) |
                              (0 << 28)          |
                              (3 << 14)          |
                              (5);
    pre_top_data_ctrl74_data = (reg_pll_frac_in << 9) |
                                reg_pll_m_in;
    pre_top_data_ctrl81_data = (reg_pix_pll_frac_in << 9) |
                                reg_pix_pll_m_in;
    addr = 0x24a;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl74_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl74_data);
    addr = 0x251;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl81_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl81_data);
    //set scaler size
    printf("#########vout scaler in  hszie %d vsize %d\n", scaler_in_h, scaler_in_v);
    printf("#########vout scaler out hszie %d vsize %d\n", scaler_o_h, scaler_o_v);
    addr = 0x209;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl9_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl9_data);
    addr = 0x200;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl0_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl0_data);
    addr = 0x201;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl1_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl1_data);
    addr = 0x202;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl2_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl2_data);
    addr = 0x203;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl3_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl3_data);


    len = sizeof(vout_1080p_to_qhd_200hz_setting)/sizeof(vout_1080p_to_qhd_200hz_setting[0]);
    for (i=0;i<len/2;i=i+1)  {
      addr = vout_1080p_to_qhd_200hz_setting[i*2];
      data = vout_1080p_to_qhd_200hz_setting[i*2+1];
      Wr((VOUT_APB_BASE_ADDR+(addr<<2)),data);
    }
    pps.src_hsize = scaler_in_h;
    pps.src_vsize = scaler_in_v;
    pps.dst_hsize = scaler_o_h;
    pps.dst_vsize = scaler_o_v;

    pps.posthsc_en = scaler_in_h == scaler_o_h ? 0 : 1;
    pps.postvsc_en = scaler_in_v == scaler_o_v ? 0 : 1;

    pps.pps_dbg_en = 0;

    cfg_mnt_pps(0, &pps);
}

void vout_in_patt_1080p_to_qhd_lvds(aml_lvds_cfg_t lvds_cfg,uint32_t scaler_in_h,uint32_t scaler_in_v,uint32_t scaler_o_h,uint32_t scaler_o_v,uint32_t h_start,uint32_t v_start,uint32_t h_tot_o,uint32_t v_tot_o,uint32_t in_patt_en)
{
    uint32_t data_tmp;
    uint32_t i, addr, data;
    uint8_t rd_setting;
    size_t len;

    uint32_t  reg_mute_httl_src_sel=0;
		uint32_t	reg_mute_hs_det_en=0;
		uint32_t	reg_mute_de_w1c_en=0;
		uint32_t	reg_mute_de_det_en=1;

    uint32_t	pre_top_data_ctrl0_data;
    uint32_t	pre_top_data_ctrl1_data;
    uint32_t	pre_top_data_ctrl2_data;
    uint32_t	pre_top_data_ctrl3_data;
    uint32_t	pre_top_data_ctrl8_data;
    uint32_t	pre_top_data_ctrl9_data;
    uint32_t	pre_top_data_ctrl74_data;
    uint32_t	pre_top_data_ctrl81_data;

	  uint32_t  reg_test_patt_enable=0;
		uint32_t	reg_test_patt_timing_slf_gen=0;
		uint32_t	reg_test_patt_mode=6;
		uint32_t	reg_test_patt_prede=0;
		uint32_t	reg_test_patt_vstart=16;
		uint32_t	reg_test_patt_hstart=36;

		uint32_t  reg_pre_bypass_proc=0;
		uint32_t	reg_pre_bypass_vpu_pq=1;
		uint32_t	reg_test_patt_vtotal;
		uint32_t	reg_test_patt_htotal;

    uint32_t	reg_test_patt_vend;
		uint32_t	reg_test_patt_hend;

    uint32_t  reg_mute_force_freerun=0;
    uint32_t  reg_mute_force_normal=1;
    uint32_t	reg_mute_htot;
		uint32_t	reg_mute_vtot;

    uint32_t	reg_pix_pll_frac_in=0x3333;
		uint32_t	reg_pix_pll_m_in=0x43;

    uint32_t	reg_pll_frac_in=0x180000;
		uint32_t	reg_pll_m_in=0xe1;

    uint64_t  scaler_phase_step_h;
    uint32_t  scaler_int_part_h;
    uint32_t  scaler_frac_part_h;

    uint64_t  scaler_phase_step_v;
    uint32_t  scaler_int_part_v;
    uint32_t  scaler_frac_part_v;

    reg_test_patt_vstart = v_start;
    reg_test_patt_hstart = h_start;
    reg_test_patt_vend   = v_start + scaler_o_v;
    reg_test_patt_hend   = h_start + scaler_o_h;

 
    reg_mute_htot        = 2200;
    reg_mute_vtot        = 1125;


    reg_test_patt_vtotal = v_tot_o;
    reg_test_patt_htotal = h_tot_o;

    pre_top_data_ctrl8_data = (reg_mute_force_freerun << 31) |
                              (reg_mute_force_normal  << 30) |
                              (reg_mute_vtot          << 15) |
                               reg_mute_htot;

    MNT_PPS_t pps;

    pps.src_hsize = scaler_in_h;
    pps.src_vsize = scaler_in_v;
    pps.dst_hsize = scaler_o_h;
    pps.dst_vsize = scaler_o_v;

    pps.posthsc_en = scaler_in_h == scaler_o_h ? 0 : 1;
    pps.postvsc_en = scaler_in_v == scaler_o_v ? 0 : 1;

    pps.pps_dbg_en = 0;

    cfg_mnt_pps(0, &pps);

    printf("video out input  size h = %d v = %d \n", scaler_in_h, scaler_in_v);
    printf("video out output size h = %d v = %d \n", scaler_o_h, scaler_o_v);
    printf("video out scaler h config int_part = %d frac_part = %d \n", scaler_int_part_h,scaler_frac_part_h);
    printf("video out scaler v config int_part = %d frac_part = %d \n", scaler_int_part_v,scaler_frac_part_v);

    //Wr((VOUT_APB_BASE_ADDR+(0xa1<<2)),((scaler_int_part_v<<24)|scaler_frac_part_v));
    //Wr((VOUT_APB_BASE_ADDR+(0xa2<<2)),((scaler_int_part_h<<24)|scaler_frac_part_h));


    Wr((VOUT_APB_BASE_ADDR+(0x8000)),0x8000);
    Wr((VOUT_APB_BASE_ADDR+(0x800c)),0x007f);

    pre_top_data_ctrl9_data = (reg_mute_httl_src_sel << 31) |
                              (reg_mute_hs_det_en << 30)    |
                              (reg_mute_de_w1c_en << 29)    |
                              (reg_mute_de_det_en << 28)    |
                              (scaler_in_v << 14)           |
                              (scaler_in_h);
    pre_top_data_ctrl0_data = (reg_pre_bypass_proc << 31)   |
                              (reg_pre_bypass_vpu_pq << 30) |
                              (reg_test_patt_vtotal << 15)  |
                              (reg_test_patt_htotal);


    pre_top_data_ctrl1_data = (reg_test_patt_enable << 31)         |
                              (reg_test_patt_timing_slf_gen << 30) |
                              (reg_test_patt_mode << 26)           |
                              (reg_test_patt_prede << 20)          |
                              (reg_test_patt_vstart << 10)         |
                              (reg_test_patt_hstart);
    pre_top_data_ctrl2_data = (0 << 31) |
                              (0 << 30) |
                              (reg_test_patt_vend << 15) |
                              (reg_test_patt_hend);
    pre_top_data_ctrl3_data = (in_patt_en << 31) |
                              (in_patt_en << 30) |
                              (0 << 28)          |
                              (3 << 14)          |
                              (5);
    pre_top_data_ctrl74_data = (reg_pll_frac_in << 9) |
                                reg_pll_m_in;
    pre_top_data_ctrl81_data = (reg_pix_pll_frac_in << 9) |
                                reg_pix_pll_m_in;
    addr = 0x24a;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl74_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl74_data);
    addr = 0x251;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl81_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl81_data);
    //set scaler size
    printf("#########vout scaler in  hszie %d vsize %d\n", scaler_in_h, scaler_in_v);
    printf("#########vout scaler out hszie %d vsize %d\n", scaler_o_h, scaler_o_v);
    addr = 0x208;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl8_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl8_data);
    addr = 0x209;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl9_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl9_data);
    addr = 0x200;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl0_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl0_data);
    addr = 0x201;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl1_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl1_data);
    addr = 0x202;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl2_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl2_data);


    len = sizeof(vout_1080p_to_qhd_200hz_case9032_setting)/sizeof(vout_1080p_to_qhd_200hz_case9032_setting[0]);
    for (i=0;i<len/2;i=i+1)  {
      addr = vout_1080p_to_qhd_200hz_case9032_setting[i*2];
      data = vout_1080p_to_qhd_200hz_case9032_setting[i*2+1];
      printf("vout wr edp test addr %x = %x\n", addr, data);
      Wr((VOUT_APB_BASE_ADDR+(addr<<2)),data);
    }
    addr = 0x203;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl3_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl3_data);
}

void vout_in_patt_vpu_1080p_to_qhd_lvds(aml_lvds_cfg_t lvds_cfg,uint32_t htot_in,uint32_t vtot_in,uint32_t scaler_in_h,uint32_t scaler_in_v,uint32_t scaler_o_h,uint32_t scaler_o_v,uint32_t h_start,uint32_t v_start,uint32_t h_tot_o,uint32_t v_tot_o,uint32_t in_patt_en)
{
    uint32_t data_tmp;
    uint32_t i, addr, data;
    uint8_t rd_setting;
    size_t len;

    uint32_t  reg_mute_httl_src_sel=0;
		uint32_t	reg_mute_hs_det_en=0;
		uint32_t	reg_mute_de_w1c_en=0;
		uint32_t	reg_mute_de_det_en=1;

    uint32_t	pre_top_data_ctrl0_data;
    uint32_t	pre_top_data_ctrl1_data;
    uint32_t	pre_top_data_ctrl2_data;
    uint32_t	pre_top_data_ctrl3_data;
    uint32_t	pre_top_data_ctrl8_data;
    uint32_t	pre_top_data_ctrl9_data;
    uint32_t	pre_top_data_ctrl74_data;
    uint32_t	pre_top_data_ctrl81_data;

	  uint32_t  reg_test_patt_enable=0;
		uint32_t	reg_test_patt_timing_slf_gen=0;
		uint32_t	reg_test_patt_mode=6;
		uint32_t	reg_test_patt_prede=0;
		uint32_t	reg_test_patt_vstart=16;
		uint32_t	reg_test_patt_hstart=36;

		uint32_t  reg_pre_bypass_proc=0;
		uint32_t	reg_pre_bypass_vpu_pq=1;
		uint32_t	reg_test_patt_vtotal;
		uint32_t	reg_test_patt_htotal;

    uint32_t	reg_test_patt_vend;
		uint32_t	reg_test_patt_hend;

    uint32_t  reg_mute_force_freerun=0;
    uint32_t  reg_mute_force_normal=1;
    uint32_t	reg_mute_htot;
		uint32_t	reg_mute_vtot;

    uint32_t	reg_pix_pll_frac_in=0x3333;
		uint32_t	reg_pix_pll_m_in=0x43;

    uint32_t	reg_pll_frac_in=0x180000;
		uint32_t	reg_pll_m_in=0xe1;

    uint64_t  scaler_phase_step_h;
    uint32_t  scaler_int_part_h;
    uint32_t  scaler_frac_part_h;

    uint64_t  scaler_phase_step_v;
    uint32_t  scaler_int_part_v;
    uint32_t  scaler_frac_part_v;

    reg_test_patt_vstart = v_start;
    reg_test_patt_hstart = h_start;
    reg_test_patt_vend   = v_start + scaler_o_v;
    reg_test_patt_hend   = h_start + scaler_o_h;

 
    reg_mute_htot        = htot_in;
    reg_mute_vtot        = vtot_in;


    reg_test_patt_vtotal = v_tot_o;
    reg_test_patt_htotal = h_tot_o;

    pre_top_data_ctrl8_data = (reg_mute_force_freerun << 31) |
                              (reg_mute_force_normal  << 30) |
                              (reg_mute_vtot          << 15) |
                               reg_mute_htot;


    MNT_PPS_t pps;

    pps.src_hsize = scaler_in_h;
    pps.src_vsize = scaler_in_v;
    pps.dst_hsize = scaler_o_h;
    pps.dst_vsize = scaler_o_v;

    pps.posthsc_en = scaler_in_h == scaler_o_h ? 0 : 1;
    pps.postvsc_en = scaler_in_v == scaler_o_v ? 0 : 1;

    pps.pps_dbg_en = 0;

    cfg_mnt_pps(0, &pps);   
    printf("video out input  size h = %d v = %d \n", scaler_in_h, scaler_in_v);
    printf("video out output size h = %d v = %d \n", scaler_o_h, scaler_o_v);


    //Wr((VOUT_APB_BASE_ADDR+(0x2a1<<2)),((scaler_int_part_v<<24)|scaler_frac_part_v));
    //Wr((VOUT_APB_BASE_ADDR+(0x2a2<<2)),((scaler_int_part_h<<24)|scaler_frac_part_h));


    Wr((VOUT_APB_BASE_ADDR+(0x8000)),0x8000);
    Wr((VOUT_APB_BASE_ADDR+(0x800c)),0x007f);

    pre_top_data_ctrl9_data = (reg_mute_httl_src_sel << 31) |
                              (reg_mute_hs_det_en << 30)    |
                              (reg_mute_de_w1c_en << 29)    |
                              (reg_mute_de_det_en << 28)    |
                              (scaler_in_v << 14)           |
                              (scaler_in_h);
    pre_top_data_ctrl0_data = (reg_pre_bypass_proc << 31)   |
                              (reg_pre_bypass_vpu_pq << 30) |
                              (reg_test_patt_vtotal << 15)  |
                              (reg_test_patt_htotal);


    pre_top_data_ctrl1_data = (reg_test_patt_enable << 31)         |
                              (reg_test_patt_timing_slf_gen << 30) |
                              (reg_test_patt_mode << 26)           |
                              (reg_test_patt_prede << 20)          |
                              (reg_test_patt_vstart << 10)         |
                              (reg_test_patt_hstart);
    pre_top_data_ctrl2_data = (0 << 31) |
                              (0 << 30) |
                              (reg_test_patt_vend << 15) |
                              (reg_test_patt_hend);
    pre_top_data_ctrl3_data = (in_patt_en << 31) |
                              (in_patt_en << 30) |
                              (0 << 28)          |
                              (3 << 14)          |
                              (5);
    pre_top_data_ctrl74_data = (reg_pll_frac_in << 9) |
                                reg_pll_m_in;
    pre_top_data_ctrl81_data = (reg_pix_pll_frac_in << 9) |
                                reg_pix_pll_m_in;
    addr = 0x24a;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl74_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl74_data);
    addr = 0x251;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl81_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl81_data);
    //set scaler size
    printf("#########vout scaler in  hszie %d vsize %d\n", scaler_in_h, scaler_in_v);
    printf("#########vout scaler out hszie %d vsize %d\n", scaler_o_h, scaler_o_v);
    addr = 0x208;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl8_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl8_data);
    addr = 0x209;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl9_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl9_data);
    addr = 0x200;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl0_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl0_data);
    addr = 0x201;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl1_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl1_data);
    addr = 0x202;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl2_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl2_data);


    len = sizeof(vout_1080p_to_qhd_200hz_case9032_setting)/sizeof(vout_1080p_to_qhd_200hz_case9032_setting[0]);
    for (i=0;i<len/2;i=i+1)  {
      addr = vout_1080p_to_qhd_200hz_case9032_setting[i*2];
      data = vout_1080p_to_qhd_200hz_case9032_setting[i*2+1];
      printf("vout wr edp test addr %x = %x\n", addr, data);
      Wr((VOUT_APB_BASE_ADDR+(addr<<2)),data);
    }
    addr = 0x203;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl3_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl3_data);
}

void vout_in_patt_vpu_1080p_to_qhd_lvds_padding(aml_lvds_cfg_t lvds_cfg,uint32_t htot_in,uint32_t vtot_in,uint32_t pps_in_h,uint32_t pps_in_v,uint32_t scaler_o_h,uint32_t scaler_o_v,uint32_t pps_o_h,uint32_t pps_o_v,uint32_t h_start,uint32_t v_start,uint32_t h_tot_o,uint32_t v_tot_o,uint32_t in_patt_en)
{
    uint32_t  addr;
    uint32_t  pre_top_data_ctrl1_data;
    uint32_t  pre_top_data_ctrl2_data;

    uint32_t  reg_test_patt_enable=0;
    uint32_t  reg_test_patt_timing_slf_gen=0;
    uint32_t  reg_test_patt_mode=6;
    uint32_t  reg_test_patt_prede=0;
    uint32_t  reg_test_patt_vstart=16;
    uint32_t  reg_test_patt_hstart=36;

    uint32_t  reg_test_patt_vend;
    uint32_t  reg_test_patt_hend;

    uint32_t  reg_pps_crop_vstart=0;
    uint32_t  reg_pps_crop_hstart=0;
    uint32_t  reg_pps_crop_vend  = pps_in_v-1;
    uint32_t  reg_pps_crop_hend  = pps_in_h-1;

    uint32_t  reg_pps_pad_vstart;
    uint32_t  reg_pps_pad_hstart;
    uint32_t  reg_pps_pad_vend; 
    uint32_t  reg_pps_pad_hend;

    printf("vout_in_patt_vpu_1080p_to_qhd_lvds_padding pps_in_h   = %d, pps_in_v= %d\n"  , pps_in_h  , pps_in_v  );
    printf("vout_in_patt_vpu_1080p_to_qhd_lvds_padding scaler_o_h = %d, scaler_o_v= %d\n", scaler_o_h, scaler_o_v);
    printf("vout_in_patt_vpu_1080p_to_qhd_lvds_padding pps_o_h    = %d, pps_o_v= %d\n"   , pps_in_h  , pps_in_v  );

    reg_test_patt_vstart = v_start;
    reg_test_patt_hstart = h_start;
    reg_test_patt_vend   = v_start + pps_o_v;
    reg_test_patt_hend   = h_start + pps_o_h;

    reg_pps_pad_vstart   = (pps_o_v - scaler_o_v)/2;
    reg_pps_pad_hstart   = (pps_o_h - scaler_o_h)/2;
    reg_pps_pad_vend     = reg_pps_pad_vstart + scaler_o_v - 1;
    reg_pps_pad_hend     = reg_pps_pad_hstart + scaler_o_h - 1;

    //input cut
    Wr_reg_bits(VOUT_PPS_TOP_CTRL    ,1                  ,0 ,1 );//reg_crop_en        
    Wr_reg_bits(VOUT_PPS_SCALE_CROP_V,reg_pps_crop_vstart,16,13);
    Wr_reg_bits(VOUT_PPS_SCALE_CROP_V,reg_pps_crop_vend  ,0 ,13);
    Wr_reg_bits(VOUT_PPS_SCALE_CROP_H,reg_pps_crop_hstart,16,13);
    Wr_reg_bits(VOUT_PPS_SCALE_CROP_H,reg_pps_crop_hend  ,0 ,13);
    //output padding 
    Wr_reg_bits(VOUT_PPS_TOP_CTRL    ,1                  ,4 ,1 );//reg_padd_en    
    Wr_reg_bits(VOUT_PPS_SCALE_PADD_H,reg_pps_pad_hstart ,16,13);
    Wr_reg_bits(VOUT_PPS_SCALE_PADD_H,reg_pps_pad_hend   ,0 ,13);
    Wr_reg_bits(VOUT_PPS_SCALE_PADD_V,reg_pps_pad_vstart ,16,13);
    Wr_reg_bits(VOUT_PPS_SCALE_PADD_V,reg_pps_pad_vend   ,0 ,13);

    vout_in_patt_vpu_1080p_to_qhd_lvds(lvds_cfg, htot_in, vtot_in, pps_in_h, pps_in_v, scaler_o_h, scaler_o_v, h_start, v_start, h_tot_o, v_tot_o, in_patt_en);

    //recfg pps 
    //MNT_PPS_t pps;
    //pps.src_hsize = pps_in_h;
    //pps.src_vsize = pps_in_v;
    //pps.dst_hsize = scaler_o_h;
    //pps.dst_vsize = scaler_o_v;
    //pps.posthsc_en = pps.src_hsize == pps.dst_hsize ? 0 : 1;
    //pps.postvsc_en = pps.src_vsize == pps.dst_vsize ? 0 : 1;
    //cfg_mnt_pps(0, &pps);

    pre_top_data_ctrl1_data = (reg_test_patt_enable << 31)         |
                              (reg_test_patt_timing_slf_gen << 30) |
                              (reg_test_patt_mode << 26)           |
                              (reg_test_patt_prede << 20)          |
                              (reg_test_patt_vstart << 10)         |
                              (reg_test_patt_hstart);

    pre_top_data_ctrl2_data = (0 << 31) |
                              (0 << 30) |
                              (reg_test_patt_vend << 15) |
                              (reg_test_patt_hend);

    addr = 0x201;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl1_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl1_data);
    addr = 0x202;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl2_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl2_data);
}


void vout_video_config_phase(aml_lvds_cfg_t lvds_cfg,uint32_t scaler_in_h,uint32_t scaler_in_v,uint32_t scaler_o_h,uint32_t scaler_o_v,uint32_t h_start,uint32_t v_start,uint32_t h_tot_o,uint32_t v_tot_o,uint32_t in_patt_en,uint32_t pll_freq)
{
    uint32_t data_tmp;
    uint32_t i, addr, data;
    uint8_t rd_setting;
    size_t len;

    uint32_t  reg_mute_httl_src_sel=0;
		uint32_t	reg_mute_hs_det_en=0;
		uint32_t	reg_mute_de_w1c_en=0;
		uint32_t	reg_mute_de_det_en=1;

    uint32_t	pre_top_data_ctrl0_data;
    uint32_t	pre_top_data_ctrl1_data;
    uint32_t	pre_top_data_ctrl2_data;
    uint32_t	pre_top_data_ctrl3_data;
    uint32_t	pre_top_data_ctrl9_data;
    uint32_t	pre_top_data_ctrl74_data;
    uint32_t	pre_top_data_ctrl81_data;

	  uint32_t  reg_test_patt_enable=0;
		uint32_t	reg_test_patt_timing_slf_gen=0;
		uint32_t	reg_test_patt_mode=6;
		uint32_t	reg_test_patt_prede=0;
		uint32_t	reg_test_patt_vstart=16;
		uint32_t	reg_test_patt_hstart=36;

		uint32_t  reg_pre_bypass_proc=0;
		uint32_t	reg_pre_bypass_vpu_pq=1;
		uint32_t	reg_test_patt_vtotal;
		uint32_t	reg_test_patt_htotal;

    uint32_t	reg_test_patt_vend;
		uint32_t	reg_test_patt_hend;

    uint32_t	reg_pix_pll_frac_in=0x3333;
		uint32_t	reg_pix_pll_m_in=0x43;

    uint32_t	reg_pll_frac_in=0x180000;
		uint32_t	reg_pll_m_in=0xe1;

    uint64_t  scaler_phase_step_h;
    uint32_t  scaler_int_part_h;
    uint32_t  scaler_frac_part_h;

    uint64_t  scaler_phase_step_v;
    uint32_t  scaler_int_part_v;
    uint32_t  scaler_frac_part_v;


    Wr((VOUT_APB_BASE_ADDR+(0x8000)),0x8000);
    Wr((VOUT_APB_BASE_ADDR+(0x800c)),0x007f);
    //MNT_PPS_t pps;

    //pps.src_hsize = scaler_in_h;
    //pps.src_vsize = scaler_in_v;
    //pps.dst_hsize = scaler_o_h;
    //pps.dst_vsize = scaler_o_v;

    //pps.posthsc_en = scaler_in_h == scaler_o_h ? 0 : 1;
    //pps.postvsc_en = scaler_in_v == scaler_o_v ? 0 : 1;

    //pps.pps_dbg_en = 0;

    //cfg_mnt_pps(0, &pps);
    reg_test_patt_vstart = v_start;
    reg_test_patt_hstart = h_start;
    reg_test_patt_vend   = v_start + scaler_o_v;
    reg_test_patt_hend   = h_start + scaler_o_h;

    reg_test_patt_vtotal = v_tot_o;
    reg_test_patt_htotal = h_tot_o;
    //pixel pll clock config
    reg_pix_pll_m_in    =  pll_freq/24000;
    reg_pix_pll_frac_in = (pll_freq-24000*reg_pix_pll_m_in)*(131072)/24000;
    printf("pixel pll m_in = %d frac_in = %d \n", reg_pix_pll_m_in, reg_pix_pll_frac_in);

    //scaler pps config
    scaler_phase_step_h = (scaler_in_h << 24)/scaler_o_h;
    scaler_int_part_h   = scaler_phase_step_h >> 24;
    scaler_frac_part_h  = scaler_phase_step_h & 0x00ffffff;

    scaler_phase_step_v = (scaler_in_v << 24)/scaler_o_v;
    scaler_int_part_v   = scaler_phase_step_v >> 24;
    scaler_frac_part_v  = scaler_phase_step_v & 0x00ffffff;

    printf("video out input  size h = %d v = %d \n", scaler_in_h, scaler_in_v);
    printf("video out output size h = %d v = %d \n", scaler_o_h, scaler_o_v);
    printf("video out scaler h config int_part = %d frac_part = %d \n", scaler_int_part_h,scaler_frac_part_h);
    printf("video out scaler v config int_part = %d frac_part = %d \n", scaler_int_part_v,scaler_frac_part_v);

    Wr((VOUT_APB_BASE_ADDR+(0xa1<<2)),((scaler_int_part_v<<24)|scaler_frac_part_v));
    Wr((VOUT_APB_BASE_ADDR+(0xa2<<2)),((scaler_int_part_h<<24)|scaler_frac_part_h));

    Wr((VOUT_APB_BASE_ADDR+(0x8000)),0x8000);
    Wr((VOUT_APB_BASE_ADDR+(0x800c)),0x007f);

    pre_top_data_ctrl9_data = (reg_mute_httl_src_sel << 31) |
                              (reg_mute_hs_det_en << 30)    |
                              (reg_mute_de_w1c_en << 29)    |
                              (reg_mute_de_det_en << 28)    |
                              (scaler_in_v << 14)           |
                              (scaler_in_h);
    pre_top_data_ctrl0_data = (reg_pre_bypass_proc << 31)   |
                              (reg_pre_bypass_vpu_pq << 30) |
                              (reg_test_patt_vtotal << 15)  |
                              (reg_test_patt_htotal);


    pre_top_data_ctrl1_data = (reg_test_patt_enable << 31)         |
                              (reg_test_patt_timing_slf_gen << 30) |
                              (reg_test_patt_mode << 26)           |
                              (reg_test_patt_prede << 20)          |
                              (reg_test_patt_vstart << 10)         |
                              (reg_test_patt_hstart);
    pre_top_data_ctrl2_data = (0 << 31) |
                              (0 << 30) |
                              (reg_test_patt_vend << 15) |
                              (reg_test_patt_hend);
    pre_top_data_ctrl3_data = (in_patt_en << 31) |
                              (in_patt_en << 30) |
                              (0 << 28)          |
                              (3 << 14)          |
                              (5);
    pre_top_data_ctrl74_data = (reg_pll_frac_in << 9) |
                                reg_pll_m_in;
    pre_top_data_ctrl81_data = (reg_pix_pll_frac_in << 9) |
                                reg_pix_pll_m_in;
    addr = 0x24a;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl74_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl74_data);
    addr = 0x251;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl81_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl81_data);
    //set scaler size
    printf("#########vout scaler in  hszie %d vsize %d\n", scaler_in_h, scaler_in_v);
    printf("#########vout scaler out hszie %d vsize %d\n", scaler_o_h, scaler_o_v);
    addr = 0x209;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl9_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl9_data);
    addr = 0x200;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl0_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl0_data);
    addr = 0x201;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl1_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl1_data);
    addr = 0x202;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl2_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl2_data);
    addr = 0x203;
    Wr((VOUT_APB_BASE_ADDR+(addr<<2)),pre_top_data_ctrl3_data);
    printf("vout wr pre top addr %x = %x\n", addr, pre_top_data_ctrl3_data);


//    len = sizeof(vout_default_setting)/sizeof(vout_default_setting[0]);
//    for (i=0;i<len/2;i=i+1)  {
//      addr = vout_default_setting[i*2];
//      data = vout_default_setting[i*2+1];
//      //printf("vout wr edp test addr %x = %x\n", addr, data);
//      Wr((VOUT_APB_BASE_ADDR+(addr<<2)),data);

    len = sizeof(vout_1080p_to_qhd_200hz_case9032_setting)/sizeof(vout_1080p_to_qhd_200hz_case9032_setting[0]);
    for (i=0;i<len/2;i=i+1)  {
      addr = vout_1080p_to_qhd_200hz_case9032_setting[i*2];
      data = vout_1080p_to_qhd_200hz_case9032_setting[i*2+1];
      //printf("vout wr edp test addr %x = %x\n", addr, data);
      Wr((VOUT_APB_BASE_ADDR+(addr<<2)),data);


    }
}

void vout_retimer_code(uint32_t scaler_in_h,uint32_t scaler_in_v,uint32_t h_tot_i,uint32_t v_tot_i,uint32_t scaler_o_h,uint32_t scaler_o_v,uint32_t h_tot_o,uint32_t v_tot_o,uint32_t in_patt_en,uint32_t force_mode,uint32_t adjust_mode,uint32_t pix_freq)
{ 
		  uint32_t CTRL0_DATA,CTRL1_DATA,CTRL2_DATA,CTRL3_DATA;
      uint32_t CTRL7_DATA,CTRL8_DATA,CTRL9_DATA,CTRL10_DATA;
      uint32_t CTRL11_DATA,CTRL12_DATA,CTRL59_DATA;
      uint32_t CTRL60_DATA,CTRL67_DATA,CTRL68_DATA,CTRL69_DATA;
      uint32_t CTRL70_DATA,CTRL71_DATA,CTRL73_DATA;
      uint32_t CTRL74_DATA,CTRL75_DATA,CTRL78_DATA,CTRL79_DATA;
      uint32_t CTRL80_DATA,CTRL81_DATA,CTRL82_DATA,CTRL83_DATA,CTRL84_DATA;
      uint32_t CTRL85_DATA,CTRL86_DATA;
      uint32_t CTRL87_DATA,CTRL88_DATA;

		  uint32_t reg_pre_bypass_proc;
			uint32_t reg_pre_bypass_vpu_pq;
			uint32_t reg_test_patt_vtotal;
			uint32_t reg_test_patt_htotal;

			uint32_t reg_test_patt_enable;
			uint32_t reg_test_patt_timing_slf_gen;
			uint32_t reg_test_patt_mode;
			uint32_t reg_test_patt_prede;
			uint32_t reg_test_patt_vstart;
			uint32_t reg_test_patt_hstart;

			uint32_t reg_pre_bypass_od             ;
			uint32_t reg_pre_bypass_scaler         ;
			uint32_t reg_test_patt_vend            ;
			uint32_t reg_test_patt_hend            ;

		  uint32_t reg_in_patt_enable            ;
		  uint32_t reg_in_patt_timing_slf_gen    ;
		  uint32_t reg_vs_sel                    ;
		  uint32_t reg_test_patt_vs_start        ;
		  uint32_t reg_test_patt_vs_end          ;

      uint32_t reg_pre_clk_sel               ;//1:in_clk 0:pix_clk
			uint32_t reg_scaler_after_buf_en       ;//need fw
			uint32_t reg_pre_hs_dly                ;
			uint32_t reg_pre_proc_gate_clk_ctrl    ;//clock gate sel
			uint32_t reg_pre_dither_gate_clk_ctrl  ;//clock gate sel
			uint32_t reg_pre_pix_clk_div4_en       ;
			uint32_t reg_pre_in_clk_sel            ;//0:in_clk 1:pix_clk 2:vp_opt_clk
			uint32_t reg_pre_vp_clk_mux            ;//0:in_clk 1:pix_clk 2:vp_opt_clk
			uint32_t reg_pre_proc_clk_en           ;
			uint32_t reg_pre_dither_clk_en         ;
			uint32_t reg_pre_od_clk_en             ;
			uint32_t reg_pre_data_clk_en           ;

      uint32_t reg_mute_force_freerun        ;
		  uint32_t reg_mute_force_normal         ;
		  uint32_t reg_mute_vtotal               ;
		  uint32_t reg_mute_htotal               ;

      uint32_t reg_mute_httl_src_sel         ;
			uint32_t reg_mute_hs_det_en            ;
			uint32_t reg_mute_de_w1c_en            ;
			uint32_t reg_mute_de_det_en            ;
			uint32_t reg_mute_vact                 ;
			uint32_t reg_mute_hact                 ;

      uint32_t reg_mute_stmq_timer           ;
			uint32_t reg_mute_stme_timer           ;
			uint32_t reg_mute_vbporch              ;
			uint32_t reg_mute_hbporch              ;

      uint32_t reg_mute_st_clkabn_reset_en   ;
			uint32_t reg_mute_st_clk_as_ref        ;
			uint32_t reg_mute_clk_w1c_en           ;
			uint32_t reg_mute_clk_det_en           ;
			uint32_t reg_mute_clk_freq_low_th      ;
			uint32_t reg_mute_vwidth               ;
			uint32_t reg_mute_hwidth               ;

      uint32_t reg_in_vid_mute_enable       ;
			uint32_t reg_patt_enable              ;
			uint32_t reg_patt_loop_number         ;
			uint32_t reg_patt_blending_alpha      ;
			uint32_t reg_mute_st_sf2n_option      ;
			uint32_t reg_mute_st_vs_as_ref        ;
			uint32_t reg_mute_st_manu_reset       ;
			uint32_t reg_mute_st_de_as_ref        ;
			uint32_t reg_mute_clk_freq_high_th    ;

      uint32_t	reg_retimer_v_dly_mode      ;
		  uint32_t	reg_retimer_h_dly_num       ;
		  uint32_t	reg_retimer_v_dly_num       ;

      uint32_t  reg_retimer_video_on_vs_lath_sel  ;
			uint32_t  reg_retimer_tmg_en                ;
			uint32_t  reg_retimer_video_on              ;

      uint32_t	reg_level_min                     ;
		  uint32_t	reg_level_max                     ;

      uint32_t  reg_pid_cal_sel                   ;//0: ref clock pid adjust 1: ref fifo level
		  uint32_t  reg_err_sum_thr_p1                ;//fifo_level - max/min,
		  uint32_t  reg_sum_sft_p1                    ;//fifo_level_sum in reg_pid_cal_cycle
		  uint32_t  reg_err_thr_p1                    ;//fifo_level_avg delta value

      uint32_t  reg_pix_count_exp_p0              ;
			uint32_t  reg_pid_cal_cycle_p1              ;

      uint32_t  reg_pix_pll_adj_thr_max           ;
      uint32_t  reg_pix_pll_adj_thr_min           ;
      uint32_t  reg_pll_adj_thr_max               ;
      uint32_t  reg_pll_adj_thr_min               ;

      uint32_t  reg_hs_regen                      ;
		  uint32_t	reg_clip_sel                      ;
		  uint32_t	reg_crc_en                        ;
		  uint32_t	reg_crc_sel                       ;
		  uint32_t	reg_htot_adj_en                   ;
		  uint32_t	reg_pll_adj_en                    ;
		  uint32_t	reg_adjust_start                  ;
		  uint32_t	reg_adjust_end                    ;

		  uint32_t	reg_pll_frac_in                   ;
		  uint32_t	reg_pll_m_in                      ;

		  uint32_t	reg_fifo_level_err_thr_max        ;
		  uint32_t	reg_fifo_level_err_thr_min        ;

		  uint32_t	reg_mute_vs_sel                   ;
		  uint32_t	reg_mute_vs_end                   ;
		  uint32_t	reg_mute_vs_start                 ;

		  uint32_t  reg_no_de_thr                     ;

		  uint32_t  reg_pix_pll_frac_in               ;
		  uint32_t  reg_pix_pll_m_in                  ;

		  uint32_t  reg_multi_factor_p0               ;
		  uint32_t  reg_cur_err_sum_thr_p0            ;

		  uint32_t  reg_pid_cal_cycle_p0              ;//base line number
		  uint32_t  reg_err_thr_p0                    ;

		  uint32_t  reg_ki_coef_p0                    ;
		  uint32_t  reg_kp_coef_p0                    ;

		  uint32_t	reg_ki_coef_p1                    ;
		  uint32_t	reg_kp_coef_p1                    ;

		  uint32_t	reg_level_delta_exp_p1            ;
		  uint32_t	reg_multi_factor_p1               ;

      uint32_t  reg_retimer_vs_regen_end          ;
      uint32_t  reg_retimer_vs_regen_start        ;
      uint32_t  reg_htot_adj_min                  ;
      uint32_t  reg_htot_adj_max                  ; 
      uint32_t  reg_edge_ctrl                     ; 
      uint32_t  reg_pls_frm_start                 ; 
      uint32_t  reg_frm_start_sel                 ; 
      uint32_t  reg_update_en                     ; 
      uint32_t  reg_ro_update_en                  ; 
      uint32_t  reg_hold_line_num                 ;

		  uint32_t	reg_pll_m_in_max;
		  uint32_t	reg_pix_pll_m_in_max;
      uint32_t	addr;
      uint64_t  value_64_0,value_64_1;
      uint64_t  value_64=1;


      uint32_t  reg_frm_v_width                   ;
      uint32_t  reg_mbw                           ;
		  uint32_t  CMPR_CTRL16_DATA                  ;

      uint32_t  reg_dec_blmt_en                   ;
      uint32_t  reg_dec_blmt_op                   ;
      uint32_t  reg_dec_blmt_th                   ;
      uint32_t  reg_frm_h_width                   ;
		  uint32_t  CMPR_CTRL17_DATA                  ;

//
		  reg_pre_bypass_proc           = 0;
			reg_pre_bypass_vpu_pq         = 1;
			reg_test_patt_vtotal             ;
			reg_test_patt_htotal             ;

			reg_test_patt_enable          = 0;
			reg_test_patt_timing_slf_gen  = 0;
			reg_test_patt_mode            = 6;
			reg_test_patt_prede           = 0;
			reg_test_patt_vstart             ;
			reg_test_patt_hstart             ;

			reg_pre_bypass_od             = 1;
			reg_pre_bypass_scaler         = 0;
			reg_test_patt_vend               ;
			reg_test_patt_hend               ;

		  reg_in_patt_enable               ;
		  reg_in_patt_timing_slf_gen       ;
		  reg_vs_sel                  = 3  ;//reg_vs_sel[1] = 1,TMG use regen vs to clr if TMG de_i to de_o delay line is more than expect
		  reg_test_patt_vs_start      = (v_tot_o-scaler_o_v)/2;
		  reg_test_patt_vs_end        = (v_tot_o-scaler_o_v)/2 + 1;

      reg_pre_clk_sel               = 0;//1:in_clk 0:pix_clk
			reg_scaler_after_buf_en       = 0;//need fw
			reg_pre_hs_dly                = 0;
			reg_pre_proc_gate_clk_ctrl    = 2;//clock gate sel
			reg_pre_dither_gate_clk_ctrl  = 2;//clock gate sel
			reg_pre_pix_clk_div4_en       = 0;
			reg_pre_in_clk_sel            = 0;//0:in_clk 1:pix_clk 2:vp_opt_clk
			reg_pre_vp_clk_mux            = 1;//0:in_clk 1:pix_clk 2:vp_opt_clk
			reg_pre_proc_clk_en           = 1;
			reg_pre_dither_clk_en         = 1;
			reg_pre_od_clk_en             = 1;
			reg_pre_data_clk_en           = 1;

      reg_mute_force_freerun        = 0;
		  reg_mute_force_normal         = 1;
		  reg_mute_vtotal                  ;
		  reg_mute_htotal                  ;

      reg_mute_httl_src_sel         = 0;
			reg_mute_hs_det_en            = 0;
			reg_mute_de_w1c_en            = 0;
			reg_mute_de_det_en            = 0;
			reg_mute_vact                    ;
			reg_mute_hact                    ;

      reg_mute_stmq_timer           = 2;
			reg_mute_stme_timer           = 2;
			reg_mute_vbporch                 ;
			reg_mute_hbporch                 ;

      reg_mute_st_clkabn_reset_en   = 0;
			reg_mute_st_clk_as_ref        = 0;
			reg_mute_clk_w1c_en           = 0;
			reg_mute_clk_det_en           = 1;
			reg_mute_clk_freq_low_th      = 1;
			reg_mute_vwidth               = 4;//used to control free run vs position
			reg_mute_hwidth               = 8;//used to control free run hs position

      reg_in_vid_mute_enable       = 0;
			reg_patt_enable              = 0;
			reg_patt_loop_number         = 511;
			reg_patt_blending_alpha      = 0;
			reg_mute_st_sf2n_option      = 0;
			reg_mute_st_vs_as_ref        = 0;
			reg_mute_st_manu_reset       = 0;
			reg_mute_st_de_as_ref        = 0;
			reg_mute_clk_freq_high_th    = 0;

       reg_retimer_v_dly_mode      = 0;
		   reg_retimer_h_dly_num       = 0;
		   reg_retimer_v_dly_num          ;

       reg_retimer_video_on_vs_lath_sel  = 0;
			 reg_retimer_tmg_en                = 1;
			 reg_retimer_video_on              = 1;

       reg_level_min                     = 1840;
		   reg_level_max                     = 5840;

       reg_pid_cal_sel                   = 0;//0: ref clock pid adjust 1: ref fifo level
		   reg_err_sum_thr_p1                = 20;//fifo_level - max/min,
		   reg_sum_sft_p1                    = 13;//fifo_level_sum in reg_pid_cal_cycle
		   reg_err_thr_p1                    = 20;//fifo_level_avg delta value

       reg_pix_count_exp_p0              = 0;
			 reg_pid_cal_cycle_p1              = 0;

       reg_hs_regen                      = 1;
		   reg_clip_sel                      = 1;
		   reg_crc_en                        = 0;
		   reg_crc_sel                       = 0;
		   reg_htot_adj_en                   = 0;
		   reg_pll_adj_en                    = 1;
		   reg_adjust_start                  = 0;
		   reg_adjust_end                    = 0;

		   reg_pll_frac_in                   = 0;
		   reg_pll_m_in                      = 0;

		   reg_fifo_level_err_thr_max        = 7500;
		   reg_fifo_level_err_thr_min        = 300;

		   reg_mute_vs_sel                   = 1;//1:mute module regen vs 0: use input vs
		   reg_mute_vs_end                   = 5;
		   reg_mute_vs_start                 = 4;

		   reg_no_de_thr                     = 60000;

		   reg_pix_pll_frac_in               = 0;
		   reg_pix_pll_m_in                  = 0;

		   reg_multi_factor_p0               = 0;
		   reg_cur_err_sum_thr_p0            = 20;

		   reg_err_thr_p0                    = 10;

		   reg_ki_coef_p0                    = 2048;
		   reg_kp_coef_p0                    = 1024;

		   reg_ki_coef_p1                    = 2048;
		   reg_kp_coef_p1                    = 2048;

		   reg_level_delta_exp_p1            = 0;
		   reg_multi_factor_p1               = 0;

       reg_edge_ctrl                     = 0; 
       reg_pls_frm_start                 = 0; 
       reg_frm_start_sel                 = 0; 
       reg_update_en                     = 1; 
       reg_ro_update_en                  = 1; 
       reg_hold_line_num                 = 4;//reg_retimer_v_dly_num;

       reg_htot_adj_max                  = 2400;
       reg_htot_adj_min                  = 1000;
       reg_retimer_vs_regen_start        = 4;
       reg_retimer_vs_regen_end          = 5;

      MNT_PPS_t pps;

printf("retimer code start = %x vactive = %x\n",scaler_in_h,scaler_in_v);
printf("retimer code start = %x vactive = %x\n",scaler_o_h,scaler_o_v);
/*
//OD CMPR width height register setting
//cmpr ctrl16
      addr             = VOUT_PRE_CMPR_CTRL16;
      reg_frm_v_width  = scaler_o_v;
      reg_mbw          = ((scaler_o_v+3)/4);
		  CMPR_CTRL16_DATA = reg_frm_v_width           << 12   |
				                 reg_mbw                           ;
      Wr(addr,CMPR_CTRL16_DATA);

//cmpr ctrl17
      addr             = VOUT_PRE_CMPR_CTRL17;
      reg_dec_blmt_en  = 0;
      reg_dec_blmt_op  = 1;
      reg_dec_blmt_th  = 100;
      reg_frm_h_width  = scaler_o_h;
		  CMPR_CTRL17_DATA = reg_frm_h_width           << 16   |
				                 reg_dec_blmt_en           << 11   |
                         reg_dec_blmt_op           << 10   |
                         reg_dec_blmt_th                   ;
      Wr(addr,CMPR_CTRL17_DATA);
*/
//reg initial
//vout top clock control
    Wr((VOUT_APB_BASE_ADDR+(0x8000)),0x8000);
    Wr((VOUT_APB_BASE_ADDR+(0x800c)),0x007f);

//pll adjust value setting
  while(pix_freq<1400000) 
  {
    pix_freq = pix_freq*2;
  }

 

  reg_pll_m_in        = pix_freq/24000;  //osin=24M
  reg_pll_frac_in     = (pix_freq-24000*reg_pll_m_in)*(131072)/24000; //pll_m*2 to align
  reg_pix_pll_m_in    = pix_freq/24000;  //osin=24M
  reg_pix_pll_frac_in = (pix_freq-24000*reg_pix_pll_m_in)*(131072)/24000; //pll_m*2 to align

  reg_htot_adj_max    = h_tot_o * (1+0.03);
  reg_htot_adj_min    = h_tot_o * (1-0.03);

  reg_pix_pll_adj_thr_max = ((reg_pix_pll_m_in << 17) + reg_pix_pll_frac_in)*(1+0.03);
  reg_pix_pll_adj_thr_min = ((reg_pix_pll_m_in << 17) + reg_pix_pll_frac_in)*(1-0.03);
  reg_pll_adj_thr_max     = ((reg_pll_m_in << 17) + reg_pll_frac_in)*(1+0.03);
  reg_pll_adj_thr_min     = ((reg_pll_m_in << 17) + reg_pll_frac_in)*(1-0.03); 

  reg_pid_cal_sel = adjust_mode;
//base clock adjust set
  if(scaler_o_v > scaler_in_v)
  {
    reg_pid_cal_cycle_p0 = 8/(scaler_o_v/scaler_in_v);
  }
  else
  {
    reg_pid_cal_cycle_p0 = 8*(scaler_in_v/scaler_o_v);
  }
  reg_pix_count_exp_p0 = (scaler_o_v/scaler_in_v) * h_tot_o * (reg_pid_cal_cycle_p0+1);
  value_64_0             =  value_64<<40;
  value_64_1             =  value_64_0/(reg_pix_count_exp_p0*reg_pix_count_exp_p0);
  reg_multi_factor_p0    =  value_64_1;

//base fifo level adjust set
  reg_sum_sft_p1         = 13;
  reg_pid_cal_cycle_p1   = 1<<13;
  reg_level_delta_exp_p1 = scaler_o_h * (reg_pid_cal_cycle_p1/v_tot_o);
  reg_multi_factor_p1    = (1<<24)/reg_level_delta_exp_p1;
  reg_adjust_start     = 1;
  reg_adjust_end       = scaler_o_v-1;

  if(scaler_in_h > scaler_o_h)//scaler down
  {
   printf("retimer scaler down = %x vactive = %x\n",scaler_in_h,scaler_in_v);
    reg_scaler_after_buf_en = 0;
    reg_pre_in_clk_sel      = 0;
    reg_pre_vp_clk_mux      = 0;
    reg_retimer_v_dly_num   = (7680/scaler_o_h)*(scaler_o_v/scaler_in_v) + 2;
  }
  else
  {
   printf("retimer scaler up = %x vactive = %x\n",scaler_in_h,scaler_in_v);
    reg_scaler_after_buf_en = 1;
    reg_pre_in_clk_sel      = 0;
    reg_pre_vp_clk_mux      = 1;
    reg_retimer_v_dly_num   = (7680/scaler_in_h) + 2;
  }

  reg_mute_htotal            = h_tot_i;
  reg_mute_vtotal            = v_tot_i;
  reg_mute_hact              = scaler_in_h;
  reg_mute_vact              = scaler_in_v;
  reg_mute_hbporch           = (h_tot_i - scaler_in_h)/2;
  reg_mute_vbporch           = (v_tot_i - scaler_in_v)/2;
//tmg output
  reg_test_patt_hstart       = (h_tot_o - scaler_o_h)/2;
  reg_test_patt_vstart       = (v_tot_o - scaler_o_v)/2;
  reg_test_patt_hend         = reg_test_patt_hstart + scaler_o_h;
  reg_test_patt_vend         = reg_test_patt_vstart + scaler_o_v;
  reg_test_patt_vtotal       = v_tot_o;
  reg_test_patt_htotal       = h_tot_o;

  if(in_patt_en==1)
{
  printf("vout in patt enable hactive = %x vactive = %x\n",scaler_in_h,scaler_in_v);

  reg_mute_force_freerun     = 0;
  reg_mute_force_normal      = force_mode;//bypass mute state
  reg_in_patt_enable         = in_patt_en;//in patt enable
  reg_in_patt_timing_slf_gen = in_patt_en;//in patt enable
}
else
{
  reg_mute_force_freerun     = 0;
  reg_mute_force_normal      = force_mode;//bypass mute state
  reg_in_patt_enable         = 0;//in patt enable
  reg_in_patt_timing_slf_gen = 0;//in patt enable
  printf("vout HDMI/DP timing enable hactive = %x vactive = %x\n",scaler_in_h,scaler_in_v);

}


			addr       = VOUT_PRE_TOP_CTRL0;
		  CTRL0_DATA = reg_pre_bypass_proc           << 31   |
				           reg_pre_bypass_vpu_pq         << 30   |
				           reg_test_patt_vtotal          << 15   |
				           reg_test_patt_htotal                  ;
      Wr(addr,CTRL0_DATA);

			addr       = VOUT_PRE_TOP_CTRL1;
			CTRL1_DATA = reg_test_patt_enable          << 31   |
				           reg_test_patt_timing_slf_gen  << 30   |
				           reg_test_patt_mode            << 26   |
				           reg_test_patt_prede           << 20   |
				           reg_test_patt_vstart          << 10   |
				           reg_test_patt_hstart                  ;
      Wr(addr,CTRL1_DATA);

			addr       = VOUT_PRE_TOP_CTRL2;
			CTRL2_DATA =	reg_pre_bypass_od             << 31   |
				            reg_pre_bypass_scaler         << 30   |
				            reg_test_patt_vend            << 15   |
				            reg_test_patt_hend                    ;
      Wr(addr,CTRL2_DATA);

			addr       = VOUT_PRE_TOP_CTRL3;
			CTRL3_DATA =	reg_in_patt_enable            << 31   |
				            reg_in_patt_timing_slf_gen    << 30   |
				            reg_vs_sel                    << 28   |
				            reg_test_patt_vs_start        << 14   |
				            reg_test_patt_vs_end                  ;
      Wr(addr,CTRL3_DATA);

      Wr(VOUT_PRE_TOP_CTRL4,0x20180601);
      Wr(VOUT_PRE_TOP_CTRL6,0x30000200);
      Wr(VOUT_PRE_TOP_CTRL12,0x0bff0008);

			addr       = VOUT_PRE_TOP_CTRL7;
			CTRL7_DATA =	reg_pre_clk_sel               << 31   |
				            reg_scaler_after_buf_en       << 30   |
				            reg_pre_hs_dly                << 17   |
				            reg_pre_proc_gate_clk_ctrl    << 15   |
				            reg_pre_dither_gate_clk_ctrl  << 11   |
				            reg_pre_pix_clk_div4_en       << 9    |
				            reg_pre_in_clk_sel            << 7    |
				            reg_pre_vp_clk_mux            << 5    |
				            reg_pre_proc_clk_en           << 4    |
				            reg_pre_dither_clk_en         << 2    |
				            reg_pre_od_clk_en             << 1    |
				            reg_pre_data_clk_en                   ;
      Wr(addr,CTRL7_DATA);

			addr        = VOUT_PRE_TOP_CTRL8;
		  CTRL8_DATA  = reg_mute_force_freerun        << 31   |
		              	reg_mute_force_normal         << 30   |
		              	reg_mute_vtotal               << 15   |
		              	reg_mute_htotal                       ;
      Wr(addr,CTRL8_DATA);
			
			addr        = VOUT_PRE_TOP_CTRL9;
		  CTRL9_DATA  = reg_mute_httl_src_sel         << 31   |
		                reg_mute_hs_det_en            << 30   |
		                reg_mute_de_w1c_en            << 29   |
		                reg_mute_de_det_en            << 28   |
		                reg_mute_vact                 << 14   |
		                reg_mute_hact                         ;
      Wr(addr,CTRL9_DATA);

			addr        = VOUT_PRE_TOP_CTRL10;
			CTRL10_DATA = reg_mute_stmq_timer           << 26   |
				            reg_mute_stme_timer           << 20   |
				            reg_mute_vbporch              << 10   |
				            reg_mute_hbporch                      ;
      Wr(addr,CTRL10_DATA);

			addr        = VOUT_PRE_TOP_CTRL11;
			CTRL11_DATA = reg_mute_st_clkabn_reset_en  <<  31   |
				            reg_mute_st_clk_as_ref       <<  30   |
				            reg_mute_clk_w1c_en          <<  29   |
				            reg_mute_clk_det_en          <<  28   |
				            reg_mute_clk_freq_low_th     <<  16   |
				            reg_mute_vwidth              <<   8   |
				            reg_mute_hwidth                       ;
      Wr(addr,CTRL11_DATA);


			addr        = VOUT_PRE_TOP_CTRL59;
			CTRL59_DATA = reg_retimer_v_dly_mode       <<  31   |
				            reg_retimer_h_dly_num        <<  16   |
				            reg_retimer_v_dly_num                 ;
      Wr(addr,CTRL59_DATA);

			addr        = VOUT_PRE_TOP_CTRL67;
			CTRL67_DATA = reg_level_min                << 13 |
				            reg_level_max                      ;
      Wr(addr,CTRL67_DATA);

			addr        = VOUT_PRE_TOP_CTRL68;
			CTRL68_DATA = reg_pid_cal_sel                   << 24 |
				            reg_err_sum_thr_p1                << 14 |
				            reg_sum_sft_p1                    << 10 |
				            reg_err_thr_p1                          ;
      Wr(addr,CTRL68_DATA);

			addr        = VOUT_PRE_TOP_CTRL69;
			CTRL69_DATA = reg_pix_count_exp_p0              << 16 |
				            reg_pid_cal_cycle_p1                    ;
      Wr(addr,CTRL69_DATA);

			addr        = VOUT_PRE_TOP_CTRL70;
			CTRL70_DATA = reg_pix_pll_adj_thr_max;
      Wr(addr,CTRL70_DATA);

			addr        = VOUT_PRE_TOP_CTRL71;
			CTRL71_DATA = reg_pll_adj_thr_max;
      Wr(addr,CTRL71_DATA);

			addr        = VOUT_PRE_TOP_CTRL73;
			CTRL73_DATA = reg_hs_regen                      << 29 |
                    reg_clip_sel                      << 27 |
				            reg_crc_en                        << 26 |
				            reg_crc_sel                       << 24 |
				            reg_htot_adj_en                   << 23 |
				            reg_pll_adj_en                    << 22 |
				            reg_adjust_start                  << 11 |
				            reg_adjust_end                          ;
      Wr(addr,CTRL73_DATA);

			addr        = VOUT_PRE_TOP_CTRL74;
			CTRL74_DATA = reg_pll_frac_in                   << 9  |
				            reg_pll_m_in                            ;
      Wr(addr,CTRL74_DATA);

			addr        = VOUT_PRE_TOP_CTRL75;
			CTRL75_DATA = reg_fifo_level_err_thr_max        << 13 |
				            reg_fifo_level_err_thr_min              ;
      Wr(addr,CTRL75_DATA);

			addr        = VOUT_PRE_TOP_CTRL78;
			CTRL78_DATA = reg_retimer_vs_regen_end          << 27 |
                    reg_retimer_vs_regen_start        << 23 |
                    reg_htot_adj_min                  << 11 |
                    reg_mute_vs_sel                   << 10 |
				            reg_mute_vs_end                   << 5  |
				            reg_mute_vs_start                       ;
      Wr(addr,CTRL78_DATA);

			addr        = VOUT_PRE_TOP_CTRL79;
			CTRL79_DATA = reg_no_de_thr                         ;
      Wr(addr,CTRL79_DATA);

      addr        = VOUT_PRE_TOP_CTRL80;
      CTRL80_DATA = reg_htot_adj_max                  << 13 |
                    reg_edge_ctrl                     << 12 |
                    reg_pls_frm_start                 << 11 |
                    reg_frm_start_sel                 << 10 |
                    reg_update_en                     <<  9 |
                    reg_ro_update_en                  <<  8 |
                    reg_hold_line_num                       ;
      Wr(addr,CTRL80_DATA);

			addr        = VOUT_PRE_TOP_CTRL81;
			CTRL81_DATA = reg_pix_pll_frac_in               << 9  |
				            reg_pix_pll_m_in                        ;
      Wr(addr,CTRL81_DATA);

			addr        = VOUT_PRE_TOP_CTRL82;
			CTRL82_DATA = reg_multi_factor_p0               << 16 |
				            reg_cur_err_sum_thr_p0                  ;
      Wr(addr,CTRL82_DATA);

			addr        = VOUT_PRE_TOP_CTRL83;
			CTRL83_DATA = reg_pid_cal_cycle_p0              << 10 |
				            reg_err_thr_p0                          ;
      Wr(addr,CTRL83_DATA);

			addr        = VOUT_PRE_TOP_CTRL84;
			CTRL84_DATA = reg_ki_coef_p0                    << 16 |
				            reg_kp_coef_p0                          ;
      Wr(addr,CTRL84_DATA);

			addr        = VOUT_PRE_TOP_CTRL85;
			CTRL85_DATA = reg_ki_coef_p1                    << 16 |
				            reg_kp_coef_p1                          ;
      Wr(addr,CTRL85_DATA);

			addr        = VOUT_PRE_TOP_CTRL86;
			CTRL86_DATA = reg_level_delta_exp_p1            << 16 |
				            reg_multi_factor_p1                     ;
      Wr(addr,CTRL86_DATA);
			addr        = VOUT_PRE_TOP_CTRL87;
      CTRL87_DATA = reg_pix_pll_adj_thr_min;
      Wr(addr,CTRL87_DATA);
			addr        = VOUT_PRE_TOP_CTRL88;
      CTRL88_DATA = reg_pll_adj_thr_min;
      Wr(addr,CTRL88_DATA);


// after intial video output on
      addr        = VOUT_PRE_TOP_CTRL60;
			CTRL60_DATA = reg_retimer_video_on_vs_lath_sel  << 2  |
				            reg_retimer_tmg_en                << 1  |
				            reg_retimer_video_on                    ;
      Wr(addr,CTRL60_DATA);

    pps.src_hsize = scaler_in_h;
    pps.src_vsize = scaler_in_v;
    pps.dst_hsize = scaler_o_h;
    pps.dst_vsize = scaler_o_v;

    pps.posthsc_en = scaler_in_h == scaler_o_h ? 0 : 1;
    pps.postvsc_en = scaler_in_v == scaler_o_v ? 0 : 1;

    pps.pps_dbg_en = 0;

    cfg_mnt_pps(0, &pps);

}


void vout_retimer_code_pps_padding(uint32_t scaler_in_h,uint32_t scaler_in_v,uint32_t h_tot_i,uint32_t v_tot_i,uint32_t scaler_o_h,uint32_t scaler_o_v,uint32_t pps_o_h,uint32_t pps_o_v,uint32_t h_tot_o,uint32_t v_tot_o,uint32_t in_patt_en,uint32_t force_mode,uint32_t adjust_mode,uint32_t pix_freq)
{ 
    uint32_t reg_pps_pad_vstart ;
    uint32_t reg_pps_pad_hstart ;
    uint32_t reg_pps_pad_vend   ;
    uint32_t reg_pps_pad_hend   ;

    reg_pps_pad_vstart   = (pps_o_v - scaler_o_v)/2;
    reg_pps_pad_hstart   = (pps_o_h - scaler_o_h)/2;
    reg_pps_pad_vend     = reg_pps_pad_vstart + scaler_o_v - 1;
    reg_pps_pad_hend     = reg_pps_pad_hstart + scaler_o_h - 1;

    //output padding 
    Wr_reg_bits(VOUT_PPS_TOP_CTRL    ,1                  ,4 ,1 );//reg_padd_en    
    Wr_reg_bits(VOUT_PPS_SCALE_PADD_H,reg_pps_pad_hstart ,16,13);
    Wr_reg_bits(VOUT_PPS_SCALE_PADD_H,reg_pps_pad_hend   ,0 ,13);
    Wr_reg_bits(VOUT_PPS_SCALE_PADD_V,reg_pps_pad_vstart ,16,13);
    Wr_reg_bits(VOUT_PPS_SCALE_PADD_V,reg_pps_pad_vend   ,0 ,13);

    vout_retimer_code(scaler_in_h,scaler_in_v,h_tot_i,v_tot_i,pps_o_h,pps_o_v,h_tot_o,v_tot_o,in_patt_en,force_mode,adjust_mode,pix_freq);
}




void vout_lut_dma_aptu_trig_init(){
    Wr(VOUT_LUT_DMA_CTRL0,(Rd(VOUT_LUT_DMA_CTRL0)|(1<<6)));//little_endian=1
    Wr(VOUT_LUT_DMA_CTRL5,0xf41fe000);//aptu baddr
    //Wr(VOUT_LUT_DMA_CTRL5,0xf4020000);//aptu baddr
    Wr(VOUT_LUT_DMA_CTRL8,2000);//aptu stride
    Wr(VOUT_LUT_DMA_CTRL10,(0+(1099<<16)));//aptu x_start,x_end
    Wr(VOUT_PRE_TOP_CTRL45,((1<<20)+(1<<19)+(1<<16)+(36<<4)));

}

void vout_lut_dma_check()
{
    uint32_t data_tmp;
    uint32_t i, j, addr, data;

     data = Rd(VOUT_APB_BASE_ADDR+(0x207<<2));
     Wr(VOUT_APB_BASE_ADDR+(0x207<<2),(0x7ff) | data);

     // lut0
     for (i=0;i<3;i++) {
       Wr(VOUT_APB_BASE_ADDR+(0x601<<2),1<<(0+i));
       for (j=0;j<289;j++) {
         data_tmp = Rd(VOUT_APB_BASE_ADDR+((0x700+j)<<2));
         //printf("vout lut0 od rd %x = %x\n", j, data_tmp);
       }
     }
    // lut1
     for (i=0;i<3;i++) {
       Wr(VOUT_APB_BASE_ADDR+(0x601<<2),1<<(3+i));
       for (j=0;j<272;j++) {
         data_tmp = Rd(VOUT_APB_BASE_ADDR+((0x700+j)<<2));
         //printf("vout lut1 od rd %x = %x\n", j, data_tmp);
       }
     }
    // lut2
     for (i=0;i<3;i++) {
       Wr(VOUT_APB_BASE_ADDR+(0x601<<2),1<<(6+i));
       for (j=0;j<272;j++) {
         data_tmp = Rd(VOUT_APB_BASE_ADDR+((0x700+j)<<2));
         //printf("vout lut2 od rd %x = %x\n", j, data_tmp);
       }
     }
    // lut3
     for (i=0;i<3;i++) {
       Wr(VOUT_APB_BASE_ADDR+(0x601<<2),1<<(9+i));
       for (j=0;j<256;j++) {
         data_tmp = Rd(VOUT_APB_BASE_ADDR+((0x700+j)<<2));
         //printf("vout lut3 od rd %x = %x\n", j, data_tmp);
       }
     }
     Wr(VOUT_APB_BASE_ADDR+(0x601<<2),0);
}
void vout_rdma_test_reg(int test_num,uint32_t exp_data)
{
     uint32_t i;
     uint32_t j;
     uint32_t data_tmp;
     uint32_t addr, width,mask;
     //test all the reg wr and rd of atcon
     for(i=0;i<test_num;i++){
       addr = (0x40+i)<<2;
       mask = 0xffffffff;
       data_tmp = Rd(VOUT_APB_BASE_ADDR+addr);
       if (data_tmp != (mask&exp_data)) {
             printf("VOUT_ERROR vout reg test %x data %x\n", addr>>2, (mask&exp_data));
        }else{
             printf("RIGHT vout reg test %x data %x\n", addr>>2, (mask&exp_data));
        }
    }
}

//dump_mode : 0:dump 8bitsx3(rgb), 1:dump 10bitsx3(rgb), other:dump r[11:2]
void vout_dump_cfg(uint32_t baddr,uint32_t width,uint32_t height,  uint32_t h_start, uint32_t h_end, uint32_t v_start, uint32_t v_end, uint32_t dump_mode)
{
    uint32_t dump_en = 1;
    uint32_t dump_start;
    uint32_t blk_num_m1 = (h_end-h_start+1)*(v_end-v_start+1)-1; 
    uint32_t in_width;

    if(dump_mode ==0){
      in_width = 24;
    }else if(dump_mode ==1){
      in_width = 30;
    }else{
      in_width = 10;
    }

    Wr(VOUT_PRE_CMPR_CTRL1,  baddr);
    Wr(VOUT_PRE_CMPR_CTRL2,  0xb33e83e8);
    Wr(VOUT_PRE_CMPR_CTRL3,  0x8008007c|(in_width<<24));
    Wr(VOUT_PRE_CMPR_CTRL4,  0x1cdf07d0);
    Wr(VOUT_PRE_CMPR_CTRL5,  0x9e08007c);
    Wr(VOUT_PRE_CMPR_CTRL12, dump_en<<21);
    Wr(VOUT_PRE_CMPR_CTRL16, height<<12);
    Wr(VOUT_PRE_CMPR_CTRL17, width<<16);
    Wr(VOUT_PRE_CMPR_CTRL18, blk_num_m1);
    dump_start =1;
    Wr(VOUT_PRE_CMPR_CTRL31, (dump_start<<31)+(dump_mode<<28)+(v_start<<14)+(h_start));
    Wr(VOUT_PRE_CMPR_CTRL32, (v_end<<14)+(h_end));
    dump_start =0;
    Wr(VOUT_PRE_CMPR_CTRL31, (dump_start<<31)+(dump_mode<<28)+(v_start<<14)+(h_start));

}

void vout_dump_clr_done(void)
{
    Wr(VOUT_PRE_CMPR_CTRL31,(Rd(VOUT_PRE_CMPR_CTRL31)|(1<<30)));
    Wr(VOUT_PRE_CMPR_CTRL31,(Rd(VOUT_PRE_CMPR_CTRL31)&(0xbfffffff)));
}

void vout_line_irq_en(void)
{
    Wr(VOUT_PRE_TOP_CTRL6, (Rd(VOUT_PRE_TOP_CTRL6)&0xfffffdff)|(1<<9));
    Wr(VOUT_PRE_TOP_CTRL45,(1<<19)+(6<<4)+(1<<16));
}

void vout_cmpr_cfg(uint32_t baddr, uint32_t width,uint32_t height,uint32_t cursor_sel,uint32_t hds2_en)
{
    uint32_t data_tmp;
    uint32_t i, addr, data;
    uint8_t rd_setting;
    uint32_t blk_num_m1;
    size_t len;

    len = sizeof(vout_cmpr_base_setting)/sizeof(vout_cmpr_base_setting[0]);
    for (i=0;i<len/2;i=i+1)  {
      addr = vout_cmpr_base_setting[i*2];
      data = vout_cmpr_base_setting[i*2+1];
      Wr((VOUT_APB_BASE_ADDR+(addr<<2)),data);
    }
    if(hds2_en ==1){
      blk_num_m1 = ((width/2+3)/4)*((height+3)/4)-1; 
    }else{
      blk_num_m1 = ((width+3)/4)*((height+3)/4)-1; 
    }

    Wr(VOUT_PRE_CMPR_CTRL1, baddr);
    Wr(VOUT_PRE_CMPR_CTRL16, (height<<12)+((width+3)/4));
    Wr(VOUT_PRE_CMPR_CTRL17, (width<<16));
    Wr(VOUT_PRE_CMPR_CTRL18, blk_num_m1);
    Wr(VOUT_PRE_CMPR_CTRL5, (Rd(VOUT_PRE_CMPR_CTRL5)&0xbfffffff)|(hds2_en<<30));
    Wr(VOUT_PRE_TOP_CTRL5, (Rd(VOUT_PRE_TOP_CTRL5)&0x0fffffff)|(cursor_sel<<28));

    if((height&0x2)!=0){//height/4 !=0
    Wr(VOUT_PRE_CMPR_CTRL14, (Rd(VOUT_PRE_CMPR_CTRL14)&0xff1fffff)|(2<<21));
    Wr(VOUT_PRE_CMPR_CTRL9, 0x1cafff);//ring_buf_len_m1
    Wr(VOUT_PRE_CMPR_CTRL10, 0x180000);//addr_frm_ofst
    }else{
    //for cmpr buf optimization
    Wr(VOUT_PRE_CMPR_CTRL9, 0x1cafff);//ring_buf_len_m1
    Wr(VOUT_PRE_CMPR_CTRL10, 0x150000);//addr_frm_ofst
    }
}

void vout_cmpr_rc_mode2_cfg(uint32_t baddr, uint32_t width,uint32_t height,uint32_t cursor_sel,uint32_t hds2_en)
{
    uint32_t data_tmp;
    uint32_t i, addr, data;
    uint8_t rd_setting;
    uint32_t blk_num_m1;
    size_t len;

    len = sizeof(vout_cmpr_rc_mode2_base_setting)/sizeof(vout_cmpr_rc_mode2_base_setting[0]);
    for (i=0;i<len/2;i=i+1)  {
      addr = vout_cmpr_rc_mode2_base_setting[i*2];
      data = vout_cmpr_rc_mode2_base_setting[i*2+1];
      Wr((VOUT_APB_BASE_ADDR+(addr<<2)),data);
    }
    if(hds2_en ==1){
      blk_num_m1 = ((width/2+3)/4)*((height+3)/4)-1; 
    }else{
      blk_num_m1 = ((width+3)/4)*((height+3)/4)-1; 
    }

    Wr(VOUT_PRE_CMPR_CTRL1, baddr);
    Wr(VOUT_PRE_CMPR_CTRL16, (height<<12)+((width+3)/4));
    Wr(VOUT_PRE_CMPR_CTRL17, (width<<16));
    Wr(VOUT_PRE_CMPR_CTRL18, blk_num_m1);
    Wr(VOUT_PRE_CMPR_CTRL5, (Rd(VOUT_PRE_CMPR_CTRL5)&0xbfffffff)|(hds2_en<<30));
    Wr(VOUT_PRE_TOP_CTRL5, (Rd(VOUT_PRE_TOP_CTRL5)&0x0fffffff)|(cursor_sel<<28));

    if((height&0x2)!=0){//height/4 !=0
    Wr(VOUT_PRE_CMPR_CTRL14, (Rd(VOUT_PRE_CMPR_CTRL14)&0xff1fffff)|(2<<21));
    }
}

void vout_cmpr_rc_mode3_cfg(uint32_t baddr, uint32_t width,uint32_t height,uint32_t cursor_sel,uint32_t hds2_en)
{
    uint32_t data_tmp;
    uint32_t i, addr, data;
    uint8_t rd_setting;
    uint32_t blk_num_m1;
    size_t len;

    len = sizeof(vout_cmpr_rc_mode3_base_setting)/sizeof(vout_cmpr_rc_mode3_base_setting[0]);
    for (i=0;i<len/2;i=i+1)  {
      addr = vout_cmpr_rc_mode3_base_setting[i*2];
      data = vout_cmpr_rc_mode3_base_setting[i*2+1];
      Wr((VOUT_APB_BASE_ADDR+(addr<<2)),data);
    }
    if(hds2_en ==1){
      blk_num_m1 = ((width/2+3)/4)*((height+3)/4)-1; 
    }else{
      blk_num_m1 = ((width+3)/4)*((height+3)/4)-1; 
    }

    Wr(VOUT_PRE_CMPR_CTRL1, baddr);
    Wr(VOUT_PRE_CMPR_CTRL16, (height<<12)+((width+3)/4));
    Wr(VOUT_PRE_CMPR_CTRL17, (width<<16));
    Wr(VOUT_PRE_CMPR_CTRL18, blk_num_m1);
    Wr(VOUT_PRE_CMPR_CTRL5, (Rd(VOUT_PRE_CMPR_CTRL5)&0xbfffffff)|(hds2_en<<30));
    Wr(VOUT_PRE_TOP_CTRL5, (Rd(VOUT_PRE_TOP_CTRL5)&0x0fffffff)|(cursor_sel<<28));

    if((height&0x2)!=0){//height/4 !=0
    Wr(VOUT_PRE_CMPR_CTRL14, (Rd(VOUT_PRE_CMPR_CTRL14)&0xff1fffff)|(2<<21));
    }
}


void vout_rdma_lut_test()
{
    int j;
    int32_t data_tmp;
       Wr(VOUT_APB_BASE_ADDR+(0x609<<2),1);
       for (j=0;j<256;j++) {
         data_tmp = Rd(VOUT_APB_BASE_ADDR+((0x700+j)<<2));
           if (data_tmp != rdma_lut[3+j*2]) {
             printf("VOUT_ERROR dither RAM, test addr %x, rd_data = %x\n",j,data_tmp);
           }else {
             //printf("RIGHT vout dither RAM, test addr %x, rd_data = %x\n",j,data_tmp);
           }
       }
       Wr(VOUT_APB_BASE_ADDR+(0x609<<2),0);
}


void vout_vx1_pll_self_test()
{
  int32_t ro_pix_freq;
  int32_t i;
   uint32_t pix_clk_measure_val[10] = {7716,7801,7887,7973,8060,8144,8230,8317,8401,8486};
  Wr_reg_bits(ANACTRL_TCON_PLL_VLOCK,0,0,1);
  Wr_reg_bits(ANACTRL_TCON_PLL_VLOCK,1,2,1);
  Wr_reg_bits(ANACTRL_TCON_PLL_VLOCK,0,5,1);


  Wr_reg_bits(ANACTRL_TCON_PLL0_CNTL0,450,0,9);   //int:433->450
  Wr_reg_bits(ANACTRL_TCON_PLL0_CNTL1,17384,0,17);//frac:16384->17384
  Wr_reg_bits(VOUT_PRE_TOP_CTRL81,1,27,1);
  //delay_us(1);
  Wr_reg_bits(VOUT_PRE_TOP_CTRL81,1,28,1);
  //delay_us(1);
  Wr_reg_bits(VOUT_PRE_TOP_CTRL81,0,28,1);
  Wr_reg_bits(VOUT_PRE_TOP_CTRL61,0,31,1);//clac_clk disable
  Wr_reg_bits(VOUT_PRE_TOP_CTRL61,1000,0,20);//period is 10us
  Wr_reg_bits(VOUT_PRE_TOP_CTRL61,1,31,1);//clac_clk enable
  //delay_us(20);
  for(i=0;i<10;i++){
  Wr_reg_bits(ANACTRL_TCON_PLL0_CNTL0,450+i*5,0,9);   
  Wr_reg_bits(ANACTRL_TCON_PLL0_CNTL1,17384+i*200,0,17);
  Wr_reg_bits(VOUT_PRE_TOP_CTRL81,1,28,1);
  //delay_us(1);
  Wr_reg_bits(VOUT_PRE_TOP_CTRL81,0,28,1);
  //delay_us(20);
  ro_pix_freq =  Rd(VOUT_PRE_TOP_CTRL63);
  printf("vout_pix_clk freq cnt: %d is %d\n",i ,ro_pix_freq);
  if((ro_pix_freq <pix_clk_measure_val[i]+2)&&(ro_pix_freq>pix_clk_measure_val[i]-2)) 
  { printf("[AOCPU-OCRAM]:vout vx1_pll pix_clk is right!\n");}
  else
  { printf("[AOCPU-OCRAM]:PLL_ERROR: vout vx1_pll pix_clk is overflow or underflow!\n");}

  }

}

void vout_pix_pll_self_test()
{
  int32_t ro_pix_freq;
  int32_t i;
   uint32_t pix_clk_measure_val[10] = {8641,8042,7441,6841,6241,5641,5041,4441,3842,3241};
  Wr_reg_bits(ANACTRL_PIX_PLL_CTRL3,0,0,1);
  Wr_reg_bits(ANACTRL_PIX_PLL_CTRL3,1,2,1);
  Wr_reg_bits(ANACTRL_PIX_PLL_CTRL3,0,1,1);


  Wr_reg_bits(ANACTRL_PIX_PLL_CTRL2,72,0,9);   //int:433->450
  Wr_reg_bits(ANACTRL_PIX_PLL_CTRL1,17384,0,17);//frac:16384->17384
  Wr_reg_bits(VOUT_PRE_TOP_CTRL81,1,27,1);
  //delay_us(1);
  Wr_reg_bits(VOUT_PRE_TOP_CTRL81,1,28,1);
  //delay_us(1);
  Wr_reg_bits(VOUT_PRE_TOP_CTRL81,0,28,1);
  Wr_reg_bits(VOUT_PRE_TOP_CTRL61,0,31,1);//clac_clk disable
  Wr_reg_bits(VOUT_PRE_TOP_CTRL61,1000,0,20);//period is 10us
  Wr_reg_bits(VOUT_PRE_TOP_CTRL61,1,31,1);//clac_clk enable
  //delay_us(20);
  for(i=0;i<10;i++){
  Wr_reg_bits(ANACTRL_PIX_PLL_CTRL2,72-i*5,0,9);   
  Wr_reg_bits(ANACTRL_PIX_PLL_CTRL1,17384+i*500,0,17);
  Wr_reg_bits(VOUT_PRE_TOP_CTRL81,1,28,1);
  //delay_us(1);
  Wr_reg_bits(VOUT_PRE_TOP_CTRL81,0,28,1);
  //delay_us(20);
  ro_pix_freq =  Rd(VOUT_PRE_TOP_CTRL63);
  printf("vout_pix_clk freq cnt: %d is %d\n",i ,ro_pix_freq);
  if((ro_pix_freq <pix_clk_measure_val[i]+2)&&(ro_pix_freq>pix_clk_measure_val[i]-2)) 
  { printf("[AOCPU-OCRAM]:vout pix_pll pix_clk is right!\n");}
  else
  { printf("[AOCPU-OCRAM]:PLL_ERROR: vout pix_pll pix_clk is overflow or underflow!\n");}

  }

}
