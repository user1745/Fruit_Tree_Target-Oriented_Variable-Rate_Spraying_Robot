#include "KEY.h"
#include "Test.h"
#include "XunJi.h"
#include "Usart.h"

/*
********************************************************************************************************************
*                  void KEY_Init(void)
*
*Description : 按键初始化函数
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void KEY_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(KEY_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = KEY1_PIN | KEY2_PIN | KEY3_PIN | KEY4_PIN | KEY5_PIN | KEY6_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);
}

/*
********************************************************************************************************************
*                  u16 KEY_Scan(u16 mode)
*
*Description : 按键处理函数
*Arguments   : mode:0,不支持连续按;1,支持连续按;
*Returns     : 返回按键值
*Notes       : 注意此函数有响应优先级,KEY1>KEY2>KEY3>KEY4>KEY5>KEY6
********************************************************************************************************************
*/
u16 KEY_Scan(u16 mode)
{
	static u16 key_up = 1; // 按键按松开标志
	if (mode)
		key_up = 1; // 支持连按
	if (key_up && (KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0 || KEY5 == 0 || KEY6 == 0))
	{
		delay_ms(15); // 去抖动
		key_up = 0;
		if (KEY1 == 0)
			return KEY1_PRES;
		else if (KEY2 == 0)
			return KEY2_PRES;
		else if (KEY3 == 0)
			return KEY3_PRES;
		else if (KEY4 == 0)
			return KEY4_PRES;
		else if (KEY5 == 0)
			return KEY5_PRES;
		else if (KEY6 == 0)
			return KEY6_PRES;
	}
	else if (KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1 && KEY5 == 1 && KEY6 == 1)
		key_up = 1;
	return 0; // 无按键按下
}

void Nei(void)
{
#define kcp 25
#define kcd 4
	//	#define Carspeed 150
	short ax, ay, az;
	volatile uint8_t digital;
	volatile uint8_t digit1;
	volatile uint8_t digit2;
	volatile uint8_t digit3;
	volatile uint8_t digit4;
	static int carspeed = 120;
	digital = GW_digital_Read();
	digit1 = GW_digital_Read_Bit(1);
	digit2 = GW_digital_Read_Bit(2);
	digit3 = GW_digital_Read_Bit(3);
	if (digit1 == 0 || digit2 == 0)
	{
		Motor_Set(0, 200, 0, 0);
		delay_ms(150);
	}
	else
	{
		int Car_err;
		int Car_err_old;
		int L_PWM;
		int R_PWM;
		MPU_Get_Gyroscope(&ax, &ay, &az);
		Car_err = Get_Err(~digital - 40);
		L_PWM = +kcp * Car_err + carspeed + (az / 62.5) * kcd;
		R_PWM = -kcp * Car_err + carspeed - (az / 62.5) * kcd;
		Car_err = (~digital - 40);
		if (Car_err == 0x00)
		{
			if (Car_err_old == 0x80)
			{
				L_PWM = carspeed + 75;
				R_PWM = carspeed - 75;
				Motor_Set(L_PWM, R_PWM, 0, 0);
				while (digital != 0x20)
					;
			}
			if (Car_err_old == 0x01)
			{
				L_PWM = carspeed - 75;
				R_PWM = carspeed + 75;
				Motor_Set(L_PWM, R_PWM, 0, 0);
				while (digital != 0x04)
					;
			}
		}
		Car_err_old = Car_err;
		Motor_Set(L_PWM, R_PWM, 0, 0);
		OLED_ShowNum(0, 0, ~digital - 40, 3, 12);
		OLED_Show3FNum(20, 0, az / 62.5, 3, 3, 12);
		OLED_ShowIntNum(0, 40, Get_Err(~digital - 40), 3, 12);
		OLED_Refresh_Gram();
	}
}

void Wai(void)
{
#define kcp 25
#define kcd 4
	//	#define Carspeed 150
	short ax, ay, az;
	volatile uint8_t digital;
	static int carspeed = 120;
	volatile uint8_t digit8;
	digit8 = GW_digital_Read_Bit(8);
	if (digit8 == 0)
	{
		Motor_Set(150, 100, 0, 0);
	}
	else
	{
		int Car_err;
		int Car_err_old;
		int L_PWM;
		int R_PWM;
		MPU_Get_Gyroscope(&ax, &ay, &az);
		digital = GW_digital_Read();
		Car_err = Get_Err(~digital - 40);
		L_PWM = +kcp * Car_err + carspeed + (az / 62.5) * kcd;
		R_PWM = -kcp * Car_err + carspeed - (az / 62.5) * kcd;
		Car_err = (~digital - 40);
		if (Car_err == 0x00)
		{
			if (Car_err_old == 0x80)
			{
				L_PWM = carspeed + 75;
				R_PWM = carspeed - 75;
				Motor_Set(L_PWM, R_PWM, 0, 0);
				while (digital != 0x20)
					;
			}
			if (Car_err_old == 0x01)
			{
				L_PWM = carspeed - 75;
				R_PWM = carspeed + 75;
				Motor_Set(L_PWM, R_PWM, 0, 0);
				while (digital != 0x04)
					;
			}
		}
		Car_err_old = Car_err;
		Motor_Set(L_PWM, R_PWM, 0, 0);
		OLED_ShowNum(0, 0, ~digital - 40, 3, 12);
		OLED_Show3FNum(20, 0, az / 62.5, 3, 3, 12);
		OLED_ShowIntNum(0, 40, Get_Err(~digital - 40), 3, 12);
		OLED_Refresh_Gram();
	}
}

void KEY_TEST(void)
{
	u8 key;

	int GX_n = 0; // 过线计数变量=0
	volatile uint8_t digital;
	volatile uint8_t digit1;
	volatile uint8_t digit2;
	volatile uint8_t digit3;
	volatile uint8_t digit4;
	//	static int carspeed=120;
	static u8 CAR_PWM_EN = 0; // PWM使能 =0不跑  =1跑
	//	static u8 i=0;
	Motor_Init();
	GW_digital_Init();
	MPU9250_Init();
	OLED_ShowString(0, 12, (void *)"Test_3 KEY", 12);
	//	OLED_Show3FNum(60,12,carspeed,4,2,12);
	OLED_Refresh_Gram();
	while (test_flag)
	{
		key = KEY_Scan(0);
		digital = GW_digital_Read();
		if (key == KEY1_PRES)
		{
			CAR_PWM_EN = !CAR_PWM_EN;
		}
		if (CAR_PWM_EN == 0)
		{
			Motor_Set(0, 0, 0, 0);
		}
		else
		{
			if (GX_n == 0)
			{
				Nei();
			}
			else
			{
				Wai();
			}
		}
		OLED_ShowNum(0, 22, GX_n, 1, 12);
		OLED_Refresh_Gram();
		if (digital == 0x00) // 如果压线
		{
			if (GX_n == 0)
			{
				GX_n = 1;
				CAR_PWM_EN = 1;
				OLED_ShowNum(0, 22, GX_n, 1, 12);
				OLED_Refresh_Gram();
				delay_ms(1000);
				continue;
			}
			if (GX_n == 1)
			{
				GX_n = 2;
				CAR_PWM_EN = 0;
				OLED_ShowNum(0, 22, GX_n, 1, 12);
				OLED_Refresh_Gram();
			}
		}

		//			if(digital == 0x00 && GX_n == 0)  //如果压线
		//			{
		//				while(digital == 0x00);  //等待过线
		//				GX_n = GX_n+1;  //过线变量使能
		//			}
		//			OLED_ShowNum(0,22,GX_n,1,12);
		//			if(digital == 0x00 && GX_n != 0)  //如果第二次压线
		//			{
		//				CAR_PWM_EN = 0;
		//			}
		//			if(GX_n != 0 && GX_n < 50)
		//			{
		//				GX_n++;  //过线变量自加
		//			}
		//			else if(GX_n == 50)
		//			{
		//				GX_n =0;
		//			}
		if (key == KEY2_PRES)
		{
			LED3 = !LED3;
		}
		if (key == KEY3_PRES)
		{
			LED3 = !LED3;
			delay_ms(100);
			LED3 = !LED3;
		}
		//		if(key==KEY4_PRES)
		//
		//		{
		//			LED1=!LED1;
		//			delay_ms(100);
		//			LED1=!LED1;
		//		}
		//		if(key==KEY5_PRES)
		//		{
		//			LED2=!LED2;
		//			delay_ms(100);
		//			LED2=!LED2;
		//		}
		//		if(key==KEY6_PRES)
		//		{
		//			LED3=!LED3;
		//			delay_ms(100);
		//			LED3=!LED3;
	}
}
