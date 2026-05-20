#ifndef _MOTOR_H
#define _MOTOR_H
#include "sys.h"

#define AIN3 PEout(6)
#define AIN4 PEout(7)
#define BIN3 PAout(0)
#define BIN4 PCout(3)

#define AIN2 PAout(10)
#define AIN1 PDout(7)
#define BIN1 PDout(10)
#define BIN2 PDout(11)

#define AIN3AIN4_RCC RCC_AHB1Periph_GPIOE
#define AIN2BIN3_RCC RCC_AHB1Periph_GPIOA
#define BIN4_RCC RCC_AHB1Periph_GPIOC
#define AIN1BIN1BIN2_RCC RCC_AHB1Periph_GPIOD

#define AIN2_PIN GPIO_Pin_10
#define AIN1_PIN GPIO_Pin_7

#define BIN1_PIN GPIO_Pin_10
#define BIN2_PIN GPIO_Pin_11
#define AIN1BIN1BIN2_PORT GPIOD
#define AIN2BIN3_PORT GPIOA
#define BIN4_PORT GPIOC
#define AIN3AIN4_PORT GPIOE
// #define 	AB_RCC			RCC_AHB1Periph_GPIOA

// #define 	AIN2_PIN   		GPIO_Pin_2
// #define 	AIN1_PIN   		GPIO_Pin_3

// #define 	BIN1_PIN  		GPIO_Pin_12
// #define 	BIN2_PIN   		GPIO_Pin_15

// #define 	AB_PORT   		GPIOA

// #define   C_RCC       RCC_AHB1Periph_GPIOB
// #define   D_RCC       RCC_AHB1Periph_GPIOD

#define AIN3_PIN GPIO_Pin_6
#define AIN4_PIN GPIO_Pin_7

#define BIN3_PIN GPIO_Pin_0
#define BIN4_PIN GPIO_Pin_3

// #define   C_PORT       GPIOB
// #define   D_PORT       GPIOD

#define Long 4.7
#define Wide 8

extern int speed_A;
extern int speed_B;
extern int speed_C;
extern int speed_D;

extern int Motor_limit;

void Motor_Init(void);

void Motor_IO_Init(void);               // IO口初始化
void TIM1_Motor_Init(u16 arr, u16 psc); // TIM1初始化
void TIM4_Motor_Init(u16 arr, u16 psc); // TIM4初始化
void TIM8_Motor_Init(u16 arr, u16 psc);

int myabs(int x); // 绝对值函数

void Motor_Set(int speedA, int speedB, int speedC, int speedD); // 电机的驱动函数
void MotorAB_Set(int speedA, int speedB);                       // AB轮驱动函数
void MotorCD_Set(int speedC, int speedD);                       // CD轮驱动函数

void Motor_Set_7843(int speedC, int speedD);
void Motor_Analysis(float Vx1, float Vx2, float Vy1, float Vy2, float Wz);
void Motor_TEST(void);
void Motor_4950_Init(void);
void Motor_4950_TEST(void);
#endif
