#include "sys.h"
#include "FSM.h"

/**
 * @brief 电机输出控制安全限幅代理函数，限制最大目标速度，防止系统发生物理失控或过载烧毁。
 */
static void Motor_Analysis_Safe(float Vy1, float Vy2, float Vx1, float Vx2, float Wz)
{
    if (Vy1 > 1000.0f) Vy1 = 1000.0f; else if (Vy1 < -1000.0f) Vy1 = -1000.0f;
    if (Vy2 > 1000.0f) Vy2 = 1000.0f; else if (Vy2 < -1000.0f) Vy2 = -1000.0f;
    if (Vx1 > 1000.0f) Vx1 = 1000.0f; else if (Vx1 < -1000.0f) Vx1 = -1000.0f;
    if (Vx2 > 1000.0f) Vx2 = 1000.0f; else if (Vx2 < -1000.0f) Vx2 = -1000.0f;
    if (Wz > 1000.0f) Wz = 1000.0f; else if (Wz < -1000.0f) Wz = -1000.0f;
    
    Motor_Analysis(Vy1, Vy2, Vx1, Vx2, Wz);
}

/* 使用宏重定向所有后续的 Motor_Analysis 调用，提供透明的安全保护屏障 */
#define Motor_Analysis(vy1, vy2, vx1, vx2, wz) Motor_Analysis_Safe(vy1, vy2, vx1, vx2, wz)

// static float Vy1; // Vy为正，前进；Vy为负，后退
// static float Vy2; // Vy为正，前进；Vy为负，后退
float Vx1 = 0.0f; // Vx设正，1、4轮子为负，2、3轮子为正，小车左移
float Vx2 = 0.0f; // Vx设正，1、4轮子为负，2、3轮子为正，小车左移

/**
 * @brief 状态转移：更新 ctx->previous / ctx->current，重置编码器
 */
#define FSM_TRANSITION(ctx, next_state)   \
    do                                    \
    {                                     \
        (ctx)->previous = (ctx)->current; \
        (ctx)->current = (next_state);    \
        TIM4->CNT = 0;                    \
    } while (0)

/**
 * @brief 右横移（X 方向）+ 前方超声波 PID 对齐
 *
 * @param base    基础速度
 * @param kp      纠偏比例系数
 * @param target  前方目标距离（mm）
 * @param Wz      航向角 PID 输出
 * @param Vx1     输出：左侧横移速度
 * @param Vx2     输出：右侧横移速度
 */
static void drive_lateral_right_fwd_pid(int base, float kp, int target, float Wz)
{
    if (UL_Dis.Forward <= (u16)(target - 2))
    {
        Vx1 = base;
        Vx2 = base - (kp * (target - UL_Dis.Forward));
    }
    else if (UL_Dis.Forward >= (u16)(target + 2))
    {
        Vx1 = base + (kp * (target - UL_Dis.Forward));
        Vx2 = base;
    }
    else
    {
        Vx1 = base;
        Vx2 = base;
    }
    Motor_Analysis(0, 0, Vx1, Vx2, Wz);
}

/**
 * @brief 左横移（X 方向）+ 前方超声波 PID 对齐
 *
 * @param base    基础速度
 * @param kp      纠偏比例系数
 * @param target  前方目标距离（mm）
 * @param Wz      航向角 PID 输出
 * @param Vx1     输出：左侧横移速度
 * @param Vx2     输出：右侧横移速度
 */
static void drive_lateral_left_fwd_pid(int base, float kp, int target, float Wz)
{
    if (UL_Dis.Forward <= (u16)(target - 2))
    {
        Vx1 = -base + (kp * (target - UL_Dis.Forward));
        Vx2 = -base;
    }
    else if (UL_Dis.Forward >= (u16)(target + 2))
    {
        Vx1 = -base;
        Vx2 = -base - (kp * (target - UL_Dis.Forward));
    }
    else
    {
        Vx1 = -base;
        Vx2 = -base;
    }
    Motor_Analysis(0, 0, Vx1, Vx2, Wz);
}

/**
 * @brief  初始化 FSM 上下文，将状态置为 IDLE
 * @param  ctx  FSM 上下文指针
 */
void FSM_Init(FSM_Context_t *ctx)
{
    ctx->current = STATE_IDLE;
    ctx->previous = STATE_IDLE;
}

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
 * @param  Vx1                 左侧速度分量输出指针
 * @param  Vx2                 右侧速度分量输出指针
 */
void FSM_Update(FSM_Context_t *ctx,
                int *Base_Speed,
                float *Lateral_Coeff, float *Lateral_Offset,
                float *Forward_Coeff, float *Forward_Offset,
                int Reverse_Speed, int Reverse_Distance, int Brake_Distance,
                float Wz)
{
    static float Vx1_val = 0.0f;
    static float Vx2_val = 0.0f;
    float *Vx1 = &Vx1_val;
    float *Vx2 = &Vx2_val;
    static int CNT;
    CNT = myabs(Encoders.cntC);
    switch (ctx->current)
    {
        
    /* ==============================================================
     * STATE_IDLE: 空闲，等待 KEY 触发（由 main.c 负责检测 KEY 并调用）
     * 此状态下 FSM_Update 不会被 main.c 调用（main.c 判断 KEY==1 后才调用）
     * ============================================================== */
    case STATE_IDLE:
        /* KEY 已按下 → 直接进入短进直垄道 */
        FSM_TRANSITION(ctx, STATE_SHORT_FORWARD);
        break;

    /* ==============================================================
     * 短进直垄道
     * ============================================================== */
    case STATE_SHORT_FORWARD:
        if (CNT < 4000)
        {
            Vx1 = (CNT / 5 + 300);
            Vx2 = (CNT / 5 + 300);
            Motor_Analysis(Vx1, Vx2, 0, 0, Wz);
            if (CNT > 3000)
            {
                FSM_TRANSITION(ctx, STATE_LONG_FORWARD);
            }
        }
        break;

    /* ==============================================================
     * 长进直垄道
     * ============================================================== */
    case STATE_LONG_FORWARD:
        if (CNT < 26000)
        {
            if (UL_Dis.Left < 72)
            {
                Vx1 = *Base_Speed - (5 * (74 - UL_Dis.Left));
                Vx2 = *Base_Speed;
            }
            else if (UL_Dis.Left > 76)
            {
                Vx1 = *Base_Speed;
                Vx2 = *Base_Speed + (5 * (74 - UL_Dis.Left));
            }
            else
            {
                Vx1 = *Base_Speed;
                Vx2 = *Base_Speed;
            }
            Motor_Analysis(Vx1, Vx2, 0, 0, Wz);
            if (CNT > 22000)
            {
                FSM_TRANSITION(ctx, STATE_BRAKE_FORWARD);
            }
        }
        break;

    /* ==============================================================
     * 直进垄道刹车
     * ============================================================== */
    case STATE_BRAKE_FORWARD:
        *Base_Speed = 900;
        *Forward_Coeff = 1.2f;
        *Forward_Offset = 50;
        if (UL_Dis.Forward > 10)
        {
            *Base_Speed = (UL_Dis.Forward * (*Forward_Coeff) - (*Forward_Offset));
            if (UL_Dis.Left < 72)
            {
                Vx1 = *Base_Speed - (3 * (74 - UL_Dis.Left));
                Vx2 = *Base_Speed;
            }
            else if (UL_Dis.Left > 76)
            {
                Vx1 = *Base_Speed;
                Vx2 = *Base_Speed + (3 * (74 - UL_Dis.Left));
            }
            Motor_Analysis(Vx1, Vx2, 0, 0, Wz);
        }
        if (UL_Dis.Forward < 260)
        {
            FSM_TRANSITION(ctx, STATE_SHIFT_RIGHT_1);
            Motor_Set(0, 0, 0, 0);
        }
        break;

    /* ==============================================================
     * 右进垄道1：右横移
     * ============================================================== */
    case STATE_SHIFT_RIGHT_1:
        Motor_Set(0, 0, 0, 0);
        Read_Encoder_cnt();
        CNT = myabs(Encoders.cntC);
        if (CNT < 16000)
        {
            if (CNT < 1500)
                *Base_Speed = CNT / 1.2f + 200;
            else
                *Base_Speed = 950;

            drive_lateral_right_fwd_pid(*Base_Speed, 5.0f, 95, Wz);

            if (CNT > 15000)
            {
                FSM_TRANSITION(ctx, STATE_BRAKE_RIGHT_1);
            }
        }
        break;

    /* ==============================================================
     * 右进垄道1刹车：右侧超声波减速
     * ============================================================== */
    case STATE_BRAKE_RIGHT_1:
        *Lateral_Coeff = 1.15f;
        *Lateral_Offset = 50;
        if (UL_Dis.Right > 10)
        {
            if (UL_Dis.Forward <= 93)
            {
                Vx1 = (UL_Dis.Right * (*Lateral_Coeff) - (*Lateral_Offset));
                Vx2 = (UL_Dis.Right * (*Lateral_Coeff) - (*Lateral_Offset)) - (4 * (95 - UL_Dis.Forward));
            }
            else if (UL_Dis.Forward >= 97)
            {
                Vx1 = (UL_Dis.Right * (*Lateral_Coeff) - (*Lateral_Offset)) + (4 * (95 - UL_Dis.Forward));
                Vx2 = (UL_Dis.Right * (*Lateral_Coeff) - (*Lateral_Offset));
            }
            else
            {
                Vx1 = (UL_Dis.Right * (*Lateral_Coeff) - (*Lateral_Offset));
                Vx2 = (UL_Dis.Right * (*Lateral_Coeff) - (*Lateral_Offset));
            }
            Motor_Analysis(0, 0, Vx1, Vx2, Wz);
            if (UL_Dis.Right <= (Brake_Distance + 10))
            {
                FSM_TRANSITION(ctx, STATE_BACKWARD_1);
            }
        }
        break;

    /* ==============================================================
     * 后退1
     * ============================================================== */
    case STATE_BACKWARD_1:
        Usart3_Send(5, 1);
        Read_Encoder_cnt();
        CNT = myabs(Encoders.cntC);
        if (CNT < (Reverse_Distance + 150))
        {
            Motor_Analysis(Reverse_Speed, Reverse_Speed, 0, 0, Wz);
        }
        else
        {
            FSM_TRANSITION(ctx, STATE_SHIFT_LEFT_2);
            Motor_Set(0, 0, 0, 0);
        }
        break;

    /* ==============================================================
     * 左进垄道2：左横移
     * ============================================================== */
    case STATE_SHIFT_LEFT_2:
        Motor_Set(0, 0, 0, 0);
        Read_Encoder_cnt();
        if ((UL_Dis.Right <= 600) && (UL_Dis.Right >= 20))
        {
            *Base_Speed = UL_Dis.Right * 3 + 150;
            Vx1 = -(*Base_Speed);
            Vx2 = -(*Base_Speed);
            Motor_Analysis(0, 0, Vx1, Vx2, Wz);
            if (UL_Dis.Right >= 300)
            {
                FSM_TRANSITION(ctx, STATE_FORWARD_LEFT_2);
            }
        }
        break;

    /* ==============================================================
     * 左进垄道2前进
     * ============================================================== */
    case STATE_FORWARD_LEFT_2:
        Read_Encoder_cnt();
        CNT = myabs(Encoders.cntC);
        if (CNT < 16000)
        {
            *Base_Speed = 950;
            drive_lateral_left_fwd_pid(*Base_Speed, 6.5f, 95, Wz);
            if (CNT > 11500)
            {
                FSM_TRANSITION(ctx, STATE_BRAKE_LEFT_2);
            }
        }
        break;

    /* ==============================================================
     * 左进垄道2刹车：左侧超声波减速
     * ============================================================== */
    case STATE_BRAKE_LEFT_2:
        *Lateral_Coeff = 1.15f;
        *Lateral_Offset = 50;
        if (UL_Dis.Left > 10)
        {
            Vx1 = -(UL_Dis.Left * (*Lateral_Coeff) - (*Lateral_Offset));
            Vx2 = -(UL_Dis.Left * (*Lateral_Coeff) - (*Lateral_Offset));
            Motor_Analysis(0, 0, Vx1, Vx2, Wz);
            if (UL_Dis.Left <= Brake_Distance + 30)
            {
                FSM_TRANSITION(ctx, STATE_BACKWARD_2);
            }
        }
        break;

    /* ==============================================================
     * 后退2
     * ============================================================== */
    case STATE_BACKWARD_2:
        Usart3_Send(5, 0);
        Read_Encoder_cnt();
        CNT = myabs(Encoders.cntC);
        if (CNT < (Reverse_Distance - 450))
        {
            Motor_Analysis(Reverse_Speed, Reverse_Speed, 0, 0, Wz);
        }
        else
        {
            FSM_TRANSITION(ctx, STATE_SHIFT_RIGHT_3);
            TIM4->CNT = 0;
        }
        break;

    /* ==============================================================
     * 右进垄道3：右横移
     * ============================================================== */
    case STATE_SHIFT_RIGHT_3:
        Motor_Set(0, 0, 0, 0);
        Read_Encoder_cnt();
        if ((UL_Dis.Left <= 600) && (UL_Dis.Left >= 20))
        {
            *Base_Speed = UL_Dis.Left * 4 + 200;
            Vx1 = *Base_Speed;
            Vx2 = *Base_Speed;
            Motor_Analysis(0, 0, Vx1, Vx2, Wz);
            if (UL_Dis.Left >= 300)
            {
                FSM_TRANSITION(ctx, STATE_FORWARD_RIGHT_3);
            }
        }
        break;

    /* ==============================================================
     * 右进垄道3前进
     * ============================================================== */
    case STATE_FORWARD_RIGHT_3:
        Read_Encoder_cnt();
        CNT = myabs(Encoders.cntC);
        if (CNT < 16000)
        {
            *Base_Speed = 950;
            drive_lateral_right_fwd_pid(*Base_Speed, 6.5f, 95, Wz);
            if (CNT > 11500)
            {
                FSM_TRANSITION(ctx, STATE_BRAKE_RIGHT_3);
            }
        }
        break;

    /* ==============================================================
     * 右进垄道3刹车：右侧超声波减速
     * ============================================================== */
    case STATE_BRAKE_RIGHT_3:
        *Lateral_Coeff = 1.2f;
        *Lateral_Offset = 50;
        if (UL_Dis.Right > 10)
        {
            Vx1 = (UL_Dis.Right * (*Lateral_Coeff) - (*Lateral_Offset));
            Vx2 = (UL_Dis.Right * (*Lateral_Coeff) - (*Lateral_Offset));
            Motor_Analysis(0, 0, Vx1, Vx2, Wz);
            if (UL_Dis.Right <= Brake_Distance)
            {
                FSM_TRANSITION(ctx, STATE_BACKWARD_3);
            }
        }
        break;

    /* ==============================================================
     * 后退3
     * ============================================================== */
    case STATE_BACKWARD_3:
        Usart3_Send(5, 1);
        Read_Encoder_cnt();
        CNT = myabs(Encoders.cntC);
        if (CNT < (Reverse_Distance + 210))
        {
            Motor_Analysis(Reverse_Speed, Reverse_Speed, 0, 0, Wz);
        }
        else
        {
            FSM_TRANSITION(ctx, STATE_SHIFT_LEFT_4);
            TIM4->CNT = 0;
        }
        break;

    /* ==============================================================
     * 左进垄道4：左横移
     * ============================================================== */
    case STATE_SHIFT_LEFT_4:
        Motor_Set(0, 0, 0, 0);
        Read_Encoder_cnt();
        if ((UL_Dis.Right <= 450) && (UL_Dis.Right >= 20))
        {
            *Base_Speed = UL_Dis.Right * 4 + 150;
            Vx1 = -(*Base_Speed);
            Vx2 = -(*Base_Speed);
            Motor_Analysis(0, 0, Vx1, Vx2, Wz);
            if (UL_Dis.Right >= 300)
            {
                FSM_TRANSITION(ctx, STATE_FORWARD_LEFT_4);
            }
        }
        break;

    /* ==============================================================
     * 左进垄道4前进
     * ============================================================== */
    case STATE_FORWARD_LEFT_4:
        Read_Encoder_cnt();
        CNT = myabs(Encoders.cntC);
        if (CNT < 16000)
        {
            *Base_Speed = 950;
            drive_lateral_left_fwd_pid(*Base_Speed, 6.5f, 95, Wz);
            if (CNT > 11500)
            {  
                FSM_TRANSITION(ctx, STATE_BRAKE_LEFT_4);
            }
        }
        break;

    /* ==============================================================
     * 左进垄道4刹车：左侧超声波减速
     * ============================================================== */
    case STATE_BRAKE_LEFT_4:
        *Lateral_Coeff = 1.2f;
        *Lateral_Offset = 50;
        if (UL_Dis.Left > 10)
        {
            Vx1 = -(UL_Dis.Left * (*Lateral_Coeff) - (*Lateral_Offset));
            Vx2 = -(UL_Dis.Left * (*Lateral_Coeff) - (*Lateral_Offset));
            Motor_Analysis(0, 0, Vx1, Vx2, Wz);
            if (UL_Dis.Left <= Brake_Distance)
            {
                FSM_TRANSITION(ctx, STATE_BACKWARD_4);
            }
        }
        break;

    /* ==============================================================
     * 后退4
     * ============================================================== */
    case STATE_BACKWARD_4:
        Usart3_Send(5, 0);
        Read_Encoder_cnt();
        CNT = myabs(Encoders.cntC);
        if (CNT < (Reverse_Distance - 200))
        {
            Motor_Analysis(Reverse_Speed, Reverse_Speed, 0, 0, Wz);
        }
        else
        {
            FSM_TRANSITION(ctx, STATE_SHIFT_RIGHT_5);
            TIM4->CNT = 0;
        }
        break;

    /* ==============================================================
     * 右进垄道5：右横移
     * ============================================================== */
    case STATE_SHIFT_RIGHT_5:
        Motor_Set(0, 0, 0, 0);
        Read_Encoder_cnt();
        if ((UL_Dis.Left <= 600) && (UL_Dis.Left >= 20))
        {
            *Base_Speed = UL_Dis.Left * 4 + 150;
            Vx1 = *Base_Speed;
            Vx2 = *Base_Speed;
            Motor_Analysis(0, 0, Vx1, Vx2, Wz);
            if (UL_Dis.Left >= 300)
            {
                FSM_TRANSITION(ctx, STATE_FORWARD_RIGHT_5);
            }
        }
        break;

    /* ==============================================================
     * 右进垄道5前进
     * ============================================================== */
    case STATE_FORWARD_RIGHT_5:
        Read_Encoder_cnt();
        CNT = myabs(Encoders.cntC);
        if (CNT < 16000)
        {
            *Base_Speed = 950;
            drive_lateral_right_fwd_pid(*Base_Speed, 6.5f, 95, Wz);
            if (CNT > 11500)
            {
                FSM_TRANSITION(ctx, STATE_BRAKE_RIGHT_5);
            }
        }
        break;

    /* ==============================================================
     * 右进垄道5刹车：右侧超声波减速
     * ============================================================== */
    case STATE_BRAKE_RIGHT_5:
        *Lateral_Coeff = 1.2f;
        *Lateral_Offset = 50;
        if (UL_Dis.Right > 10)
        {
            Vx1 = (UL_Dis.Right * (*Lateral_Coeff) - (*Lateral_Offset));
            Vx2 = (UL_Dis.Right * (*Lateral_Coeff) - (*Lateral_Offset));
            Motor_Analysis(0, 0, Vx1, Vx2, Wz);
            if (UL_Dis.Right <= Brake_Distance)
            {
                FSM_TRANSITION(ctx, STATE_BACKWARD_5);
            }
        }
        break;

    /* ==============================================================
     * 后退5
     * ============================================================== */
    case STATE_BACKWARD_5:
        Usart3_Send(5, 1);
        Read_Encoder_cnt();
        CNT = myabs(Encoders.cntC);
        if (CNT < (Reverse_Distance - 100))
        {
            Motor_Analysis(Reverse_Speed, Reverse_Speed, 0, 0, Wz);
        }
        else
        {
            FSM_TRANSITION(ctx, STATE_SHIFT_LEFT_6);
            Motor_Set(0, 0, 0, 0);
        }
        break;

    /* ==============================================================
     * 左进垄道6：左横移
     * ============================================================== */
    case STATE_SHIFT_LEFT_6:
        Motor_Set(0, 0, 0, 0);
        Read_Encoder_cnt();
        if ((UL_Dis.Right <= 600) && (UL_Dis.Right >= 20))
        {
            *Base_Speed = UL_Dis.Right * 3 + 150;
            Vx1 = -(*Base_Speed);
            Vx2 = -(*Base_Speed);
            Motor_Analysis(0, 0, Vx1, Vx2, Wz);
            if (UL_Dis.Right >= 300)
            {
                FSM_TRANSITION(ctx, STATE_FORWARD_LEFT_6);
            }
        }
        break;

    /* ==============================================================
     * 左进垄道6前进
     * ============================================================== */
    case STATE_FORWARD_LEFT_6:
        Read_Encoder_cnt();
        CNT = myabs(Encoders.cntC);
        if (CNT < 16000)
        {
            *Base_Speed = 950;
            if (UL_Dis.Forward <= 93)
            {
                Vx1 = -(*Base_Speed) + (6.5f * (95 - UL_Dis.Forward));
                Vx2 = -(*Base_Speed);
            }
            else if (UL_Dis.Forward >= 96)
            {
                Vx1 = -(*Base_Speed);
                Vx2 = -(*Base_Speed) - (6.5f * (95 - UL_Dis.Forward));
            }
            else
            {
                Vx1 = -(*Base_Speed);
                Vx2 = -(*Base_Speed);
            }
            Motor_Analysis(0, 0, Vx1, Vx2, Wz);
            if (CNT > 11500)
            {
                FSM_TRANSITION(ctx, STATE_BRAKE_LEFT_6);
            }
        }
        break;

    /* ==============================================================
     * 左进垄道6刹车：左侧超声波减速
     * ============================================================== */
    case STATE_BRAKE_LEFT_6:
        *Lateral_Coeff = 1.2f;
        *Lateral_Offset = 50;
        if (UL_Dis.Left > 10)
        {
            Vx1 = -(UL_Dis.Left * (*Lateral_Coeff) - (*Lateral_Offset));
            Vx2 = -(UL_Dis.Left * (*Lateral_Coeff) - (*Lateral_Offset));
            Motor_Analysis(0, 0, Vx1, Vx2, Wz);
            if (UL_Dis.Left <= (Brake_Distance + 50))
            {
                FSM_TRANSITION(ctx, STATE_RETURN_HOME);
            }
        }
        break;

    /* ==============================================================
     * 返航后退
     * ============================================================== */
    case STATE_RETURN_HOME:
        Read_Encoder_cnt();
        CNT = myabs(Encoders.cntC);
        if (CNT < 3400)
        {
            Motor_Analysis(-850, -700, 0, 0, Wz);
        }
        else
        {
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
    }
}
