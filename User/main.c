#include "sys.h"

static int KEY = 0;

static FSM_Context_t g_fsm; // 有限状态机上下文

int Base_Speed = 900;

float Lateral_Coeff;         // 横移刹车系数
float Lateral_Offset;        // 横移刹车偏置
float Forward_Coeff;         // 直进刹车系数
float Forward_Offset;        // 直进刹车偏置
int Reverse_Speed = -600;    // 后退车速
int Reverse_Distance = 4580; // 后退距离
int Brake_Distance = 220;    // 左右超声波刹车距离
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

    FSM_Init(&g_fsm); // 初始化有限状态机

    while (1)
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

        mpu_dmp_get_data(&pitch, &roll, &yaw);
        Wz = ControlPID1(yaw);
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
            FSM_Update(&g_fsm, &Base_Speed, &Lateral_Coeff, &Lateral_Offset, &Forward_Coeff, &Forward_Offset, Reverse_Speed, Reverse_Distance, Brake_Distance, Wz, &Vx1, &Vx2);
        }
    }
}
