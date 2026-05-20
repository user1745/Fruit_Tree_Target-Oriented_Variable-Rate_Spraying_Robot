#include "Task_c.h"

/***********************拓展任务①***********************/
void Task_c_1(void)
{
	OLED_ShowString(12, 0, (void *)"Task_c_1 Running", 12);
	OLED_Refresh_Gram();
	PID_PWMA.Expected = 50;
	PID_PWMB.Expected = 50;
	PID_PWMC.Expected = 50;
	PID_PWMD.Expected = 50;
	while (task_flag)
	{
		//		OLED_ShowString(0,12,(void *)"speedA:",12);
		//		OLED_ShowString(0,24,(void *)"speedB:",12);
		//		OLED_Show3FNum(55,12,Encoders.speedA,5,2,12);
		//		OLED_Show3FNum(55,24,Encoders.speedB,5,2,12);

		OLED_ShowString(0, 12, (void *)"speedC:", 12);
		OLED_ShowString(0, 24, (void *)"speedD:", 12);
		OLED_Show3FNum(55, 12, Encoders.speedC, 5, 2, 12);
		OLED_Show3FNum(55, 24, Encoders.speedD, 5, 2, 12);

		//		OLED_ShowString(0,36,(void *)"Aout:",12);
		//		OLED_ShowString(0,48,(void *)"Bout:",12);
		//		OLED_Show3FNum(55,36,PID_PWMA.out,3,2,12);
		//		OLED_Show3FNum(55,48,PID_PWMB.out,3,2,12);

		OLED_ShowString(0, 36, (void *)"Cout:", 12);
		OLED_ShowString(0, 48, (void *)"Dout:", 12);
		OLED_Show3FNum(55, 36, PID_PWMC.out, 3, 2, 12);
		OLED_Show3FNum(55, 48, PID_PWMD.out, 3, 2, 12);
		OLED_Refresh_Gram();
		MotorAB_Set(PID_PWMA.out, PID_PWMB.out);
		MotorCD_Set(PID_PWMC.out, PID_PWMD.out);
	}
}

/***********************拓展任务②***********************/
void Task_c_2(void) // 前行 角度环+速度环
{
	OLED_ShowString(12, 0, (void *)"Task_c_2 Running", 12);
	OLED_Refresh_Gram();
	PID_2.Expected = 6;
	PID_3.Expected = 6;
	PID_4.Expected = 10;
	while (task_flag)
	{
		Go_Left();
		//		Go_straight_Y(dir_right,6);
		//		printf("BLO:      %d       %.3f     %.2f\r\n\r\n",UL_Dis.Behind_Right_Outside,PID_2.err,PID_2.out);
		//		printf("FLO:      %d       %.3f     %.2f\r\n\r\n\r\n",UL_Dis.Front_Right_Outside,PID_3.err,PID_3.out);

		//		D_value_X=(PID_2.out+PID_3.out)/2;
		//		Ultra_LI_flag=1;
		//		Motor_Analysis(30,PID_4.out,PID_1.out);

		//		D_value_X=(PID_2.out+PID_3.out)/2;
		//		Ultra_LO_flag=1;
		//		Motor_Analysis(D_value_X,30,PID_1.out);

		//		D_value_X=10*(PID_2.out+PID_3.out)/2;
		//		Motor_Analysis(D_value_X,800,10*PID_1.out);

		//		printf("D_value_X:         %.3f\r\n\r\n",D_value_X);
		//		printf("MPU6050:  %.3f     %.3f     %.2f\r\n\r\n",mpu_yaw,PID_1.err,PID_1.out);
		//		printf("BLO:      %d       %.3f     %.2f\r\n\r\n",UL_Dis.Behind_Left_Outside,PID_2.err,PID_2.out);
		//		printf("FLO:      %d       %.3f     %.2f\r\n\r\n\r\n",UL_Dis.Front_Left_Outside,PID_3.err,PID_3.out);
		//		printf("MotorA:   %.3f   %.3f   %.2f\r\n\r\n",Encoders.speedA,PID_PWMA.err,PID_PWMA.out);
		//		printf("MotorB:   %.3f   %.3f   %.2f\r\n\r\n",Encoders.speedB,PID_PWMB.err,PID_PWMB.out);
		//		printf("MotorC:   %.3f   %.3f   %.2f\r\n\r\n",Encoders.speedC,PID_PWMC.err,PID_PWMC.out);
		//		printf("MotorD:   %.3f   %.3f   %.2f\r\n\r\n\r\n\r\n",Encoders.speedD,PID_PWMD.err,PID_PWMD.out);
		delay_ms(80);
	}
}

/***********************拓展任务③***********************/
void Task_c_3(void) // 横行
{
	OLED_ShowString(12, 0, (void *)"Task_c_3 Running", 12);
	OLED_Refresh_Gram();
	PID_PWMA.Expected = -50;
	PID_PWMB.Expected = -50;
	PID_PWMC.Expected = 50;
	PID_PWMD.Expected = 50;
	while (task_flag)
	{
		MotorAB_Set(PID_PWMA.out, PID_PWMB.out);
		MotorCD_Set(PID_PWMC.out, PID_PWMD.out);
		printf("MotorA:   %.3f   %.3f   %.2f\r\n\r\n", Encoders.speedA, PID_PWMA.err, PID_PWMA.out);
		printf("MotorB:   %.3f   %.3f   %.2f\r\n\r\n", Encoders.speedB, PID_PWMB.err, PID_PWMB.out);
		printf("MotorC:   %.3f   %.3f   %.2f\r\n\r\n", Encoders.speedC, PID_PWMC.err, PID_PWMC.out);
		printf("MotorD:   %.3f   %.3f   %.2f\r\n\r\n\r\n\r\n", Encoders.speedD, PID_PWMD.err, PID_PWMD.out);
		delay_ms(100);
	}
}

/***********************拓展任务④***********************/
void Task_c_4(void) // 顺时针旋转
{
	OLED_ShowString(12, 0, (void *)"Task_c_4 Running", 12);
	OLED_Refresh_Gram();
	while (task_flag)
	{
		PID_PWMA.Expected = -50;
		PID_PWMB.Expected = -50;
		PID_PWMC.Expected = 50;
		PID_PWMD.Expected = 50;
		MotorAB_Set(PID_PWMA.out, PID_PWMB.out);
		MotorCD_Set(PID_PWMC.out, PID_PWMD.out);
		printf("MotorA:   %.3f   %.3f   %.2f\r\n\r\n", Encoders.speedA, PID_PWMA.err, PID_PWMA.out);
		printf("MotorB:   %.3f   %.3f   %.2f\r\n\r\n", Encoders.speedB, PID_PWMB.err, PID_PWMB.out);
		printf("MotorC:   %.3f   %.3f   %.2f\r\n\r\n", Encoders.speedC, PID_PWMC.err, PID_PWMC.out);
		printf("MotorD:   %.3f   %.3f   %.2f\r\n\r\n\r\n\r\n", Encoders.speedD, PID_PWMD.err, PID_PWMD.out);
		delay_ms(100);
	}
}
