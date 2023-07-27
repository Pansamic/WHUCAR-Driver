/*****************************************************************************
THIS PROGRAM IS FREE SOFTWARE. YOU CAN REDISTRIBUTE IT AND/OR MODIFY IT
UNDER THE TERMS OF THE GNU GPLV3 AS PUBLISHED BY THE FREE SOFTWARE FOUNDATION.

Copyright (C), 2022-2023, pansamic(Wang GengJie) pansamic@foxmail.com

Filename:    mdp_command.h
Author:      Pansamic
Version:     1.0
Create date: 2023.1.15
Description:
Others:      none

History:
1. <author>    <date>                  <desc>
   pansamic  2023.1.15    create v1.0 version.
*****************************************************************************/
#ifndef __MDP_COMMAND_H__
#define __MDP_COMMAND_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdarg.h>
#include <stdint.h>
/**********************************************************************************************
 *                                                                                            *
 *                                         MACROS                                             *
 *                                                                                            *
 **********************************************************************************************/
#define PARAMTYPE_char            1
#define PARAMTYPE_unsigned_char   2
#define PARAMTYPE_int             3
#define PARAMTYPE_unsigned_int    4
#define PARAMTYPE_short           5
#define PARAMTYPE_unsigned_short  6
#define PARAMTYPE_long            7
#define PARAMTYPE_unsigned_long   8
#define PARAMTYPE_float           9
#define PARAMTYPE_double          10
#define PARAMTYPE_int8_t          11
#define PARAMTYPE_int16_t         12
#define PARAMTYPE_int32_t         13
#define PARAMTYPE_uint8_t         14
#define PARAMTYPE_uint16_t        15
#define PARAMTYPE_uint32_t        16

/**********************************************************************************************
 *                                                                                            *
 *                                         TYPEDEF                                            *
 *                                                                                            *
 **********************************************************************************************/
typedef struct ioDeviceTypeDef MDP_io;

/**********************************************************************************************
 * @brief:
 * @params:
 *    1.CmdName:a string of command name.
 *    2.ParamsDescription:a string that describes parameter of the command. e.g."float,uint8_t"
 *    3.ParamsContainer:a buffer that contains the value of command. Its capacity is
 *        initialized as the sum of the spaces taken up by all the parameters.
 *    4.CmdCallbackFunc:the function that will be called when the command is input.
 *    5.pNext:next pointer of 'MDP_Cmd'.
 * */
typedef struct ioCmdDefinition
{
	const char  *CmdName;
	const char  *ParamsDescription;
	uint8_t     *ParamsContainer;
	void       (*CmdCallbackFunc)(uint8_t*);
	struct ioCmdDefinition *pNext;
}MDP_Cmd;

/**********************************************************************************************
 *                                                                                            *
 *                                         EXTERN                                             *
 *                                                                                            *
 **********************************************************************************************/

/**********************************************************************************************
 *                                                                                            *
 *                                     GLOBAL FUNCTION                                        *
 *                                                                                            *
 **********************************************************************************************/
/* command related */
void       io_CommandMode             (MDP_io *ioDevice);
void       io_AddCmd                  (MDP_io *ioDevice, const char *CmdName, const char *ParamsDescription, void(*CmdCallbackFunc)(uint8_t*));

void      _io_CmdProcess              (MDP_Cmd *CmdList, char *CmdStr);
int       _io_ScanWriteParamData      (char *str, uint8_t ParamType, uint8_t *pDataDst);
void      _io_CmdExecute              (MDP_Cmd *Cmd);
uint8_t   _io_GetParamSizeByStr       (char *ParamStr);
uint8_t   _io_GetParamType            (char *Str);
uint8_t   _io_GetParamSizeByTypeCode  (uint8_t ParamType);

#ifdef __cplusplus
}
#endif
#endif
