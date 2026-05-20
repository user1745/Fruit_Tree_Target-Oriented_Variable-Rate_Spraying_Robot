#ifndef _STEP_H
#define _STEP_H
#include "sys.h"

#define SENA PEout(9)
#define SDIRA PEout(8)
#define SDIRB PAout(12)
#define SENB PAout(15)

#define SENC PBout(10)
#define SDIRC PBout(11)
#define SDIRD PDout(10)
#define SEND PDout(11)

#define SA_RCC RCC_AHB1Periph_GPIOE
#define SB_RCC RCC_AHB1Periph_GPIOA
#define SENA_PIN GPIO_Pin_9
#define SDIRA_PIN GPIO_Pin_8

#define SENB_PIN GPIO_Pin_12
#define SDIRB_PIN GPIO_Pin_15
#define SA_PORT GPIOE
#define SB_PORT GPIOA

#define SC_RCC RCC_AHB1Periph_GPIOB
#define SD_RCC RCC_AHB1Periph_GPIOD

#define SENC_PIN GPIO_Pin_10
#define SDIRC_PIN GPIO_Pin_11

#define SDIRD_PIN GPIO_Pin_10
#define SEND_PIN GPIO_Pin_11

#define SC_PORT GPIOB
#define SD_PORT GPIOD

#define STEPA 0
#define STEPB 1
#define STEPC 2
#define STEPD 3
#define DIVIDE 8 // 细分数
void Step_TEST(void);
void Step_init(void);
void Step_control(u8 index, u8 dir, int angle);
#endif
