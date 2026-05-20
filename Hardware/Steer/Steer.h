#ifndef _STEER_H
#define _STEER_H
#include "sys.h"

extern u32 Steer_PWM1; // 舵机控制脉宽
extern u32 Steer_PWM2; // 舵机控制脉宽
extern u32 Steer_PWM3; // 舵机控制脉宽
extern u32 Steer_PWM4; // 舵机控制脉宽

void Steer_Init(void);
void TIM2_Steer_Init(u16 arr, u16 psc);
void Steer_Angle_Control(int Angle1, int Angle2);

#endif
