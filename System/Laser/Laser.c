#include "sys.h"
#include "Laser.h"
#include "string.h"
#include "OLED.h"

#if LASER // 如果使能了接收
// 串口1中断服务程序
u8 USART3_RX_BUF[USART3_REC_LEN] = {0}; // 接收缓冲,最大USART_REC_LEN个字节.
// 接收状态
// bit15，	接收完成标志
// bit14，	接收到0x0d
// bit13~0，	接收到的有效字节数目
u16 USART3_RX_STA = 0; // 接收状态标记

// 初始化IO 串口1
// bound:波特率
void laser_uart3_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  // 使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); // 使能USART1时钟

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3); // GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3); // GPIOA10复用为USART1

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; // GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		   // 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   // 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   // 推挽复用输 出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   // 上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);				   // 初始化PA9，PA10

	USART_InitStructure.USART_BaudRate = bound;										// 波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// 字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// 一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// 收发模式
	USART_Init(USART3, &USART_InitStructure);										// 初始化串口1

	USART_Cmd(USART3, ENABLE); // 使能串口1

#if LASER
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // 开启相关中断

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		  // 串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  // 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  // 根据指定的参数初始化VIC寄存器、
#endif

	send_com(0x45);
}

//*******************激光************************************
u8 Receive_ok = 0;
void USART3_IRQHandler(void)
{
	static uint8_t i = 0, rebuf[20] = {0};
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // 判断接收标志
	{
		rebuf[i++] = USART_ReceiveData(USART3); // 读取串口数据，同时清接收标志
		if (rebuf[0] != 0x5a)					// 帧头不对
			i = 0;
		if ((i == 2) && (rebuf[1] != 0x5a)) // 帧头不对
			i = 0;

		if (i > 3) // i等于4时，已经接收到数据量字节rebuf[3]
		{
			if (i != (rebuf[3] + 5)) // 判断是否接收一帧数据完毕
				return;				 // 直接就返回了，下面的代码不会再执行
			switch (rebuf[2])		 // 接收完毕后处理
			{
			case 0x15:
				if (!Receive_ok) // 当数据处理完成后才接收新的数据
				{
					memcpy(USART3_RX_BUF, rebuf, 8); // 拷贝接收到的数据
					Receive_ok = 1;					 // 接收完成标志
				}
				break;
			}
			i = 0; // 缓存清0
		}
	}
}

void USART3_send_byte(uint8_t byte)
{
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
		; // 等待发送完成
	USART3->DR = byte;
}

// 发送多字节数据
void USART3_Send_bytes(uint8_t *Buffer, uint8_t Length)
{
	uint8_t i = 0;
	while (i < Length)
	{
		USART3_send_byte(Buffer[i++]);
	}
}
// 发送多字节数据+校验和
void USART3_Send(uint8_t *Buffer, uint8_t Length)
{
	uint8_t i = 0;
	while (i < Length)
	{
		if (i < (Length - 1))
			Buffer[Length - 1] += Buffer[i]; // 累加Length-1前的数据
		USART3_send_byte(Buffer[i++]);
	}
}

void send_com(u8 data)
{
	u8 bytes[3] = {0};
	bytes[0] = 0xa5;
	bytes[1] = data;	   // 功能字节
	USART3_Send(bytes, 3); // 发送帧头、功能字节、校验和
}

/************************************************************
 *  测距函数
 *  注意：在初始化完成必须要添加写命令 如：send_com(0x45);
 *   0x45: 连续输出距离数据
 *   0x50: 长距离测量模式
 *   0x51：快速测量模式
 *   0x52：高精度测量模式
 *   0x53: 一般测量模式
 ************************************************************/
void Mea_dis(void)
{
	u8 sum = 0, i = 0;
	uint16_t distance = 0;
	double dis = 0.0;
	if (Receive_ok) // 串口接收完毕
	{
		for (sum = 0, i = 0; i < (USART3_RX_BUF[3] + 4); i++) // rgb_data[3]=3
			sum += USART3_RX_BUF[i];
		if (sum == USART3_RX_BUF[i]) // 校验和判断
		{
			distance = USART3_RX_BUF[4] << 8 | USART3_RX_BUF[5];
			Receive_ok = 0; // 处理数据完毕标志
			dis = distance / 10.0;
			OLED_ShowNum(0, 0, dis, 8, 12);
			OLED_Refresh_Gram();
		}
	}
}
#endif
