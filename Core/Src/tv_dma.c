/**
 * @file tv_gpio.c
 * @author pansamic (wanggengjie@ieee.org)
 * @brief uDMA initialization and configuration
 * @version 0.1
 * @date 2023-06-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <main.h>
#include <tm4c129_it.h>
#include <tv_dma.h>
#include <sysctl.h>
#include <udma.h>
#include <uart.h>
#include <hw_memmap.h>
#include <hw_ints.h>


uint8_t uDMA_buf[1024] __attribute__ ((aligned(1024)));

void TV_DMA_Init(void)
{
    // Enable uDMA module clock
	SysCtlPeripheralEnable( SYSCTL_PERIPH_UDMA );

	uDMAEnable();

	uDMAControlBaseSet( uDMA_buf );

    // /* UART0 */
	// uDMAChannelControlSet( UDMA_PRI_SELECT | UDMA_CH9_UART0TX , UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE | UDMA_ARB_1 );
	// uDMAChannelControlSet( UDMA_PRI_SELECT | UDMA_CH8_UART0RX , UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 | UDMA_ARB_1 );
	// UARTDMAEnable( UART0_BASE , UART_DMA_TX | UART_DMA_RX );
	// uDMAChannelAssign(UDMA_CH9_UART0TX);
	// uDMAChannelAssign(UDMA_CH8_UART0RX);

	// /* UART1 */
	// uDMAChannelControlSet( UDMA_PRI_SELECT | UDMA_CH23_UART1TX , UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE | UDMA_ARB_1 );
	// uDMAChannelControlSet( UDMA_PRI_SELECT | UDMA_CH22_UART1RX , UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 | UDMA_ARB_1 );
	// UARTDMAEnable( UART1_BASE , UART_DMA_TX | UART_DMA_RX );
	// uDMAChannelAssign(UDMA_CH23_UART1TX);
	// uDMAChannelAssign(UDMA_CH22_UART1RX);

	// /* UART2 */
    // uDMAChannelControlSet( UDMA_PRI_SELECT | UDMA_CH13_UART2TX , UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE | UDMA_ARB_1 );
	// uDMAChannelControlSet( UDMA_PRI_SELECT | UDMA_CH12_UART2RX , UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 | UDMA_ARB_1 );
	// UARTDMAEnable( UART2_BASE , UART_DMA_TX | UART_DMA_RX );	
	// uDMAChannelAssign(UDMA_CH13_UART2TX);	
	// uDMAChannelAssign(UDMA_CH12_UART2RX);

	// /* UART3 */
    // uDMAChannelControlSet( UDMA_PRI_SELECT | UDMA_CH17_UART3TX , UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE | UDMA_ARB_1 );
	// uDMAChannelControlSet( UDMA_PRI_SELECT | UDMA_CH16_UART3RX , UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 | UDMA_ARB_1 );
	// UARTDMAEnable( UART3_BASE , UART_DMA_TX | UART_DMA_RX );	
	// uDMAChannelAssign(UDMA_CH17_UART3TX);
	// uDMAChannelAssign(UDMA_CH16_UART3RX);

	/* UART4 */
    uDMAChannelControlSet( UDMA_PRI_SELECT | UDMA_CH19_UART4TX , UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE | UDMA_ARB_1 );
	// uDMAChannelControlSet( UDMA_PRI_SELECT | UDMA_CH18_UART4RX , UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 | UDMA_ARB_1 );
	UARTDMAEnable( UART4_BASE , UART_DMA_TX);	
	uDMAChannelAssign(UDMA_CH19_UART4TX);
	// uDMAChannelAssign(UDMA_CH18_UART4RX);

	// /* REgister and enable DMA interrupt */
	// uDMAIntRegister(INT_UDMA, uDMA_IRQHandler);
	// IntMasterEnable();

}