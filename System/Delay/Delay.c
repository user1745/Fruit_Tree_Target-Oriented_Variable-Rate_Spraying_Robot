#include "Delay.h"
#include "sys.h"

static u8 fac_us = 0;

// 初始化延迟函数
// SYSTICK的时钟固定为AHB时钟的1/8
// SYSCLK:系统时钟频率
void delay_init(u8 SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SYSCLK / 8; // 不论是否使用OS,fac_us都需要使用
}

// 延时nus
// nus为要延时的us数.
// 注意:nus的值,不要大于798915us(最大值即2^24/fac_us@fac_us=21)
void delay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD = nus * fac_us;			  // 时间加载
	SysTick->VAL = 0x00;					  // 清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // 开始倒数
	do
	{
		temp = SysTick->CTRL;
	} while ((temp & 0x01) && !(temp & (1 << 16))); // 等待时间到达
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // 关闭计数器
	SysTick->VAL = 0X00;					   // 清空计数器
}

// 延时nms
// nms:0~65535
void delay_ms(u16 nms)
{
	while (nms--)
	{
		delay_us(1000);
	}
}
