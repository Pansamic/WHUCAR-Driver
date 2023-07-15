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

void TV_I2C_Init(void);
uint8_t I2C_ReadByte(uint32_t I2CHandle, uint8_t DevAddress, uint8_t DevReg);
void I2C_WriteByte(uint32_t I2CHandle, uint8_t DevAddress, uint8_t DevReg, uint8_t Data);
void I2C_ReadData(uint32_t I2CHandle, uint8_t DevAddress, uint8_t DevReg, uint8_t *Data, uint8_t Length);
uint8_t I2C_TestReadByte(uint32_t I2CHandle, uint8_t DevAddress, uint8_t DevReg);
#ifdef __cplusplus
}
#endif
#endif /* APP_I2C_H_ */
