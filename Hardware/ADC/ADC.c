#include "ADC.h"

FilterStruct ADCStruct = {{0.0}, 0};
float power_vol;
float ch11_Vol;
float ch12_Vol;

/*
********************************************************************************************************************
*                  void  Adc_Init(void)
*
*Description    :Adc的初始化    端口为PA0    使用ADC1 channel_0
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // 使能ADC1时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;	 // 模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);			 // 初始化

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);  // ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE); // 复位结束

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent; // 独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; // DMA失能
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; // 12位模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;		   // 非扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // 禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						// 右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;									// 1个转换在规则序列中 也就是只转换规则序列1
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_Cmd(ADC1, ENABLE); // 开启AD转换器
}

/*
********************************************************************************************************************
*                  u16 Get_Adc()
*
*Description    :得到ADC的数值								已设置默认通道为  ADC_Channel_0  ！！！！！！！
*Returns     :ADC的采样值(十进制)
*Notes       : none
********************************************************************************************************************
*/
u16 Get_Adc(u8 ch)
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles);

	ADC_SoftwareStartConv(ADC1);

	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
		;

	return ADC_GetConversionValue(ADC1);
}

u16 Get_Adc_Average(u8 ch, u8 times)
{
	u32 temp_val = 0;
	u8 t;
	for (t = 0; t < times; t++)
	{
		temp_val += Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val / times;
}

/*
********************************************************************************************************************
*                  u16 Get_Vol(u16 AdcVal)
*
*Description :得到电压值
*Arguments   :AdcVal:  ADC的采样值
*Returns     :电池的电压值
*Notes       : none
********************************************************************************************************************
*/
float Get_Vol(u8 ch)
{
	u8 t;
	u32 temp_val;
	float vol;
	for (t = 0; t < 20; t++)
	{
		temp_val += Get_Adc(ch);
		delay_ms(2);
	}
	vol = (temp_val / 20) * (3.3 / 4096);
	return vol;
}

/*
********************************************************************************************************************
*                  u16 Get_Vol(u16 AdcVal)
*
*Description :得到电压值
*Arguments   :AdcVal:  ADC的采样值
*Returns     :电池的电压值
*Notes       : none
********************************************************************************************************************
*/
void Get_power_Vol(void)
{
	u32 temp_val;
	float vol;
	temp_val = Get_Adc(10);
	vol = temp_val * (3.3 / 4096);
	vol = vol * 3.98f;
	power_vol = meanFilter(&ADCStruct, vol, 10);
}

void Get_ch11_Vol(void)
{
	u8 t;
	u32 temp_val;
	float vol;
	for (t = 0; t < 2; t++)
	{
		temp_val += Get_Adc(12);
		delay_ms(2);
	}
	vol = (temp_val / 2) * (3.3 / 4096);
	ch11_Vol = meanFilter(&ADCStruct, vol, 8);
}

void Get_ch12_Vol(void)
{
	u8 t;
	u32 temp_val;
	float vol;
	for (t = 0; t < 2; t++)
	{
		temp_val += Get_Adc(12);
		delay_ms(2);
	}
	vol = (temp_val / 2) * (3.3 / 4096);
	ch12_Vol = meanFilter(&ADCStruct, vol, 8);
}
