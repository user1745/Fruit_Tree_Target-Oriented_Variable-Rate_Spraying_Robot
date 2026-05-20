/*************************************************************
 *
 * @file    I2C.h
 * @version V1.0
 * @date    2016-xx-xx
 * @brief   STM32模拟IIC协议
 *
 *************************************************************/
/* $ PAGE*/
#include "I2C.h"
#include "Delay.h"

void Delay_IIC(unsigned char n)
{
	unsigned int i, j;
	for (j = 0; j < n; j++)
	{
		for (i = 0; i < 2; i++)
			;
	}
}

/******************************************************************************
 *                           void IIC_Init(void)
 *
 *Description : 初始化I2C对应的接口引脚
 *Arguments   : none
 *Returns     : none
 *Notes       : none
 *******************************************************************************
 */
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); // 使能GPIOB时钟

	// GPIO初始化设置
	GPIO_InitStructure.GPIO_Pin = IIC_SCL_TRPIN | IIC_SDA_TRPIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	  // 普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	  // 开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	  // 上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);			  // 初始化

	IIC_SCL = 1;
	IIC_SDA = 1;
}

/******************************************************************************
 *                           void IIC_Start(void)
 *
 *Description : 产生IIC起始信号
 *Arguments   : none
 *Returns     : none
 *Notes       : none
 *******************************************************************************
 */
int IIC_Start(void)
{
	SDA_OUT(); // sda线输出
	IIC_SDA = 1;
	if (!READ_SDA)
		return 0;
	IIC_SCL = 1;
	delay_us(1);
	IIC_SDA = 0; // START:when CLK is high,DATA change form high to low
	if (READ_SDA)
		return 0;
	delay_us(1);
	IIC_SCL = 0; // 钳住I2C总线，准备发送或接收数据
	return 1;
}

// void IIC_Start(void)
//{
//	SDA_OUT();
//	IIC_SDA=1;
//	IIC_SCL=1;
//	for(int i=0;i<135;i++){__NOP();}
//  	IIC_SDA=0;
//	for(int i=0;i<135;i++){__NOP();}
//	IIC_SCL=0;
// }
/******************************************************************************
 *                           void IIC_Stop(void)
 *
 *Description : 产生IIC停止信号
 *Arguments   : none
 *Returns     : none
 *Notes       : none
 *******************************************************************************
 */
// void IIC_Stop(void)
//{
//	SDA_OUT();//sda线输出
//	IIC_SCL=0;
//	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
//  	delay_us(1);
//	IIC_SCL=1;
//	IIC_SDA=1;//发送I2C总线结束信号
//	delay_us(1);
// }

void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL = 0;
	IIC_SDA = 0;
	for (int i = 0; i < 135; i++)
	{
		__NOP();
	}
	IIC_SCL = 1;
	IIC_SDA = 1;
	for (int i = 0; i < 135; i++)
	{
		__NOP();
	}
}

/******************************************************************************
 *                         u8 IIC_Wait_Ack(void)
 *
 *Description : 等待应答信号到来
 *Arguments   : none
 *Returns     : 1:接收应答失败    0:接收应答成功
 *Notes       : none
 *******************************************************************************
 */
// u8 IIC_Wait_Ack(void)
//{
//	u8 ucErrTime=0;
//	SDA_IN();      //SDA设置为输入
//	IIC_SDA=1;
//	delay_us(1);
//	IIC_SCL=1;
//	delay_us(1);
//	while(READ_SDA)
//	{
//		ucErrTime++;
//		if(ucErrTime>50)
//		{
//			IIC_Stop();
//			return 1;
//		}
//	  delay_us(1);
//	}
//	IIC_SCL=0;//时钟输出0
//	return 0;
// }

u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_IN();
	IIC_SDA = 1;
	for (int i = 0; i < 30; i++)
	{
		__NOP();
	}
	IIC_SCL = 1;
	for (int i = 0; i < 30; i++)
	{
		__NOP();
	}
	while (READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 50)
		{
			IIC_Stop();
			return 1;
		}
		//	  Delay_IIC(1);
	}
	IIC_SCL = 0;
	return 0;
}

/******************************************************************************
 *                         void IIC_Ack(void)
 *
 *Description : 产生ACK应答
 *Arguments   : none
 *Returns     : none
 *Notes       : none
 *******************************************************************************
 */
// void IIC_Ack(void)
//{
//	IIC_SCL=0;
//	SDA_OUT();
//	IIC_SDA=0;
//	delay_us(1);
//	IIC_SCL=1;
//	delay_us(1);
//	IIC_SCL=0;
// }
void IIC_Ack(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 0;
	for (int i = 0; i < 65; i++)
	{
		__NOP();
	}
	IIC_SCL = 1;
	for (int i = 0; i < 65; i++)
	{
		__NOP();
	}
	IIC_SCL = 0;
}
/******************************************************************************
 *                         void IIC_NAck(void)
 *
 *Description : 产生NACK应答
 *Arguments   : none
 *Returns     : none
 *Notes       : none
 *******************************************************************************
 */
// void IIC_NAck(void)
//{
//	IIC_SCL=0;
//	SDA_OUT();
//	IIC_SDA=1;
//	delay_us(1);
//	IIC_SCL=1;
//	delay_us(1);
//	IIC_SCL=0;
// }
void IIC_NAck(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 1;
	for (int i = 0; i < 65; i++)
	{
		__NOP();
	}
	IIC_SCL = 1;
	for (int i = 0; i < 65; i++)
	{
		__NOP();
	}
	IIC_SCL = 0;
}
/******************************************************************************
 *                        void IIC_Send_Byte(u8 txd)
 *
 *Description : IIC发送一个字节
 *Arguments   : txd ：发送的数据
 *Returns     : none
 *Notes       : none
 *******************************************************************************
 */

// void IIC_Send_Byte(u8 txd)
//{
//     u8 t;
//	SDA_OUT();
//     IIC_SCL=0;//拉低时钟开始数据传输
//     for(t=0;t<8;t++)
//     {
//         IIC_SDA=(txd&0x80)>>7;
//         txd<<=1;
//		delay_us(1);
//		IIC_SCL=1;
//		delay_us(1);
//		IIC_SCL=0;
//		delay_us(1);
//     }
// }

void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL = 0;
	for (t = 0; t < 8; t++)
	{
		IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		for (int i = 0; i < 65; i++)
		{
			__NOP();
		}
		IIC_SCL = 1;
		for (int i = 0; i < 65; i++)
		{
			__NOP();
		}
		IIC_SCL = 0;
		for (int i = 0; i < 65; i++)
		{
			__NOP();
		}
	}
}

/******************************************************************************
 *                       u8 IIC_Read_Byte(unsigned char ack)
 *
 *Description : 读1个字节，
 *Arguments   : ack=1，发送ACK，ack=0，发送nACK
 *Returns     : 读回的数据
 *Notes       : none
 *******************************************************************************
 */
// u8 IIC_Read_Byte(unsigned char ack)
//{
//	unsigned char i,receive=0;
//	SDA_IN();//SDA设置为输入
//     for(i=0;i<8;i++ )
//	{
//         IIC_SCL=0;
//         delay_us(2);
//		IIC_SCL=1;
//         receive<<=1;
//         if(READ_SDA)receive++;
//		delay_us(2);
//     }
//     if (ack)
//         IIC_Ack(); //发送ACK
//     else
//         IIC_NAck();//发送nACK
//     return receive;
// }

u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	SDA_IN();
	for (i = 0; i < 8; i++)
	{
		IIC_SCL = 0;
		for (int i = 0; i < 30; i++)
		{
			__NOP();
		}
		IIC_SCL = 1;
		receive <<= 1;
		if (READ_SDA)
			receive++;
		for (int i = 0; i < 30; i++)
		{
			__NOP();
		}
	}
	if (ack)
		IIC_Ack();
	else
		IIC_NAck();
	return receive;
}

/******************************************************************************
 *    unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
 *
 *Description : 读取指定设备 指定寄存器的一个值
 *Arguments   : I2C_Addr : 目标设备地址   addr	   : 寄存器地址
 *Returns     : 读回的数据
 *Notes       : none
 *******************************************************************************
 */
unsigned char I2C_ReadOneByte(unsigned char I2C_Addr, unsigned char addr)
{
	unsigned char res = 0;

	IIC_Start();
	IIC_Send_Byte(I2C_Addr);
	res++;
	IIC_Wait_Ack();
	IIC_Send_Byte(addr);
	res++;
	IIC_Wait_Ack();
	// IIC_Stop();
	IIC_Start();
	IIC_Send_Byte(I2C_Addr + 1);
	res++;
	IIC_Wait_Ack();
	res = IIC_Read_Byte(0);
	IIC_Stop();

	return res;
}

/******************************************************************************
 *     u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
 *
 *Description : 读取指定设备,指定寄存器的length个值
 *Arguments   : dev    : 目标设备地址	reg	   : 寄存器地址 	length : 要读的字节数
 *data  : 读出的数据将要存放的指针
 *Returns     : 读取到多少个数据
 *Notes       : none
 *******************************************************************************
 */
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
{
	u8 count = 0;
	u8 temp;
	IIC_Start();
	IIC_Send_Byte(dev);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(dev + 1);
	IIC_Wait_Ack();

	for (count = 0; count < length; count++)
	{
		if (count != (length - 1))
			temp = IIC_Read_Byte(1);
		else
			temp = IIC_Read_Byte(0);
		data[count] = temp;
	}
	IIC_Stop();
	return count;
}

/******************************************************************************
*        u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data)
*
*Description : 将多个字节写入指定设备 指定寄存器
*Arguments   : dev    : 目标设备地址
						 reg	  : 寄存器地址
						 length : 要写的字节数
						 *data  : 将要写的数据的首地址
*Returns     : 返回是否成功
*Notes       : none
*******************************************************************************
*/
u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8 *data)
{
	u8 count = 0;
	IIC_Start();
	IIC_Send_Byte(dev);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	for (count = 0; count < length; count++)
	{
		IIC_Send_Byte(data[count]);
		IIC_Wait_Ack();
	}
	IIC_Stop();

	return 1;
}

/******************************************************************************
*           u8 IICreadByte(u8 dev, u8 reg, u8 *data)
*
*Description : 读取指定设备 指定寄存器的一个值
*Arguments   : dev   : 目标设备地址
						   reg	  : 寄存器地址
						   *data : 读出的数据将要存放的地址
*Returns     : 1
*Notes       : none
*******************************************************************************
*/
u8 IICreadByte(u8 dev, u8 reg, u8 *data)
{
	*data = I2C_ReadOneByte(dev, reg);
	return 1;
}

/******************************************************************************
* unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
*
*Description : 写入指定设备 指定寄存器一个字节
*Arguments   : dev  : 目标设备地址
						 reg  : 寄存器地址
						 data : 将要写入的字节
*Returns     : 返回是否成功
*Notes       : none
*******************************************************************************
*/
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
{
	return IICwriteBytes(dev, reg, 1, &data);
}

/******************************************************************************
*     u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
*
*Description : 读 修改 写 指定设备 指定寄存器一个字节 中的多个位
*Arguments   : dev     : 目标设备地址
						   reg	   : 寄存器地址
						   bitStart: 目标字节的起始位
						   length  : 位长度
						   data    : 存放改变目标字节位的值
*Returns     : 成功 :1
						   失败 :0
*Notes       : none
*******************************************************************************
*/
u8 IICwriteBits(u8 dev, u8 reg, u8 bitStart, u8 length, u8 data)
{

	u8 b;
	if (IICreadByte(dev, reg, &b) != 0)
	{
		u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
		data <<= (8 - length);
		data >>= (7 - bitStart);
		b &= mask;
		b |= data;
		return IICwriteByte(dev, reg, b);
	}
	else
		return 0;
}
/******************************************************************************
*          u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
*
*Description : 读 修改 写 指定设备 指定寄存器一个字节 中的1个位
*Arguments   : dev   : 目标设备地址
						   reg	  : 寄存器地址
						   bitNum: 要修改目标字节的bitNum位
						   data  :
										0:目标位将被清0
										1:目标位将被置位
*Returns     : 成功 :1
						   失败 :0
*Notes       : none
*******************************************************************************
*/
u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
{
	u8 b;
	IICreadByte(dev, reg, &b);
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));

	return IICwriteByte(dev, reg, b);
}

/**************************ÊµÏÖº¯Êý********************************************
 *º¯ÊýÔ­ÐÍ:		bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
 *¹¦¡¡¡¡ÄÜ:
 *******************************************************************************/
int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
	int i;
	IIC_Start();
	IIC_Send_Byte(addr << 1);
	if (!IIC_Wait_Ack())
	{
		IIC_Stop();
		return 1;
	}
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	for (i = 0; i < len; i++)
	{
		IIC_Send_Byte(data[i]);
		if (!IIC_Wait_Ack())
		{
			IIC_Stop();
			return 0;
		}
	}
	IIC_Stop();
	return 0;
}
/**************************ÊµÏÖº¯Êý********************************************
 *º¯ÊýÔ­ÐÍ:		bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
 *¹¦¡¡¡¡ÄÜ:
 *******************************************************************************/
int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	IIC_Start();
	IIC_Send_Byte(addr << 1);
	if (!IIC_Wait_Ack())
	{
		IIC_Stop();
		return 1;
	}
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte((addr << 1) + 1);
	IIC_Wait_Ack();
	while (len)
	{
		if (len == 1)
			*buf = IIC_Read_Byte(0);
		else
			*buf = IIC_Read_Byte(1);
		buf++;
		len--;
	}
	IIC_Stop();
	return 0;
}
