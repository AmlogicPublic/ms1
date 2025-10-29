// #ifdef WIN32
// #include <windows.h>
// #include <process.h>
// #else
// #include <pthread.h>
// #endif
// #pragma comment(lib,"USBIOX.LIB")
//
// #include <stdio.h>
// #include "USBIOX.h"
// #include	<windows.h>
#include <stdio.h>
#include <stdlib.h>

// #include	<conio.h>
// #include	<winioctl.h>
//
// #include	"../inc/USBIOX.H"
// #include    "../inc/rw_reg_i2c.h"
// #include <unistd.h>
#include "edp_tx_cfg.h"
#include "vpu_include.h"

static uint32_t mIndex = 1;
static uint8_t mDevAddr = 10; // 16;
static int current_page = 0;

static int print_every_addr = 0;

static int do_have_usb_i2c = 1;
static uint8_t *fake_register;
static int fake_rw_print = 0;
static char key_info[8] = {0x14, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static unsigned long tmp[3];

void edp_write_reg_nobase(uint32_t addr, uint32_t data) {
  printf("write addr = %x\n", addr);
  printf("write data = %x\n", data);
  key_info[1] = ((addr << 2) >> 16) & 0xff;
  key_info[2] = ((addr << 2) >> 8) & 0xff;
  key_info[3] = (addr << 2) & 0xff;
  key_info[4] = data & 0xff;
  key_info[5] = (data >> 8) & 0xff;
  key_info[6] = (data >> 16) & 0xff;
  key_info[7] = (data >> 24) & 0xff;
  USBIO_StreamI2C(0x00, 0x8, &key_info[0], 0x00, &tmp[2]);
  /*else {
          USBIO_StreamI2C(0x00,0x4,&key_info[0],0x04,&tmp[2]);
          printf("return read data %lx\n",tmp[2]);
  }*/
}
void edp_write_reg(uint32_t addr, uint32_t data) {
  printf("write addr = %x\n", addr);
  printf("write data = %x\n", data);
  //   uint32_t    base_addr  =   0x4000;
  uint32_t base_addr = 0x11000;
  printf("base addr = %x\n", base_addr);
  uint32_t a_addr = base_addr + addr;
  printf("actual addr = %x\n\n", a_addr);
  key_info[1] = ((a_addr << 2) >> 16) & 0xff;
  key_info[2] = ((a_addr << 2) >> 8) & 0xff;
  key_info[3] = (a_addr << 2) & 0xff;
  key_info[4] = data & 0xff;
  key_info[5] = (data >> 8) & 0xff;
  key_info[6] = (data >> 16) & 0xff;
  key_info[7] = (data >> 24) & 0xff;
  USBIO_StreamI2C(0x00, 0x8, &key_info[0], 0x00, &tmp[2]);
  /*else {
          USBIO_StreamI2C(0x00,0x4,&key_info[0],0x04,&tmp[2]);
          printf("return read data %lx\n",tmp[2]);
  }*/
}

uint32_t edp_read_reg(uint32_t addr) {
  printf("read addr = %x\n", addr);
  // uint32_t    base_addr  =   0x4000;
  uint32_t base_addr = 0x11000;
  printf("base addr = %x\n", base_addr);
  uint32_t a_addr = base_addr + addr;
  printf("actual addr = %x\n\n", a_addr);
  uint32_t read_data;
  key_info[1] = ((a_addr << 2) >> 16) & 0xff;
  key_info[2] = ((a_addr << 2) >> 8) & 0xff;
  key_info[3] = (a_addr << 2) & 0xff;
  USBIO_StreamI2C(0x00, 0x4, &key_info[0], 0x04, &read_data);
  printf("return read data %x\n\n", read_data);
  return read_data;
}

void edp_aux_wait_reply() {
  uint32_t temp;
  uint32_t mask_reg;
  /*temp    =   edp_read_reg(0x00000144);//read mask
  mask_reg    =   temp;
  temp    &=  0xFFFFFFF3;
  edp_write_reg(0x00000144,temp);//write mask
  */
  usleep(450);                     // delay 450us
  temp = edp_read_reg(0x00000130); // read interrupt state
  if ((temp >> 3) && 0x1)
    printf("error\n");
  if ((temp >> 2) && 0x1) {
  } else
    printf("error\n");

  temp = edp_read_reg(0x00000140); // read interrupt cause
  if ((temp >> 3) && 0x1)
    printf("error\n");
  if ((temp >> 2) && 0x1) {
  } else
    printf("error\n");

  usleep(10);                      // delay1
  temp = edp_read_reg(0x00000140); // read interrupt cause
  if ((temp >> 3) && 0x1)
    printf("error\n");
  if ((temp >> 2) && 0x1)
    printf("error\n");

  // edp_write_reg(0x00000144,mask_reg);//write mask
  temp = edp_read_reg(0x00000138); // read reply code
  temp &= 0x0000000F;
  if (temp != 0)
    printf("error reply code = %x\n", temp);
}

void edp_aux_write(uint32_t aux_addr, uint32_t num, uint32_t *aux_warray) {
  if (num > 16)
    return;
  // sent data
  edp_write_reg(0x00000108, aux_addr);
  // edp_write_reg(0x00000108,0xa5a5a5a5);
  int i;
  for (i = 0; i < num; i++) {
    edp_write_reg(0x00000104, aux_warray[i]);
  }
  edp_write_reg(0x00000100, (0x0800 + num - 1));
  // reply
  edp_aux_wait_reply();
}
void edp_aux_read(uint32_t aux_addr, uint32_t num, uint32_t *aux_rarray) {
  if (num > 16)
    return;
  // sent command
  edp_write_reg(0x00000108, aux_addr);
  edp_write_reg(0x00000100, (0x0900 + num - 1));
  // wait reply
  edp_aux_wait_reply();
  // get read data
  int i;
  for (i = 0; i < num; i++) {
    aux_rarray[i] = edp_read_reg(0x00000134); // read data
  }
}

void edp_read_capaility() {
  uint32_t aux_rarray[16];
  uint32_t aux_warray[16];
  edp_aux_read(0x00000000, 16, aux_rarray);
  for (int i = 0; i < 16; i++) {
    printf("DPCD[0000%x] = %x\n", i, aux_rarray[i]);
  }
}
void edp_tx_cfg() {
  unsigned char buffer[2];
  // unsigned int test = 0;
  unsigned int w_test = 0;
  unsigned long test = 0;
  int j = 0;
  int i = 0;

  char *p_w;
  char *p;
  char data_buff[6] = {0x14, 0xfc, 0x01, 0x00, 0x00, 0x00};

  char data_read[2] = {0x14, 0x04};
  char test_addr[2] = {0x14, 0x00};
  char test_addr1[2] = {0x14, 0x08}; //{0x14,0x02};
  char test_addr2[2] = {0x14, 0x14}; //{0x14,0x05};

  char test_addr3[2] = {0x14, 0x20}; //{0x14,0x08};
  char test_addr4[2] = {0x14, 0x24}; //{0x14,0x09};
  char test_addr5[2] = {0x14, 0xa0}; //{0x14,0x28};
  char test_addr6[2] = {0x14, 0xb0}; //{0x14,0x2c};

  char test_addr7[2] = {0x14, 0x10}; //{0x14,0x04};
  char test_addr8[2] = {0x14, 0x94}; //{0x14,0x25};

  char test_zw[8] = {0x14, 0x40, 0x00, 0x12, 0x34, 0x56, 0x78, 0x00};
  char test_zw2[8] = {0x14, 0x40, 0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0x00};
  char test_zw3[8] = {0x14, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00};

  char tcon_tope[7] = {0x14, 0x40, 0x38, 0x1f, 0x00, 0x00, 0x00};
  char tcon_top0[7] = {0x14, 0x40, 0x00, 0x99, 0x1d, 0x00, 0x00};
  char tcon_top9[7] = {0x14, 0x40, 0x24, 0x3b, 0x00, 0x00, 0x00};
  char tcon_top9_2[7] = {0x14, 0x40, 0x24, 0x3b, 0x00, 0x00, 0x00};
  char tcon_topb[7] = {0x14, 0x40, 0x2c, 0x3f, 0x00, 0x00, 0x00};
  char tcon_topb_2[7] = {0x14, 0x40, 0x2c, 0x00, 0x00, 0x00, 0x00};
  char test_status[7] = {0x14, 0x40, 0x20, 0x50, 0x00, 0x00, 0x00};
  int key_addr = 0;
  int key_addr_offset_msb = 0;
  int key_addr_offset_lsb = 0;
  int write_data_x_bits = 0;
  int mask = 0;
  int mask_data = 0;

  int write_data_by_offset = 0;
  char key_wr = 0;
  int key_data = 0;
  int file_data;
  int file_addr;
  FILE *fp_fiti;
  FILE *fp_lut;
  FILE *fp_bwidth;
#ifdef DEBUG_WITHOUT_USBI2C

#else
  if (init_reg_rw() != 0)
    return -1;
#endif
  // USBIO_SetStream(0,1);
  int readbuf_cs602init[42];
  int test_buf[1] = {0};
  for (i = 0; i < 42; i++)
    readbuf_cs602init[i] = 0;

  while (1) {
    printf("input operation: \n\t0-read,\n\t1-write\n\t2-test_flow\n\t3-read_aux\n\t4-write_aux\n");
    scanf("%hhd", &key_wr);
    printf("you entered %d\n", key_wr);
    uint32_t aux_rarray[16];
    uint32_t aux_warray[16];
    if (key_wr == 4) {
      uint32_t inaddr;
      uint32_t indata;
      printf("input write DPCD addr\n");
      scanf("%x", &inaddr);
      printf("you entered %x\n", inaddr);
      printf("input write DPCD data\n");
      scanf("%x", &indata);
      printf("you entered %x\n", indata);
      edp_aux_write(inaddr, 1, &indata);
    } else if (key_wr == 3) {
      uint32_t inaddr;
      uint32_t rdata;
      printf("input read DPCD addr\n");
      scanf("%x", &inaddr);
      printf("you entered %x\n", inaddr);
      edp_aux_read(inaddr, 1, &rdata);
      printf("read DPCD[%x] = %x\n", inaddr, rdata);
    } else if (key_wr == 2) {

      uint32_t lane_num = 2;
      edp_read_capaility();

      edp_write_reg(0x0000010c, 0x00000019);
      edp_read_reg(0x0000010c);
      edp_write_reg(0x00000080, 0x00000001);
      edp_read_reg(0x00000080);

      edp_write_reg(0x00000000, 0x0000000a);
      edp_read_reg(0x00000000);
      edp_write_reg(0x00000004, lane_num);
      edp_read_reg(0x00000004);

      uint32_t temp;
      uint32_t mask_reg;
      temp = edp_read_reg(0x00000144); // read mask
      temp &= 0xFFFFFFF3;
      edp_write_reg(0x00000144, temp); // write mask

      aux_warray[0] = 0xa;
      edp_aux_write(0x00000100, 1, aux_warray);
      aux_warray[0] = lane_num;
      edp_aux_write(0x00000101, 1, aux_warray);
      edp_aux_read(0x00000100, 1, aux_rarray);
      printf("[DPCD 0x00100] = %x\n", aux_rarray[0]);
      edp_aux_read(0x00000101, 1, aux_rarray);
      printf("[DPCD 0x00101] = %x\n", aux_rarray[0]);

      aux_warray[0] = 1;
      edp_aux_write(0x00000600, 1, aux_warray);
      edp_aux_read(0x00000600, 1, aux_rarray);
      printf("[DPCD 0x00600] = %x\n", aux_rarray[0]);

      edp_write_reg(0x00000014, 0x00000000);
      edp_write_reg(0x0000000c, 0x00000001);
      aux_warray[0] = 0x21;
      edp_aux_write(0x00000102, 1, aux_warray);
      edp_aux_read(0x00000102, 1, aux_rarray);
      printf("[DPCD 0x00102] = %x\n", aux_rarray[0]);

      uint32_t times;
      times = 5;
      aux_rarray[0] = 0;
      aux_rarray[1] = 0;

      //      while(times>0 && aux_rarray[0] != 0x11 && aux_rarray[1] != 0x00){
      while (times > 0 && aux_rarray[0] != 0x11) {
        usleep(100);
        edp_aux_read(0x00000206, 2, aux_rarray);
        printf("[DPCD 0x00206] = %x\n[DPCD 0x00207] = %x\n", aux_rarray[0], aux_rarray[1]);
        edp_aux_read(0x00000202, 2, aux_rarray);
        printf("[DPCD 0x00202] = %x\n[DPCD 0x00203] = %x\n", aux_rarray[0], aux_rarray[1]);
        times--;
      }
      if (times == 0)
        printf("PATTERN 1 timeout\n");

      times = 5;
      aux_rarray[0] = 0;
      aux_rarray[1] = 0;
      aux_rarray[2] = 0;
      edp_write_reg(0x0000000c, 0x00000002);
      aux_warray[0] = 0x22;
      edp_aux_write(0x00000102, 1, aux_warray);
      //     while(times>0 && aux_rarray[0] != 0x77 && aux_rarray[1] != 0x00 && aux_rarray[2] != 1){
      while (times > 0) {
        if (aux_rarray[0] == 0x77 && aux_rarray[2] == 0x01)
          break;

        //       while(times>0 && aux_rarray[0] != 0x77  && aux_rarray[2] != 1){
        usleep(400);
        edp_aux_read(0x00000206, 2, aux_rarray);
        printf("[DPCD 0x00206] = %x\n[DPCD 0x00207] = %x\n", aux_rarray[0], aux_rarray[1]);
        edp_aux_read(0x00000202, 3, aux_rarray);
        printf("[DPCD 0x00202] = %x\n[DPCD 0x00203] = %x\n[DPCD 0x00204] = %x\n", aux_rarray[0], aux_rarray[1], aux_rarray[2]);
        times--;
      }
      if (times == 0)
        printf("PATTERN 2 timeout\n");

      edp_write_reg(0x0000000c, 0x00000000);
      aux_warray[0] = 0x00;
      edp_aux_write(0x00000102, 1, aux_warray);

      // config pattern
      //   edp_write_reg_nobase(0x1014,0x05c90aa0); //2560x1440
      //   edp_write_reg_nobase(0x1016,0x05ab0a14); //2560x1440
      //   edp_write_reg_nobase(0x1018,0x01ff01ff);
      //   edp_write_reg_nobase(0x1019,0x000001ff);
      //   edp_write_reg_nobase(0x1015,0xa840200f); //1080
      //   edp_write_reg_nobase(0x1015,0xa8402c14); //2560x1440

      edp_write_reg_nobase(0x1034, 0x60000000);
      edp_write_reg_nobase(0x1033, 0x00000000);
      edp_write_reg_nobase(0x10100, 0x00000007);
      edp_write_reg_nobase(0x10101, 0x5926631c);
      edp_write_reg_nobase(0x10102, 0x00006363);
      edp_write_reg_nobase(0x10103, 0x00136363);
      edp_write_reg_nobase(0x10104, 0xcf008000);
      edp_write_reg_nobase(0x10105, 0x2d49ca0f);
      edp_write_reg_nobase(0x10106, 0x1cd62907);
      edp_write_reg_nobase(0x10107, 0x0c418820);
      edp_write_reg_nobase(0x10109, 0x87809111);
      edp_write_reg_nobase(0x1010a, 0x000043c0);
      edp_write_reg_nobase(0x10200, 0x42328898);
      edp_write_reg_nobase(0x10201, 0xa840200f);
      edp_write_reg_nobase(0x10202, 0xc220078f);
      edp_write_reg_nobase(0x10203, 0x00008006);
      edp_write_reg_nobase(0x10204, 0x20180601);
      edp_write_reg_nobase(0x10206, 0x30000200);
      edp_write_reg_nobase(0x10207, 0x00010017);
      edp_write_reg_nobase(0x10208, 0x42328898);
      edp_write_reg_nobase(0x10209, 0x010e0780);
      edp_write_reg_nobase(0x1020a, 0x08202850);
      edp_write_reg_nobase(0x1020b, 0x1fff0208);
      edp_write_reg_nobase(0x1020c, 0x0bff0008);
      edp_write_reg_nobase(0x1020d, 0x0008ffff);
      edp_write_reg_nobase(0x1020e, 0x01006000);
      edp_write_reg_nobase(0x1020f, 0x03ff0009);
      edp_write_reg_nobase(0x10211, 0x00000009);
      edp_write_reg_nobase(0x10212, 0x000003ff);
      edp_write_reg_nobase(0x10213, 0x00000009);
      edp_write_reg_nobase(0x10214, 0x03ff0000);
      edp_write_reg_nobase(0x10215, 0x1e8c0003);
      edp_write_reg_nobase(0x10217, 0xc6880001);
      edp_write_reg_nobase(0x10218, 0x000000fa);
      edp_write_reg_nobase(0x10219, 0x0c140002);
      edp_write_reg_nobase(0x10222, 0x54070000);
      edp_write_reg_nobase(0x10223, 0x32800500);
      edp_write_reg_nobase(0x10224, 0x010e0000);
      edp_write_reg_nobase(0x10225, 0x0003800c);
      edp_write_reg_nobase(0x1023c, 0x00000003);
      edp_write_reg_nobase(0x1023d, 0x800186a0);
      edp_write_reg_nobase(0x10040, 0xd246c806);
      edp_write_reg_nobase(0x10041, 0x3fff6db6);
      edp_write_reg_nobase(0x10042, 0x70488028);
      edp_write_reg_nobase(0x10043, 0x80c5821c);
      edp_write_reg_nobase(0x10044, 0x40000410);
      edp_write_reg_nobase(0x10045, 0x02818014);
      edp_write_reg_nobase(0x10047, 0x00000046);
      edp_write_reg_nobase(0x10062, 0xb33e83e8);
      edp_write_reg_nobase(0x10063, 0x9808007c);
      edp_write_reg_nobase(0x10064, 0x1cdf07d0);
      edp_write_reg_nobase(0x10065, 0x8df007d0);
      edp_write_reg_nobase(0x10069, 0x000228ff);
      edp_write_reg_nobase(0x1006a, 0x00022800);
      edp_write_reg_nobase(0x1006b, 0x000000f0);
      edp_write_reg_nobase(0x1006c, 0x00040008);
      edp_write_reg_nobase(0x1006e, 0x0004c488);

      edp_write_reg_nobase(0x1006f, 0x10282828);
      edp_write_reg_nobase(0x10070, 0x004381e0);
      edp_write_reg_nobase(0x10071, 0x07800000);
      edp_write_reg_nobase(0x10072, 0x0001fa3f);
      edp_write_reg_nobase(0x1007c, 0x0840d855);
      edp_write_reg_nobase(0x10076, 0x04050067);
      edp_write_reg_nobase(0x1007b, 0x04619064);
      edp_write_reg_nobase(0x1007c, 0x08c0d855);
      edp_write_reg_nobase(0x1007d, 0x0a203041);
      edp_write_reg_nobase(0x1007e, 0x0000400a);
      edp_write_reg_nobase(0x1016, 0x8440078f);
      edp_write_reg_nobase(0x1018, 0x01ff01ff);
      edp_write_reg_nobase(0x1019, 0x000001ff);
      edp_write_reg_nobase(0x1015, 0xb040200f);
      edp_write_reg_nobase(0x1036, 0x120d08fc);
      edp_write_reg_nobase(0x1033, 0x40000000);

      // config IP video  1080p 148.5m
      edp_write_reg(0x000820, 0x00000898);
      edp_write_reg(0x000824, 0x00000465);
      edp_write_reg(0x000828, 0x00000003);
      edp_write_reg(0x00082c, 0x00000005); // 44
      edp_write_reg(0x000830, 0x00000002);
      edp_write_reg(0x000834, 0x00000780); // 1920
      edp_write_reg(0x000838, 0x00000438); // 1080
      edp_write_reg(0x00083c, 0x00000008); // h start 192
      edp_write_reg(0x000840, 0x00000003); // v start 41
      edp_write_reg(0x000848, 0x00000000);
      edp_write_reg(0x00084c, 0x00003a02); // 14850
      //    edp_write_reg(0x00084c,0x00001d01);//14850
      edp_write_reg(0x000850, 0x00000040);
      edp_write_reg(0x00085c, 0x00000870); // 2160
      edp_write_reg(0x000864, 0x0000000f);
      edp_write_reg(0x000854, 0x00006978);
      edp_write_reg(0x000858, 0x00000001);

      edp_write_reg(0x000850, 0x09270040);
      edp_write_reg(0x000810, 0x00000606);
      edp_write_reg(0x000094, 0x00000001);
      edp_write_reg(0x000800, 0x00000001);
      edp_write_reg(0x000a00, 0x00000000);
      edp_write_reg(0x000c00, 0x00000000);
      edp_write_reg(0x000e00, 0x00000000);
      edp_write_reg(0x0000c0, 0x00000001);

      /*      //config IP video 1440p 214.71m
            edp_write_reg(0x000820,0x00000aa0);
            edp_write_reg(0x000824,0x000005c9);
            edp_write_reg(0x000828,0x00000003);
            edp_write_reg(0x00082c,0x00000005);//44
            edp_write_reg(0x000830,0x00000002);
            edp_write_reg(0x000834,0x00000a00);//1920
            edp_write_reg(0x000838,0x000005a0);//1080
            edp_write_reg(0x00083c,0x00000008);//h start 192
            edp_write_reg(0x000840,0x0000000c);//v start 41
            edp_write_reg(0x000848,0x00000000);
            edp_write_reg(0x00084c,0x00005e6c);//14850
            edp_write_reg(0x000850,0x00000040);
            edp_write_reg(0x00085c,0x000005a0);//2160
            edp_write_reg(0x000864,0x0000000f);
            edp_write_reg(0x000854,0x00006978);
            edp_write_reg(0x000858,0x00000001);


            edp_write_reg(0x000850,0x03200040);
            edp_write_reg(0x000810,0x00000604);
            edp_write_reg(0x000094,0x00000001);
            edp_write_reg(0x000800,0x00000001);
            edp_write_reg(0x000a00,0x00000000);
            edp_write_reg(0x000c00,0x00000000);
            edp_write_reg(0x000e00,0x00000000);
            edp_write_reg(0x0000c0,0x00000001);*/

      // config IP video 1440p 148.5m
      /*    edp_write_reg(0x000820,0x00000aa0);
          edp_write_reg(0x000824,0x000005c9);
          edp_write_reg(0x000828,0x00000003);
          edp_write_reg(0x00082c,0x00000005);
          edp_write_reg(0x000830,0x00000002);
          edp_write_reg(0x000834,0x00000a00);
          edp_write_reg(0x000838,0x000005a0);
          edp_write_reg(0x00083c,0x00000008);
          edp_write_reg(0x000840,0x0000000c);
          edp_write_reg(0x000848,0x00000000);
          edp_write_reg(0x00084c,0x00003a02);//14850
          edp_write_reg(0x000850,0x00000040);
          edp_write_reg(0x00085c,0x000005a0);
          edp_write_reg(0x000864,0x0000000f);
          edp_write_reg(0x000854,0x00006978);
          edp_write_reg(0x000858,0x00000001);


          edp_write_reg(0x000850,0x0c130040);
          edp_write_reg(0x000810,0x00000604);
          edp_write_reg(0x000094,0x00000001);
          edp_write_reg(0x000800,0x00000001);
          edp_write_reg(0x000a00,0x00000000);
          edp_write_reg(0x000c00,0x00000000);
          edp_write_reg(0x000e00,0x00000000);
          edp_write_reg(0x0000c0,0x00000001);*/

    } else {

      printf("input 24bit hex address\n");
      scanf("%x", &key_addr);
      printf("you entered %x \n", key_addr);
      key_info[1] = ((key_addr << 2) >> 16) & 0xff;
      key_info[2] = ((key_addr << 2) >> 8) & 0xff;
      key_info[3] = (key_addr << 2) & 0xff;

      if (key_wr) {
        printf("input 32bit data\n");
        scanf("%x", &key_data);
        printf("you entered %x\n", key_data);
        // USBIO_StreamI2C(0x00,0x4,&key_info[0],0x04,&tmp[2]);
        // printf("read before write %x\n",tmp[2]);
        key_info[4] = key_data & 0xff;
        key_info[5] = (key_data >> 8) & 0xff;
        key_info[6] = (key_data >> 16) & 0xff;
        key_info[7] = (key_data >> 24) & 0xff;
        USBIO_StreamI2C(0x00, 0x8, &key_info[0], 0x00, &tmp[2]);
      } else {
        USBIO_StreamI2C(0x00, 0x4, &key_info[0], 0x04, &tmp[2]);
        printf("return read data %lx\n", tmp[2]);
      }
    }
  }
}

void apb_write_bits(int apb_addresses, int apb_data, int start_bits, int bits_num) {
  int i;
  int apb_rdata;
  int bits_vld;
  unsigned int bits_mask;
  int bits_data;
  int key_data;

  for (i = 0; i <= 32; i++) {
    if (bits_num == 32)
      bits_vld = 0xffffffff;
    else if (i == bits_num)
      bits_vld = (1 << i) - 1;
  }

  for (i = 0; i < 32; i++) {
    if (i == start_bits) {
      bits_mask = ~(bits_vld << i);
      bits_data = (apb_data & bits_vld) << i;
    }
  }
  key_info[1] = ((apb_addresses << 2) >> 16) & 0xff;
  key_info[2] = ((apb_addresses << 2) >> 8) & 0xff;
  key_info[3] = (apb_addresses << 2) & 0xff;
  // rd
  if ((start_bits == 0) & (bits_num == 0x20)) {
    key_data = apb_data;
  } else {
    USBIO_StreamI2C(0x00, 0x4, &key_info[0], 0x04, &tmp[2]);
    printf("return read data %lx\n", tmp[2]);
    // wr
    key_data = ((tmp[2] & bits_mask) | bits_data);
  }
  key_info[4] = key_data & 0xff;
  key_info[5] = (key_data >> 8) & 0xff;
  key_info[6] = (key_data >> 16) & 0xff;
  key_info[7] = (key_data >> 24) & 0xff;
  USBIO_StreamI2C(0x00, 0x8, &key_info[0], 0x00, &tmp[2]);
  USBIO_StreamI2C(0x00, 0x4, &key_info[0], 0x04, &tmp[2]);
  USBIO_StreamI2C(0x00, 0x8, &key_info[0], 0x00, &tmp[2]);
  printf("return write  data %x\n", key_data);
}

// CSPI
void cspi_v03_pmic_init() {
  char test_infomat; // add by fxj to stop dos infomation
  unsigned char buffer[2];
  unsigned long wr_test[4];
  int i;
  unsigned long tmp[3];
  int key_addr = 0;
  char key_wr = 0;
  int key_data = 0;
  //	char key_info[7] = {0x14,0x40,0x00,0x00,0x00,0x00,0x00};
  char key_info[7] = {0xe8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};

  unsigned char reg_addr;
  unsigned long reg_data;
  char pmic[14] = {0x40, // dev addr
                   0x1,  // start addr 1
                   0x1B, // 0x28,  //val
                   // 0xf,
                   0x5,
                   0x0,
                   0x0,
                   0xb,
                   0x2,
                   0x23, // 0x25,
                   0x4,  // 0x8,
                   0x4,
                   0x0,
                   0x1, // 0x4,
                   0x0};

  /*	int buf20800_vol[20] = {847, //1+
                                     833, //2+
                                     0x0, //3
                                     738, //4+
                                     652, //5+
                                     615, //6+
                                     0x0, //7
                                     530, //8+
                                     461, //9+
                                     375, //10+
                                     306,  //11+
                                     0x0,  //12
                                     227,  //13+
                                     173,  //14+
                                     101, //15+
                                     0x0,  //16
                                     59,  //17+
                                     3,   //18+
                                     0x187, //19
                                     0x17d};//20*/

  int buf20800_vol[20] = {0x3c3,  // 1+
                          0x3b3,  // 2+
                          0x0,    // 3
                          0x348,  // 4+
                          0x2e8,  // 5+
                          0x2bd,  // 6+
                          0x0,    // 7
                          0x25a,  // 8+
                          0x20c,  // 9+
                          0x1ad,  // 10+
                          0x15c,  // 11+
                          0x0,    // 12
                          0x104,  // 13+
                          0xc5,   // 14+                       xzxzxzxzxzxzxzxzxzxzxzxzxzxzxzxzxzxz
                          0x74,   // 15+
                          0x0,    // 16
                          0x43,   // 17+
                          0x03,   // 18+
                          0x187,  // 19
                          0x17d}; // 20

  int rt6937_vol[8] = {0x0c,  // VDD
                       0x01,  // VCC1
                       0x03,  // VCC2
                       0x03,  // 0x0d,//HVDD
                       0x09,  // VGH_LT
                       0x08,  // VGH_HT
                       0x2a,  // VGL
                       0x07}; // VGL2
  // init_reg_rw();

  // test_usb_i2c_func();
  // printf("hello world\n");

  wr_test[0] = 0x0000ff40;
  wr_test[1] = 0x0040;
  wr_test[2] = 0x0080ff40;
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);

  // gamma_writecommandbytes(0x0,0x4,&wr_test[0]);
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);
  buffer[0] = 0;
  buffer[1] = 0;

  /*
          // pmic
  USBIO_StreamI2C(0x0,  14, &pmic[0], 0, &buffer[0]);
  USBIO_StreamI2C(0x0,  3, &wr_test[0], 0, &buffer[0]);
  USBIO_StreamI2C(0x0,  3, &wr_test[2], 0, &buffer[0]);
  */

  // write buf20800
  wr_test[0] = 0x3f0007e8;
  for (i = 0; i < 20; i++) {
    wr_test[0] = 0;
    wr_test[0] = (((buf20800_vol[i] & 0xff) << 24) | (((buf20800_vol[i] >> 8) & 0xf) << 16) | (i << 8) | 0xe8);
    gamma_writecommandbytes(0x0, 0x4, &wr_test[0]);
  }

  // write rt6937
  for (i = 0; i < 8; i++) {
    wr_test[0] = 0;
    wr_test[0] = ((0x0 << 24) | ((rt6937_vol[i] & 0xff) << 16) | (i << 8) | 0x40);
    gamma_writecommandbytes(0x0, 0x4, &wr_test[0]);
  }
  wr_test[0] = ((0x0 << 24) | ((0x80) << 16) | (0xff << 8) | 0x40);
  gamma_writecommandbytes(0x0, 0x4, &wr_test[0]);

  /*while(1){
          printf("please input reg addr \n");
          scanf("%x",&reg_addr);
          printf("you entered %x\n",reg_addr);
          reg_data = rt6937_readcommandbytes(reg_addr);
          printf("the reg data of offset %x is  %x\n",reg_addr,(reg_data&0xff));
  }*/
}

void cspi_v03_load_setting() {
  unsigned char buffer[2];
  // unsigned int test = 0;
  unsigned int w_test = 0;
  unsigned long test = 0;

  int j = 0;
  int i = 0;

  char *p_w;
  char *p;
  char data_buff[6] = {0x14, 0xfc, 0x01, 0x00, 0x00, 0x00};
  // unsigned long tmp[2];
  unsigned long tmp[3];

  // char  data_buff[6]  = {0x14,0x04,0x00,0x00,0x00,0x00};
  // char  data_buff2[6] = {0x14,0x04,0x00,0x00,0x00,0x00};

  char data_read[2] = {0x14, 0x04};
  // char  test_addr[2]   = {0x14,0xfc};
  // char  test_addr1[2]   = {0x14,0x00};
  char test_addr[2] = {0x14, 0x00};
  char test_addr1[2] = {0x14, 0x08}; //{0x14,0x02};
  char test_addr2[2] = {0x14, 0x14}; //{0x14,0x05};

  char test_addr3[2] = {0x14, 0x20}; //{0x14,0x08};
  char test_addr4[2] = {0x14, 0x24}; //{0x14,0x09};
  char test_addr5[2] = {0x14, 0xa0}; //{0x14,0x28};
  char test_addr6[2] = {0x14, 0xb0}; //{0x14,0x2c};

  char test_addr7[2] = {0x14, 0x10}; //{0x14,0x04};
  char test_addr8[2] = {0x14, 0x94}; //{0x14,0x25};

  char test_zw[8] = {0x14, 0x40, 0x00, 0x12, 0x34, 0x56, 0x78, 0x00};
  char test_zw2[8] = {0x14, 0x40, 0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0x00};
  char test_zw3[8] = {0x14, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00};

  char tcon_tope[7] = {0x14, 0x40, 0x38, 0x1f, 0x00, 0x00, 0x00};
  char tcon_top0[7] = {0x14, 0x40, 0x00, 0x99, 0x1d, 0x00, 0x00};
  char tcon_top9[7] = {0x14, 0x40, 0x24, 0x3b, 0x00, 0x00, 0x00};
  char tcon_top9_2[7] = {0x14, 0x40, 0x24, 0x3b, 0x00, 0x00, 0x00};
  /*char tcon_top9[7] = {0x14,0x40,0x24,0x39,0x00,0x00,0x00};
  char tcon_top9_2[7] = {0x14,0x40,0x24,0x39,0x00,0x00,0x00};*/
  char tcon_topb[7] = {0x14, 0x40, 0x2c, 0x3f, 0x00, 0x00, 0x00};
  char tcon_topb_2[7] = {0x14, 0x40, 0x2c, 0x00, 0x00, 0x00, 0x00};
  char test_status[7] = {0x14, 0x40, 0x20, 0x50, 0x00, 0x00, 0x00};
  char key_info[7] = {0x14, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};
  int key_addr = 0;
  int key_addr_offset_msb = 0;
  int key_addr_offset_lsb = 0;
  int write_data_x_bits = 0;
  int mask = 0;
  int mask_data = 0;

  int write_data_by_offset = 0;
  char key_wr = 0;
  int key_data = 0;
  // char test_zw[2] = {0x14,0x40,0x00};
  int file_data;
  int file_addr;
  FILE *fp_fiti;
  FILE *fp_lut;
  //
  // fp_fiti = fopen("fiti_1080.txt","r");
  // fp_fiti = fopen("fiti_setting_20170727_no_od.txt","r");
  // fp_fiti = fopen("fiti_setting_20170809_tcl.txt","r");
  // fp_fiti = fopen("fiti_setting_20170816_tcl_od3.txt","r");
  // fp_fiti = fopen("fiti_setting_20170824_tcl_zigzag_invod_255.txt","r");
  // fp_fiti = fopen("fiti_setting_20170825_tcl_zigzag_invod_00.txt","r");
  // fp_fiti = fopen("UHD_setting_cspi_20190516.txt","r");
  // fp_fiti = fopen("UHD_setting_14_case108_20191118.txt","r");
  // fp_fiti = fopen("UHD_setting_cspi_20190909.txt","r");
  // fp_fiti = fopen("UHD_setting_cspi_20190919_zw.txt","r");

  // fp_fiti = fopen("./data/setting_cspi_1113_svn25162_golden.txt","r");
  // fp_fiti = fopen("./data/UHD_setting_11_case108_20191114.txt","r");
  // fp_fiti = fopen("./data/PRJ_UHD_setting_cspi_case530_FHDinUHDout_videoin_20200511_2.txt","r");
  // fp_fiti = fopen("./data/PRJ_UHD_setting_cspi_case530_FHDinUHDout_videoin_20200518.txt","r");
  // fp_fiti = fopen("./data/PRJ_UHD_setting_cspi_case530_FHDinUHDout_videoin_20200523_test.txt","r");//ud60 golden
  // fp_fiti = fopen("./data/PRJ_UHD_setting_cspi_case530_FHDinUHDout_videoin_20200523_dvi.txt","r");
  // fp_fiti = fopen("./data/PRJ_UHD_setting_cspi_case530_FHDinUHDout_videoin_20200523_dvi_gam.txt","r");

  // t3x   0x
  // fp_fiti = fopen("./data/PRJ_UHD_setting_cspi_case530_FHDinUHDout_videoin_20200523_test.txt","r");//ud60 golden
  // fp_fiti = fopen("./data/uhd120_case630_cspi_t3x_20220705.txt","r");
  // fp_fiti = fopen("./data/uhd120_case630_cspi_t3x_new_clk_20220722.txt","r");   // uhd120 golden

  // fp_fiti = fopen("./data/uhd120_case630_cspi_t3x_new_clk_dvi_20220722.txt","r");
  // fp_fiti = fopen("./data/uhd120_case630_cspi_t3x_new_clk_20220801_pol5.txt","r");
  // fp_fiti = fopen("./data/svn1173_avac_20220801_setting.txt","r");
  // fp_fiti = fopen("./data/svn1173_avac_20220801_setting.txt","r");
  // fp_fiti = fopen("./data/uhd120_case630_cspi_t3x_new_clk_dvi_demura_20220722.txt","r");
  // fp_fiti = fopen("./data/case630_cspi_vac_20220802_setting.txt","r");
  // fp_fiti = fopen("./data/uhd120_cspi_case630_setting_20220803.txt","r");
  // fp_fiti = fopen("./data/uhd120_case630_cspi_vac_new_clk_20220803_setting_dvi.txt","r");

  // lge t3e
  // fp_fiti = fopen("./data/PRJ_UHD_setting_cspi_case530_FHDinUHDout_videoin_20200523_test.txt","r");//ud60 golden
  // fp_fiti = fopen("./data/PRJ_UHD_setting_cspi_case530_FHDinUHDout_videoin_20200523_dvi_gam.txt","r");//ud60 golden
  // fp_fiti = fopen("./data/uhd60_lge_cspi_case630_setting_20221114.txt","r");//ud60 golden
  // fp_fiti = fopen("./data/PRJ_UHD_setting_cspi_case530_FHDinUHDout_videoin_20221114_vac_test.txt","r");//ud60 golden
  // fp_fiti = fopen("./data/uhd60_lge_cspi_case630_setting_20221115.txt","r"); // test ok
  // fp_fiti = fopen("./data/case630_lge_cspi_vac_20221115.setting.txt","r");
  // fp_fiti = fopen("./data/case630_lge_cspi_vac_20221116.setting_v1.txt","r"); //ok
  // fp_fiti = fopen("./data/case630_lge_cspi_vac_20221116.setting_test.txt","r"); //ok

  // fp_fiti = fopen("./data/uhd120_cspi_t3x_svn_avac_20220725_dvi_setting.txt","r");

  // fp_fiti = fopen("./data/uhd120_cspi_t3x_case631_setting_new_clk_20220804.txt","r");

  // fp_fiti = fopen("./data/case626_vac_before_od_20231129_test_lut.txt","r");  // only test

  // d17_trigate_cspi
  // fp_fiti = fopen("./data/trigate_cspi_d17_1_4port2pair_setting_timing.txt","r");
  // fp_fiti = fopen("./data/t3e_case621_trigate_cspi_d17_1_4port2pair_231017.txt","r");
  // fp_fiti = fopen("./data/case621_trigate_cspi_d17_1_4port2pair_231018.txt","r");
  // fp_fiti = fopen("./data/case622_trigate_cspi_d17_1_4port2pair_168P75M_231019.txt","r");
  // fp_fiti = fopen("./data/case622_trigate_cspi_d17_1_4port2pair_168P75M_231019_2.txt","r");
  // fp_fiti = fopen("./data/case622_trigate_cspi_d17_1_4port2pair_168P75M_231020_2.txt","r");
  // fp_fiti = fopen("./data/case622_trigate_cspi_d17_1_4port2pair_168P75M_231023.txt","r");
  // fp_fiti = fopen("./data/case622_trigate_cspi_d17_1_4port2pair_168P75M_231023_2.txt","r");
  // fp_fiti = fopen("./data/case622_trigate_cspi_d17_1_4port2pair_168P75M_231024.txt","r");//
  // fp_fiti = fopen("./data/case622_trigate_cspi_d17_1_4port2pair_168P75M_231024_2.txt","r");
  // fp_fiti = fopen("./data/case623_trigate_cspi_d17_1_4port2pair_156P25M_231025.txt","r");
  // fp_fiti = fopen("./data/case622_trigate_cspi_d17_1_4port2pair_168P75M_lod_lut_1027.txt","r");
  // fp_fiti = fopen("./data/case622_tri_cspi_168p75m_lod_lut_20231030_2.txt","r");
  // fp_fiti = fopen("./data/case623_tri_cspi_156p25m_lod_lut_20231030_2.txt","r");
  // fp_fiti = fopen("./data/case623_tri_cspi_156p25m_lod_lut_20231030_2_gain204.txt","r"); // t5 8bit
  // fp_fiti = fopen("./data/case622_trigate_cspi_d17_1_4port2pair_168P75M_231101.txt","r");
  // fp_fiti = fopen("./data/case622_trigate_cspi_d17_1_4port2pair_168P75M_goa_rgd_231101.txt","r");
  // fp_fiti = fopen("./data/case623_trigate_cspi_d17_1_4port2pair_156P25M_rgd_231102.txt","r");
  // fp_fiti = fopen("./data/case623_trigate_cspi_d17_1_4port2pair_156P25M_231103.txt","r");
  // fp_fiti = fopen("./data/case623_tri_cspi_156p25m_lod_lut_20231106.txt","r");
  // fp_fiti = fopen("./data/case623_tri_cspi_156p25m_lod_lut_t5_20231106.txt","r"); // 10bit
  // fp_fiti = fopen("./data/case623_tri_cspi_156p25m_20231107.txt","r");
  // fp_fiti = fopen("./data/case6231_tri_cspi_156p25m_lod_lut_20231108.txt","r");
  // fp_fiti = fopen("./data/case623_trigate_cspi_d17_1_4port2pair_156P25M_goa_rgd_231109.txt","r");

  // fp_fiti = fopen("./data/case626_vac_before_od_20231129_setting_v3.txt","r");

  // pt550gs03-1
  // fp_fiti = fopen("./data/uhd60_case640_hkc550gs03_1_6Port2Pair_isp_setting_20240508.txt","r");

  // hkc430gt03_1
  // fp_fiti = fopen("./data/uhd60_case641_fpga_hkc430gt03_1_6P2Pr_isp_setting20240511.txt","r");
  // fp_fiti = fopen("./data/uhd60_case641_fpga_hkc430gt03_1_6P2Pr_isp_setting20240513.txt","r");
  // fp_fiti = fopen("./data/uhd60_case641_fpga_hkc430gt03_1_6P2Pr_isp_setting20240514.txt","r");
  // fp_fiti = fopen("./data/uhd60_case641_fpga_hkc430gt03_1_6P2Pr_isp_setting20240514_posttiming_ok_setting.txt","r");
  // fp_fiti = fopen("./data/uhd60_case641_fpga_hkc430gt03_1_6P2Pr_isp_setting20240514_oldfpgaaddr_for_oldbit.txt","r");
  // fp_fiti = fopen("./data/uhd60_case641_fpga_hkc430gt03_1_6P2Pr_isp_setting20240520_shsc.txt","r");
  // fp_fiti = fopen("./data/uhd60_case642_fpga_hkc430gt03_1_shsc_setting_2.txt","r");
  // fp_fiti = fopen("./data/uhd60_case641_t5m_test.txt","r");
  fp_fiti = fopen("./data/uhd60_case641_t5m_test_2.txt", "r");
  // vrr test
  // fp_fiti = fopen("./data/uhd60_case641_fpga_vrr_test.txt","r");

  // d12-8
  // fp_fiti = fopen("./data/case650_ud120_dlg_20240529_120hz_setting.txt","r");
  // fp_fiti = fopen("./data/case650_ud120_dlg_20240529_60hz_setting.txt","r");
  // fp_fiti = fopen("./data/uhd60_case641_fpga_hkc430gt03_1_6P2Pr_isp_setting20240514_posttiming_ok_setting_p2p_dbg.txt","r");

  if (fp_fiti == NULL)
    printf("error while opening file fiti_setting\n");

  for (i = 0;; i++) {
    // if((fscanf(fp_fiti,"%x, %x\n",&file_addr,&file_data))<0) break;
    if ((fscanf(fp_fiti, "0x%x, 0x%x,\n", &file_addr, &file_data)) < 0)
      break;
    test_zw3[1] = 0;
    test_zw3[2] = ((file_addr << 2) >> 8) & 0xff;
    test_zw3[3] = (file_addr << 2) & 0xff;
    test_zw3[4] = file_data & 0xff;
    test_zw3[5] = (file_data & 0xff00) >> 8;
    test_zw3[6] = (file_data & 0xff0000) >> 16;
    test_zw3[7] = (file_data & 0xff000000) >> 24;

    printf("i = %x, addr = %x data = %x\n", (i << 2), file_addr, file_data);
    USBIO_StreamI2C(0x00, 0x8, &test_zw3[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &test_zw3[0], 0x04, &tmp[2]);
  }
}

static int gamma_writecommandbytes(int index, int inbbytes, unsigned long *pucdatabuffer) {
  int i2c_flag = 0;
  unsigned int i = 0;
  unsigned int i2c_try_cnt = 10;
  unsigned char rwlen = 0;
  // unsigned char buffer[40];

  i2c_flag = USBIO_StreamI2C(index, inbbytes, &pucdatabuffer[0], rwlen, &pucdatabuffer[0]);

repeat:
  // i2c_flag = i2c_transfer(si2176->adapter, msg, 1);
  if (i2c_flag == 0) {
    printf("  error in write sli2176, %d byte(s) should be read,. \n", inbbytes);
    if (i++ < i2c_try_cnt) {
      printf("error in wirte sli2176, try again!!!\n");
      goto repeat;
    } else
      return -1;
  } else {
    // printf("  write %d bytes\n" , inbbytes);
    return inbbytes;
  }
}
static int wr1byte(int index, int inbbytes, unsigned long *pucdatabuffer) {
  int i2c_flag = 0;
  unsigned int i = 0;
  unsigned int i2c_try_cnt = 10;
  unsigned char rwlen = 0;
  // unsigned char buffer[40];

  // i2c_flag = USBIO_WriteI2C(index,  inbbytes, &pucdatabuffer[0], rwlen, &pucdatabuffer[0]);

repeat:
  // i2c_flag = i2c_transfer(si2176->adapter, msg, 1);
  if (i2c_flag == 0) {
    printf("  error in write sli2176, %d byte(s) should be read,. \n", inbbytes);
    if (i++ < i2c_try_cnt) {
      printf("error in wirte sli2176, try again!!!\n");
      goto repeat;
    } else
      return -1;
  } else {
    // printf("  write %d bytes\n" , inbbytes);
    return inbbytes;
  }
}
unsigned long rt6937_readcommandbytes(unsigned char regaddr) {
  int i2c_flag = 0;
  unsigned int index = 0;
  unsigned long pucdatabuffer;

  // unsigned char dev_addr = 0xe8;
  unsigned long rt6937_cmd1, rt6937_cmd2, rt6937_cmd3;
  rt6937_cmd1 = 0x40 | (0xff) << 8 | (0x00) << 16;
  rt6937_cmd2 = 0x40 | regaddr << 8;
  rt6937_cmd3 = 0x40;
  i2c_flag = USBIO_StreamI2C(index, 3, &rt6937_cmd1, 0, &pucdatabuffer);
  i2c_flag = USBIO_StreamI2C(index, 2, &rt6937_cmd2, 0, &pucdatabuffer);
  i2c_flag = USBIO_StreamI2C(index, 1, &rt6937_cmd3, 1, &pucdatabuffer);

  return (pucdatabuffer);
}

// mini-LVDS config function
void minilvds_init() {
  unsigned char buffer[2];
  // unsigned int test = 0;
  unsigned int w_test = 0;
  unsigned long test = 0;

  int j = 0;
  int i = 0;

  char *p_w;
  char *p;
  char data_buff[6] = {0x14, 0xfc, 0x01, 0x00, 0x00, 0x00};
  // unsigned long tmp[2];
  // unsigned long tmp[3];

  // char  data_buff[6]  = {0x14,0x04,0x00,0x00,0x00,0x00};
  // char  data_buff2[6] = {0x14,0x04,0x00,0x00,0x00,0x00};

  char data_read[2] = {0x14, 0x04};
  // char  test_addr[2]   = {0x14,0xfc};
  // char  test_addr1[2]   = {0x14,0x00};
  char test_addr[2] = {0x14, 0x00};
  char test_addr1[2] = {0x14, 0x08}; //{0x14,0x02};
  char test_addr2[2] = {0x14, 0x14}; //{0x14,0x05};

  char test_addr3[2] = {0x14, 0x20}; //{0x14,0x08};
  char test_addr4[2] = {0x14, 0x24}; //{0x14,0x09};
  char test_addr5[2] = {0x14, 0xa0}; //{0x14,0x28};
  char test_addr6[2] = {0x14, 0xb0}; //{0x14,0x2c};

  char test_addr7[2] = {0x14, 0x10}; //{0x14,0x04};
  char test_addr8[2] = {0x14, 0x94}; //{0x14,0x25};

  char test_zw[8] = {0x14, 0x40, 0x00, 0x12, 0x34, 0x56, 0x78, 0x00};
  char test_zw2[8] = {0x14, 0x40, 0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0x00};
  char test_zw3[8] = {0x14, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00};

  char tcon_tope[7] = {0x14, 0x40, 0x38, 0x1f, 0x00, 0x00, 0x00};
  char tcon_top0[7] = {0x14, 0x40, 0x00, 0x99, 0x1d, 0x00, 0x00};
  char tcon_top9[7] = {0x14, 0x40, 0x24, 0x3b, 0x00, 0x00, 0x00};
  char tcon_top9_2[7] = {0x14, 0x40, 0x24, 0x3b, 0x00, 0x00, 0x00};
  /*char tcon_top9[7] = {0x14,0x40,0x24,0x39,0x00,0x00,0x00};
  char tcon_top9_2[7] = {0x14,0x40,0x24,0x39,0x00,0x00,0x00};*/
  char tcon_topb[7] = {0x14, 0x40, 0x2c, 0x3f, 0x00, 0x00, 0x00};
  char tcon_topb_2[7] = {0x14, 0x40, 0x2c, 0x00, 0x00, 0x00, 0x00};
  char test_status[7] = {0x14, 0x40, 0x20, 0x50, 0x00, 0x00, 0x00};
  // char key_info[7] = {0x14,0x40,0x00,0x00,0x00,0x00,0x00};
  int key_addr = 0;
  int key_addr_offset_msb = 0;
  int key_addr_offset_lsb = 0;
  int write_data_x_bits = 0;
  int mask = 0;
  int mask_data = 0;

  int write_data_by_offset = 0;
  char key_wr = 0;
  int key_data = 0;
  // char test_zw[2] = {0x14,0x40,0x00};
  int file_data;
  int file_addr;
  FILE *fp_fiti;
  FILE *fp_lut;
  FILE *fp_bwidth;

  // fp_fiti = fopen("PRJ_FHD_FHD_setting_case506_video_in_20200629_prediv2_3.txt","r");//PRJ_FHD prediv2

  // fp_fiti = fopen("PRJ_FHD_FHD_setting_case006_20200722.txt","r");//PRJ_FHD
  // fp_fiti = fopen("PRJ_UHD_setting_fhd_14_case501_video_in_2020608_prediv2_dvi.txt","r");//PRJ_UHD// prediv2  dvi board
  // fp_fiti = fopen("./data/PRJ_UHD_FHD_setting_14_case001_fhd_minilvds_video_in_od_dbg_202000602.txt","r");//PRJ_UHD//       full
  // fp_fiti = fopen("./data/PRJ_UHD_setting_fhd_14_case501_video_in_20210514_prediv2_base.txt","r");//PRJ_UHD// prediv2
  // fp_fiti = fopen("./data/PRJ_UHD_setting_fhd_14_case502_video_in_20210519_prediv2.txt","r");//PRJ_UHD// prediv2
  // fp_fiti = fopen("./data/PRJ_FHD_FHD_setting_case506_video_in_20200629_prediv2.txt","r");//PRJ_UHD// prediv2

  // fp_fiti = fopen("./data/PRJ_UHD_setting_fhd_14_case501_video_in_2020608_prediv2.txt","r");//PRJ_UHD// prediv2
  // fp_fiti = fopen("./data/PRJ_UHD_FHD_setting_14_case001_fhd_minilvds_video_in_20200518.txt","r");//PRJ_UHD// prediv2
  // fp_fiti = fopen("./data/PRJ_UHD_FHD_setting_14_case001_fhd_minilvds_video_in_od_dbg_202000602.txt","r");//PRJ_UHD//
  // fp_fiti = fopen("./data/PRJ_UHD_FHD_setting_14_case001_fhd_minilvds_video_in_od_dbg_202000602_golden.txt","r");

  // t5m
  // fp_fiti = fopen("./data/t5m_case501_mininlvds_half_panel_lut_20220613.txt","r");//PRJ_UHD// prediv2
  // fp_fiti = fopen("./data/t5m_case001_minilvds_full_panel_20220613.txt","r");//PRJ_UHD// prediv2
  // fp_fiti = fopen("./data/t5m_case501_mininlvds_half_panel_lut_20220613.txt","r");//PRJ_UHD// half panel golden
  // fp_fiti = fopen("./data/t5m_case001_mininlvds_full_panel_lut_20220614.txt","r");//PRJ_UHD// full panel golden
  // fp_fiti = fopen("./data/PRJ_UHD_setting_minilvds_case001_fhd_out.txt","r");
  // fp_fiti = fopen("./data/PRJ_UHD60_minilvds_t5m_case001_2022_0706.txt","r");
  // fp_fiti = fopen("./data/t5m_case502_mininlvds_half_panel_2frm_od_20220613.txt","r");//PRJ_UHD// prediv2
  // fp_fiti = fopen("./data/t5m_case502_mininlvds_half_panel_2frm_od_same_lut_20220614.txt","r");//PRJ_UHD// prediv2

  // t3x ud120
  // fp_fiti = fopen("./data/uhd120_case001_minilvds_selftiminggen_test_20220704.txt","r");
  // fp_fiti = fopen("./data/uhd120_case001_minilvds_test_20220712.txt","r");
  // fp_fiti = fopen("./data/PRJ_UHD120_minilvds_case601_setting_20220715.txt","r");
  // fp_fiti = fopen("./data/uhd120_minilvds_case700_all_lut_setting_20220720.txt","r");
  // fp_fiti = fopen("./data/uhd120_minilvds_case601_setting_20220720.txt","r");
  // fp_fiti = fopen("./data/uhd120_minilvds_case700_setting_20220721.txt","r");
  // fp_fiti = fopen("./data/uhd120_minilvds_case700_setting_20220727_demura.txt","r");
  // fp_fiti = fopen("./data/uhd120_minilvds_case700_setting_20220727_demura_test.txt","r");
  // fp_fiti = fopen("./data/uhd120_minilvds_case700_setting_gam2_20220801.txt","r");
  // fp_fiti = fopen("./data/uhd120_case001_minilvds_demura_test_20220803.txt","r");

  // fp_fiti = fopen("./data/uhd120_t3x_case700_setting_all_lut_20220803.txt","r");
  // fp_fiti = fopen("./data/uhd120_minilvds_t3x_case700_setting_20220808.txt","r");
  // fp_fiti = fopen("./data/t5m_case001_mininlvds_full_panel_lge_test_20220614.txt","r");//PRJ_UHD// full panel golden

  // pd test
  // fp_fiti = fopen("./data/uhd120_case001_minilvds_pd_test_20220803.txt","r");

  // lge
  // fp_fiti = fopen("./data/uhd60_lge_minilvds_case601_setting_20221111.txt","r");
  // fp_fiti = fopen("./data/lge/uhd60_t3e_minilvds_full_case601_setting_20230217.txt","r");

  // fp_fiti = fopen("./data/uhd60_case502_lg_2frm_od_minilvds_half_setting_20221115.txt","r");
  // fp_fiti = fopen("./data/lge/ud60_t3e_lod_lut_case024_setting_20230113_ori.txt","r");
  // fp_fiti = fopen("./data/lge/ud60_t3e_lod_lut_case024_setting_20230209_1.txt","r");
  // fp_fiti = fopen("./data/lge/ud60_t3e_lod_lut_case024_setting_20230209_1_test.txt","r");

  // fp_fiti = fopen("./data/lge/ud60_t3e_lod_lut_case024_setting_20230217.txt","r");
  // fp_fiti = fopen("./data/lge/ud60_t3e_lod_lut_case024_setting_20230217_test.txt","r");

  // fp_fiti = fopen("./data/lge/t3e_UHD_mini_full_path230216_setting_2.txt","r");
  // fp_fiti = fopen("./data/lge/t3e_UHD_mini_full_path230216_setting_3.txt","r");
  // fp_fiti = fopen("./data/lge/t3e_UHD_mini_full_path230216_setting_3_test.txt","r");
  // fp_fiti = fopen("./data/lge/t3e_case601_FHD_mini2P3Pair_IMG_FPGA_setting.txt","r");

  // fhd
  // fp_fiti = fopen("./data/PRJ_FHD_FHD_setting_case506_video_in_20200707_prediv2.txt","r");//PRJ_FHD golden prediv2

  // fp_fiti = fopen("./data/FHD/fhd60_minilvds_full_case006_setting_20230129.txt","r");
  // fp_fiti = fopen("./data/FHD/fhd60_minilvds_full_case006_setting_20230117_test.txt","r");
  // fp_fiti = fopen("./data/PRJ_FHD_FHD_setting_case006_pd_video_in_20230110.txt","r");

  // fp_fiti = fopen("./data/FHD/fhd60_minilvds_half_case006_setting_20230103.txt","r");
  // fp_fiti = fopen("./data/PRJ_FHD_FHD_setting_case006_video_in_20200629.txt","r");//PRJ_FHDs
  // fp_fiti = fopen("./data/FHD_mini2P3Pair_full_path_setting230214_2.txt","r");

  // fp_fiti = fopen("./data/uhd60_2frm_od_minilvds_half_new_cmpr_setting_20231128.txt","r");
  // fp_fiti = fopen("./data/t5m_case501_mininlvds_half_panel_lut_20240830.txt","r");//PRJ_UHD//ud60 half panel golden new fpga reg addr

  // fp_fiti = fopen("./data/uhd120_minilvds_t6x_base_t3x_case700_setting_20250226.txt","r");

  fp_fiti = fopen("./data/uhd120_minilvds_t6x_base_t3x_case700_setting_20250228.txt", "r");

  if (fp_fiti == NULL)
    printf("error while opening file fiti_setting\n");

  for (i = 0;; i++) {
    // if((fscanf(fp_fiti,"%x, %x\n",&file_addr,&file_data))<0) break;
    if ((fscanf(fp_fiti, "0x%x, 0x%x,\n", &file_addr, &file_data)) < 0)
      break;
    test_zw3[1] = 0;
    test_zw3[2] = ((file_addr << 2) >> 8) & 0xff;
    test_zw3[3] = (file_addr << 2) & 0xff;
    test_zw3[4] = file_data & 0xff;
    test_zw3[5] = (file_data & 0xff00) >> 8;
    test_zw3[6] = (file_data & 0xff0000) >> 16;
    test_zw3[7] = (file_data & 0xff000000) >> 24;

    printf("i = %x, addr = %x data = %x\n", (i << 2), file_addr, file_data);
    USBIO_StreamI2C(0x00, 0x8, &test_zw3[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &test_zw3[0], 0x04, &tmp[2]);
  }
}

void minilvds_gam_setting() {
  char test_infomat; // add by fxj to stop dos infomation
  unsigned char buffer[2];
  unsigned long wr_test[4];
  int i;

  unsigned long tmp[3];
  int key_addr = 0;
  char key_wr = 0;
  int key_data = 0;
  //	char key_info[7] = {0x14,0x40,0x00,0x00,0x00,0x00,0x00};
  char key_info[7] = {0xe8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};
  unsigned char reg_addr;
  unsigned long reg_data;
  char pmic[14] = {0x40, // dev addr
                   0x1,  // start addr 1
                   0x1a, // 0x28,  //val
                   // 0xf,
                   0x5,
                   0x0,
                   0x0,
                   0xb,
                   0x2,
                   0x23, // 0x25,
                   0x4,  // 0x8,
                   0x4,
                   0x0,
                   0x1, // 0x4,
                   0x0};

  int vol[20] = {0x3e4,  // 0x3e5, //0x3ff,  //1+
                 0x3dc,  // 0x3dd, //0x39c,  //2+
                 0x36e,  // 0x371, //0x329,  //3
                 0x304,  // 0x306,//0x2fc,  //4+
                 0x2bf,  // 0x2c1,//0x2c5,  //5+
                 0x251,  // 0x253,//0x290,  //6+
                 0x21a,  // 0x21c,//0x265, //7
                 0x1fe,  // 0x201,//0x210, //8+
                 0x19d,  // 0x1a2,//0x1f8, //9+
                 0x13c,  // 0x140,//0x1de, //10+
                 0xec,   // 0xed,//0x1cc, //11+
                 0x78,   // 0x79,//0x175, //12
                 0x17,   // 0x17,//0x14c, //13+
                 0x11,   // 0x12,//0x11a, //14+
                 0x3ff,  // 0xe3, //15+
                 0xb8,   // 16
                 0x54,   // 17+
                 0x1b0,  // 18+
                 0x12b,  // 0x1c0, //19
                 0x12b}; // 0x1c0};//20
  // init_reg_rw();

  wr_test[0] = 0x0000ff40;
  wr_test[1] = 0x0040;
  wr_test[2] = 0x0080ff40;
  buffer[0] = 0;
  buffer[1] = 0;
  gamma_readcommandbytes(0x0, 2, &buffer[0]);
  gamma_writecommandbytes(0x0, 0x4, &wr_test[0]);
  buffer[0] = 0;
  buffer[1] = 0;
  gamma_readcommandbytes(0x0, 2, &buffer[0]);
  buffer[0] = 0;
  buffer[1] = 0;

  wr_test[0] = 0x3f0007e8;

  for (i = 0; i < 20; i++) {
    wr_test[0] = 0;
    wr_test[0] = (((vol[i] & 0xff) << 24) | (((vol[i] >> 8) & 0xf) << 16) | (i << 8) | 0xe8);
    gamma_writecommandbytes(0x0, 0x4, &wr_test[0]);
  }
  printf("gamma setting done!!!\n");
}

static int gamma_readcommandbytes(int index, int inbbytes, unsigned char *pucdatabuffer) {
  int i2c_flag = 0;
  unsigned int i = 0;
  unsigned int i2c_try_cnt = 10;
  unsigned char mwlen = 2;
  // unsigned char dev_addr = 0xe8;
  unsigned char dev_addr[2];
  dev_addr[0] = 0x07;
  dev_addr[1] = 0xe8;
  i2c_flag = USBIO_StreamI2C(index, mwlen, &dev_addr[0], inbbytes, &pucdatabuffer[0]);
  // i2c_flag = USBIO_WriteI2C(index, dev_addr, 1, pucdatabuffer[0]);
repeat:
  // i2c_flag = i2c_transfer(si2176->adapter, msg, 1);
  if (i2c_flag == 0) {
    printf("  error in read sli2176, %d byte(s) should be read,. \n", inbbytes);
    if (i++ < i2c_try_cnt) {
      printf("error in read sli2176, try again!!!\n");
      goto repeat;
    } else
      return -1;
  } else {
    // printf("  read %d bytes\n" , inbbytes);
    return inbbytes;
  }
}

// CEDS
void ceds_v03_pmic_init() {
  char test_infomat; // add by fxj to stop dos infomation
  unsigned char buffer[2];
  unsigned long wr_test[4];
  int i;
  unsigned long tmp[3];
  int key_addr = 0;
  char key_wr = 0;
  int key_data = 0;
  //	char key_info[7] = {0x14,0x40,0x00,0x00,0x00,0x00,0x00};
  char key_info[7] = {0xe8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};

  unsigned char reg_addr;
  unsigned long reg_data;
  char pmic[14] = {0x40, // dev addr
                   0x1,  // start addr 1
                   0x1B, // 0x28,  //val
                   // 0xf,
                   0x5,
                   0x0,
                   0x0,
                   0xb,
                   0x2,
                   0x23, // 0x25,
                   0x4,  // 0x8,
                   0x4,
                   0x0,
                   0x1, // 0x4,
                   0x0};

  int buf20800_vol[20] = {942,  // GM1,CEDS,15.92
                          0,    // 2+
                          0,    // 3+
                          767,  // 4+,CEDS,13
                          711,  // 5+,CEDS,12.1
                          661,  // 6+,CEDS,11.25
                          0,    // 7
                          0,    // 8+
                          503,  // 9+,CEDS,8.63
                          443,  // 10+,CEDS,7.62
                          0,    // 11+
                          0,    // 12
                          286,  // 13+,CEDS,5
                          236,  // 14+,CEDS,4.17
                          181,  // 15+,CEDS,3.26
                          0,    // 16
                          0,    // 17+
                          3,    // 18+,CEDS,0.3
                          441,  // 19,CEDS,7.5
                          441}; // 20,CEDS,7.5

  int rt6937_vol[8] = {0x0c,  // VDD
                       0x01,  // VCC1
                       0x03,  // VCC2
                       0x01,  // HVDD
                       0x0f,  // VGH_LT
                       0x0f,  // VGH_HT
                       0x32,  // VGL
                       0x07}; // VGL2
  // init_reg_rw();

  // test_usb_i2c_func();
  printf("hello world\n");

  wr_test[0] = 0x0000ff40;
  wr_test[1] = 0x0040;
  wr_test[2] = 0x0080ff40;
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);

  // gamma_writecommandbytes(0x0,0x4,&wr_test[0]);
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);
  buffer[0] = 0;
  buffer[1] = 0;

  /*
          // pmic
  USBIO_StreamI2C(0x0,  14, &pmic[0], 0, &buffer[0]);
  USBIO_StreamI2C(0x0,  3, &wr_test[0], 0, &buffer[0]);
  USBIO_StreamI2C(0x0,  3, &wr_test[2], 0, &buffer[0]);
  */

  // write buf20800
  wr_test[0] = 0x3f0007e8;
  for (i = 0; i < 20; i++) {
    wr_test[0] = 0;
    wr_test[0] = (((buf20800_vol[i] & 0xff) << 24) | (((buf20800_vol[i] >> 8) & 0xf) << 16) | (i << 8) | 0xe8);
    gamma_writecommandbytes(0x0, 0x4, &wr_test[0]);
  }

  // write rt6937
  for (i = 0; i < 8; i++) {
    wr_test[0] = 0;
    wr_test[0] = ((0x0 << 24) | ((rt6937_vol[i] & 0xff) << 16) | (i << 8) | 0x40);
    gamma_writecommandbytes(0x0, 0x4, &wr_test[0]);
  }
  wr_test[0] = ((0x0 << 24) | ((0x80) << 16) | (0xff << 8) | 0x40);
  gamma_writecommandbytes(0x0, 0x4, &wr_test[0]);

  /*
  while(1){
          printf("please input reg addr \n");
          scanf("%x",&reg_addr);
          printf("you entered %x\n",reg_addr);
          reg_data = rt6937_readcommandbytes(reg_addr);
          printf("the reg data of offset %x is  %x\n",reg_addr,(reg_data&0xff));
  }*/
}

// CEDS
void ceds_f21_pmic_init() {
  char test_infomat; // add by fxj to stop dos infomation
  unsigned char buffer[2];
  unsigned long wr_test[4];
  int i;
  unsigned long tmp[3];
  int key_addr = 0;
  char key_wr = 0;
  int key_data = 0;
  //	char key_info[7] = {0x14,0x40,0x00,0x00,0x00,0x00,0x00};
  char key_info[7] = {0xe8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};

  unsigned char reg_addr;
  unsigned long reg_data;
  /*
          char pmic[14]= {0x40, //dev addr
                     0x1,  //start addr 1
                             0x1B,//0x28,  //val
                             //0xf,
                             0x5,
                             0x0,
                             0x0,
                             0xb,
                             0x2,
                             0x23,//0x25,
                             0x4,//0x8,
                             0x4,
                             0x0,
                 0x1,//0x4,
                 0x0};
  */
  char pmic[14] = {0x40, // dev addr
                   0x1,  // start addr 1
                   0x1B, // 0x28,  //val
                   // 0xf,
                   0x5,
                   0x0,
                   0x0,
                   0xb,
                   0x2,
                   0x23, // 0x25,
                   0x4,  // 0x8,
                   0x0,
                   0x0,
                   0x1, // 0x4,
                   0x0};

  int buf20800_vol[20] = {1003, // GM1,CEDS,15.32
                          0,    // 2+
                          0,    // 3+
                          795,  // 4+,CEDS,12.20
                          714,  // 5+,CEDS, 10.98 V
                          675,  // 6+,CEDS,675: 10.37 V
                          0,    // 7 CEDS
                          0,    // 8+ CEDS
                          542,  // 9+,CEDS,8.4
                          463,  // 10+ CEDS,7.21
                          0,    // 11+ CEDS
                          0,    // 12
                          333,  // 13+,333??CEDS,5.25
                          287,  // 14+,287??CEDS,4.55
                          200,  // 15+,CEDS ,3.23
                          0,    // 16
                          0,    // 17+
                          3,    // 18+,CEDS,0.28
                          475,  // 19,CEDS,vcom1 451: 6.93V, 459: 7.04V, 443: 6.82V
                          0};   // 20,CEDS,vcom2

  int rt6937_vol[8] = {0x0c,  // VDD
                       0x01,  // VCC1
                       0x00,  // VCC2  3
                       0x06,  // HVDD
                       0x04,  // VGH_LT
                       0x0f,  // VGH_HT
                       0x2a,  // VGL
                       0x03}; // VGL2
  // init_reg_rw();

  // test_usb_i2c_func();
  printf("hello world\n");

  wr_test[0] = 0x0000ff40;
  wr_test[1] = 0x0040;
  wr_test[2] = 0x0080ff40;
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);

  // gamma_writecommandbytes(0x0,0x4,&wr_test[0]);
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);
  buffer[0] = 0;
  buffer[1] = 0;

  /*
          // pmic
  USBIO_StreamI2C(0x0,  14, &pmic[0], 0, &buffer[0]);
  USBIO_StreamI2C(0x0,  3, &wr_test[0], 0, &buffer[0]);
  USBIO_StreamI2C(0x0,  3, &wr_test[2], 0, &buffer[0]);
  */

  // write buf20800
  wr_test[0] = 0x3f0007e8;
  for (i = 0; i < 20; i++) {
    wr_test[0] = 0;
    wr_test[0] = (((buf20800_vol[i] & 0xff) << 24) | (((buf20800_vol[i] >> 8) & 0xf) << 16) | (i << 8) | 0xe8);
    gamma_writecommandbytes(0x0, 0x4, &wr_test[0]);
  }

  // write rt6937
  for (i = 0; i < 8; i++) {
    wr_test[0] = 0;
    wr_test[0] = ((0x0 << 24) | ((rt6937_vol[i] & 0xff) << 16) | (i << 8) | 0x40);
    gamma_writecommandbytes(0x0, 0x4, &wr_test[0]);
  }
  wr_test[0] = ((0x0 << 24) | ((0x80) << 16) | (0xff << 8) | 0x40);
  gamma_writecommandbytes(0x0, 0x4, &wr_test[0]);

  /*
  while(1){
          printf("please input reg addr \n");
          scanf("%x",&reg_addr);
          printf("you entered %x\n",reg_addr);
          reg_data = rt6937_readcommandbytes(reg_addr);
          printf("the reg data of offset %x is  %x\n",reg_addr,(reg_data&0xff));
  }*/
}

void ceds_v03_load_setting() {
  unsigned char buffer[2];
  // unsigned int test = 0;
  unsigned int w_test = 0;
  unsigned long test = 0;

  int j = 0;
  int i = 0;

  char *p_w;
  char *p;
  char data_buff[6] = {0x14, 0xfc, 0x01, 0x00, 0x00, 0x00};
  // unsigned long tmp[2];
  unsigned long tmp[3];

  // char  data_buff[6]  = {0x14,0x04,0x00,0x00,0x00,0x00};
  // char  data_buff2[6] = {0x14,0x04,0x00,0x00,0x00,0x00};

  char data_read[2] = {0x14, 0x04};
  // char  test_addr[2]   = {0x14,0xfc};
  // char  test_addr1[2]   = {0x14,0x00};
  char test_addr[2] = {0x14, 0x00};
  char test_addr1[2] = {0x14, 0x08}; //{0x14,0x02};
  char test_addr2[2] = {0x14, 0x14}; //{0x14,0x05};

  char test_addr3[2] = {0x14, 0x20}; //{0x14,0x08};
  char test_addr4[2] = {0x14, 0x24}; //{0x14,0x09};
  char test_addr5[2] = {0x14, 0xa0}; //{0x14,0x28};
  char test_addr6[2] = {0x14, 0xb0}; //{0x14,0x2c};

  char test_addr7[2] = {0x14, 0x10}; //{0x14,0x04};
  char test_addr8[2] = {0x14, 0x94}; //{0x14,0x25};

  char test_zw[8] = {0x14, 0x40, 0x00, 0x12, 0x34, 0x56, 0x78, 0x00};
  char test_zw2[8] = {0x14, 0x40, 0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0x00};
  char test_zw3[8] = {0x14, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00};

  char tcon_tope[7] = {0x14, 0x40, 0x38, 0x1f, 0x00, 0x00, 0x00};
  char tcon_top0[7] = {0x14, 0x40, 0x00, 0x99, 0x1d, 0x00, 0x00};
  char tcon_top9[7] = {0x14, 0x40, 0x24, 0x3b, 0x00, 0x00, 0x00};
  char tcon_top9_2[7] = {0x14, 0x40, 0x24, 0x3b, 0x00, 0x00, 0x00};
  /*char tcon_top9[7] = {0x14,0x40,0x24,0x39,0x00,0x00,0x00};
  char tcon_top9_2[7] = {0x14,0x40,0x24,0x39,0x00,0x00,0x00};*/
  char tcon_topb[7] = {0x14, 0x40, 0x2c, 0x3f, 0x00, 0x00, 0x00};
  char tcon_topb_2[7] = {0x14, 0x40, 0x2c, 0x00, 0x00, 0x00, 0x00};
  char test_status[7] = {0x14, 0x40, 0x20, 0x50, 0x00, 0x00, 0x00};
  char key_info[7] = {0x14, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};
  int key_addr = 0;
  int key_addr_offset_msb = 0;
  int key_addr_offset_lsb = 0;
  int write_data_x_bits = 0;
  int mask = 0;
  int mask_data = 0;

  int write_data_by_offset = 0;
  char key_wr = 0;
  int key_data = 0;
  // char test_zw[2] = {0x14,0x40,0x00};
  int file_data;
  int file_addr;
  FILE *fp_fiti;
  FILE *fp_lut;
  // fp_fiti = fopen("fiti_setting_20170721_no_od.txt","r");
  // fp_fiti = fopen("fiti_1080.txt","r");
  // fp_fiti = fopen("fiti_setting_20170727_no_od.txt","r");
  // fp_fiti = fopen("fiti_setting_20170809_tcl.txt","r");
  // fp_fiti = fopen("fiti_setting_20170816_tcl_od3.txt","r");
  // fp_fiti = fopen("fiti_setting_20170818_tcl_zigzag.txt","r");
  // fp_fiti = fopen("fiti_setting_20170824_tcl_zigzag_invod_255.txt","r");
  // fp_fiti = fopen("fiti_setting_20170825_tcl_zigzag_invod_00.txt","r");
  // fp_fiti = fopen("UHD_setting_cspi_20190516.txt","r");
  // fp_fiti = fopen("UHD_setting_cspi_20190909_dot.txt","r");
  //

  // fp_fiti = fopen("UHD_setting_44_case013_ceds_videoin_20200117.txt","r");
  // fp_fiti = fopen("UHD_setting_ceds_44_case011_20191216.txt","r");
  // fp_fiti = fopen("UHD_setting_ceds_44_case011_video_in_20191218.txt","r");
  // fp_fiti = fopen("UHD_setting_14_case015_videoin_ceds_demura_20200115_2.txt","r");
  // fp_fiti = fopen("./data/PRJ_UHD_setting_fhd_14_case512_FHDinUHDout_594M_video_in_20200529_lod_dbg.txt","r");
  // fp_fiti = fopen("./data/uhd60_ceds_iptest_case512setting_20220810.txt","r");
  // t3x
  // fp_fiti = fopen("./data/uhd120_ceds_t3x_case611_setting_20220805.txt","r");

  // fp_fiti = fopen("./data/uhd120_ceds_t3x_case611_setting_20220805_alllut.txt","r");

  // fp_fiti = fopen("./data/uhd120_ceds_case611_setting_20220811_2.txt","r");
  // fp_fiti = fopen("./data/uhd120_ceds_case611_setting_20220812.txt","r");

  // fp_fiti = fopen("./data/uhd120_ceds_case512_lod_lut_setting_20220812.txt","r");
  //  58 f20
  // fp_fiti = fopen("./data/uhd120_ceds_case620_f20_setting_20220928.txt","r");
  // fp_fiti = fopen("./data/uhd120_ceds_case620_fpga_58f20_setting_20220929_7.txt","r");
  // fp_fiti = fopen("./data/uhd120_ceds_case620_fpga_58f20_setting_20221009.txt","r");

  // fp_fiti = fopen("./data/case620_ceds_hf580qub_f20_xiaomi_fpga_20221012.txt","r");
  // fp_fiti = fopen("./data/case620_ceds_hf580qub_f20_xiaomi_fpga_20221018_2.txt","r");
  // fp_fiti = fopen("./data/case620_ceds_lod_hf580qub_f20_xiaomi_fpga_20221019.txt","r");

  // fp_fiti = fopen("./data/case620_ceds_lod_hf580qub_f20_xiaomi_fpga_20221027_nrm_gam_xiaomi.txt","r");
  // fp_fiti = fopen("./data/case620_ceds_lod_hf580qub_f20_xiaomi_fpga_20221027_nrm.txt","r"); // t3x 120 golden setting

  // fp_fiti = fopen("./data/case630_lge_cspi_vac_20221116.setting_test.txt","r"); //ok
  // lge
  // fp_fiti = fopen("./data/case620_ceds_lod_hf580qub_f20_xiaomi_fpga_lge_20221110.txt","r");
  // fp_fiti = fopen("./data/uhd60_case620_ceds_xiaomi_f20_setting_20221122_test.txt","r");
  // fp_fiti = fopen("./data/uhd60_case620_ceds_xiaomi_f20_setting_20221122.txt","r");   // lge uhd60 golden

  // fp_fiti = fopen("./data/case622_ceds_f20_xiaomi_lge_pclrc_20221130_v1.txt","r");
  // fp_fiti = fopen("./data/PRJ_UHD_setting_fhd_14_case512_FHDinUHDout_594M_video_in_20200529_lod_dbg_tmp.txt","r");
  // fp_fiti = fopen("./data/case622_ceds_f20_xiaomi_demura_setting_20221201.txt","r");

  // fp_fiti = fopen("./data/case622_lge_ceds_f20_pclrc_20221205.setting.txt","r");
  // fp_fiti = fopen("./data/case622_lge_ceds_f20_pclrc_20221205_v1_setting.txt","r");
  // fp_fiti = fopen("./data/uhd60_lg_ceds_case620_setting_20221207_10bit.txt","r");

  // fp_fiti = fopen("./data/case621_ceds_f20_xiaomi_gam_dth_setting.txt","r");
  // fp_fiti = fopen("./data/case621_ceds_f20_xiaomi_gam_dth_fpga_2.txt","r");
  // fp_fiti = fopen("./data/case621_ceds_f20_xiaomi_dth_12b_6_old_setting.txt","r");
  // fp_fiti = fopen("./data/case621_ceds_f20_xiaomi_dth_12b_1_setting.txt","r");
  // fp_fiti = fopen("./data/case621_ceds_f20_xiaomi_dth_12b_7_setting.txt","r");
  // fp_fiti = fopen("./data/case623_ceds_lgd_13b_gam_dth_fpga_setting.txt","r");
  // fp_fiti = fopen("./data/case681_epi_LD5550EGJ_LG_for_fpga_148_setting.txt","r");
  // fp_fiti = fopen("./data/case681_epi_LD5550EGJ_LG_for_fpga_595_setting.txt","r");
  // fp_fiti = fopen("./data/case681_epi_LD5550EGJ_LG_for_fpga_setting_221208_v1.txt","r");
  // fp_fiti = fopen("./data/case681_epi_LD5550EGJ_LG_for_fpga_setting_221209.txt","r");
  // fp_fiti = fopen("./data/case681_lg_setting_20221209.txt","r");
  // fp_fiti = fopen("./data/case622_lge_ceds_f20_pclrc_20221205.setting.txt","r");

  // f21 120hz actually is 60hz, 60hz is 30hz
  // fp_fiti = fopen("./data/case625_uhd_2p775g_f21_20231206_setting.txt","r");
  // fp_fiti = fopen("./data/case625_uhd_2p775g_f21_20231207_setting.txt","r");
  // fp_fiti = fopen("./data/case625_uhd_2p772g_f21_20231207_v1_setting.txt","r");

  // fp_fiti = fopen("./data/case625_fpga_58f21_2p772g_ctc_chpi_20240112_setting.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_chpi_f21_setting_20240127.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_chpi_f21_2p97g_60hz_20240129.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_chpi_f21_setting_20240130.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_chpi_f21_setting_20240130_2.txt","r");
  // fp_fiti = fopen("./data/uhd120_case625_chpi_f21_setting_20240131.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_60hz_normal_setting_20240301.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_60hz_normal_setting_20240301_test.txt","r");  // normal golden 60hz/30hz

  // fp_fiti = fopen("./data/uhd60_case625_chpi_f21_setting_120hz_hsr_20240202.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_chpi_f21_setting_60hz_hsr_20240202_2.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_chpi_f21_setting_60hz_hsr_20240202_5.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_chpi_f21_setting_60hz_hsr_test.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_chpi_f21_setting_120hz_hsr_test_0222.txt","r"); // may ok
  // fp_fiti = fopen("./data/uhd60_case625_chpi_f21_setting_120hz_hsr_test_0223.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_chpi_f21_setting_20240223_base.txt","r"); // hsr120 golden
  // fp_fiti = fopen("./data/uhd60_case630_chpi_f21_setting_pol_lod_hsr_20240226.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_chpi_f21_setting_20240227_pre_148.txt","r"); // pre 148.5 120hz golden
  // fp_fiti = fopen("./data/uhd60_case625_chpi_f21_setting_60hz_hsr_test_pre_148.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_60hz_chpi_f21_hsr_setting_pre_148_20240301.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_60hz_hsr_setting_pre_148_20240301_1.txt","r");//pre 148.5 30hz hsr golden

  // fp_fiti = fopen("./data/uhd60_case625_fpga_58f21_2p772g_chpi_120hz_shsc_setting.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_fpga_58f21_2p772g_chpi_120hz_vctc_setting.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_fpga_58f21_2p772g_chpi_120hz_vctc_shsc_setting.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_fpga_58f21_2p772g_chpi_120hz_vctc_shsc_setting_2.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_fpga_58f21_2p772g_chpi_120hz_ip_setting_240306.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_fpga_58f21_2p772g_chpi_120hz_ip_setting_shsc_lut_240306.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_fpga_58f21_2p772g_chpi_120hz_ip_setting_shsc_lut_pol1010_2403022.txt","r");
  // fp_fiti = fopen("./data/uhd60_case625_fpga_58f21_2p772g_chpi_120hz_shsc_setting_240412.txt","r");

  // lod test
  // fp_fiti = fopen("./data/uhd60_case630_pol_lod_setting_20240419.txt","r");

  // asf test
  fp_fiti = fopen("./data/uhd60_case631_fpga_58f21_4k1k_120hz_asf_20240429.txt", "r");

  if (fp_fiti == NULL)
    printf("error while opening file fiti_setting\n");

  for (i = 0;; i++) {
    if ((fscanf(fp_fiti, "0x%x, 0x%x,\n", &file_addr, &file_data)) < 0)
      break;
    // if((fscanf(fp_fiti,"%x, %x\n",&file_addr,&file_data))<0) break;
    test_zw3[1] = 0;
    test_zw3[2] = ((file_addr << 2) >> 8) & 0xff;
    test_zw3[3] = (file_addr << 2) & 0xff;
    test_zw3[4] = file_data & 0xff;
    test_zw3[5] = (file_data & 0xff00) >> 8;
    test_zw3[6] = (file_data & 0xff0000) >> 16;
    test_zw3[7] = (file_data & 0xff000000) >> 24;

    printf("i = %x, addr = %x data = %x\n", (i << 2), file_addr, file_data);
    USBIO_StreamI2C(0x00, 0x8, &test_zw3[0], 0x00, &tmp[2]);
    // USBIO_StreamI2C(0x00,0x4,&test_zw3[0],0x04,&tmp[2]);
  }
}

void f71_ceds_load_setting() {
  unsigned char buffer[2];
  // unsigned int test = 0;
  unsigned int w_test = 0;
  unsigned long test = 0;

  int j = 0;
  int i = 0;

  char *p_w;
  char *p;
  char data_buff[6] = {0x14, 0xfc, 0x01, 0x00, 0x00, 0x00};
  // unsigned long tmp[2];
  unsigned long tmp[3];

  // char  data_buff[6]  = {0x14,0x04,0x00,0x00,0x00,0x00};
  // char  data_buff2[6] = {0x14,0x04,0x00,0x00,0x00,0x00};

  char data_read[2] = {0x14, 0x04};
  // char  test_addr[2]   = {0x14,0xfc};
  // char  test_addr1[2]   = {0x14,0x00};
  char test_addr[2] = {0x14, 0x00};
  char test_addr1[2] = {0x14, 0x08}; //{0x14,0x02};
  char test_addr2[2] = {0x14, 0x14}; //{0x14,0x05};

  char test_addr3[2] = {0x14, 0x20}; //{0x14,0x08};
  char test_addr4[2] = {0x14, 0x24}; //{0x14,0x09};
  char test_addr5[2] = {0x14, 0xa0}; //{0x14,0x28};
  char test_addr6[2] = {0x14, 0xb0}; //{0x14,0x2c};

  char test_addr7[2] = {0x14, 0x10}; //{0x14,0x04};
  char test_addr8[2] = {0x14, 0x94}; //{0x14,0x25};

  char test_zw[8] = {0x14, 0x40, 0x00, 0x12, 0x34, 0x56, 0x78, 0x00};
  char test_zw2[8] = {0x14, 0x40, 0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0x00};
  char test_zw3[8] = {0x14, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00};

  char tcon_tope[7] = {0x14, 0x40, 0x38, 0x1f, 0x00, 0x00, 0x00};
  char tcon_top0[7] = {0x14, 0x40, 0x00, 0x99, 0x1d, 0x00, 0x00};
  char tcon_top9[7] = {0x14, 0x40, 0x24, 0x3b, 0x00, 0x00, 0x00};
  char tcon_top9_2[7] = {0x14, 0x40, 0x24, 0x3b, 0x00, 0x00, 0x00};
  /*char tcon_top9[7] = {0x14,0x40,0x24,0x39,0x00,0x00,0x00};
  char tcon_top9_2[7] = {0x14,0x40,0x24,0x39,0x00,0x00,0x00};*/
  char tcon_topb[7] = {0x14, 0x40, 0x2c, 0x3f, 0x00, 0x00, 0x00};
  char tcon_topb_2[7] = {0x14, 0x40, 0x2c, 0x00, 0x00, 0x00, 0x00};
  char test_status[7] = {0x14, 0x40, 0x20, 0x50, 0x00, 0x00, 0x00};
  char key_info[7] = {0x14, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};
  int key_addr = 0;
  int key_addr_offset_msb = 0;
  int key_addr_offset_lsb = 0;
  int write_data_x_bits = 0;
  int mask = 0;
  int mask_data = 0;

  int write_data_by_offset = 0;
  char key_wr = 0;
  int key_data = 0;
  // char test_zw[2] = {0x14,0x40,0x00};
  int file_data;
  int file_addr;
  FILE *fp_fiti;
  FILE *fp_lut;

  fp_fiti = fopen("./data/F71/case660_fpga_4k1k_120hz_12pt1pr_boe_f71_ceds_setting_240711.txt", "r");

  if (fp_fiti == NULL)
    printf("error while opening file fiti_setting\n");

  for (i = 0;; i++) {
    if ((fscanf(fp_fiti, "0x%x, 0x%x,\n", &file_addr, &file_data)) < 0)
      break;
    // if((fscanf(fp_fiti,"%x, %x\n",&file_addr,&file_data))<0) break;
    test_zw3[1] = 0;
    test_zw3[2] = ((file_addr << 2) >> 8) & 0xff;
    test_zw3[3] = (file_addr << 2) & 0xff;
    test_zw3[4] = file_data & 0xff;
    test_zw3[5] = (file_data & 0xff00) >> 8;
    test_zw3[6] = (file_data & 0xff0000) >> 16;
    test_zw3[7] = (file_data & 0xff000000) >> 24;

    printf("i = %x, addr = %x data = %x\n", (i << 2), file_addr, file_data);
    USBIO_StreamI2C(0x00, 0x8, &test_zw3[0], 0x00, &tmp[2]);
    // USBIO_StreamI2C(0x00,0x4,&test_zw3[0],0x04,&tmp[2]);
  }
}

void e22_ceds_load_setting() {
  unsigned char buffer[2];
  // unsigned int test = 0;
  unsigned int w_test = 0;
  unsigned long test = 0;

  int j = 0;
  int i = 0;

  char *p_w;
  char *p;
  char data_buff[6] = {0x14, 0xfc, 0x01, 0x00, 0x00, 0x00};
  // unsigned long tmp[2];
  unsigned long tmp[3];

  // char  data_buff[6]  = {0x14,0x04,0x00,0x00,0x00,0x00};
  // char  data_buff2[6] = {0x14,0x04,0x00,0x00,0x00,0x00};

  char data_read[2] = {0x14, 0x04};
  // char  test_addr[2]   = {0x14,0xfc};
  // char  test_addr1[2]   = {0x14,0x00};
  char test_addr[2] = {0x14, 0x00};
  char test_addr1[2] = {0x14, 0x08}; //{0x14,0x02};
  char test_addr2[2] = {0x14, 0x14}; //{0x14,0x05};

  char test_addr3[2] = {0x14, 0x20}; //{0x14,0x08};
  char test_addr4[2] = {0x14, 0x24}; //{0x14,0x09};
  char test_addr5[2] = {0x14, 0xa0}; //{0x14,0x28};
  char test_addr6[2] = {0x14, 0xb0}; //{0x14,0x2c};

  char test_addr7[2] = {0x14, 0x10}; //{0x14,0x04};
  char test_addr8[2] = {0x14, 0x94}; //{0x14,0x25};

  char test_zw[8] = {0x14, 0x40, 0x00, 0x12, 0x34, 0x56, 0x78, 0x00};
  char test_zw2[8] = {0x14, 0x40, 0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0x00};
  char test_zw3[8] = {0x14, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00};

  char tcon_tope[7] = {0x14, 0x40, 0x38, 0x1f, 0x00, 0x00, 0x00};
  char tcon_top0[7] = {0x14, 0x40, 0x00, 0x99, 0x1d, 0x00, 0x00};
  char tcon_top9[7] = {0x14, 0x40, 0x24, 0x3b, 0x00, 0x00, 0x00};
  char tcon_top9_2[7] = {0x14, 0x40, 0x24, 0x3b, 0x00, 0x00, 0x00};
  /*char tcon_top9[7] = {0x14,0x40,0x24,0x39,0x00,0x00,0x00};
  char tcon_top9_2[7] = {0x14,0x40,0x24,0x39,0x00,0x00,0x00};*/
  char tcon_topb[7] = {0x14, 0x40, 0x2c, 0x3f, 0x00, 0x00, 0x00};
  char tcon_topb_2[7] = {0x14, 0x40, 0x2c, 0x00, 0x00, 0x00, 0x00};
  char test_status[7] = {0x14, 0x40, 0x20, 0x50, 0x00, 0x00, 0x00};
  char key_info[7] = {0x14, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};
  int key_addr = 0;
  int key_addr_offset_msb = 0;
  int key_addr_offset_lsb = 0;
  int write_data_x_bits = 0;
  int mask = 0;
  int mask_data = 0;

  int write_data_by_offset = 0;
  char key_wr = 0;
  int key_data = 0;
  // char test_zw[2] = {0x14,0x40,0x00};
  int file_data;
  int file_addr;
  FILE *fp_fiti;
  FILE *fp_lut;

  // fp_fiti = fopen("./data/e22/case670_fpga_4k2k_60hz_16pt1pr_e22_ceds_setting_240904.txt","r");
  // fp_fiti = fopen("./data/e22/case670_fpga_4k2k_60hz_16pt1pr_e22_ceds_setting_240904_2.txt","r");
  // fp_fiti = fopen("./data/e22/case670_fpga_4k2k_60hz_16pt1pr_e22_ceds_setting_240904_3.txt","r");
  fp_fiti = fopen("./data/e22/case670_fpga_4k2k_60hz_16pt1pr_e22_ceds_setting_240911_3.txt", "r");

  if (fp_fiti == NULL)
    printf("error while opening file fiti_setting\n");

  for (i = 0;; i++) {
    if ((fscanf(fp_fiti, "0x%x, 0x%x,\n", &file_addr, &file_data)) < 0)
      break;
    // if((fscanf(fp_fiti,"%x, %x\n",&file_addr,&file_data))<0) break;
    test_zw3[1] = 0;
    test_zw3[2] = ((file_addr << 2) >> 8) & 0xff;
    test_zw3[3] = (file_addr << 2) & 0xff;
    test_zw3[4] = file_data & 0xff;
    test_zw3[5] = (file_data & 0xff00) >> 8;
    test_zw3[6] = (file_data & 0xff0000) >> 16;
    test_zw3[7] = (file_data & 0xff000000) >> 24;

    printf("i = %x, addr = %x data = %x\n", (i << 2), file_addr, file_data);
    USBIO_StreamI2C(0x00, 0x8, &test_zw3[0], 0x00, &tmp[2]);
    // USBIO_StreamI2C(0x00,0x4,&test_zw3[0],0x04,&tmp[2]);
  }
}

void i2c_delay() {
}

void tcon_reg_test() {

#ifdef T3E_TCON_REG_TEST
  t3e_tcon_reg_test();
#else
  char write_data[8] = {0x14, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00};
  char read_info[7] = {0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  unsigned long tmp[3];
  unsigned long tmp1[3];
  for (int i = 0x100; i < 0x1500; i++) {

    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  printf("write all reg ffffffff done\n");

  FILE *fp_fiti;
  int j;
  int file_data;

  fp_fiti = fopen("TCON_register_init.txt", "w+");
  if (fp_fiti == NULL) {
    printf("Export TCON register text error (%s)\n", "TCON_register_init.txt");
    return;
  }

  for (j = 0x100; j <= 0x1500; j++) {

    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    if (tmp1[2] != 0)
      fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }

  fclose(fp_fiti);
  printf("Export TCON register text OK!\n");
#endif

  return;
}

// CEDS
void ceds_58f20_v03_pmic_init() {
  char test_infomat; // add by fxj to stop dos infomation
  unsigned char buffer[2];
  unsigned long wr_test[4];
  int i;
  unsigned long tmp[3];
  int key_addr = 0;
  char key_wr = 0;
  int key_data = 0;
  //	char key_info[7] = {0x14,0x40,0x00,0x00,0x00,0x00,0x00};
  char key_info[7] = {0xe8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};

  unsigned char reg_addr;
  unsigned long reg_data;

  int buf20800_vol[20] = {

      986,  // GM1,CEDS,15.08
      0,    // 2+
      0,    // 3+
      795,  // 4+,CEDS,12.2
      718,  // 5+,CEDS,11.05
      669,  // 6+,CEDS,10.31
      0,    // 7
      0,    // 8+
      519,  // 9+,CEDS,8.05
      473,  // 10+,CEDS,7.36
      0,    // 11+
      0,    // 12
      326,  // 13+,CEDS,5.14
      272,  // 14+,CEDS,4.33
      192,  // 15+,CEDS,3.11
      0,    // 16
      0,    // 17+
      6,    // 18+,CEDS,0.324
      458,  // 19,CEDS,VCOM1
      458}; // 20,CEDS,VCOM2 7.07

  int rt6937_vol[8] = {
      0x0c, // VDD
      0x01, // VCC1
      0x03, // VCC2
      0x04, // HVDD
      0x04, // VGH_LT
      0x05, // VGH_HT
      0x2a, // VGL
      0x07  // VGL2

  };
  // init_reg_rw();

  // test_usb_i2c_func();
  printf("hello world\n");

  wr_test[0] = 0x0000ff40;
  wr_test[1] = 0x0040;
  wr_test[2] = 0x0080ff40;
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);

  // gamma_writecommandbytes(0x0,0x4,&wr_test[0]);
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);
  buffer[0] = 0;
  buffer[1] = 0;

  /*
          // pmic
  USBIO_StreamI2C(0x0,  14, &pmic[0], 0, &buffer[0]);
  USBIO_StreamI2C(0x0,  3, &wr_test[0], 0, &buffer[0]);
  USBIO_StreamI2C(0x0,  3, &wr_test[2], 0, &buffer[0]);
  */

  // write buf20800
  wr_test[0] = 0x3f0007e8;
  for (i = 0; i < 20; i++) {
    wr_test[0] = 0;
    wr_test[0] = (((buf20800_vol[i] & 0xff) << 24) | (((buf20800_vol[i] >> 8) & 0xf) << 16) | (i << 8) | 0xe8);
    gamma_writecommandbytes(0x0, 0x4, &wr_test[0]);
  }

  // write rt6937
  for (i = 0; i < 8; i++) {
    wr_test[0] = 0;
    wr_test[0] = ((0x0 << 24) | ((rt6937_vol[i] & 0xff) << 16) | (i << 8) | 0x40);
    gamma_writecommandbytes(0x0, 0x4, &wr_test[0]);
  }
  wr_test[0] = ((0x0 << 24) | ((0x80) << 16) | (0xff << 8) | 0x40);
  gamma_writecommandbytes(0x0, 0x4, &wr_test[0]);
  /*
  while(1){
          printf("please input reg addr \n");
          scanf("%x",&reg_addr);
          printf("you entered %x\n",reg_addr);
          reg_data = rt6937_readcommandbytes(reg_addr);
          printf("the reg data of offset %x is  %x\n",reg_addr,(reg_data&0xff));
  }*/
}

// CEDS
void ceds_EDM_init() {
  char test_infomat; // add by fxj to stop dos infomation
  unsigned char buffer[2];
  unsigned long wr_test[4];
  int i;
  unsigned long tmp[3];
  int key_addr = 0;
  char key_wr = 0;
  int key_data = 0;
  //	char key_info[7] = {0x14,0x40,0x00,0x00,0x00,0x00,0x00};
  char key_info[7] = {0xe8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};

  unsigned char reg_addr;
  unsigned long reg_data;

  int buf20800_vol[20] = {

      997,  // GM1,CEDS,15.94
      0,    // 2+
      0,    // 3+
      0,    // 4+,CEDS,/
      781,  // 5+,CEDS,12.54
      714,  // 6+,CEDS,11.49
      0,    // 7
      0,    // 8+
      519,  // 9+,CEDS,8.41
      479,  // 10+,CEDS,7.78
      0,    // 11+
      0,    // 12
      338,  // 13+,CEDS,5.56
      266,  // 14+,CEDS,4.43
      0,    // 15+,CEDS,/
      0,    // 16
      0,    // 17+
      43,   // 18+,CEDS,0.92
      338,  // 19,CEDS,VCOM1 5.51
      338}; // 20,CEDS,VCOM2 5.51

  int rt6937_vol[8] = {
      0x0c, // VDD
      0x01, // VCC1
      0x03, // VCC2
      0x06, // HVDD 8.09
      0x07, // VGH_LT 28.2
      0x06, // VGH_HT
      0x35, // VGL -7.1
      0x0b  // VGL2 -10

  };
  // init_reg_rw();

  // test_usb_i2c_func();
  printf("hello world\n");

  wr_test[0] = 0x0000ff40;
  wr_test[1] = 0x0040;
  wr_test[2] = 0x0080ff40;
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);

  // gamma_writecommandbytes(0x0,0x4,&wr_test[0]);
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);
  buffer[0] = 0;
  buffer[1] = 0;

  /*
          // pmic
  USBIO_StreamI2C(0x0,  14, &pmic[0], 0, &buffer[0]);
  USBIO_StreamI2C(0x0,  3, &wr_test[0], 0, &buffer[0]);
  USBIO_StreamI2C(0x0,  3, &wr_test[2], 0, &buffer[0]);
  */

  // write buf20800
  wr_test[0] = 0x3f0007e8;
  for (i = 0; i < 20; i++) {
    wr_test[0] = 0;
    wr_test[0] = (((buf20800_vol[i] & 0xff) << 24) | (((buf20800_vol[i] >> 8) & 0xf) << 16) | (i << 8) | 0xe8);
    gamma_writecommandbytes(0x0, 0x4, &wr_test[0]);
  }

  // write rt6937
  for (i = 0; i < 8; i++) {
    wr_test[0] = 0;
    wr_test[0] = ((0x0 << 24) | ((rt6937_vol[i] & 0xff) << 16) | (i << 8) | 0x40);
    gamma_writecommandbytes(0x0, 0x4, &wr_test[0]);
  }
  wr_test[0] = ((0x0 << 24) | ((0x80) << 16) | (0xff << 8) | 0x40);
  gamma_writecommandbytes(0x0, 0x4, &wr_test[0]);
  /*
  while(1){
          printf("please input reg addr \n");
          scanf("%x",&reg_addr);
          printf("you entered %x\n",reg_addr);
          reg_data = rt6937_readcommandbytes(reg_addr);
          printf("the reg data of offset %x is  %x\n",reg_addr,(reg_data&0xff));
  }*/
}

// cs602 test
void d17_trigate_cs602_init() {
  char test_infomat; // add by fxj to stop dos infomation
  unsigned char buffer[2];
  unsigned long wr_test[4];
  int i;
  unsigned long tmp[3];
  int key_addr = 0;
  char key_wr = 0;
  int key_data = 0;
  //	char key_info[7] = {0x14,0x40,0x00,0x00,0x00,0x00,0x00};
  char key_info[7] = {0xe8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};

  unsigned char reg_addr;
  unsigned long reg_data;

  int readbuf[42];
  int test_buf[1] = {0};
  for (i = 0; i < 42; i++)
    readbuf[i] = 0;
  USBIO_WriteI2C(0x0, 0x33, 0x64, 0x80);
  int cs602_buf[42] = {
      0xc8,
      0x19,
      0x1b,
      0x00,
      0x2b,
      0x23,
      0xfc,
      0x86,
      0xf7,
      0x25,
      0x23,
      0x00,
      0x11,
      0x11,
      0x0b,
      0x0b,
      0x3e, //
      0xb3, //
      0xcf, // gam1gam2
      0x34, //
      0x72, //
      0xee, // gam3gam4
      0x2c, //
      0x2a, //
      0x45, // gam5gam6
      0x23, //
      0x11, //
      0x97, // gam7gam8
      0x18, //
      0xd1, //
      0x35, // gam9gam10
      0x0f, //
      0xa0, //
      0x97, // gam11gam12
      0x01, //
      0x40, //
      0x0d, // gam13gam14
      0x16,
      0x51,
      0x65,
      0xf0,
      0xcc

  };

  // init_reg_rw();

  // test_usb_i2c_func();
  printf("hello world\n");

  wr_test[0] = 0x0000ff40;
  wr_test[1] = 0x0040;
  wr_test[2] = 0x0080ff40;
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);

  // gamma_writecommandbytes(0x0,0x4,&wr_test[0]);
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);
  buffer[0] = 0;
  buffer[1] = 0;

  /*
          // pmic
  USBIO_StreamI2C(0x0,  14, &pmic[0], 0, &buffer[0]);
  USBIO_StreamI2C(0x0,  3, &wr_test[0], 0, &buffer[0]);
  USBIO_StreamI2C(0x0,  3, &wr_test[2], 0, &buffer[0]);
  */
  USBIO_WriteI2C(0x0, 0x33, 0x64, 0x80);
  wr_test[0] = 0x3f0007e8;
  for (i = 0; i < 42; i++) {
    // wr_test[0]=0;
    // wr_test[0]= ((cs602_buf[i] << 16) | (i << 8) | 0x66);
    // USBIO_WriteI2C(0x0,0x33,i,0x7f);
    printf("buf[%d] = %x\n", i, cs602_buf[i]);
    USBIO_WriteI2C(0x0, 0x33, i, cs602_buf[i]);
    for (int j = 0; j < 10000; j++) {
    }
    //	gamma_writecommandbytes(0x0,0x3,&wr_test[0]);
  }
  // USBIO_WriteI2C(0x0,0x33,0,cs602_buf[0]);
  // USBIO_WriteI2C(0x0,0x33,0,cs602_buf[0]);

  // for(i=0;i<42;i++)
  //{
  //   USBIO_ReadI2C(0x0,0x33,i,readbuf+i);
  //    for(int j = 0;j < 10000;j++){}
  //   printf("buf[%d] = %x\n",i,readbuf[i]);
  //}

  // for(i=0;i<5;i++)
  // {
  //    USBIO_ReadI2C(0x0,0x33,0x60+i,readbuf+i);
  //     for(int j = 0;j < 10000;j++){}
  //    printf("buf[%x] = %x\n",0x60+i,readbuf[i]);
  // }

  /*
  while(1){
          printf("please input reg addr \n");
          scanf("%x",&reg_addr);
          printf("you entered %x\n",reg_addr);
          reg_data = rt6937_readcommandbytes(reg_addr);
          printf("the reg data of offset %x is  %x\n",reg_addr,(reg_data&0xff));
  }*/
}

void pt430gt03_1_bl03_pmic_init() {
  char test_infomat; // add by fxj to stop dos infomation
  unsigned char buffer[2];
  unsigned long wr_test[4];
  int i;
  unsigned long tmp[3];
  int key_addr = 0;
  char key_wr = 0;
  int key_data = 0;
  //	char key_info[7] = {0x14,0x40,0x00,0x00,0x00,0x00,0x00};
  char key_info[7] = {0xe8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};

  unsigned char reg_addr;
  unsigned long reg_data;

  uint8_t readbuf[44];
  int test_buf[1] = {0};
  for (i = 0; i < 44; i++)
    readbuf[i] = 0;
  USBIO_WriteI2C(0x0, 0x33, 0x64, 0x80);
  char cs602_buf[46] = {
      0x66,
      0x00,
      0x48,
      0x1a,
      0x27,
      0x00,
      0x21,
      0x2b,
      0xf4,
      0x7a,
      0x7f,
      0x2a,
      0x28,
      0xf2,
      0x4e,
      0x20,
      0x0b,
      0x0b,
      0x39, //
      0x03, //
      0x8b, // gam1gam2
      0x30, //
      0x42, //
      0xa6, // gam3gam4
      0x28, //
      0xe1, //
      0xf8, // gam5gam6
      0x1f, //
      0x81, //
      0xb1, // gam7gam8
      0x1a, //
      0xe1, //
      0x26, // gam9gam10
      0x0e, //
      0x70, //
      0x9c, // gam11gam12
      0x01, //
      0xf0, //
      0x1b, // gam13gam14
      0x1b,
      0xe1,
      0x5f,
      0x00,
      0x00,
      0x01,
      0x5a};

  printf("hello world\n");

  wr_test[0] = 0x0000ff40;
  wr_test[1] = 0x0040;
  wr_test[2] = 0x0080ff40;
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);

  // gamma_writecommandbytes(0x0,0x4,&wr_test[0]);
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);
  buffer[0] = 0;
  buffer[1] = 0;

  wr_test[0] = 0x3f0007e8;

  USBIO_StreamI2C(0x0, 46, &cs602_buf[0], 0, &buffer[0]);
}

void cspi_dlg_d12_8_load_setting() {
  unsigned char buffer[2];
  // unsigned int test = 0;
  unsigned int w_test = 0;
  unsigned long test = 0;

  int j = 0;
  int i = 0;

  char *p_w;
  char *p;
  char data_buff[6] = {0x14, 0xfc, 0x01, 0x00, 0x00, 0x00};
  // unsigned long tmp[2];
  unsigned long tmp[3];

  // char  data_buff[6]  = {0x14,0x04,0x00,0x00,0x00,0x00};
  // char  data_buff2[6] = {0x14,0x04,0x00,0x00,0x00,0x00};

  char data_read[2] = {0x14, 0x04};
  // char  test_addr[2]   = {0x14,0xfc};
  // char  test_addr1[2]   = {0x14,0x00};
  char test_addr[2] = {0x14, 0x00};
  char test_addr1[2] = {0x14, 0x08}; //{0x14,0x02};
  char test_addr2[2] = {0x14, 0x14}; //{0x14,0x05};

  char test_addr3[2] = {0x14, 0x20}; //{0x14,0x08};
  char test_addr4[2] = {0x14, 0x24}; //{0x14,0x09};
  char test_addr5[2] = {0x14, 0xa0}; //{0x14,0x28};
  char test_addr6[2] = {0x14, 0xb0}; //{0x14,0x2c};

  char test_addr7[2] = {0x14, 0x10}; //{0x14,0x04};
  char test_addr8[2] = {0x14, 0x94}; //{0x14,0x25};

  char test_zw[8] = {0x14, 0x40, 0x00, 0x12, 0x34, 0x56, 0x78, 0x00};
  char test_zw2[8] = {0x14, 0x40, 0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0x00};
  char test_zw3[8] = {0x14, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00};

  char tcon_tope[7] = {0x14, 0x40, 0x38, 0x1f, 0x00, 0x00, 0x00};
  char tcon_top0[7] = {0x14, 0x40, 0x00, 0x99, 0x1d, 0x00, 0x00};
  char tcon_top9[7] = {0x14, 0x40, 0x24, 0x3b, 0x00, 0x00, 0x00};
  char tcon_top9_2[7] = {0x14, 0x40, 0x24, 0x3b, 0x00, 0x00, 0x00};
  /*char tcon_top9[7] = {0x14,0x40,0x24,0x39,0x00,0x00,0x00};
  char tcon_top9_2[7] = {0x14,0x40,0x24,0x39,0x00,0x00,0x00};*/
  char tcon_topb[7] = {0x14, 0x40, 0x2c, 0x3f, 0x00, 0x00, 0x00};
  char tcon_topb_2[7] = {0x14, 0x40, 0x2c, 0x00, 0x00, 0x00, 0x00};
  char test_status[7] = {0x14, 0x40, 0x20, 0x50, 0x00, 0x00, 0x00};
  char key_info[7] = {0x14, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};
  int key_addr = 0;
  int key_addr_offset_msb = 0;
  int key_addr_offset_lsb = 0;
  int write_data_x_bits = 0;
  int mask = 0;
  int mask_data = 0;

  int write_data_by_offset = 0;
  char key_wr = 0;
  int key_data = 0;
  // char test_zw[2] = {0x14,0x40,0x00};
  int file_data;
  int file_addr;
  FILE *fp_fiti;
  FILE *fp_lut;
  //

  // d12-8
  // fp_fiti = fopen("./data/case650_ud120_dlg_20240529_120hz_setting_v1.txt","r");
  // fp_fiti = fopen("./data/case650_ud120_dlg_20240605_120hz_setting.txt","r");
  // fp_fiti = fopen("./data/uhd60_case641_fpga_hkc430gt03_1_6P2Pr_isp_setting20240514_posttiming_ok_setting_p2p_dbg.txt","r");

  // fp_fiti = fopen("./data/ud120_dlg_20240618_60hz_setting_vrr_test.txt","r");
  // fp_fiti = fopen("./data/uhd120_dlg_d12_8_vrr_test_20240606.txt","r"); // vrr ok

  // fp_fiti = fopen("./data/uhd120_dlg_d12_8_vrr_line_gam_20240617.txt","r");
  // fp_fiti = fopen("./data/uhd120_dlg_d12_8_vrr_line_gam_20240625_base.txt","r");
  // fp_fiti = fopen("./data/uhd120_dlg_case650_blanktoggle_20240701.txt","r");

  // vdf test
  // fp_fiti = fopen("./data/uhd120_dlg_case650_blanktoggle_vdf_path_20240722.txt","r");

  // fp_fiti = fopen("./data/uhd120_dlg_case650_blanktoggle_sram_20240731.txt","r");
  // fp_fiti = fopen("./data/uhd120_dlg_case650_blanktoggle_sram_20240802.txt","r");
  // fp_fiti = fopen("./data/uhd120_dlg_case650_blanktoggle_sram_20240805.txt","r");
  // fp_fiti = fopen("./data/uhd120_dlg_case650_blanktoggle_sram_20240806.txt","r");  //add dither 4ppc
  // fp_fiti = fopen("./data/uhd120_dlg_case650_blanktoggle_sram_20240807_i2c.txt","r");  //add dither 4ppc
  // fp_fiti = fopen("./data/uhd120_dlg_case650_blanktoggle_sram_20240807_all_lut.txt","r");  //add dither 4ppc

  // dly 0frm
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240806_lut16.txt","r");  //add dither 4ppc
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240806_lut32.txt","r");  //add dither 4ppc
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240806_lut48.txt","r");  //add dither 4ppc
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240806_lut64.txt","r");  //add dither 4ppc
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240806_lut80.txt","r");  //add dither 4ppc
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240806_lut96.txt","r");  //add dither 4ppc
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240806_lut112.txt","r");  //add dither 4ppc
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240806_lut128.txt","r");  //add dither 4ppc
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240806_lut256.txt","r");  //add dither 4ppc
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240808_base16.txt","r");  //add dither 4ppc
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240808_checklist.txt","r");  //add dither 4ppc

  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240809_checklist_16.txt","r");  //add dither 4ppc
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240809_checklist_24.txt","r");  //add dither 4ppc
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240809_checklist_32.txt","r");  //add dither 4ppc
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240809_checklist_64.txt","r");  //add dither 4ppc
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240809_checklist_8.txt","r");  //add dither 4ppc

  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240809_checklist_8_dither.txt","r");
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240809_checklist_16_dither.txt","r");
  fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240809_checklist_16_dither_i2c.txt", "r");
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240809_checklist_24_dither.txt","r");

  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240809_checklist_16_16dither.txt","r");
  // fp_fiti = fopen("./data/vdf_setting/uhd120_dlg_case650_blanktoggle_sram_20240809_checklist_16_16dither_sel_pol.txt","r");  //add dither 4ppc

  if (fp_fiti == NULL)
    printf("error while opening file fiti_setting\n");

  for (i = 0;; i++) {
    // if((fscanf(fp_fiti,"%x, %x\n",&file_addr,&file_data))<0) break;
    if ((fscanf(fp_fiti, "0x%x, 0x%x,\n", &file_addr, &file_data)) < 0)
      break;
    test_zw3[1] = ((file_addr << 2) >> 16) & 0xff;
    test_zw3[2] = ((file_addr << 2) >> 8) & 0xff;
    test_zw3[3] = (file_addr << 2) & 0xff;
    test_zw3[4] = file_data & 0xff;
    test_zw3[5] = (file_data & 0xff00) >> 8;
    test_zw3[6] = (file_data & 0xff0000) >> 16;
    test_zw3[7] = (file_data & 0xff000000) >> 24;

    printf("i = %x, addr = %x data = %x\n", (i << 2), file_addr, file_data);
    USBIO_StreamI2C(0x00, 0x8, &test_zw3[0], 0x00, &tmp[2]);
    // USBIO_StreamI2C(0x00,0x4,&test_zw3[0],0x04,&tmp[2]);
  }
}

void cspi_ud60_d12_8_load_setting() {
  unsigned char buffer[2];
  // unsigned int test = 0;
  unsigned int w_test = 0;
  unsigned long test = 0;

  int j = 0;
  int i = 0;

  char *p_w;
  char *p;
  char data_buff[6] = {0x14, 0xfc, 0x01, 0x00, 0x00, 0x00};
  // unsigned long tmp[2];
  unsigned long tmp[3];

  // char  data_buff[6]  = {0x14,0x04,0x00,0x00,0x00,0x00};
  // char  data_buff2[6] = {0x14,0x04,0x00,0x00,0x00,0x00};

  char data_read[2] = {0x14, 0x04};
  // char  test_addr[2]   = {0x14,0xfc};
  // char  test_addr1[2]   = {0x14,0x00};
  char test_addr[2] = {0x14, 0x00};
  char test_addr1[2] = {0x14, 0x08}; //{0x14,0x02};
  char test_addr2[2] = {0x14, 0x14}; //{0x14,0x05};

  char test_addr3[2] = {0x14, 0x20}; //{0x14,0x08};
  char test_addr4[2] = {0x14, 0x24}; //{0x14,0x09};
  char test_addr5[2] = {0x14, 0xa0}; //{0x14,0x28};
  char test_addr6[2] = {0x14, 0xb0}; //{0x14,0x2c};

  char test_addr7[2] = {0x14, 0x10}; //{0x14,0x04};
  char test_addr8[2] = {0x14, 0x94}; //{0x14,0x25};

  char test_zw[8] = {0x14, 0x40, 0x00, 0x12, 0x34, 0x56, 0x78, 0x00};
  char test_zw2[8] = {0x14, 0x40, 0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0x00};
  char test_zw3[8] = {0x14, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00};

  char tcon_tope[7] = {0x14, 0x40, 0x38, 0x1f, 0x00, 0x00, 0x00};
  char tcon_top0[7] = {0x14, 0x40, 0x00, 0x99, 0x1d, 0x00, 0x00};
  char tcon_top9[7] = {0x14, 0x40, 0x24, 0x3b, 0x00, 0x00, 0x00};
  char tcon_top9_2[7] = {0x14, 0x40, 0x24, 0x3b, 0x00, 0x00, 0x00};
  /*char tcon_top9[7] = {0x14,0x40,0x24,0x39,0x00,0x00,0x00};
  char tcon_top9_2[7] = {0x14,0x40,0x24,0x39,0x00,0x00,0x00};*/
  char tcon_topb[7] = {0x14, 0x40, 0x2c, 0x3f, 0x00, 0x00, 0x00};
  char tcon_topb_2[7] = {0x14, 0x40, 0x2c, 0x00, 0x00, 0x00, 0x00};
  char test_status[7] = {0x14, 0x40, 0x20, 0x50, 0x00, 0x00, 0x00};
  char key_info[7] = {0x14, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};
  int key_addr = 0;
  int key_addr_offset_msb = 0;
  int key_addr_offset_lsb = 0;
  int write_data_x_bits = 0;
  int mask = 0;
  int mask_data = 0;

  int write_data_by_offset = 0;
  char key_wr = 0;
  int key_data = 0;
  // char test_zw[2] = {0x14,0x40,0x00};s
  int file_data;
  int file_addr;
  FILE *fp_fiti;
  FILE *fp_lut;
  //

  // d12-8

  // fp_fiti = fopen("./data/uhd120_dlg_case650_blanktoggle_20240701.txt","r");

  // ud60
  // fp_fiti = fopen("./data/t5m_case0325_d12_8_vac_20240731_setting_v1_tableok.txt","r"); //vac ok,�߽���˸
  // fp_fiti = fopen("./data/case626_cspi_125g_vac_setting_20240806_33table.txt","r"); //vac golden
  // fp_fiti = fopen("./data/t5m_case0325_d12_8_vac_20240731_tableok_new_fpga_setting.txt","r");
  // fp_fiti = fopen("./data/case626_cspi_125g_vac_setting_20240805_255table.txt","r");
  // fp_fiti = fopen("./data/ud60_case205_demura_20240808_zhiyuan.txt","r");
  // fp_fiti = fopen("./data/ud60_case205_demura_20240808_yujie.txt","r");
  // fp_fiti = fopen("./data/ud60_case205_demura_20240809.txt","r");
  // fp_fiti = fopen("./data/ud60_case205_demura_20240822_intf.txt","r");

  // fp_fiti = fopen("./data/t5m_case0325_d12_8_vac_20240731_setting_v1_tableok.txt","r"); //vac ok,�߽���˸
  // fp_fiti = fopen("./data/case626_cspi_125g_vac_setting_20240806_33table.txt","r"); //vac golden
  // fp_fiti = fopen("./data/t5m_case0325_d12_8_vac_20240731_tableok_new_fpga_setting.txt","r");
  // fp_fiti = fopen("./data/case626_cspi_125g_vac_setting_20240805_255table.txt","r");
  // fp_fiti = fopen("./data/ud60_vac/case626_vac_cspi_d12_8_setting_20240814.txt","r");  // lut change to reg ok

  // fp_fiti = fopen("./data/ud60_vac/case626_vac_cspi_d12_8_setting_cd_20240816.txt","r");  // color detection ok

  // fp_fiti = fopen("./data/ud60_vac/case626_vac_cspi_d12_8_cd_setting_20240828.txt","r");

  // fp_fiti = fopen("./data/ud60_vac/case626_vac_cspi_d12_8_setting_4_mapping_20240821_l0h0l1h1.txt","r");

  // fp_fiti = fopen("./data/ud60_vac/case626_vac_cspi_d12_8_setting_4_mapping_20240821_h1l1.txt","r");//ok

  // fp_fiti = fopen("./data/ud60_vac/case626_vac_cspi_d12_8_setting_20240827_t5w_test.txt","r");

  // lut dma test
  // fp_fiti = fopen("./data/ud60_lut_dma/ud60_case627_lut_dma_test_vrr.txt","r");

  // fp_fiti = fopen("./data/ud60_lut_dma/ud60_case627_lut_dma_test.txt","r");

  // uhd120 60hz hctc tests
  // fp_fiti = fopen("./data/d12_8_hctc/case651_fpga_4k2k_60hz_d12_8_setting_20241204.txt","r");

  // fp_fiti = fopen("./data/d12_8_hctc/case651_fpga_4k2k_60hz_d12_8_setting_20241209.txt","r");
  // fp_fiti = fopen("./data/d12_8_hctc/case651_fpga_55d12_8_isp_60hz_for_hctc_setting_241211.txt","r");
  // fp_fiti = fopen("./data/d12_8_hctc/case651_fpga_55d12_8_isp_60hz_for_hctc_setting_250114.txt","r");
  fp_fiti = fopen("./data/d12_8_hctc/case651_fpga_55d12_8_isp_60hz_for_hctc_setting_250117.txt", "r");

  if (fp_fiti == NULL)
    printf("error while opening file fiti_setting\n");

  for (i = 0;; i++) {
    // if((fscanf(fp_fiti,"%x, %x\n",&file_addr,&file_data))<0) break;
    if ((fscanf(fp_fiti, "0x%x, 0x%x,\n", &file_addr, &file_data)) < 0)
      break;
    test_zw3[1] = 0;
    test_zw3[2] = ((file_addr << 2) >> 8) & 0xff;
    test_zw3[3] = (file_addr << 2) & 0xff;
    test_zw3[4] = file_data & 0xff;
    test_zw3[5] = (file_data & 0xff00) >> 8;
    test_zw3[6] = (file_data & 0xff0000) >> 16;
    test_zw3[7] = (file_data & 0xff000000) >> 24;

    printf("i = %x, addr = %x data = %x\n", (i << 2), file_addr, file_data);
    USBIO_StreamI2C(0x00, 0x8, &test_zw3[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &test_zw3[0], 0x04, &tmp[2]);
  }
}

void dlg_ud120_pmic_init() {
  char test_infomat; // add by fxj to stop dos infomation
  unsigned char buffer[2];
  unsigned long wr_test[4];
  int i;
  unsigned long tmp[3];
  int key_addr = 0;
  char key_wr = 0;
  int key_data = 0;
  //	char key_info[7] = {0x14,0x40,0x00,0x00,0x00,0x00,0x00};
  char key_info[7] = {0xe8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};

  unsigned char reg_addr;
  unsigned long reg_data;

  uint8_t readbuf[46];
  int test_buf[1] = {0};
  for (i = 0; i < 46; i++)
    readbuf[i] = 0;
  USBIO_WriteI2C(0x0, 0x33, 0x64, 0x80);
  /*
          char cs602_buf[46] = {
      0x66,
      0x00,
      0x48, //0
      0x19, //1
      0x24, //2
      0x00, //3
      0x23, //4
      0x0b, //5
      0xf0, //6
      0x7a, //7
      0x7f, //8
      0x2b, //9
      0x29, //a
      0xe1, //b
      0x10, //c
      0x10, //d
      0x07, //e
      0x07, //f
      0x3d, //10  //
      0xe3, //11  //
      0xc6, //12 //gam1gam2
      0x33, //13
      0x12, //14
      0xda, //15 //gam3gam4
      0x2a, //16
      0xd2, //17
      0x34, //18 //gam5gam6
      0x21, //19
      0x31, //1a
      0xb9, //1b //gam7gam8
      0x1a, //1c
      0x11, //1d
      0x34, //1e //gam9gam10
      0x0f, //1f
      0x80, //20
      0x95, //21 //gam11gam12
      0x01, //22
      0x60, //23
      0x0d, //24 //gam13gam14
      0x23, //25
      0x01, //26
      0xa2, //27
      0x00, //28
      0x00, //29
      0x01, //2a
      0xa5  //2b //vcom2
      };
  */

  char cs602_buf[46] = {
      0x66,
      0x00,
      0x48, // 0
      0x19, // 1
      0x24, // 2
      0x00, // 3
      0x23, // 4
      0x0b, // 5
      0xf0, // 6
      0x7a, // 7
      0x7f, // 8
      0x2b, // 9
      0x29, // a
      0xe1, // b
      0x10, // c
      0x10, // d
      0x07, // e
      0x07, // f
      0x3d, // 10  //
      0xe3, // 11  //
      0xc6, // 12 //gam1gam2
      0x33, // 13
      0x12, // 14
      0xda, // 15 //gam3gam4
      0x2a, // 16
      0xd2, // 17
      0x34, // 18 //gam5gam6
      0x21, // 19
      0x31, // 1a
      0xb9, // 1b //gam7gam8
      0x1a, // 1c
      0x11, // 1d
      0x34, // 1e //gam9gam10
      0x0f, // 1f
      0x80, // 20
      0x95, // 21 //gam11gam12
      0x01, // 22
      0x60, // 23
      0x0d, // 24 //gam13gam14
      0x23, // 25
      0x01, // 26
      0xa2, // 27
      0x00, // 28
      0x00, // 29
      0x01, // 2a
      0xa5  // 2b //vcom2  //a8 6.13//58 4.96//f5 7.24//95 5.86//ba 6.39//aa 6.16//a5 6.09
  };

  printf("hello world\n");

  wr_test[0] = 0x0000ff40;
  wr_test[1] = 0x0040;
  wr_test[2] = 0x0080ff40;
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);

  // gamma_writecommandbytes(0x0,0x4,&wr_test[0]);
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);
  buffer[0] = 0;
  buffer[1] = 0;

  wr_test[0] = 0x3f0007e8;

  USBIO_StreamI2C(0x0, 46, &cs602_buf[0], 0, &buffer[0]);
}

void d028_e22_cs602_pmic_init() {
  char test_infomat; // add by fxj to stop dos infomation
  unsigned char buffer[2];
  unsigned long wr_test[4];
  int i;
  unsigned long tmp[3];
  int key_addr = 0;
  char key_wr = 0;
  int key_data = 0;
  //	char key_info[7] = {0x14,0x40,0x00,0x00,0x00,0x00,0x00};
  char key_info[7] = {0xe8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};

  unsigned char reg_addr;
  unsigned long reg_data;

  uint8_t readbuf[44];
  int test_buf[1] = {0};
  for (i = 0; i < 44; i++)
    readbuf[i] = 0;
  USBIO_WriteI2C(0x0, 0x33, 0x64, 0x80);
  USBIO_WriteI2C(0x0, 0x36, 0x64, 0x80);
  char cs602_buf[44] = {
      0x66,
      0x00,
      0xc8, // 0
      0x19, // 1
      0xa4, // 2
      0x00, // 3
      0x26, // 4 // LVGL
      0x23, // 5
      0xfc, // 6
      0x66, // 7
      0xfb, // 8
      0x33, // 9  //AVDD
      0x2f, // a
      0x00, // b
      0x0e, // c  //VGH
      0x10, // d
      0x03, // e //VGL
      0x07, // f
      0x3e, // 10  //
      0xf3, // 11  //
      0x50, // 12 //gam1gam2
      0x2f, // 13
      0x02, // 14
      0xde, // 15 //gam3gam4
      0x2b, // 16
      0x02, // 17
      0x3d, // 18 //gam5gam6
      0x21, // 19
      0xa1, // 1a
      0xef, // 1b //gam7gam8
      0x1a, // 1c
      0x71, // 1d
      0x5a, // 1e //gam9gam10
      0x0f, // 1f
      0xd1, // 20
      0x1a, // 21 //gam11gam12
      0x0b, // 22
      0xf0, // 23
      0x18, // 24 //gam13gam14
      0x28, // 25
      0x01, // 26
      0xe0, // 27 //vcom0 1
      0x3d, // 28
      0xf9  // 29
  };

  char cs603_buf[44] = {
      0x6c,
      0x00,
      0x48, // 0
      0x09, // 1
      0xA4, // 2
      0x10, // 3
      0x23, // 4
      0x85, // 5
      0xFC, // 6
      0x56, // 7
      0x77, // 8
      0x0B, // 9
      0x08, // a
      0x00, // b
      0x0e, // c    //VGH
      0x03, // d    //VGL
      0x01, // e
      0xFF, // f
      0x3E, // 10
      0x63, // 11
      0xCA, // 12
      0x32, // 13
      0x42, // 14
      0xCC, // 15
      0x29, // 16
      0xD2, // 17
      0x34, // 18
      0x21, // 19
      0x81, // 1a
      0xBD, // 1b
      0x1A, // 1c
      0x11, // 1d
      0x33, // 1e
      0x0F, // 1f
      0xF0, // 20
      0xA3, // 21
      0x01, // 22
      0xC0, // 23
      0x0D, // 24
      0x1A, // 25
      0x11, // 26
      0xCF, // 27
      0x9c, // 28
      0x92  // 29
  };

  printf("hello world\n");

  wr_test[0] = 0x0000ff40;
  wr_test[1] = 0x0040;
  wr_test[2] = 0x0080ff40;
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);

  // gamma_writecommandbytes(0x0,0x4,&wr_test[0]);
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);
  buffer[0] = 0;
  buffer[1] = 0;

  wr_test[0] = 0x3f0007e8;

  USBIO_StreamI2C(0x0, 44, &cs602_buf[0], 0, &buffer[0]); // config cs602
  USBIO_StreamI2C(0x0, 44, &cs603_buf[0], 0, &buffer[0]); // config cs603, only for stv1B
}
void f71_ud120_cs602_pmic_init() {
  char test_infomat; // add by fxj to stop dos infomation
  unsigned char buffer[2];
  unsigned long wr_test[4];
  int i;
  unsigned long tmp[3];
  int key_addr = 0;
  char key_wr = 0;
  int key_data = 0;
  //	char key_info[7] = {0x14,0x40,0x00,0x00,0x00,0x00,0x00};
  char key_info[7] = {0xe8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};

  unsigned char reg_addr;
  unsigned long reg_data;

  int readbuf[46];
  int test_buf[1] = {0};
  for (i = 0; i < 46; i++)
    readbuf[i] = 0;
  USBIO_WriteI2C(0x0, 0x33, 0x64, 0x80);
  char cs602_buf[46] = {
      0x66,
      0x00,
      0xc8,
      0x19,
      0x19,
      0x01,
      0x27,
      0x2b,
      0xf8,
      0x6a,
      0x7f,
      0x2f,
      0x2d,
      0xe3,
      0x1c,
      0x1c,
      0x0b,
      0x0b,
      0x3d,
      0xc0,
      0x00,
      0x34,
      0x52,
      0xe8,
      0x2b,
      0x70,
      0x00,
      0x21,
      0x21,
      0xe2,
      0x00,
      0x01,
      0x3e,
      0x10,
      0xe0,
      0xae,
      0x00,
      0x00,
      0x19,
      0x1d,
      0x80,
      0x00,
      0x00,
      0x00,
      0x00,
      0x00};

  printf("hello world\n");

  wr_test[0] = 0x0000ff40;
  wr_test[1] = 0x0040;
  wr_test[2] = 0x0080ff40;
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);

  // gamma_writecommandbytes(0x0,0x4,&wr_test[0]);
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);
  buffer[0] = 0;
  buffer[1] = 0;

  wr_test[0] = 0x3f0007e8;

  USBIO_StreamI2C(0x0, 46, &cs602_buf[0], 0, &buffer[0]);
}

void d17_trigate_cs602_read() {
  char test_infomat; // add by fxj to stop dos infomation
  unsigned char buffer[2];
  unsigned long wr_test[4];
  int i;
  unsigned long tmp[3];
  int key_addr = 0;
  char key_wr = 0;
  int key_data = 0;
  //	char key_info[7] = {0x14,0x40,0x00,0x00,0x00,0x00,0x00};
  char key_info[7] = {0xe8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};

  unsigned char reg_addr;
  unsigned long reg_data;

  uint8_t readbuf[44];
  int test_buf[1] = {0};
  for (i = 0; i < 44; i++)
    readbuf[i] = 0;

  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);

  // gamma_writecommandbytes(0x0,0x4,&wr_test[0]);
  buffer[0] = 0;
  buffer[1] = 0;
  // gamma_readcommandbytes(0x0,2,&buffer[0]);
  buffer[0] = 0;
  buffer[1] = 0;

  for (i = 0; i < 44; i++) {
    USBIO_ReadI2C(0x0, 0x33, i, readbuf + i);
    for (int j = 0; j < 10000; j++) {
    }
    printf("buf[%x] = %x\n", i, readbuf[i]);
  }

  for (i = 0; i < 5; i++) {
    USBIO_ReadI2C(0x0, 0x33, 0x60 + i, readbuf + i);
    for (int j = 0; j < 10000; j++) {
    }
    printf("buf[%x] = %x\n", 0x60 + i, readbuf[i]);
  }
}

void dump_pclrc_reg() {
  FILE *fp_fiti;
  int i;
  int file_addr;
  int ret_data;
  fp_fiti = fopen("dump_pclrc_reg.txt", "w");
  //  test
  /*
          for(i=0;i<10;i++){
      file_addr = 0x1400 + i ;
      ret_data = ReadReg((file_addr<<2));
      fprintf(fp_fiti,"%x : %x\n",file_addr, ret_data);
          }
  */
  WriteReg(0x610 << 2, 1);
  for (i = 0; i < 3136; i++) {
    file_addr = 0x611;
    ret_data = ReadReg((file_addr << 2));
    fprintf(fp_fiti, "0x%x, 0x%x,\n", file_addr, ret_data);
  }
  WriteReg(0x610 << 2, 0);

  fclose(fp_fiti);
  return;
}

void t3e_tcon_reg_test() {
  char write_data[8] = {0x14, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00};
  char read_info[7] = {0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  unsigned long tmp[3];
  unsigned long tmp1[3];
  // aptu
  for (int i = 0x300; i <= 0x371; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // avac
  for (int i = 0x290; i <= 0x2a5; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  for (int i = 0x2a7; i <= 0x2a8; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // lod
  for (int i = 0x150; i <= 0x16c; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // p2p_chpi_bcc
  for (int i = 0x500; i <= 0x516; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  for (int i = 0x519; i <= 0x519; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // p2p
  for (int i = 0x400; i <= 0x4f1; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  for (int i = 0x4fb; i <= 0x4fe; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // post_top
  for (int i = 0x100; i <= 0x13f; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // pre_cmpr
  for (int i = 0x261; i <= 0x27e; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  for (int i = 0x261; i <= 0x265; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  for (int i = 0x269; i <= 0x27e; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // pre_demura
  for (int i = 0x190; i <= 0x1b7; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  for (int i = 0x1b9; i <= 0x1ba; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // pre_od
  for (int i = 0x240; i <= 0x250; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // pre_pd
  for (int i = 0x1300; i <= 0x137c; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  for (int i = 0x137e; i <= 0x139b; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // pre_top
  for (int i = 0x200; i <= 0x225; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  for (int i = 0x22d; i <= 0x233; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  printf("write all reg ffffffff done\n");

  FILE *fp_fiti;
  int j;
  int file_data;

  fp_fiti = fopen("TCON_register_init.txt", "w+");
  if (fp_fiti == NULL) {
    printf("Export TCON register text error (%s)\n", "TCON_register_init.txt");
    return;
  }

  // aptu
  for (int j = 0x300; j <= 0x371; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // avac
  for (int j = 0x290; j <= 0x2a5; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  for (int j = 0x2a7; j <= 0x2a8; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // lod
  for (int j = 0x150; j <= 0x16c; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // p2p_chpi_bcc
  for (int j = 0x500; j <= 0x516; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  for (int j = 0x519; j <= 0x519; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // p2p
  for (int j = 0x400; j <= 0x4f1; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  for (int j = 0x4fb; j <= 0x4fe; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // post_top
  for (int j = 0x100; j <= 0x13f; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // pre_cmpr
  for (int j = 0x261; j <= 0x27e; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  for (int j = 0x261; j <= 0x265; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  for (int j = 0x269; j <= 0x27e; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // pre_demura
  for (int j = 0x190; j <= 0x1b7; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  for (int j = 0x1b9; j <= 0x1ba; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // pre_od
  for (int j = 0x240; j <= 0x250; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // pre_pd
  for (int j = 0x1300; j <= 0x137c; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  for (int j = 0x137e; j <= 0x139b; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // pre_top
  for (int j = 0x200; j <= 0x225; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  for (int j = 0x22d; j <= 0x233; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }

  fclose(fp_fiti);
  printf("Export T3e TCON register text OK!\n");
}

void t6w_tcon_reg_test() {
  char write_data[8] = {0x14, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00};
  char read_info[7] = {0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  unsigned long tmp[3];
  unsigned long tmp1[3];
  // aptu
  for (int i = 0x300; i <= 0x377; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // avac
  for (int i = 0x1500; i <= 0x15e7; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // asf
  for (int i = 0x500; i <= 0x5b7; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // cmt
  for (int i = 0x1400; i <= 0x14ba; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // ip32
  for (int i = 0x1001; i <= 0x1031; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // intf
  for (int i = 0x0; i <= 0x8; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // lod
  for (int i = 0x150; i <= 0x172; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // lut dma
  for (int i = 0x5d0; i <= 0x5dd; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // p2p_chpi_bcc
  for (int i = 0x500; i <= 0x51d; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // p2p
  for (int i = 0x400; i <= 0x4ff; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // post_top
  for (int i = 0x100; i <= 0x14a; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // pre_cmpr
  for (int i = 0x261; i <= 0x27e; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // pre_demura
  for (int i = 0x190; i <= 0x1bd; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // pre_od
  for (int i = 0x240; i <= 0x250; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // pre_pd
  for (int i = 0x1300; i <= 0x139b; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // pre_top
  for (int i = 0x200; i <= 0x23e; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  // vctc
  for (int i = 0x10; i <= 0x3e; i++) {
    write_data[1] = 0;
    write_data[2] = ((i << 2) >> 8) & 0xff;
    write_data[3] = (i << 2) & 0xff;
    write_data[4] = 0xff;
    write_data[5] = 0xff;
    write_data[6] = 0xff;
    write_data[7] = 0xff;

    USBIO_StreamI2C(0x00, 0x8, &write_data[0], 0x00, &tmp[2]);
    USBIO_StreamI2C(0x00, 0x4, &write_data[0], 0x04, &tmp[2]);
    printf("i = %x, addr = %x data = %x\n", i, i, 0xffffffff);
  }
  printf("write all reg ffffffff done\n");

  FILE *fp_fiti;
  int j;
  int file_data;

  fp_fiti = fopen("t6w_TCON_register_init.txt", "w+");
  if (fp_fiti == NULL) {
    printf("Export TCON register text error (%s)\n", "TCON_register_init.txt");
    return;
  }

  // aptu
  for (int j = 0x300; j <= 0x377; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // avac
  for (int j = 0x1500; j <= 0x15e7; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // asf
  for (int j = 0x550; j <= 0x5b7; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // cmt
  for (int j = 0x1400; j <= 0x14ba; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // ip32
  for (int j = 0x1001; j <= 0x1031; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // intf
  for (int j = 0x0; j <= 0x8; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // lod
  for (int j = 0x150; j <= 0x172; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // lut dma
  for (int j = 0x5d0; j <= 0x5dd; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // p2p_chpi_bcc
  for (int j = 0x500; j <= 0x51d; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // p2p
  for (int j = 0x400; j <= 0x4ff; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // post_top
  for (int j = 0x100; j <= 0x14a; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // pre_cmpr
  for (int j = 0x261; j <= 0x27e; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // pre_demura
  for (int j = 0x190; j <= 0x1bd; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // pre_od
  for (int j = 0x240; j <= 0x250; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // pre_pd
  for (int j = 0x1300; j <= 0x139b; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // pre_top
  for (int j = 0x200; j <= 0x23e; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }
  // vctc
  for (int j = 0x10; j <= 0x3e; j++) {
    read_info[1] = 0;
    read_info[2] = ((j << 2) >> 8) & 0xff;
    read_info[3] = (j << 2) & 0xff;
    USBIO_StreamI2C(0x00, 0x4, &read_info[0], 0x04, &tmp1[2]);
    printf("i = %x, addr = %x data = %lx\n", j, (j << 2), tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
    fprintf(fp_fiti, "0x%08x, 0x%08lx\n", j, tmp1[2]);
  }

  fclose(fp_fiti);
  printf("Export T3e TCON register text OK!\n");
}

/*
void Write_setting()
{
  unsigned long tmp[3];
  int file_data;
  int file_addr;
    FILE *fp_fiti;
    FILE *fp_lut;
    fp_fiti = fopen("./data/case622_lge_ceds_f20_pclrc_20221205_v1_setting.txt","r");
    if (fp_fiti == NULL)
        printf("error while opening file fiti_setting\n");

        for (i=0;;i++) {
     if((fscanf(fp_fiti,"0x%x, 0x%x,\n",&file_addr,&file_data))<0) break;
          test_zw3[1] = 0;
          test_zw3[2] = ((file_addr << 2) >> 8) & 0xff;
          test_zw3[3] = (file_addr << 2) & 0xff;
          test_zw3[4] = file_data&0xff;
          test_zw3[5] = (file_data&0xff00)>>8;
          test_zw3[6] = (file_data&0xff0000)>>16;
          test_zw3[7] = (file_data&0xff000000)>>24;

          printf("i = %x, addr = %x data = %x\n",(i<<2),file_addr, file_data);
          USBIO_StreamI2C(0x00,0x8,&test_zw3[0],0x00,&tmp[2]);
        }


}
*/

void gam_lut_tool() {

  int lut_addr = 0;
  int key_addr = 0;
  int key_data = 0;
  int key_sel = 0;

  char key_info[8] = {0x14, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
  int p1_en[2] = {0x0, 0x0};
  int p2_en[2] = {0x0, 0x0};

  printf("select rgb, 1:b, 2:g, 3:r\n");
  scanf("%x", &key_sel);
  if (key_sel == 1) {
    p1_en[0] = 0x1;
    p1_en[1] = 0x2;
    p2_en[0] = 0x4;
    p2_en[1] = 0x8;
  } else if (key_sel == 2) {
    p1_en[0] = 0x10;
    p1_en[1] = 0x20;
    p2_en[0] = 0x40;
    p2_en[1] = 0x80;
  } else if (key_sel == 3) {
    p1_en[0] = 0x100;
    p1_en[1] = 0x200;
    p2_en[0] = 0x400;
    p2_en[1] = 0x800;
  }

  printf("input 8bit gray scale\n");
  scanf("%d", &key_addr);

  printf("you entered %x \n", key_addr);

  lut_addr = 0x700 + (key_addr >> 1);

  key_info[1] = ((lut_addr << 2) >> 16) & 0xff;
  key_info[2] = ((lut_addr << 2) >> 8) & 0xff;
  key_info[3] = (lut_addr << 2) & 0xff;
  printf("wr addr %x\n", lut_addr);
  printf("input 12bit data\n");
  scanf("%x", &key_data);
  printf("you entered %x\n", key_data);

  key_info[4] = key_data & 0xff;
  key_info[5] = (key_data >> 8) & 0xff;
  key_info[6] = (key_data >> 16) & 0xff;
  key_info[7] = (key_data >> 24) & 0xff;

  // wr ram en and disable
  char end_wr_ram[8] = {0x14, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
  char wr_ram[8] = {0x14, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

  end_wr_ram[1] = ((0x600 << 2) >> 16) & 0xff;
  end_wr_ram[2] = ((0x600 << 2) >> 8) & 0xff;
  end_wr_ram[3] = (0x600 << 2) & 0xff;

  wr_ram[1] = ((0x600 << 2) >> 16) & 0xff;
  wr_ram[2] = ((0x600 << 2) >> 8) & 0xff;
  wr_ram[3] = (0x600 << 2) & 0xff;

  for (int i = 0; i < 2; i = i + 1) {
    if (key_addr % 2 == 0) {
      wr_ram[4] = p1_en[i] & 0xff;
      wr_ram[5] = (p1_en[i] >> 8) & 0xff;
      wr_ram[6] = (p1_en[i] >> 16) & 0xff;
      wr_ram[7] = (p1_en[i] >> 24) & 0xff;
      printf("wr p1 data %x\n", p1_en[i]);
      USBIO_StreamI2C(0x00, 0x8, &wr_ram[0], 0x00, &tmp[2]);
      USBIO_StreamI2C(0x00, 0x8, &key_info[0], 0x00, &tmp[2]);
      USBIO_StreamI2C(0x00, 0x8, &end_wr_ram[0], 0x00, &tmp[2]);
    } else {
      wr_ram[4] = p2_en[i] & 0xff;
      wr_ram[5] = (p2_en[i] >> 8) & 0xff;
      wr_ram[6] = (p2_en[i] >> 16) & 0xff;
      wr_ram[7] = (p2_en[i] >> 24) & 0xff;
      printf("wr p2 data %x\n", p2_en[i]);
      USBIO_StreamI2C(0x00, 0x8, &wr_ram[0], 0x00, &tmp[2]);
      USBIO_StreamI2C(0x00, 0x8, &key_info[0], 0x00, &tmp[2]);
      USBIO_StreamI2C(0x00, 0x8, &end_wr_ram[0], 0x00, &tmp[2]);
    }
  }
  printf("wr done\n");
}

void wr_ddr_func() {

  FILE *fpti;
  char str1[200];
  int data_buf[4];

  int key_addr = 0;
  printf("input read address\n");
  scanf("%x", &key_addr);
  printf("you entered %x \n", key_addr);

  WriteReg(0x9001 << 2, key_addr); // addr

  // fpti = fopen("./data/ddr_data/vdf_lut_dma_60hz.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_60hz_line.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_60hz_adj.txt","r");
  // fpti = fopen("./data/ddr_data/test_ddr.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_0628.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_0709_120lut.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_32_0725.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_0725.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_0712.txt","r");

  // fpti = fopen("./data/ddr_data/vdf_lut_dma_plus8.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_minus8.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_plus32.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_minus32.txt","r");

  // fpti = fopen("./data/ddr_data/vdf_lut_dma_minus128.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_plus128.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_128.txt","r"); //+-128

  // fpti = fopen("./data/ddr_data/vdf_lut_dma_32chg.txt","r"); //+-32
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_64chg.txt","r"); //+-32
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_96chg.txt","r"); //+-32
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_128chg.txt","r"); //+-32
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_112chg.txt","r"); //+-32
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_80chg.txt","r"); //+-32
  // fpti = fopen("./data/ddr_data/vdf_lut_dma_256chg.txt","r"); //+-32

  // linear
  // fpti = fopen("./data/ddr_data/vdf_lut_0825/linear256_vdf_lut.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_0825/linear128_vdf_lut.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_0825/linear112_vdf_lut.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_0825/linear96_vdf_lut.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_0825/linear80_vdf_lut.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_0825/linear32_vdf_lut.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_0825/linear64_vdf_lut.txt","r");
  // fpti = fopen("./data/ddr_data/vdf_lut_0825/linear48_vdf_lut.txt","r");

  // lut dma test

  // fpti = fopen("./data/ud60_lut_dma/aptu_lut_dma.txt","r");
  // fpti = fopen("./data/ud60_lut_dma/vrr_lut_dma.txt","r");
  fpti = fopen("./data/ud60_lut_dma/vrr_lut_dma_only.txt", "r");

  int check_sum = 0;

  if (fpti == NULL)
    printf("error while opening file\n");
  else {
    for (int i = 0;; i++) {

      printf("cnt %d\n", i);

      if ((fscanf(fpti, "%s,", str1)) < 0) {
        printf("cnt break %d\n", i);
        break;
      }

      // fscanf(fpti,"%s",str1);
      printf("%s\n", str1);

      // str2 = str1 + 2;
      sscanf(str1, "%08x%08x%08x%08x", &data_buf[0], &data_buf[1], &data_buf[2], &data_buf[3]);

      check_sum = check_sum + data_buf[0] + data_buf[1] + data_buf[2] + data_buf[3];

      printf("0x%08x,  0x%08x   0x%08x   0x%08x\n ", data_buf[0], data_buf[1], data_buf[2], data_buf[3]);

      // wr wr ddr reg
      WriteReg(0x9000 << 2, 1); // enable wr

      WriteReg(0x9003 << 2, data_buf[0]);
      WriteReg(0x9003 << 2, data_buf[1]);
      WriteReg(0x9003 << 2, data_buf[2]);
      WriteReg(0x9003 << 2, data_buf[3]);
    }
  }
  WriteReg(0x9000 << 2, 0);
  printf("wr_128bits_ddr_data_done\n");
  printf("check_sum = %x\n", check_sum);
  check_sum = 0;
  fclose(fpti);
}

void rd_ddr_func() {
  int key_addr = 0;
  int key_num = 0;

  printf("input read address\n");
  scanf("%x", &key_addr);
  printf("you entered %x \n", key_addr);

  printf("input read num\n");
  scanf("%x", &key_num);
  printf("you entered %x \n", key_num);

  WriteReg(0xa002 << 2, 0xffffffff);
  WriteReg(0xa005 << 2, 0xffffffff);
  WriteReg(0xa001 << 2, key_addr);
  WriteReg(0xa003 << 2, key_num);
  WriteReg(0xa000 << 2, 0x1); // enable rd

  int r_data;

  printf("0         |1         |2         |3         |\r\n");
  for (int cnt = 0; cnt < key_num * 4; cnt++) {
    r_data = ReadReg(0xa004 << 2);
    // rd_chk_sum[cnt%4] = rd_chk_sum[cnt%4] + r_data;
    printf("0x%08x|", r_data);
    if (cnt % 4 == 3) {
      printf("\r\n");
    }
  }

  WriteReg(0xa000 << 2, 0x0);
  printf("read ddr data done\n");
}

void vdf_lut_check() {
  int ram_sel = 0;
  printf("sel func: 1:wr, 2:rd\n");
  scanf("%x", &ram_sel);
  printf("you entered %x \n", ram_sel);

  if (ram_sel == 2) {
    int key_addr = 0;
    printf("input read vdf ram num\n");
    scanf("%x", &key_addr);
    printf("you entered %x \n", key_addr);

    WriteReg(0x60c << 2, key_addr); // addr
    int r_data;
    for (int i = 0; i < 33; i++) {
      r_data = ReadReg((0x700 + i) << 2);
      printf("0x%04x, 0x%06x\n", (0x700 + i), r_data);
    }
    WriteReg(0x60c << 2, 0); // addr
    printf("read done\n");
  }

  else {
    int i, j;
    int lut[33] = {
        0x000000,
        0x080080,
        0x100100,
        0x180180,
        0x200200,
        0x280280,
        0x300300,
        0x380380,
        0x400400,
        0x480480,
        0x500500,
        0x580580,
        0x600600,
        0x680680,
        0x700700,
        0x780780,
        0x800800,
        0x880880,
        0x900900,
        0x980980,
        0xa00a00,
        0xa80a80,
        0xb00b00,
        0xb80b80,
        0xc00c00,
        0xc80c80,
        0xd00d00,
        0xd80d80,
        0xd80d80,
        0xe80e80,
        0xf00f00,
        0xf80f80,
        0xffffff};
    for (i = 0; i < 6; i++) {
      WriteReg(0x60c << 2, i); // addr
      printf("wr num %d\n", i);
      for (j = 0; j < 33; j++)
        WriteReg((0x700 + j) << 2, lut[j]);
    }

    WriteReg(0x60c << 2, 0); // addr
    printf("wr done\n");
  }
}

// USBIO_StreamI2C(0x00,0x8,&key_info[0],0x00,&tmp[2]);
/*int  USBIO_StreamI2C(uint8_t p0, uint8_t p1,char key_info[7],uint8_t p3,unsigned long tmp[3]){*/
bool USBIO_StreamI2C(
    uint64_t iIndex,
    uint64_t iWriteLength,
    const void *iWriteBuffer,
    uint64_t iReadLength,
    void *oReadBuffer) {
  // uint32_t addr,uint32_t data;
  const uint8_t *buf = (const uint8_t *)iWriteBuffer;
  uint32_t addr = buf[3] | (buf[2] << 8) | (buf[1] << 16);
  uint32_t data = buf[4] | (buf[5] << 8) | (buf[6] << 16) | ((uint32_t)buf[7] << 24);

  Wr(addr, data);

  return 1;
}

bool USBIO_WriteI2C( // ??I2C???��????????????
    uint64_t iIndex, // ???mUSBIO_PACKET_LENGTH?��???
    uint8_t iDevice, // ??7��???I2C?��???
    uint8_t iAddr,   // ?????????????
    uint8_t iByte)   // ??��??????????
{
  Wr((uintptr_t)iAddr, (uint32_t)iByte);

  return 1;
}

bool USBIO_ReadI2C(
    uint64_t iIndex,
    uint8_t iDevice,
    uint8_t iAddr,
    uint8_t *oByte) {
  *oByte = (uint8_t)Rd((uintptr_t)iAddr);

  return 1;
}

void WriteReg(uint32_t addr, uint32_t data) {
  Wr(addr, data);
}

uint32_t ReadReg(uint32_t addr) {
  uint32_t rdata;
  rdata = Rd(addr);

  return rdata;
}

void usleep(unsigned int p0) {
}

void edp_tx_cfg_v1() {
  /*Wr(0x000449c8, 0x07800438);
       Wr(0x000449cc, 0x07800438);
       Wr(0x000449d0, 0x07800438);
       Wr(0x000449c4, 0x07800438);
       Wr(0x00044924, 0x07800438);
       Wr(0x000449c0, 0x07800438);
       Wr(0x00045c4c, 0x00000000);
       Wr(0x00049020, 0x00000000);
       Wr(0x00044800, 0x000000ff);
       Wr(0x00045000, 0x0000205e);
       Wr(0x00045008, 0x04ad0000);
       Wr(0x0004500c, 0x06be04ad);
       Wr(0x00045010, 0xff3f1d63);
       Wr(0x00045014, 0x04ad089a);
       Wr(0x00045018, 0x00000000);
       Wr(0x00045028, 0x00000000);
       Wr(0x0004502c, 0x00000000);
       Wr(0x00045030, 0xffc00e00);
       Wr(0x00045034, 0xfffffe00);
       Wr(0x000450ec, 0x00000001);
       Wr(0x00045038, 0x00ba0273);
       Wr(0x0004503c, 0x003f1f99);
       Wr(0x00045040, 0xfea701c0);
       Wr(0x00045044, 0x01c01e69);
       Wr(0x00045048, 0xffffffd7);
       Wr(0x00045058, 0x00400200);
       Wr(0x0004505c, 0x00000200);
       Wr(0x00045060, 0x00000000);
       Wr(0x00045064, 0x00000000);
       Wr(0x00045070, 0x02000200);
       Wr(0x000450f0, 0x00000001);
       Wr(0x00045000, 0x0000205e);
       Wr(0x00045078, 0x00000000);
       Wr(0x0004507c, 0x0000e800);
       Wr(0x0004507c, 0x00010400);
       Wr(0x0004507c, 0x00017000);
       Wr(0x0004507c, 0x0001b000);
       Wr(0x0004507c, 0x0001dc80);
       Wr(0x0004507c, 0x00020440);
       Wr(0x0004507c, 0x00021e40);
       Wr(0x0004507c, 0x00023c40);
       Wr(0x0004507c, 0x00024f00);
       Wr(0x0004507c, 0x000261e0);
       Wr(0x0004507c, 0x000276c0);
       Wr(0x0004507c, 0x00029340);
       Wr(0x0004507c, 0x0002af60);
       Wr(0x0004507c, 0x0002c7e0);
       Wr(0x0004507c, 0x0002da50);
       Wr(0x0004507c, 0x0002ef10);
       Wr(0x0004507c, 0x00030318);
       Wr(0x0004507c, 0x00030fe8);
       Wr(0x0004507c, 0x00032d70);
       Wr(0x0004507c, 0x0003483e);
       Wr(0x0004507c, 0x00035cba);
       Wr(0x0004507c, 0x00037460);
       Wr(0x0004507c, 0x000387b5);
       Wr(0x0004507c, 0x00039707);
       Wr(0x0004507c, 0x0003a846);
       Wr(0x0004507c, 0x0003bb91);
       Wr(0x0004507c, 0x0003c884);
       Wr(0x0004507c, 0x0003d468);
       Wr(0x0004507c, 0x0003efee);
       Wr(0x0004507c, 0x00040878);
       Wr(0x0004507c, 0x00041c0b);
       Wr(0x0004507c, 0x00043308);
       Wr(0x0004507c, 0x000446e7);
       Wr(0x0004507c, 0x00045664);
       Wr(0x0004507c, 0x00046831);
       Wr(0x0004507c, 0x00047c8c);
       Wr(0x0004507c, 0x000489da);
       Wr(0x0004507c, 0x000496fa);
       Wr(0x0004507c, 0x0004a5ca);
       Wr(0x0004507c, 0x0004b673);
       Wr(0x0004507c, 0x0004c491);
       Wr(0x0004507c, 0x0004cf03);
       Wr(0x0004507c, 0x0004daa8);
       Wr(0x0004507c, 0x0004e79d);
       Wr(0x0004507c, 0x0004f5ff);
       Wr(0x0004507c, 0x000502f7);
       Wr(0x0004507c, 0x00050bc5);
       Wr(0x0004507c, 0x0005157d);
       Wr(0x0004507c, 0x00052032);
       Wr(0x0004507c, 0x00052bfa);
       Wr(0x0004507c, 0x000538eb);
       Wr(0x0004507c, 0x0005438e);
       Wr(0x0004507c, 0x00054b55);
       Wr(0x0004507c, 0x000553d7);
       Wr(0x0004507c, 0x00055d24);
       Wr(0x0004507c, 0x0005674c);
       Wr(0x0004507c, 0x00057260);
       Wr(0x0004507c, 0x00057e72);
       Wr(0x0004507c, 0x000585cb);
       Wr(0x0004507c, 0x00058cf2);
       Wr(0x0004507c, 0x000594b7);
       Wr(0x0004507c, 0x00059d28);
       Wr(0x0004507c, 0x0005a652);
       Wr(0x0004507c, 0x0005b042);
       Wr(0x0004507c, 0x0005bb07);
       Wr(0x0004507c, 0x0005c359);
       Wr(0x0004507c, 0x0005c9aa);
       Wr(0x0004507c, 0x0005d080);
       Wr(0x0004507c, 0x0005d7e4);
       Wr(0x0004507c, 0x0005dfe2);
       Wr(0x0004507c, 0x0005e884);
       Wr(0x0004507c, 0x0005f1d7);
       Wr(0x0004507c, 0x0005fbe7);
       Wr(0x0004507c, 0x00060361);
       Wr(0x0004507c, 0x0006093d);
       Wr(0x0004507c, 0x00060f8d);
       Wr(0x0004507c, 0x0006165c);
       Wr(0x0004507c, 0x00061db2);
       Wr(0x0004507c, 0x00062598);
       Wr(0x0004507c, 0x00062e1a);
       Wr(0x0004507c, 0x00063743);
       Wr(0x0004507c, 0x0006408f);
       Wr(0x0004507c, 0x000645dc);
       Wr(0x0004507c, 0x00064b91);
       Wr(0x0004507c, 0x000651b4);
       Wr(0x0004507c, 0x0006584c);
       Wr(0x0004507c, 0x00065f64);
       Wr(0x0004507c, 0x00066704);
       Wr(0x0004507c, 0x00066f35);
       Wr(0x0004507c, 0x00067803);
       Wr(0x0004507c, 0x000680bc);
       Wr(0x0004507c, 0x000685d0);
       Wr(0x0004507c, 0x00068b45);
       Wr(0x0004507c, 0x00069122);
       Wr(0x0004507c, 0x0006976c);
       Wr(0x0004507c, 0x00069e2e);
       Wr(0x0004507c, 0x0006a56f);
       Wr(0x0004507c, 0x0006ad39);
       Wr(0x0004507c, 0x0006b595);
       Wr(0x0004507c, 0x0006be8f);
       Wr(0x0004507c, 0x0006c418);
       Wr(0x0004507c, 0x0006c944);
       Wr(0x0004507c, 0x0006ced0);
       Wr(0x0004507c, 0x0006d4c5);
       Wr(0x0004507c, 0x0006db29);
       Wr(0x0004507c, 0x0006e206);
       Wr(0x0004507c, 0x0006e962);
       Wr(0x0004507c, 0x0006f14a);
       Wr(0x0004507c, 0x0006f9c5);
       Wr(0x0004507c, 0x00070170);
       Wr(0x0004507c, 0x00070652);
       Wr(0x0004507c, 0x00070b91);
       Wr(0x0004507c, 0x00071132);
       Wr(0x0004507c, 0x0007173d);
       Wr(0x0004507c, 0x00071dbb);
       Wr(0x0004507c, 0x000724b2);
       Wr(0x0004507c, 0x00072c2d);
       Wr(0x0004507c, 0x00073436);
       Wr(0x0004507c, 0x00073cd6);
       Wr(0x0004507c, 0x0007430d);
       Wr(0x0004507c, 0x00074807);
       Wr(0x0004507c, 0x00074d60);
       Wr(0x0004507c, 0x0007531f);
       Wr(0x0004507c, 0x0007594b);
       Wr(0x0004507c, 0x00075fed);
       Wr(0x0004507c, 0x0007670f);
       Wr(0x0004507c, 0x00076eba);
       Wr(0x0004507c, 0x000776f8);
       Wr(0x0004507c, 0x00077fd5);
       Wr(0x0004507c, 0x000784af);
       Wr(0x0004507c, 0x000789d0);
       Wr(0x0004507c, 0x00078f54);
       Wr(0x0004507c, 0x00079545);
       Wr(0x0004507c, 0x00079ba9);
       Wr(0x0004507c, 0x0007a28b);
       Wr(0x0004507c, 0x0007a9f5);
       Wr(0x0004507c, 0x0007b1f0);
       Wr(0x0004507c, 0x0007ba8a);
       Wr(0x0004507c, 0x0007c1e7);
       Wr(0x0004507c, 0x0007c6e6);
       Wr(0x0004507c, 0x0007cc49);
       Wr(0x0004507c, 0x0007d218);
       Wr(0x0004507c, 0x0007d85c);
       Wr(0x0004507c, 0x0007df1e);
       Wr(0x0004507c, 0x0007e66b);
       Wr(0x0004507c, 0x0007ee4c);
       Wr(0x0004507c, 0x0007f6ce);
       Wr(0x0004507c, 0x0007ffff);
       Wr(0x00045080, 0x00000000);
       Wr(0x00045084, 0x00000000);
       Wr(0x00045084, 0x00000000);
       Wr(0x00045084, 0x00000000);
       Wr(0x00045084, 0x00010001);
       Wr(0x00045084, 0x00020001);
       Wr(0x00045084, 0x00020002);
       Wr(0x00045084, 0x00020002);
       Wr(0x00045084, 0x00030003);
       Wr(0x00045084, 0x00030003);
       Wr(0x00045084, 0x00040004);
       Wr(0x00045084, 0x00050004);
       Wr(0x00045084, 0x00060005);
       Wr(0x00045084, 0x00070006);
       Wr(0x00045084, 0x00080007);
       Wr(0x00045084, 0x000a0009);
       Wr(0x00045084, 0x000b000a);
       Wr(0x00045084, 0x000d000c);
       Wr(0x00045084, 0x0010000e);
       Wr(0x00045084, 0x00120011);
       Wr(0x00045084, 0x00150014);
       Wr(0x00045084, 0x00190017);
       Wr(0x00045084, 0x001e001c);
       Wr(0x00045084, 0x00210020);
       Wr(0x00045084, 0x00240023);
       Wr(0x00045084, 0x00270026);
       Wr(0x00045084, 0x002a0029);
       Wr(0x00045084, 0x002e002b);
       Wr(0x00045084, 0x00320030);
       Wr(0x00045084, 0x00360034);
       Wr(0x00045084, 0x003a0038);
       Wr(0x00045084, 0x003f003c);
       Wr(0x00045084, 0x00450042);
       Wr(0x00045084, 0x004a0048);
       Wr(0x00045084, 0x004f004d);
       Wr(0x00045084, 0x00560052);
       Wr(0x00045084, 0x005f005b);
       Wr(0x00045084, 0x00660063);
       Wr(0x00045084, 0x006d006a);
       Wr(0x00045084, 0x00770070);
       Wr(0x00045084, 0x0082007c);
       Wr(0x00045084, 0x008c0087);
       Wr(0x00045084, 0x00960091);
       Wr(0x00045084, 0x00a3009a);
       Wr(0x00045084, 0x00b200ab);
       Wr(0x00045084, 0x00c000b9);
       Wr(0x00045084, 0x00cd00c7);
       Wr(0x00045084, 0x00df00d3);
       Wr(0x00045084, 0x00f400ea);
       Wr(0x00045084, 0x010800fe);
       Wr(0x00045084, 0x01190111);
       Wr(0x00045084, 0x01320122);
       Wr(0x00045084, 0x014f0141);
       Wr(0x00045084, 0x016a015d);
       Wr(0x00045084, 0x01820176);
       Wr(0x00045084, 0x01a3018d);
       Wr(0x00045084, 0x01cb01b8);
       Wr(0x00045084, 0x01f001de);
       Wr(0x00045084, 0x02110201);
       Wr(0x00045084, 0x023f0221);
       Wr(0x00045084, 0x0276025b);
       Wr(0x00045084, 0x02a7028f);
       Wr(0x00045084, 0x02d502bf);
       Wr(0x00045084, 0x031402eb);
       Wr(0x00045084, 0x035f033b);
       Wr(0x00045084, 0x03a30382);
       Wr(0x00045084, 0x03e203c3);
       Wr(0x00045084, 0x03ff03ff);
       Wr(0x00045084, 0x03ff03ff);
       Wr(0x00045084, 0x03ff03ff);
       Wr(0x00045084, 0x03ff03ff);
       Wr(0x00045084, 0x03ff03ff);
       Wr(0x00045084, 0x03ff03ff);
       Wr(0x00045084, 0x03ff03ff);
       Wr(0x00045084, 0x03ff03ff);
       Wr(0x00045084, 0x00000fff);
       Wr(0x00045098, 0x00000000);
       Wr(0x0004509c, 0x0fff0400);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0fff0fff);
       Wr(0x0004509c, 0x0ffe0fff);
       Wr(0x0004509c, 0x0ffe0ffe);
       Wr(0x0004509c, 0x0ffe0ffe);
       Wr(0x0004509c, 0x0ffd0ffd);
       Wr(0x0004509c, 0x0ffd0ffd);
       Wr(0x0004509c, 0x0ffc0ffc);
       Wr(0x0004509c, 0x0ffb0ffc);
       Wr(0x0004509c, 0x0ffa0ffb);
       Wr(0x0004509c, 0x0ff90ffa);
       Wr(0x0004509c, 0x0ff80ff9);
       Wr(0x0004509c, 0x0ff60ff7);
       Wr(0x0004509c, 0x0ff50ff6);
       Wr(0x0004509c, 0x0ff30ff4);
       Wr(0x0004509c, 0x0ff00ff1);
       Wr(0x0004509c, 0x0fed0fee);
       Wr(0x0004509c, 0x0fea0feb);
       Wr(0x0004509c, 0x0fe50fe8);
       Wr(0x0004509c, 0x0fdf0fe2);
       Wr(0x0004509c, 0x0fd90fdc);
       Wr(0x0004509c, 0x0fd30fd6);
       Wr(0x0004509c, 0x0fca0fd0);
       Wr(0x0004509c, 0x0fbe0fc4);
       Wr(0x0004509c, 0x0fb20fb8);
       Wr(0x0004509c, 0x0fa60fac);
       Wr(0x0004509c, 0x0f940fa0);
       Wr(0x0004509c, 0x0f7c0f88);
       Wr(0x0004509c, 0x0f640f70);
       Wr(0x0004509c, 0x0f4c0f58);
       Wr(0x0004509c, 0x0f280f40);
       Wr(0x0004509c, 0x0ef80f10);
       Wr(0x0004509c, 0x0ec80ee0);
       Wr(0x0004509c, 0x0e990eb1);
       Wr(0x0004509c, 0x0e510e81);
       Wr(0x0004509c, 0x0df20e21);
       Wr(0x0004509c, 0x0d930dc3);
       Wr(0x0004509c, 0x0d350d64);
       Wr(0x0004509c, 0x0ca90d06);
       Wr(0x0004509c, 0x0bf00c4c);
       Wr(0x0004509c, 0x0b3a0b94);
       Wr(0x0004509c, 0x0a880ae0);
       Wr(0x0004509c, 0x09840a30);
       Wr(0x0004509c, 0x083d08de);
       Wr(0x0004509c, 0x070e07a2);
       Wr(0x0004509c, 0x05fc0681);
       Wr(0x0004509c, 0x050d0580);
       Wr(0x0004509c, 0x044304a3);
       Wr(0x0004509c, 0x03a403ee);
       Wr(0x0004509c, 0x03350366);
       Wr(0x0004509c, 0x02f90310);
       Wr(0x0004509c, 0x02f502f0);
       Wr(0x0004509c, 0x032e030a);
       Wr(0x0004509c, 0x03a90363);
       Wr(0x0004509c, 0x00000400);
       Wr(0x00045088, 0x00000000);
       Wr(0x0004508c, 0x00000000);
       Wr(0x0004508c, 0x00000000);
       Wr(0x0004508c, 0x00000000);
       Wr(0x0004508c, 0x00000000);
       Wr(0x0004508c, 0x00000000);
       Wr(0x0004508c, 0x00000000);
       Wr(0x0004508c, 0x03ff0000);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x03ff03ff);
       Wr(0x0004508c, 0x000003ff);
       Wr(0x00045090, 0x09480398);
       Wr(0x00045094, 0x840000d0);
       Wr(0x000450a0, 0x00511805);
       Wr(0x000450a4, 0x04000400);
       Wr(0x000450a8, 0xa07c0400);
       Wr(0x000450ac, 0x00000000);
       Wr(0x000450b0, 0x00000000);
       Wr(0x000450b4, 0x00000000);
       Wr(0x000450b8, 0x04000400);
       Wr(0x000450bc, 0x00000400);
       Wr(0x000450c0, 0x00000008);
       Wr(0x000450c4, 0xff6a01a9);
       Wr(0x000450c8, 0xffe1ffee);
       Wr(0x000450cc, 0xfffe0122);
       Wr(0x000450d0, 0xffe7fffb);
       Wr(0x000450d4, 0x0000011e);
       Wr(0x00045100, 0x814f8c00);
       Wr(0x00045104, 0x014f8c00);
       Wr(0x00045108, 0x014f8c00);
       Wr(0x0004510c, 0x07c84900);
       Wr(0x00045110, 0x07378500);
       Wr(0x00045114, 0x07204b00);
       Wr(0x00045118, 0x02522c00);
       Wr(0x0004511c, 0x07378500);
       Wr(0x00045120, 0x07204b00);
       Wr(0x00045000, 0x0000205e);
       Wr(0x00045000, 0x0000205e);
       Wr(0x00044000, 0x00000003);
       Wr(0x00044384, 0x07800438);
       Wr(0x00044004, 0x077f0000);
       Wr(0x00044008, 0x04370000);
       Wr(0x000440ac, 0x00000080);
       Wr(0x000440ac, 0x00000040);
       Wr(0x00044010, 0x00000000);
       Wr(0x000440b4, 0x00004080);
       Wr(0x00044388, 0x00000001);
       Wr(0x00044380, 0x00000000);
       Wr(0x0004439c, 0x07800438);
       Wr(0x0004438c, 0x00000014);
       Wr(0x00044140, 0x00000000);
       Wr(0x00044178, 0x0fc00e00);
       Wr(0x00044180, 0x04a80000);
       Wr(0x00044184, 0x066204a8);
       Wr(0x00044188, 0x1e6f1cbf);
       Wr(0x0004418c, 0x04a80811);
       Wr(0x00044190, 0x00000000);
       Wr(0x000441a0, 0x00000000);
       Wr(0x00044148, 0x009f0000);
       Wr(0x0004414c, 0x01df013f);
       Wr(0x00044150, 0x031f027f);
       Wr(0x00044154, 0x045f03bf);
       Wr(0x00044158, 0x059f04ff);
       Wr(0x0004415c, 0x06df063f);
       Wr(0x00044160, 0x0000077f);
       Wr(0x00044164, 0x00860000);
       Wr(0x00044168, 0x0194010d);
       Wr(0x0004416c, 0x02a2021b);
       Wr(0x00044170, 0x03b00329);
       Wr(0x00044174, 0x00000437);
       Wr(0x00044144, 0x0000000c);
       Wr(0x00044284, 0x00000c08);
       Wr(0x00044308, 0x0000007e);
       Wr(0x0004430c, 0x00000a8c);
       Wr(0x00044280, 0x00001111);
       Wr(0x00044280, 0x80001111);
       Wr(0x000443a0, 0x00000001);
       Wr(0x000443a4, 0x00000003);
       Wr(0x000443a8, 0x07800438);
       Wr(0x00044340, 0x00000040);
       Wr(0x00044344, 0x007f0000);
       Wr(0x00044348, 0x007f0000);
       Wr(0x00048008, 0x00000003);
       Wr(0x00048008, 0x00000013);
       Wr(0x00048218, 0x00000001);
       Wr(0x00048000, 0x00000104);
       Wr(0x000481d4, 0x00000c08);
       Wr(0x000481a4, 0x00a00000);
       Wr(0x000481a8, 0x01e00140);
       Wr(0x000481ac, 0x03200280);
       Wr(0x000481b0, 0x046003c0);
       Wr(0x000481b4, 0x05a00500);
       Wr(0x000481b8, 0x06e00640);
       Wr(0x000481bc, 0x00000780);
       Wr(0x000481c0, 0x00870000);
       Wr(0x000481c4, 0x0195010e);
       Wr(0x000481c8, 0x02a3021c);
       Wr(0x000481cc, 0x03b1032a);
       Wr(0x000481d0, 0x00000438);
       Wr(0x00048244, 0x00000001);
       Wr(0x00048248, 0x00000000);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x0004824c, 0x20000000);
       Wr(0x0004824c, 0x200fffff);
       Wr(0x00048244, 0x00000000);
       Wr(0x00048244, 0x00000001);
       Wr(0x00048248, 0x80000000);
       Wr(0x00048244, 0x00000000);
       Wr(0x00045a80, 0x00000208);
       Wr(0x00045a84, 0x0000001d);
       Wr(0x00045a80, 0x00000100);
       Wr(0x00045a84, 0x00140800);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x00000108);
       Wr(0x00045a84, 0x00140800);
       Wr(0x00045a84, 0x0000000a);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x00000110);
       Wr(0x00045a84, 0x00140800);
       Wr(0x00045a84, 0x00000012);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x00000118);
       Wr(0x00045a84, 0x00140800);
       Wr(0x00045a84, 0x0000000a);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x00000120);
       Wr(0x00045a84, 0x00140881);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x00000128);
       Wr(0x00045a84, 0x00140800);
       Wr(0x00045a84, 0x000000f6);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x00000130);
       Wr(0x00045a84, 0x00140800);
       Wr(0x00045a84, 0x000000ee);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x00000138);
       Wr(0x00045a84, 0x00140800);
       Wr(0x00045a84, 0x000000f8);
       Wr(0x00045a84, 0xfdfc0000);
       Wr(0x00045a84, 0x0000fffe);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x00000140);
       Wr(0x00045a84, 0x0000007f);
       Wr(0x00045a84, 0xfe000000);
       Wr(0x00045a84, 0xfaf8fffc);
       Wr(0x00045a84, 0x1400fffc);
       Wr(0x00045a84, 0x08080a14);
       Wr(0x00045a80, 0x00000148);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0xf8000000);
       Wr(0x00045a84, 0xf4f4f8f8);
       Wr(0x00045a84, 0x1400fff6);
       Wr(0x00045a84, 0x08080a14);
       Wr(0x00045a80, 0x00000150);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0xf6000000);
       Wr(0x00045a84, 0xf4f0fcf6);
       Wr(0x00045a84, 0x140000f6);
       Wr(0x00045a84, 0x08080a14);
       Wr(0x00045a80, 0x00000158);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0xf6000000);
       Wr(0x00045a84, 0xf8f6fef6);
       Wr(0x00045a84, 0x140000fa);
       Wr(0x00045a84, 0x08080a14);
       Wr(0x00045a80, 0x00000160);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0xf8000000);
       Wr(0x00045a84, 0xfcfcfef8);
       Wr(0x00045a84, 0x140000fc);
       Wr(0x00045a84, 0x08080a14);
       Wr(0x00045a80, 0x00000168);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0xfa000000);
       Wr(0x00045a84, 0x0202fffa);
       Wr(0x00045a84, 0x14000001);
       Wr(0x00045a84, 0x08080a14);
       Wr(0x00045a80, 0x00000170);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x02000000);
       Wr(0x00045a84, 0x00040002);
       Wr(0x00045a84, 0x14000000);
       Wr(0x00045a84, 0x08080a14);
       Wr(0x00045a80, 0x00000178);
       Wr(0x00045a84, 0xff0a0800);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x14000000);
       Wr(0x00045a84, 0x08080a14);
       Wr(0x00045a80, 0x00000180);
       Wr(0x00045a84, 0xff141206);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x12000000);
       Wr(0x00045a84, 0x04060a12);
       Wr(0x00045a80, 0x00000188);
       Wr(0x00045a84, 0x041e120a);
       Wr(0x00045a84, 0x0000000a);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x0a000000);
       Wr(0x00045a84, 0x0004060a);
       Wr(0x00045a80, 0x00000190);
       Wr(0x00045a84, 0x0428120a);
       Wr(0x00045a84, 0x00000014);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x08000000);
       Wr(0x00045a84, 0x00000408);
       Wr(0x00045a80, 0x00000198);
       Wr(0x00045a84, 0x0428120e);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x000001a0);
       Wr(0x00045a84, 0x04281210);
       Wr(0x00045a84, 0x000000ec);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x000001a8);
       Wr(0x00045a84, 0x1824120e);
       Wr(0x00045a84, 0x000000f6);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x000001b0);
       Wr(0x00045a84, 0x1414120c);
       Wr(0x00045a84, 0x000000f8);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x000001b8);
       Wr(0x00045a84, 0x1414120a);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x000001c0);
       Wr(0x00045a84, 0x0a0a0e08);
       Wr(0x00045a84, 0x0a000000);
       Wr(0x00045a84, 0x00001414);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x000001c8);
       Wr(0x00045a84, 0x0a140804);
       Wr(0x00045a84, 0x0a000000);
       Wr(0x00045a84, 0x00001414);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x000001d0);
       Wr(0x00045a84, 0x0a140800);
       Wr(0x00045a84, 0x0a000000);
       Wr(0x00045a84, 0x00000a0a);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x000001d8);
       Wr(0x00045a84, 0x0a140800);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x000001e0);
       Wr(0x00045a84, 0x0a140800);
       Wr(0x00045a84, 0xf6000000);
       Wr(0x00045a84, 0x0000f6f6);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x000001e8);
       Wr(0x00045a84, 0x0a140800);
       Wr(0x00045a84, 0xee000000);
       Wr(0x00045a84, 0x0000eeee);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x000001f0);
       Wr(0x00045a84, 0x0a140800);
       Wr(0x00045a84, 0xf6000000);
       Wr(0x00045a84, 0x0000f6f6);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x000001f8);
       Wr(0x00045a84, 0x0a020800);
       Wr(0x00045a84, 0xfc000000);
       Wr(0x00045a84, 0x0000fcfc);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a84, 0x00000000);
       Wr(0x00045a80, 0x00000200);
       Wr(0x00045a84, 0x1c14120f);
       Wr(0x00045a80, 0x00000201);
       Wr(0x00045a84, 0x121c1e1e);
       Wr(0x00045a80, 0x00000202);
       Wr(0x00045a84, 0x0000000f);
       Wr(0x00045a80, 0x0000021c);
       Wr(0x00045a84, 0x03ff0000);
       Wr(0x00045a80, 0x0000021d);
       Wr(0x00045a84, 0x03ff0000);
       Wr(0x00045a80, 0x0000021e);
       Wr(0x00045a84, 0x03ff0000);
       Wr(0x00045a80, 0x00000205);
       Wr(0x00045a84, 0x05a00a00);
       Wr(0x00045a80, 0x0000021f);
       Wr(0x00045a84, 0x00040400);
       Wr(0x00045c18, 0x077f0000);
       Wr(0x00045c1c, 0x04370000);
       Wr(0x00045c20, 0x077f0000);
       Wr(0x00045c24, 0x04370000);
       Wr(0x00045c28, 0x077f0000);
       Wr(0x00045c2c, 0x04370000);
       Wr(0x00045c30, 0x077f0000);
       Wr(0x00045c34, 0x04370000);
       Wr(0x00045c04, 0x00000001);
       Wr(0x00045c08, 0x00000000);
       Wr(0x00045c0c, 0x00000003);
       Wr(0x00045c10, 0x00000002);
       Wr(0x00045c14, 0x00000001);
       Wr(0x00045908, 0x00000080);
       Wr(0x0004590c, 0x01000000);
       Wr(0x00045910, 0x00000100);
       Wr(0x00045914, 0x80040000);
       Wr(0x00045918, 0x0000080d);
       Wr(0x0004591c, 0x891e0240);
       Wr(0x00045920, 0x000007fa);
       Wr(0x00045900, 0x00000002);
       Wr(0x00045900, 0x00000003);
       Wr(0x00045940, 0x01000000);
       Wr(0x00045944, 0x018a0100);
       Wr(0x00045948, 0x1fd11f8a);
       Wr(0x0004594c, 0x010001d1);
       Wr(0x00045950, 0x00000000);
       Wr(0x00045954, 0x00000600);
       Wr(0x00045958, 0x00000600);
       Wr(0x0004595c, 0x00000000);
       Wr(0x00045960, 0x00000000);
       Wr(0x00045924, 0x04000103);
       Wr(0x0004892c, 0x00000001);
       Wr(0x00048930, 0x00000000);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00800000);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00800080);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00800100);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00800180);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00800200);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00800280);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00800300);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00800380);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x008003ff);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x01000000);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x01000080);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x01000100);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x01000180);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x01000200);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x01000280);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x01000300);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x01000380);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x010003ff);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x01800000);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x01800080);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x01800100);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x01800180);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x01800200);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x01800280);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x01800300);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x01800380);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x018003ff);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x02000000);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x02000080);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x02000100);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x02000180);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x02000200);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x02000280);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x02000300);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x02000380);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x020003ff);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x02800000);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x02800080);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x02800100);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x02800180);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x02800200);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x02800280);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x02800300);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x02800380);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x028003ff);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03000000);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03000080);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03000100);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03000180);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03000200);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03000280);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03000300);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03000380);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x030003ff);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03800000);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03800080);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03800100);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03800180);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03800200);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03800280);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03800300);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03800380);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x038003ff);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03ff0000);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03ff0080);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03ff0100);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03ff0180);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03ff0200);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03ff0280);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03ff0300);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03ff0380);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x03ff03ff);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00800000);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00800080);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00800100);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00800180);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00800200);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00800280);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00800300);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00800380);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x008003ff);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x01000000);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x01000080);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x01000100);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x01000180);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x01000200);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x01000280);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x01000300);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x01000380);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x010003ff);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x01800000);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x01800080);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x01800100);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x01800180);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x01800200);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x01800280);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x01800300);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x01800380);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x018003ff);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x02000000);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x02000080);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x02000100);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x02000180);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x02000200);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x02000280);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x02000300);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x02000380);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x020003ff);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x02800000);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x02800080);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x02800100);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x02800180);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x02800200);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x02800280);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x02800300);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x02800380);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x028003ff);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03000000);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03000080);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03000100);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03000180);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03000200);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03000280);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03000300);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03000380);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x030003ff);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03800000);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03800080);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03800100);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03800180);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03800200);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03800280);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03800300);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03800380);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x038003ff);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03ff0000);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03ff0080);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03ff0100);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03ff0180);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03ff0200);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03ff0280);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03ff0300);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03ff0380);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x03ff03ff);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00800000);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00800080);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00800100);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00800180);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00800200);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00800280);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00800300);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00800380);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x008003ff);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x01000000);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x01000080);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x01000100);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x01000180);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x01000200);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x01000280);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x01000300);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x01000380);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x010003ff);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x01800000);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x01800080);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x01800100);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x01800180);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x01800200);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x01800280);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x01800300);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x01800380);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x018003ff);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x02000000);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x02000080);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x02000100);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x02000180);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x02000200);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x02000280);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x02000300);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x02000380);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x020003ff);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x02800000);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x02800080);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x02800100);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x02800180);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x02800200);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x02800280);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x02800300);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x02800380);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x028003ff);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03000000);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03000080);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03000100);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03000180);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03000200);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03000280);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03000300);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03000380);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x030003ff);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03800000);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03800080);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03800100);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03800180);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03800200);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03800280);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03800300);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03800380);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x038003ff);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03ff0000);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03ff0080);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03ff0100);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03ff0180);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03ff0200);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03ff0280);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03ff0300);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03ff0380);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x03ff03ff);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00800000);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00800080);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00800100);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00800180);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00800200);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00800280);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00800300);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00800380);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x008003ff);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x01000000);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x01000080);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x01000100);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x01000180);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x01000200);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x01000280);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x01000300);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x01000380);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x010003ff);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x01800000);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x01800080);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x01800100);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x01800180);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x01800200);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x01800280);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x01800300);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x01800380);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x018003ff);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x02000000);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x02000080);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x02000100);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x02000180);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x02000200);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x02000280);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x02000300);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x02000380);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x020003ff);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x02800000);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x02800080);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x02800100);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x02800180);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x02800200);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x02800280);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x02800300);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x02800380);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x028003ff);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03000000);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03000080);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03000100);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03000180);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03000200);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03000280);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03000300);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03000380);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x030003ff);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03800000);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03800080);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03800100);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03800180);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03800200);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03800280);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03800300);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03800380);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x038003ff);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03ff0000);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03ff0080);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03ff0100);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03ff0180);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03ff0200);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03ff0280);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03ff0300);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03ff0380);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x03ff03ff);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00800000);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00800080);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00800100);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00800180);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00800200);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00800280);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00800300);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00800380);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x008003ff);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x01000000);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x01000080);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x01000100);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x01000180);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x01000200);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x01000280);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x01000300);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x01000380);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x010003ff);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x01800000);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x01800080);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x01800100);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x01800180);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x01800200);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x01800280);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x01800300);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x01800380);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x018003ff);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x02000000);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x02000080);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x02000100);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x02000180);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x02000200);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x02000280);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x02000300);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x02000380);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x020003ff);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x02800000);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x02800080);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x02800100);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x02800180);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x02800200);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x02800280);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x02800300);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x02800380);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x028003ff);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03000000);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03000080);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03000100);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03000180);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03000200);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03000280);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03000300);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03000380);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x030003ff);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03800000);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03800080);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03800100);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03800180);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03800200);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03800280);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03800300);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03800380);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x038003ff);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03ff0000);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03ff0080);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03ff0100);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03ff0180);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03ff0200);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03ff0280);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03ff0300);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03ff0380);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x03ff03ff);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00800000);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00800080);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00800100);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00800180);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00800200);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00800280);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00800300);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00800380);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x008003ff);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x01000000);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x01000080);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x01000100);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x01000180);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x01000200);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x01000280);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x01000300);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x01000380);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x010003ff);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x01800000);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x01800080);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x01800100);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x01800180);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x01800200);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x01800280);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x01800300);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x01800380);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x018003ff);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x02000000);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x02000080);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x02000100);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x02000180);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x02000200);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x02000280);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x02000300);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x02000380);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x020003ff);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x02800000);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x02800080);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x02800100);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x02800180);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x02800200);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x02800280);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x02800300);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x02800380);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x028003ff);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03000000);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03000080);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03000100);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03000180);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03000200);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03000280);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03000300);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03000380);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x030003ff);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03800000);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03800080);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03800100);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03800180);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03800200);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03800280);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03800300);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03800380);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x038003ff);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03ff0000);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03ff0080);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03ff0100);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03ff0180);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03ff0200);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03ff0280);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03ff0300);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03ff0380);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x03ff03ff);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00800000);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00800080);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00800100);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00800180);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00800200);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00800280);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00800300);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00800380);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x008003ff);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x01000000);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x01000080);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x01000100);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x01000180);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x01000200);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x01000280);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x01000300);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x01000380);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x010003ff);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x01800000);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x01800080);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x01800100);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x01800180);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x01800200);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x01800280);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x01800300);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x01800380);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x018003ff);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x02000000);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x02000080);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x02000100);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x02000180);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x02000200);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x02000280);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x02000300);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x02000380);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x020003ff);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x02800000);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x02800080);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x02800100);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x02800180);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x02800200);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x02800280);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x02800300);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x02800380);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x028003ff);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03000000);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03000080);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03000100);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03000180);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03000200);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03000280);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03000300);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03000380);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x030003ff);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03800000);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03800080);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03800100);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03800180);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03800200);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03800280);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03800300);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03800380);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x038003ff);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03ff0000);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03ff0080);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03ff0100);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03ff0180);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03ff0200);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03ff0280);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03ff0300);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03ff0380);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x03ff03ff);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00800000);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00800080);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00800100);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00800180);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00800200);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00800280);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00800300);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00800380);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x008003ff);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x01000000);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x01000080);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x01000100);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x01000180);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x01000200);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x01000280);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x01000300);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x01000380);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x010003ff);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x01800000);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x01800080);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x01800100);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x01800180);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x01800200);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x01800280);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x01800300);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x01800380);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x018003ff);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x02000000);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x02000080);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x02000100);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x02000180);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x02000200);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x02000280);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x02000300);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x02000380);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x020003ff);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x02800000);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x02800080);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x02800100);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x02800180);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x02800200);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x02800280);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x02800300);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x02800380);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x028003ff);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03000000);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03000080);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03000100);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03000180);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03000200);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03000280);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03000300);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03000380);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x030003ff);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03800000);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03800080);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03800100);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03800180);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03800200);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03800280);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03800300);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03800380);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x038003ff);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03ff0000);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03ff0080);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03ff0100);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03ff0180);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03ff0200);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03ff0280);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03ff0300);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03ff0380);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x03ff03ff);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x00000000);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00000080);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00000100);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00000180);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00000200);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00000280);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00000300);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00000380);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00800000);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00800080);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00800100);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00800180);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00800200);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00800280);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00800300);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x00800380);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x008003ff);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x01000000);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x01000080);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x01000100);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x01000180);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x01000200);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x01000280);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x01000300);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x01000380);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x010003ff);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x01800000);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x01800080);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x01800100);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x01800180);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x01800200);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x01800280);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x01800300);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x01800380);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x018003ff);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x02000000);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x02000080);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x02000100);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x02000180);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x02000200);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x02000280);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x02000300);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x02000380);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x020003ff);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x02800000);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x02800080);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x02800100);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x02800180);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x02800200);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x02800280);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x02800300);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x02800380);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x028003ff);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03000000);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03000080);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03000100);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03000180);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03000200);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03000280);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03000300);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03000380);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x030003ff);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03800000);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03800080);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03800100);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03800180);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03800200);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03800280);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03800300);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03800380);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x038003ff);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03ff0000);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03ff0080);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03ff0100);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03ff0180);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03ff0200);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03ff0280);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03ff0300);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03ff0380);
       Wr(0x00048934, 0x000003ff);
       Wr(0x00048934, 0x03ff03ff);
       Wr(0x00048934, 0x000003ff);
       Wr(0x0004892c, 0x00000000);
       Wr(0x00048928, 0x00000070);
       Wr(0x00048928, 0x00000070);
       Wr(0x00048928, 0x00000071);
       Wr(0x00048880, 0x00000006);
       Wr(0x00048884, 0x00e60252);
       Wr(0x00048888, 0x00341f83);
       Wr(0x0004888c, 0x1ebd01c0);
       Wr(0x00048890, 0x01c01e64);
       Wr(0x00048894, 0x00021fdc);
       Wr(0x00048898, 0x00000000);
       Wr(0x0004889c, 0x00000000);
       Wr(0x000488a0, 0x00000000);
       Wr(0x00045a00, 0x04000400);
       Wr(0x00045a04, 0x04000000);
       Wr(0x00045a08, 0x00000000);
       Wr(0x00045a0c, 0x00000000);
       Wr(0x00045a10, 0x00000000);
       Wr(0x00045a00, 0x84000400);
       Wr(0x00049204, 0x00000200);
       Wr(0x00049208, 0x00000000);
       Wr(0x00049208, 0x05214852);
       Wr(0x00049208, 0x0711c471);
       Wr(0x00049208, 0x08822088);
       Wr(0x00049208, 0x09b26c9b);
       Wr(0x00049208, 0x0ab2acab);
       Wr(0x00049208, 0x0ba2e8ba);
       Wr(0x00049208, 0x0c731cc7);
       Wr(0x00049208, 0x0d4350d4);
       Wr(0x00049208, 0x0e0380e0);
       Wr(0x00049208, 0x0eb3aceb);
       Wr(0x00049208, 0x0f53d4f5);
       Wr(0x00049208, 0x0ff3fcff);
       Wr(0x00049208, 0x10842108);
       Wr(0x00049208, 0x11144511);
       Wr(0x00049208, 0x11a4691a);
       Wr(0x00049208, 0x12248922);
       Wr(0x00049208, 0x12a4a92a);
       Wr(0x00049208, 0x1324c932);
       Wr(0x00049208, 0x13a4e93a);
       Wr(0x00049208, 0x14150541);
       Wr(0x00049208, 0x14952549);
       Wr(0x00049208, 0x15054150);
       Wr(0x00049208, 0x15655956);
       Wr(0x00049208, 0x15d5755d);
       Wr(0x00049208, 0x16459164);
       Wr(0x00049208, 0x16a5a96a);
       Wr(0x00049208, 0x1705c170);
       Wr(0x00049208, 0x1765d976);
       Wr(0x00049208, 0x17d5f57d);
       Wr(0x00049208, 0x18260982);
       Wr(0x00049208, 0x18862188);
       Wr(0x00049208, 0x18e6398e);
       Wr(0x00049208, 0x19465194);
       Wr(0x00049208, 0x19966599);
       Wr(0x00049208, 0x19e6799e);
       Wr(0x00049208, 0x1a4691a4);
       Wr(0x00049208, 0x1a96a5a9);
       Wr(0x00049208, 0x1ae6b9ae);
       Wr(0x00049208, 0x1b36cdb3);
       Wr(0x00049208, 0x1b86e1b8);
       Wr(0x00049208, 0x1bd6f5bd);
       Wr(0x00049208, 0x1c2709c2);
       Wr(0x00049208, 0x1c771dc7);
       Wr(0x00049208, 0x1cc731cc);
       Wr(0x00049208, 0x1d1745d1);
       Wr(0x00049208, 0x1d5755d5);
       Wr(0x00049208, 0x1da769da);
       Wr(0x00049208, 0x1de779de);
       Wr(0x00049208, 0x1e378de3);
       Wr(0x00049208, 0x1e779de7);
       Wr(0x00049208, 0x1ec7b1ec);
       Wr(0x00049208, 0x1f07c1f0);
       Wr(0x00049208, 0x1f57d5f5);
       Wr(0x00049208, 0x1f97e5f9);
       Wr(0x00049208, 0x1fd7f5fd);
       Wr(0x00049208, 0x20180601);
       Wr(0x00049208, 0x20581605);
       Wr(0x00049208, 0x20982609);
       Wr(0x00049208, 0x20e83a0e);
       Wr(0x00049208, 0x21284a12);
       Wr(0x00049208, 0x21685a16);
       Wr(0x00049208, 0x21986619);
       Wr(0x00049208, 0x21d8761d);
       Wr(0x00049208, 0x22188621);
       Wr(0x00049208, 0x22589625);
       Wr(0x00049208, 0x2298a629);
       Wr(0x00049208, 0x22d8b62d);
       Wr(0x00049208, 0x2318c631);
       Wr(0x00049208, 0x2348d234);
       Wr(0x00049208, 0x2388e238);
       Wr(0x00049208, 0x23c8f23c);
       Wr(0x00049208, 0x23f8fe3f);
       Wr(0x00049208, 0x24390e43);
       Wr(0x00049208, 0x24791e47);
       Wr(0x00049208, 0x24a92a4a);
       Wr(0x00049208, 0x24e93a4e);
       Wr(0x00049208, 0x25194651);
       Wr(0x00049208, 0x25595655);
       Wr(0x00049208, 0x25896258);
       Wr(0x00049208, 0x25c9725c);
       Wr(0x00049208, 0x25f97e5f);
       Wr(0x00049208, 0x26298a62);
       Wr(0x00049208, 0x26699a66);
       Wr(0x00049208, 0x2699a669);
       Wr(0x00049208, 0x26c9b26c);
       Wr(0x00049208, 0x2709c270);
       Wr(0x00049208, 0x2739ce73);
       Wr(0x00049208, 0x2769da76);
       Wr(0x00049208, 0x2799e679);
       Wr(0x00049208, 0x27d9f67d);
       Wr(0x00049208, 0x280a0280);
       Wr(0x00049208, 0x283a0e83);
       Wr(0x00049208, 0x286a1a86);
       Wr(0x00049208, 0x289a2689);
       Wr(0x00049208, 0x28da368d);
       Wr(0x00049208, 0x290a4290);
       Wr(0x00049208, 0x293a4e93);
       Wr(0x00049208, 0x296a5a96);
       Wr(0x00049208, 0x299a6699);
       Wr(0x00049208, 0x29ca729c);
       Wr(0x00049208, 0x29fa7e9f);
       Wr(0x00049208, 0x2a2a8aa2);
       Wr(0x00049208, 0x2a5a96a5);
       Wr(0x00049208, 0x2a8aa2a8);
       Wr(0x00049208, 0x2abaaeab);
       Wr(0x00049208, 0x2aeabaae);
       Wr(0x00049208, 0x2b1ac6b1);
       Wr(0x00049208, 0x2b4ad2b4);
       Wr(0x00049208, 0x2b7adeb7);
       Wr(0x00049208, 0x2baaeaba);
       Wr(0x00049208, 0x2bcaf2bc);
       Wr(0x00049208, 0x2bfafebf);
       Wr(0x00049208, 0x2c2b0ac2);
       Wr(0x00049208, 0x2c5b16c5);
       Wr(0x00049208, 0x2c8b22c8);
       Wr(0x00049208, 0x2cbb2ecb);
       Wr(0x00049208, 0x2cdb36cd);
       Wr(0x00049208, 0x2d0b42d0);
       Wr(0x00049208, 0x2d3b4ed3);
       Wr(0x00049208, 0x2d6b5ad6);
       Wr(0x00049208, 0x2d8b62d8);
       Wr(0x00049208, 0x2dbb6edb);
       Wr(0x00049208, 0x2deb7ade);
       Wr(0x00049208, 0x2e1b86e1);
       Wr(0x00049208, 0x2e3b8ee3);
       Wr(0x00049208, 0x2e6b9ae6);
       Wr(0x00049208, 0x2e9ba6e9);
       Wr(0x00049208, 0x2ebbaeeb);
       Wr(0x00049208, 0x2eebbaee);
       Wr(0x00049208, 0x2f1bc6f1);
       Wr(0x00049208, 0x2f3bcef3);
       Wr(0x00049208, 0x2f6bdaf6);
       Wr(0x00049208, 0x2f8be2f8);
       Wr(0x00049208, 0x2fbbeefb);
       Wr(0x00049208, 0x2febfafe);
       Wr(0x00049208, 0x300c0300);
       Wr(0x00049208, 0x303c0f03);
       Wr(0x00049208, 0x305c1705);
       Wr(0x00049208, 0x308c2308);
       Wr(0x00049208, 0x30ac2b0a);
       Wr(0x00049208, 0x30dc370d);
       Wr(0x00049208, 0x30fc3f0f);
       Wr(0x00049208, 0x312c4b12);
       Wr(0x00049208, 0x314c5314);
       Wr(0x00049208, 0x317c5f17);
       Wr(0x00049208, 0x319c6719);
       Wr(0x00049208, 0x31cc731c);
       Wr(0x00049208, 0x31ec7b1e);
       Wr(0x00049208, 0x321c8721);
       Wr(0x00049208, 0x323c8f23);
       Wr(0x00049208, 0x326c9b26);
       Wr(0x00049208, 0x328ca328);
       Wr(0x00049208, 0x32acab2a);
       Wr(0x00049208, 0x32dcb72d);
       Wr(0x00049208, 0x32fcbf2f);
       Wr(0x00049208, 0x332ccb32);
       Wr(0x00049208, 0x334cd334);
       Wr(0x00049208, 0x336cdb36);
       Wr(0x00049208, 0x339ce739);
       Wr(0x00049208, 0x33bcef3b);
       Wr(0x00049208, 0x33dcf73d);
       Wr(0x00049208, 0x340d0340);
       Wr(0x00049208, 0x342d0b42);
       Wr(0x00049208, 0x344d1344);
       Wr(0x00049208, 0x347d1f47);
       Wr(0x00049208, 0x349d2749);
       Wr(0x00049208, 0x34bd2f4b);
       Wr(0x00049208, 0x34ed3b4e);
       Wr(0x00049208, 0x350d4350);
       Wr(0x00049208, 0x352d4b52);
       Wr(0x00049208, 0x354d5354);
       Wr(0x00049208, 0x357d5f57);
       Wr(0x00049208, 0x359d6759);
       Wr(0x00049208, 0x35bd6f5b);
       Wr(0x00049208, 0x35dd775d);
       Wr(0x00049208, 0x360d8360);
       Wr(0x00049208, 0x362d8b62);
       Wr(0x00049208, 0x364d9364);
       Wr(0x00049208, 0x366d9b66);
       Wr(0x00049208, 0x369da769);
       Wr(0x00049208, 0x36bdaf6b);
       Wr(0x00049208, 0x36ddb76d);
       Wr(0x00049208, 0x36fdbf6f);
       Wr(0x00049208, 0x371dc771);
       Wr(0x00049208, 0x373dcf73);
       Wr(0x00049208, 0x376ddb76);
       Wr(0x00049208, 0x378de378);
       Wr(0x00049208, 0x37adeb7a);
       Wr(0x00049208, 0x37cdf37c);
       Wr(0x00049208, 0x37edfb7e);
       Wr(0x00049208, 0x380e0380);
       Wr(0x00049208, 0x382e0b82);
       Wr(0x00049208, 0x385e1785);
       Wr(0x00049208, 0x387e1f87);
       Wr(0x00049208, 0x389e2789);
       Wr(0x00049208, 0x38be2f8b);
       Wr(0x00049208, 0x38de378d);
       Wr(0x00049208, 0x38fe3f8f);
       Wr(0x00049208, 0x391e4791);
       Wr(0x00049208, 0x393e4f93);
       Wr(0x00049208, 0x395e5795);
       Wr(0x00049208, 0x397e5f97);
       Wr(0x00049208, 0x39ae6b9a);
       Wr(0x00049208, 0x39ce739c);
       Wr(0x00049208, 0x39ee7b9e);
       Wr(0x00049208, 0x3a0e83a0);
       Wr(0x00049208, 0x3a2e8ba2);
       Wr(0x00049208, 0x3a4e93a4);
       Wr(0x00049208, 0x3a6e9ba6);
       Wr(0x00049208, 0x3a8ea3a8);
       Wr(0x00049208, 0x3aaeabaa);
       Wr(0x00049208, 0x3aceb3ac);
       Wr(0x00049208, 0x3aeebbae);
       Wr(0x00049208, 0x3b0ec3b0);
       Wr(0x00049208, 0x3b2ecbb2);
       Wr(0x00049208, 0x3b4ed3b4);
       Wr(0x00049208, 0x3b6edbb6);
       Wr(0x00049208, 0x3b8ee3b8);
       Wr(0x00049208, 0x3baeebba);
       Wr(0x00049208, 0x3bcef3bc);
       Wr(0x00049208, 0x3beefbbe);
       Wr(0x00049208, 0x3c0f03c0);
       Wr(0x00049208, 0x3c2f0bc2);
       Wr(0x00049208, 0x3c4f13c4);
       Wr(0x00049208, 0x3c6f1bc6);
       Wr(0x00049208, 0x3c8f23c8);
       Wr(0x00049208, 0x3caf2bca);
       Wr(0x00049208, 0x3cbf2fcb);
       Wr(0x00049208, 0x3cdf37cd);
       Wr(0x00049208, 0x3cff3fcf);
       Wr(0x00049208, 0x3d1f47d1);
       Wr(0x00049208, 0x3d3f4fd3);
       Wr(0x00049208, 0x3d5f57d5);
       Wr(0x00049208, 0x3d7f5fd7);
       Wr(0x00049208, 0x3d9f67d9);
       Wr(0x00049208, 0x3dbf6fdb);
       Wr(0x00049208, 0x3ddf77dd);
       Wr(0x00049208, 0x3dff7fdf);
       Wr(0x00049208, 0x3e1f87e1);
       Wr(0x00049208, 0x3e2f8be2);
       Wr(0x00049208, 0x3e4f93e4);
       Wr(0x00049208, 0x3e6f9be6);
       Wr(0x00049208, 0x3e8fa3e8);
       Wr(0x00049208, 0x3eafabea);
       Wr(0x00049208, 0x3ecfb3ec);
       Wr(0x00049208, 0x3eefbbee);
       Wr(0x00049208, 0x3effbfef);
       Wr(0x00049208, 0x3f1fc7f1);
       Wr(0x00049208, 0x3f3fcff3);
       Wr(0x00049208, 0x3f5fd7f5);
       Wr(0x00049208, 0x3f7fdff7);
       Wr(0x00049208, 0x3f9fe7f9);
       Wr(0x00049208, 0x3fbfeffb);
       Wr(0x00049208, 0x3fcff3fc);
       Wr(0x00049208, 0x3feffbfe);
       Wr(0x00049208, 0x3fffffff);
       Wr(0x00049200, 0x00000041);
       Wr(0x000448c0, 0x07e4046a);
       Wr(0x000448f0, 0x07b10032);
       Wr(0x000448f4, 0x04690032);
       Wr(0x000448dc, 0x001f000f);
       Wr(0x000448e0, 0x000c000a);
       Wr(0x000448e4, 0x001f000f);
       Wr(0x000448d4, 0x00000001);
       Wr(0x000448d0, 0x00000094);
       Wr(0x000448cc, 0x00f00438);*/

  Wr(0x00001034, 0x60000000);
  Wr(0x00001033, 0x00000000);
  Wr(0x00010100, 0x00000007);
  Wr(0x00010101, 0x5926631c);
  Wr(0x00010102, 0x00006363);
  Wr(0x00010103, 0x00136363);
  Wr(0x00010104, 0xcf008000);
  Wr(0x00010105, 0x2d49ca0f);
  Wr(0x00010106, 0x1cd62907);
  Wr(0x00010107, 0x0c418820);
  Wr(0x00010109, 0x87809111);
  Wr(0x0001010a, 0x000043c0);
  Wr(0x00010200, 0x02328898);
  Wr(0x00010201, 0xa840200f);
  Wr(0x00010202, 0xc220078f);
  Wr(0x00010203, 0x00008006);
  Wr(0x00010204, 0x20180601);
  Wr(0x00010206, 0x30000200);
  Wr(0x00010207, 0x00010017);
  Wr(0x00010208, 0x42328898);
  Wr(0x00010209, 0x010e0780);
  Wr(0x0001020a, 0x08202850);
  Wr(0x0001020b, 0x1fff0208);
  Wr(0x0001020c, 0x0bff0008);
  Wr(0x0001020d, 0x0008ffff);
  Wr(0x0001020e, 0x01006000);
  Wr(0x0001020f, 0x03ff0009);
  Wr(0x00010211, 0x00000009);
  Wr(0x00010212, 0x000003ff);
  Wr(0x00010213, 0x00000009);
  Wr(0x00010214, 0x03ff0000);
  Wr(0x00010215, 0x1e8c0003);
  Wr(0x00010217, 0xc6880001);
  Wr(0x00010218, 0x000000fa);
  Wr(0x00010219, 0x0c140002);
  Wr(0x00010222, 0x54070000);
  Wr(0x00010223, 0x32800500);
  Wr(0x00010224, 0x010e0000);
  Wr(0x00010225, 0x0003800c);
  Wr(0x0001023c, 0x00000003);
  Wr(0x0001023d, 0x800186a0);
  Wr(0x00010040, 0xd246c806);
  Wr(0x00010041, 0x3fff6db6);
  Wr(0x00010042, 0x70488028);
  Wr(0x00010043, 0x80c5821c);
  Wr(0x00010044, 0x40000410);
  Wr(0x00010045, 0x02818014);
  Wr(0x00010047, 0x00000046);
  Wr(0x00010062, 0xb33e83e8);
  Wr(0x00010063, 0x9808007c);
  Wr(0x00010064, 0x1cdf07d0);
  Wr(0x00010065, 0x8df007d0);
  Wr(0x00010069, 0x000228ff);
  Wr(0x0001006a, 0x00022800);
  Wr(0x0001006b, 0x000000f0);
  Wr(0x0001006c, 0x00040008);
  Wr(0x0001006e, 0x0004c488);
  Wr(0x0001006f, 0x10282828);
  Wr(0x00010070, 0x004381e0);
  Wr(0x00010071, 0x07800000);
  Wr(0x00010072, 0x0001fa3f);
  Wr(0x0001007c, 0x0840d855);
  Wr(0x00010076, 0x04050067);
  Wr(0x0001007b, 0x04619064);
  Wr(0x0001007c, 0x08c0d855);
  Wr(0x0001007d, 0x0a203041);
  Wr(0x0001007e, 0x0000400a);
  Wr(0x00001016, 0x8440078f);
  Wr(0x00001018, 0x01ff01ff);
  Wr(0x00001019, 0x000001ff);
  Wr(0x00001015, 0xb040200f);
  Wr(0x00001033, 0x40000000);
  Wr(0x0001110c, 0x00000019);
  Wr(0x00011080, 0x00000001);
  Wr(0x00011000, 0x0000000a);
  Wr(0x00011004, 0x00000002);
  Wr(0x00011108, 0x00000100);
  Wr(0x00011104, 0x0000000a);
  Wr(0x00011100, 0x00000800);
  Wr(0x00011144, 0x000ffff3);
  Wr(0x00011144, 0x000fffff);
  Wr(0x00011108, 0x00000101);
  Wr(0x00011104, 0x00000082);
  Wr(0x00011100, 0x00000800);
  Wr(0x00011144, 0x000ffff3);
  Wr(0x00011144, 0x000fffff);
  Wr(0x00011108, 0x00000600);
  Wr(0x00011104, 0x00000001);
  Wr(0x00011100, 0x00000800);
  Wr(0x00011144, 0x000ffff3);
  Wr(0x00011144, 0x000fffff);
  Wr(0x00011014, 0x00000000);
  Wr(0x0001100c, 0x00000001);
  Wr(0x00011108, 0x00000102);
  Wr(0x00011104, 0x00000021);
  Wr(0x00011100, 0x00000800);
  Wr(0x00011144, 0x000ffff3);
  Wr(0x00011144, 0x000fffff);
  Wr(0x00011108, 0x00000102);
  Wr(0x00011100, 0x00000900);
  Wr(0x00011144, 0x000ffff3);
  Wr(0x00011144, 0x000fffff);
  Wr(0x00011108, 0x00000202);
  Wr(0x00011100, 0x00000902);
  Wr(0x00011144, 0x000ffff3);
  Wr(0x00011144, 0x000fffff);
  Wr(0x0001100c, 0x00000002);
  Wr(0x00011108, 0x00000102);
  Wr(0x00011104, 0x00000022);
  Wr(0x00011100, 0x00000800);
  Wr(0x00011144, 0x000ffff3);
  Wr(0x00011144, 0x000fffff);
  Wr(0x00011108, 0x00000202);
  Wr(0x00011100, 0x00000902);
  Wr(0x00011144, 0x000ffff3);
  Wr(0x00011144, 0x000fffff);
  Wr(0x00011108, 0x00000202);
  Wr(0x00011100, 0x00000902);
  Wr(0x00011144, 0x000ffff3);
  Wr(0x00011144, 0x000fffff);
  Wr(0x0001100c, 0x00000000);
  Wr(0x00011108, 0x00000102);
  Wr(0x00011104, 0x00000000);
  Wr(0x00011100, 0x00000800);
  Wr(0x00011144, 0x000ffff3);
  Wr(0x00011144, 0x000fffff);
  Wr(0x00011108, 0x00000202);
  Wr(0x00011100, 0x00000901);
  Wr(0x00011144, 0x000ffff3);
  Wr(0x00011144, 0x000fffff);
  Wr(0x00011820, 0x00000898);
  Wr(0x00011824, 0x00000465);
  Wr(0x00011828, 0x00000003);
  Wr(0x0001182c, 0x0000002c);
  Wr(0x00011830, 0x00000005);
  Wr(0x00011834, 0x00000780);
  Wr(0x00011838, 0x00000438);
  Wr(0x0001183c, 0x000000c0);
  Wr(0x00011840, 0x00000029);
  Wr(0x00011848, 0x00000000);
  Wr(0x0001184c, 0x00003a02);
  Wr(0x00011850, 0x00000040);
  Wr(0x0001185c, 0x00000870);
  Wr(0x00011864, 0x0000000f);
  Wr(0x00011854, 0x00006978);
  Wr(0x00011814, 0x00000000);
  Wr(0x00011844, 0x00000000);
  Wr(0x00011848, 0x00000000);
  Wr(0x00011858, 0x00000001);
  Wr(0x00011850, 0x09270040);
  Wr(0x00011810, 0x00000606);
  Wr(0x00011a10, 0x00002004);
  Wr(0x00011c10, 0x00002004);
  Wr(0x00011e10, 0x00002004);
  Wr(0x00011094, 0x00000001);
  Wr(0x00011800, 0x00000001);
  Wr(0x00011a00, 0x00000000);
  Wr(0x00011c00, 0x00000000);
  Wr(0x00011e00, 0x00000000);
  Wr(0x000110c0, 0x00000001);
  Wr(0x00011840, 0x0000000f); // v start 41
  // Wr(0x00010200, 0x02328818);
}
