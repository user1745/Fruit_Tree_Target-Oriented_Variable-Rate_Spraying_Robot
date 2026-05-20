#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

// steer//
extern int steer1_angle;
extern int steer2_angle;

// motor//
extern u8 motor_delay_flag;
extern u8 motor_cnt_flag;

// openmv_Wide//
extern u8 openmv_Wide_flag; // 1说明此次接收有效，0说明此次接收无效
extern u16 red_x_Wide;
extern u16 red_y_Wide;
extern u8 openmv_Wide[6];

extern u8 debuga;
extern u8 debugb;
extern u8 debugc;
extern u8 debugd;
extern u8 debuge;
extern u8 debugf;

// openmv_Narrow//
extern u8 openmv_Narrow_flag; // 1说明此次接收有效，0说明此次接收无效
extern u16 red_x_Narrow;
extern u16 red_y_Narrow;
extern u8 openmv_Narrow[6];

extern u8 debugg;
extern u8 debugh;
extern u8 debugi;
extern u8 debugj;
extern u8 debugk;
extern u8 debugm;

// 6050//
extern u8 Mpu_Y_flag;
extern u8 Mpu_X_flag;

// Ultra//
extern u8 Ultra_LO_flag;
extern u8 Ultra_RO_flag;
extern u8 Ultra_LI_flag;

void TIMER_Interrupt_Init(void);
void TIM6_Int_Init(u16 arr, u16 psc);
void TIM7_Int_Init(u16 arr, u16 psc);

#endif
