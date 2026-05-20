#ifndef __PID_H
#define __PID_H

#include "sys.h"

/*
******************************************************************************************************************
*Description : 变量及宏定义声明
*Notes       : none
******************************************************************************************************************
*/
typedef struct
{
	float P;
	float I;
	float D;
	float err;
	float last_err;
	float integral;
	float I_sep;
	float I_Max;
	float umax;
	float umin;
	float Expected;
	float out;
} PID;

extern PID PID_1;
extern PID PID_6050;
extern PID PID_2;
extern PID PID_3;
extern PID PID_4;
extern PID PID_x_CAM;
extern PID PID_y_CAM;
extern PID PID_PWMA;
extern PID PID_PWMB;
extern PID PID_PWMC;
extern PID PID_PWMD;

void PID_Init(void);

float ControlPID1(float Actual);
float ControlPID6050(float Actual);
float ControlPID2(float Actual);
float ControlPID3(float Actual);
float ControlPID4(float Actual);
float Cam_x_PID5(u16 Actual);
float Cam_y_PID6(u16 Actual);
float PWMA_PID7(float Actual);
float PWMB_PID8(float Actual);
float PWMC_PID9(float Actual);
float PWMD_PID10(float Actual);

#endif
