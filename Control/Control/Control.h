#ifndef __CONTROL_H
#define __CONTROL_H

#include "sys.h"

// 闭环直行
void Go_straight_Y(int dire, int distance);
void Go_back_Y(int dire, int distance);
// 闭环横行
void Go_Right_X(int dire, int distance);
void Go_Left_X(int dire, int distance);

// 半闭环直行
void Go_straight_y(int dire, int distance, int mood);
void Go_back_y(int dire, int distance, int mood);

// 开环直行
void Go_straight(void);
void Go_back(void);
// 开环横行
void Go_Right(void);
void Go_Left(void);

// 减速
void Slow_down(u8 speed);
// 停止
void Stop(void);
// 启动
void Start(void);

// 前往目标靶处
void goto_target(int laser_x, int dire);

#endif
