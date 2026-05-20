#include "Usart.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "Ultrasonic.h"
#if 1
#pragma import(__use_no_semihosting)
// 标准库需要的支持函数
struct __FILE
{
	int handle;
};

FILE __stdout;
// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}

// printf调用
int fputc(int ch, FILE *f)
{
	while ((USART3->SR & 0X40) == 0)
		; // 循环发送,直到发送完毕
	USART3->DR = (u8)ch;
	return ch;
}
#endif

void Usart_init(void)
{
#if USART1_EN
	Usart1_init(115200);
#endif

#if USART2_EN
	Usart2_init(115200);
#endif

#if USART3_EN
	Usart3_init(9600);
#endif

#if UART4_EN
	Uart4_init(115200);
#endif

#if LASER
	laser_uart3_init(9600);
#endif

#if USART1_DMA_EN
	USART1_DMA_Init(115200);
#endif

#if USART2_DMA_EN
	USART2_DMA_Init(115200);
#endif

#if USART3_DMA_EN
	USART3_DMA_Init(115200);
#endif

#if UART4_DMA_EN
	UART4_DMA_Init(115200);
#endif

#if UART5_DMA_EN
	RS485_Init();
	UART5_DMA_Init(115200);
#endif
}

// printf函数（u?_printf(char* fmt,...);）
#if USART1_EN
// 串口1,printf 函数
// 确保一次发送数据不超过USART1_MAX_SEND_LEN字节
void u1_printf(char *fmt, ...)
{
	u16 i, j;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)USART1_TX_BUF, fmt, ap);
	va_end(ap);
	i = strlen((const char *)USART1_TX_BUF); // 此次发送数据的长度
	for (j = 0; j < i; j++)					 // 循环发送数据
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;											   // 等待上次传输完成
		USART_SendData(USART1, (uint8_t)USART1_TX_BUF[j]); // 发送数据到串口3
	}
}
#endif

#if USART2_EN
// 串口2,printf 函数
// 确保一次发送数据不超过USART2_MAX_SEND_LEN字节
void u2_printf(char *fmt, ...)
{
	u16 i, j;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)USART2_TX_BUF, fmt, ap);
	va_end(ap);
	i = strlen((const char *)USART2_TX_BUF); // 此次发送数据的长度
	for (j = 0; j < i; j++)					 // 循环发送数据
	{
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;											   // 等待上次传输完成
		USART_SendData(USART2, (uint8_t)USART2_TX_BUF[j]); // 发送数据到串口3
	}
}
#endif

#if USART3_EN
// 串口3,printf 函数
// 确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u3_printf(char *fmt, ...)
{
	u16 i, j;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)USART3_TX_BUF, fmt, ap);
	va_end(ap);
	i = strlen((const char *)USART3_TX_BUF); // 此次发送数据的长度
	for (j = 0; j < i; j++)					 // 循环发送数据
	{
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;											   // 等待上次传输完成
		USART_SendData(USART3, (uint8_t)USART3_TX_BUF[j]); // 发送数据到串口3
	}
}
#endif

#if UART4_EN
// 串口4,printf 函数
// 确保一次发送数据不超过USART4_MAX_SEND_LEN字节
void u4_printf(char *fmt, ...)
{
	u16 i, j;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)UART4_TX_BUF, fmt, ap);
	va_end(ap);
	i = strlen((const char *)UART4_TX_BUF); // 此次发送数据的长度
	for (j = 0; j < i; j++)					// 循环发送数据
	{
		while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET)
			;											 // 等待上次传输完成
		USART_SendData(UART4, (uint8_t)UART4_TX_BUF[j]); // 发送数据到串口4
	}
}
#endif

// 串口初始化函数和中断函数
#if USART1_EN
/*
***********************************************************************************************************************
*******************************************            USART1         *************************************************
***********************************************************************************************************************
*/
__align(8) u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; // 发送缓冲,最大USART1_MAX_SEND_LEN字节
#if USART1_RX_EN								  // 如果使能了接收

// 接收状态
// bit15，	接收完成标志
// bit14，	接收到0x0d
// bit13~0，	接收到的有效字节数目
u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; // 接收缓冲,最大USART_REC_LEN个字节.
u16 USART1_RX_STA = 0;				   // 接收状态标记

/*
********************************************************************************************************************
*                  void uart1_init(u32 bound)
*
*Description : USART1初始化
*Arguments   : bound为波特率
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void Usart1_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  // 使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // 使能USART1时钟

	// 串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1); // GPIOB6复用为USART1
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1); // GPIOB7复用为USART1

	// USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // GPIOB6与GPIOB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		   // 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   // 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   // 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   // 上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);				   // 初始化PB6,PB7

	// USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;										// 波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// 字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// 一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// 收发模式
	USART_Init(USART1, &USART_InitStructure);										// 初始化串口1

	USART_Cmd(USART1, ENABLE); // 使能串口1

	// USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 开启相关中断

	// Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		  // 串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  // 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  // 根据指定的参数初始化VIC寄存器、
}

/*
********************************************************************************************************************
*                  void USART1_IRQHandler(void)
*
*Description : 串口1中断服务程序
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/

void USART1_IRQHandler(void) // 串口1中断服务程序
{
	u8 com_data;
	// 接收中断
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 清除中断标志
		com_data = USART1->DR;
		Openmv_Wide_RX(com_data); // openmv数据处理函数
	}
}
#endif

void USART1_send_byte(uint8_t byte)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		; // 等待发送完成
	USART1->DR = byte;
}
#endif

#if USART2_EN
/*
***********************************************************************************************************************
*******************************************            USART2         *************************************************
***********************************************************************************************************************
*/
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; // 发送缓冲,最大USART2_MAX_SEND_LEN字节
#if USART2_RX_EN								  // 如果使能了接收

// 接收状态
// bit15，	接收完成标志
// bit14，	接收到0x0d
// bit13~0，	接收到的有效字节数目
// u8  USART2_RX_BUF[USART2_MAX_RECV_LEN];   			//接收缓冲,最USART2_MAX_RECV_LEN个字节.
// u16 USART2_RX_STA=0;       							//接收状态标记

/*
********************************************************************************************************************
*                  void uart2_init(u32 bound)
*
*Description : USART2初始化
*Arguments   : bound为波特率
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;
int16_t Cx = 0, Cy = 0;
int16_t Dx = 0, Dy = 0;
int16_t Rx = 0, Ry = 0;
int16_t top_error, down_error;
extern float Angle1, Angle2;

void Usart2_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  // 使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // 使能USART2时钟

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2); // GPIOD5复用为USART2
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2); // GPIOD6复用为USART2

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; // GPIOD5与GPIOD6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		   // 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   // 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   // 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   // 上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);				   // 初始化PD5，PD6

	USART_InitStructure.USART_BaudRate = bound;										// 波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// 字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// 一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// 收发模式
	USART_Init(USART2, &USART_InitStructure);										// 初始化串口2
	USART_ClearFlag(USART2, USART_IT_RXNE);
	USART_Cmd(USART2, ENABLE); // 使能串口2
#if USART2_RX_EN
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 开启相关中断

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		  // 串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  // 子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  // 根据指定的参数初始化NVIC寄存器
#endif
}
/*
********************************************************************************************************************
*                  void USART2_IRQHandler(void)
*
*Description : 串口2中断服务程序
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/

void USART2_IRQHandler(void)
{
	u8 com_data;
	u8 i;

	static u8 RxCounter1 = 0;
	static u16 RxBuffer1[16] = {0};
	static u8 RxState = 0;
	static u8 RxFlag1 = 0;

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // 接收中断
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE); // 清除中断标志
		com_data = USART_ReceiveData(USART2);

		if (RxState == 0 && com_data == 0x2C) // 0x2c帧头
		{
			RxBuffer1[RxCounter1++] = com_data;
			RxState = 1;
		}

		else if (RxState == 1 && com_data == 0x12) // 0x12帧头
		{
			RxBuffer1[RxCounter1++] = com_data;
			RxState = 2;
		}
		else if (RxState == 2)
		{
			RxBuffer1[RxCounter1++] = com_data;

			if (RxCounter1 >= 12 || com_data == 0x5B) // RxBuffer1接受满了,接收数据结束
			{
				RxState = 3;
				RxFlag1 = 1;
				Rx = RxBuffer1[2] + RxBuffer1[3] * 256;
				Ry = RxBuffer1[4] + RxBuffer1[5] * 256;
				Dx = RxBuffer1[6] + RxBuffer1[7] * 256;
				Dy = RxBuffer1[8] + RxBuffer1[9] * 256;
				Cx = RxBuffer1[10] + RxBuffer1[11] * 256;
				Cy = RxBuffer1[12] + RxBuffer1[13] * 256;
			}
		}

		else if (RxState == 3) // 检测是否接受到结束标志
		{
			if (RxBuffer1[RxCounter1 - 1] == 0x5B)
			{
				USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); // 关闭DTSABLE中断
				if (RxFlag1)
				{
					// 计算偏差，被减数是openmv所设置的分辨率中的w或h的二分之一，也就是计算到色块到中心的坐标偏差。IDE右侧帧缓冲区的下方文字有写。
					down_error = Rx - Dx;
					top_error = Ry - Dy;
					RxFlag1 = 0;
					RxCounter1 = 0;
					RxState = 0;
				}
				USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
			}
			else // 接收错误
			{
				RxState = 0;
				RxCounter1 = 0;
				for (i = 0; i < 16; i++)
				{
					RxBuffer1[i] = 0x00; // 将存放数据数组清零
				}
			}
		}

		else // 接收异常
		{
			RxState = 0;
			RxCounter1 = 0;
			for (i = 0; i < 16; i++)
			{
				RxBuffer1[i] = 0x00; // 将存放数据数组清零
			}
		}
	}
}

#endif
#endif

#if USART3_EN
/*
***********************************************************************************************************************
*******************************************            USART3         *************************************************
***********************************************************************************************************************
*/
__align(8) u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; // 发送缓冲,最大USART3_MAX_SEND_LEN字节
#if USART3_RX_EN								  // 如果使能了接收

// 接收状态
// bit15，	接收完成标志
// bit14，	接收到0x0d
// bit13~0，	接收到的有效字节数目
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; // 接收缓冲,最大USART_REC_LEN个字节.
u16 USART3_RX_STA = 0;				   // 接收状态标记

/*
********************************************************************************************************************
*                  void uart3_init(u32 bound)
*
*Description : USART3初始化
*Arguments   : bound为波特率
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void Usart3_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  // 使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); // 使能USART3时钟

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3); // GPIOD8复用为USART3
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3); // GPIOD9复用为USART3

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; // GPIOD8与GPIOD9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		   // 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   // 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   // 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   // 上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);				   // 初始化PD8，D9

	USART_InitStructure.USART_BaudRate = bound;										// 波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// 字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// 一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// 收发模式
	USART_Init(USART3, &USART_InitStructure);										// 初始化串口3

	USART_Cmd(USART3, ENABLE); // 使能串口3

#if USART3_RX_EN
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // 开启相关中断

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		  // 串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // 抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // 子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  // 根据指定的参数初始化NVIC寄存器

#endif
}

/*
********************************************************************************************************************
*                  void USART3_IRQHandler(void)
*
*Description : 串口3中断服务程序
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
u8 data3_to_send[50] = {0xaa, 0xbb, 0, 0x0d, 0x0a};
u8 data3_to_send2[50] = {0xaa, 0xbb, 1, 0x0d, 0x0a};
u8 data3_to_send3[50] = {0xaa, 0xbb, 3, 0x0d, 0x0a};
void Usart3_Send(unsigned char length, int k)
{

	unsigned char i;
	for (i = 0; i < length; i++)

	{

		if (k == 0)
		{
			USART_SendData(USART3, data3_to_send[i]);
			while (USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET)
				; // 等待发送结束
		}

		if (k == 1)
		{
			USART_SendData(USART3, data3_to_send2[i]);
			while (USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET)
				; // 等待发送结束
		}

		if (k == 3)
		{
			USART_SendData(USART3, data3_to_send3[i]);
			while (USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET)
				; // 等待发送结束
		}
	}
}

void USART3_IRQHandler(void)
{
	u8 Res;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // 接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART3); //(USART2->DR);	//读取接收到的数据

		if ((USART3_RX_STA & 0X8000) == 0) // 接收未完成
		{
			if (USART3_RX_STA & 0X4000) // 已经接收到了0x0d
			{
				if (Res != 0X0A)
					USART3_RX_STA = 0; // 接收错误,重新开始
				else
				{
					USART3_RX_STA |= 0x8000; // 接收完成了
					if (USART3_RX_BUF[0] == 0xAA && USART3_RX_BUF[1] == 0xBB)
					{
						pos_x = USART3_RX_BUF[2] * 256 + USART3_RX_BUF[3]; // 摄像头中的X
						pos_y = USART3_RX_BUF[4] * 256 + USART3_RX_BUF[5]; // 摄像头中的Y
					}
					USART3_RX_STA = 0;
				}
			}
			else // 还没收到0X0D
			{
				if (Res == 0X0D)
					USART3_RX_STA |= 0X4000;
				else
				{
					USART3_RX_BUF[USART3_RX_STA & 0X3FFF] = Res;
					USART3_RX_STA++;
					if (USART3_RX_STA > (USART3_MAX_RECV_LEN - 1))
						USART3_RX_STA = 0; // 接收数据错误,重新开始接收
				}
			}
		}
	}
}
// void USART3_IRQHandler(void)                	//串口1中断服务程序
//{
//	u8 pc_data;
//   	//接收中断
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET )
//	{
//		USART_ClearITPendingBit(USART3,USART_IT_RXNE);//清除中断标志
//		pc_data = USART3->DR;
////		printf("get%d\r\n",pc_data);
//		dataprocess(pc_data);

////		if(pc_data == 8)
////			LED1 = 1;
////		else if(pc_data == 5)
////			LED1 = 0;
////		else if(pc_data == 1)
////			LED2 = 1;
////		else if(pc_data == 2)
////			LED2 = 0;

////		OLED_Show3FNum(55,12,pc_data,5,2,12);
////		OLED_Refresh_Gram();
//	}
//}
#endif
#endif

#if UART4_EN
/*
***********************************************************************************************************************
*******************************************            UART4         *************************************************
***********************************************************************************************************************
*/
__align(8) u8 UART4_TX_BUF[UART4_MAX_SEND_LEN]; // 发送缓冲,最大USART3_MAX_SEND_LEN字节
#if UART4_RX_EN									// 如果使能了接收

// 接收状态
// bit15，	接收完成标志
// bit14，	接收到0x0d
// bit13~0，	接收到的有效字节数目
u8 UART4_RX_BUF[UART4_MAX_RECV_LEN]; // 接收缓冲,最大USART_REC_LEN个字节.
u16 UART4_RX_STA = 0;				 // 接收状态标记

/*
********************************************************************************************************************
*                  void uart3_init(u32 bound)
*
*Description : USART3初始化
*Arguments   : bound为波特率
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void Uart4_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // 使能GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); // 使能UART4时钟

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4); // GPIOC10复用为USART1
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4); // GPIOC11复用为USART1

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; // GPIOC10与GPIOC11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			 // 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			 // 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			 // 上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 // 初始化PC10，PC11

	USART_InitStructure.USART_BaudRate = bound;										// 波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// 字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// 一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// 收发模式
	USART_Init(UART4, &USART_InitStructure);										// 初始化串口4

	USART_Cmd(UART4, ENABLE); // 使能串口4

#if UART4_RX_EN
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); // 开启相关中断

	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;		  // 串口4中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  // 子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  // 根据指定的参数初始化VIC寄存器、

#endif
}

/*
********************************************************************************************************************
*                  void UART4_IRQHandler(void)
*
*Description : 串口4中断服务程序
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void UART4_IRQHandler(void)
{
	u8 com_data;
	// 接收中断
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_RXNE); // 清除中断标志
		com_data = UART4->DR;
		Openmv_Narrow_RX(com_data); // openmv数据处理函数
	}
}
#endif
#endif

//=================================================================================================================================
//======================================以下为匿名上位机============================================================================
//=================================================================================================================================
#if Niming2
/*
****************************************************************************************************************************
***************************************匿名2.6数据传输协议******************************************************************
****************************************************************************************************************************
*/

void usart_send_char(u8 c)
{
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
		;
	USART_SendData(USART2, c);
}

/*
********************************************************************************************************************
*                  void usart1_niming_report(u8 fun,u8*data,u8 len)
*
*Description : 传送数据给匿名四轴上位机软件(V2.6版本)
*Arguments   : none
*Returns     : none
*Notes       : fun:功能字. 0XA0~0XAF
							 data:数据缓存区,最多28字节
							 len:data区有效数据个数
********************************************************************************************************************
*/
void usart_niming_report(u8 fun, u8 *data, u8 len)
{
	u8 send_buf[32];
	u8 i;
	if (len > 28)
		return;			   // 最多28字节数据
	send_buf[len + 3] = 0; // 校验数置零
	send_buf[0] = 0X88;	   // 帧头
	send_buf[1] = fun;	   // 功能字
	send_buf[2] = len;	   // 数据长度
	for (i = 0; i < len; i++)
		send_buf[3 + i] = data[i]; // 复制数据
	for (i = 0; i < len + 3; i++)
		send_buf[len + 3] += send_buf[i]; // 计算校验和
	for (i = 0; i < len + 4; i++)
		usart_send_char(send_buf[i]); // 发送数据到串口1
}

/*
********************************************************************************************************************
*    void NM16_send_data(s16 u_a,s16 u_b,s16 u_c,s16 u_d,s16 u_e,s16 u_f,s16 u_g,s16 u_h,s16 u_i,s16 u_j)
*
*Description : 发送10个 short型 （16位）给匿名上位机
*Arguments   : none
*Returns     : none
*Notes       : 数据最多16位（-32768——+32767）
********************************************************************************************************************
*/
void NM16_send_data(s16 u_a, s16 u_b, s16 u_c, s16 u_d, s16 u_e, s16 u_f, s16 u_g, s16 u_h, s16 u_i, s16 u_j)
{
	u8 tbuf[28];
	tbuf[0] = (u_a >> 8) & 0XFF;
	tbuf[1] = u_a & 0XFF;
	tbuf[2] = (u_b >> 8) & 0XFF;
	tbuf[3] = u_b & 0XFF;
	tbuf[4] = (u_c >> 8) & 0XFF;
	tbuf[5] = u_c & 0XFF;
	tbuf[6] = (u_d >> 8) & 0XFF;
	tbuf[7] = u_d & 0XFF;
	tbuf[8] = (u_e >> 8) & 0XFF;
	tbuf[9] = u_e & 0XFF;
	tbuf[10] = (u_f >> 8) & 0XFF;
	tbuf[11] = u_f & 0XFF;
	tbuf[12] = (u_g >> 8) & 0XFF;
	tbuf[13] = u_g & 0XFF;
	tbuf[14] = (u_h >> 8) & 0XFF;
	tbuf[15] = u_h & 0XFF;
	tbuf[16] = (u_i >> 8) & 0XFF;
	tbuf[17] = u_i & 0XFF;
	tbuf[18] = (u_j >> 8) & 0XFF;
	tbuf[19] = u_j & 0XFF;
	usart_niming_report(0XA1, tbuf, 28); // 自定义帧,0XA1
}

/*
***************************************************************************************************************
*    void NM32_send_data(s32 ch1,s32 ch2,s32 ch3,s32 ch4)
*
*Description : 发送4个 int型 （32位）给匿名上位机
*Arguments   : none
*Returns     : none
*Notes       : 数据最多32位(其实只有28位)
***************************************************************************************************************
*/
void NM32_send_data(s32 ch1, s32 ch2, s32 ch3, s32 ch4)
{
	u8 tbuf[16];
	tbuf[0] = (ch1 >> 24) & 0XFF;
	tbuf[1] = (ch1 >> 16) & 0XFF;
	tbuf[2] = (ch1 >> 8) & 0XFF;
	tbuf[3] = ch1 & 0XFF;
	tbuf[4] = (ch2 >> 24) & 0XFF;
	tbuf[5] = (ch2 >> 16) & 0XFF;
	tbuf[6] = (ch2 >> 8) & 0XFF;
	tbuf[7] = ch2 & 0XFF;
	tbuf[8] = (ch3 >> 24) & 0XFF;
	tbuf[9] = (ch3 >> 16) & 0XFF;
	tbuf[10] = (ch3 >> 8) & 0XFF;
	tbuf[11] = ch3 & 0XFF;
	tbuf[12] = (ch4 >> 24) & 0XFF;
	tbuf[13] = (ch4 >> 16) & 0XFF;
	tbuf[14] = (ch4 >> 8) & 0XFF;
	tbuf[15] = ch4 & 0XFF;
	usart_niming_report(0XA1, tbuf, 16); // 自定义帧,0XA1
}
#endif

#if Niming4
/*
****************************************************************************************************************
*****************************************匿名4.34数据传输协议****************************************************
****************************************************************************************************************
*/

u8 data_to_send[50] = {0x11};
u8 data_to_send2[50] = {2};
u8 data_to_send3[50] = {3};
void Usart_Send(unsigned char length)
{
	unsigned char i;
	for (i = 0; i < length; i++)
	{
		if (KEY1 == 0)
		{
			USART_SendData(USART2, data_to_send[i]);
			while (USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET)
				; // 等待发送结束
		}

		if (KEY2 == 0)
		{
			USART_SendData(USART2, data_to_send2[i]);
			while (USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET)
				; // 等待发送结束
		}

		if (KEY3 == 0)
		{
			USART_SendData(USART2, data_to_send3[i]);
			while (USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET)
				; // 等待发送结束
		}
	}
}

/*
***************************************************************************************************************
*   void Usart_Send_float(float d1,float d2,float d3,float d4)
*
*Description : 发送8个 int型 （32位）给匿名上位机
*Arguments   : none
*Returns     : none
*Notes       : 数据最多32位(其实只有28位)
***************************************************************************************************************
*/
void Usart_Send_float(float d1, float d2, float d3, float d4)
{
	u8 _cnt = 0;
	u8 sum = 0;
	u8 i;

	data_to_send[_cnt++] = 0xAA;
	data_to_send[_cnt++] = 0xAA;
	data_to_send[_cnt++] = 0xF1;
	data_to_send[_cnt++] = 0;

	data_to_send[_cnt++] = BYTE3(d1);
	data_to_send[_cnt++] = BYTE2(d1);
	data_to_send[_cnt++] = BYTE1(d1);
	data_to_send[_cnt++] = BYTE0(d1);

	data_to_send[_cnt++] = BYTE3(d2);
	data_to_send[_cnt++] = BYTE2(d2);
	data_to_send[_cnt++] = BYTE1(d2);
	data_to_send[_cnt++] = BYTE0(d2);

	data_to_send[_cnt++] = BYTE3(d3);
	data_to_send[_cnt++] = BYTE2(d3);
	data_to_send[_cnt++] = BYTE1(d3);
	data_to_send[_cnt++] = BYTE0(d3);

	data_to_send[_cnt++] = BYTE3(d4);
	data_to_send[_cnt++] = BYTE2(d4);
	data_to_send[_cnt++] = BYTE1(d4);
	data_to_send[_cnt++] = BYTE0(d4);

	data_to_send[3] = _cnt - 4;

	for (i = 0; i < _cnt; i++)
		sum += data_to_send[i];

	data_to_send[_cnt++] = sum;

	Usart_Send(_cnt);
}

/*
***************************************************************************************************************
*    void Usart_Send_User(s32 d1,s32 d2,s32 d3,s32 d4,s32 d5,s32 d6,s32 d7,s32 d8)
*
*Description : 发送8个 int型 （32位）给匿名上位机
*Arguments   : none
*Returns     : none
*Notes       : 数据最多32位(其实只有28位)
***************************************************************************************************************
*/
void Usart_Send_User(s32 d1, s32 d2, s32 d3, s32 d4, s32 d5, s32 d6, s32 d7, s32 d8)
{
	u8 _cnt = 0;
	u8 sum = 0;
	u8 i;

	data_to_send[_cnt++] = 0xAA;
	data_to_send[_cnt++] = 0xAA;
	data_to_send[_cnt++] = 0xF1;
	data_to_send[_cnt++] = 0;

	data_to_send[_cnt++] = BYTE3(d1);
	data_to_send[_cnt++] = BYTE2(d1);
	data_to_send[_cnt++] = BYTE1(d1);
	data_to_send[_cnt++] = BYTE0(d1);

	data_to_send[_cnt++] = BYTE3(d2);
	data_to_send[_cnt++] = BYTE2(d2);
	data_to_send[_cnt++] = BYTE1(d2);
	data_to_send[_cnt++] = BYTE0(d2);

	data_to_send[_cnt++] = BYTE3(d3);
	data_to_send[_cnt++] = BYTE2(d3);
	data_to_send[_cnt++] = BYTE1(d3);
	data_to_send[_cnt++] = BYTE0(d3);

	data_to_send[_cnt++] = BYTE3(d4);
	data_to_send[_cnt++] = BYTE2(d4);
	data_to_send[_cnt++] = BYTE1(d4);
	data_to_send[_cnt++] = BYTE0(d4);

	data_to_send[_cnt++] = BYTE3(d5);
	data_to_send[_cnt++] = BYTE2(d5);
	data_to_send[_cnt++] = BYTE1(d5);
	data_to_send[_cnt++] = BYTE0(d5);

	data_to_send[_cnt++] = BYTE3(d6);
	data_to_send[_cnt++] = BYTE2(d6);
	data_to_send[_cnt++] = BYTE1(d6);
	data_to_send[_cnt++] = BYTE0(d6);

	data_to_send[_cnt++] = BYTE3(d7);
	data_to_send[_cnt++] = BYTE2(d7);
	data_to_send[_cnt++] = BYTE1(d7);
	data_to_send[_cnt++] = BYTE0(d7);

	data_to_send[_cnt++] = BYTE3(d8);
	data_to_send[_cnt++] = BYTE2(d8);
	data_to_send[_cnt++] = BYTE1(d8);
	data_to_send[_cnt++] = BYTE0(d8);

	data_to_send[3] = _cnt - 4;

	for (i = 0; i < _cnt; i++)
		sum += data_to_send[i];

	data_to_send[_cnt++] = sum;

	Usart_Send(_cnt);
}
#endif
