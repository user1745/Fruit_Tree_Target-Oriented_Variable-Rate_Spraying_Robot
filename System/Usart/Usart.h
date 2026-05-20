#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "stm32f4xx_conf.h"
#include "sys.h"
#include <stdarg.h>

#define USART1_EN 0
#define USART2_EN 1
#define USART3_EN 1
#define UART4_EN 0

#define USART1_DMA_EN 0
#define USART2_DMA_EN 0
#define USART3_DMA_EN 0
#define UART4_DMA_EN 0
#define UART5_DMA_EN 0

#define Niming2 0
#define Niming4 1

#if USART1_EN

#define USART1_MAX_RECV_LEN 400               // 最大接收缓存字节数
#define USART1_MAX_SEND_LEN 400               // 最大发送缓存字节数
#define USART1_RX_EN 0                        // 0,不接收;1,接收.
extern u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; // 接收缓冲,最大USART1_MAX_RECV_LEN字节
extern u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; // 发送缓冲,最大USART1_MAX_SEND_LEN字节
extern u16 USART1_RX_STA;                     // 接收数据状态

#endif

#if USART2_EN

#define USART2_MAX_RECV_LEN 400               // 最大接收缓存字节数
#define USART2_MAX_SEND_LEN 400               // 最大发送缓存字节数
#define USART2_RX_EN 1                        // 0,不接收;1,接收.
extern u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; // 接收缓冲,最大USART2_MAX_RECV_LEN字节
extern u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; // 发送缓冲,最大USART2_MAX_SEND_LEN字节
extern u16 USART2_RX_STA;                     // 接收数据状态
#endif

#if USART3_EN

#define USART3_MAX_RECV_LEN 400               // 最大接收缓存字节数
#define USART3_MAX_SEND_LEN 400               // 最大发送缓存字节数
#define USART3_RX_EN 1                        // 0,不接收;1,接收.
extern u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; // 接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; // 发送缓冲,最大USART3_MAX_SEND_LEN字节
extern u16 USART3_RX_STA;                     // 接收数据状态

#endif

#if UART4_EN

#define UART4_MAX_RECV_LEN 400              // 最大接收缓存字节数
#define UART4_MAX_SEND_LEN 400              // 最大发送缓存字节数
#define UART4_RX_EN 0                       // 0,不接收;1,接收.
extern u8 UART4_RX_BUF[UART4_MAX_RECV_LEN]; // 接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8 UART4_TX_BUF[UART4_MAX_SEND_LEN]; // 发送缓冲,最大USART3_MAX_SEND_LEN字节
extern u16 UART4_RX_STA;                    // 接收数据状态

#endif

#define BYTE0(dwTemp) (*(char *)(&dwTemp))       // 字分离四字节
#define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1)) // 字分离四字节
#define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2)) // 字分离四字节
#define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3)) // 字分离四字节

void Usart_init(void);

void u1_printf(char *fmt, ...);
void u2_printf(char *fmt, ...);
void u3_printf(char *fmt, ...);
void u4_printf(char *fmt, ...);

void Usart1_init(u32 bound);
void Usart2_init(u32 bound);
void Usart3_init(u32 bound);
void Uart4_init(u32 bound);

void usart_send_char(u8 c);

void usart_getmpu(void);

void usart_niming_report(u8 fun, u8 *data, u8 len);
void NM16_send_data(s16 u_a, s16 u_b, s16 u_c, s16 u_d, s16 u_e, s16 u_f, s16 u_g, s16 u_h, s16 u_i, s16 u_j);
void NM32_send_data(s32 ch1, s32 ch2, s32 ch3, s32 ch4);

void Usart_Send(unsigned char length);
void Usart3_Send(unsigned char length, int k);
void Usart_Send_User(s32 d1, s32 d2, s32 d3, s32 d4, s32 d5, s32 d6, s32 d7, s32 d8);
void Usart_Send_float(float d1, float d2, float d3, float d4);

#endif
