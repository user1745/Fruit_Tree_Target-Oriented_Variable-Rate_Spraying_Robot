
#ifndef __USART3_DMA_H
#define __USART3_DMA_H
#include "sys.h"
#include "Usart.h"

#if USART3_DMA_EN

#define U3_DATA_LEN 200

extern u8 connect_flag;              // 连接标志
extern u8 rec_data_u3[U3_DATA_LEN];  // 接收数据
extern u8 send_data_u3[U3_DATA_LEN]; // 接收数据

void USART3_DMA_Init(int Baud);
void USART3_DMATransfer(uint32_t *BufferSRC, uint32_t BufferSize); // UASRT DMA发送设置
void DMA1_S1_Reset(void);                                          // DMA1_Stream1 接收重置

#endif
#endif
