#include "PID.h"
#include <math.h>

PID PID_1;
PID PID_6050;
PID PID_2;
PID PID_3;
PID PID_4;
PID PID_x_CAM;
PID PID_y_CAM;
PID PID_PWMA;
PID PID_PWMB;
PID PID_PWMC;
PID PID_PWMD;

/******************************************************************************************************************
 *                            PID_Init()
 *Description : PID参数初始化
 *Returns     : none
 *Notes       : none
 *******************************************************************************************************************
 */
void PID_Init(void)
{
	/*
	*************************************
	* 			PID_1参数初始化				角度PID
	*************************************
	*/
	PID_1.P = 1;   // 比例
	PID_1.I = 0.1; // 积分
	PID_1.D = 0;   // 微分

	PID_1.Expected = 0; // 期望值

	PID_1.err = 0;		// 偏差
	PID_1.last_err = 0; // 上一次偏差
	PID_1.integral = 0; // 偏差总和（积分值）
	PID_1.I_Max = 2;	// 积分限幅
	PID_1.umax = 50;
	PID_1.umin = -50;

	PID_1.out = 0; // 输出

	/*
	*************************************
	* 			PID_6050参数初始化				角度PID
	*************************************
	*/
	PID_6050.P = 50;  // 比例
	PID_6050.I = 0.1; // 积分
	PID_6050.D = 0;	  // 微分

	PID_6050.Expected = 0; // 期望值

	PID_6050.err = 0;	   // 偏差
	PID_6050.last_err = 0; // 上一次偏差
	PID_6050.integral = 0; // 偏差总和（积分值）
	PID_6050.I_Max = 2;	   // 积分限幅
	PID_6050.umax = 50;
	PID_6050.umin = -50;

	PID_6050.out = 0; // 输出

	/*
	************************************
	*			PID_2参数初始化        距离PID_Y_AB
	************************************
	*/
	PID_2.P = 0.004;	 // 比例
	PID_2.I = 0.0000015; // 积分
	PID_2.D = 0.02;		 // 微分

	PID_2.Expected = 0; // 期望值

	PID_2.err = 0;		// 偏差
	PID_2.last_err = 0; // 上一次偏差
	PID_2.integral = 0; // 偏差总和（积分值）
	PID_2.I_Max = 10;	// 积分限幅
	PID_2.umax = 2;
	PID_2.umin = -2;

	PID_2.out = 0; // 输出

	/*
	************************************
	*			PID_3参数初始化				距离PID_Y_CD
	************************************
	*/
	PID_3.P = 12.8; // 比例
	PID_3.I = 0.01; // 积分
	PID_3.D = 7.2;	// 微分

	PID_3.Expected = 0; // 期望值

	PID_3.err = 0;		   // 偏差
	PID_3.last_err = 0;	   // 上一次偏差
	PID_3.integral = 0.01; // 偏差总和（积分值）
	PID_3.I_Max = 10;	   // 积分限幅
	PID_3.umax = 30;
	PID_3.umin = -30;

	PID_3.out = 0; // 输出
	/*
	************************************
	*			PID_4参数初始化				距离PID_X
	************************************
	*/
	PID_4.P = 3.2; // 比例
	PID_4.I = 0;   // 积分
	PID_4.D = 1.8; // 微分

	PID_4.Expected = 0; // 期望值

	PID_4.err = 0;		// 偏差
	PID_4.last_err = 0; // 上一次偏差
	PID_4.integral = 0; // 偏差总和（积分值）
	PID_4.I_Max = 8;	// 积分限幅
	PID_4.umax = 20;
	PID_4.umin = -20;

	PID_4.out = 0; // 输出
	/*
	************************************
	*			PID_CAM_NarrowX参数初始化
	************************************
	*/
	PID_x_CAM.P = 0.2;	// 比例
	PID_x_CAM.I = 0.01; // 积分
	PID_x_CAM.D = 0.1;	// 微分

	PID_x_CAM.Expected = 100; // 期望值

	PID_x_CAM.err = 0;		// 偏差
	PID_x_CAM.last_err = 0; // 上一次偏差
	PID_x_CAM.integral = 0; // 偏差总和（积分值）
	PID_x_CAM.I_Max = 8;	// 积分限幅
	PID_x_CAM.umax = 20;
	PID_x_CAM.umin = -20;

	PID_x_CAM.out = 0; // 输出
	/*
	 ************************************
	 *			PID_CAM_NarrowY参数初始化
	 ************************************
	 */
	PID_y_CAM.P = 0.2;	// 比例
	PID_y_CAM.I = 0.01; // 积分
	PID_y_CAM.D = 0.1;	// 微分

	PID_y_CAM.Expected = 225; // 期望值

	PID_y_CAM.err = 0;		// 偏差
	PID_y_CAM.last_err = 0; // 上一次偏差
	PID_y_CAM.integral = 0; // 偏差总和（积分值）
	PID_y_CAM.I_Max = 8;	// 积分限幅
	PID_y_CAM.umax = 20;
	PID_y_CAM.umin = -20;

	PID_y_CAM.out = 0; // 输出

	/*
	************************************
	*			PID_PWMA参数初始化							//速度环
	************************************
	*/
	PID_PWMA.P = 0.8;  // 比例
	PID_PWMA.I = 0.15; // 积分
	PID_PWMA.D = 20;   // 微分

	PID_PWMA.Expected = 40; // 期望值

	PID_PWMA.err = 0;	   // 偏差
	PID_PWMA.last_err = 0; // 上一次偏差
	PID_PWMA.integral = 0; // 偏差总和（积分值）
	PID_PWMA.I_Max = 100;  // 积分限幅
	PID_PWMA.I_sep = 40;
	PID_PWMA.umax = 999;
	PID_PWMA.umin = -999;

	PID_PWMA.out = 0; // 输出
	/*
	************************************
	*			PID_PWMB参数初始化							//速度环
	************************************
	*/
	PID_PWMB.P = 0.8;  // 比例
	PID_PWMB.I = 0.15; // 积分
	PID_PWMB.D = 20;   // 微分

	PID_PWMB.Expected = 30; // 期望值

	PID_PWMB.err = 0;	   // 偏差
	PID_PWMB.last_err = 0; // 上一次偏差
	PID_PWMB.integral = 0; // 偏差总和（积分值）
	PID_PWMB.I_Max = 100;  // 积分限幅
	PID_PWMB.I_sep = 40;
	PID_PWMB.umax = 999;
	PID_PWMB.umin = -999;

	PID_PWMB.out = 0; // 输出

	/*
	************************************
	*			PID_PWMC参数初始化							//速度环
	************************************
	*/
	PID_PWMC.P = 6; // 比例
	PID_PWMC.I = 2; // 积分
	PID_PWMC.D = 0; // 微分

	PID_PWMC.Expected = 10; // 期望值

	PID_PWMC.err = 0;	   // 偏差
	PID_PWMC.last_err = 0; // 上一次偏差
	PID_PWMC.integral = 0; // 偏差总和（积分值）
	PID_PWMC.I_Max = 100;  // 积分限幅
	PID_PWMC.I_sep = 40;
	PID_PWMC.umax = 999;
	PID_PWMC.umin = -999;

	PID_PWMC.out = 0; // 输出

	/*
	************************************
	*			PID_PWMD参数初始化							//速度环
	************************************
	*/
	PID_PWMD.P = 6; // 比例
	PID_PWMD.I = 2; // 积分
	PID_PWMD.D = 0; // 微分

	PID_PWMD.Expected = 0; // 期望值

	PID_PWMD.err = 0;	   // 偏差
	PID_PWMD.last_err = 0; // 上一次偏差
	PID_PWMD.integral = 0; // 偏差总和（积分值）
	PID_PWMD.I_Max = 100;  // 积分限幅
	PID_PWMD.I_sep = 40;
	PID_PWMD.umax = 999;
	PID_PWMD.umin = -999;

	PID_PWMD.out = 0; // 输出
}

float ControlPID1(float Actual)
{
	PID_1.err = PID_1.Expected - Actual;
	PID_1.integral += PID_1.err;

	if (PID_1.integral > PID_1.I_Max)
		PID_1.integral = PID_1.I_Max;
	if (PID_1.integral < -PID_1.I_Max)
		PID_1.integral = (-PID_1.I_Max);

	PID_1.out = PID_1.P * PID_1.err + PID_1.I * PID_1.integral + PID_1.D * (PID_1.err - PID_1.last_err);
	PID_1.last_err = PID_1.err; // 用本次偏差值替换上次偏差值

	if (PID_1.out > PID_1.umax)
		PID_1.out = PID_1.umax;
	if (PID_1.out < PID_1.umin)
		PID_1.out = PID_1.umin;

	return PID_1.out;
}

float ControlPID6050(float Actual)
{
	PID_6050.err = PID_6050.Expected - Actual;
	PID_6050.integral += PID_6050.err;

	if (PID_6050.integral > PID_6050.I_Max)
		PID_6050.integral = PID_6050.I_Max;
	if (PID_6050.integral < -PID_6050.I_Max)
		PID_6050.integral = (-PID_6050.I_Max);

	PID_6050.out = PID_6050.P * PID_6050.err + PID_6050.I * PID_6050.integral + PID_6050.D * (PID_6050.err - PID_6050.last_err);
	PID_6050.last_err = PID_6050.err; // 用本次偏差值替换上次偏差值

	if (PID_6050.out > PID_6050.umax)
		PID_6050.out = PID_6050.umax;
	if (PID_6050.out < PID_6050.umin)
		PID_6050.out = PID_6050.umin;

	return PID_6050.out;
}

float ControlPID2(float Actual)
{
	PID_2.err = PID_2.Expected - Actual;
	PID_2.integral += PID_2.err;

	if (PID_2.integral > PID_2.I_Max)
		PID_2.integral = PID_2.I_Max;
	if (PID_2.integral < -PID_2.I_Max)
		PID_2.integral = (-PID_2.I_Max);

	PID_2.out = PID_2.P * PID_2.err + PID_2.I * PID_2.integral + PID_2.D * (PID_2.err - PID_2.last_err);
	PID_2.last_err = PID_2.err; // 用本次偏差值替换上次偏差值

	if (PID_2.out > PID_2.umax)
		PID_2.out = PID_2.umax;
	if (PID_2.out < PID_2.umin)
		PID_2.out = PID_2.umin;

	return PID_2.out;
}

float ControlPID3(float Actual)
{
	PID_3.err = PID_3.Expected - Actual;
	PID_3.integral += PID_3.err;

	if (PID_3.integral > PID_3.I_Max)
		PID_3.integral = PID_3.I_Max;
	if (PID_3.integral < -PID_3.I_Max)
		PID_3.integral = (-PID_3.I_Max);

	PID_3.out = PID_3.P * PID_3.err + PID_3.I * PID_3.integral + PID_3.D * (PID_3.err - PID_3.last_err);
	PID_3.last_err = PID_3.err; // 用本次偏差值替换上次偏差值

	if (PID_3.out > PID_3.umax)
		PID_3.out = PID_3.umax;
	if (PID_3.out < PID_3.umin)
		PID_3.out = PID_3.umin;

	return PID_3.out;
}

float ControlPID4(float Actual)
{
	PID_4.err = PID_4.Expected - Actual;
	PID_4.integral += PID_4.err;

	if (PID_4.integral > PID_4.I_Max)
		PID_4.integral = PID_4.I_Max;
	if (PID_4.integral < -PID_4.I_Max)
		PID_4.integral = (-PID_4.I_Max);

	PID_4.out = PID_4.P * PID_4.err + PID_4.I * PID_4.integral + PID_4.D * (PID_4.err - PID_4.last_err);
	PID_4.last_err = PID_4.err; // 用本次偏差值替换上次偏差值

	if (PID_4.out > PID_4.umax)
		PID_4.out = PID_4.umax;
	if (PID_4.out < PID_4.umin)
		PID_4.out = PID_4.umin;

	PID_4.out = -PID_4.out;
	return PID_4.out;
}

float Cam_x_PID5(u16 Actual)
{
	PID_x_CAM.err = PID_x_CAM.Expected - Actual;
	PID_x_CAM.integral += PID_x_CAM.err;

	if (PID_x_CAM.integral > PID_x_CAM.I_Max)
		PID_x_CAM.integral = PID_x_CAM.I_Max;
	if (PID_x_CAM.integral < -PID_x_CAM.I_Max)
		PID_x_CAM.integral = (-PID_x_CAM.I_Max);

	PID_x_CAM.out = PID_x_CAM.P * PID_x_CAM.err + PID_x_CAM.I * PID_x_CAM.integral + PID_x_CAM.D * (PID_x_CAM.err - PID_x_CAM.last_err);
	PID_x_CAM.last_err = PID_x_CAM.err; // 用本次偏差值替换上次偏差值

	if (PID_x_CAM.out > PID_x_CAM.umax)
		PID_x_CAM.out = PID_x_CAM.umax;
	if (PID_x_CAM.out < PID_x_CAM.umin)
		PID_x_CAM.out = PID_x_CAM.umin;

	return PID_x_CAM.out;
}

float Cam_y_PID6(u16 Actual)
{
	PID_y_CAM.err = PID_y_CAM.Expected - Actual;
	PID_y_CAM.integral += PID_y_CAM.err;

	if (PID_y_CAM.integral > PID_y_CAM.I_Max)
		PID_y_CAM.integral = PID_y_CAM.I_Max;
	if (PID_y_CAM.integral < -PID_y_CAM.I_Max)
		PID_y_CAM.integral = (-PID_y_CAM.I_Max);

	PID_y_CAM.out = PID_y_CAM.P * PID_y_CAM.err + PID_y_CAM.I * PID_y_CAM.integral + PID_y_CAM.D * (PID_y_CAM.err - PID_y_CAM.last_err);
	PID_y_CAM.last_err = PID_y_CAM.err; // 用本次偏差值替换上次偏差值

	if (PID_y_CAM.out > PID_y_CAM.umax)
		PID_y_CAM.out = PID_y_CAM.umax;
	if (PID_y_CAM.out < PID_y_CAM.umin)
		PID_y_CAM.out = PID_y_CAM.umin;

	return PID_y_CAM.out;
}

float PWMA_PID7(float Actual)
{
	PID_PWMA.err = PID_PWMA.Expected - Actual;
	PID_PWMA.out += PID_PWMA.P * (PID_PWMA.err - PID_PWMA.last_err) + PID_PWMA.I * PID_PWMA.err;
	PID_PWMA.last_err = PID_PWMA.err; // 用本次偏差值替换上次偏差值

	if (PID_PWMA.out > PID_PWMA.umax)
		PID_PWMA.out = PID_PWMA.umax;
	if (PID_PWMA.out < PID_PWMA.umin)
		PID_PWMA.out = PID_PWMA.umin;

	return PID_PWMA.out;
}

float PWMB_PID8(float Actual)
{
	PID_PWMB.err = PID_PWMB.Expected - Actual;
	PID_PWMB.out += PID_PWMB.P * (PID_PWMB.err - PID_PWMB.last_err) + PID_PWMB.I * PID_PWMB.err;
	PID_PWMB.last_err = PID_PWMB.err; // 用本次偏差值替换上次偏差值

	if (PID_PWMB.out > PID_PWMB.umax)
		PID_PWMB.out = PID_PWMB.umax;
	if (PID_PWMB.out < PID_PWMB.umin)
		PID_PWMB.out = PID_PWMB.umin;

	return PID_PWMB.out;
}

float PWMC_PID9(float Actual)
{
	PID_PWMC.err = PID_PWMC.Expected - Actual;
	PID_PWMC.out += PID_PWMC.P * (PID_PWMC.err - PID_PWMC.last_err) + PID_PWMC.I * PID_PWMC.err;
	PID_PWMC.last_err = PID_PWMC.err; // 用本次偏差值替换上次偏差值

	if (PID_PWMC.out > PID_PWMC.umax)
		PID_PWMC.out = PID_PWMC.umax;
	if (PID_PWMC.out < PID_PWMC.umin)
		PID_PWMC.out = PID_PWMC.umin;

	return PID_PWMC.out;
}

float PWMD_PID10(float Actual)
{
	PID_PWMD.err = PID_PWMD.Expected - Actual;
	PID_PWMD.out += PID_PWMD.P * (PID_PWMD.err - PID_PWMD.last_err) + PID_PWMD.I * PID_PWMD.err;
	PID_PWMD.last_err = PID_PWMD.err; // 用本次偏差值替换上次偏差值

	if (PID_PWMD.out > PID_PWMD.umax)
		PID_PWMD.out = PID_PWMD.umax;
	if (PID_PWMD.out < PID_PWMD.umin)
		PID_PWMD.out = PID_PWMD.umin;

	return PID_PWMD.out;
}

// float PWMB_PID8(float Actual)
//{
//	int index=0;
//	PID_PWMB.err=PID_PWMB.Expected-Actual;
//	if(PID_PWMB.out>PID_PWMB.umax)         //抗积分饱和  执行机构已达极限
//	{
//		//积分分离
//		if(fabs(PID_PWMB.err)>=PID_PWMB.I_sep)
//		{
//			index = 0;
//		}
//		else
//		{
//			index = 1;
//			if(PID_PWMB.err<0)
//			PID_PWMB.integral += PID_PWMB.err;
//		}
//	}
//	else if(PID_PWMB.out<PID_PWMB.umin)
//	{
//		//积分分离
//		if(fabs(PID_PWMB.err)>=PID_PWMB.I_sep)
//		{
//			index = 0;
//		}
//		else
//		{
//			index = 1;
//			if(PID_PWMB.err>0)
//			PID_PWMB.integral += PID_PWMB.err;
//		}
//	}
//	else
//	{
//		//积分分离
//		if(fabs(PID_PWMB.err)>=PID_PWMB.I_sep)
//		{
//			index = 0;
//		}
//		else
//		{
//			index = 1;
//			PID_PWMB.integral += PID_PWMB.err;
//		}
//	}

//	if(PID_PWMB.integral>	PID_PWMB.I_Max)
//		PID_PWMB.integral=	PID_PWMB.I_Max;
//	if(PID_PWMB.integral< -PID_PWMB.I_Max)
//		PID_PWMB.integral= (-PID_PWMB.I_Max);
//
//	PID_PWMB.out = PID_PWMB.P*PID_PWMB.err	+	index*PID_PWMB.I*PID_PWMB.integral	+	PID_PWMB.D*(PID_PWMB.err-PID_PWMB.last_err);
//	PID_PWMB.last_err = PID_PWMB.err;								//用本次偏差值替换上次偏差值

//	return PID_PWMB.out;
//}
