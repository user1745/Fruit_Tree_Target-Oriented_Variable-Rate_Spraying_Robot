#ifndef _ENCODE_H
#define _ENCODE_H

#include "sys.h"

// 编码器数据结构体
typedef struct
{
	float cntA;
	float cntB;
	float cntC;
	float cntD;
	float speedA;
	float speedB;
	float speedC;
	float speedD;
	float disA;
	float disB;
	float disC;
	float disD;
} EncoderStruct;

// 编码器滤波结构体
typedef struct
{
	unsigned char Complete;
	unsigned char Count;
	float Cache;		// 计算中间缓存值
	float Last_Data;	// 上一次捕获值
	float Data_His[10]; // 数据历史值
} EncoderFilter_Struct;

extern EncoderStruct Encoders;

void TIM2_Encoder_Init(u16 arr, u16 psc);
void TIM3_Encoder_Init(u16 arr, u16 psc);
void TIM4_Encoder_Init(u16 arr, u16 psc);
void TIM8_Encoder_Init(u16 arr, u16 psc);
void Encoder_TEST(void);
void Encoder_Init(void);
void Read_Encoder_cnt(void);
void Solve_EncoData(float time);

void Encoder_Filter(EncoderFilter_Struct *filterStruct, float newDatas, unsigned char FilterNum, float *Out);

#endif
