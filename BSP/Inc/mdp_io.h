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
 *     1.UART_Handle:the pointer of usart that transmit and receive data.
 *     2.ActivatedOutBufIndex:the index of activated output buffer.
 *     3.Outputing:whether the output module is working.
 *     4.OutputBuf:the pointer of output buffers.
 *     5.OutputBufSize:the size of output buffers.
 *     6.OutputBufPosition:the position of output buffers.
 *     7.InputBuf:the pointer of input buffer.
 *     8.InputBufLen:the length of input buffer.
 *     9.OutputBuf1:the pointer of output buffer1.
 *     10.OutputBuf1Len:the length of output buffer1.
 *     11.OutputBuf2:the pointer of output buffer2.
 *     12.OutputBuf2Len:the length of output buffer2.
 *     13.Stringcb:the callback function when a string is received.
 *     14.PkgProcessor:the callback function when a package is received.
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
