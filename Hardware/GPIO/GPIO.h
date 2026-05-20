#ifndef __GPIO_LB_H
#define __GPIO_LB_H

#include "sys.h"

// LED 和 BEEP 端口定义
#define LED1 PCout(13)
#define LED2 PCout(4)
#define LED3 PCout(5)
#define BEEP PBout(8)

#define LED1_PIN GPIO_Pin_13
#define LED2_PIN GPIO_Pin_4
#define LED3_PIN GPIO_Pin_5
#define BEEP_PIN GPIO_Pin_8

#define LED_PORT GPIOC
#define BEEP_PORT GPIOB

#define LED_RCC RCC_AHB1Periph_GPIOC
#define BEEP_RCC RCC_AHB1Periph_GPIOB

void GPIO_LB_Init(void); // 初始化
void LED_TEST(void);
void BEEP_TEST(void);

#endif
