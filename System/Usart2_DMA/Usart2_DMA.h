
#ifndef __USART2_DMA_H
#define __USART2_DMA_H
#include "sys.h"
#include "Usart.h"

#if USART2_DMA_EN

#define U2_DATA_LEN 200

extern u8 rec_data_u2[U2_DATA_LEN];  // 接收数据
extern u8 send_data_u2[U2_DATA_LEN]; // 接收数据

void USART2_DMA_Init(int Baud);
void USART2_DMATransfer(uint32_t *BufferSRC, uint32_t BufferSize); // UASRT DMA发送设置
void DMA1_S5_Reset(void);                                          // DMA1_Stream6 接收重置

#endif
#endif
