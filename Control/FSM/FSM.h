#ifndef __FSM_H
#define __FSM_H

#include "sys.h"

/* ============================================================
 * 状态枚举结构体
 * ============================================================ */
typedef enum
{
    STATE_IDLE = 0,        // 空闲，等待 KEY 启动
    STATE_SHORT_FORWARD,   // 短进直垄道：入垄加速段
    STATE_LONG_FORWARD,    // 长进直垄道：超声波左侧循迹
    STATE_BRAKE_FORWARD,   // 直进垄道刹车：前方超声波减速
    STATE_SHIFT_RIGHT_1,   // 右进垄道1：右横移入垄
    STATE_BRAKE_RIGHT_1,   // 右进垄道1刹车：右侧超声波减速
    STATE_BACKWARD_1,      // 后退1：编码器计数后退
    STATE_SHIFT_LEFT_2,    // 左进垄道2：左横移入垄
    STATE_FORWARD_LEFT_2,  // 左进垄道2前进：前方超声波循迹
    STATE_BRAKE_LEFT_2,    // 左进垄道2刹车：左侧超声波减速
    STATE_BACKWARD_2,      // 后退2：编码器计数后退
    STATE_SHIFT_RIGHT_3,   // 右进垄道3：右横移入垄
    STATE_FORWARD_RIGHT_3, // 右进垄道3前进：前方超声波循迹
    STATE_BRAKE_RIGHT_3,   // 右进垄道3刹车：右侧超声波减速
    STATE_BACKWARD_3,      // 后退3：编码器计数后退
    STATE_SHIFT_LEFT_4,    // 左进垄道4：左横移入垄
    STATE_FORWARD_LEFT_4,  // 左进垄道4前进：前方超声波循迹
    STATE_BRAKE_LEFT_4,    // 左进垄道4刹车：左侧超声波减速
    STATE_BACKWARD_4,      // 后退4：编码器计数后退
    STATE_SHIFT_RIGHT_5,   // 右进垄道5：右横移入垄
    STATE_FORWARD_RIGHT_5, // 右进垄道5前进：前方超声波循迹
    STATE_BRAKE_RIGHT_5,   // 右进垄道5刹车：右侧超声波减速
    STATE_BACKWARD_5,      // 后退5：编码器计数后退
    STATE_SHIFT_LEFT_6,    // 左进垄道6：左横移入垄
    STATE_FORWARD_LEFT_6,  // 左进垄道6前进：前方超声波循迹
    STATE_BRAKE_LEFT_6,    // 左进垄道6刹车：左侧超声波减速
    STATE_RETURN_HOME,     // 返航后退：斜向返回终点
    STATE_DONE             // 完成，停止所有电机
} RobotState_t;

/* ============================================================
 * FSM 上下文结构体
 * ============================================================ */
typedef struct
{
    RobotState_t current;  /* 当前状态 */
    RobotState_t previous; /* 上一状态（用于调试） */
} FSM_Context_t;

/**
 * @brief  初始化 FSM 上下文，将状态置为 IDLE
 * @param  ctx  FSM 上下文指针
 */
void FSM_Init(FSM_Context_t *ctx);

/**
 * @brief  FSM 状态机调度函数，每次主循环调用一次
 *         内部根据 ctx->current 执行当前状态的动作，
 *         并在满足转移条件时切换到下一状态。
 * @param  ctx                 FSM 上下文指针
 * @param  Base_Speed          基础速度
 * @param  Lateral_Coeff       横移刹车系数
 * @param  Lateral_Offset      横移刹车偏置
 * @param  Forward_Coeff       直进刹车系数
 * @param  Forward_Offset      直进刹车偏置
 * @param  Reverse_Speed       后退速度
 * @param  Reverse_Distance    后退距离
 * @param  Brake_Distance      左右超声波刹车距离
 * @param  Wz                  航向角 PID 输出
 */
void FSM_Update(FSM_Context_t *ctx,
                int *Base_Speed,
                float *Lateral_Coeff, float *Lateral_Offset,
                float *Forward_Coeff, float *Forward_Offset,
                int Reverse_Speed, int Reverse_Distance, int Brake_Distance,
                float Wz);

#endif
