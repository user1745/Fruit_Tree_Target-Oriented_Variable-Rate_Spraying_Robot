#include "sys.h"
#include "Control.h"

//****************闭环纵行**********************//
// 闭环前行
// 左溜边为dire=dir_left   右溜边为dire=dir_right
void Go_straight_Y(int dire, int distance)
{
	Mpu_Y_flag = 1;
	PID_2.Expected = distance;
	PID_3.Expected = distance;
	if (dire == dir_left) // 左溜边
	{
		Ultra_LO_flag = 1;

		OLED_Show3FNum(0, 12, PID_2.err, 2, 2, 12);
		OLED_ShowNum(50, 12, UL_Dis.Behind_Left_Outside, 3, 12);
		OLED_Show3FNum(0, 24, PID_3.err, 2, 2, 12);
		OLED_ShowNum(50, 24, UL_Dis.Front_Left_Outside, 3, 12);
		OLED_Show3FNum(0, 36, PID_1.err, 2, 1, 12);
		OLED_ShowIntNum(50, 36, mpu_yaw, 3, 12);
		OLED_Refresh_Gram();

		D_value_X = (PID_2.out + PID_3.out) / 2;
	}

	if (dire == dir_right) // 右溜边
	{
		Ultra_RO_flag = 1;

		OLED_Show3FNum(0, 12, PID_2.err, 2, 2, 12);
		OLED_ShowNum(50, 12, UL_Dis.Behind_Right_Outside, 3, 12);
		OLED_Show3FNum(0, 24, PID_3.err, 2, 2, 12);
		OLED_ShowNum(50, 24, UL_Dis.Front_Right_Outside, 3, 12);
		OLED_Show3FNum(0, 36, PID_1.err, 2, 1, 12);
		OLED_ShowIntNum(50, 36, mpu_yaw, 3, 12);
		OLED_Refresh_Gram();

		D_value_X = -(PID_2.out + PID_3.out) / 2;
	}

	//		printf("MPU6050:  %.3f   %.3f     %.2f\r\n\r\n",mpu_yaw,PID_1.err,PID_1.out);
	//		printf("BLO:      %d     %.3f     %.2f\r\n\r\n",UL_Dis.Behind_Left_Outside,PID_2.err,PID_2.out);
	//		printf("FLO:      %d     %.3f     %.2f\r\n\r\n\r\n",UL_Dis.Front_Left_Outside,PID_3.err,PID_3.out);
	//		printf("MotorA:   %.3f   %.3f   %.2f\r\n\r\n",Encoders.speedA,PID_PWMA.err,PID_PWMA.out);
	//		printf("MotorB:   %.3f   %.3f   %.2f\r\n\r\n",Encoders.speedB,PID_PWMB.err,PID_PWMB.out);
	//		printf("MotorC:   %.3f   %.3f   %.2f\r\n\r\n",Encoders.speedC,PID_PWMC.err,PID_PWMC.out);
	//		printf("MotorD:   %.3f   %.3f   %.2f\r\n\r\n\r\n\r\n",Encoders.speedD,PID_PWMD.err,PID_PWMD.out);
	//		delay_ms(100);

	D_value_Y = base_speed_y;
	D_value_Z = PID_6050.out;

	delay_ms(1);
	Motor_Analysis(D_value_X, D_value_Y, D_value_Z);

	if (motor_delay_flag == 1)
	{
		Motor_Set(0, 0, 0, 0);
		delay_ms(1000);
		delay_ms(1000);
		PID_PWMA.out = 0;
		PID_PWMB.out = 0;
		PID_PWMC.out = 0;
		PID_PWMD.out = 0;
		motor_cnt_flag = 1;
		motor_delay_flag = 0;
	}
}

// 闭环后行
// 左溜边为dire=dir_left   右溜边为dire=dir_right
void Go_back_Y(int dire, int distance)
{
	Mpu_Y_flag = 1;
	PID_2.Expected = distance;
	PID_3.Expected = distance;
	if (dire == dir_left) // 左溜边
	{
		Ultra_LO_flag = 1;

		OLED_Show3FNum(0, 30, PID_2.err, 2, 2, 12);
		OLED_ShowNum(0, 50, UL_Dis.Behind_Left_Outside, 3, 12);
		OLED_Show3FNum(40, 30, PID_3.err, 2, 2, 12);
		OLED_ShowNum(40, 50, UL_Dis.Front_Left_Outside, 3, 12);
		OLED_Show3FNum(60, 30, PID_1.err, 2, 1, 12);
		OLED_ShowIntNum(60, 50, mpu_yaw, 3, 12);
		OLED_Refresh_Gram();

		D_value_X = (PID_2.out + PID_3.out) / 2;
	}

	if (dire == dir_right) // 右溜边
	{
		Ultra_RO_flag = 1;

		OLED_Show3FNum(0, 30, PID_2.err, 2, 2, 12);
		OLED_ShowNum(0, 50, UL_Dis.Behind_Right_Outside, 3, 12);
		OLED_Show3FNum(40, 30, PID_3.err, 2, 2, 12);
		OLED_ShowNum(40, 50, UL_Dis.Front_Right_Outside, 3, 12);
		OLED_Show3FNum(60, 30, PID_1.err, 2, 1, 12);
		OLED_ShowIntNum(60, 50, mpu_yaw, 3, 12);
		OLED_Refresh_Gram();

		D_value_X = -(PID_2.out + PID_3.out) / 2;
	}

	D_value_Y = -base_speed_y;
	D_value_Z = PID_6050.out;

	delay_ms(1);
	Motor_Analysis(D_value_X, D_value_Y, D_value_Z);

	if (motor_delay_flag == 1)
	{
		Motor_Set(0, 0, 0, 0);
		delay_ms(1000);
		delay_ms(1000);
		PID_PWMA.out = 0;
		PID_PWMB.out = 0;
		PID_PWMC.out = 0;
		PID_PWMD.out = 0;
		motor_cnt_flag = 1;
		motor_delay_flag = 0;
	}
}

//****************闭环横行**********************//
// 闭环右行
// 左侧超声波工作为dire=dir_left     右侧超声波工作为dire=dir_right
void Go_Right_X(int dire, int distance)
{
	Mpu_X_flag = 1;
	PID_4.Expected = distance;
	if (dire == dir_right) // 右侧超声波工作
	{
		//		Ultra_RI_flag=1;
		OLED_Show3FNum(0, 12, PID_2.err, 2, 2, 12);
		OLED_ShowNum(50, 12, UL_Dis.Behind_Right_Inside, 3, 12);
		OLED_Show3FNum(0, 24, PID_3.err, 2, 2, 12);
		OLED_ShowNum(50, 24, UL_Dis.Front_Right_Inside, 3, 12);
		OLED_Show3FNum(0, 36, PID_1.err, 2, 1, 12);
		OLED_ShowIntNum(50, 36, mpu_yaw, 3, 12);
		OLED_Refresh_Gram();
	}

	if (dire == dir_left) // 左侧超声波工作
	{
		Ultra_LI_flag = 1;

		OLED_Show3FNum(0, 12, PID_4.err, 2, 2, 12);
		OLED_ShowNum(50, 12, UL_Dis.Behind_Left_Inside, 3, 12);

		OLED_Show3FNum(0, 24, PID_1.err, 2, 1, 12);
		OLED_ShowIntNum(50, 24, mpu_yaw, 3, 12);
		OLED_Refresh_Gram();
	}

	printf("openmv_flag:   %d   %d  \r\n\r\n", openmv_Wide_flag, openmv_Narrow_flag);
	delay_ms(20);

	D_value_X = base_speed_x;
	D_value_Y = PID_4.out;
	D_value_Z = PID_1.out;

	delay_ms(1);
	Motor_Analysis(D_value_X, D_value_Y, D_value_Z);
}

// 闭环左行
// 左侧超声波工作为dire=dir_left     右侧超声波工作为dire=dir_right
void Go_Left_X(int dire, int distance)
{
	Mpu_X_flag = 1;
	PID_4.Expected = distance;
	if (dire == dir_right) // 右侧超声波工作
	{
		//		Ultra_RI_flag=1;
		OLED_Show3FNum(0, 12, PID_2.err, 2, 2, 12);
		OLED_ShowNum(50, 12, UL_Dis.Behind_Right_Inside, 3, 12);
		OLED_Show3FNum(0, 24, PID_3.err, 2, 2, 12);
		OLED_ShowNum(50, 24, UL_Dis.Front_Right_Inside, 3, 12);
		OLED_Show3FNum(0, 36, PID_1.err, 2, 1, 12);
		OLED_ShowIntNum(50, 36, mpu_yaw, 3, 12);
		OLED_Refresh_Gram();
	}

	if (dire == dir_left) // 左侧超声波工作
	{
		Ultra_LI_flag = 1;

		OLED_Show3FNum(0, 12, PID_4.err, 2, 2, 12);
		OLED_ShowNum(50, 12, UL_Dis.Behind_Left_Inside, 3, 12);

		OLED_Show3FNum(0, 24, PID_1.err, 2, 1, 12);
		OLED_ShowIntNum(50, 24, mpu_yaw, 3, 12);
		OLED_Refresh_Gram();
	}

	printf("openmv_flag:   %d   %d  \r\n\r\n", openmv_Wide_flag, openmv_Narrow_flag);
	delay_ms(20);

	D_value_X = -base_speed_x;
	D_value_Y = PID_4.out;
	D_value_Z = PID_1.out;

	delay_ms(1);
	Motor_Analysis(D_value_X, D_value_Y, D_value_Z);
}

//****************半闭环纵行**********************//
// 半闭环前行
// 左溜边为dire=dir_left     右溜边为dire=dir_right
// 用前边的超声波闭环为mood_forward
// 用后边的超声波闭环为mood_back
void Go_straight_y(int dire, int distance, int mood)
{
	Mpu_Y_flag = 1;
	PID_2.Expected = distance;
	PID_3.Expected = distance;
	if (dire == dir_left) // 左溜边
	{
		Ultra_LO_flag = 1;

		OLED_Show3FNum(0, 12, PID_2.err, 2, 2, 12);
		OLED_ShowNum(50, 12, UL_Dis.Behind_Left_Outside, 3, 12);
		OLED_Show3FNum(0, 24, PID_3.err, 2, 2, 12);
		OLED_ShowNum(50, 24, UL_Dis.Front_Left_Outside, 3, 12);
		OLED_Show3FNum(0, 36, PID_1.err, 2, 1, 12);
		OLED_ShowIntNum(50, 36, mpu_yaw, 3, 12);
		OLED_Refresh_Gram();
	}

	if (dire == dir_right) // 右溜边
	{
		Ultra_RO_flag = 1;

		OLED_Show3FNum(0, 12, PID_2.err, 2, 2, 12);
		OLED_ShowNum(50, 12, UL_Dis.Behind_Right_Outside, 3, 12);
		OLED_Show3FNum(0, 24, PID_3.err, 2, 2, 12);
		OLED_ShowNum(50, 24, UL_Dis.Front_Right_Outside, 3, 12);
		OLED_Show3FNum(0, 36, PID_1.err, 2, 1, 12);
		OLED_ShowIntNum(50, 36, mpu_yaw, 3, 12);
		OLED_Refresh_Gram();

		D_value_X = -(PID_2.out + PID_3.out) / 2;
	}

	if (mood == dir_forward) // 前边的超声波闭环
	{
		D_value_X = PID_3.out;
		D_value_Y = base_speed_y;
		D_value_Z = PID_6050.out;
	}

	if (mood == mood_back) // 后边的超声波闭环
	{
		D_value_X = PID_2.out;
		D_value_Y = base_speed_y;
		D_value_Z = PID_6050.out;
	}

	delay_ms(1);
	Motor_Analysis(D_value_X, D_value_Y, D_value_Z);

	if (motor_delay_flag == 1)
	{
		Motor_Set(0, 0, 0, 0);
		delay_ms(1000);
		delay_ms(1000);
		PID_PWMA.out = 0;
		PID_PWMB.out = 0;
		PID_PWMC.out = 0;
		PID_PWMD.out = 0;
		motor_cnt_flag = 1;
		motor_delay_flag = 0;
	}
}

// 半闭环后行
// 左溜边为dire=dir_left     右溜边为dire=dir_right
// 用前边的超声波闭环为mood_forward
// 用后边的超声波闭环为mood_back
void Go_back_y(int dire, int distance, int mood)
{
	Mpu_Y_flag = 1;
	PID_2.Expected = distance;
	PID_3.Expected = distance;
	if (dire == dir_left) // 左溜边
	{
		Ultra_LO_flag = 1;

		OLED_Show3FNum(60, 30, PID_2.err, 2, 2, 12);
		OLED_ShowNum(60, 50, UL_Dis.Behind_Left_Outside, 3, 12);
		OLED_Show3FNum(40, 30, PID_3.err, 2, 2, 12);
		OLED_ShowNum(40, 50, UL_Dis.Front_Left_Outside, 3, 12);
		OLED_Show3FNum(60, 30, PID_1.err, 2, 1, 12);
		OLED_ShowIntNum(60, 50, mpu_yaw, 3, 12);
		OLED_Refresh_Gram();
	}

	if (dire == dir_right) // 右溜边
	{
		Ultra_RO_flag = 1;

		OLED_Show3FNum(60, 30, PID_2.err, 2, 2, 12);
		OLED_ShowNum(60, 50, UL_Dis.Behind_Left_Outside, 3, 12);
		OLED_Show3FNum(40, 30, PID_3.err, 2, 2, 12);
		OLED_ShowNum(40, 50, UL_Dis.Front_Left_Outside, 3, 12);
		OLED_Show3FNum(60, 30, PID_1.err, 2, 1, 12);
		OLED_ShowIntNum(60, 50, mpu_yaw, 3, 12);
		OLED_Refresh_Gram();
	}

	if (mood == dir_forward) // 前边的超声波闭环
	{
		D_value_X = PID_3.out;
		D_value_Y = -base_speed_y;
		D_value_Z = PID_6050.out;
	}

	if (mood == mood_back) // 后边的超声波闭环
	{
		D_value_X = PID_2.out;
		D_value_Y = -base_speed_y;
		D_value_Z = PID_6050.out;
	}

	delay_ms(1);

	Motor_Analysis(D_value_X, D_value_Y, D_value_Z);

	if (motor_delay_flag == 1)
	{
		Motor_Set(0, 0, 0, 0);
		delay_ms(1000);
		delay_ms(1000);
		PID_PWMA.out = 0;
		PID_PWMB.out = 0;
		PID_PWMC.out = 0;
		PID_PWMD.out = 0;
		motor_cnt_flag = 1;
		motor_delay_flag = 0;
	}
}

//****************开环纵行**********************//
// 开环前行
void Go_straight()
{
	Mpu_Y_flag = 1;
	Motor_Analysis(0, base_speed_y, PID_6050.out);
}

// 开环后行
void Go_back()
{
	Mpu_Y_flag = 1;
	Motor_Analysis(0, -base_speed_y, PID_6050.out);
}

//****************开环横行**********************//
// 开环右行
void Go_Right()
{
	Mpu_X_flag = 1;
	Motor_Analysis(base_speed_x, 0, PID_1.out);
}

// 开环左行
void Go_Left()
{
	Mpu_X_flag = 1;
	Motor_Analysis(-base_speed_x, 0, PID_1.out);
}

//****************减速**********************//
void Slow_down(u8 speed) // PWM波需要加大
{
	PID_PWMA.out = PID_PWMA.out / 3.0f;
	PID_PWMB.out = PID_PWMB.out / 3.0f;
	PID_PWMC.out = PID_PWMC.out / 3.0f;
	PID_PWMD.out = PID_PWMD.out / 3.0f;
	base_speed_x = speed;
	PID_PWMA.Expected = speed;
	PID_PWMB.Expected = speed;
	PID_PWMC.Expected = speed;
	PID_PWMD.Expected = speed;
}

//****************停止**********************//
void Stop(void) // PWM波需要加大
{
	PID_PWMA.out = 0;
	PID_PWMB.out = 0;
	PID_PWMC.out = 0;
	PID_PWMD.out = 0;
	base_speed_x = base_speed;
	PID_PWMA.Expected = 0;
	PID_PWMB.Expected = 0;
	PID_PWMC.Expected = 0;
	PID_PWMD.Expected = 0;
	MotorAB_Set(0, 0);
	MotorCD_Set(0, 0);
}

// 开环前行
void Start()
{
	Mpu_Y_flag = 1;
	Motor_Analysis(0, 60, PID_6050.out);
}
