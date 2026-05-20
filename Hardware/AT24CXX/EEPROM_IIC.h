#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"

/**************************************移植相关**********************************************************************/
// IO方向设置  SCL--PE10   SDA--PE11
// 因为使用的是模拟IIC，引脚只需接普通IO口即可，修改以下宏定义即可完成配置
#define EEPROM_SDA_IN()                  \
    {                                    \
        GPIOE->MODER &= ~(3 << (9 * 2)); \
        GPIOE->MODER |= 0 << 9 * 2;      \
    } // PB9输入模式
#define EEPROM_SDA_OUT()                 \
    {                                    \
        GPIOE->MODER &= ~(3 << (9 * 2)); \
        GPIOE->MODER |= 1 << 9 * 2;      \
    } // PB9输出模式

#define EEPROM_RCC RCC_AHB1Periph_GPIOE

#define EEPROM_SCL_PIN GPIO_Pin_8
#define EEPROM_SDA_PIN GPIO_Pin_9
#define EEPROM_PORT GPIOE

#define EEPROM_IIC_SCL PEout(8) // SCL
#define EEPROM_IIC_SDA PEout(9) // SDA
#define EEPROM_READ_SDA PEin(9) // 输入SDA

// IIC所有操作函数
void EEPROM_IIC_Init(void);                 // 初始化IIC的IO口
void EEPROM_IIC_Start(void);                // 发送IIC开始信号
void EEPROM_IIC_Stop(void);                 // 发送IIC停止信号
void EEPROM_IIC_Send_Byte(u8 txd);          // IIC发送一个字节
u8 EEPROM_IIC_Read_Byte(unsigned char ack); // IIC读取一个字节
u8 EEPROM_IIC_Wait_Ack(void);               // IIC等待ACK信号
void EEPROM_IIC_Ack(void);                  // IIC发送ACK信号
void EEPROM_IIC_NAck(void);                 // IIC不发送ACK信号

void EEPROM_IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 EEPROM_IIC_Read_One_Byte(u8 daddr, u8 addr);

#endif
