#include "OpenMV.h"

u8 openmv_control_flag = 0;

// openmv_Wide接收协议
void Openmv_Wide_RX(int16_t data) // 接收Openmv_Wide传过来的数据
{
	u8 i;
	static u8 state = 0;			// 标志
	static u8 bit_number = 0;		// 数据指针
	if (state == 0 && data == 0x2C) // 帧头校验
	{
		openmv_Wide[bit_number++] = data;
		if (bit_number == 1)
			state = 1;
		else
		{
			state = 0;
			bit_number = 0;
		}
	}

	else if (state == 1)
	{
		openmv_Wide[bit_number++] = data;
		if (bit_number == 2)
			state = 2;
		else
		{
			state = 0;
			bit_number = 0;
		}
	}

	else if (state == 2)
	{
		openmv_Wide[bit_number++] = data;
		if (bit_number == 3)
			state = 3;
		else
		{
			state = 0;
			bit_number = 0;
		}
	}

	else if (state == 3)
	{
		openmv_Wide[bit_number++] = data;
		if (bit_number == 4)
			state = 4;
		else
		{
			state = 0;
			bit_number = 0;
		}
	}

	else if (state == 4) // 检测是否接受到结束标志
	{
		if (data == 0x5B) // 帧尾校验
		{
			openmv_control_flag = 1;
			state = 0;
			openmv_Wide[bit_number++] = data;
			red_x_Wide = openmv_Wide[1] * 256 + openmv_Wide[2]; // 用左移
			red_y_Wide = openmv_Wide[3];
		}
		else
		{
			state = 0;
			bit_number = 0;
			for (i = 0; i < 6; i++)
			{
				openmv_Wide[i] = 0x00;
			}
		}
	}

	else
	{
		state = 0;
		bit_number = 0;
	}
}

// openmv_Narrow接收协议
void Openmv_Narrow_RX(int16_t data) // 接收Openmv_Narrow传过来的数据
{
	u8 i;
	static u8 state = 0;			// 标志
	static u8 bit_number = 0;		// 数据指针
	if (state == 0 && data == 0x2C) // 帧头校验
	{
		openmv_Narrow[bit_number++] = data;
		if (bit_number == 1)
			state = 1;
		else
			state = 0;
	}

	else if (state == 1)
	{
		openmv_Narrow[bit_number++] = data;
		if (bit_number == 2)
			state = 2;
		else
			state = 0;
	}

	else if (state == 2)
	{
		openmv_Narrow[bit_number++] = data;
		if (bit_number == 3)
			state = 3;
		else
			state = 0;
	}

	else if (state == 3)
	{
		openmv_Narrow[bit_number++] = data;
		if (bit_number == 4)
			state = 4;
		else
			state = 0;
	}

	else if (state == 4) // 检测是否接受到结束标志
	{
		if (data == 0x5B) // 帧尾校验
		{
			openmv_control_flag = 1;
			state = 0;
			bit_number = 0;
			openmv_Narrow[bit_number++] = data;
			red_y_Narrow = openmv_Narrow[1] * 256 + openmv_Narrow[2]; // 用左移
			red_x_Narrow = openmv_Narrow[3];
		}
		else
		{
			state = 0;
			bit_number = 0;
			for (i = 0; i < 6; i++)
			{
				openmv_Narrow[i] = 0x00;
			}
		}
	}

	else
	{
		state = 0;
		bit_number = 0;
	}
}
