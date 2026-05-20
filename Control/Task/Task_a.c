#include "Task_a.h"
#include "inv_mpu.h"
#include "Usart.h"
#include "NRF24L01.h"

/***********************基础任务①遍历垄道巡检***********************/
void Task_a_1(void)
{
	OLED_ShowString(12, 0, (void *)"Task_a_1 Running", 12);
	OLED_Refresh_Gram();
	/**********************************************************/
	while (1)
	{
		// 0 开环前行****************************************//
		while (1)
		{
			Start();
			if (UL_Dis.Front_Left_Outside < 15)
			{
				//				Stop();
				//				delay_ms(50);
				break;
			}
		}

		//
		motor_cnt_flag = 1;
		//

		// 1 半闭环前行，左溜边****************************************//

		while (1)
		{
			Go_straight_y(dir_left, 5, mood_forward);
			if (UL_Dis.Behind_Left_Outside < 15)
			{
				//				Stop();
				//				delay_ms(50);
				break;
			}
		}

		// 2 闭环前行，左溜边****************************************//

		while (1)
		{
			Go_straight_Y(dir_left, 5);
			if (UL_Dis.Laser_dis >= 6 && UL_Dis.Laser_dis <= 15)
			{
				Stop(); // 准备进入第一垄
				delay_ms(200);
				break;
			}
		}

		// 3 开环右行 ->第一垄****************************************//

		while (1)
		{
			Go_Right();
			if (UL_Dis.Behind_Left_Inside < 20)
			{
				//				Stop();
				//				delay_ms(10);
				break;
			}
		}

		// 4 闭环右行 第一垄****************************************//

		while (1)
		{
			Go_Right_X(dir_left, 10);
			if (UL_Dis.Front_Right_Outside < 40 && UL_Dis.Behind_Right_Outside < 40)
			{
				Stop(); // 准备进入第一垄
				//				delay_ms(10);
				break;
			}
		}

		// 5 开环右行 第一垄****************************************//

		while (1)
		{
			Go_Right();
			if (UL_Dis.Front_Right_Outside < 10 || UL_Dis.Behind_Right_Outside < 10)
			{
				Stop();
				delay_ms(200);
				break;
			}
		}

		// 6 闭环后行 右溜边 第一垄->第二垄****************************************//

		while (1)
		{
			Go_back_Y(dir_right, 5);
			if (UL_Dis.Laser_dis >= 80)
			{
				Stop(); // 准备进入第二垄
				delay_ms(200);
				break;
			}
		}

		// 7 开环左行 ->第二垄****************************************//

		while (1)
		{
			Go_Left();
			if (UL_Dis.Behind_Left_Inside < 20)
			{
				//				Stop();
				//				delay_ms(10);
				break;
			}
		}

		// 8 闭环左行 第二垄****************************************//

		while (1)
		{
			Go_Left_X(dir_left, 10);
			if (UL_Dis.Front_Left_Outside < 40 && UL_Dis.Behind_Left_Outside < 40)
			{
				//				Stop();
				//				delay_ms(10);
				break;
			}
		}

		// 9 开环左行 第二垄****************************************//

		while (1)
		{
			Go_Left();
			if (UL_Dis.Front_Left_Outside < 10 || UL_Dis.Behind_Left_Outside < 10)
			{
				Stop();
				delay_ms(200);
				break;
			}
		}

		// 10 闭环后行 左溜边 第二垄->第三垄****************************************//

		while (1)
		{
			Go_back_Y(dir_left, 5);
			if (UL_Dis.Laser_dis > 155)
			{
				Stop(); // 准备进入第三垄
				delay_ms(200);
				break;
			}
		}

		// 11 开环右行  ->第三垄****************************************//

		while (1)
		{
			Go_Right();
			if (UL_Dis.Behind_Left_Inside < 20)
			{
				//				Stop();
				//				delay_ms(10);
				break;
			}
		}

		// 12 闭环右行  第三垄****************************************//

		while (1)
		{
			Go_Right_X(dir_left, 10);
			if (UL_Dis.Front_Right_Outside < 40 && UL_Dis.Behind_Right_Outside < 40)
			{
				//				Stop();
				//				delay_ms(10);
				break;
			}
		}

		// 13 开环右行  第三垄****************************************//

		while (1)
		{
			Go_Right();
			if (UL_Dis.Front_Right_Outside < 10 || UL_Dis.Behind_Right_Outside < 10)
			{
				Stop();
				delay_ms(200);
				break;
			}
		}

		// 14 闭环后行 右溜边  第三垄->第四垄****************************************//

		while (1)
		{
			Go_back_Y(dir_right, 5);
			if (UL_Dis.Behind_Right_Inside < 85)
			{
				Stop(); // 准备进入第四垄
				delay_ms(200);
				break;
			}
		}

		// 15 开环左行  -> 第四垄****************************************//

		while (1)
		{
			Go_Left();
			if (UL_Dis.Behind_Left_Inside < 20)
			{
				//				Stop();
				//				delay_ms(10);
				break;
			}
		}

		// 16 闭环左行  第四垄****************************************//

		while (1)
		{
			PID_1.Expected = -0.2;
			Go_Left_X(dir_left, 10);
			if (UL_Dis.Front_Left_Outside < 40 && UL_Dis.Behind_Left_Outside < 40)
			{
				//				Stop();
				//				delay_ms(10);
				break;
			}
		}

		// 17 闭环右行  - >第五垄****************************************//

		while (1)
		{
			Go_Right_X(dir_left, 10);
			if (UL_Dis.Front_Right_Outside < 40 && UL_Dis.Behind_Right_Outside < 40)
			{
				//				Stop();
				//				delay_ms(10);
				break;
			}
		}

		// 18 开环右行   第五垄****************************************//

		while (1)
		{
			Go_Right();
			if (UL_Dis.Front_Right_Outside < 10 || UL_Dis.Behind_Right_Outside < 10)
			{
				Stop();
				delay_ms(200);
				break;
			}
		}

		// 19 闭环后行  第五垄->第六垄****************************************//

		while (1)
		{
			Go_back_Y(dir_right, 5);
			if (UL_Dis.Behind_Right_Inside < 18)
			{
				Stop(); // 准备进入第五垄
				delay_ms(200);
				break;
			}
		}

		// 20 闭环左行  第六垄****************************************//

		while (1)
		{
			PID_1.Expected = -0.5;
			Go_Left_X(dir_left, 10);
			if (UL_Dis.Front_Left_Outside < 40 && UL_Dis.Behind_Left_Outside < 40)
			{
				//				Stop();
				//				delay_ms(10);
				break;
			}
		}

		// 21 开环左行  第五垄****************************************//

		while (1)
		{
			Go_Left();
			if (UL_Dis.Front_Left_Outside < 10 || UL_Dis.Behind_Left_Outside < 10)
			{
				Stop();
				delay_ms(200);
				break;
			}
		}

		// 25 半闭环后行 左溜边****************************************//

		while (1)
		{
			Go_back_y(dir_left, 3, mood_forward);
			if (UL_Dis.Laser_dis > 375 || UL_Dis.Front_Left_Outside > 30)
			{
				//				Stop();
				//				delay_ms(500);
				break;
			}
		}

		// 26 开环后行****************************************//

		while (1)
		{
			Go_back();
			if ((UL_Dis.Laser_dis > 406) || (UL_Dis.Behind_Left_Inside > 5 && UL_Dis.Behind_Left_Inside < 15))
			{
				Stop();
				//				break;
			}
		}
	}
}

/***********************基础任务②***********************/
void Task_a_2(void)
{
	base_speed_x = 20;
	base_speed = 20;
	while (1)
	{
		// 0 开环前行****************************************//
		while (1)
		{
			Start();
			if (UL_Dis.Front_Left_Outside < 15)
			{
				//				Stop();
				//				delay_ms(50);
				break;
			}
		}
		//
		motor_cnt_flag = 1;
		//
		// 1 半闭环前行，左溜边****************************************//

		while (1)
		{
			Go_straight_y(dir_left, 5, mood_forward);
			if (UL_Dis.Behind_Left_Outside < 15)
			{
				//				Stop();
				//				delay_ms(50);
				break;
			}
		}

		// 2 闭环前行，左溜边****************************************//

		while (1)
		{
			Go_straight_Y(dir_left, 5);
			if (UL_Dis.Laser_dis >= 6 && UL_Dis.Laser_dis <= 15)
			{
				Stop(); // 准备进入第一垄
				delay_ms(200);
				break;
			}
		}

		// 3 开环右行 ->第一垄****************************************//

		while (1)
		{
			Go_Right();
			if (UL_Dis.Behind_Left_Inside < 20)
			{
				//				Stop();
				//				delay_ms(10);
				break;
			}
			if (openmv_control_flag == 1)
				openmv_control();
		}

		// 4 闭环右行 第一垄****************************************//

		while (1)
		{
			Go_Right_X(dir_left, 10);
			if (UL_Dis.Front_Right_Outside < 40 && UL_Dis.Behind_Right_Outside < 40)
			{
				Stop(); // 准备进入第一垄
				//				delay_ms(10);
				break;
			}
			if (openmv_control_flag == 1)
				openmv_control();
		}

		// 5 开环右行 第一垄****************************************//

		while (1)
		{
			Go_Right();
			if (UL_Dis.Front_Right_Outside < 10 || UL_Dis.Behind_Right_Outside < 10)
			{
				Stop();
				delay_ms(200);
				break;
			}
			if (openmv_control_flag == 1)
				openmv_control();
		}

		// 6 闭环后行 右溜边 第一垄->第二垄****************************************//

		while (1)
		{
			Go_back_Y(dir_right, 5);
			if (UL_Dis.Laser_dis >= 80)
			{
				Stop(); // 准备进入第二垄
				delay_ms(200);
				break;
			}
		}

		// 7 开环左行 ->第二垄****************************************//

		while (1)
		{
			Go_Left();
			if (UL_Dis.Behind_Left_Inside < 20)
			{
				//				Stop();
				//				delay_ms(10);
				break;
			}
			if (openmv_control_flag == 1)
				openmv_control();
		}

		// 8 闭环左行 第二垄****************************************//

		while (1)
		{
			Go_Left_X(dir_left, 10);
			if (UL_Dis.Front_Left_Outside < 40 && UL_Dis.Behind_Left_Outside < 40)
			{
				//				Stop();
				//				delay_ms(10);
				break;
			}
			if (openmv_control_flag == 1)
				openmv_control();
		}

		// 9 开环左行 第二垄****************************************//

		while (1)
		{
			Go_Left();
			if (UL_Dis.Front_Left_Outside < 10 || UL_Dis.Behind_Left_Outside < 10)
			{
				Stop();
				delay_ms(200);
				break;
			}
			if (openmv_control_flag == 1)
				openmv_control();
		}

		// 10 闭环后行 左溜边 第二垄->第三垄****************************************//

		while (1)
		{
			Go_back_Y(dir_left, 5);
			if (UL_Dis.Laser_dis > 155)
			{
				Stop(); // 准备进入第三垄
				delay_ms(200);
				break;
			}
		}

		// 11 开环右行  ->第三垄****************************************//

		while (1)
		{
			Go_Right();
			if (UL_Dis.Behind_Left_Inside < 20)
			{
				//				Stop();
				//				delay_ms(10);
				break;
			}
			if (openmv_control_flag == 1)
				openmv_control();
		}

		// 12 闭环右行  第三垄****************************************//

		while (1)
		{
			Go_Right_X(dir_left, 10);
			if (UL_Dis.Front_Right_Outside < 40 && UL_Dis.Behind_Right_Outside < 40)
			{
				//				Stop();
				//				delay_ms(10);
				break;
			}
			if (openmv_control_flag == 1)
				openmv_control();
		}

		// 13 开环右行  第三垄****************************************//

		while (1)
		{
			Go_Right();
			if (UL_Dis.Front_Right_Outside < 10 || UL_Dis.Behind_Right_Outside < 10)
			{
				Stop();
				delay_ms(200);
				break;
			}
			if (openmv_control_flag == 1)
				openmv_control();
		}

		// 14 闭环后行 右溜边  第三垄->第四垄****************************************//

		while (1)
		{
			Go_back_Y(dir_right, 5);
			if (UL_Dis.Behind_Right_Inside < 85)
			{
				Stop(); // 准备进入第四垄
				delay_ms(200);
				break;
			}
		}

		// 15 开环左行  -> 第四垄****************************************//

		while (1)
		{
			Go_Left();
			if (UL_Dis.Behind_Left_Inside < 20)
			{
				//				Stop();
				//				delay_ms(10);
				break;
			}
			if (openmv_control_flag == 1)
				openmv_control();
		}

		// 17 闭环右行  - >第五垄****************************************//

		while (1)
		{
			Go_Right_X(dir_left, 10);
			if (UL_Dis.Front_Right_Outside < 40 && UL_Dis.Behind_Right_Outside < 40)
			{
				//				Stop();
				//				delay_ms(10);
				break;
			}
		}

		// 18 开环右行   第五垄****************************************//

		while (1)
		{
			Go_Right();
			if (UL_Dis.Front_Right_Outside < 10 || UL_Dis.Behind_Right_Outside < 10)
			{
				Stop();
				delay_ms(200);
				break;
			}
		}

		// 19 闭环后行  第五垄->第六垄****************************************//

		while (1)
		{
			Go_back_Y(dir_right, 5);
			if (UL_Dis.Behind_Right_Inside < 18)
			{
				Stop(); // 准备进入第五垄
				delay_ms(200);
				break;
			}
		}

		// 20 闭环左行  第六垄****************************************//

		while (1)
		{
			PID_1.Expected = -0.5;
			Go_Left_X(dir_left, 10);
			if (UL_Dis.Front_Left_Outside < 40 && UL_Dis.Behind_Left_Outside < 40)
			{
				//				Stop();
				//				delay_ms(10);
				break;
			}
			if (openmv_control_flag == 1)
				openmv_control();
		}

		// 21 开环左行  第六垄****************************************//

		while (1)
		{
			Go_Left();
			if (UL_Dis.Front_Left_Outside < 10 || UL_Dis.Behind_Left_Outside < 10)
			{
				Stop();
				delay_ms(200);
				break;
			}
			if (openmv_control_flag == 1)
				openmv_control();
		}

		// 21 开环右行  第五垄****************************************//

		while (1)
		{
			Go_Right();
			if (UL_Dis.Front_Right_Outside < 10 || UL_Dis.Behind_Right_Outside < 10)
			{
				Stop();
				delay_ms(200);
				break;
			}
			if (openmv_control_flag == 1)
				openmv_control();
		}

		// 25 半闭环后行 左溜边****************************************//

		while (1)
		{
			Go_back_y(dir_left, 3, mood_forward);
			if (UL_Dis.Laser_dis > 375 || UL_Dis.Front_Left_Outside > 30)
			{
				//				Stop();
				//				delay_ms(500);
				break;
			}
		}

		// 26 开环后行****************************************//

		while (1)
		{
			Go_back();
			if ((UL_Dis.Laser_dis > 406) || (UL_Dis.Behind_Left_Inside > 5 && UL_Dis.Behind_Left_Inside < 15))
			{
				Stop();
				//				break;
			}
		}
	}
}

/***********************基础任务③***********************/
void Task_a_3(void)
{
	OLED_ShowString(12, 0, (void *)"Task_a_3 Running", 12);
	OLED_Refresh_Gram();

	while (1)
	{
		Go_Right_X(dir_left, 10);
		if (openmv_control_flag == 1)
			openmv_control();
	}
}

/***********************基础任务④***********************/
void Task_a_4(void)
{
	OLED_ShowString(12, 0, (void *)"Task_a_4 Running", 12);
	OLED_Refresh_Gram();
	while (1)
	{
		Go_Right_X(dir_left, 10);
		printf("X:        %d     %.3f   %.2f\r\n\r\n", red_x_Narrow, PID_x_CAM.err, PID_x_CAM.out);
		printf("Y:				%d     %.3f   %.2f\r\n\r\n", red_y_Narrow, PID_y_CAM.err, PID_y_CAM.out);
		printf("MPU6050:  %.3f   %.3f   %.2f\r\n\r\n", mpu_yaw, PID_1.err, PID_1.out);
		printf("BLI:      %d     %.3f   %.2f\r\n\r\n", UL_Dis.Behind_Left_Inside, PID_4.err, PID_4.out);
		printf("MotorA:   %.3f   %.3f   %.2f\r\n\r\n", Encoders.speedA, PID_PWMA.err, PID_PWMA.out);
		printf("MotorB:   %.3f   %.3f   %.2f\r\n\r\n", Encoders.speedB, PID_PWMB.err, PID_PWMB.out);
		printf("MotorC:   %.3f   %.3f   %.2f\r\n\r\n", Encoders.speedC, PID_PWMC.err, PID_PWMC.out);
		printf("MotorD:   %.3f   %.3f   %.2f\r\n\r\n\r\n\r\n", Encoders.speedD, PID_PWMD.err, PID_PWMD.out);
		delay_ms(120);
	}
}
