#ifndef __RS485_H
#define __RS485_H
#include "sys.h"

#define RS485_mode PDout(3) // RS485模式选择，拉高为发送模式

void RS485_Init(void); // Motor_24L01初始化
void RS485_Send_Data(uint32_t *BufferSRC, uint32_t BufferSize);
// 接收函数在串口5接收中断 UART5_IRQHandler 中UART5_IRQHandler
// 配合串口5使用
#endif
