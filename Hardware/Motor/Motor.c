#include "Motor.h"
#include "math.h"
#include "Delay.h"

#define SPEED_BASEA 0  // A轮初始速度
#define SPEED_BASEB 0  // B轮初始速度
#define SPEED_BASEC 0  // C轮初始速度
#define SPEED_BASED 0  // D轮初始速度
int Motor_limit = 999; // 速度最大值

void Motor_Init(void)
{
	Motor_IO_Init();
	TIM1_Motor_Init(1000 - 1, 14 - 1); // ABCD轮定时器1初始化
	TIM8_Motor_Init(1000 - 1, 14 - 1);

	//	TIM2_free_Init(1000-1,7-1);
	//	TIM10_free_Init(1000-1,14-1);
	//	TIM11_free_Init(1000-1,14-1);
	//
	//	TIM4_Encoder_Init(0xFFFF,0);
	//	TIM5_Encoder_Init(0xFFFF,0);
}

void Motor_4950_Init(void)
{
	Motor_IO_Init();
	TIM1_Motor_Init(1000 - 1, 56 - 1); // ABCD轮定时器1初始化
	TIM1->CCR1 = 0;
}

/*
********************************************************************************************************************
*                  void Motor_IO_Init(void)
*
*Description : 电机控制引脚的初始化
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void Motor_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(AIN1BIN1BIN2_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(AIN2BIN3_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = AIN1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(AIN1BIN1BIN2_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = AIN2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(AIN2BIN3_PORT, &GPIO_InitStructure);
	GPIO_Init(AIN1BIN1BIN2_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(AIN1BIN1BIN2_PORT, AIN1_PIN);
	GPIO_ResetBits(AIN2BIN3_PORT, AIN2_PIN);

	RCC_AHB1PeriphClockCmd(AIN1BIN1BIN2_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = BIN1_PIN | BIN2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(AIN1BIN1BIN2_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(AIN1BIN1BIN2_PORT, BIN1_PIN | BIN2_PIN);

	RCC_AHB1PeriphClockCmd(AIN3AIN4_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = AIN3_PIN | AIN4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(AIN3AIN4_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(AIN3AIN4_PORT, AIN3_PIN | AIN4_PIN);

	RCC_AHB1PeriphClockCmd(AIN2BIN3_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(BIN4_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = BIN3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(AIN2BIN3_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = BIN4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(BIN4_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(AIN2BIN3_PORT, BIN3_PIN);
	GPIO_ResetBits(BIN4_PORT, BIN4_PIN);
}

/*
********************************************************************************************************************
*                  void TIM1_Motor_Init(u16 arr,u16 psc)
*
*Description : 定时器1的电机PWM配置
*Arguments   : 溢出时间为：（arr+1）*（psc+1）/168M
*Returns     : none
*Notes       : arr:自动重装值,psc:时钟预分频数
*				TIM1_Motor_Init(1000-1,14-1);			//12KHz
********************************************************************************************************************
*/
void TIM1_Motor_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
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

void TIM8_Motor_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC1Init(TIM8, &TIM_OCInitStructure);
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM8, ENABLE);
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
	TIM_Cmd(TIM8, ENABLE);
}

/******************* int myabs(int x)绝对值函数 ***********************/
int myabs(int x)
{
	int temp;
	if (x < 0)
		temp = -x;
	else
		temp = x;
	return temp;
}

/*
****************************************************************************************************************
**	void Motor_Set(int speedA,int speedB)		电机速度设定，可以为负值，便于PID调节
**	speedA:PWMA初始输入；speedB:PWMB初始输入
**	因电机特性，不能保证四个电机相同占空比时转速相等，若要相等要加相应的初值SPEED_BASE
**  右前，左前，左后，右后
****************************************************************************************************************
*/
void Motor_Set(int speedA, int speedB, int speedC, int speedD)
{
	//	speedA=-speedA;
	//	speedB=-speedB;

	speedA >= 0 ? ((AIN1 = 1), (AIN2 = 0)) : ((AIN1 = 0), (AIN2 = 1));
	speedB >= 0 ? ((BIN1 = 0), (BIN2 = 1)) : ((BIN1 = 1), (BIN2 = 0));
	speedC >= 0 ? ((AIN3 = 1), (AIN4 = 0)) : ((AIN3 = 0), (AIN4 = 1));
	speedD >= 0 ? ((BIN3 = 0), (BIN4 = 1)) : ((BIN3 = 1), (BIN4 = 0));

	if (speedA == 0)
	{
		AIN1 = 1;
		AIN2 = 1;
	}
	if (speedB == 0)
	{
		BIN1 = 1;
		BIN2 = 1;
	}
	if (speedC == 0)
	{
		AIN3 = 1;
		AIN4 = 1;
	}
	if (speedD == 0)
	{
		BIN3 = 1;
		BIN4 = 1;
	}

	if (speedA < -Motor_limit)
		speedA = -Motor_limit;
	if (speedA > Motor_limit)
		speedA = Motor_limit;

	if (speedB < -Motor_limit)
		speedB = -Motor_limit;
	if (speedB > Motor_limit)
		speedB = Motor_limit;

	if (speedC < -Motor_limit)
		speedC = -Motor_limit;
	if (speedC > Motor_limit)
		speedC = Motor_limit;

	if (speedD < -Motor_limit)
		speedD = -Motor_limit;
	if (speedD > Motor_limit)
		speedD = Motor_limit;

	TIM1->CCR1 = myabs(speedA) + SPEED_BASEA;
	TIM1->CCR2 = myabs(speedB) + SPEED_BASEB;
	TIM8->CCR1 = myabs(speedC) + SPEED_BASEC;
	TIM8->CCR2 = myabs(speedD) + SPEED_BASED;
}

void Motor_4950_Set(int speedA)
{

	if (speedA < -Motor_limit)
		speedA = -Motor_limit;
	if (speedA > Motor_limit)
		speedA = Motor_limit;

	if (speedA >= 0)
	{
		AIN1 = 0;
		TIM1->CCR1 = myabs(speedA) + SPEED_BASEA;
	}
	else if (speedA < 0)
	{
		AIN1 = 1;
		TIM1->CCR1 = myabs(speedA + 1000) + SPEED_BASEA;
	}
}

void Motor_4950_TEST(void)
{
	OLED_Clear();
	//	Motor_4950_Init();
	OLED_ShowString(12, 0, (void *)"MOTOR_4950_TEST", 12);
	OLED_Refresh_Gram();
	delay_ms(200);
	u8 dira;
	int speeda;
	while (test_flag && upmenumode == motor_test_mode)
	{
		dira = (receive_data_saveA & 0x80) >> 7;
		speeda = receive_data_saveA & 0x7f;
		dira == 1 ? (speeda = -speeda) : (speeda = speeda);
		OLED_ShowIntNum(55, 12, speeda, 5, 12);
		OLED_Refresh_Gram();
		Motor_4950_Set(speeda * 20);
	}
}

/*******************麦轮实现函数************************/

// Vx：横向移动速度
// Vy：直行速度
// Vz：旋转速度
int speed_A = 0; // A轮速度
int speed_B = 0; // B轮速度
int speed_C = 0; // C轮速度
int speed_D = 0; // D轮速度

void Motor_Analysis(float Vx1, float Vx2, float Vy1, float Vy2, float Wz) // max:
{
	speed_A = Vy1 - Vx1 - Wz * (Long + Wide);
	speed_B = Vy2 + Vx2 + Wz * (Long + Wide);
	speed_C = Vy2 + Vx2 - Wz * (Long + Wide);
	speed_D = Vy1 - Vx1 + Wz * (Long + Wide);
	Motor_Set(speed_A, speed_B, speed_C, speed_D);
	//	Motor_Set(PID_PWMA.out,PID_PWMA.out,PID_PWMA.out,PID_PWMA.out);
	//	speed_A=-Vx+Vy-Wz*(Long + Wide);
	//	speed_B=-Vx-Vy-Wz*(Long + Wide);
	//	speed_C=Vx-Vy-Wz*(Long + Wide);
	//	speed_D=Vx+Vy-Wz*(Long + Wide);
	//	Motor_Set(speed_A,speed_B,speed_C,speed_D);
}

// void Motor_Analysis_Forward(float Wz, int C)
//{
//	  float Vy=(2.7547*C+17.3986);
//	  Motor_Analysis(0,Vy,Wz);
// }

// void Motor_Analysis_Back(float Wz, int C)
//{
//	  float Vy=-(2.7547*C+17.3986);
//	  Motor_Analysis(0,Vy,Wz);
// }

// void Motor_Analysis_Left(float Wz, int C)
//{
//	  float Vx=(2.7547*C+17.3986);
//	  Motor_Analysis(Vx,0,Wz);
// }

// void Motor_Analysis_Right(float Wz, int C)
//{
//	  float Vx=-(2.7547*C+17.3986);
//	  Motor_Analysis(Vx,0,Wz);
// }

void Motor_TEST(void)
{
	u8 key;
	Motor_Init();
	int SpeedA, SpeedB, SpeedC, SpeedD;
	OLED_ShowString(12, 0, (void *)"Test_8 TB", 12);
	OLED_Refresh_Gram();
	while (test_flag)
	{
		key = KEY_Scan(0);
		if (key == ku)
		{
			SpeedA = 200;
			OLED_Show3FNum(30, 12, SpeedA, 4, 2, 12);
			OLED_Refresh_Gram();
		}
		if (key == kd)
		{
			SpeedB = 300;
			OLED_Show3FNum(30, 24, SpeedB, 4, 2, 12);
			OLED_Refresh_Gram();
		}
		if (key == kl)
		{
			SpeedC = 500;
			OLED_Show3FNum(30, 36, SpeedC, 4, 2, 12);
			OLED_Refresh_Gram();
		}
		if (key == kr)
		{
			SpeedD = 800;
			OLED_Show3FNum(30, 48, SpeedD, 4, 2, 12);
			OLED_Refresh_Gram();
		}
		if (key == kc)
		{
			SpeedA = -150;
			SpeedB = -300;
			SpeedC = -500;
			SpeedD = -800;
		}
		OLED_ShowString(0, 0, (void *)"Speed:", 12);
		Motor_Set(SpeedA, SpeedB, SpeedC, SpeedD);
	}
}
