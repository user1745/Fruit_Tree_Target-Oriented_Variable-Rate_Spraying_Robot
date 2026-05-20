#include "sys.h"
#include "Test.h"

/**************测试、调试、输入函数**************/
// LED测试  1
void Test_1(void)
{
	OLED_ShowString(12, 0, (void *)"Test_1 LED", 12);
	OLED_Refresh_Gram();
	while (test_flag)
	{
		LED1 = !LED1;
		LED2 = !LED2;
		LED3 = !LED3;
		delay_ms(100);
	}
	LED1 = 0;
	LED2 = 0;
	LED3 = 0;
}

// 蜂鸣器测试  2
void Test_2(void)
{
	OLED_ShowString(12, 0, (void *)"Test_2 BEEP", 12);
	OLED_Refresh_Gram();
	while (test_flag)
	{
		BEEP = !BEEP;
		delay_ms(1000);
	}
	BEEP = 0;
}

// 按键测试  3
void Test_3(void)
{
	u8 key;
	OLED_ShowString(12, 0, (void *)"Test_3 KEY", 12);
	OLED_Refresh_Gram();

	while (test_flag)
	{
		key = KEY_Scan(0);
		if (key == KEY1_PRES)
			LED1 = !LED1;
		if (key == KEY2_PRES)
			LED2 = !LED2;
		if (key == KEY3_PRES)
			LED3 = !LED3;
		if (key == KEY4_PRES)
		{
			LED1 = !LED1;
			delay_ms(100);
			LED1 = !LED1;
		}
		if (key == KEY5_PRES)
		{
			LED2 = !LED2;
			delay_ms(100);
			LED2 = !LED2;
		}
		if (key == KEY6_PRES)
		{
			LED3 = !LED3;
			delay_ms(100);
			LED3 = !LED3;
		}
	}
}

// OLED测试  4
void Test_4(void)
{
	OLED_ShowString(12, 0, (void *)"Test_4 OLED", 12);
	OLED_Refresh_Gram();

	while (test_flag)
	{
		OLED_ShowString(0, 16, (void *)"OLED OK", 12);
		OLED_ShowIntNum(0, 32, 886688, 4, 12);
		OLED_Refresh_Gram();
	}
}

// MPU6050测试  5
void Test_5(void)
{
	OLED_ShowString(12, 0, (void *)"Test_5 6050", 12);
	OLED_Refresh_Gram();

	while (test_flag)
	{
		//		mpu_dmp_get_data(&mpu_pitch,&mpu_roll,&mpu_yaw);
		delay_ms(10);
		OLED_ShowString(0, 16, (void *)"pitch:", 12);
		OLED_ShowString(0, 32, (void *)"roll:", 12);
		OLED_ShowString(0, 48, (void *)"yaw:", 12);
		OLED_Show3FNum(45, 16, mpu_pitch, 3, 2, 12);
		OLED_Show3FNum(45, 32, mpu_roll, 3, 2, 12);
		OLED_Show3FNum(45, 48, mpu_yaw, 3, 2, 12);
		printf("%f\r\n", mpu_yaw);
		OLED_Refresh_Gram();
	}
}

// NRF24L01测试  6
void Test_6(void)
{
	u8 key;
	u8 tx[24] = {1, 2, 3, 4};
	u8 rx[24] = {0};
	OLED_ShowString(12, 0, (void *)"Test_6 24L01", 12);
	OLED_ShowString(12, 16, (void *)"->K1 TX", 12);
	OLED_ShowString(12, 32, (void *)"->K2 RX", 12);
	OLED_Refresh_Gram();

	while (test_flag)
	{
		key = KEY_Scan(0);
		if (key == KEY1_PRES)
		{
			NRF24L01_TX_Mode();
			OLED_Clear();
			OLED_ShowString(12, 0, (void *)"TX MODE", 12);
			OLED_ShowString(12, 16, (void *)"TXING", 12);
			OLED_Refresh_Gram();
			delay_ms(1000);
			OLED_Clear();
			while (test_flag)
			{
				if (NRF24L01_TxPacket(tx) == TX_OK)
				{
					OLED_ShowString(12, 0, (void *)"TX_OK", 12);
					OLED_Refresh_Gram();
					delay_ms(1000);
					OLED_ShowString(12, 0, (void *)"TXING", 12);
					OLED_Refresh_Gram();
				}
			}
		}

		if (key == KEY2_PRES)
		{
			NRF24L01_RX_Mode();
			OLED_Clear();
			OLED_ShowString(12, 0, (void *)"RX MODE", 12);
			OLED_ShowString(12, 16, (void *)"RXING", 12);
			OLED_Refresh_Gram();
			delay_ms(1000);
			OLED_Clear();
			while (test_flag)
			{
				if (NRF24L01_RxPacket(rx) == 0)
				{
					OLED_ShowString(12, 0, (void *)"RX_OK", 12);
					OLED_ShowNum(0, 12, rx[0], 4, 12);
					OLED_ShowNum(0, 24, rx[1], 4, 12);
					OLED_ShowNum(0, 36, rx[2], 4, 12);
					OLED_ShowNum(0, 48, rx[3], 4, 12);
					OLED_Refresh_Gram();
					delay_ms(1000);
					OLED_ShowString(12, 0, (void *)"RXING", 12);
					OLED_Refresh_Gram();
				}
			}
		}
	}
}

// 超声波测试  7
void Test_7(void)
{
	OLED_ShowString(12, 0, (void *)"Test_7 Sonic", 12);
	OLED_Refresh_Gram();

	while (1)
	{
		Ultrasonic_Get_Distance();
		delay_ms(100);
		LED1 = !LED1;
		OLED_ShowNum(45, 36, UL_Dis.Left, 3, 12);
		OLED_ShowNum(45, 48, UL_Dis.Right, 3, 12);
		OLED_Refresh_Gram();
	}
}

// 电机驱动测试  8
void Test_8(void)
{
	u8 key;
	int SpeedA, SpeedB, SpeedC, SpeedD;
	OLED_ShowString(12, 0, (void *)"Test_8 TB", 12);
	OLED_Refresh_Gram();
	while (test_flag)
	{
		key = KEY_Scan(0);
		if (key == ku)
		{
			SpeedA = 200;
			OLED_Show3FNum(30, 12, SpeedA, 4, 2, 12);
			OLED_Refresh_Gram();
		}
		if (key == kd)
		{
			SpeedB = 300;
			OLED_Show3FNum(30, 24, SpeedB, 4, 2, 12);
			OLED_Refresh_Gram();
		}
		if (key == kl)
		{
			SpeedC = 500;
			OLED_Show3FNum(30, 36, SpeedC, 4, 2, 12);
			OLED_Refresh_Gram();
		}
		if (key == kr)
		{
			SpeedD = 800;
			OLED_Show3FNum(30, 48, SpeedD, 4, 2, 12);
			OLED_Refresh_Gram();
		}
		if (key == kc)
		{
			SpeedA = -150;
			SpeedB = -300;
			SpeedC = -500;
			SpeedD = -800;
		}
		OLED_ShowString(0, 0, (void *)"Speed:", 12);
		Motor_Set(SpeedA, SpeedB, SpeedC, SpeedD);
	}
}

// 编码器测试  9
void Test_9(void)
{
	OLED_ShowString(12, 0, (void *)"Test_9 Encoder", 12);
	OLED_Refresh_Gram();
	Motor_Set(300, 300, 300, 300);
	while (test_flag)
	{
		OLED_ShowString(0, 12, (void *)"speedA:", 12);
		OLED_ShowString(0, 24, (void *)"speedB:", 12);
		OLED_ShowString(0, 36, (void *)"speedC:", 12);
		OLED_ShowString(0, 48, (void *)"speedD:", 12);
		OLED_Show3FNum(55, 12, Encoders.speedA, 5, 2, 12);
		OLED_Show3FNum(55, 24, Encoders.speedB, 5, 2, 12);
		OLED_Show3FNum(55, 36, Encoders.speedC, 5, 2, 12);
		OLED_Show3FNum(55, 48, Encoders.speedD, 5, 2, 12);
		//		OLED_Show3FNum(55,12,Encoders.cntA,5,2,12);
		//		OLED_Show3FNum(55,24,Encoders.cntB,5,2,12);
		//		OLED_Show3FNum(55,36,Encoders.cntC,5,2,12);
		//		OLED_Show3FNum(55,48,Encoders.cntD,5,2,12);
		//		OLED_ShowString(0,12,(void *)"disA:",12);
		//		OLED_ShowString(0,24,(void *)"disB:",12);
		//		OLED_ShowString(0,36,(void *)"disC:",12);
		//		OLED_ShowString(0,48,(void *)"disD:",12);
		//		OLED_Show3FNum(50,12,Encoders.disA,5,2,12);
		//		OLED_Show3FNum(50,24,Encoders.disB,5,2,12);
		//		OLED_Show3FNum(50,36,Encoders.disC,5,2,12);
		//		OLED_Show3FNum(50,48,Encoders.disD,5,2,12);
		OLED_Refresh_Gram();
		delay_ms(20);
	}
}

// 舵机测试  10
void Test_10(void)
{
	u8 key;
	float Angle = 0.0;
	OLED_ShowString(12, 0, (void *)"Test_10 Steer", 12);
	OLED_Refresh_Gram();

	while (test_flag)
	{
		key = KEY_Scan(0);
		if (key == ku)
		{
			Angle += 10;
		}
		if (key == kd)
		{
			Angle -= 10;
		}
		if (key == kb)
		{
			Angle = 0;
		}
		if (key == kc)
			Steer_Angle_Control(Angle, Angle);

		OLED_ShowString(0, 16, (void *)"Angle:", 12);
		OLED_Show3FNum(30, 16, Angle, 4, 2, 12);
		OLED_Refresh_Gram();
	}
}

// 步进测试  11
void Test_11(void)
{
	u8 key;
	float angle = 0.0;
	OLED_ShowString(12, 0, (void *)"Test_11 Step", 12);
	OLED_Refresh_Gram();
	while (test_flag)
	{
		key = KEY_Scan(0);
		if (key == ku)
		{
			angle += 0.3f;
		}
		if (key == kd)
		{
			angle -= 0.3f;
		}
		if (key == kb)
		{
			angle = 0;
		}
		if (key == kc)
		{
			//			Step_Control_1(angle,10);
			//		  Step_Control_2(angle,10);
		}

		OLED_ShowString(0, 16, (void *)"angle:", 12);
		OLED_Show3FNum(30, 16, angle, 4, 2, 12);
		OLED_Refresh_Gram();
	}
}

// 无线串口测试  12
void Test_12(void)
{
	OLED_ShowString(12, 0, (void *)"Test_12 WSN31", 12);
	OLED_Refresh_Gram();

	while (test_flag)
	{
		printf("hello,world!\r\n");
		OLED_ShowString(0, 20, (void *)"hello,world!", 12);
		OLED_Refresh_Gram();
		delay_ms(30);
	}
}

// 测试 13 上位机
void Test_13(void)
{

	OLED_ShowString(0, 16, "receive data is :", 12);
	OLED_Refresh_Gram();

	while (test_flag)
	{
	}
}

// 激光测试 14
void Test_14(void)
{
	OLED_ShowString(12, 0, (void *)"Test_14 Luna", 12);
	OLED_Refresh_Gram();

	while (test_flag)
	{
		OLED_Show3FNum(55, 12, Encoders.speedA, 5, 2, 12);
		OLED_Show3FNum(55, 24, Encoders.speedB, 5, 2, 12);
		OLED_Show3FNum(55, 36, Encoders.speedC, 5, 2, 12);
		OLED_Show3FNum(55, 48, Encoders.speedD, 5, 2, 12);
		OLED_Refresh_Gram();
		printf("MotorA:   %.3f   %.3f   %.2f\r\n\r\n", Encoders.cntA, PID_PWMA.err, PID_PWMA.out);
		printf("MotorB:   %.3f   %.3f   %.2f\r\n\r\n", Encoders.cntB, PID_PWMB.err, PID_PWMB.out);
		printf("MotorC:   %.3f   %.3f   %.2f\r\n\r\n", Encoders.cntC, PID_PWMC.err, PID_PWMC.out);
		printf("MotorD:   %.3f   %.3f   %.2f\r\n\r\n\r\n\r\n", Encoders.cntD, PID_PWMD.err, PID_PWMD.out);
		delay_ms(100);
	}
}

// 速度闭环  15
void Test_15(void)
{
	OLED_ShowString(12, 0, (void *)"Test_15 Motor", 12);
	OLED_Refresh_Gram();
	PID_PWMA.Expected = 10;
	PID_PWMB.Expected = 20;
	PID_PWMC.Expected = 30;
	PID_PWMD.Expected = 40;
	while (1)
	{
		OLED_ShowString(0, 12, (void *)"speedB:", 12);
		OLED_Show3FNum(55, 12, Encoders.speedB, 5, 2, 12);
		OLED_Show3FNum(55, 36, PID_PWMB.err, 5, 2, 12);
		OLED_Show3FNum(55, 48, PID_PWMB.out, 5, 2, 12);
		OLED_Refresh_Gram();
		Motor_Set(PID_PWMA.out, PID_PWMB.out, PID_PWMC.out, PID_PWMD.out);
		//		printf("%.3f\r\n",Encoders.speedA);
		printf("%.3f\r\n", Encoders.speedB);
		//		printf("%.3f\r\n",Encoders.speedC);
		//		printf("%.3f\r\n",Encoders.speedD);
		//		printf("MotorA:   %.3f   %.3f   %d\r\n\r\n",Encoders.speedA,PID_PWMA.err,PID_PWMA.out);
		//		printf("MotorB:   %.3f   %.3f   %d\r\n\r\n",Encoders.speedB,PID_PWMB.err,PID_PWMB.out);
		//		printf("MotorC:   %.3f   %.3f   %d\r\n\r\n",Encoders.speedC,PID_PWMC.err,PID_PWMC.out);
		//		printf("MotorD:   %.3f   %.3f   %d\r\n\r\n\r\n\r\n",Encoders.speedD,PID_PWMD.err,PID_PWMD.out);
		delay_ms(100);
	}
}

// DAC-ADC测试  16
void Test_16(void)
{
	float DAC1 = 0.0;
	float DAC2 = 0.0;
	OLED_ShowString(12, 0, (void *)"Test_16 DAC/ADC", 12);
	OLED_Refresh_Gram();

	while (test_flag)
	{
		Dac1_Set_Vol(1800);
		Dac2_Set_Vol(3200);
		DAC1 = Get_Vol(11);
		DAC2 = Get_Vol(12);
		OLED_ShowString(0, 20, (void *)"DAC1:", 12);
		OLED_Show3FNum(30, 20, DAC1, 2, 2, 12);
		OLED_ShowString(0, 40, (void *)"DAC2:", 12);
		OLED_Show3FNum(30, 40, DAC2, 2, 2, 12);
		OLED_Refresh_Gram(); // 更新显示到OLED
	}
}
