#ifndef __UART5_DMA_H
#define __UART5_DMA_H
#include "sys.h"

#if UART5_DMA_EN

#define U5_DATA_LEN 15
extern u8 rec_data_u5[U5_DATA_LEN];  // 接收数据
extern u8 send_data_u5[U5_DATA_LEN]; // 接收数据

extern void UART5_DMA_Init(int Baud);
extern void UART5_DMATransfer(uint32_t *BufferSRC, uint32_t BufferSize); // UASRT DMA发送设置
extern void DMA1_S0_Reset(void);                                         // DMA1_Stream1 接收重置

#endif
#endif
