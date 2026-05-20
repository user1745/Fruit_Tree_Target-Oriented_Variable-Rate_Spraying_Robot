#include "Ultrasonic.h"

// 捕获状态
//[7]:0,没有成功的捕获; 1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)

u32 CH1_CAP_STA = 0; // 输入捕获状态
u32 CH1_CAP_VAL = 0; // 输入捕获值(TIM2/TIM5是32位)

u32 CH2_CAP_STA = 0; // 输入捕获状态
u32 CH2_CAP_VAL = 0; // 输入捕获值(TIM2/TIM5是32位)

u32 CH3_CAP_STA = 0; // 输入捕获状态
u32 CH3_CAP_VAL = 0; // 输入捕获值(TIM2/TIM5是32位)

u32 CH4_CAP_STA = 0; // 输入捕获状态
u32 CH4_CAP_VAL = 0; // 输入捕获值(TIM2/TIM5是32位)

u32 temp1 = 0;
u32 temp2 = 0;
u32 temp3 = 0;
u32 temp4 = 0;

float TEMP1 = 0;
float TEMP2 = 0;
float TEMP3 = 0;
float TEMP4 = 0;

// 存放距离的结构体
UltrasonicDis UL_Dis = {0, 0};

// 滤波的结构体
UltrasonicFilter_Struct Ultrasonic1_Filter = {0, 0, 0, 0, {0}};
UltrasonicFilter_Struct Ultrasonic2_Filter = {0, 0, 0, 0, {0}};
UltrasonicFilter_Struct Ultrasonic3_Filter = {0, 0, 0, 0, {0}};
UltrasonicFilter_Struct Ultrasonic4_Filter = {0, 0, 0, 0, {0}};

void Ultrasonic_Init(void)
{
    Ultrasonic_IO_Init();
    TIM3_Cap_Init(0XFFFF, 84 - 1);
}

/*
********************************************************************************************************************
*                  void Ultra_Init(void)
*
*Description : 超声波初始化
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void Ultrasonic_IO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13; // TRIG(PE12.13)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;            // 设为输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           // 设为推挽模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOE, GPIO_Pin_12 | GPIO_Pin_13);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15; // TRIG(PE14.15)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;            // 设为输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           // 设为推挽模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOE, GPIO_Pin_14 | GPIO_Pin_15);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // ECHO(PA6.7)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           // 模式设为复用功能
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;         // 设为下拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIOA
    GPIO_ResetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_7);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; // ECHO(PB0.1)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           // 模式设为复用功能
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;         // 设为下拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); // 初始化GPIOA
    GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3); // PA6复用为定时器3
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3); // PA7复用为定时器3
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3); // PA6复用为定时器3
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3); // PA7复用为定时器3
}

/*
********************************************************************************************************************
*                  void TIM3_Cap_Init(u32 arr,u16 psc)
*
*Description : 定时器5的输入捕获配置
*Arguments   : 溢出时间为：（arr+1）*（psc+1）/84M
*Returns     : none
*Notes       : arr:自动重装值,psc:时钟预分频数
********************************************************************************************************************
*/
void TIM3_Cap_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef TIMx_ICInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIMx_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIMx_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIMx_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIMx_ICInitStructure.TIM_ICFilter = 0x00;

    TIMx_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInit(TIM3, &TIMx_ICInitStructure);

    TIMx_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(TIM3, &TIMx_ICInitStructure);

    TIMx_ICInitStructure.TIM_Channel = TIM_Channel_3;
    TIM_ICInit(TIM3, &TIMx_ICInitStructure);

    TIMx_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInit(TIM3, &TIMx_ICInitStructure);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
    TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
    TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);
    TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        // 子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM3, ENABLE); // 使能定时器                                                                    //根据指定的参数初始化VIC寄存器
}

/*
********************************************************************************************************************
*                  void TIM3_IRQHandler(void)
*
*Description : 定时器3中断服务函数
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void TIM3_IRQHandler(void)
{
    if ((CH1_CAP_STA & 0X80) == 0) // 还未成功捕获
    {
        if (TIM_GetITStatus(CAP_TIMER, TIM_IT_Update) != RESET) // 溢出
        {
            if (CH1_CAP_STA & 0X40) // 已经捕获到高电平了
            {
                if ((CH1_CAP_STA & 0X3F) == 0X3F) // 高电平太长了
                {
                    CH1_CAP_STA |= 0X80; // 标记成功捕获了一次
                    CH1_CAP_VAL = CAP_Overflow;
                }
                else
                    CH1_CAP_STA++;
            }
        }
        if (TIM_GetITStatus(CAP_TIMER, TIM_IT_CC1) != RESET) // 捕获通道3发生捕获事件
        {
            if (CH1_CAP_STA & 0X40) // 捕获到一个下降沿
            {
                CH1_CAP_STA |= 0X80;                                     // 标记成功捕获到一次高电平脉宽
                CH1_CAP_VAL = TIM_GetCapture1(CAP_TIMER);                // 获取当前的捕获值.
                TIM_OC1PolarityConfig(CAP_TIMER, TIM_ICPolarity_Rising); // CC3P=0 设置为上升沿捕获
            }
            else // 还未开始,第一次捕获上升沿
            {
                CH1_CAP_STA = 0; // 清空
                CH1_CAP_VAL = 0;
                CH1_CAP_STA |= 0X40;         // 标记捕获到了上升沿
                TIM_Cmd(CAP_TIMER, DISABLE); // 关闭定时器
                TIM_SetCounter(CAP_TIMER, 0);
                TIM_OC1PolarityConfig(CAP_TIMER, TIM_ICPolarity_Falling); // CC3P=1 设置为下降沿捕获
                TIM_Cmd(CAP_TIMER, ENABLE);                               // 使能定时器
            }
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    if ((CH2_CAP_STA & 0X80) == 0) // 还未成功捕获
    {
        if (TIM_GetITStatus(CAP_TIMER, TIM_IT_Update) != RESET) // 溢出
        {
            if (CH2_CAP_STA & 0X40) // 已经捕获到高电平了
            {
                if ((CH2_CAP_STA & 0X3F) == 0X3F) // 高电平太长了
                {
                    CH2_CAP_STA |= 0X80; // 标记成功捕获了一次
                    CH2_CAP_VAL = CAP_Overflow;
                }
                else
                    CH2_CAP_STA++;
            }
        }
        if (TIM_GetITStatus(CAP_TIMER, TIM_IT_CC2) != RESET) // 捕获通道3发生捕获事件
        {
            if (CH2_CAP_STA & 0X40) // 捕获到一个下降沿
            {
                CH2_CAP_STA |= 0X80;                                     // 标记成功捕获到一次高电平脉宽
                CH2_CAP_VAL = TIM_GetCapture2(CAP_TIMER);                // 获取当前的捕获值.
                TIM_OC2PolarityConfig(CAP_TIMER, TIM_ICPolarity_Rising); // CC3P=0 设置为上升沿捕获
            }
            else // 还未开始,第一次捕获上升沿
            {
                CH2_CAP_STA = 0; // 清空
                CH2_CAP_VAL = 0;
                CH2_CAP_STA |= 0X40;         // 标记捕获到了上升沿
                TIM_Cmd(CAP_TIMER, DISABLE); // 关闭定时器
                TIM_SetCounter(CAP_TIMER, 0);
                TIM_OC2PolarityConfig(CAP_TIMER, TIM_ICPolarity_Falling); // CC3P=1 设置为下降沿捕获
                TIM_Cmd(CAP_TIMER, ENABLE);                               // 使能定时器
            }
        }
    }

    //-----------------------------------------------------------------------------------------------------------------

    if ((CH3_CAP_STA & 0X80) == 0) // 还未成功捕获
    {
        if (TIM_GetITStatus(CAP_TIMER, TIM_IT_Update) != RESET) // 溢出
        {
            if (CH3_CAP_STA & 0X40) // 已经捕获到高电平了
            {
                if ((CH3_CAP_STA & 0X3F) == 0X3F) // 高电平太长了
                {
                    CH3_CAP_STA |= 0X80; // 标记成功捕获了一次
                    CH3_CAP_VAL = CAP_Overflow;
                }
                else
                    CH3_CAP_STA++;
            }
        }
        if (TIM_GetITStatus(CAP_TIMER, TIM_IT_CC3) != RESET) // 捕获通道3发生捕获事件
        {
            if (CH3_CAP_STA & 0X40) // 捕获到一个下降沿
            {
                CH3_CAP_STA |= 0X80;                                     // 标记成功捕获到一次高电平脉宽
                CH3_CAP_VAL = TIM_GetCapture3(CAP_TIMER);                // 获取当前的捕获值.
                TIM_OC3PolarityConfig(CAP_TIMER, TIM_ICPolarity_Rising); // CC3P=0 设置为上升沿捕获
            }
            else // 还未开始,第一次捕获上升沿
            {
                CH3_CAP_STA = 0; // 清空
                CH3_CAP_VAL = 0;
                CH3_CAP_STA |= 0X40;         // 标记捕获到了上升沿
                TIM_Cmd(CAP_TIMER, DISABLE); // 关闭定时器
                TIM_SetCounter(CAP_TIMER, 0);
                TIM_OC3PolarityConfig(CAP_TIMER, TIM_ICPolarity_Falling); // CC3P=1 设置为下降沿捕获
                TIM_Cmd(CAP_TIMER, ENABLE);                               // 使能定时器
            }
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    if ((CH4_CAP_STA & 0X80) == 0) // 还未成功捕获
    {
        if (TIM_GetITStatus(CAP_TIMER, TIM_IT_Update) != RESET) // 溢出
        {
            if (CH4_CAP_STA & 0X40) // 已经捕获到高电平了
            {
                if ((CH4_CAP_STA & 0X3F) == 0X3F) // 高电平太长了
                {
                    CH4_CAP_STA |= 0X80; // 标记成功捕获了一次
                    CH4_CAP_VAL = CAP_Overflow;
                }
                else
                    CH4_CAP_STA++;
            }
        }
        if (TIM_GetITStatus(CAP_TIMER, TIM_IT_CC4) != RESET) // 捕获通道4发生捕获事件
        {
            if (CH4_CAP_STA & 0X40) // 捕获到一个下降沿
            {
                CH4_CAP_STA |= 0X80;                                     // 标记成功捕获到一次高电平脉宽
                CH4_CAP_VAL = TIM_GetCapture4(CAP_TIMER);                // 获取当前的捕获值.
                TIM_OC4PolarityConfig(CAP_TIMER, TIM_ICPolarity_Rising); // CC4P=0 设置为上升沿捕获
            }
            else // 还未开始,第一次捕获上升沿
            {
                CH4_CAP_STA = 0; // 清空
                CH4_CAP_VAL = 0;
                CH4_CAP_STA |= 0X40;         // 标记捕获到了上升沿
                TIM_Cmd(CAP_TIMER, DISABLE); // 关闭定时器
                TIM_SetCounter(CAP_TIMER, 0);
                TIM_OC4PolarityConfig(CAP_TIMER, TIM_ICPolarity_Falling); // CC4P=1 设置为下降沿捕获
                TIM_Cmd(CAP_TIMER, ENABLE);                               // 使能定时器
            }
        }
    }
    TIM_ClearITPendingBit(CAP_TIMER, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_Update); // 清除中断标志位
}

/*
********************************************************************************************************************
*                  void Ultrasonic_Get_Distance(void)
*
*Description : 距离读取函数
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void Ultrasonic_Get_Distance(void)
{
    TRIG1_out TRIG2_out // 四个超声波Trig引脚15us高电平触发
        TRIG3_out TRIG4_out

        if (CH1_CAP_STA & 0X80) // 成功捕获到了一次高电平
    {
        temp1 = CH1_CAP_STA & 0X3F;
        temp1 *= CAP_Overflow; // 溢出时间总和
        temp1 += CH1_CAP_VAL;  // 得到总的高电平时间
        CH1_CAP_STA = 0;       // 开启下一次捕获
    }
    TEMP1 = temp1 * 34 / 200; // 测量距离单位cm
    Ultrasonic_Filter(&Ultrasonic1_Filter, TEMP1, 7, &UL_Dis.Forward);

    if (CH2_CAP_STA & 0X80) // 成功捕获到了一次高电平
    {
        temp2 = CH3_CAP_STA & 0X3F;
        temp2 *= CAP_Overflow; // 溢出时间总和
        temp2 += CH2_CAP_VAL;  // 得到总的高电平时间
        CH2_CAP_STA = 0;       // 开启下一次捕获
    }
    TEMP2 = temp2 * 34 / 200; // 测量距离单位cm
    Ultrasonic_Filter(&Ultrasonic2_Filter, TEMP2, 7, &UL_Dis.Back);

    if (CH3_CAP_STA & 0X80) // 成功捕获到了一次高电平
    {
        temp3 = CH3_CAP_STA & 0X3F;
        temp3 *= CAP_Overflow; // 溢出时间总和
        temp3 += CH3_CAP_VAL;  // 得到总的高电平时间
        CH3_CAP_STA = 0;       // 开启下一次捕获
    }
    TEMP3 = temp3 * 34 / 200; // 测量距离单位cm
    Ultrasonic_Filter(&Ultrasonic3_Filter, TEMP3, 7, &UL_Dis.Right);

    if (CH4_CAP_STA & 0X80) // 成功捕获到了一次高电平
    {
        temp4 = CH4_CAP_STA & 0X3F;
        temp4 *= CAP_Overflow; // 溢出时间总和
        temp4 += CH4_CAP_VAL;  // 得到总的高电平时间
        CH4_CAP_STA = 0;       // 开启下一次捕获
    }
    TEMP4 = temp4 * 34 / 200; // 测量距离单位cm
    Ultrasonic_Filter(&Ultrasonic4_Filter, TEMP4, 7, &UL_Dis.Left);
    if (UL_Dis.Left > 5000)
        UL_Dis.Left = 5000;
    if (UL_Dis.Right > 2100)
        UL_Dis.Right = 2100;
    if (UL_Dis.Forward > 999)
        UL_Dis.Forward = 999;
}

void Ultrasonic_Get_Distance_1(void)
{
    TRIG1_out // 四个超声波Trig引脚15us高电平触发

        if (CH1_CAP_STA & 0X80) // 成功捕获到了一次高电平
    {
        temp1 = CH1_CAP_STA & 0X3F;
        temp1 *= CAP_Overflow; // 溢出时间总和
        temp1 += CH1_CAP_VAL;  // 得到总的高电平时间
        CH1_CAP_STA = 0;       // 开启下一次捕获
    }
    TEMP1 = temp1 * 34 / 200; // 测量距离单位cm
    Ultrasonic_Filter(&Ultrasonic1_Filter, TEMP1, 7, &UL_Dis.Forward);
    if (UL_Dis.Forward > 999)
        UL_Dis.Forward = 999;
}

void Ultrasonic_Get_Distance_2(void)
{
    TRIG2_out // 四个超声波Trig引脚15us高电平触发

        if (CH2_CAP_STA & 0X80) // 成功捕获到了一次高电平
    {
        temp2 = CH3_CAP_STA & 0X3F;
        temp2 *= CAP_Overflow; // 溢出时间总和
        temp2 += CH2_CAP_VAL;  // 得到总的高电平时间
        CH2_CAP_STA = 0;       // 开启下一次捕获
    }
    TEMP2 = temp2 * 34 / 200; // 测量距离单位cm
    Ultrasonic_Filter(&Ultrasonic2_Filter, TEMP2, 7, &UL_Dis.Back);
    if (UL_Dis.Back > 2000)
        UL_Dis.Back = 2000;
}

void Ultrasonic_Get_Distance_4(void)
{
    TRIG4_out // 四个超声波Trig引脚15us高电平触发

        if (CH4_CAP_STA & 0X80) // 成功捕获到了一次高电平
    {
        temp4 = CH4_CAP_STA & 0X3F;
        temp4 *= CAP_Overflow; // 溢出时间总和
        temp4 += CH4_CAP_VAL;  // 得到总的高电平时间
        CH4_CAP_STA = 0;       // 开启下一次捕获
    }
    TEMP4 = temp4 * 34 / 200; // 测量距离单位cm
    Ultrasonic_Filter(&Ultrasonic4_Filter, TEMP4, 7, &UL_Dis.Left);
    if (UL_Dis.Left > 2000)
        UL_Dis.Left = 2000;
}

void Ultrasonic_Get_Distance_3(void)
{
    TRIG3_out // 四个超声波Trig引脚15us高电平触发

        if (CH3_CAP_STA & 0X80) // 成功捕获到了一次高电平
    {
        temp3 = CH3_CAP_STA & 0X3F;
        temp3 *= CAP_Overflow; // 溢出时间总和
        temp3 += CH3_CAP_VAL;  // 得到总的高电平时间
        CH3_CAP_STA = 0;       // 开启下一次捕获
    }
    TEMP3 = temp3 * 34 / 200; // 测量距离单位cm
    Ultrasonic_Filter(&Ultrasonic3_Filter, TEMP3, 7, &UL_Dis.Right);
    if (UL_Dis.Right > 2000)
        UL_Dis.Right = 2000;
}
/*
********************************************************************************************************************
*          Enaverage_Filter(Filter_Struct *filterStruct,short int newDatas,float FilterNum)
*
*Description : 增强型均值滤波  去掉一个最大值和最小值
*Arguments   : none
*Returns     : none
*Notes       : 输入的数据在程序中完成缓存
                            *filterStruct       选择滤波的结构体
                            newDatas                新数据
                            FilterNum               滤波样本总数
                            *Out                        输出值
********************************************************************************************************************
*/
void Ultrasonic_Filter(UltrasonicFilter_Struct *filterStruct, float newDatas, unsigned char FilterNum, u16 *Out)
{
    unsigned int i = 0;
    float FilterSum = 0;
    static unsigned short int max, min;

    filterStruct->Data_His[filterStruct->Count++] = newDatas;

    if (filterStruct->Complete) // 已经采集完最初的AdcFilterNum个数据
    {
        max = min = filterStruct->Data_His[0]; // 初始化
        // 找到最大最小值
        for (i = 1; i < FilterNum; i++)
        {
            if (filterStruct->Data_His[i] > max)
            {
                max = filterStruct->Data_His[i];
            }

            if (filterStruct->Data_His[i] < min)
            {
                min = filterStruct->Data_His[i];
            }
        }

        for (i = 0; i < FilterNum; i++)
        {
            FilterSum += filterStruct->Data_His[i];
        }
        FilterSum -= max + min;
        filterStruct->Cache = FilterSum / (FilterNum - 2);
    }

    if (filterStruct->Complete == 1)
    {
        if (filterStruct->Cache < 5000)
        {
            *Out = filterStruct->Cache;
            filterStruct->Last_Data = filterStruct->Cache;
        }
        else
            *Out = filterStruct->Last_Data;
    }
    if (filterStruct->Count == FilterNum)
    {
        filterStruct->Complete = 1;
        filterStruct->Count = 0;
    }
}

void Ultrasonic_TEST(void)
{
    OLED_Clear();
    Ultrasonic_Init();
    delay_ms(20);
    OLED_ShowString(12, 0, (void *)"Test_7 Sonic", 12);
    OLED_Refresh_Gram();
    //  u8 dis_train_low = 0;
    //  u8 dis_train_high = 0;
    while (test_flag && upmenumode == sonic_test_mode)
    {
        Ultrasonic_Get_Distance();
        delay_ms(100);
        LED1 = !LED1;
        OLED_ShowNum(45, 36, UL_Dis.Left, 3, 12);

        printf("%d\r\n", UL_Dis.Left);

        OLED_ShowNum(45, 48, UL_Dis.Right, 3, 12);
        OLED_Refresh_Gram();
    }
}
