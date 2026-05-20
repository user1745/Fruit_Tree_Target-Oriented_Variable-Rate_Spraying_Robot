/*
 ******************************************************************************
 * 文件名称: FSM.c
 * 功能描述: 农业装备机器人行进任务的有限状态机（FSM）实现
 *
 * 重构说明:
 *   原 main.c 使用 26 个全局 flagN 变量 + 约 22 层 if-else 嵌套来描述
 *   机器人的行进任务序列，并在后退阶段使用阻塞式 while 循环。
 *   本文件将相同的控制逻辑重构为扁平化的 switch-case 有限状态机：
 *     1. 用 RobotState_t 枚举替代 flag1~flag26；
 *     2. 用非阻塞 case 替代阻塞式 while（每次主循环只执行当前状态的一步）；
 *     3. 提取重复的横移/直行 PID 控制逻辑为内部静态函数。
 *   控制参数、传感器读取、PID 调用均与 main.c 完全一致，不修改任何控制逻辑。
 *
 * 注意: FSM_Update() 必须在 Read_Encoder_cnt() 和 Ultrasonic_Get_Distance()
 *       调用之后执行，以确保传感器数据为最新值。
 ******************************************************************************
 */

#include "sys.h"
#include "FSM.h"

/* ============================================================
 * 内部辅助宏
 * ============================================================ */

/**
 * @brief 状态转移：更新 ctx->previous / ctx->current，重置编码器
 */
#define FSM_TRANSITION(ctx, next_state)  \
    do {                                 \
        (ctx)->previous = (ctx)->current;\
        (ctx)->current  = (next_state);  \
        TIM4->CNT = 0;                   \
    } while (0)

/* ============================================================
 * 内部静态辅助函数（替代 main.c 中大量重复的 PID 控制代码片段）
 * ============================================================ */

/**
 * @brief 直行（Y 方向）+ 左侧超声波 PID 横向纠偏
 *        与 main.c 中"长进直垄道"段逻辑完全一致
 *
 * @param base    基础速度（C1）
 * @param kp      纠偏比例系数
 * @param target  左侧目标距离（mm）
 * @param Wz      航向角 PID 输出
 * @param Vx1     输出：左侧速度
 * @param Vx2     输出：右侧速度
 */
static void drive_forward_left_pid(int base, float kp, int target,
                                   float Wz, float *Vx1, float *Vx2)
{
    if (UL_Dis.Left < (u16)(target - 2))
    {
        *Vx1 = base - (kp * (target - UL_Dis.Left));
        *Vx2 = base;
    }
    else if (UL_Dis.Left > (u16)(target + 2))
    {
        *Vx1 = base;
        *Vx2 = base + (kp * (target - UL_Dis.Left));
    }
    else
    {
        *Vx1 = base;
        *Vx2 = base;
    }
    Motor_Analysis(*Vx1, *Vx2, 0, 0, Wz);
}

/**
 * @brief 右横移（X 方向）+ 前方超声波 PID 对齐
 *        与 main.c 中"右进垄道"段逻辑完全一致（正值右移）
 *
 * @param base    基础速度（C1）
 * @param kp      纠偏比例系数
 * @param target  前方目标距离（mm）
 * @param Wz      航向角 PID 输出
 * @param Vx1     输出：左侧横移速度
 * @param Vx2     输出：右侧横移速度
 */
static void drive_lateral_right_fwd_pid(int base, float kp, int target,
                                        float Wz, float *Vx1, float *Vx2)
{
    if (UL_Dis.Forward <= (u16)(target - 2))
    {
        *Vx1 = base;
        *Vx2 = base - (kp * (target - UL_Dis.Forward));
    }
    else if (UL_Dis.Forward >= (u16)(target + 2))
    {
        *Vx1 = base + (kp * (target - UL_Dis.Forward));
        *Vx2 = base;
    }
    else
    {
        *Vx1 = base;
        *Vx2 = base;
    }
    Motor_Analysis(0, 0, *Vx1, *Vx2, Wz);
}

/**
 * @brief 左横移（X 方向）+ 前方超声波 PID 对齐
 *        与 main.c 中"左进垄道"段逻辑完全一致（负值左移）
 *
 * @param base    基础速度正值（C1，函数内部取负）
 * @param kp      纠偏比例系数
 * @param target  前方目标距离（mm）
 * @param Wz      航向角 PID 输出
 * @param Vx1     输出：左侧横移速度
 * @param Vx2     输出：右侧横移速度
 */
static void drive_lateral_left_fwd_pid(int base, float kp, int target,
                                       float Wz, float *Vx1, float *Vx2)
{
    if (UL_Dis.Forward <= (u16)(target - 2))
    {
        *Vx1 = -base + (kp * (target - UL_Dis.Forward));
        *Vx2 = -base;
    }
    else if (UL_Dis.Forward >= (u16)(target + 2))
    {
        *Vx1 = -base;
        *Vx2 = -base - (kp * (target - UL_Dis.Forward));
    }
    else
    {
        *Vx1 = -base;
        *Vx2 = -base;
    }
    Motor_Analysis(0, 0, *Vx1, *Vx2, Wz);
}

/* ============================================================
 * 公开接口实现
 * ============================================================ */

void FSM_Init(FSM_Context_t *ctx)
{
    ctx->current  = STATE_IDLE;
    ctx->previous = STATE_IDLE;
}

/* ------------------------------------------------------------------
 * FSM_Update
 * 每次主循环调用一次，执行当前状态动作并在条件满足时切换状态。
 * 函数为非阻塞式：每次调用仅执行一个状态的单步动作，立即返回。
 * ------------------------------------------------------------------ */
void FSM_Update(FSM_Context_t *ctx,
                int   *C1,
                float *A,  float *a,
                float *B,  float *b,
                int    back, int BACK, int stop,
                float  Wz,
                float *Vx1, float *Vx2)
{
    float pitch_tmp, roll_tmp, yaw_tmp; /* 临时姿态角，供各状态内部使用 */

    switch (ctx->current)
    {
    /* ==============================================================
     * STATE_IDLE: 空闲，等待 KEY 触发（由 main.c 负责检测 KEY 并调用）
     * 此状态下 FSM_Update 不会被 main.c 调用（main.c 判断 KEY==1 后才调用）
     * 保留此 case 作为防御性处理。
     * ============================================================== */
    case STATE_IDLE:
        Motor_Set(0, 0, 0, 0);
        break;

    /* ==============================================================
     * 短进直垄道
     * 原逻辑: if ((myabs(Encoders.cntC) < 4000) && (flag1 == 1))
     * ============================================================== */
    case STATE_SHORT_FORWARD:
        if (myabs(Encoders.cntC) < 4000)
        {
            *Vx1 = (myabs(Encoders.cntC) / 5 + 300);
            *Vx2 = (myabs(Encoders.cntC) / 5 + 300);
            mpu_dmp_get_data(&pitch_tmp, &roll_tmp, &yaw_tmp);
            Wz = ControlPID1(yaw_tmp);
            Motor_Analysis(*Vx1, *Vx2, 0, 0, Wz);
            if (myabs(Encoders.cntC) > 3000)
            {
                /* flag1 = 0 → 进入长进直垄道 */
                FSM_TRANSITION(ctx, STATE_LONG_FORWARD);
            }
        }
        break;

    /* ==============================================================
     * 长进直垄道
     * 原逻辑: if ((myabs(Encoders.cntC) < 26000) && (flag1==0) && (flag2==1))
     * ============================================================== */
    case STATE_LONG_FORWARD:
        if (myabs(Encoders.cntC) < 26000)
        {
            mpu_dmp_get_data(&pitch_tmp, &roll_tmp, &yaw_tmp);
            Wz = ControlPID1(yaw_tmp);
            /* 左侧超声波 PID 循迹，目标距离 74 mm，系数 5 */
            if (UL_Dis.Left < 72)
            {
                *Vx1 = *C1 - (5 * (74 - UL_Dis.Left));
                *Vx2 = *C1;
            }
            else if (UL_Dis.Left > 76)
            {
                *Vx1 = *C1;
                *Vx2 = *C1 + (5 * (74 - UL_Dis.Left));
            }
            else
            {
                *Vx1 = *C1;
                *Vx2 = *C1;
            }
            Motor_Analysis(*Vx1, *Vx2, 0, 0, Wz);
            if (myabs(Encoders.cntC) > 22000)
            {
                /* flag2 = 0 → 进入直进刹车 */
                FSM_TRANSITION(ctx, STATE_BRAKE_FORWARD);
            }
        }
        break;

    /* ==============================================================
     * 直进垄道刹车
     * 原逻辑: if ((flag2==0) && (flag3==1))
     * ============================================================== */
    case STATE_BRAKE_FORWARD:
        *C1  = 900;
        mpu_dmp_get_data(&pitch_tmp, &roll_tmp, &yaw_tmp);
        *B   = 1.2f;
        *b   = 50;
        if (UL_Dis.Forward > 10)
        {
            *C1 = (UL_Dis.Forward * (*B) - (*b));
            if (UL_Dis.Left < 72)
            {
                *Vx1 = *C1 - (3 * (74 - UL_Dis.Left));
                *Vx2 = *C1;
            }
            else if (UL_Dis.Left > 76)
            {
                *Vx1 = *C1;
                *Vx2 = *C1 + (3 * (74 - UL_Dis.Left));
            }
            Motor_Analysis(*Vx1, *Vx2, 0, 0, Wz);
        }
        if (UL_Dis.Forward < 260)
        {
            /* flag3 = 0 → 进入右进垄道1 */
            FSM_TRANSITION(ctx, STATE_SHIFT_RIGHT_1);
            Motor_Set(0, 0, 0, 0);
        }
        break;

    /* ==============================================================
     * 右进垄道1：右横移
     * 原逻辑: if ((myabs(Encoders.cntC) < 16000) && (flag4==1) && (flag3==0))
     * ============================================================== */
    case STATE_SHIFT_RIGHT_1:
        Motor_Set(0, 0, 0, 0);
        Read_Encoder_cnt();
        if (myabs(Encoders.cntC) < 16000)
        {
            if (myabs(Encoders.cntC) < 1500)
                *C1 = myabs(Encoders.cntC) / 1.2f + 200;
            else
                *C1 = 950;

            drive_lateral_right_fwd_pid(*C1, 5.0f, 95, Wz, Vx1, Vx2);

            if (myabs(Encoders.cntC) > 15000)
            {
                /* flag4 = 0 → 进入右进垄道1刹车 */
                FSM_TRANSITION(ctx, STATE_BRAKE_RIGHT_1);
            }
        }
        break;

    /* ==============================================================
     * 右进垄道1刹车：右侧超声波减速
     * 原逻辑: if ((flag4==0) && (UL_Dis.Right > 10) && (flag5==1))
     * ============================================================== */
    case STATE_BRAKE_RIGHT_1:
        *A = 1.15f;
        *a = 50;
        if (UL_Dis.Right > 10)
        {
            if (UL_Dis.Forward <= 93)
            {
                *Vx1 = (UL_Dis.Right * (*A) - (*a));
                *Vx2 = (UL_Dis.Right * (*A) - (*a)) - (4 * (95 - UL_Dis.Forward));
            }
            else if (UL_Dis.Forward >= 97)
            {
                *Vx1 = (UL_Dis.Right * (*A) - (*a)) + (4 * (95 - UL_Dis.Forward));
                *Vx2 = (UL_Dis.Right * (*A) - (*a));
            }
            else
            {
                *Vx1 = (UL_Dis.Right * (*A) - (*a));
                *Vx2 = (UL_Dis.Right * (*A) - (*a));
            }
            Motor_Analysis(0, 0, *Vx1, *Vx2, Wz);
            if (UL_Dis.Right <= (stop + 10))
            {
                /* flag5 = 0 → 进入后退1 */
                FSM_TRANSITION(ctx, STATE_BACKWARD_1);
            }
        }
        break;

    /* ==============================================================
     * 后退1
     * 原逻辑: while ((flag6==1) && (flag5==0)) { ... }
     *         非阻塞化：每次主循环调用执行一步，条件满足后转移状态
     * ============================================================== */
    case STATE_BACKWARD_1:
        Usart3_Send(5, 1);
        Read_Encoder_cnt();
        if (myabs(Encoders.cntC) < (BACK + 150))
        {
            mpu_dmp_get_data(&pitch_tmp, &roll_tmp, &yaw_tmp);
            Wz = ControlPID1(yaw_tmp);
            Motor_Analysis(back, back, 0, 0, Wz);
        }
        else
        {
            /* flag6 = 0 → 进入左进垄道2 */
            FSM_TRANSITION(ctx, STATE_SHIFT_LEFT_2);
            Motor_Set(0, 0, 0, 0);
        }
        break;

    /* ==============================================================
     * 左进垄道2：左横移
     * 原逻辑: if ((UL_Dis.Right<=600) && (UL_Dis.Right>=20) && (flag6==0) && (flag7==1))
     * ============================================================== */
    case STATE_SHIFT_LEFT_2:
        Motor_Set(0, 0, 0, 0);
        Read_Encoder_cnt();
        if ((UL_Dis.Right <= 600) && (UL_Dis.Right >= 20))
        {
            *C1  = UL_Dis.Right * 3 + 150;
            *Vx1 = -(*C1);
            *Vx2 = -(*C1);
            Motor_Analysis(0, 0, *Vx1, *Vx2, Wz);
            if (UL_Dis.Right >= 300)
            {
                /* flag7 = 0 → 进入左进垄道2前进 */
                FSM_TRANSITION(ctx, STATE_FORWARD_LEFT_2);
            }
        }
        break;

    /* ==============================================================
     * 左进垄道2前进
     * 原逻辑: if ((myabs(Encoders.cntC) < 16000) && (flag8==1) && (flag7==0))
     * ============================================================== */
    case STATE_FORWARD_LEFT_2:
        Read_Encoder_cnt();
        if (myabs(Encoders.cntC) < 16000)
        {
            *C1 = 950;
            drive_lateral_left_fwd_pid(*C1, 6.5f, 95, Wz, Vx1, Vx2);
            if (myabs(Encoders.cntC) > 11500)
            {
                /* flag8 = 0 → 进入左进垄道2刹车 */
                FSM_TRANSITION(ctx, STATE_BRAKE_LEFT_2);
            }
        }
        break;

    /* ==============================================================
     * 左进垄道2刹车：左侧超声波减速
     * 原逻辑: if ((flag8==0) && (UL_Dis.Left > 10) && (flag9==1))
     * ============================================================== */
    case STATE_BRAKE_LEFT_2:
        *A = 1.15f;
        *a = 50;
        if (UL_Dis.Left > 10)
        {
            *Vx1 = -(UL_Dis.Left * (*A) - (*a));
            *Vx2 = -(UL_Dis.Left * (*A) - (*a));
            Motor_Analysis(0, 0, *Vx1, *Vx2, Wz);
            if (UL_Dis.Left <= stop + 30)
            {
                /* flag9 = 0 → 进入后退2 */
                FSM_TRANSITION(ctx, STATE_BACKWARD_2);
            }
        }
        break;

    /* ==============================================================
     * 后退2
     * 原逻辑: while ((flag10==1) && (flag9==0)) { ... }
     * ============================================================== */
    case STATE_BACKWARD_2:
        Usart3_Send(5, 0);
        Read_Encoder_cnt();
        if (myabs(Encoders.cntC) < (BACK - 450))
        {
            mpu_dmp_get_data(&pitch_tmp, &roll_tmp, &yaw_tmp);
            Wz = ControlPID1(yaw_tmp);
            Motor_Analysis(back, back, 0, 0, Wz);
        }
        else
        {
            /* flag10 = 0 → 进入右进垄道3 */
            FSM_TRANSITION(ctx, STATE_SHIFT_RIGHT_3);
            TIM4->CNT = 0;
        }
        break;

    /* ==============================================================
     * 右进垄道3：右横移
     * 原逻辑: if ((UL_Dis.Left<=600) && (UL_Dis.Left>=20) && (flag10==0) && (flag11==1))
     * ============================================================== */
    case STATE_SHIFT_RIGHT_3:
        Motor_Set(0, 0, 0, 0);
        Read_Encoder_cnt();
        if ((UL_Dis.Left <= 600) && (UL_Dis.Left >= 20))
        {
            *C1  = UL_Dis.Left * 4 + 200;
            *Vx1 = *C1;
            *Vx2 = *C1;
            Motor_Analysis(0, 0, *Vx1, *Vx2, Wz);
            if (UL_Dis.Left >= 300)
            {
                /* flag11 = 0 → 进入右进垄道3前进 */
                FSM_TRANSITION(ctx, STATE_FORWARD_RIGHT_3);
            }
        }
        break;

    /* ==============================================================
     * 右进垄道3前进
     * 原逻辑: if ((myabs(Encoders.cntC) < 16000) && (flag12==1) && (flag11==0))
     * ============================================================== */
    case STATE_FORWARD_RIGHT_3:
        Read_Encoder_cnt();
        if (myabs(Encoders.cntC) < 16000)
        {
            *C1 = 950;
            drive_lateral_right_fwd_pid(*C1, 6.5f, 95, Wz, Vx1, Vx2);
            if (myabs(Encoders.cntC) > 11500)
            {
                /* flag12 = 0 → 进入右进垄道3刹车 */
                FSM_TRANSITION(ctx, STATE_BRAKE_RIGHT_3);
            }
        }
        break;

    /* ==============================================================
     * 右进垄道3刹车：右侧超声波减速
     * 原逻辑: if ((flag12==0) && (UL_Dis.Right > 10) && (flag13==1))
     * ============================================================== */
    case STATE_BRAKE_RIGHT_3:
        *A = 1.2f;
        *a = 50;
        if (UL_Dis.Right > 10)
        {
            *Vx1 = (UL_Dis.Right * (*A) - (*a));
            *Vx2 = (UL_Dis.Right * (*A) - (*a));
            Motor_Analysis(0, 0, *Vx1, *Vx2, Wz);
            if (UL_Dis.Right <= stop)
            {
                /* flag13 = 0 → 进入后退3 */
                FSM_TRANSITION(ctx, STATE_BACKWARD_3);
            }
        }
        break;

    /* ==============================================================
     * 后退3
     * 原逻辑: while ((flag13==0) && (flag14==1)) { ... }
     * ============================================================== */
    case STATE_BACKWARD_3:
        Usart3_Send(5, 1);
        Read_Encoder_cnt();
        if (myabs(Encoders.cntC) < (BACK + 210))
        {
            mpu_dmp_get_data(&pitch_tmp, &roll_tmp, &yaw_tmp);
            Wz = ControlPID1(yaw_tmp);
            Motor_Analysis(back, back, 0, 0, Wz);
        }
        else
        {
            /* flag14 = 0 → 进入左进垄道4 */
            FSM_TRANSITION(ctx, STATE_SHIFT_LEFT_4);
            TIM4->CNT = 0;
        }
        break;

    /* ==============================================================
     * 左进垄道4：左横移
     * 原逻辑: if ((UL_Dis.Right<=450) && (UL_Dis.Right>=20) && (flag14==0) && (flag15==1))
     * ============================================================== */
    case STATE_SHIFT_LEFT_4:
        Motor_Set(0, 0, 0, 0);
        Read_Encoder_cnt();
        if ((UL_Dis.Right <= 450) && (UL_Dis.Right >= 20))
        {
            *C1  = UL_Dis.Right * 4 + 150;
            *Vx1 = -(*C1);
            *Vx2 = -(*C1);
            Motor_Analysis(0, 0, *Vx1, *Vx2, Wz);
            if (UL_Dis.Right >= 300)
            {
                /* flag15 = 0 → 进入左进垄道4前进 */
                FSM_TRANSITION(ctx, STATE_FORWARD_LEFT_4);
            }
        }
        break;

    /* ==============================================================
     * 左进垄道4前进
     * 原逻辑: if ((myabs(Encoders.cntC) < 16000) && (flag16==1) && (flag15==0))
     * ============================================================== */
    case STATE_FORWARD_LEFT_4:
        Read_Encoder_cnt();
        if (myabs(Encoders.cntC) < 16000)
        {
            *C1 = 950;
            drive_lateral_left_fwd_pid(*C1, 6.5f, 95, Wz, Vx1, Vx2);
            if (myabs(Encoders.cntC) > 11500)
            {
                /* flag16 = 0 → 进入左进垄道4刹车 */
                FSM_TRANSITION(ctx, STATE_BRAKE_LEFT_4);
            }
        }
        break;

    /* ==============================================================
     * 左进垄道4刹车：左侧超声波减速
     * 原逻辑: if ((flag16==0) && (UL_Dis.Left > 10) && (flag17==1))
     * ============================================================== */
    case STATE_BRAKE_LEFT_4:
        *A = 1.2f;
        *a = 50;
        if (UL_Dis.Left > 10)
        {
            *Vx1 = -(UL_Dis.Left * (*A) - (*a));
            *Vx2 = -(UL_Dis.Left * (*A) - (*a));
            Motor_Analysis(0, 0, *Vx1, *Vx2, Wz);
            if (UL_Dis.Left <= stop)
            {
                /* flag17 = 0 → 进入后退4 */
                FSM_TRANSITION(ctx, STATE_BACKWARD_4);
            }
        }
        break;

    /* ==============================================================
     * 后退4
     * 原逻辑: while ((flag18==1) && (flag17==0)) { ... }
     * ============================================================== */
    case STATE_BACKWARD_4:
        Usart3_Send(5, 0);
        Read_Encoder_cnt();
        if (myabs(Encoders.cntC) < (BACK - 200))
        {
            mpu_dmp_get_data(&pitch_tmp, &roll_tmp, &yaw_tmp);
            Wz = ControlPID1(yaw_tmp);
            Motor_Analysis(back, back, 0, 0, Wz);
        }
        else
        {
            /* flag18 = 0 → 进入右进垄道5 */
            FSM_TRANSITION(ctx, STATE_SHIFT_RIGHT_5);
            TIM4->CNT = 0;
        }
        break;

    /* ==============================================================
     * 右进垄道5：右横移
     * 原逻辑: if ((UL_Dis.Left<=600) && (UL_Dis.Left>=20) && (flag18==0) && (flag19==1))
     * ============================================================== */
    case STATE_SHIFT_RIGHT_5:
        Motor_Set(0, 0, 0, 0);
        Read_Encoder_cnt();
        if ((UL_Dis.Left <= 600) && (UL_Dis.Left >= 20))
        {
            *C1  = UL_Dis.Left * 4 + 150;
            *Vx1 = *C1;
            *Vx2 = *C1;
            Motor_Analysis(0, 0, *Vx1, *Vx2, Wz);
            if (UL_Dis.Left >= 300)
            {
                /* flag19 = 0 → 进入右进垄道5前进 */
                FSM_TRANSITION(ctx, STATE_FORWARD_RIGHT_5);
            }
        }
        break;

    /* ==============================================================
     * 右进垄道5前进
     * 原逻辑: if ((myabs(Encoders.cntC) < 16000) && (flag20==1) && (flag19==0))
     * ============================================================== */
    case STATE_FORWARD_RIGHT_5:
        Read_Encoder_cnt();
        if (myabs(Encoders.cntC) < 16000)
        {
            *C1 = 950;
            drive_lateral_right_fwd_pid(*C1, 6.5f, 95, Wz, Vx1, Vx2);
            if (myabs(Encoders.cntC) > 11500)
            {
                /* flag20 = 0 → 进入右进垄道5刹车 */
                FSM_TRANSITION(ctx, STATE_BRAKE_RIGHT_5);
            }
        }
        break;

    /* ==============================================================
     * 右进垄道5刹车：右侧超声波减速
     * 原逻辑: if ((flag20==0) && (UL_Dis.Right > 10) && (flag21==1))
     * ============================================================== */
    case STATE_BRAKE_RIGHT_5:
        *A = 1.2f;
        *a = 50;
        if (UL_Dis.Right > 10)
        {
            *Vx1 = (UL_Dis.Right * (*A) - (*a));
            *Vx2 = (UL_Dis.Right * (*A) - (*a));
            Motor_Analysis(0, 0, *Vx1, *Vx2, Wz);
            if (UL_Dis.Right <= stop)
            {
                /* flag21 = 0 → 进入后退5 */
                FSM_TRANSITION(ctx, STATE_BACKWARD_5);
            }
        }
        break;

    /* ==============================================================
     * 后退5
     * 原逻辑: while ((flag21==0) && (flag22==1)) { ... }
     * ============================================================== */
    case STATE_BACKWARD_5:
        Usart3_Send(5, 1);
        Read_Encoder_cnt();
        if (myabs(Encoders.cntC) < (BACK - 100))
        {
            mpu_dmp_get_data(&pitch_tmp, &roll_tmp, &yaw_tmp);
            Wz = ControlPID1(yaw_tmp);
            Motor_Analysis(back, back, 0, 0, Wz);
        }
        else
        {
            /* flag22 = 0 → 进入左进垄道6 */
            FSM_TRANSITION(ctx, STATE_SHIFT_LEFT_6);
            Motor_Set(0, 0, 0, 0);
        }
        break;

    /* ==============================================================
     * 左进垄道6：左横移
     * 原逻辑: if ((UL_Dis.Right<=600) && (UL_Dis.Right>=20) && (flag22==0) && (flag23==1))
     * ============================================================== */
    case STATE_SHIFT_LEFT_6:
        Motor_Set(0, 0, 0, 0);
        Read_Encoder_cnt();
        if ((UL_Dis.Right <= 600) && (UL_Dis.Right >= 20))
        {
            *C1  = UL_Dis.Right * 3 + 150;
            *Vx1 = -(*C1);
            *Vx2 = -(*C1);
            Motor_Analysis(0, 0, *Vx1, *Vx2, Wz);
            if (UL_Dis.Right >= 300)
            {
                /* flag23 = 0 → 进入左进垄道6前进 */
                FSM_TRANSITION(ctx, STATE_FORWARD_LEFT_6);
            }
        }
        break;

    /* ==============================================================
     * 左进垄道6前进
     * 原逻辑: if ((myabs(Encoders.cntC) < 16000) && (flag24==1) && (flag23==0))
     * ============================================================== */
    case STATE_FORWARD_LEFT_6:
        Read_Encoder_cnt();
        if (myabs(Encoders.cntC) < 16000)
        {
            *C1 = 950;
            /* 注意：原代码此段 Forward >= 96（非97），保持原逻辑 */
            if (UL_Dis.Forward <= 93)
            {
                *Vx1 = -(*C1) + (6.5f * (95 - UL_Dis.Forward));
                *Vx2 = -(*C1);
            }
            else if (UL_Dis.Forward >= 96)
            {
                *Vx1 = -(*C1);
                *Vx2 = -(*C1) - (6.5f * (95 - UL_Dis.Forward));
            }
            else
            {
                *Vx1 = -(*C1);
                *Vx2 = -(*C1);
            }
            Motor_Analysis(0, 0, *Vx1, *Vx2, Wz);
            if (myabs(Encoders.cntC) > 11500)
            {
                /* flag24 = 0 → 进入左进垄道6刹车 */
                FSM_TRANSITION(ctx, STATE_BRAKE_LEFT_6);
            }
        }
        break;

    /* ==============================================================
     * 左进垄道6刹车：左侧超声波减速
     * 原逻辑: if ((flag24==0) && (UL_Dis.Left > 10) && (flag25==1))
     * ============================================================== */
    case STATE_BRAKE_LEFT_6:
        *A = 1.2f;
        *a = 50;
        if (UL_Dis.Left > 10)
        {
            *Vx1 = -(UL_Dis.Left * (*A) - (*a));
            *Vx2 = -(UL_Dis.Left * (*A) - (*a));
            Motor_Analysis(0, 0, *Vx1, *Vx2, Wz);
            if (UL_Dis.Left <= (stop + 50))
            {
                /* flag25 = 0 → 进入返航后退 */
                FSM_TRANSITION(ctx, STATE_RETURN_HOME);
            }
        }
        break;

    /* ==============================================================
     * 返航后退
     * 原逻辑: while ((flag26==1) && (flag25==0)) { ... }
     * ============================================================== */
    case STATE_RETURN_HOME:
        Read_Encoder_cnt();
        if (myabs(Encoders.cntC) < 3400)
        {
            mpu_dmp_get_data(&pitch_tmp, &roll_tmp, &yaw_tmp);
            Wz = ControlPID1(yaw_tmp);
            Motor_Analysis(-850, -700, 0, 0, Wz);
        }
        else
        {
            /* flag26 = 0 → 任务完成 */
            FSM_TRANSITION(ctx, STATE_DONE);
            Motor_Set(0, 0, 0, 0);
        }
        break;

    /* ==============================================================
     * STATE_DONE: 全部任务完成，停止
     * ============================================================== */
    case STATE_DONE:
        Motor_Set(0, 0, 0, 0);
        break;

    default:
        Motor_Set(0, 0, 0, 0);
        break;
    }
}
