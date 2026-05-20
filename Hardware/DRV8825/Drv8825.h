#ifndef _DRV8825_H
#define _DRV8825_H

#include "sys.h"

// DRV8825 端口定义

#define PULSE_RCC RCC_AHB1Periph_GPIOA
#define DIR_RCC RCC_AHB1Periph_GPIOE

#define PULSE1_PIN GPIO_Pin_2
#define PULSE2_PIN GPIO_Pin_3
#define DIR1_PIN GPIO_Pin_6
#define DIR2_PIN GPIO_Pin_7

#define PULSE_PORT GPIOA
#define DIR_PORT GPIOE

#define STEP_DIR1 PEout(6)
#define STEP_DIR2 PEout(7)

void DRV8825_Init(void);
void DRV8825_IO_Init(void);
void TIM9_DRV8825_PWM_Init(u16 arr, u16 psc); // PWM初始化
void DRV8825_Driver(u8 index, u8 dir);        // 方圆电机驱动
void DRV8825_Stop(void);                      // 方圆电机驱动全关

#endif
