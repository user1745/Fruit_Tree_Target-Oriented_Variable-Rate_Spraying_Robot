#ifndef __FSM_H
#define __FSM_H

#include "sys.h"

/*
 ******************************************************************************
 * 文件名称: FSM.h
 * 功能描述: 农业装备机器人行进任务的有限状态机（FSM）定义
 *           将 main.c 中深层嵌套的 if-else 逻辑重构为扁平化 switch-case 结构，
 *           用单一枚举变量替代原 flag1~flag26 共 26 个全局标志位。
 * 注意事项: 不修改任何原有控制逻辑，仅做结构重构。
 ******************************************************************************
 */

/* ============================================================
 * 状态枚举：对应 main.c 中各注释段落的运动阶段
 * ============================================================ */
typedef enum
{
    STATE_IDLE = 0,       /* 空闲，等待 KEY 启动                    (原: KEY==0)         */
    STATE_SHORT_FORWARD,  /* 短进直垄道：入垄加速段                  (原: flag1==1)       */
    STATE_LONG_FORWARD,   /* 长进直垄道：超声波左侧循迹              (原: flag1==0,flag2==1) */
    STATE_BRAKE_FORWARD,  /* 直进垄道刹车：前方超声波减速            (原: flag2==0,flag3==1) */
    STATE_SHIFT_RIGHT_1,  /* 右进垄道1：右横移入垄                   (原: flag3==0,flag4==1) */
    STATE_BRAKE_RIGHT_1,  /* 右进垄道1刹车：右侧超声波减速           (原: flag4==0,flag5==1) */
    STATE_BACKWARD_1,     /* 后退1：编码器计数后退                   (原: flag5==0,flag6==1) */
    STATE_SHIFT_LEFT_2,   /* 左进垄道2：左横移入垄                   (原: flag6==0,flag7==1) */
    STATE_FORWARD_LEFT_2, /* 左进垄道2前进：前方超声波循迹           (原: flag7==0,flag8==1) */
    STATE_BRAKE_LEFT_2,   /* 左进垄道2刹车：左侧超声波减速           (原: flag8==0,flag9==1) */
    STATE_BACKWARD_2,     /* 后退2：编码器计数后退                   (原: flag9==0,flag10==1)*/
    STATE_SHIFT_RIGHT_3,  /* 右进垄道3：右横移入垄                   (原: flag10==0,flag11==1)*/
    STATE_FORWARD_RIGHT_3,/* 右进垄道3前进：前方超声波循迹           (原: flag11==0,flag12==1)*/
    STATE_BRAKE_RIGHT_3,  /* 右进垄道3刹车：右侧超声波减速           (原: flag12==0,flag13==1)*/
    STATE_BACKWARD_3,     /* 后退3：编码器计数后退                   (原: flag13==0,flag14==1)*/
    STATE_SHIFT_LEFT_4,   /* 左进垄道4：左横移入垄                   (原: flag14==0,flag15==1)*/
    STATE_FORWARD_LEFT_4, /* 左进垄道4前进：前方超声波循迹           (原: flag15==0,flag16==1)*/
    STATE_BRAKE_LEFT_4,   /* 左进垄道4刹车：左侧超声波减速           (原: flag16==0,flag17==1)*/
    STATE_BACKWARD_4,     /* 后退4：编码器计数后退                   (原: flag17==0,flag18==1)*/
    STATE_SHIFT_RIGHT_5,  /* 右进垄道5：右横移入垄                   (原: flag18==0,flag19==1)*/
    STATE_FORWARD_RIGHT_5,/* 右进垄道5前进：前方超声波循迹           (原: flag19==0,flag20==1)*/
    STATE_BRAKE_RIGHT_5,  /* 右进垄道5刹车：右侧超声波减速           (原: flag20==0,flag21==1)*/
    STATE_BACKWARD_5,     /* 后退5：编码器计数后退                   (原: flag21==0,flag22==1)*/
    STATE_SHIFT_LEFT_6,   /* 左进垄道6：左横移入垄                   (原: flag22==0,flag23==1)*/
    STATE_FORWARD_LEFT_6, /* 左进垄道6前进：前方超声波循迹           (原: flag23==0,flag24==1)*/
    STATE_BRAKE_LEFT_6,   /* 左进垄道6刹车：左侧超声波减速           (原: flag24==0,flag25==1)*/
    STATE_RETURN_HOME,    /* 返航后退：斜向返回终点                  (原: flag25==0,flag26==1)*/
    STATE_DONE,           /* 完成，停止所有电机                      (原: flag26==0)      */
    STATE_COUNT           /* 状态总数（边界检查，不作为实际状态使用）                      */
} RobotState_t;

/* ============================================================
 * FSM 上下文结构体：替代全局 flag1~flag26
 * ============================================================ */
typedef struct
{
    RobotState_t current;  /* 当前状态 */
    RobotState_t previous; /* 上一状态（用于调试） */
} FSM_Context_t;

/* ============================================================
 * 对外接口
 * ============================================================ */

/**
 * @brief  初始化 FSM 上下文，将状态置为 IDLE
 * @param  ctx  FSM 上下文指针
 */
void FSM_Init(FSM_Context_t *ctx);

/**
 * @brief  FSM 状态机调度函数，每次主循环调用一次
 *         内部根据 ctx->current 执行当前状态的动作，
 *         并在满足转移条件时切换到下一状态。
 * @param  ctx     FSM 上下文指针
 * @param  C1      速度基准值（与 main.c 共享）
 * @param  A       横移刹车系数（与 main.c 共享）
 * @param  a       横移刹车偏置（与 main.c 共享）
 * @param  B       直进刹车系数（与 main.c 共享）
 * @param  b       直进刹车偏置（与 main.c 共享）
 * @param  back    后退速度（与 main.c 共享）
 * @param  BACK    后退距离阈值（与 main.c 共享）
 * @param  stop    左右超声波刹车距离（与 main.c 共享）
 * @param  Wz      航向角 PID 输出（与 main.c 共享）
 * @param  Vx1     左侧速度分量输出指针（与 main.c 共享）
 * @param  Vx2     右侧速度分量输出指针（与 main.c 共享）
 */
void FSM_Update(FSM_Context_t *ctx,
                int   *C1,
                float *A,  float *a,
                float *B,  float *b,
                int    back, int BACK, int stop,
                float  Wz,
                float *Vx1, float *Vx2);

#endif /* __FSM_H */
