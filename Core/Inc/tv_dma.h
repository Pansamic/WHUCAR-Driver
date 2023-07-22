/**
 * @file tv_dma.h
 * @author pansamic (wanggengjie@ieee.org)
 * @brief uDMA module initialization and configuration
 * @version 0.1
 * @date 2023-06-24
 * 
 * @copyright Copyright (c) 2023 ALL RIGHTS RESERVED FOR PANSAMIC
 * 
 */

#ifndef __TV_DMA_H__
#define __TV_DMA_H__
#ifdef __cplusplus
extern "C" {
#endif
#define DMA_INT_CH0  0x00000001
#define DMA_INT_CH1  0x00000002
#define DMA_INT_CH2  0x00000004
#define DMA_INT_CH3  0x00000008
#define DMA_INT_CH4  0x00000010
#define DMA_INT_CH5  0x00000020
#define DMA_INT_CH6  0x00000040
#define DMA_INT_CH7  0x00000080
#define DMA_INT_CH8  0x00000100
#define DMA_INT_CH9  0x00000200
#define DMA_INT_CH10 0x00000400
#define DMA_INT_CH11 0x00000800
#define DMA_INT_CH12 0x00001000
#define DMA_INT_CH13 0x00002000
#define DMA_INT_CH14 0x00004000
#define DMA_INT_CH15 0x00008000
#define DMA_INT_CH16 0x00010000
#define DMA_INT_CH17 0x00020000
#define DMA_INT_CH18 0x00040000
#define DMA_INT_CH19 0x00080000
#define DMA_INT_CH20 0x00100000
#define DMA_INT_CH21 0x00200000
#define DMA_INT_CH22 0x00400000
#define DMA_INT_CH23 0x00800000
#define DMA_INT_CH24 0x01000000
#define DMA_INT_CH25 0x02000000
#define DMA_INT_CH26 0x04000000
#define DMA_INT_CH27 0x08000000
#define DMA_INT_CH28 0x10000000
#define DMA_INT_CH29 0x20000000
#define DMA_INT_CH30 0x40000000
#define DMA_INT_CH31 0x80000000

void TV_DMA_Init( void );



#ifdef __cplusplus
}
#endif
#endif // __TV_DMA_H__
