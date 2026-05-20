#include "Drv8825.h"

void DRV8825_Init(void)
{
	DRV8825_IO_Init();
	TIM9_DRV8825_PWM_Init(1000 - 1, 168 - 1);
	DRV8825_Stop();
}

/******************************************************************************************************************
 *                                              DRV8825_IO_Init()
 *
 *Description : 电机 IO初始化
 *Arguments   : none
 *Returns     : none
 *Notes       : none
 *******************************************************************************************************************
 */
void DRV8825_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(DIR_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DIR1_PIN | DIR2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(DIR_PORT, &GPIO_InitStructure);

	GPIO_SetBits(DIR_PORT, DIR1_PIN | DIR2_PIN); // ENA ENB
}

/*
********************************************************************************************************************
*                  void TIM9_DRV8825_PWM_Init(u16 arr,u16 psc)
*
*Description : 定时器9的PWM配置
*Arguments   : 溢出时间为：（arr+1）*（psc+1）/168M
*Returns     : none
*Notes       : arr:自动重装值,psc:时钟预分频数
*			TIM9_DRV8825_PWM_Init(5000-1,168-1);			//200Hz
********************************************************************************************************************
*/
void TIM9_DRV8825_PWM_Init(u16 arr, u16 psc) // PWM初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE); // TIM2时钟使能
	RCC_AHB1PeriphClockCmd(PULSE_RCC, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM9); // PA8复用为TIM2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM9); // PA9复用为TIM2

	GPIO_InitStructure.GPIO_Pin = PULSE1_PIN | PULSE2_PIN; //_A1 _A2 _B1 _B2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(PULSE_PORT, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; /***********解决普通输出时的互补BUG************/
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;

	TIM_OC1Init(TIM9, &TIM_OCInitStructure);
	TIM_OC2Init(TIM9, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM9, ENABLE); // ARPE使能
	TIM_CtrlPWMOutputs(TIM9, ENABLE);
	TIM_Cmd(TIM9, ENABLE); // 使能TIM4
}

void DRV8825_Driver(u8 index, u8 dir) // 方圆电机驱动
{

	if (index == 0) // A
	{
		if (dir == 0)
		{
			STEP_DIR1 = 0;
			TIM_SetCompare1(TIM9, 500);
		}
		else
		{
			STEP_DIR1 = 1;
			TIM_SetCompare1(TIM9, 500);
		}
	}
	else
	{
		if (dir == 1)
		{
			STEP_DIR2 = 0;
			TIM_SetCompare2(TIM9, 500);
		}
		else
		{
			STEP_DIR2 = 1;
			TIM_SetCompare2(TIM9, 500);
		}
	}
}

void DRV8825_Stop() // 方圆电机驱动全关
{
	TIM_SetCompare1(TIM9, 0);
	TIM_SetCompare2(TIM9, 0);
}

// void TIM2_DRV8825_PWM_Init(u16 arr,u16 psc)//PWM初始化
//{
//	GPIO_InitTypeDef         GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef        TIM_OCInitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//TIM2时钟使能
//	RCC_AHB1PeriphClockCmd(PULSE_RCC, ENABLE);

//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2);//PA8复用为TIM2
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM2);//PA9复用为TIM2

//	GPIO_InitStructure.GPIO_Pin = PULSE1_PIN | PULSE2_PIN ;    //_A1 _A2 _B1 _B2
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(PULSE_PORT,&GPIO_InitStructure);

//	TIM_TimeBaseStructure.TIM_Prescaler=psc;
//	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseStructure.TIM_Period=arr;
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

//	TIM_OCInitStructure.TIM_OCIdleState= TIM_OCIdleState_Reset;/***********解决普通输出时的互补BUG************/
//	TIM_OCInitStructure.TIM_OCNIdleState= TIM_OCNIdleState_Reset;
//	TIM_OCInitStructure.TIM_OutputNState= TIM_OutputNState_Disable;
//	TIM_OCInitStructure.TIM_Pulse= 0;

//	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
//	TIM_OC4Init(TIM2, &TIM_OCInitStructure);

//	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);
//	TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);

//	TIM_ARRPreloadConfig(TIM2,ENABLE);                  //ARPE使能
//	TIM_CtrlPWMOutputs(TIM2, ENABLE);
//	TIM_Cmd(TIM2,ENABLE );                           //使能TIM4

//}
