/*****************************************************************************
THIS PROGRAM IS FREE SOFTWARE. YOU CAN REDISTRIBUTE IT AND/OR MODIFY IT
UNDER THE TERMS OF THE GNU GPLV3 AS PUBLISHED BY THE FREE SOFTWARE FOUNDATION.

Copyright (C), 2022-2023, pansamic(Wang GengJie) pansamic@foxmail.com

Filename:    tf_luna.h
Author:      Pansamic
Version:     0.1
Create date: 2023.1.14
Description: This file contains all supported peripherals.
Others:      none

History:
1. <author>    <date>                  <desc>
   pansamic  2023.1.14     create v0.1 version.
*****************************************************************************/
#ifndef __TF_LUNA_H__
#define __TF_LUNA_H__
#ifdef __cplusplus
extern "C"{
#endif

#include <mdp_io.h>

#define LUNA_IO_INPUT_BUF_LEN 256
#define LUNA_IO_OUTPUT_BUF1_LEN 32
#define LUNA_IO_OUTPUT_BUF2_LEN 32

typedef struct
{
	uint16_t Distance;
	uint16_t Amplification;
	MDP_io   ioDevice;
	uint8_t  ParamsContainer[7];
	uint8_t  InputBuf[LUNA_IO_INPUT_BUF_LEN];
	uint8_t  OutputBuf1[LUNA_IO_OUTPUT_BUF1_LEN];
	uint8_t  OutputBuf2[LUNA_IO_OUTPUT_BUF2_LEN];
}TFLuna;

void     Add_TFLuna         (TFLuna *pTFLuna, const char *Name, uint32_t UARTHandle);
void     TFLuna_Reset       (TFLuna *pTFLuna);
uint16_t TFLuna_GetDistance (TFLuna *pTFLuna);
uint16_t TFLuna_GetAmp      (TFLuna *pTFLuna);

#ifdef __cplusplus
}
#endif
#endif
