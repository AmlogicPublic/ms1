
#ifndef EDP_TX_CFG_H
#define EDP_TX_CFG_H

#include <stdbool.h>
#include <stdint.h>


#define DEBUG_WITHOUT_USBI2C

extern void edp_tx_cfg();

static int gamma_readcommandbytes(int index, int inbbytes, unsigned char *pucdatabuffer);
static int gamma_writecommandbytes(int index, int inbbytes, unsigned long *pucdatabuffer);

void usleep(unsigned int p0);
void WriteReg(uint32_t addr, uint32_t data);
uint32_t ReadReg(uint32_t addr);

bool USBIO_StreamI2C(         // 处理I2C数据流,2线接口,时钟线为SCL引脚,数据线为SDA引脚(准双向I/O),速度约56K字节
    uint64_t iIndex,          // 指定mUSBIO_PACKET_LENGTH设备序号
    uint64_t iWriteLength,    // 准备写出的数据字节数
    const void *iWriteBuffer, // 指向一个缓冲区,放置准备写出的数据,首字节通常是I2C设备地址及读写方向位
    uint64_t iReadLength,     // 准备读取的数据字节数
    void *oReadBuffer);       // 指向一个缓冲区,返回后是读入的数据

bool USBIO_WriteI2C( // 向I2C接口写入一个字节数据
    uint64_t iIndex, // 指定mUSBIO_PACKET_LENGTH设备序号
    uint8_t iDevice, // 低7位指定I2C设备地址
    uint8_t iAddr,   // 指定数据单元的地址
    uint8_t iByte);  // 待写入的字节数据

bool USBIO_ReadI2C(  // 向I2C接口写入一个字节数据
    uint64_t iIndex, // 指定mUSBIO_PACKET_LENGTH设备序号
    uint8_t iDevice, // 低7位指定I2C设备地址
    uint8_t iAddr,   // 指定数据单元的地址
    uint8_t *oByte); // 指向一个字节单元,用于保存读取的字节数据

void edp_tx_cfg_v1();

#endif
