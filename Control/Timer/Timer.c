
#include "Timer.h"

// steer//
int steer1_angle = 89;
int steer2_angle = 0;

// motor//
u8 motor_delay_flag = 0; // 1:开始延时，0停止延时
u8 motor_cnt_flag = 0;	 // 1:开始计时，0停止计时

// openmv_Wide//
u8 openmv_Wide_flag = 1; // 1说明此次接收有效，0说明此次接收无效
u16 red_x_Wide = 0;
u16 red_y_Wide = 0;
u8 openmv_Wide[6] = {0};

u8 debuga = 0;
u8 debugb = 0;
u8 debugc = 0;
u8 debugd = 0;
u8 debuge = 0;
u8 debugf = 0;

// openmv_Narrow//
u8 openmv_Narrow_flag = 1; // 1说明此次接收有效，0说明此次接收无效
u16 red_x_Narrow = 0;
u16 red_y_Narrow = 0;
u8 openmv_Narrow[6] = {0};

u8 debugg = 0;
u8 debugh = 0;
u8 debugi = 0;
u8 debugj = 0;
u8 debugk = 0;
u8 debugm = 0;

// 6050//
u8 Mpu_Y_flag = 0;
u8 Mpu_X_flag = 0;

// Ultra//
u8 Ultra_LO_flag = 0;
u8 Ultra_RO_flag = 0;
u8 Ultra_LI_flag = 0;

void TIMER_Interrupt_Init(void)
{
	// TIM6_Int_Init(1000-1,840-1);
	TIM7_Int_Init(5000 - 1, 840 - 1);
}

/*
********************************************************************************************************************
*                  void TIM6_Int_Init(u16 arr,u16 psc)
*
*Description : 定时器6的中断配置
*Arguments   : 溢出时间为：（arr+1）*（psc+1）/84M
*Returns     : none
*Notes       : arr:自动重装值,psc:时钟预分频数
********************************************************************************************************************
*/
// void TIM6_Int_Init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
//
//	TIM_TimeBaseInitStructure.TIM_Period = arr;
//	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
//
//	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);
//
////	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
//	 TIM_Cmd(TIM6,ENABLE);
//
////	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn;
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
////	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
////	NVIC_Init(&NVIC_InitStructure);
//}

/*
********************************************************************************************************************
*                  void TIM7_Int_Init(u16 arr,u16 psc)
*
*Description : 定时器7的中断配置
*Arguments   : 溢出时间为：（arr+1）*（psc+1）/84M
*Returns     : none
*Notes       : arr:自动重装值,psc:时钟预分频数
********************************************************************************************************************
*/
void TIM7_Int_Init(u16 arr, u16 psc)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStructure);

	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM7, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
********************************************************************************************************************
*                  void TIM6_DAC_IRQHandler(void)
*
*Description : 定时器6中断服务函数
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
// void TIM6_DAC_IRQHandler(void)
//{
//	static u16 ms_10=0;
////	static u16 ms_2000=0;
////	static u16 ms_4000=0;
////	static u16 ms_5000=0;
//	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET) //溢出中断
//	{
//		ms_10++;
//	}

//	if(ms_10>=1)
//	{
//		Solve_EncoData(0.01);
//		ms_10=0;
//	}
//
//	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);  //清除中断标志位
//}

/*
********************************************************************************************************************
*                  void TIM7_IRQHandler(void)
*
*Description : 定时器7中断服务函数
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
// void TIM7_IRQHandler(void)
//{
//	static u16 ms_10=0;
//	static u16 ms_50=0;
//	static u16 ms_100=0;
//	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //溢出中断
//	{
//		ms_10++;
//		ms_50++;
//		ms_100++;
//	}
//
//	if(ms_10>=10)
//	{
//		mpu_dmp_get_data(&mpu_pitch,&mpu_roll,&mpu_yaw);
//		ms_10=0;
//	}

//	if(ms_50>=50)
//	{
//		if(Mpu_X_flag==1)
//		{
//			ControlPID1(mpu_yaw);
//			Mpu_X_flag=0;
//		}
//		if(Mpu_Y_flag==1)
//		{
//			ControlPID6050(mpu_yaw);
//			Mpu_Y_flag=0;
//		}
//
//		if((openmv_control_flag==1) && (red_x_Narrow!=0) && (red_y_Narrow!=0))
//		{
//			Cam_x_PID5(red_x_Narrow);
//			Cam_y_PID6(red_y_Narrow);
//		}
//		PWMA_PID7(Encoders.speedA);
//		PWMB_PID8(Encoders.speedB);
//		PWMC_PID9(Encoders.speedC);
//		PWMD_PID10(Encoders.speedD);
//		ms_50=0;
//	}
//
//	if(ms_100>=100)
//	{
//		if(Ultra_LO_flag==1)
//		{
//			ControlPID2(UL_Dis.Behind_Left_Outside);
//			ControlPID3(UL_Dis.Front_Left_Outside);
//			Ultra_LO_flag=0;
//		}
//		if(Ultra_RO_flag==1)
//		{
//			ControlPID2(UL_Dis.Behind_Right_Outside);
//			ControlPID3(UL_Dis.Front_Right_Outside);
//			Ultra_RO_flag=0;
//		}
//		if(Ultra_LI_flag==1)
//		{
//			ControlPID4(UL_Dis.Behind_Left_Inside);
//			Ultra_LI_flag=0;
//		}
//		ms_100=0;
//	}
//
//	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  //清除中断标志位
//}
