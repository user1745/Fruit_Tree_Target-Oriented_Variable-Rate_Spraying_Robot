#include "Encoder.h"

// 存放编码器数据的结构体
EncoderStruct Encoders = {0, 0, 0, 0,
						  0, 0, 0, 0,
						  0, 0, 0, 0};

// 滤波的结构体
EncoderFilter_Struct EncoAspeed_Filter = {0, 0, 0, 0, {0}};
EncoderFilter_Struct EncoBspeed_Filter = {0, 0, 0, 0, {0}};
EncoderFilter_Struct EncoCspeed_Filter = {0, 0, 0, 0, {0}};
EncoderFilter_Struct EncoDspeed_Filter = {0, 0, 0, 0, {0}};

/*
********************************************************************************************************************
*                  void Encoder_Init()
*
*Description : 编码器初始化
*Arguments   : 4个定时器做编码器：T2 T3 T4 T8
*Returns     : none
*Notes       :
********************************************************************************************************************
*/
void Encoder_Init(void)
{
	TIM2_Encoder_Init(0XFFFF, 1 - 1);
	// TIM3_Encoder_Init(0XFFFF,1-1);
	TIM4_Encoder_Init(0XFFFF, 1 - 1);
	// TIM8_Encoder_Init(0XFFFF,1-1);
}

/*
********************************************************************************************************************
*                  void TIM2_Encoder_Init(u16 arr,u16 psc)
*
*Description : 定时器2的输入捕获配置
*Arguments   : 溢出时间为：（arr+1）*（psc+1）/84M
*Returns     : none
*Notes       : arr:自动重装值,psc:时钟预分频数
********************************************************************************************************************
*/
void TIM2_Encoder_Init(u16 arr, u16 psc) // EncoderA
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); // 使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	   // 清除TIM的更新标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 允许更新中断

	TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2, ENABLE);
}

/*
********************************************************************************************************************
*                  void TIM3_Encoder_Init(u16 arr,u16 psc)
*
*Description : 定时器3的输入捕获配置
*Arguments   : 溢出时间为：（arr+1）*（psc+1）/84M
*Returns     : none
*Notes       : arr:自动重装值,psc:时钟预分频数
********************************************************************************************************************
*/
// void TIM3_Encoder_Init(u16 arr,u16 psc)				//EncoderB
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	TIM_ICInitTypeDef TIM_ICInitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);

//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	TIM_TimeBaseStructure.TIM_Prescaler=psc;
//	TIM_TimeBaseStructure.TIM_Period=arr;
//	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

//	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
//	TIM_ICStructInit(&TIM_ICInitStructure);
//	TIM_ICInitStructure.TIM_ICFilter = 6;
//	TIM_ICInit(TIM3, &TIM_ICInitStructure);

//	TIM_ClearFlag(TIM3, TIM_FLAG_Update);		//清除TIM的更新标志位
//	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);	//允许更新中断

//	TIM_SetCounter(TIM3,0);
//	TIM_Cmd(TIM3, ENABLE);
//}

/*
********************************************************************************************************************
*                  void TIM4_Encoder_Init(u16 arr,u16 psc)
*
*Description : 定时器4的输入捕获配置
*Arguments   : 溢出时间为：（arr+1）*（psc+1）/84M
*Returns     : none
*Notes       : arr:自动重装值,psc:时钟预分频数
********************************************************************************************************************
*/
void TIM4_Encoder_Init(u16 arr, u16 psc) // EncoderC
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM3);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); // 使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);	   // 清除TIM的更新标志位
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); // 允许更新中断

	TIM_SetCounter(TIM4, 0);
	TIM_Cmd(TIM4, ENABLE);
}

/*
********************************************************************************************************************
*                  void TIM8_Encoder_Init(u16 arr,u16 psc)
*
*Description : 定时器8的输入捕获配置
*Arguments   : 溢出时间为：（arr+1）*（psc+1）/168M
*Returns     : none
*Notes       : arr:自动重装值,psc:时钟预分频数
********************************************************************************************************************
*/
// void TIM8_Encoder_Init(u16 arr,u16 psc)				//EncoderD
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	TIM_ICInitTypeDef TIM_ICInitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);
//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);

//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);

//	TIM_TimeBaseStructure.TIM_Prescaler=psc;
//	TIM_TimeBaseStructure.TIM_Period=arr;
//	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);

//	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
//	TIM_ICStructInit(&TIM_ICInitStructure);
//	TIM_ICInitStructure.TIM_ICFilter = 6;
//	TIM_ICInit(TIM8, &TIM_ICInitStructure);

//	TIM_ClearFlag(TIM8, TIM_FLAG_Update);		//清除TIM的更新标志位
//	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);	//允许更新中断

//	TIM_SetCounter(TIM8,0);
//	TIM_Cmd(TIM8, ENABLE);
//}

/*
********************************************************************************************************************
*                  void Read_Encoder_cnt(void)
*
*Description : 左右编码器测速函数
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void Read_Encoder_cnt(void)
{
	Encoders.cntA = -(short)TIM2->CNT; // （short）等效：将最高数据变为符号位
	//	Encoders.cntB= -(short)TIM3->CNT;
	Encoders.cntC = (short)TIM4->CNT;
	// Encoders.cntD= (short)TIM8->CNT;
	//	TIM2->CNT=0;
	//	//TIM3->CNT=0;
	//	TIM4->CNT=0;
	// TIM8->CNT=0;
}
/*
********************************************************************************************************************
*                  void Solve_EncoData(void)
*
*Description : 编码器解算距离、速度函数
*Arguments   : none
*Returns     : none
*Notes       : 3168：电机正转一圈，CNT计数值  3.14：Pi   10.0：轮子直径/cm  time：解算频率/s
********************************************************************************************************************
*/
void Solve_EncoData(float time)
{
	float speedA = 0;
	// float speedB=0;
	float speedC = 0;
	// float speedD=0;
	Read_Encoder_cnt();
	TIM2->CNT = 0;
	TIM4->CNT = 0;
	Encoders.disA = (Encoders.cntA / 1680.0f) * 3.14f * 7.6f; // 单位厘米 cm
	//	Encoders.disA+=Encoders.disA;
	// Encoders.disB=(Encoders.cntB/3168.0f)*3.14f*10.0f;
	Encoders.disC = (Encoders.cntC / 1680.0f) * 3.14f * 7.6f;
	//	Encoders.disC+=Encoders.disC;
	// Encoders.disD=(Encoders.cntD/3168.0f)*3.14f*10.0f;
	speedA = Encoders.disA / time; // 单位厘米 cm/s
	// speedB=-Encoders.disB/time;
	speedC = (Encoders.disC / time);
	// speedD=-Encoders.disD/time;
	Encoder_Filter(&EncoAspeed_Filter, speedA, 5, &Encoders.speedA);
	// Encoder_Filter(&EncoBspeed_Filter,speedB,5,&Encoders.speedB);
	Encoder_Filter(&EncoCspeed_Filter, speedC, 5, &Encoders.speedC);
	// Encoder_Filter(&EncoDspeed_Filter,speedD,5,&Encoders.speedD);
}

/******************************************************************************************************************
*             void Encoder_Filter(Filter_Struct *filterStruct,float newDatas,unsigned char FilterNum,float *Out)
*
*Description : 增强型均值滤波  去掉一个最大值和最小值
*Returns     : 当前的数据
*Notes       : 输入的数据在程序中完成缓存
*
							*filterStruct		选择滤波的结构体
							newDatas				新数据
							FilterNum				滤波样本总数
							*Out						输出值
*******************************************************************************************************************
*/
void Encoder_Filter(EncoderFilter_Struct *filterStruct, float newDatas, unsigned char FilterNum, float *Out)
{
	unsigned int i = 0;
	float FilterSum = 0;
	static float max, min;

	filterStruct->Data_His[filterStruct->Count++] = newDatas;

	if (filterStruct->Complete) // 已经采集完最初的AdcFilterNum个数据
	{
		max = min = filterStruct->Data_His[0]; // 初始化
		// 找到最大最小值
		for (i = 1; i < FilterNum; i++)
		{
			if (filterStruct->Data_His[i] > max)
			{
				max = filterStruct->Data_His[i];
			}

			if (filterStruct->Data_His[i] < min)
			{
				min = filterStruct->Data_His[i];
			}
		}

		for (i = 0; i < FilterNum; i++)
		{
			FilterSum += filterStruct->Data_His[i];
		}
		FilterSum -= max + min;
		filterStruct->Cache = FilterSum / (FilterNum - 2);
	}

	if (filterStruct->Complete == 1)
	{
		if (filterStruct->Cache < 5000)
		{
			*Out = filterStruct->Cache;
			filterStruct->Last_Data = filterStruct->Cache;
		}
		else
			*Out = filterStruct->Last_Data;
	}
	if (filterStruct->Count == FilterNum)
	{
		filterStruct->Complete = 1;
		filterStruct->Count = 0;
	}
}

void Encoder_TEST(void)
{
	OLED_Clear();
	Encoder_Init();
	Motor_Init();
	OLED_ShowString(12, 0, (void *)"Encoder_TEST", 12);
	OLED_Refresh_Gram();
	delay_ms(200);

	while (test_flag && upmenumode == encoder_test_mode)
	{
		Motor_Set(receive_data_saveA * 20, receive_data_saveB * 20, receive_data_saveC * 20, receive_data_saveD * 20);
		OLED_ShowString(0, 12, (void *)"speedA:", 12);
		// OLED_ShowString(0,24,(void *)"speedB:",12);
		OLED_ShowString(0, 24, (void *)"speedC:", 12);
		// OLED_ShowString(0,48,(void *)"speedD:",12);
		OLED_Show3FNum(55, 12, Encoders.speedA, 5, 2, 12);
		// OLED_Show3FNum(55,24,Encoders.speedB,5,2,12);
		OLED_Show3FNum(55, 36, Encoders.speedC, 5, 2, 12);
		// OLED_Show3FNum(55,48,Encoders.speedD,5,2,12);
		OLED_ShowNum(55, 12, receive_data_saveA, 5, 12);
		//		OLED_ShowNum(55,24,receive_data_saveB,5,12);
		OLED_ShowNum(55, 24, receive_data_saveC, 5, 12);
		//		OLED_ShowNum(55,48,receive_data_saveD,5,12);
		OLED_Refresh_Gram();
		// printf("%d,%d,%d,%d\r\n",(int)Encoders.speedA,(int)Encoders.speedB,(int)Encoders.speedC,(int)Encoders.speedD);
		delay_ms(100);
	}
	Motor_Set(0, 0, 0, 0);
}
