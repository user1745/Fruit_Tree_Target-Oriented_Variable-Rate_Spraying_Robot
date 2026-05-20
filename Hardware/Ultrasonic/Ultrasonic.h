
#ifndef __Ultrasonic_H
#define __Ultrasonic_H

#include "sys.h"
#include "Delay.h"

#define CAP_TIMER TIM3
#define CAP_Overflow 0XFFFFFFFF

#define TRIG1 PEout(12)
#define TRIG2 PEout(13)

#define TRIG3 PEout(14)
#define TRIG4 PEout(15)

#define ECHO1 PAin(6)
#define ECHO2 PAin(7)

#define ECHO3 PBin(0)
#define ECHO4 PBin(1)

#define TRIG1_out       \
	{                   \
		TRIG1 = 1;      \
		delay_us(4000); \
		TRIG1 = 0;      \
	}
#define TRIG2_out     \
	{                 \
		TRIG2 = 1;    \
		delay_us(10); \
		TRIG2 = 0;    \
	}

#define TRIG3_out        \
	{                    \
		TRIG3 = 1;       \
		delay_us(20000); \
		TRIG3 = 0;       \
	}
#define TRIG4_out        \
	{                    \
		TRIG4 = 1;       \
		delay_us(20000); \
		TRIG4 = 0;       \
	}

#define TRIGALL       \
	{                 \
		TRIG1 = 1;    \
		TRIG2 = 1;    \
		TRIG3 = 1;    \
		TRIG4 = 1;    \
		delay_us(20); \
		TRIG1 = 0;    \
		TRIG2 = 0;    \
		TRIG3 = 0;    \
		TRIG4 = 0     \
	}

typedef struct
{
	unsigned char Complete;
	unsigned char Count;
	float Cache;		// 计算中间缓存值
	float Last_Data;	// 上一次捕获值
	float Data_His[15]; // 数据历史值
} UltrasonicFilter_Struct;

typedef struct
{
	u16 Forward;
	u16 Back;

	u16 Left;
	u16 Right;

} UltrasonicDis;

extern UltrasonicDis UL_Dis;

void Ultrasonic_Init(void);			  // 超声波IO和定时器初始化
void TIM3_Cap_Init(u16 arr, u16 psc); // 定时器捕获初始化
void Ultrasonic_IO_Init(void);		  // 超声波IO口初始化函数
									  // 测距函数
void Ultrasonic_Get_Distance(void);
void Ultrasonic_Get_Distance_1(void);
void Ultrasonic_Get_Distance_2(void);
void Ultrasonic_Get_Distance_3(void);
void Ultrasonic_Get_Distance_4(void);

void Ultrasonic_Filter(UltrasonicFilter_Struct *filterStruct, float newDatas, unsigned char FilterNum, u16 *Out);
// 超声波数据滤波处理（增强型均值滤波）
void Ultrasonic_TEST(void);

#endif
