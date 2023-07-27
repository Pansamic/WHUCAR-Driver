/*****************************************************************************
THIS PROGRAM IS FREE SOFTWARE. YOU CAN REDISTRIBUTE IT AND/OR MODIFY IT
UNDER THE TERMS OF THE GNU GPLV3 AS PUBLISHED BY THE FREE SOFTWARE FOUNDATION.

Copyright (C), 2022-2023, pansamic(Wang GengJie) pansamic@foxmail.com

Filename:    tf_luna.c
Author:      Pansamic
Version:     0.1
Create date: 2023.1.14
Description: This file contains all supported peripherals.
Others:      none

History:
1. <author>    <date>                  <desc>
   pansamic  2023.1.14     create v0.1 version.
*****************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif
#include <stdio.h>
#include <mdp_io.h>
#include <tf_luna.h>


#define GetTFLunaFromio(addr,type,field) ((type*)((unsigned char*)addr - (unsigned long)&((type*)0)->field))
const uint8_t TFLunaPkgHead[2]={0x59,0x59};

const MDP_PkgFmt TFLunaPkgFmt=
{
    1,               // enable package head
	TFLunaPkgHead,   // use default head sequence
    2,               // head sequence length equals two
    0,               // disable data check
    NULL,            // don't use check algorithm
    0,               // disable tail
    NULL,            // use null tail sequence
    0,               // tail sequence length equals 0
    0                // tail after check value. unnecessary value, because tail is disabled
};


void TFLuna_ioPkgProccb(MDP_io* ioDevice,uint8_t *DataPkg);


void Add_TFLuna(TFLuna *pTFLuna, const char *Name,uint32_t UARTHandle)
{
	if(pTFLuna == NULL)
	{
		printf("[error  ] pass null TFluna pointer to init function.\r\n");
		return;
	}

	io_Init(&(pTFLuna->ioDevice), UARTHandle, pTFLuna->InputBuf, LUNA_IO_INPUT_BUF_LEN, pTFLuna->OutputBuf1, LUNA_IO_OUTPUT_BUF1_LEN, pTFLuna->OutputBuf2, LUNA_IO_OUTPUT_BUF2_LEN);
	io_PackageMode(&(pTFLuna->ioDevice), pTFLuna->ParamsContainer, 1, 7, TFLuna_ioPkgProccb);
	io_SetPkgParseFmt(&(pTFLuna->ioDevice),&TFLunaPkgFmt);
	TFLuna_Reset(pTFLuna);
	printf("TF-Luna laser sensor '%s' is ready.\r\n",Name);
}

void TFLuna_Reset(TFLuna *pTFLuna)
{
	static const uint8_t ResetSequence[5] = {0x5A,0x05,0x05,0x06,0x00};
	io_SendData(&(pTFLuna->ioDevice),(void*)ResetSequence,5);
	pTFLuna->Amplification = 0;
	pTFLuna->Distance = 0;
}

uint16_t TFLuna_GetDistance(TFLuna *pTFLuna)
{
	return pTFLuna->Distance;
}

uint16_t TFLuna_GetAmp(TFLuna *pTFLuna)
{
	return pTFLuna->Amplification;
}

void TFLuna_ioPkgProccb(MDP_io* ioDevice,uint8_t *DataPkg)
{
	TFLuna* pTFLuna = GetTFLunaFromio(ioDevice,TFLuna,ioDevice);
	uint16_t Distance = DataPkg[0]|DataPkg[1]<<8;
	uint16_t Amplification = DataPkg[2]|DataPkg[3]<<8;
	if(Distance != 0 && Amplification > 100)
	{
		pTFLuna->Distance = DataPkg[0]|DataPkg[1]<<8;
	}
	pTFLuna->Amplification = Amplification;
}

#ifdef __cplusplus
}
#endif
