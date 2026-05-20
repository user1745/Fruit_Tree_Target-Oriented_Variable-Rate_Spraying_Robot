#include "sys.h"

int main(void)
{

    u8 KEY = 0;

    int Reverse_Speed = -600;    // 后退车速
    int Reverse_Distance = 4580; // 后退距离
    int Brake_Distance = 220;    // 左右超声波刹车距离

    float pitch = 0.0f;
    float roll = 0.0f;
    float yaw = 0.0f;

    float Wz = 0.0f; // 航向角 PID 输出

    FSM_Context_t g_fsm; // 有限状态机上下文

    Delay_init(168);
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
    FSM_Init(&g_fsm); // 硬件就绪后再初始化有限状态机

    while (1)
    {
        if (KEY == 0) // 按下一次后就不再检测
        {
            switch (KEY_Scan(0))
            {
            case KEY2_PRES:
                KEY = 1;
                break;

            case KEY3_PRES:
                Reverse_Distance = Reverse_Distance + 100;
                break;

            case KEY4_PRES:
                Reverse_Distance = Reverse_Distance - 100;
                break;

            case KEY5_PRES:
                Brake_Distance = Brake_Distance + 20;
                break;

            case KEY6_PRES:
                Brake_Distance = Brake_Distance - 20;
                break;

            default:
                break;
            }
        }

        if (mpu_dmp_get_data(&pitch, &roll, &yaw) == 0)
        {
            Wz = ControlPID1(yaw);
        }
        Read_Encoder_cnt();
        Ultrasonic_Get_Distance();

        OLED_Show3FNum(0, 52, Reverse_Distance, 4, 1, 12);
        OLED_Show3FNum(62, 52, Brake_Distance, 4, 1, 12);

        OLED_Show3FNum(22, 0, yaw, 3, 3, 12);

        OLED_Show3FNum(22, 12, UL_Dis.Forward, 5, 3, 12);
        OLED_Show3FNum(0, 22, UL_Dis.Left, 3, 3, 12);
        OLED_Show3FNum(60, 22, UL_Dis.Right, 4, 3, 12);

        OLED_ShowNum(0, 32, (int)g_fsm.current, 2, 12); // 显示当前FSM状态编号

        OLED_Refresh_Gram();

        /*
        *********************************************************************************************************************************
        *                                              FSM 状态机调度
        *********************************************************************************************************************************
        */
        if (KEY == 1)
        {
            FSM_Update(&g_fsm, Reverse_Speed, Reverse_Distance, Brake_Distance, Wz);
        }
    }
}
