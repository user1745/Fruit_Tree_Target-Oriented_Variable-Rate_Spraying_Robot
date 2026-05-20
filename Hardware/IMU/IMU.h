#ifndef __IMU_H
#define __IMU_H

#include "sys.h"
#include "mpu9250.h"

/*
******************************************************************************************************************
*                                            CONSTANTS & MACROS
*Description : none
*Notes       : none
******************************************************************************************************************
*/
// #define PI      3.1415926f
#define RtA 57.324841f	  // 弧度到角度
#define AtR 0.0174533f	  // 角到角度
#define Acc_G 0.0011963f  // 加速度变成G
#define Gyro_G 0.0152672f // 角速度变成度

#define Gyro_Gr Gyro_G *PI / 180.0f

// #define IMU_ReadData()  MPU9250_ReadValue()
#define IMU_ReadData()                                                                              \
	{                                                                                               \
		MPU_Get_Gyroscope(&MPU_origistrure.groyy, &MPU_origistrure.groyx, &MPU_origistrure.groyz);  \
		MPU_Get_Accelerometer(&MPU_origistrure.accx, &MPU_origistrure.accy, &MPU_origistrure.accz); \
	}

#define MPU_Init_FilterNUM 20 // 初始化滤波次数，得到一个初始的偏置值

typedef struct
{
	float X;
	float Y;
	float Z;
} S_FLOAT_XYZ;

typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;
} S_INT16_XYZ;

typedef struct
{ // 保存陀螺仪原始数据的结构体
	int16_t accx;
	int16_t accy;
	int16_t accz;

	int16_t groyx;
	int16_t groyy;
	int16_t groyz;
} MPU_origistru;

typedef struct
{
	float X;
	float Y;
	float Z;
} MPU_ACC;

typedef struct
{
	float ax;
	float ay;
	float az;
	int gx;
	int gy;
	int gz;
} DATA;

/*
******************************************************************************************************************
*                                            FUNCTION PROTOTYPES
*****************************************************************************************************************
*/
extern MPU_origistru MPU_origistrure; // 保存陀螺仪原始数据
extern S_FLOAT_XYZ Q_ANGLE;			  // 四元数计算出的角度
extern S_FLOAT_XYZ GYRO_I;			  // 陀螺仪积分
extern S_INT16_XYZ ACC_AVG;			  // 平均滤波后的加速度
extern MPU_ACC mpu_acc;				  // 获取加速度数组
extern DATA MPU_Initdata;			  // MPU初始化偏置值

void Prepare_Data(void);
void Get_Attitude(void);
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);
void Get_ACC(float AX, float AY, float AZ);
void MPU_Data_Init(void);
#endif
