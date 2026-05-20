#include "Step.h"

u16 capture = 0;
vu32 CCR1_Val = 500; // f = 168M/84/(500*2) = 2khz
vu32 CCR2_Val = 250;
vu32 CCR3_Val = 1000;
vu32 CCR4_Val = 200;
int pulsesA = 0;					  // A已走的脉冲数
int angleA = 10 * (200 * DIVIDE) * 2; // A设定转的角度      1个脉冲1.8°/8       360/1.8/8/2
int stateA = 0;						  // 步进A的状态

int pulsesB = 0;					 // B已走的脉冲数
int angleB = 1 * (200 * DIVIDE) * 2; // B设定转的角度      1个脉冲1.8°/8       360/1.8/8/2
int stateB = 0;						 // 步进B的状态

int pulsesC = 0;					  // C已走的脉冲数
int angleC = 10 * (200 * DIVIDE) * 2; // C设定转的角度      1个脉冲1.8°/8       360/1.8/8/2
int stateC = 0;						  // 步进B的状态

int pulsesD = 0;					  // D已走的脉冲数
int angleD = 10 * (200 * DIVIDE) * 2; // D设定转的角度      1个脉冲1.8°/8       360/1.8/8/2
int stateD = 0;						  // 步进D的状态
void Step_IO_Init(void);
void TIM1_Step_Init(u16 arr, u16 psc);
void Step_init(void)
{
	Step_IO_Init();
	TIM1_Step_Init(65535, 84 - 1);
}

void Step_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(SA_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SENA_PIN | SDIRA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(SA_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(SA_PORT, SENA_PIN | SDIRA_PIN);

	RCC_AHB1PeriphClockCmd(SB_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SENB_PIN | SDIRB_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(SB_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(SB_PORT, SENB_PIN | SDIRB_PIN);

	RCC_AHB1PeriphClockCmd(SC_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SENC_PIN | SDIRC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(SC_PORT, SENC_PIN | SDIRC_PIN);

	RCC_AHB1PeriphClockCmd(SD_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SEND_PIN | SDIRD_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(SD_PORT, SEND_PIN | SDIRD_PIN);
}

void TIM1_Step_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// GPIO初始化
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

	// 定时器初始化
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					// 定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseStructure.TIM_Period = arr;						// 自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); // 初始化定时器1

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;			  // PWM模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 正向通道有效
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;	  // 输出极性

	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);

	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);

	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Disable);

	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Disable);

	// 中断初始化
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearFlag(TIM1, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4);
	TIM_ITConfig(TIM1, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);

	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

// 步进驱动函数
void Step_Driver(u8 index, u8 dir)
{
	if (index == STEPA)
	{
		SDIRA = dir;
		SENA = 1;
		stateA = 1;
	}
	if (index == STEPB)
	{
		SDIRB = dir;
		SENB = 1;
		stateB = 1;
	}
	if (index == STEPC)
	{
		SDIRC = dir;
		SENC = 1;
		stateC = 1;
	}
	if (index == STEPD)
	{
		SDIRD = dir;
		SEND = 1;
	}
}

// 步进停止函数
void Step_Stop(u8 index)
{
	if (index == STEPA)
	{
		SENA = 0;
		stateA = 0;
	}
	if (index == STEPB)
	{
		SENB = 0;
		stateB = 0;
	}
	if (index == STEPC)
	{
		SENC = 0;
		stateC = 0;
	}
	if (index == STEPD)
	{
		SEND = 0;
	}
}

void Step_TEST(void)
{
	OLED_Clear();
	OLED_ShowString(16, 0, "Step_TEST", 12);
	OLED_ShowString(0, 16, "KEY1->fast KEY5->slow", 12);
	OLED_ShowString(0, 32, "KEY2->stop KEY3->run", 12);
	OLED_ShowString(0, 48, "KEY6->step", 12);
	OLED_Refresh_Gram();
	u8 key = 0;
	u8 index = 0;
	int CCR_Val = 500;
	Step_init();
	while (1)
	{
		key = KEY_Scan(0);
		if (key == KEY1_PRES)
			CCR_Val = CCR_Val / 2;
		else if (key == KEY2_PRES)
			Step_Stop(index);
		else if (key == KEY3_PRES)
			//		Step_Driver(index,1);
			Step_control(index, 1, 2 * (200 * DIVIDE) * 2);
		else if (key == KEY5_PRES)
			CCR_Val = CCR_Val + 50;
		else if (key == KEY6_PRES)
			index = (index + 1) % 4;

		if (CCR_Val <= 1)
			CCR_Val = 1;
		CCR1_Val = CCR_Val;
		CCR2_Val = CCR_Val;
		CCR3_Val = CCR_Val;
		CCR4_Val = CCR_Val;
		OLED_ShowNum(64, 48, index, 2, 12);
		OLED_Show3FNum(90, 48, 1000 / (CCR_Val * 1.0), 2, 2, 12);
		OLED_ShowNum(90, 0, CCR_Val, 4, 12);
		OLED_Refresh_Gram();
	};
}

void TIM1_CC_IRQHandler(void)
{
	uint32_t vlue;
	if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
	{
		vlue = TIM_GetCapture1(TIM1);
		TIM_SetCompare1(TIM1, vlue + CCR1_Val);
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
		if (stateA == 1)
		{
			pulsesA++;
			if (pulsesA >= angleA)
			{
				Step_Stop(STEPA);
				pulsesA = 0;
			}
		}
	}

	if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
	{
		vlue = TIM_GetCapture2(TIM1);
		TIM_SetCompare2(TIM1, vlue + CCR2_Val);
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
		if (stateB == 1)
		{
			pulsesB++;
			if (pulsesB >= angleB)
			{
				Step_Stop(STEPB);
				pulsesB = 0;
			}
		}
	}

	if (TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
	{
		vlue = TIM_GetCapture3(TIM1);
		TIM_SetCompare3(TIM1, vlue + CCR3_Val);
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
		if (stateC == 1)
		{
			pulsesC++;
			if (pulsesC >= angleC)
			{
				Step_Stop(STEPC);
				pulsesC = 0;
			}
		}
	}

	if (TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)
	{
		vlue = TIM_GetCapture4(TIM1);
		TIM_SetCompare4(TIM1, vlue + CCR4_Val);
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);
	}
}

void Step_control(u8 index, u8 dir, int angle)
{
	Step_Driver(index, dir);
	if (index == 0)
		angleA = angle;
	else if (index == 1)
		angleB = angle;
	else if (index == 2)
		angleC = angle;
}
