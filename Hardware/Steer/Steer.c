#include "Steer.h"

u32 Steer_PWM1; // 舵机控制脉宽
u32 Steer_PWM2; // 舵机控制脉宽
u32 Steer_PWM3; // 舵机控制脉宽
u32 Steer_PWM4; // 舵机控制脉宽

/*
********************************************************************************************************************
*                  void Steer_Init(void)
*
*Description : 定时器8和定时器9的舵机PWM配置
*Arguments   : 溢出时间为：（arr+1）*（psc+1）/168M
*Returns     : none
*Notes       : arr:自动重装值,psc:时钟预分频数
*							 舵机频率为50Hz
********************************************************************************************************************
*/
void Steer_Init(void)
{
	TIM2_Steer_Init(20000 - 1, 84 - 1); // 50HZ
}

/*
********************************************************************************************************************
*                  void TIM9_Steer_Init(u16 arr,u16 psc)
*
*Description : 定时器5的电机PWM配置
*Arguments   : 溢出时间为：（arr+1）*（psc+1）/84M
*Returns     : none
*Notes       : arr:自动重装值,psc:时钟预分频数
*				TIM8_Steer_Init(1000-1,1680-1);
********************************************************************************************************************
*/
void TIM2_Steer_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);

	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_CtrlPWMOutputs(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

/******************************************************************************
*              void Steer_Angle_Control(int Angle1,int Angle2,int Angle3,int Angle4)
*
*Description : 舵机角度控制
*Arguments   : Which：1 or 2
							 Steer_Angle:舵机旋转角度：-90-90
*Returns     : none              最小值250   中值750 满值1250
*Notes       : none
*******************************************************************************
*/
void Steer_Angle_Control(int Angle1, int Angle2)
{
	if (Angle1 < -135)
		Angle1 = -135;
	if (Angle2 < -135)
		Angle2 = -135;
	if (Angle1 > 135)
		Angle1 = 135;
	if (Angle2 > 135)
		Angle2 = 135;
	Steer_PWM1 = (u32)(Angle1 * 2000 / 270 + 1500);
	Steer_PWM2 = (u32)(Angle2 * 2000 / 270 + 1500);
	TIM2->CCR3 = Steer_PWM1;
	TIM2->CCR4 = Steer_PWM2;
}
