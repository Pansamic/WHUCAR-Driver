/**
 * @file tv_uart.c
 * @author pansamic (wanggengjie@ieee.org)
 * @brief UART initialization and customized functions
 * @version 0.1
 * @date 2023-06-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <main.h>
#include <tv_uart.h>
#include <tm4c129_it.h>
#include <pin_map.h>
#include <hw_ints.h>

void TV_UART_Init(void)
{
#if USE_K210_UART
	/* UART0 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0|GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    UARTConfigSetExpClk(UART0_BASE, SYSTEM_FREQUENCY, 115200, UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE);
    UARTIntRegister(UART0_BASE, UART0_IRQHandler);
    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_RX1_8, UART_FIFO_TX1_8);
	UARTIntEnable(UART0_BASE,UART_INT_RX);
    IntPrioritySet(INT_UART0,2<<5);
    IntEnable(INT_UART0);
#endif

#if USE_BLE_UART
	/* UART1 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinConfigure(GPIO_PB0_U1RX);
    UARTConfigSetExpClk(UART1_BASE, SYSTEM_FREQUENCY, 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	UARTIntEnable(UART1_BASE,UART_INT_RX);
    UARTIntRegister(UART1_BASE,UART1_IRQHandler);
    UARTFIFOLevelSet(UART1_BASE, UART_FIFO_RX1_8, UART_FIFO_TX1_8);
    IntPrioritySet(INT_UART1,2<<5);
    IntEnable(INT_UART1);
#endif

#if USE_JETSON_UART
	/* UART2 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinConfigure(GPIO_PA7_U2TX);
    GPIOPinConfigure(GPIO_PA6_U2RX);
    UARTConfigSetExpClk(UART2_BASE, SYSTEM_FREQUENCY, 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	UARTIntEnable(UART2_BASE,UART_INT_RX);
	// UARTFIFOLevelSet(UART2_BASE, UART_FIFO_RX1_8, UART_FIFO_TX1_8);
    UARTFIFODisable(UART2_BASE);
    UARTIntRegister(UART2_BASE,UART2_IRQHandler);
    IntPrioritySet(INT_UART2,2<<5);
    IntEnable(INT_UART2);
#endif

#if USE_OPENMV_UART
	/* UART3 */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4);
    GPIOPinConfigure(GPIO_PA5_U3TX);
    GPIOPinConfigure(GPIO_PA4_U3RX);
    UARTConfigSetExpClk(UART3_BASE, SYSTEM_FREQUENCY, 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	UARTIntEnable(UART3_BASE,UART_INT_RX);
	UARTFIFOLevelSet(UART3_BASE, UART_FIFO_RX1_8, UART_FIFO_TX1_8);
    UARTIntRegister(UART3_BASE,UART3_IRQHandler);
    IntPrioritySet(INT_UART3,2<<5);
    IntEnable(INT_UART3);
#endif

#if USE_USB_UART
	/* UART4 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
	GPIOPinTypeUART(GPIO_PORTK_BASE, GPIO_PIN_1 | GPIO_PIN_0);
    GPIOPinConfigure(GPIO_PK1_U4TX);
    GPIOPinConfigure(GPIO_PK0_U4RX);
    UARTConfigSetExpClk(UART4_BASE, 120000000, 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	UARTIntEnable(UART4_BASE,UART_INT_RX);
	UARTFIFOLevelSet(UART4_BASE, UART_FIFO_RX1_8, UART_FIFO_TX1_8);
    UARTIntRegister(UART4_BASE,UART4_IRQHandler);
    IntPrioritySet(INT_UART4,2<<5);
	IntEnable(INT_UART4);
}
#endif

void UART_Transmit(uint32_t UART_Handle, void *pData, uint32_t Length)
{
    while(Length--)
    {
        UARTCharPut(UART_Handle, *(uint8_t *)pData);
        pData = (uint8_t *)pData + 1;
    }
}