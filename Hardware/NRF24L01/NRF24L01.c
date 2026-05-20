/*
******************************************************************************************************************
								CE--PD4
								CSN--PD1
								SCK--PB3
								MOSI--PB5
								MISO--PB4
								IRQ--PD0

******************************************************************************************************************
*/

#include "NRF24L01.h"
#include "SPI3.h"
/*
******************************************************************************************************************
*                                            CONSTANTS & MACROS
******************************************************************************************************************
*/
const u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; // 发送的地址
const u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01};

u8 t;
u8 NRF_Flag = 0;
u8 tmp_buf[33];

#define NRF_BYTE0(dwTemp) (*(char *)(&dwTemp))
#define NRF_BYTE1(dwTemp) (*((char *)(&dwTemp) + 1))
#define NRF_BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
#define NRF_BYTE3(dwTemp) (*((char *)(&dwTemp) + 3))

void NRF24L01_Init(void)
{
	u8 key;
	NRF24L01_IO_Init();
	NRF_SPI3_Init();
	//	while(1)
	//		NRF24L01_Check();
	while (NRF24L01_Check())
	{
		OLED_ShowString(0, 0, (void *)"NRF24L01 Error!", 12);
		OLED_ShowString(0, 20, (void *)"Press Back to Break!", 12);
		OLED_Refresh_Gram();
		key = KEY_Scan(0);
		if (key == kb)
			break;
	}
	delay_ms(200);
	OLED_Clear();
	OLED_Refresh_Gram();
}

/*
*****************************************************************************************************************
*                                     void Motor_24L01_Init(void)
*
*Description : 运动控制板24L01通讯初始化
*Arguments   : none
*Returns     : none
*Notes       : none
*******************************************************************************************************************
*/
void NRF24L01_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  // Motor_24L01_CE(PD4)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); // 使能SYSCFG时钟

	GPIO_InitStructure.GPIO_Pin = NRF24L01_CSN_Pin;	   // PD1推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	   // 普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   // 上拉
	GPIO_Init(NRF24L01_CSN_Port, &GPIO_InitStructure); // 初始化PD1

	GPIO_InitStructure.GPIO_Pin = NRF24L01_CE_Pin;	   // PD4推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	   // 普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   // 上拉
	GPIO_Init(NRF24L01_CE_Port, &GPIO_InitStructure);  // 初始化PD4

	GPIO_InitStructure.GPIO_Pin = NRF24L01_IRQ_Pin;	   // PD0上拉输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	   // 输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   // 上拉
	GPIO_Init(NRF24L01_IRQ_Port, &GPIO_InitStructure); // 初始化PD0

	//	GPIO_SetBits(GPIOD,GPIO_Pin_1);//PD1输出1,防止SPI FLASH干扰NRF的通信

	NRF24L01_CE = 0;  // 使能Motor_24L01 片选信号
	NRF24L01_CSN = 1; // Motor_SPI片选取消	 		 	 SPI片选信号
}

/*
*****************************************************************************************************************
*                                     u8 Motor_24L01_Check(void)
*
*Description : 检测Motor_24L01是否存在
*Arguments   : none
*Returns     : 返回值:0，成功;1，失败
*Notes       : none
*******************************************************************************************************************
*/
u8 NRF24L01_Check(void)
{
	u8 buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
	u8 i;
	NRF_SPI3_SetSpeed(SPI_BaudRatePrescaler_8);			 // spi速度为10.5Mhz（24L01的最大SPI时钟为10Mhz）
	NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, buf, 5); // 写入5个字节的地址.
	NRF24L01_Read_Buf(TX_ADDR, buf, 5);					 // 读出写入的地址
	for (i = 0; i < 5; i++)
		if (buf[i] != 0XA5)
			break;
	if (i != 5)
		return 1; // 检测24L01错误
	return 0;	  // 检测到24L01
}

/*
*****************************************************************************************************************
*                                     u8 Motor_24L01_Write_Reg(u8 reg,u8 value)
*
*Description : Motor_SPI写寄存器
*Arguments   : reg:指定寄存器地址; value:写入的值
*Returns     : none
*Notes       : none
*******************************************************************************************************************
*/
u8 NRF24L01_Write_Reg(u8 reg, u8 value)
{
	u8 status;
	NRF24L01_CSN = 0;					  // 使能SPI传输
	status = NRF_SPI3_ReadWriteByte(reg); // 发送寄存器号
	NRF_SPI3_ReadWriteByte(value);		  // 写入寄存器的值
	NRF24L01_CSN = 1;					  // 禁止SPI传输
	return (status);					  // 返回状态值
}

/*
*****************************************************************************************************************
*                                     u8 Motor_24L01_Read_Reg(u8 reg)
*
*Description : 读取Motor_SPI寄存器值
*Arguments   : reg:要读的寄存器
*Returns     : none
*Notes       : none
*******************************************************************************************************************
*/
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;
	NRF24L01_CSN = 0;						// 使能SPI传输
	NRF_SPI3_ReadWriteByte(reg);			// 发送寄存器号
	reg_val = NRF_SPI3_ReadWriteByte(0xff); // 读取寄存器内容
	NRF24L01_CSN = 1;						// 禁止SPI传输
	return (reg_val);						// 返回状态值
}

/*
*****************************************************************************************************************
*                                    u8 Motor_24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
*
*Description : 在指定位置读出指定长度的数据
*Arguments   : reg:寄存器(位置); *pBuf:数据指针; len:数据长度
*Returns     : 此次读到的状态寄存器值
*Notes       : none
*******************************************************************************************************************
*/
u8 NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status, u8_ctr;
	NRF24L01_CSN = 0;					  // 使能SPI传输
	status = NRF_SPI3_ReadWriteByte(reg); // 发送寄存器值(位置),并读取状态值
	for (u8_ctr = 0; u8_ctr < len; u8_ctr++)
		pBuf[u8_ctr] = NRF_SPI3_ReadWriteByte(0xff); // 读出数据
	NRF24L01_CSN = 1;								 // 关闭SPI传输
	return status;									 // 返回读到的状态值
}

/*
*****************************************************************************************************************
*                                    u8 Motor_24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
*
*Description : 在指定位置写指定长度的数据
*Arguments   : reg:寄存器(位置); *pBuf:数据指针; len:数据长度
*Returns     : 此次读到的状态寄存器值
*Notes       : none
*******************************************************************************************************************
*/
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status, u8_ctr;
	NRF24L01_CSN = 0;					  // 使能SPI传输
	status = NRF_SPI3_ReadWriteByte(reg); // 发送寄存器值(位置),并读取状态值
	for (u8_ctr = 0; u8_ctr < len; u8_ctr++)
		NRF_SPI3_ReadWriteByte(*pBuf++); // 写入数据
	NRF24L01_CSN = 1;					 // 关闭SPI传输
	return status;						 // 返回读到的状态值
}

/*
*****************************************************************************************************************
*                                   u8 Motor_24L01_TxPacket(u8 *txbuf)
*
*Description : 启动Motor_24L01发送一次数据
*Arguments   : txbuf:待发送数据首地址
*Returns     : TX_OK:发送完成状况; 0xff:发送失败
*Notes       : none
*******************************************************************************************************************
*/
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
	NRF_SPI3_SetSpeed(SPI_BaudRatePrescaler_8); // spi速度为10.5Mhz（24L01的最大SPI时钟为10Mhz）
	NRF24L01_CE = 0;
	NRF24L01_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH); // 写数据到TX BUF  32个字节
	NRF24L01_CE = 1;										// 启动发送
	while (NRF24L01_IRQ != 0)
		;											 // 等待发送完成
	sta = NRF24L01_Read_Reg(STATUS);				 // 读取状态寄存器的值
	NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, sta); // 清除TX_DS或MAX_RT中断标志
	if (sta & MAX_TX)								 // 达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX, 0xff); // 清除TX FIFO寄存器
		return MAX_TX;
	}
	if (sta & TX_OK) // 发送完成
	{
		return TX_OK;
	}
	return 0xff; // 其他原因发送失败
}

/*
*****************************************************************************************************************
*                                   u8 Motor_24L01_RxPacket(u8 *rxbuf)
*
*Description : 启动NRF24L01接收一次数据
*Arguments   : rxbuf:待接收数据首地址
*Returns     : 0:接收完成状况；其他，错误代码
*Notes       : none
*******************************************************************************************************************
*/
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;
	NRF_SPI3_SetSpeed(SPI_BaudRatePrescaler_8);		 // spi速度为10.5Mhz（24L01的最大SPI时钟为10Mhz）
	sta = NRF24L01_Read_Reg(STATUS);				 // 读取状态寄存器的值
	NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, sta); // 清除TX_DS或MAX_RT中断标志
	if (sta & RX_OK)								 // 接收到数据
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD, rxbuf, RX_PLOAD_WIDTH); // 读取数据
		NRF24L01_Write_Reg(FLUSH_RX, 0xff);					   // 清除RX FIFO寄存器
		return 0;
	}
	return 1; // 没收到任何数据
}

/*
*****************************************************************************************************************
*                                   void Motor_24L01_RX_Mode(void)
*
*Description : 该函数初始化Motor_24L01到RX模式
*Arguments   : 设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
*                          当CE变高后,即进入RX模式,并可以接收数据了
*Returns     : none
*Notes       : none
*******************************************************************************************************************
*/
void NRF24L01_RX_Mode(void)
{
	NRF24L01_CE = 0;
	NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (u8 *)RX_ADDRESS, RX_ADR_WIDTH); // 写RX节点地址

	NRF24L01_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);			  // 使能通道0的自动应答
	NRF24L01_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);		  // 使能通道0的接收地址
	NRF24L01_Write_Reg(NRF_WRITE_REG + RF_CH, 40);				  // 设置RF通信频率
	NRF24L01_Write_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); // 选择通道0的有效数据宽度
	NRF24L01_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);			  // 设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);			  // 配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
	NRF24L01_CE = 1;											  // CE为高,进入接收模式
}

/*
*****************************************************************************************************************
*                                   void Motor_24L01_TX_Mode(void)
*
*Description : 该函数初始化Motor_24L01到TX模式
*Arguments   : 设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
*                          PWR_UP,CRC使能;当CE变高后,即进入RX模式,并可以接收数据了
*                          CE为高大于10us,则启动发送.
*Returns     : none
*Notes       : none
*******************************************************************************************************************
*/
void NRF24L01_TX_Mode(void)
{
	NRF24L01_CE = 0;
	NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, (u8 *)TX_ADDRESS, TX_ADR_WIDTH);	// 写TX节点地址
	NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (u8 *)RX_ADDRESS, RX_ADR_WIDTH); // 设置TX节点地址,主要为了使能ACK

	NRF24L01_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);	  // 使能通道0的自动应答
	NRF24L01_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);  // 使能通道0的接收地址
	NRF24L01_Write_Reg(NRF_WRITE_REG + SETUP_RETR, 0x1a); // 设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	NRF24L01_Write_Reg(NRF_WRITE_REG + RF_CH, 40);		  // 设置RF通道为40
	NRF24L01_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);	  // 设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);	  // 配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	NRF24L01_CE = 1;									  // CE为高,10us后启动发送
}

/*
*****************************************************************************************************************
*                                     void NRF24L01_Com_RX(void)
*
*Description : NRF24L01通讯接收函数
*Arguments   : none
*Returns     : none
*Notes       : none
*******************************************************************************************************************
*/
u32 Rece_Data;

void NRF24L01_Com_RX(void)
{
	u8 Sum;
	OLED_ShowString(0, 0, (void *)"NRF24L01 RX_Mode", 12);
	OLED_Refresh_Gram();

	NRF24L01_RX_Mode();

	OLED_ShowNum(55, 10, Rece_Data, 4, 16); // 显示出来
	OLED_Refresh_Gram();

	if (NRF24L01_RxPacket(tmp_buf) == 0) // 接收到信息
	{
		Sum = 0;
		for (t = 0; t < 7; t++)
			Sum += tmp_buf[t];
		if (tmp_buf[7] == Sum)
		{
			Rece_Data = tmp_buf[2];
			if ((tmp_buf[0] == 0xAA) & (tmp_buf[1] == 0xBB))
				OLED_ShowNum(55, 10, Rece_Data, 4, 16); // 显示出来
			OLED_Refresh_Gram();
		}
	}
}

/*
*****************************************************************************************************************
*                                     void NRF24L01_Com_TX(u32 data)
*
*Description : NRF24L01通讯发送函数
*Arguments   : data ：要发送的数据
*Returns     : none
*Notes       : none
*******************************************************************************************************************
*/
void NRF24L01_Com_TX(u32 data)
{
	u8 Sum, t;

	OLED_ShowString(0, 0, (void *)"NRF24L01 TX_Mode", 12);
	OLED_Refresh_Gram();
	NRF24L01_TX_Mode();

	tmp_buf[0] = 0xAA;
	tmp_buf[1] = 0xBB;
	tmp_buf[2] = NRF_BYTE0(data);
	tmp_buf[3] = NRF_BYTE1(data);
	tmp_buf[4] = NRF_BYTE2(data);
	tmp_buf[5] = NRF_BYTE3(data);
	tmp_buf[6] = NRF_Flag;

	Sum = 0;
	for (t = 0; t < 7; t++)
		Sum += tmp_buf[t]; // 校验求和
	tmp_buf[7] = Sum;
	NRF24L01_TxPacket(tmp_buf); // 开始传输
}

/*
*****************************************************************************************************************
*                                     void NRF24L01_Rece_mpu(void)
*
*Description : NRF24L01通讯接收函数
*Arguments   : none
*Returns     : none
*Notes       : none
*******************************************************************************************************************
*/
void NRF24L01_Rece_mpu(void)
{
	NRF24L01_RX_Mode();

	if (NRF24L01_RxPacket(tmp_buf) == 0) // 接收到信息
	{
		if (tmp_buf[0] == 0xAA && tmp_buf[7] == 0xBB)
		{
			Rece_pitch = (tmp_buf[1]) | (tmp_buf[2] << 8);
			Rece_roll = (tmp_buf[3]) | (tmp_buf[4] << 8);
			Rece_yaw = (tmp_buf[5]) | (tmp_buf[6] << 8);
			mpu_pitch = Rece_pitch / 100.0;
			mpu_roll = Rece_roll / 100.0;
			mpu_yaw = Rece_yaw / 100.0;
		}
	}
}
void NRF24L01_Test(void)
{
	NRF24L01_Init();
	u8 key;
	u8 tx[24] = {1, 2, 3, 4};
	u8 rx[24] = {0};
	OLED_ShowString(12, 0, (void *)"Test_6 24L01", 12);
	OLED_ShowString(12, 16, (void *)"->K1 TX", 12);
	OLED_ShowString(12, 32, (void *)"->K2 RX", 12);
	OLED_Refresh_Gram();

	while (test_flag)
	{
		key = KEY_Scan(0);
		if (key == KEY1_PRES)
		{
			NRF24L01_TX_Mode();
			OLED_Clear();
			OLED_ShowString(12, 0, (void *)"TX MODE", 12);
			OLED_ShowString(12, 16, (void *)"TXING", 12);
			OLED_Refresh_Gram();
			delay_ms(1000);
			OLED_Clear();
			while (test_flag)
			{
				if (NRF24L01_TxPacket(tx) == TX_OK)
				{
					OLED_ShowString(12, 0, (void *)"TX_OK", 12);
					OLED_Refresh_Gram();
					delay_ms(1000);
					OLED_ShowString(12, 0, (void *)"TXING", 12);
					OLED_Refresh_Gram();
				}
			}
		}

		if (key == KEY2_PRES)
		{
			NRF24L01_RX_Mode();
			OLED_Clear();
			OLED_ShowString(12, 0, (void *)"RX MODE", 12);
			OLED_ShowString(12, 16, (void *)"RXING", 12);
			OLED_Refresh_Gram();
			delay_ms(1000);
			OLED_Clear();
			while (test_flag)
			{
				if (NRF24L01_RxPacket(rx) == 0)
				{
					OLED_ShowString(12, 0, (void *)"RX_OK", 12);
					OLED_ShowNum(0, 12, rx[0], 4, 12);
					OLED_ShowNum(0, 24, rx[1], 4, 12);
					OLED_ShowNum(0, 36, rx[2], 4, 12);
					OLED_ShowNum(0, 48, rx[3], 4, 12);
					OLED_Refresh_Gram();
					delay_ms(1000);
					OLED_ShowString(12, 0, (void *)"RXING", 12);
					OLED_Refresh_Gram();
				}
			}
		}
	}
}
