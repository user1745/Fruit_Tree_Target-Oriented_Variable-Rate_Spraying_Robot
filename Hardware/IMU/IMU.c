#include "IMU.h"
#include "arm_math.h"
// #include "Usart.h"
#include "Filter.h"

MPU_origistru MPU_origistrure; // 保存陀螺仪原始数据
/*
******************************************************************************************************************
*                                            CONSTANTS & MACROS
*Description : none
*Notes       : none
******************************************************************************************************************
*/
#define FILTER_NUM 20

#define Kp 10.0f	 // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.008f	 // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.001f // 采样周期一半

static float q0 = 1, q1 = 0, q2 = 0, q3 = 0; // quaternion elements representing the estimated orientation
static float pa = 0, pb = 0, pc = 0;
static float exInt = 0, eyInt = 0, ezInt = 0; // scaled integral error

S_INT16_XYZ ACC_AVG; // 平均滤波后的加速度
S_FLOAT_XYZ Q_ANGLE; // 四元数计算出来的角度
MPU_ACC mpu_acc;	 // 解算后加速度
DATA MPU_Initdata;	 // MPU初始化数据（offset）

FilterStruct1 MPU_Initdata_filter_AX = {{0}, 0};
FilterStruct1 MPU_Initdata_filter_AY = {{0}, 0};
FilterStruct1 MPU_Initdata_filter_AZ = {{0}, 0};

int16_t ACC_X_BUF[FILTER_NUM], ACC_Y_BUF[FILTER_NUM], ACC_Z_BUF[FILTER_NUM]; // 加速度滑动窗口滤波数组
/******************************************************************************************************************
 *                                             Prepare_Data()
 *
 *Description : 更新数据
 *Arguments   : none
 *Returns     : none
 *Notes       : none
 *******************************************************************************************************************
 */
void Prepare_Data(void)
{
	static uint8_t filter_cnt = 0;
	int32_t temp1 = 0, temp2 = 0, temp3 = 0;
	uint8_t i;

	IMU_ReadData(); // 得到数据

	ACC_X_BUF[filter_cnt] = MPU_origistrure.accx; // 更新滑动窗口数组
	ACC_Y_BUF[filter_cnt] = MPU_origistrure.accy;
	ACC_Z_BUF[filter_cnt] = MPU_origistrure.accz;
	for (i = 0; i < FILTER_NUM; i++)
	{
		temp1 += ACC_X_BUF[i];
		temp2 += ACC_Y_BUF[i];
		temp3 += ACC_Z_BUF[i];
	}

	ACC_AVG.X = temp1 / FILTER_NUM;
	ACC_AVG.Y = temp2 / FILTER_NUM;
	ACC_AVG.Z = temp3 / FILTER_NUM;
	filter_cnt++;
	if (filter_cnt == FILTER_NUM)
		filter_cnt = 0;
}
/******************************************************************************************************************
 *                                               Get_Attitude()
 *
 *Description : 获取姿态
 *Arguments   : 角速度以及加速度的x,y,z轴的分量
 *Returns     : none
 *Notes       : none
 *******************************************************************************************************************
 */
void Get_Attitude(void)
{
	IMUupdate(MPU_origistrure.groyx * Gyro_Gr,
			  MPU_origistrure.groyy * Gyro_Gr,
			  MPU_origistrure.groyz * Gyro_Gr,
			  ACC_AVG.X, ACC_AVG.Y, ACC_AVG.Z);
}
/******************************************************************************************************************
 *                                               invSqrt()
 *
 *Description : 开方
 *Arguments   : 要开方的数X
 *Returns     : X的开方之后的值
 *Notes       : none
 *******************************************************************************************************************
 */
float invSqrt(float x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long *)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float *)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}
/******************************************************************************************************************
 *                                               IMUupdate()
 *
 *Description : 姿态角更新
 *Arguments   : 角速度以及加速度的x,y,z轴的分量
 *Returns     : none
 *Notes       : none
 *******************************************************************************************************************
 */
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;
	float q0q0 = q0 * q0; // 计算需要用到数据
	float q0q1 = q0 * q1;
	float q0q2 = q0 * q2;
	float q1q1 = q1 * q1;
	float q1q3 = q1 * q3;
	float q2q2 = q2 * q2;
	float q2q3 = q2 * q3;
	float q3q3 = q3 * q3;

	if (ax * ay * az == 0)
		return;

	norm = invSqrt(ax * ax + ay * ay + az * az); // acc数据归一化  norm = invSqrt(ax*ax + ay*ay + az*az);
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;

	// 估计重力方向和流量/变迁
	vx = 2 * (q1q3 - q0q2); // 四元素中xyz的表示
	vy = 2 * (q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;

	// error is sum of cross product between reference direction of fields and direction measured by sensors
	ex = (ay * vz - az * vy); // 向量外积在相减得到差分就是误差
	ey = (az * vx - ax * vz);
	ez = (ax * vy - ay * vx);

	exInt += ex; // 对误差进行积分
	eyInt += ey;
	ezInt += ez;

	// adjusted gyroscope measurements
	gx = gx + Kp * ex + Ki * exInt; // 将误差PI后补偿到陀螺仪，即补偿零点漂移
	gy = gy + Kp * ey + Ki * eyInt;
	gz = gz + Kp * ez + Ki * ezInt; // 这里的gz由于没有观测者进行矫正会产生漂移，表现出来的就是积分自增或自减

	// integrate quaternion rate and normalise			   //四元素的微分方程
	pa = q1;
	pb = q2;
	pc = q3;
	q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
	q1 = pa + (q0 * gx + q2 * gz - q3 * gy) * halfT;
	q2 = pb + (q0 * gy - q1 * gz + q3 * gx) * halfT;
	q3 = pc + (q0 * gz + q1 * gy - q2 * gx) * halfT;

	// normalise quaternion
	norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 = q0 * norm;
	q1 = q1 * norm;
	q2 = q2 * norm;
	q3 = q3 * norm;

	Q_ANGLE.X = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3 + 2.1f;									// pitch
	Q_ANGLE.Y = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3 - 0.7f; // roll

	//	if((gz < 0.10f) && (gz > -0.10f))
	//	{
	//		gz = 0;
	//	}
	//	Q_ANGLE.Z += gz * 0.11459f;
}

/******************************************************************************************************************
 *                                  void Get_ACC(float AX,float AY,float AZ)
 *
 *Description : 解算三轴加速度
 *Arguments   : 加速度的x,y,z轴的分量
 *Returns     : none
 *Notes       : none
 *******************************************************************************************************************
 */
void Get_ACC(float AX, float AY, float AZ)
{
	mpu_acc.X = AX / 16384.0f * 9.8f;
	mpu_acc.Y = AY / 16384.0f * 9.8f;
	mpu_acc.Z = AZ / 16384.0f * 9.8f;
	mpu_acc.X = meanFilter1(&MPU_Initdata_filter_AX, mpu_acc.X, 5);
	mpu_acc.Y = meanFilter1(&MPU_Initdata_filter_AY, mpu_acc.Y, 5);
	mpu_acc.Z = meanFilter1(&MPU_Initdata_filter_AZ, mpu_acc.Z, 5);
}

/******************************************************************************************************************
 *                                  void MPU_Data_Init(void)
 *
 *Description : 陀螺仪初始化数据滤波
 *Arguments   : 加速度的x,y,z轴的分量
 *Returns     : none
 *Notes       : none
 *******************************************************************************************************************
 */
void MPU_Data_Init(void)
{
	u8 i = 0;
	float ACC_SUM1 = 0.0f, ACC_SUM2 = 0.0f, ACC_SUM3 = 0.0f;
	int GYRO_SUM1 = 0, GYRO_SUM2 = 0, GYRO_SUM3 = 0;
	for (i = 0; i < MPU_Init_FilterNUM; i++)
	{
		//		Prepare_Data();
		//		Get_Attitude();
		Get_ACC((float)ACC_AVG.X, (float)ACC_AVG.Y, (float)ACC_AVG.Z);
		ACC_SUM1 += mpu_acc.X;
		ACC_SUM2 += mpu_acc.Y;
		ACC_SUM3 += mpu_acc.Z;
		GYRO_SUM1 += MPU_origistrure.groyx;
		GYRO_SUM2 += MPU_origistrure.groyy;
		GYRO_SUM3 += MPU_origistrure.groyz;
	}
	MPU_Initdata.ax = ACC_SUM1 / MPU_Init_FilterNUM;
	MPU_Initdata.ay = ACC_SUM2 / MPU_Init_FilterNUM;
	MPU_Initdata.az = ACC_SUM3 / MPU_Init_FilterNUM;
	MPU_Initdata.gx = GYRO_SUM1 / MPU_Init_FilterNUM;
	MPU_Initdata.gy = GYRO_SUM2 / MPU_Init_FilterNUM;
	MPU_Initdata.gz = GYRO_SUM3 / MPU_Init_FilterNUM;
}
