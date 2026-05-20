#include "sys.h"
#include "RS485.h"
#include "Delay.h"

/*
*****************************************************************************************************************
*                     void RS485_Init(void)
*
*Description : RS485通讯初始化
*Arguments   : none
*Returns     : none
*Notes       : none
*******************************************************************************************************************
*/

void RS485_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;				  // 初始化模式设置端口RE
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); // 使能GPIOC时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	   // 普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   // 上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);			   // 初始化GPIO
	GPIO_ResetBits(GPIOD, GPIO_Pin_3);				   // 拉低，默认为接受模式
}

/*
*****************************************************************************************************************
*                     void RS485_Send_Data(uint32_t *BufferSRC, uint32_t BufferSize)
*
*Description : RS485发送函数
*Arguments   : BufferSRC:发送数据存放地址
			   BufferSize:发送数据字节数
*Returns     : none
*Notes       : none
*******************************************************************************************************************
*/

void RS485_Send_Data(uint32_t *BufferSRC, uint32_t BufferSize) // UASRT DMA发送设置
{
	RS485_mode = 1; // 配置485模式为发送模式
	DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);
	DMA1_Stream7->NDTR = BufferSize;		  // 配置BUFFER大小
	DMA1_Stream7->M0AR = (uint32_t)BufferSRC; // 配置地址
	DMA1_Stream7->CR |= DMA_SxCR_EN;		  // 打开DMA,开始发送
	delay_ms(1);
	RS485_mode = 0; // 配置485为接收模式，默认为接收模式
}

/*
主函数调用
u8 send_485_data[10]={51,49,55,0};
key=KEY_Scan(0);
		{
			delay_ms(10);//消抖
			if(key==KEY1_PRES)
				{
					RS485_mode=1;
					OLED_ShowString(0,24,"trans",12);
					OLED_Refresh_Gram();//更新显示到OLED
					while(1)
					{
							for(t=0;t<10;t++)
							{
								USART_SendData(UART5, send_485_data[t]);         //向串口1发送数据
								while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);//等待发送结束
							}
							delay_ms(10);
					}
				}
			else if(key==KEY4_PRES)
				{

					OLED_ShowString(0,24,"receive",12);
					OLED_Refresh_Gram();//更新显示到OLED
					RS485_mode=0;
					while(1)
					{
							OLED_ShowString(0,0,UART5_RX_BUF,12);
							OLED_Refresh_Gram();//更新显示到OLED
							delay_ms(10);
							UART5_RX_STA=0;
					}

				}

			}
*/
