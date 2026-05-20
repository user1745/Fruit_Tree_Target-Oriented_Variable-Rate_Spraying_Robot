#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

#define KEY1 PEin(0)
#define KEY2 PEin(1)
#define KEY3 PEin(2)
#define KEY4 PEin(3)
#define KEY5 PEin(4)
#define KEY6 PEin(5)

#define KEY1_PIN GPIO_Pin_0
#define KEY2_PIN GPIO_Pin_1
#define KEY3_PIN GPIO_Pin_2
#define KEY4_PIN GPIO_Pin_3
#define KEY5_PIN GPIO_Pin_4
#define KEY6_PIN GPIO_Pin_5

#define KEY_PORT GPIOE
#define KEY_RCC RCC_AHB1Periph_GPIOE

#define KEY1_PRES 1
#define KEY2_PRES 2
#define KEY3_PRES 3
#define KEY4_PRES 4
#define KEY5_PRES 5
#define KEY6_PRES 6

#define ku 1
#define kd 5
#define kl 2
#define kr 3
#define kc 6
#define kb 4

void KEY_Init(void);    // IO初始化
u16 KEY_Scan(u16 mode); // 按键扫描函数
void KEY_TEST(void);
void Nei(void);
void Wai(void);

#endif
