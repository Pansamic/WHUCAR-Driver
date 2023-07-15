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
#include <main.h>
#include <hw_types.h>
#include <hw_i2c.h>
#include <tv_i2c.h>
uint16_t I2CMasterBusyTimeOut(uint32_t ulBase);

void TV_I2C_Init(void)
{
	SysCtlPeripheralDisable(SYSCTL_PERIPH_I2C8);
	SysCtlPeripheralReset(SYSCTL_PERIPH_I2C8);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C8);
	I2CMasterInitExpClk(I2C8_BASE, SYSTEM_FREQUENCY, true);
    I2CMasterGlitchFilterConfigSet(I2C8_BASE, I2C_MASTER_GLITCH_FILTER_8);
    // I2CTxFIFOConfigSet(I2C8_BASE, I2C_FIFO_CFG_TX_MASTER | I2C_FIFO_CFG_TX_TRIG_1);
	GPIOPinConfigure(GPIO_PA2_I2C8SCL);
	GPIOPinConfigure(GPIO_PA3_I2C8SDA);
	GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_2);
	GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_3);
    HWREG(I2C8_BASE + I2C_O_FIFOCTL) = 80008000; // clear I2C FIFOs
}

uint8_t I2C_ReadByte(uint32_t I2CHandle, uint8_t DevAddress, uint8_t DevReg)
{
    uint8_t Data;

    //specify that we want to communicate to device address with an intended write to bus
    I2CMasterSlaveAddrSet(I2CHandle, DevAddress, false);

    //the register to be read
    I2CMasterDataPut(I2CHandle, DevReg);

    //send control byte and register address byte to slave device
    I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_SEND_START);
    // I2CMasterControl(I2CHandle, I2C_MASTER_CMD_SINGLE_SEND);

    //wait for MCU to complete send transaction
    while(I2CMasterBusy(I2CHandle));

    //read from the specified slave device
    I2CMasterSlaveAddrSet(I2CHandle, DevAddress, true);

    // Repeated START condition followed by RECEIVE operation with a negative ACK (master remains in Master Receive state).
    // I2CMasterControl(I2CHandle, 0x13);
    I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_RECEIVE_START);

    // wait Ack from slave
    while(I2CMasterBusy(I2CHandle));

    // Get the data from the MCU register
    Data = I2CMasterDataGet(I2CHandle);

    // RECEIVE operation with negative ACK (master remains in Master Receive state).
    // I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

    // while(I2CMasterBusy(I2CHandle));

    return Data;
}

void I2C_WriteByte(uint32_t I2CHandle, uint8_t DevAddress, uint8_t DevReg, uint8_t Data)
{
    //specify that we want to communicate to device address with an intended write to bus
    I2CMasterSlaveAddrSet(I2CHandle, DevAddress, false);

    //register to be read
    I2CMasterDataPut(I2CHandle, DevReg);

    // while(I2CMasterBusy(I2CHandle));

    //send control byte and register address byte to slave device
    I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_SEND_START);

    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2CHandle));

    //specify data to be written to the above mentioned DevReg
    I2CMasterDataPut(I2CHandle, Data);

    //wait while checking for MCU to complete the transaction
    I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_SEND_FINISH);

    //wait for MCU & device to complete transaction
    while(I2CMasterBusy(I2CHandle));
}

void I2C_ReadData(uint32_t I2CHandle, uint8_t DevAddress, uint8_t DevReg, uint8_t *Data, uint8_t Length)
{
    uint8_t Index=0;

    I2CMasterSlaveAddrSet(I2CHandle, DevAddress, false);
    I2CMasterDataPut(I2CHandle, DevReg);
    while(I2CMasterBusy(I2CHandle));
    I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2CHandle));

    I2CMasterSlaveAddrSet(I2CHandle, DevAddress, true);
    I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while(I2CMasterBusy(I2CHandle));
    Data[0] = I2CMasterDataGet(I2CHandle);
    Index+=1;
    Length-=1;

    while(Length>1)
    {
        I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
        while(I2CMasterBusy(I2CHandle));
        Data[Index] = I2CMasterDataGet(I2CHandle);
        Index+=1;
        Length-=1;
    }


    I2CMasterControl(I2CHandle, 0x01);
    while(I2CMasterBusy(I2CHandle));
    Data[Index] = I2CMasterDataGet(I2CHandle);

    I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP);
    while(I2CMasterBusy(I2CHandle));
}

uint8_t I2C_TestReadByte(uint32_t I2CHandle, uint8_t DevAddress, uint8_t DevReg)
{
    uint8_t Data;
    //Write the command
    I2CMasterSlaveAddrSet(	I2CHandle,
                            DevAddress,		//7-bit slave address
                            false);	//write
    I2CMasterDataPut(I2CHandle, DevReg);				//cmd = read registers
    if(I2CMasterBusyTimeOut(I2CHandle))	return 0xFF;
    I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_SEND_START);

    if(I2CMasterBusyTimeOut(I2CHandle)) {
        I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_SEND_STOP);
        return 0xFF;
    }

    //read 1th word
    I2CMasterSlaveAddrSet(	I2CHandle,
                            DevAddress,		//7-bit slave address
                            true);	//read
    I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_RECEIVE_START);

    if(I2CMasterBusyTimeOut(I2CHandle)) {
        I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_SEND_STOP);
        return 0xFF;
    }
    Data = I2CMasterDataGet(I2CHandle);
    I2CMasterControl(I2CHandle, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

    return Data;
}
uint16_t I2CMasterBusyTimeOut(uint32_t ulBase)
{

	uint16_t i;

	//wait for the bus to be idle
	for(i = 0; i < I2C_MASTER_MAX_RETRIES; i++) {
		if(!I2CMasterBusBusy(ulBase))			return 0;
		SysCtlDelay(SYSTEM_FREQUENCY * 10e-6);		//wait 10us
	}
	if(I2CMasterBusBusy(ulBase))				return 1;
	else 										return 0;
}
