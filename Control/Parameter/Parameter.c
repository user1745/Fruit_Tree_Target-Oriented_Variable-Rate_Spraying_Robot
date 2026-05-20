#include "Parameter.h"

u8 menu_level = 1;

/**********接收的6050数据**********/
float mpu_correct = 0.0;

s16 Rece_pitch;
s16 Rece_roll;
s16 Rece_yaw;
float mpu_pitch;
float mpu_roll;
float mpu_yaw;

int pos_x = 0;
int pos_y = 0;

/**************PID控制相关变量***************/
int D_value_6050 = 0;
int D_value_AB = 0;
int D_value_CD = 0;
int D_valueAB = 0;
int D_valueCD = 0;
int base_AB = 60;
int base_CD = 60;

////麦轮相关
float D_value_X = 0;
float D_value_Y = 0;
float D_value_Z = 0;

// 作业
int base_speed_x = 50;
int base_speed_y = 75;

// 巡垄
// int base_speed_x=55;
// int base_speed_y=40;

int base_speed = 50;

int Motor_base_speed = 0;
int turn_speed = 0;
