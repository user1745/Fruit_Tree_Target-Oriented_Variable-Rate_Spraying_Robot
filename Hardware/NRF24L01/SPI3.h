#ifndef __SPI3_H
#define __SPI3_H

#include "sys.h"

void NRF_SPI3_Init(void); // 初始化SPI1口
void NRF_SPI3_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 NRF_SPI3_ReadWriteByte(u8 TxData); // SPI1总线读写一个字节

#endif
