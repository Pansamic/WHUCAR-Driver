/*****************************************************************************
THIS PROGRAM IS FREE SOFTWARE. YOU CAN REDISTRIBUTE IT AND/OR MODIFY IT
UNDER THE TERMS OF THE GNU GPLV3 AS PUBLISHED BY THE FREE SOFTWARE FOUNDATION.

Copyright (C), 2022-2023, pansamic(Wang GengJie) pansamic@foxmail.com

Filename:    mdp_io.h
Author:      Pansamic
Version:     1.0
Create date: 2023.1.15
Description:
Others:      none

History:
1. <author>    <date>                  <desc>
   pansamic  2023.1.15    create v1.0 version.
*****************************************************************************/
#ifndef __MDP_IO_H__
#define __MDP_IO_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdarg.h>
#include <mdp_pkgproc.h>
/**********************************************************************************************
 *                                                                                            *
 *                                         MACROS                                             *
 *                                                                                            *
 **********************************************************************************************/

#define PACKAGE_MODE              1
#define STRING_MODE               2

#define MAXIODEVICEAMOUNT         6
/**********************************************************************************************
 *                                                                                            *
 *                                         TYPEDEF                                            *
 *                                                                                            *
 **********************************************************************************************/

/**********************************************************************************************
 * @brief:This struct describes a general model of input/outpur device. Input buffer is a ring
 *     buffer. Output module uses ping-pong buffer technique.
 * @params:
 *     1.Name:A string containing the name of io device.
 *     2.huart:the pointer of usart that transmit and receive data.
 *     3.ActivatedOutBufIndex:current writable buffer index.
 *     4.Outputing:1 when buffer data is transmitting, 0 when no buffer is transmitting.
 *     5.OutputBuf:an array that contains two buffer address.
 *     6.OutputBufSize:an array that contains the size of both output buffers.
 *     7.OutputBufPosition:an array that contains the length of data that is put into buffer.
 *     8.InputBuf:the pointer of input buffer.
 *     9.InputBufSize:size of input buffer.
 *     10.InputHeadIndex:ring buffer head index.
 *     11.InputTailIndex:ring buffer tail index.
 *     12.InputBufFull:a flag indicating whether input buffer is full. its value can be 0 or 1.
 *     13.CmdList:chain of 'MDP_Cmd' type node.
 *     14.DataPackage:if the value is 0, it means the io device works in command mode.
 *         if the value is greater than 0, that means the io device works in data package mode.
 * */
typedef struct ioDeviceTypeDef
{
	uint32_t UART_Handle;
	/* double buffer */
	uint8_t     ActivatedOutBufIndex;
	uint8_t     Outputing;
	uint8_t    *OutputBuf[2];
	uint32_t    OutputBufSize[2];
	uint32_t    OutputBufPosition[2];


	/* input buffer related */
	uint8_t    *InputBuf;
	uint32_t    InputBufSize;
	uint32_t    InputHeadIndex;
	uint32_t    InputTailIndex;
	uint8_t     InputBufFull;

	uint8_t     ReceiveMode;
	void      (*Stringcb)(char*);
	MDP_PkgPrc PkgProcessor;
}MDP_io;

/**********************************************************************************************
 *                                                                                            *
 *                                         EXTERN                                             *
 *                                                                                            *
 **********************************************************************************************/
/* system built-in io device */
extern MDP_io mdpio;

/**********************************************************************************************
 *                                                                                            *
 *                                     GLOBAL FUNCTION                                        *
 *                                                                                            *
 **********************************************************************************************/

void       io_Init                    (MDP_io *ioDevice, uint32_t UART_Handle, uint8_t *InputBuf, uint32_t InputBufLen, uint8_t *OutputBuf1, uint32_t OutputBuf1Len, uint8_t *OutputBuf2, uint32_t OutputBuf2Len);
void       mdpioInit                  (uint32_t UART_Handle);
void       io_Process                 (void);


/*----------------*/
/* output related */
/*----------------*/
void       MDP_printf                 (const char *fmt,...);
void       io_printf                  (MDP_io *ioDevice,const char *fmt,...);
void       io_SendData                (MDP_io *ioDevice, void *pData, uint32_t Length);
void       io_TransOverHandler        (uint32_t huart);

void      _io_vprintf                 (MDP_io *ioDevice,const char *fmt, va_list ap);
void      _io_OutputProcess           (MDP_io *ioDevice);
uint32_t  _io_GetSpareOutBuf          (MDP_io *ioDevice, uint8_t **pDataDst);
uint8_t   _io_ActivatedBufEmpty       (MDP_io *ioDevice);
void      _io_Transmit                (MDP_io *ioDevice, uint8_t *pData, uint32_t Length);

/*---------------*/
/* input related */
/*---------------*/
uint32_t   io_GetData                 (MDP_io *ioDevice, uint8_t *pDataDst, uint32_t Length);
void       io_StringMode              (MDP_io *ioDevice, void(*Stringcb)(char*));
void      _io_InputProcess            (MDP_io *ioDevice);
void      _io_InputBufWrite           (MDP_io *ioDevice, uint8_t *pData, uint32_t Length);
void      _io_InputBufWriteByte       (MDP_io *ioDevice, uint8_t Data);
uint32_t  _io_GetLine                 (MDP_io *ioDevice, uint8_t *pDataDst, uint32_t DstLength);
uint8_t   _io_GetByte                 (MDP_io *ioDevice, uint8_t *pDataDst);
void      _io_InputBufRemove          (MDP_io *ioDevice, uint32_t Length);
uint8_t   _io_InputBufRemoveByte      (MDP_io *ioDevice);
void      _io_InputBufReset           (MDP_io *ioDevice);
uint8_t   _io_InputBufFull            (MDP_io *ioDevice);
uint8_t   _io_InputBufEmpty           (MDP_io *ioDevice);
uint32_t  _io_InputBufCapacity        (MDP_io *ioDevice);
uint32_t  _io_InputBufGetSize         (MDP_io *ioDevice);
uint32_t  _io_InputBufGetSpare        (MDP_io *ioDevice);

#ifdef __cplusplus
}
#endif
#endif
