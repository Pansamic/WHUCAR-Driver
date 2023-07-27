/**
 * @file tv_i2c.h
 * @author pansamic (wanggengjie@ieee.org)
 * @brief Customized I2C driver header file
 * @version 0.1
 * @date 2023-06-02
 * 
 * @copyright Copyright (c) 2023 ALL RIGHTS RESERVED FOR PANSAMIC
 * 
 */

#ifndef _APP_I2C_H_
#define _APP_I2C_H_
#ifdef __cplusplus
extern "C"{
#endif
#include <stdint.h>
#include <i2cm_drv.h>
extern tI2CMInstance I2C8Inst;
extern tI2CMInstance I2C2Inst;
extern volatile bool I2C8Done;
extern volatile bool I2C2Done;
extern tI2CMWrite8 I2C8WriteInst;
extern tI2CMWrite8 I2C2WriteInst;

void TV_I2C_Init(void);
uint8_t I2C8_ReadByte(uint8_t DevAddress, uint8_t DevReg);
void I2C8_WriteByte(uint8_t DevAddress, uint8_t DevReg, uint8_t Data);
void I2C8_Read(uint8_t DevAddress, uint8_t DevReg, uint8_t *pDataDst, uint8_t Length);
void I2C8_Write(uint8_t DevAddress, uint8_t DevReg, uint8_t *pData, uint8_t Length);

uint8_t I2C2_ReadByte(uint8_t DevAddress, uint8_t DevReg);
void I2C2_WriteByte(uint8_t DevAddress, uint8_t DevReg, uint8_t Data);
void I2C2_Read(uint8_t DevAddress, uint8_t DevReg, uint8_t *pDataDst, uint8_t Length);
void I2C2_Write(uint8_t DevAddress, uint8_t DevReg, uint8_t *pData, uint8_t Length);
#ifdef __cplusplus
}
#endif
#endif /* APP_I2C_H_ */
