#ifndef __PARAMETER_H
#define __PARAMETER_H

#include "sys.h"

// 作业车行进方向
#define dir_right 0   // 右
#define dir_left 1    // 左
#define dir_forward 2 // 前
#define dir_back 3    // 后

// 作业车半闭环模式
#define mood_right 0   // 右
#define mood_left 1    // 左
#define mood_forward 2 // 前
#define mood_back 3    // 后

#define task_flag menu_level >= 3

#define test_flag menu_level >= 4
extern u8 menu_level;

/**********接收的6050数据**********/
extern float mpu_correct;

extern s16 Rece_pitch;
extern s16 Rece_roll;
extern s16 Rece_yaw;
extern float mpu_pitch;
extern float mpu_roll;
extern float mpu_yaw;

extern int pos_x;
extern int pos_y;

/**************PID控制相关变量***************/
extern int D_value_6050;
extern int D_value_AB;
extern int D_value_CD;
extern int D_valueAB;
extern int D_valueCD;
extern int base_AB;
extern int base_CD;

/**************麦克纳姆轮相关变量***************/
extern float D_value_X;
extern float D_value_Y;
extern float D_value_Z;
extern int base_speed_x;
extern int base_speed_y;

extern int base_speed;
extern int Motor_base_speed;
extern int turn_speed;

#endif
