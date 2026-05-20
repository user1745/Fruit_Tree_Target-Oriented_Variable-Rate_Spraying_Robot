/*************************************************************
 *
 * @file    led.h
 * @version V1.0
 * @date    2016-xx-xx
 * @brief   led/flash
 *
 *************************************************************/
/* $ PAGE*/
#ifndef __I2C_H
#define __I2C_H
#include "sys.h"

/*************************************************************
  @Macro
*************************************************************/
#define IIC_TRRCC RCC_AHB1Periph_GPIOE
#define IIC_SCL_TRPIN GPIO_Pin_10
#define IIC_SDA_TRPIN GPIO_Pin_11
#define IIC_TRPORT GPIOE

////驱动接口，GPIO模拟IIC

#define SDA_IN()                           \
  {                                        \
    GPIOE->MODER &= ~((u32)3 << (11 * 2)); \
    GPIOE->MODER |= 0 << 11 * 2;           \
  }
#define SDA_OUT()                          \
  {                                        \
    GPIOE->MODER &= ~((u32)3 << (11 * 2)); \
    GPIOE->MODER |= 1 << 11 * 2;           \
  }

// IO操作函数
#define IIC_SCL PEout(10) // SCL
#define IIC_SDA PEout(11) // SDA
#define READ_SDA PEin(11) // 输入SDA

/*************************************************************
  @Function
*************************************************************/
// IIC所有操作函数
void IIC_Init(void);                 // 初始化IIC的IO口
int IIC_Start(void);                 // 发送IIC开始信号
void IIC_Stop(void);                 // 发送IIC停止信号
void IIC_Send_Byte(u8 txd);          // IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack); // IIC读取一个字节
u8 IIC_Wait_Ack(void);               // IIC等待ACK信号
void IIC_Ack(void);                  // IIC发送ACK信号
void IIC_NAck(void);                 // IIC不发送ACK信号

// void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
// u8 IIC_Read_One_Byte(u8 daddr,u8 addr);
// unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);
// u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data);
// u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data);
// u8 IICwriteBit(u8 dev,u8 reg,u8 bitNum,u8 data);
// u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data);
// int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
// int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);

#endif

//*************************************************************************************
