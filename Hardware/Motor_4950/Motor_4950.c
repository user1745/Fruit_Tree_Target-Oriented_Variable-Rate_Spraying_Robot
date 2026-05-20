#include "Motor_4950.h"

#define SPEED_BASEA 0		// A轮初始速度
#define SPEED_BASEB 0		// B轮初始速度
#define SPEED_BASEC 0		// C轮初始速度
#define SPEED_BASED 0		// D轮初始速度
int Motor_4950_limit = 999; // 速度最大值
void Motor_4950_IO_Init(void);
void TIM1_Motor_4950_Init(u16 arr, u16 psc);
void Motor_4950_Init(void)
{
	Motor_4950_IO_Init();
	TIM1_Motor_4950_Init(1000, 56);
}

void Motor_4950_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(A_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = AIN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(A_PORT, &GPIO_InitStructure);
	GPIO_SetBits(A_PORT, AIN_PIN);
}

/*
********************************************************************************************************************
*                  void TIM1_Motor_Init(u16 arr,u16 psc)
*
*Description : 定时器1的电机PWM配置
*Arguments   : 溢出时间为：（arr+1）*（psc+1）/168M
*Returns     : none
*Notes       : arr:自动重装值,psc:时钟预分频数
*				TIM1_Motor_Init(1000-1,56-1);			//3KHz
********************************************************************************************************************
*/

void TIM1_Motor_4950_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}

void Motor_4950_Set(int speedA)
{

	if (speedA < -Motor_4950_limit)
		speedA = -Motor_limit;
	if (speedA > Motor_4950_limit)
		speedA = Motor_limit;

	if (speedA >= 0)
	{
		DIRA = 0;
		TIM1->CCR1 = myabs(speedA) + SPEED_BASEA;
	}
	else if (speedA < 0)
	{
		DIRA = 1;
		TIM1->CCR1 = myabs(speedA + 1000) + SPEED_BASEA;
	}
}

void Motor_4950_TEST(void)
{
	Motor_4950_Init();
	Motor_4950_Set(500);
}
