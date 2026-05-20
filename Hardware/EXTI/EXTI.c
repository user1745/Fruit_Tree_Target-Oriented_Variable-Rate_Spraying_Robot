#include "KEY.h"
#include "EXTI.h"
#include "Delay.h"

// 外部中断初始化程序
// 初始化PE2~4,PA0为中断输入.
void EXTI_kb_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);		  // 使能SYSCFG时钟
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3); // PE3 连接到中断线3

	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		// 中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				// 中断线使能
	EXTI_Init(&EXTI_InitStructure);							// 配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;		  // 外部中断4
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; // 抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		  // 子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // 使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);							  // 配置
}

// 外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	delay_ms(10); // 消抖
	if (KEY4 == 0 && menu_level > 1)
	{
		menu_level--;
	}
	EXTI_ClearITPendingBit(EXTI_Line3); // 清除LINE4上的中断标志位
}
