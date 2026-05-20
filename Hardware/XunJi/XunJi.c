#include "XunJi.h"
#include "GWKJ.h"
#include "gw_grayscale_sensor.h"

int Get_Err(uint8_t IR)
{
	char L, R, i;

	L = 0;
	for (i = 0; i < 7; i++)
	{
		if ((((~IR - 40) & 1 << (i + 1)) >> (i + 1)) - (((~IR - 40) & 1 << i) >> i) == 1) // 左向右找上升沿
		{
			L = i;
			break;
		}
	}
	R = 0;
	for (i = 0; i < 7; i++)
	{
		if ((((~IR - 40) & 0x80 >> (i + 1)) << (i + 1)) - (((~IR - 40) & 0x80 >> (i)) << (i)) == 0x80) // 左向右找上升沿
		{
			R = i;
			break;
		}
	}

	return (int)(L + 7 - R - 7);
}
