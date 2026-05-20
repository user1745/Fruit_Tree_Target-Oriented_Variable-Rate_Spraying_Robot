
#ifndef __UART4_DMA_H
#define __UART4_DMA_H
#include "sys.h"
#include "Usart.h"

#if UART4_DMA_EN

#define U4_DATA_LEN 200

extern u8 rec_data_u4[U4_DATA_LEN];  // 接收数据
extern u8 send_data_u4[U4_DATA_LEN]; // 接收数据

void UART4_DMA_Init(int Baud);
void USART4_DMATransfer(uint32_t *BufferSRC, uint32_t BufferSize); // UASRT DMA发送设置
void DMA1_S2_Reset(void);                                          // DMA1_Stream2 接收重置

#endif
#endif
