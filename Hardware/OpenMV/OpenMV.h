#ifndef __OPENMV_H
#define __OPENMV_H
#include "sys.h"

void Openmv_Wide_RX(int16_t data);
void Openmv_Narrow_RX(int16_t data);
void openmv_control(void);

extern u8 openmv_control_flag;

extern u8 Encoder_flag;
#endif
