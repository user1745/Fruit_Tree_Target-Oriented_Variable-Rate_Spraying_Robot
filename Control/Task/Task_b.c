#include "Task_b.h"

/***********************发挥任务①***********************/
void Task_b_1(void)
{
	u8 key;
	OLED_ShowString(12, 0, (void *)"Task_b_1 Running", 12);
	OLED_Refresh_Gram();
	while (task_flag)
	{
		//		OLED_ShowString(0,0,(void*)"UltraBLI:",12);
		//  	OLED_Show3FNum(50,0,UL_Dis.Behind_Left_Inside,3,1,12);
		//		OLED_ShowString(0,16,(void*)"UltraBLO:",12);
		//  	OLED_Show3FNum(50,16,UL_Dis.Behind_Left_Outside,3,1,12);
		//		OLED_ShowString(0,32,(void*)"UltraFLO:",12);
		//  	OLED_Show3FNum(50,32,UL_Dis.Front_Left_Outside,3,1,12);
		//		OLED_Refresh_Gram();
		OLED_ShowString(0, 20, (void *)"->k1", 12);
		OLED_ShowString(0, 35, (void *)"->k2", 12);
		OLED_ShowString(0, 50, (void *)"->k3", 12);
		OLED_ShowString(35, 20, (void *)"->k4", 12);
		OLED_ShowString(35, 35, (void *)"->k5", 12);
		OLED_ShowString(35, 50, (void *)"->k6", 12);
		OLED_Refresh_Gram();
		key = KEY_Scan(0);
		if (key == KEY1_PRES)
		{
			while (1)
			{
				Go_straight_Y(dir_left, 6);
			}
		}
		if (key == KEY2_PRES)
		{
			while (1)
			{
				Go_Right_X(dir_left, 15);
			}
		}
		if (key == KEY3_PRES)
		{
			while (1)
			{
				Go_Left_X(dir_left, 15);
			}
		}
		if (key == KEY4_PRES)
		{
			while (1)
			{
				Go_back_Y(dir_left, 6);
			}
		}
		if (key == KEY5_PRES)
		{
			OLED_Clear();
			while (1)
			{
				Go_straight();
			}
		}
		//		Go_straight();
	}
}

/***********************发挥任务②***********************/
void Task_b_2(void)
{
	OLED_ShowString(12, 0, (void *)"Task_b_2 Running", 12);
	OLED_Refresh_Gram();
	while (task_flag)
	{
		OLED_ShowString(0, 0, (void *)"UltraBLI:", 12);
		OLED_Show3FNum(50, 0, UL_Dis.Behind_Left_Inside, 3, 1, 12);
		OLED_ShowString(0, 16, (void *)"UltraBLO:", 12);
		OLED_Show3FNum(50, 16, UL_Dis.Behind_Left_Outside, 3, 1, 12);
		OLED_ShowString(0, 32, (void *)"UltraFLI:", 12);
		OLED_Show3FNum(50, 32, UL_Dis.Front_Left_Inside, 3, 1, 12);
		OLED_ShowString(0, 32, (void *)"UltraFLO:", 12);
		OLED_Show3FNum(50, 32, UL_Dis.Front_Left_Outside, 3, 1, 12);
		OLED_Refresh_Gram();
	}
}

/***********************发挥任务③***********************/
void Task_b_3(void)
{
	OLED_ShowString(12, 0, (void *)"Task_b_3 Running", 12);
	OLED_Refresh_Gram();
	while (task_flag)
	{
		OLED_ShowNum(0, 30, UL_Dis.Behind_Right_Outside, 4, 12); // 左1
		OLED_ShowNum(25, 30, UL_Dis.Behind_Right_Inside, 4, 12); // 左2
		OLED_ShowNum(0, 45, UL_Dis.Behind_Left_Outside, 4, 12);	 // 左3
		OLED_ShowNum(25, 45, UL_Dis.Behind_Left_Inside, 4, 12);	 // 左4
		OLED_ShowNum(60, 30, UL_Dis.Front_Right_Outside, 4, 12); // 右1
		OLED_ShowNum(85, 30, UL_Dis.Front_Right_Inside, 4, 12);	 // 右2
		OLED_ShowNum(60, 45, UL_Dis.Front_Left_Outside, 4, 12);	 // 右3
		OLED_ShowNum(85, 45, UL_Dis.Front_Left_Inside, 4, 12);	 // 右4
		OLED_ShowNum(40, 15, UL_Dis.Laser_dis, 4, 12);
		OLED_Refresh_Gram(); // 更新显示到OLED
	}
}

/***********************发挥任务④***********************/
void Task_b_4(void)
{
	motor_cnt_flag = 1;
	OLED_ShowString(12, 0, (void *)"Task_b_4 Running", 12);
	OLED_Refresh_Gram();
	while (task_flag)
	{
		Go_straight_Y(dir_left, 8);
	}
}
