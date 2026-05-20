#include "GPIO.h"

/*
********************************************************************************************************************
*                  void GPIO_LB_Init(void)
*
*Description : LED和BEEP的IO口初始化
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void GPIO_LB_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(LED_RCC | BEEP_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(LED_PORT, LED1_PIN | LED2_PIN | LED3_PIN);
	GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
}

void LED_TEST(void)
{
	u16 light_time = 1000;
	OLED_Clear();
	OLED_ShowString(12, 0, (void *)"Test_1 LED", 12);
	OLED_Refresh_Gram();
	while (test_flag && upmenumode == led_test_mode)
	{

		OLED_ShowNum(16, 32, receive_data_saveA, 2, 12);
		OLED_Refresh_Gram();
		light_time = receive_data_saveA * 100;
		LED1 = !LED1;
		LED2 = !LED2;
		LED3 = !LED3;
		delay_ms(200);
		delay_ms(light_time);
	}
	LED1 = 0;
	LED2 = 0;
	LED3 = 0;
}

void BEEP_TEST(void)
{
	OLED_Clear();
	OLED_ShowString(12, 0, (void *)"Test_2 BEEP", 12);
	OLED_Refresh_Gram();
	while (test_flag && upmenumode == beep_test_mode)
	{
		BEEP = !BEEP;
		delay_ms(receive_data_saveA * 100);
	}
	BEEP = 0;
}
