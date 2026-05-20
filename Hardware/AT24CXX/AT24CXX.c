#include "AT24CXX.h"
#include "EEPROM_IIC.h"

// 初始化IIC接口
void AT24CXX_Init(void)
{
	u8 key;
	EEPROM_IIC_Init();		// IIC初始化
	while (AT24CXX_Check()) // 检测不到24c02
	{
		OLED_ShowString(20, 0, (void *)"EEPROM error", 12);
		OLED_ShowString(0, 20, (void *)"Press Back to Break!", 12);
		OLED_Refresh_Gram();
		key = KEY_Scan(0);
		if (key == kb)
			break;
	}
	OLED_Clear();
	OLED_Refresh_Gram();
}

// 在AT24CXX指定地址读出一个数据
// ReadAddr:开始读数的地址
// 返回值  :读到的数据
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
	u8 temp = 0;
	EEPROM_IIC_Start();
	if (EE_TYPE > AT24C16)
	{
		EEPROM_IIC_Send_Byte(0XA0); // 发送写命令
		EEPROM_IIC_Wait_Ack();
		EEPROM_IIC_Send_Byte(ReadAddr >> 8); // 发送高地址
	}
	else
		EEPROM_IIC_Send_Byte(0XA0 + ((ReadAddr / 256) << 1)); // 发送器件地址0XA0,写数据
	EEPROM_IIC_Wait_Ack();
	EEPROM_IIC_Send_Byte(ReadAddr % 256); // 发送低地址
	EEPROM_IIC_Wait_Ack();
	EEPROM_IIC_Start();
	EEPROM_IIC_Send_Byte(0XA1); // 进入接收模式
	EEPROM_IIC_Wait_Ack();
	temp = EEPROM_IIC_Read_Byte(0);
	EEPROM_IIC_Stop(); // 产生一个停止条件
	return temp;
}

// 在AT24CXX指定地址写入一个数据
// WriteAddr  :写入数据的目的地址
// DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
	EEPROM_IIC_Start();
	if (EE_TYPE > AT24C16)
	{
		EEPROM_IIC_Send_Byte(0XA0); // 发送写命令
		EEPROM_IIC_Wait_Ack();
		EEPROM_IIC_Send_Byte(WriteAddr >> 8); // 发送高地址
	}
	else
		EEPROM_IIC_Send_Byte(0XA0 + ((WriteAddr / 256) << 1)); // 发送器件地址0XA0,写数据
	EEPROM_IIC_Wait_Ack();
	EEPROM_IIC_Send_Byte(WriteAddr % 256); // 发送低地址
	EEPROM_IIC_Wait_Ack();
	EEPROM_IIC_Send_Byte(DataToWrite); // 发送字节
	EEPROM_IIC_Wait_Ack();
	EEPROM_IIC_Stop(); // 产生一个停止条件
	delay_ms(10);
}

// 在AT24CXX里面的指定地址开始写入长度为Len的数据
// 该函数用于写入16bit或者32bit的数据.
// WriteAddr  :开始写入的地址
// DataToWrite:数据数组首地址
// Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(u16 WriteAddr, u32 DataToWrite, u8 Len)
{
	u8 t;
	for (t = 0; t < Len; t++)
	{
		AT24CXX_WriteOneByte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xff);
	}
}

// 在AT24CXX里面的指定地址开始读出长度为Len的数据
// 该函数用于读出16bit或者32bit的数据.
// ReadAddr   :开始读出的地址
// 返回值     :数据
// Len        :要读出数据的长度2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr, u8 Len)
{
	u8 t;
	u32 temp = 0;
	for (t = 0; t < Len; t++)
	{
		temp <<= 8;
		temp += AT24CXX_ReadOneByte(ReadAddr + Len - t - 1);
	}
	return temp;
}

// 检查AT24CXX是否正常
// 这里用了24XX的最后一个地址(255)来存储标志字.
// 如果用其他24C系列,这个地址要修改
// 返回1:检测失败
// 返回0:检测成功
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp = AT24CXX_ReadOneByte(255); // 避免每次开机都写AT24CXX
	if (temp == 0X55)
		return 0;
	else // 排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255, 0X55);
		temp = AT24CXX_ReadOneByte(255);
		if (temp == 0X55)
			return 0;
	}
	return 1;
}

// 在AT24CXX里面的指定地址开始读出指定个数的数据
// ReadAddr :开始读出的地址 对24c02为0~255
// pBuffer  :数据数组首地址
// NumToRead:要读出数据的个数
void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead)
{
	while (NumToRead)
	{
		*pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
		NumToRead--;
	}
}

// 在AT24CXX里面的指定地址开始写入指定个数的数据
// WriteAddr :开始写入的地址 对24c02为0~255
// pBuffer   :数据数组首地址
// NumToWrite:要写入数据的个数
void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
	while (NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}

/*
********************************************************************************************************************
*                 void AT24CXX_Writefloat(u16 WriteAddr,float DataToWrite)
*
*Description : 写入浮点型数据
*Arguments   : none
*Returns     : none
*Notes       : WriteAddr:写入地址（0~255），DataToWrite:写入的浮点型数据
********************************************************************************************************************
*/
void AT24CXX_Writefloat(u16 WriteAddr, float DataToWrite)
{
	u8 i = 0;
	u8 data_save[4] = {0};
	data_save[0] = EEP_BYTE0(DataToWrite);
	data_save[1] = EEP_BYTE1(DataToWrite);
	data_save[2] = EEP_BYTE2(DataToWrite);
	data_save[3] = EEP_BYTE3(DataToWrite);
	for (i = 0; i < 4; i++)
	{
		AT24CXX_WriteOneByte(WriteAddr + i, data_save[i]);
	}
}

/*
********************************************************************************************************************
*                 float AT24CXX_Readfloat(u16 ReadAddr)
*
*Description : 读出浮点型数据
*Arguments   : none
*Returns     : none
*Notes       : ReadAddr:读出地址（0~255），哪里写，哪里读
********************************************************************************************************************
*/
float AT24CXX_Readfloat(u16 ReadAddr)
{
	u8 i = 0;
	int temp = 0;
	float float_data;
	for (i = 0; i < 4; i++)
	{
		temp <<= 8;
		temp += AT24CXX_ReadOneByte(ReadAddr + 4 - i - 1);
	}
	float_data = *(float *)&temp;
	return float_data;
}
