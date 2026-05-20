#include "EEPROM_IIC.h"

// 初始化IIC
void EEPROM_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(EEPROM_RCC, ENABLE); // 使能GPIO时钟

	GPIO_InitStructure.GPIO_Pin = EEPROM_SCL_PIN | EEPROM_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	   // 普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   // 上拉
	GPIO_Init(EEPROM_PORT, &GPIO_InitStructure);	   // 初始化
	EEPROM_IIC_SCL = 1;
	EEPROM_IIC_SDA = 1;
}

// 产生IIC起始信号
void EEPROM_IIC_Start(void)
{
	EEPROM_SDA_OUT(); // sda线输出
	EEPROM_IIC_SDA = 1;
	EEPROM_IIC_SCL = 1;
	delay_us(4);
	EEPROM_IIC_SDA = 0; // START:when CLK is high,DATA change form high to low
	delay_us(4);
	EEPROM_IIC_SCL = 0; // 钳住I2C总线，准备发送或接收数据
}

// 产生IIC停止信号
void EEPROM_IIC_Stop(void)
{
	EEPROM_SDA_OUT(); // sda线输出
	EEPROM_IIC_SCL = 0;
	EEPROM_IIC_SDA = 0; // STOP:when CLK is high DATA change form low to high
	delay_us(4);
	EEPROM_IIC_SCL = 1;
	EEPROM_IIC_SDA = 1; // 发送I2C总线结束信号
	delay_us(4);
}

// 等待应答信号到来
// 返回值：1，接收应答失败
//         0，接收应答成功
u8 EEPROM_IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	EEPROM_SDA_IN(); // SDA设置为输入
	EEPROM_IIC_SDA = 1;
	delay_us(1);
	EEPROM_IIC_SCL = 1;
	delay_us(1);
	while (EEPROM_READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			EEPROM_IIC_Stop();
			return 1;
		}
	}
	EEPROM_IIC_SCL = 0; // 时钟输出0
	return 0;
}

// 产生ACK应答
void EEPROM_IIC_Ack(void)
{
	EEPROM_IIC_SCL = 0;
	EEPROM_SDA_OUT();
	EEPROM_IIC_SDA = 0;
	delay_us(2);
	EEPROM_IIC_SCL = 1;
	delay_us(2);
	EEPROM_IIC_SCL = 0;
}

// 不产生ACK应答
void EEPROM_IIC_NAck(void)
{
	EEPROM_IIC_SCL = 0;
	EEPROM_SDA_OUT();
	EEPROM_IIC_SDA = 1;
	delay_us(2);
	EEPROM_IIC_SCL = 1;
	delay_us(2);
	EEPROM_IIC_SCL = 0;
}

// IIC发送一个字节
// 返回从机有无应答
// 1，有应答
// 0，无应答
void EEPROM_IIC_Send_Byte(u8 txd)
{
	u8 t;
	EEPROM_SDA_OUT();
	EEPROM_IIC_SCL = 0; // 拉低时钟开始数据传输
	for (t = 0; t < 8; t++)
	{
		EEPROM_IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		delay_us(2); // 对TEA5767这三个延时都是必须的
		EEPROM_IIC_SCL = 1;
		delay_us(2);
		EEPROM_IIC_SCL = 0;
		delay_us(2);
	}
}

// 读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 EEPROM_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	EEPROM_SDA_IN(); // SDA设置为输入
	for (i = 0; i < 8; i++)
	{
		EEPROM_IIC_SCL = 0;
		delay_us(2);
		EEPROM_IIC_SCL = 1;
		receive <<= 1;
		if (EEPROM_READ_SDA)
			receive++;
		delay_us(1);
	}
	if (!ack)
		EEPROM_IIC_NAck(); // 发送nACK
	else
		EEPROM_IIC_Ack(); // 发送ACK
	return receive;
}
