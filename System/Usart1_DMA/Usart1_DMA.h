
#ifndef __USART1_DMA_H
#define __USART1_DMA_H
#include "sys.h"
#include "Usart.h"

#if USART1_DMA_EN

#define U1_DATA_LEN 200

extern u8 rec_data_u1[U1_DATA_LEN];  // 接收数据
extern u8 send_data_u2[U1_DATA_LEN]; // 发送数据

void USART1_DMA_Init(int Baud);
void USART1_DMATransfer(uint32_t *BufferSRC, uint32_t BufferSize); // UASRT DMA发送设置
void DMA2_S5_Reset(void);                                          // DMA1_Stream6 接收重置

#endif
#endif
