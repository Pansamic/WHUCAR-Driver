/**
 * @file tv_i2c.c
 * @author pansamic (wanggengjie@ieee.org)
 * @brief I2C initialization and customized I2C functions
 * @version 0.1
 * @date 2023-06-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <string.h>
#include <main.h>
#include <hw_types.h>
#include <hw_i2c.h>
#include <hw_ints.h>
#include <tv_i2c.h>
#include <tm4c129_it.h>

#include <FreeRTOS.h>

tI2CMInstance I2C8Inst;
tI2CMInstance I2C2Inst;
volatile bool I2C8Done = true;
volatile bool I2C2Done = true;
tI2CMWrite8 I2C8WriteInst;
tI2CMWrite8 I2C2WriteInst;

/* I2C Master Module Test */
// void I2CMSimpleExample(void);
void I2C8_I2CMCallback(void *pvData, uint_fast8_t ui8Status);

void I2C2_I2CMCallback(void *pvData, uint_fast8_t ui8Status);


void TV_I2C_Init(void)
{
	SysCtlPeripheralDisable(SYSCTL_PERIPH_I2C8);
	SysCtlPeripheralReset(SYSCTL_PERIPH_I2C8);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C8);
	I2CMasterInitExpClk(I2C8_BASE, SYSTEM_FREQUENCY, true);
	GPIOPinConfigure(GPIO_PA2_I2C8SCL);
	GPIOPinConfigure(GPIO_PA3_I2C8SDA);
	GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_2);
	GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_3);
    I2CIntRegister(I2C8_BASE,I2C8_IntHandler);
    I2CMasterIntEnable(I2C8_BASE);
    HWREG(I2C8_BASE + I2C_O_FIFOCTL) = 80008000; // clear I2C FIFOs
    I2CMInit(&I2C8Inst, I2C8_BASE, INT_I2C8, 0xff, 0xff, 120000000);

    // SysCtlPeripheralDisable(SYSCTL_PERIPH_I2C2);
    // SysCtlPeripheralReset(SYSCTL_PERIPH_I2C2);
    // SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
    // SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
    // I2CMasterInitExpClk(I2C2_BASE, SYSTEM_FREQUENCY, true);
    // GPIOPinConfigure(GPIO_PL1_I2C2SCL);
    // GPIOPinConfigure(GPIO_PL0_I2C2SDA);
    // GPIOPinTypeI2CSCL(GPIO_PORTL_BASE, GPIO_PIN_1);
    // GPIOPinTypeI2C(GPIO_PORTL_BASE, GPIO_PIN_0);
    // I2CIntRegister(I2C2_BASE,I2C2_IntHandler);
    // I2CMasterIntEnable(I2C2_BASE);
    // HWREG(I2C2_BASE + I2C_O_FIFOCTL) = 80008000; // clear I2C FIFOs
    // I2CMInit(&I2C2Inst, I2C2_BASE, INT_I2C2, 0xff, 0xff, 120000000);
}

uint8_t I2C8_ReadByte(uint8_t DevAddress, uint8_t DevReg)
{
    uint8_t Data;
    I2C8Done = false;
    I2CMRead(&I2C8Inst, DevAddress, &DevReg, 1, &Data, 1, I2C8_I2CMCallback, 0);
    while(!I2C8Done)
    {}
    return Data;
}

uint8_t I2C2_ReadByte(uint8_t DevAddress, uint8_t DevReg)
{
    uint8_t Data;
    I2C2Done = false;
    I2CMRead(&I2C2Inst, DevAddress, &DevReg, 1, &Data, 1, I2C2_I2CMCallback, 0);
    while(!I2C2Done)
    {}
    return Data;
}

void I2C8_Read(uint8_t DevAddress, uint8_t DevReg, uint8_t *pDataDst, uint8_t Length)
{
    I2C8Done = false;
    I2CMRead(&I2C8Inst, DevAddress, &DevReg, 1, pDataDst, Length, I2C8_I2CMCallback, 0);
    while(!I2C8Done)
    {}
}

void I2C2_Read(uint8_t DevAddress, uint8_t DevReg, uint8_t *pDataDst, uint8_t Length)
{
    I2C2Done = false;
    I2CMRead(&I2C2Inst, DevAddress, &DevReg, 1, pDataDst, Length, I2C2_I2CMCallback, 0);
    while(!I2C2Done)
    {}
}

void I2C8_WriteByte(uint8_t DevAddress, uint8_t DevReg, const uint8_t Data)
{
    I2C8Done = false;
    I2CMWrite8(&I2C8WriteInst,&I2C8Inst,(uint_fast8_t)DevAddress,DevReg,&Data,1,I2C8_I2CMCallback,0);
    while(!I2C8Done)
    {}
}

void I2C2_WriteByte(uint8_t DevAddress, uint8_t DevReg, const uint8_t Data)
{
    I2C2Done = false;
    I2CMWrite8(&I2C2WriteInst,&I2C2Inst,(uint_fast8_t)DevAddress,DevReg,&Data,1,I2C2_I2CMCallback,0);
    while(!I2C2Done)
    {}
}

void I2C8_Write(uint8_t DevAddress, uint8_t DevReg, uint8_t *pData, uint8_t Length)
{
    uint8_t *DataBuf = pvPortMalloc(Length+1);
    if(DataBuf==NULL)
    {
        printf("I2C8_Write: malloc failed\r\n");
        return;
    }
    DataBuf[0]=DevReg;
    memcpy(DataBuf+1, pData, Length);
    I2C8Done = false;
    I2CMWrite(&I2C8Inst, DevAddress, DataBuf, Length+1, I2C8_I2CMCallback, 0);
    while(!I2C8Done)
    {}
    vPortFree(DataBuf);
}

void I2C2_Write(uint8_t DevAddress, uint8_t DevReg, uint8_t *pData, uint8_t Length)
{
    uint8_t *DataBuf = pvPortMalloc(Length+1);
    if(DataBuf==NULL)
    {
        printf("I2C2_Write: malloc failed\r\n");
        return;
    }
    DataBuf[0]=DevReg;
    memcpy(DataBuf+1, pData, Length);
    I2C2Done = false;
    I2CMWrite(&I2C2Inst, DevAddress, DataBuf, Length+1, I2C2_I2CMCallback, 0);
    while(!I2C2Done)
    {}
    vPortFree(DataBuf);
}

void I2C8_I2CMCallback(void *pvData, uint_fast8_t ui8Status)
{
    //
    // See if an error occurred.
    //
    if(ui8Status != I2CM_STATUS_SUCCESS)
    {
    //
    // An error occurred, so handle it here if required.
    //
    }
    //
    // Indicate that the I2C transaction has completed.
    //
    I2C8Done = true;
}

void I2C2_I2CMCallback(void *pvData, uint_fast8_t ui8Status)
{
    //
    // See if an error occurred.
    //
    if(ui8Status != I2CM_STATUS_SUCCESS)
    {
    //
    // An error occurred, so handle it here if required.
    //
    printf("err:%d\r\n",ui8Status);
    }
    //
    // Indicate that the I2C transaction has completed.
    //
    I2C2Done = true;
}
