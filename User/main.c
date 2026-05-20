#include "sys.h"

u8 key;
static int KEY = 0;

static int flag1 = 1;
static int flag2 = 1;
static int flag3 = 1;
static int flag4 = 1;
static int flag5 = 1;
static int flag6 = 1;
static int flag7 = 1;
static int flag8 = 1;
static int flag9 = 1;
static int flag10 = 1;
static int flag11 = 1;
static int flag12 = 1;
static int flag13 = 1;
static int flag14 = 1;
static int flag15 = 1;
static int flag16 = 1;
static int flag17 = 1;
static int flag18 = 1;
static int flag19 = 1;
static int flag20 = 1;
static int flag21 = 1;
static int flag22 = 1;
static int flag23 = 1;
static int flag24 = 1;
static int flag25 = 1;
static int flag26 = 1;

int C1 = 900;

float A;         // 右进垄道——刹车系数
float a;         // 右进垄道——刹车系数
float B;         // 直进垄道——刹车系数
float b;         // 直进垄道——刹车系数
int back = -600; // 后退车速
int BACK = 4580; // 后退垄道距离
int stop = 220;  // 左右超声波刹车距离
short gx;
short gy;
short gz;

float AngFlag;
float pitch;
float roll;
float yaw;

// static float Vy1;//Vy为正，前进；Vy为负，后退
// static float Vy2;//Vy为正，前进；Vy为负，后退
static float Vx1; // Vx设正，1、4轮子为负，2、3轮子为正，小车左移；Vx设负，1、4轮子为正，2、3轮子为负，小车右移。
static float Vx2; // Vx设正，1、4轮子为负，2、3轮子为正，小车左移；Vx设负，1、4轮子为正，2、3轮子为负，小车右移。
static float Wz;  // 角度

int main(void)
{
    delay_init(168);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Usart_init(); // 可修改预编译选择串口
    PID_Init();
    KEY_Init();
    OLED_Init();
    MPU9250_Init();
    mpu_dmp_init();
    Motor_Init();
    Encoder_Init();
    Ultrasonic_Init();
    OLED_Show3FNum(22, 12, UL_Dis.Forward, 5, 3, 12);
    OLED_Refresh_Gram();
    while (1)
    {
        key = KEY_Scan(0);
        if (key == KEY2_PRES)
        {
            KEY = 1;
        }
        if (key == KEY3_PRES)
        {
            BACK = BACK + 100;
        }
        if (key == KEY4_PRES)
        {
            BACK = BACK - 100;
        }
        if (key == KEY5_PRES)
        {
            stop = stop + 20;
        }
        if (key == KEY6_PRES)
        {
            stop = stop - 20;
        }
        mpu_dmp_get_data(&pitch, &roll, &yaw);
        Wz = ControlPID1(yaw);
        OLED_Show3FNum(0, 52, BACK, 4, 1, 12);
        OLED_Show3FNum(62, 52, stop, 4, 1, 12);
        OLED_Show3FNum(22, 0, yaw, 3, 3, 12);
        OLED_Show3FNum(22, 12, UL_Dis.Forward, 5, 3, 12);
        OLED_Show3FNum(0, 22, UL_Dis.Left, 3, 3, 12);
        OLED_Show3FNum(60, 22, UL_Dis.Right, 4, 3, 12);

        OLED_ShowNum(0, 32, flag1, 1, 12);
        OLED_ShowNum(5, 32, flag2, 1, 12);
        OLED_ShowNum(10, 32, flag3, 1, 12);
        OLED_ShowNum(15, 32, flag4, 1, 12);
        OLED_ShowNum(20, 32, flag5, 1, 12);

        OLED_ShowNum(30, 32, flag6, 1, 12);
        OLED_ShowNum(35, 32, flag7, 1, 12);
        OLED_ShowNum(40, 32, flag8, 1, 12);
        OLED_ShowNum(45, 32, flag9, 1, 12);
        OLED_ShowNum(50, 32, flag10, 1, 12);

        OLED_ShowNum(60, 32, flag11, 1, 12);
        OLED_ShowNum(65, 32, flag12, 1, 12);
        OLED_ShowNum(70, 32, flag13, 1, 12);
        OLED_ShowNum(75, 32, flag14, 1, 12);
        OLED_ShowNum(80, 32, flag15, 1, 12);

        OLED_ShowNum(90, 32, flag16, 1, 12);
        OLED_ShowNum(95, 32, flag17, 1, 12);
        OLED_ShowNum(100, 32, flag18, 1, 12);
        OLED_ShowNum(105, 32, flag19, 1, 12);
        OLED_ShowNum(110, 32, flag20, 1, 12);

        OLED_ShowNum(0, 42, flag21, 1, 12);
        OLED_ShowNum(5, 42, flag22, 1, 12);
        OLED_ShowNum(10, 42, flag23, 1, 12);
        OLED_ShowNum(15, 42, flag24, 1, 12);
        OLED_ShowNum(20, 42, flag25, 1, 12);

        OLED_ShowNum(30, 42, flag26, 1, 12);
        OLED_Refresh_Gram();
        Read_Encoder_cnt();
        Ultrasonic_Get_Distance();
        /*
        *********************************************************************************************************************************
        *                                              短进直垄道
        *********************************************************************************************************************************
        */
        if (KEY == 1)
        {
            if ((myabs(Encoders.cntC) < 4000) && (flag1 == 1))
            {
                Vx1 = (myabs(Encoders.cntC) / 5 + 300);
                Vx2 = (myabs(Encoders.cntC) / 5 + 300);
                mpu_dmp_get_data(&pitch, &roll, &yaw);
                Wz = ControlPID1(yaw);
                Motor_Analysis(Vx1, Vx2, 0, 0, Wz);
                if (myabs(Encoders.cntC) > 3000)
                {
                    flag1 = 0;
                }
            }
            /*
            *********************************************************************************************************************************
            *                                              长进直垄道
            *********************************************************************************************************************************
            */
            else
            {
                if ((myabs(Encoders.cntC) < 26000) && (flag1 == 0) && (flag2 == 1))
                {
                    mpu_dmp_get_data(&pitch, &roll, &yaw);
                    Wz = ControlPID1(yaw);
                    if (UL_Dis.Left < 72)
                    {
                        Vx1 = C1 - (5 * (74 - UL_Dis.Left));
                        Vx2 = C1;
                    }
                    else if (UL_Dis.Left > 76)
                    {
                        Vx1 = C1;
                        Vx2 = C1 + (5 * (74 - UL_Dis.Left));
                    }
                    else
                    {
                        Vx1 = C1;
                        Vx2 = C1;
                    }
                    Motor_Analysis(Vx1, Vx2, 0, 0, Wz);
                    if (myabs(Encoders.cntC) > 22000)
                    {
                        flag2 = 0;
                        // Motor_Set(0,0,0,0);
                    }
                }
                /*
                *********************************************************************************************************************************
                *                                              直进垄道刹车
                *********************************************************************************************************************************
                */
                else
                {
                    if ((flag2 == 0) && (flag3 == 1))
                    {
                        C1 = 900;
                        mpu_dmp_get_data(&pitch, &roll, &yaw);
                        B = 1.2;
                        b = 50;
                        if (UL_Dis.Forward > 10)
                        {
                            C1 = (UL_Dis.Forward * B - b);
                            C1 = (UL_Dis.Forward * B - b);
                            if (UL_Dis.Left < 72)
                            {
                                Vx1 = C1 - (3 * (74 - UL_Dis.Left));
                                Vx2 = C1;
                            }
                            else if (UL_Dis.Left > 76)
                            {
                                Vx1 = C1;
                                Vx2 = C1 + (3 * (74 - UL_Dis.Left));
                            }
                            Motor_Analysis(Vx1, Vx2, 0, 0, Wz);
                        }
                        if (UL_Dis.Forward < 260)
                        {
                            flag3 = 0;
                            TIM4->CNT = 0;
                            Motor_Set(0, 0, 0, 0);
                        }
                    }
                    /*
                    *********************************************************************************************************************************
                    *                                              右进垄道1
                    *********************************************************************************************************************************
                    */
                    else
                    {
                        Motor_Set(0, 0, 0, 0);
                        Read_Encoder_cnt();
                        OLED_Show3FNum(42, 42, Encoders.cntC, 5, 3, 12);
                        OLED_Refresh_Gram();
                        if ((myabs(Encoders.cntC) < 16000) && (flag4 == 1) && (flag3 == 0))
                        {
                            if (myabs(Encoders.cntC) < 1500)
                            {
                                C1 = myabs(Encoders.cntC) / 1.2 + 200;
                            }
                            else
                            {
                                C1 = 950;
                            }
                            if (UL_Dis.Forward <= 93)
                            {
                                Vx1 = C1;
                                Vx2 = C1 - (5 * (95 - UL_Dis.Forward));
                            }
                            else if (UL_Dis.Forward >= 97)
                            {
                                Vx1 = C1 + (5 * (95 - UL_Dis.Forward));
                                Vx2 = C1;
                            }
                            else
                            {
                                Vx1 = C1;
                                Vx2 = C1;
                            }
                            Motor_Analysis(0, 0, Vx1, Vx2, Wz);
                            if (myabs(Encoders.cntC) > 15000)
                            {
                                flag4 = 0;
                            }
                        }
                        else
                        {
                            A = 1.15;
                            a = 50;
                            if ((flag4 == 0) && (UL_Dis.Right > 10) && (flag5 == 1))
                            {
                                if (UL_Dis.Forward <= 93)
                                {
                                    Vx1 = (UL_Dis.Right * A - a);
                                    Vx2 = (UL_Dis.Right * A - a) - (4 * (95 - UL_Dis.Forward));
                                }
                                else if (UL_Dis.Forward >= 97)
                                {
                                    Vx1 = (UL_Dis.Right * A - a) + (4 * (95 - UL_Dis.Forward));
                                    Vx2 = (UL_Dis.Right * A - a);
                                }
                                else
                                {
                                    Vx1 = (UL_Dis.Right * A - a);
                                    Vx2 = (UL_Dis.Right * A - a);
                                }
                                Motor_Analysis(0, 0, Vx1, Vx2, Wz);
                                if (UL_Dis.Right <= (stop + 10))
                                {
                                    flag5 = 0;
                                    TIM4->CNT = 0;
                                }
                            }
                            else
                            {
                                while ((flag6 == 1) && (flag5 == 0))
                                {
                                    Usart3_Send(5, 1);
                                    Read_Encoder_cnt();
                                    if ((myabs(Encoders.cntC) < (BACK + 150)) && (flag5 == 0))
                                    {
                                        mpu_dmp_get_data(&pitch, &roll, &yaw);
                                        Wz = ControlPID1(yaw);
                                        Motor_Analysis(back, back, 0, 0, Wz);
                                    }
                                    else
                                    {
                                        flag6 = 0;
                                        TIM4->CNT = 0;
                                        Motor_Set(0, 0, 0, 0);
                                    }
                                }
                                /*
                                *********************************************************************************************************************************
                                *                                              左进垄道2
                                *********************************************************************************************************************************
                                */

                                if ((UL_Dis.Right <= 600) && (UL_Dis.Right >= 20) && (flag6 == 0) && (flag7 == 1))
                                {
                                    Motor_Set(0, 0, 0, 0);
                                    Read_Encoder_cnt();
                                    C1 = UL_Dis.Right * 3 + 150;
                                    Vx1 = -C1;
                                    Vx2 = -C1;
                                    Motor_Analysis(0, 0, Vx1, Vx2, Wz);
                                    if (UL_Dis.Right >= 300)
                                    {
                                        flag7 = 0;
                                        TIM4->CNT = 0;
                                    }
                                }
                                else
                                {
                                    Read_Encoder_cnt();
                                    OLED_Show3FNum(22, 42, Encoders.cntC, 5, 3, 12);
                                    OLED_Refresh_Gram();
                                    if ((myabs(Encoders.cntC) < 16000) && (flag8 == 1) && (flag7 == 0))
                                    {
                                        C1 = 950;
                                        if (UL_Dis.Forward <= 93)
                                        {
                                            Vx1 = -C1 + (6.5 * (95 - UL_Dis.Forward));
                                            Vx2 = -C1;
                                        }
                                        else if (UL_Dis.Forward >= 97)
                                        {
                                            Vx1 = -C1;
                                            Vx2 = -C1 - (6.5 * (95 - UL_Dis.Forward));
                                        }
                                        else
                                        {
                                            Vx1 = -C1;
                                            Vx2 = -C1;
                                        }
                                        Motor_Analysis(0, 0, Vx1, Vx2, Wz);
                                        if (myabs(Encoders.cntC) > 11500)
                                        {
                                            flag8 = 0;
                                        }
                                    }
                                    else
                                    {
                                        A = 1.15;
                                        a = 50;
                                        if ((flag8 == 0) && (UL_Dis.Left > 10) && (flag9 == 1))
                                        {
                                            Vx1 = -(UL_Dis.Left * A - a);
                                            Vx2 = -(UL_Dis.Left * A - a);
                                            Motor_Analysis(0, 0, Vx1, Vx2, Wz);
                                            if (UL_Dis.Left <= stop + 30)
                                            {
                                                flag9 = 0;
                                                TIM4->CNT = 0;
                                            }
                                        }
                                        else
                                        {
                                            while ((flag10 == 1) && (flag9 == 0))
                                            {
                                                Usart3_Send(5, 0);
                                                Read_Encoder_cnt();
                                                if ((myabs(Encoders.cntC) < (BACK - 450)) && (flag9 == 0))
                                                {
                                                    mpu_dmp_get_data(&pitch, &roll, &yaw);
                                                    Wz = ControlPID1(yaw);
                                                    Motor_Analysis(back, back, 0, 0, Wz);
                                                }
                                                else
                                                {
                                                    flag10 = 0;
                                                    TIM4->CNT = 0;
                                                }
                                            }
                                            /*
                                            *********************************************************************************************************************************
                                            *                                              右进垄道3
                                            *********************************************************************************************************************************
                                            */
                                            if ((UL_Dis.Left <= 600) && (UL_Dis.Left >= 20) && (flag10 == 0) && (flag11 == 1))
                                            {
                                                Motor_Set(0, 0, 0, 0);
                                                Read_Encoder_cnt();
                                                C1 = UL_Dis.Left * 4 + 200;
                                                Vx1 = C1;
                                                Vx2 = C1;
                                                Motor_Analysis(0, 0, Vx1, Vx2, Wz);
                                                if (UL_Dis.Left >= 300)
                                                {
                                                    flag11 = 0;
                                                    TIM4->CNT = 0;
                                                }
                                            }
                                            else
                                            {
                                                Read_Encoder_cnt();
                                                OLED_Show3FNum(22, 42, Encoders.cntC, 5, 3, 12);
                                                OLED_Refresh_Gram();
                                                if ((myabs(Encoders.cntC) < 16000) && (flag12 == 1) && (flag11 == 0))
                                                {
                                                    C1 = 950;
                                                    if (UL_Dis.Forward <= 93)
                                                    {
                                                        Vx1 = C1;
                                                        Vx2 = C1 - (6.5 * (95 - UL_Dis.Forward));
                                                    }
                                                    else if (UL_Dis.Forward >= 97)
                                                    {
                                                        Vx1 = C1 + (6.5 * (95 - UL_Dis.Forward));
                                                        Vx2 = C1;
                                                    }
                                                    else
                                                    {
                                                        Vx1 = C1;
                                                        Vx2 = C1;
                                                    }
                                                    Motor_Analysis(0, 0, Vx1, Vx2, Wz);
                                                    if (myabs(Encoders.cntC) > 11500)
                                                    {
                                                        flag12 = 0;
                                                    }
                                                }
                                                else
                                                {
                                                    A = 1.2;
                                                    a = 50;
                                                    if ((flag12 == 0) && (UL_Dis.Right > 10) && (flag13 == 1))
                                                    {
                                                        Vx1 = (UL_Dis.Right * A - a);
                                                        Vx2 = (UL_Dis.Right * A - a);
                                                        Motor_Analysis(0, 0, Vx1, Vx2, Wz);
                                                        if (UL_Dis.Right <= stop)
                                                        {
                                                            flag13 = 0;
                                                            TIM4->CNT = 0;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        while ((flag13 == 0) && (flag14 == 1))
                                                        {
                                                            Usart3_Send(5, 1);
                                                            Read_Encoder_cnt();
                                                            if ((myabs(Encoders.cntC) < (BACK + 210)) && (flag13 == 0))
                                                            {
                                                                mpu_dmp_get_data(&pitch, &roll, &yaw);
                                                                Wz = ControlPID1(yaw);
                                                                Motor_Analysis(back, back, 0, 0, Wz);
                                                            }
                                                            else
                                                            {
                                                                flag14 = 0;
                                                                TIM4->CNT = 0;
                                                            }
                                                        }
                                                        /*
                                                        *********************************************************************************************************************************
                                                        *                                              左进垄道4
                                                        *********************************************************************************************************************************
                                                        */
                                                        if ((UL_Dis.Right <= 450) && (UL_Dis.Right >= 20) && (flag14 == 0) && (flag15 == 1))
                                                        {
                                                            Motor_Set(0, 0, 0, 0);
                                                            Read_Encoder_cnt();
                                                            C1 = UL_Dis.Right * 4 + 150;
                                                            Vx1 = -C1;
                                                            Vx2 = -C1;
                                                            Motor_Analysis(0, 0, Vx1, Vx2, Wz);
                                                            if (UL_Dis.Right >= 300)
                                                            {
                                                                flag15 = 0;
                                                                TIM4->CNT = 0;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            Read_Encoder_cnt();
                                                            OLED_Show3FNum(22, 42, Encoders.cntC, 5, 3, 12);
                                                            OLED_Refresh_Gram();
                                                            if ((myabs(Encoders.cntC) < 16000) && (flag16 == 1) && (flag15 == 0))
                                                            {
                                                                C1 = 950;
                                                                if (UL_Dis.Forward <= 93)
                                                                {
                                                                    Vx1 = -C1 + (6.5 * (95 - UL_Dis.Forward));
                                                                    Vx2 = -C1;
                                                                }
                                                                else if (UL_Dis.Forward >= 97)
                                                                {
                                                                    Vx1 = -C1;
                                                                    Vx2 = -C1 - (6.5 * (95 - UL_Dis.Forward));
                                                                }
                                                                else
                                                                {
                                                                    Vx1 = -C1;
                                                                    Vx2 = -C1;
                                                                }
                                                                Motor_Analysis(0, 0, Vx1, Vx2, Wz);
                                                                if (myabs(Encoders.cntC) > 11500)
                                                                {
                                                                    flag16 = 0;
                                                                }
                                                            }
                                                            else
                                                            {
                                                                A = 1.2;
                                                                a = 50;
                                                                if ((flag16 == 0) && (UL_Dis.Left > 10) && (flag17 == 1))
                                                                {
                                                                    Vx1 = -(UL_Dis.Left * A - a);
                                                                    Vx2 = -(UL_Dis.Left * A - a);
                                                                    Motor_Analysis(0, 0, Vx1, Vx2, Wz);
                                                                    if (UL_Dis.Left <= stop)
                                                                    {
                                                                        flag17 = 0;
                                                                        TIM4->CNT = 0;
                                                                    }
                                                                }
                                                                else
                                                                {
                                                                    while ((flag18 == 1) && (flag17 == 0))
                                                                    {
                                                                        Usart3_Send(5, 0);
                                                                        Read_Encoder_cnt();
                                                                        if ((myabs(Encoders.cntC) < (BACK - 200)) && (flag17 == 0))
                                                                        {
                                                                            mpu_dmp_get_data(&pitch, &roll, &yaw);
                                                                            Wz = ControlPID1(yaw);
                                                                            Motor_Analysis(back, back, 0, 0, Wz);
                                                                        }
                                                                        else
                                                                        {
                                                                            flag18 = 0;
                                                                            TIM4->CNT = 0;
                                                                        }
                                                                    }
                                                                    /*
                                                                    *********************************************************************************************************************************
                                                                    *                                          右进垄道5
                                                                    *********************************************************************************************************************************
                                                                    */
                                                                    if ((UL_Dis.Left <= 600) && (UL_Dis.Left >= 20) && (flag18 == 0) && (flag19 == 1))
                                                                    {
                                                                        Motor_Set(0, 0, 0, 0);
                                                                        Read_Encoder_cnt();
                                                                        C1 = UL_Dis.Left * 4 + 150;
                                                                        Vx1 = C1;
                                                                        Vx2 = C1;
                                                                        Motor_Analysis(0, 0, Vx1, Vx2, Wz);
                                                                        if (UL_Dis.Left >= 300)
                                                                        {
                                                                            flag19 = 0;
                                                                            TIM4->CNT = 0;
                                                                        }
                                                                    }
                                                                    else
                                                                    {
                                                                        Read_Encoder_cnt();
                                                                        OLED_Show3FNum(22, 42, Encoders.cntC, 5, 3, 12);
                                                                        OLED_Refresh_Gram();
                                                                        if ((myabs(Encoders.cntC) < 16000) && (flag20 == 1) && (flag19 == 0))
                                                                        {
                                                                            C1 = 950;
                                                                            if (UL_Dis.Forward <= 93)
                                                                            {
                                                                                Vx1 = C1;
                                                                                Vx2 = C1 - (6.5 * (95 - UL_Dis.Forward));
                                                                            }
                                                                            else if (UL_Dis.Forward >= 97)
                                                                            {
                                                                                Vx1 = C1 + (6.5 * (95 - UL_Dis.Forward));
                                                                                Vx2 = C1;
                                                                            }
                                                                            else
                                                                            {
                                                                                Vx1 = C1;
                                                                                Vx2 = C1;
                                                                            }
                                                                            Motor_Analysis(0, 0, Vx1, Vx2, Wz);
                                                                            if (myabs(Encoders.cntC) > 11500)
                                                                            {
                                                                                flag20 = 0;
                                                                            }
                                                                        }
                                                                        else
                                                                        {
                                                                            A = 1.2;
                                                                            a = 50;
                                                                            if ((flag20 == 0) && (UL_Dis.Right > 10) && (flag21 == 1))
                                                                            {
                                                                                Vx1 = (UL_Dis.Right * A - a);
                                                                                Vx2 = (UL_Dis.Right * A - a);
                                                                                Motor_Analysis(0, 0, Vx1, Vx2, Wz);
                                                                                if (UL_Dis.Right <= stop)
                                                                                {
                                                                                    flag21 = 0;
                                                                                    TIM4->CNT = 0;
                                                                                }
                                                                            }
                                                                            else
                                                                            {

                                                                                while ((flag21 == 0) && (flag22 == 1))
                                                                                {
                                                                                    Usart3_Send(5, 1);
                                                                                    Read_Encoder_cnt();
                                                                                    if ((myabs(Encoders.cntC) < (BACK - 100)) && (flag21 == 0))
                                                                                    {
                                                                                        mpu_dmp_get_data(&pitch, &roll, &yaw);
                                                                                        Wz = ControlPID1(yaw);
                                                                                        Motor_Analysis(back, back, 0, 0, Wz);
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        flag22 = 0;
                                                                                        TIM4->CNT = 0;
                                                                                        Motor_Set(0, 0, 0, 0);
                                                                                    }
                                                                                }
                                                                                /*
                                                                                *********************************************************************************************************************************
                                                                                *                                              左进垄道6
                                                                                *********************************************************************************************************************************
                                                                                */
                                                                                if ((UL_Dis.Right <= 600) && (UL_Dis.Right >= 20) && (flag22 == 0) && (flag23 == 1))
                                                                                {
                                                                                    Motor_Set(0, 0, 0, 0);
                                                                                    Read_Encoder_cnt();
                                                                                    C1 = UL_Dis.Right * 3 + 150;
                                                                                    Vx1 = -C1;
                                                                                    Vx2 = -C1;
                                                                                    Motor_Analysis(0, 0, Vx1, Vx2, Wz);
                                                                                    if (UL_Dis.Right >= 300)
                                                                                    {
                                                                                        flag23 = 0;
                                                                                        TIM4->CNT = 0;
                                                                                    }
                                                                                }
                                                                                else
                                                                                {
                                                                                    Read_Encoder_cnt();
                                                                                    OLED_Show3FNum(22, 42, Encoders.cntC, 5, 3, 12);
                                                                                    OLED_Refresh_Gram();
                                                                                    if ((myabs(Encoders.cntC) < 16000) && (flag24 == 1) && (flag23 == 0))
                                                                                    {
                                                                                        C1 = 950;
                                                                                        if (UL_Dis.Forward <= 93)
                                                                                        {
                                                                                            Vx1 = -C1 + (6.5 * (95 - UL_Dis.Forward));
                                                                                            Vx2 = -C1;
                                                                                        }
                                                                                        else if (UL_Dis.Forward >= 96)
                                                                                        {
                                                                                            Vx1 = -C1;
                                                                                            Vx2 = -C1 - (6.5 * (95 - UL_Dis.Forward));
                                                                                        }
                                                                                        else
                                                                                        {
                                                                                            Vx1 = -C1;
                                                                                            Vx2 = -C1;
                                                                                        }
                                                                                        Motor_Analysis(0, 0, Vx1, Vx2, Wz);
                                                                                        if (myabs(Encoders.cntC) > 11500)
                                                                                        {
                                                                                            flag24 = 0;
                                                                                        }
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        A = 1.2;
                                                                                        a = 50;
                                                                                        if ((flag24 == 0) && (UL_Dis.Left > 10) && (flag25 == 1))
                                                                                        {
                                                                                            Vx1 = -(UL_Dis.Left * A - a);
                                                                                            Vx2 = -(UL_Dis.Left * A - a);
                                                                                            Motor_Analysis(0, 0, Vx1, Vx2, Wz);
                                                                                            if (UL_Dis.Left <= (stop + 50))
                                                                                            {
                                                                                                flag25 = 0;
                                                                                                TIM4->CNT = 0;
                                                                                            }
                                                                                        }
                                                                                        else
                                                                                        {
                                                                                            while ((flag26 == 1) && (flag25 == 0))
                                                                                            {
                                                                                                Read_Encoder_cnt();
                                                                                                if ((myabs(Encoders.cntC) < 3400) && (flag25 == 0))
                                                                                                {
                                                                                                    mpu_dmp_get_data(&pitch, &roll, &yaw);
                                                                                                    Wz = ControlPID1(yaw);
                                                                                                    Motor_Analysis(-850, -700, 0, 0, Wz);
                                                                                                }
                                                                                                else
                                                                                                {
                                                                                                    flag26 = 0;
                                                                                                    Motor_Set(0, 0, 0, 0);
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
