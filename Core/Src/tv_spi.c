/**
 * @file tv_spi.c
 * @author darkbe
 * @brief 
 * @version 0.1
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <main.h>
#include <tv_spi.h>>
#include <tv_gpio.h>
#include <hw_ints.h>
#include <mdp.h>

#define DC_PIN  GPIO_PIN_0
#define MOSI_PIN  GPIO_PIN_1
#define NUM_SSI_DATA 3

void TV_SPI_Init(void){
    uint32_t pui32DataTx[NUM_SSI_DATA];
    uint32_t pui32DataRx[NUM_SSI_DATA];
    uint32_t ui32Index;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    GPIOPinConfigure(GPIO_PD3_SSI2CLK);
    GPIOPinConfigure(GPIO_PD2_SSI2FSS);
    GPIOPinConfigure(GPIO_PD1_SSI2XDAT0);
    GPIOPinConfigure(GPIO_PD0_SSI2XDAT1);

    GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    SSIConfigSetExpClk(SSI2_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 8);
    SSIEnable(SSI2_BASE);
    while(SSIDataGetNonBlocking(SSI2_BASE, &pui32DataRx[0]))
    {
    }
}
void TV_SPI_W(uint8_t uc_SendData)
{
    /* 向SSI FIFO写入数据 */
    SSIDataPut(SSI2_BASE, uc_SendData);
}

void TV_SPI_R(uint8_t uc_RecvData)
{
    while(SSIBusy(SSI2_BASE));
    
}

