#ifndef _MOTOR_4950_H
#define _MOTOR_4950_H

#include "sys.h"

#define DIRA PEout(8)

#define A_RCC RCC_AHB1Periph_GPIOE
#define AIN_PIN GPIO_Pin_8
#define A_PORT GPIOE

void Motor_4950_IO_Init(void);
void TIM1_Motor_4950_Init(u16 arr, u16 psc);
void Motor_4950_Set(int speedA);
void Motor_4950_TEST(void);

#endif
