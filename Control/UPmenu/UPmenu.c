#include "UPmenu.h"

u8 upmenumode = 0;
u8 level_flag = 0;
u8 receive_data[33];
u8 receive_data_saveA;
u8 receive_data_saveB;
u8 receive_data_saveC;
u8 receive_data_saveD;
u8 num = 0;
u8 state = 0;
void Initial_Window(void);
void dataprocess(u8 data)
{
	if (data == 0xfc && state == 0)
	{
		num = 0;
		state = 1;
	}
	if (state == 1)
	{
		receive_data[num] = data;
		num++;
	}
	if (data == 0xfe && state == 1)
	{
		for (int i = 0; i < num; i++)
		{
			printf("%d ,", receive_data[i]);
		}
		printf("\r\n");
		receive_data_saveA = receive_data[1];
		receive_data_saveB = receive_data[2];
		receive_data_saveC = receive_data[3];
		receive_data_saveD = receive_data[4];
		state = 0;
		num = 0;
		if (receive_data[1] == 0x13 && receive_data[3] == 0x14)
			Upmenumode_get(receive_data[2]);

		for (int i = 0; i < 6; i++)
		{
			receive_data[i] = 0x00;
		}
	}
	if (num >= 32)
	{
		state = 0;
		num = 0;
		printf("overload/r/f");
	}
}

void Upmenumode_get(u8 choice)
{
	printf("get ask%d\r\n", choice);
	switch (choice)
	{
	case 1:
		upmenumode = led_test_mode;
		break;
	case 2:
		upmenumode = beep_test_mode;
		;
		break;
	case 3:
		upmenumode = sonic_test_mode;
		break;
	case 4:
		upmenumode = mpu_test_mode;
		break;
	case 5:
		upmenumode = motor_test_mode;
		break;
	case 6:
		upmenumode = encoder_test_mode;
		break;
	case 9:
		upmenumode = 0;
		break;
	default:
		break;
	}
}

void Upmenu(void)
{
	cover_oledmean();
	while (1)
	{
		switch (upmenumode)
		{
		case 0:
			Initial_Window();
			break;
		case led_test_mode:
			LED_TEST();
			break;
		case beep_test_mode:
			BEEP_TEST();
			break;
		case sonic_test_mode:
			Ultrasonic_TEST();
			break;
		case mpu_test_mode:
			MPU9250_TEST();
			break;
		case motor_test_mode:
			Motor_4950_TEST();
			break;
		case encoder_test_mode:
			Encoder_TEST();
			break;
		default:
			break;
		}
	}
}

void Initial_Window(void)
{
	OLED_Clear();
	OLED_ShowString(16, 4, "Please wait command", 24);
	OLED_Refresh_Gram();
	while (!upmenumode)
		;
}
