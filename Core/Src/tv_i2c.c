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
volatile bool I2C8Done = true;
tI2CMWrite8 I2C8WriteInst;

/* I2C Master Module Test */
// void I2CMSimpleExample(void);
void I2C8_I2CMCallback(void *pvData, uint_fast8_t ui8Status);


void TV_I2C_Init(void)
{
	SysCtlPeripheralDisable(SYSCTL_PERIPH_I2C8);
	SysCtlPeripheralReset(SYSCTL_PERIPH_I2C8);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C8);
	I2CMasterInitExpClk(I2C8_BASE, SYSTEM_FREQUENCY, true);
    // I2CMasterGlitchFilterConfigSet(I2C8_BASE, I2C_MASTER_GLITCH_FILTER_8);
    // I2CTxFIFOConfigSet(I2C8_BASE, I2C_FIFO_CFG_TX_MASTER | I2C_FIFO_CFG_TX_TRIG_1);
	GPIOPinConfigure(GPIO_PA2_I2C8SCL);
	GPIOPinConfigure(GPIO_PA3_I2C8SDA);
	GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_2);
	GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_3);
    I2CIntRegister(I2C8_BASE,I2C8_IntHandler);
    I2CMasterIntEnable(I2C8_BASE);
    HWREG(I2C8_BASE + I2C_O_FIFOCTL) = 80008000; // clear I2C FIFOs
    I2CMInit(&I2C8Inst, I2C8_BASE, INT_I2C8, 0xff, 0xff, 120000000);
}

//
// The simple I2C master driver example.
//
// void I2CMSimpleExample(void)
// {
//     uint8_t ui8Reg;
//     uint8_t pui8Data[16];
//     //
//     // Initialize the I2C master driver. It is assumed that the I2C module has
//     // already been enabled and the I2C pins have been configured.
//     //
//     I2CMInit(&I2C8Inst, I2C8_BASE, INT_I2C8, 0xff, 0xff, 120000000);
//     //
//     // Write two bytes of data to the I2C device at address 0x22.
//     //
//     I2C8Done = false;
//     ui8Reg = 0x13;
//     pui8Data[0] = 0x20;
//     pui8Data[1] = 0x21;
//     pui8Data[2] = 0x22;
//     pui8Data[3] = 0x23;

// //    I2CMWrite(&g_sI2CMSimpleInst, 0x69, pui8Data, 4, I2C8_I2CMCallback, 0);
// 	I2CMWrite8(&I2C8WriteInst,&I2C8Inst,0x69,0x13,pui8Data,1,I2C8_I2CMCallback,0);
//     while(!I2C8Done)
//     {
//     }
//     //
//     // Read four bytes of data from the I2C device at address 0x31.
//     //
//     I2C8Done = false;
//     pui8Data[0] = 0x88;
//     I2CMRead(&I2C8Inst, 0x69, &ui8Reg, 1, pui8Data, 4, I2C8_I2CMCallback, 0);
//     while(!I2C8Done)
//     {
//     }
//     printf("%x | %x | %x | %x\r\n", pui8Data[0], pui8Data[1], pui8Data[2], pui8Data[3]);
// }
uint8_t I2C8_ReadByte(uint8_t DevAddress, uint8_t DevReg)
{
    uint8_t Data;
    I2C8Done = false;
    I2CMRead(&I2C8Inst, DevAddress, &DevReg, 1, &Data, 1, I2C8_I2CMCallback, 0);
    while(!I2C8Done)
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

void I2C8_WriteByte(uint8_t DevAddress, uint8_t DevReg, const uint8_t Data)
{
    I2C8Done = false;
    I2CMWrite8(&I2C8WriteInst,&I2C8Inst,(uint_fast8_t)DevAddress,DevReg,&Data,1,I2C8_I2CMCallback,0);
    while(!I2C8Done)
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


// uint8_t I2C_ReadByte(uint32_t I2CHandle, uint8_t DevAddress, uint8_t DevReg)
// {
//     uint8_t Data;

//     //specify that we want to communicate to device address with an intended write to bus
//     I2CMasterSlaveAddrSet(I2CHandle, DevAddress, false);

//     //the register to be read
//     I2CMasterDataPut(I2CHandle, DevReg);

//     //send control byte and register address byte to slave device
//     I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_SEND_START);
//     // I2CMasterControl(I2CHandle, I2C_MASTER_CMD_SINGLE_SEND);

//     //wait for MCU to complete send transaction
//     while(I2CMasterBusy(I2CHandle));

//     //read from the specified slave device
//     I2CMasterSlaveAddrSet(I2CHandle, DevAddress, true);

//     // Repeated START condition followed by RECEIVE operation with a negative ACK (master remains in Master Receive state).
//     // I2CMasterControl(I2CHandle, 0x13);
//     I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_RECEIVE_START);

//     // wait Ack from slave
//     while(I2CMasterBusy(I2CHandle));

//     // Get the data from the MCU register
//     Data = I2CMasterDataGet(I2CHandle);

//     // RECEIVE operation with negative ACK (master remains in Master Receive state).
//     // I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

//     // while(I2CMasterBusy(I2CHandle));

//     return Data;
// }

// void I2C_WriteByte(uint32_t I2CHandle, uint8_t DevAddress, uint8_t DevReg, uint8_t Data)
// {
//     //specify that we want to communicate to device address with an intended write to bus
//     I2CMasterSlaveAddrSet(I2CHandle, DevAddress, false);

//     //register to be read
//     I2CMasterDataPut(I2CHandle, DevReg);

//     // while(I2CMasterBusy(I2CHandle));

//     //send control byte and register address byte to slave device
//     I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_SEND_START);

//     //wait for MCU to finish transaction
//     while(I2CMasterBusy(I2CHandle));

//     //specify data to be written to the above mentioned DevReg
//     I2CMasterDataPut(I2CHandle, Data);

//     //wait while checking for MCU to complete the transaction
//     I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_SEND_FINISH);

//     //wait for MCU & device to complete transaction
//     while(I2CMasterBusy(I2CHandle));
// }

// void I2C_ReadData(uint32_t I2CHandle, uint8_t DevAddress, uint8_t DevReg, uint8_t *Data, uint8_t Length)
// {
//     uint8_t Index=0;

//     I2CMasterSlaveAddrSet(I2CHandle, DevAddress, false);
//     I2CMasterDataPut(I2CHandle, DevReg);
//     while(I2CMasterBusy(I2CHandle));
//     I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_SEND_START);
//     while(I2CMasterBusy(I2CHandle));

//     I2CMasterSlaveAddrSet(I2CHandle, DevAddress, true);
//     I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_RECEIVE_START);
//     while(I2CMasterBusy(I2CHandle));
//     Data[0] = I2CMasterDataGet(I2CHandle);
//     Index+=1;
//     Length-=1;

//     while(Length>1)
//     {
//         I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
//         while(I2CMasterBusy(I2CHandle));
//         Data[Index] = I2CMasterDataGet(I2CHandle);
//         Index+=1;
//         Length-=1;
//     }


//     I2CMasterControl(I2CHandle, 0x01);
//     while(I2CMasterBusy(I2CHandle));
//     Data[Index] = I2CMasterDataGet(I2CHandle);

//     I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP);
//     while(I2CMasterBusy(I2CHandle));
// }

