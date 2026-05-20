#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "sys.h"

/**************************************移植相关**********************************************************************/
//										普通6050
// IO方向设置  SCL--PE10   SDA--PE11
// 因为使用的是模拟IIC，引脚只需接普通IO口即可，修改以下宏定义即可完成配置
#define MPU_SDA_IN()                      \
    {                                     \
        GPIOE->MODER &= ~(3 << (11 * 2)); \
        GPIOE->MODER |= 0 << 11 * 2;      \
    } // PB11输入模式
#define MPU_SDA_OUT()                     \
    {                                     \
        GPIOE->MODER &= ~(3 << (11 * 2)); \
        GPIOE->MODER |= 1 << 11 * 2;      \
    } // PB11输出模式

#define MPU_RCC RCC_AHB1Periph_GPIOE

#define MPU_SCL_PIN GPIO_Pin_10
#define MPU_SDA_PIN GPIO_Pin_11
#define MPU_PORT GPIOE

#define MPU_IIC_SCL PEout(10) // SCL
#define MPU_IIC_SDA PEout(11) // SDA
#define MPU_READ_SDA PEin(11) // 输入SDA
/*******************************************************************************************************************/

///**************************************移植相关**********************************************************************/
////										原子6050
////IO方向设置  SCL--PE11   SDA--PE10
////因为使用的是模拟IIC，引脚只需接普通IO口即可，修改以下宏定义即可完成配置
// #define MPU_SDA_IN()  {GPIOE->MODER&=~(3<<(10*2));GPIOE->MODER|=0<<10*2;}	//PB11输入模式
// #define MPU_SDA_OUT() {GPIOE->MODER&=~(3<<(10*2));GPIOE->MODER|=1<<10*2;}   //PB11输出模式

// #define MPU_RCC       RCC_AHB1Periph_GPIOE

// #define MPU_SCL_PIN    GPIO_Pin_11
// #define MPU_SDA_PIN    GPIO_Pin_10
// #define MPU_PORT   	   GPIOE

// #define MPU_IIC_SCL    PEout(11) //SCL
// #define MPU_IIC_SDA    PEout(10) //SDA
// #define MPU_READ_SDA  	   PEin(10)  //输入SDA
///*******************************************************************************************************************/

// IIC所有操作函数
void MPU_IIC_Init(void);                 // 初始化IIC的IO口
void MPU_IIC_Start(void);                // 发送IIC开始信号
void MPU_IIC_Stop(void);                 // 发送IIC停止信号
void MPU_IIC_Send_Byte(u8 txd);          // IIC发送一个字节
u8 MPU_IIC_Read_Byte(unsigned char ack); // IIC读取一个字节
u8 MPU_IIC_Wait_Ack(void);               // IIC等待ACK信号
void MPU_IIC_Ack(void);                  // IIC发送ACK信号
void MPU_IIC_NAck(void);                 // IIC不发送ACK信号

void MPU_IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr, u8 addr);
#endif
