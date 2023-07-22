/**
 * @file tm4c129_it.c
 * @author pansamic (wanggengjie@ieee.org)
 * @brief interrupt handler file
 * @version 0.1
 * @date 2023-06-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <main.h>
#include <tm4c129_it.h>
#include <tv_dma.h>
#include <tv_uart.h>
#include <tv_i2c.h>
#include <mdp_io.h>
#include <icm20602.h>

extern tI2CMInstance g_sI2CMSimpleInst;

void LFEA_Process(void);
void LFEB_Process(void);
void LREA_Process(void);
void LREB_Process(void);
void RFEA_Process(void);
void RFEB_Process(void);
void RREA_Process(void);
void RREB_Process(void);

void EXTI_GPIOG_IRQHandler(void)
{
	uint32_t IntPins=GPIOIntStatus(GPIO_PORTG_BASE,false);
	if(IntPins&EncA_LeftRear_IntPin)
		LREA_Process();
	if(IntPins&EncB_LeftRear_IntPin)
		LREB_Process();
	if(IntPins&EncA_RightFront_IntPin)
		RFEA_Process();
	if(IntPins&EncB_RightFront_IntPin)
		RFEB_Process();
	if(IntPins&EncA_RightRear_IntPin)
		RREA_Process();
	if(IntPins&EncB_RightRear_IntPin)
		RREB_Process();
}

void EXTI_GPIOQ_IRQHandler(void)
{
	uint32_t IntPins=GPIOIntStatus(GPIO_PORTQ_BASE,false);
	if(IntPins&EncA_LeftFront_IntPin)
		LFEA_Process();
	if(IntPins&EncB_LeftFront_IntPin)
		LFEB_Process();
}

/*
 * @brief:UART0 receive interrupt handler.
 * */
void UART0_IRQHandler(void)
{
	uint32_t IntStatus = UARTIntStatus(UART0_BASE, true);
	uint8_t Char;
	if(IntStatus&UART_INT_RX)
	{
		UARTIntClear(UART0_BASE,UART_INT_RX);
		while(UARTCharsAvail(UART0_BASE))
		{
			Char = UARTCharGetNonBlocking(UART0_BASE);
			UARTCharPutNonBlocking(UART0_BASE,Char);
		}
	}
}

/*
 * @brief:UART1 receive interrupt handler.
 * */
void UART1_IRQHandler(void)
{
	uint32_t IntStatus = UARTIntStatus(UART1_BASE, true);
	uint8_t Char;
	if(IntStatus&UART_INT_RX)
	{
		UARTIntClear(UART1_BASE,UART_INT_RX);
		while(UARTCharsAvail(UART1_BASE))
		{
			Char = UARTCharGetNonBlocking(UART1_BASE);
			UARTCharPutNonBlocking(UART1_BASE,Char);
		}
	}
}

/*
 * @brief:UART2 receive interrupt handler.
 * */
#if USE_JETSON_UART
void UART2_IRQHandler(void)
{
	uint32_t IntStatus = UARTIntStatus(UART2_BASE, true);
	uint8_t Char;
#if USE_JETSON_UART_RECEIVE
	if(IntStatus&UART_INT_RX)
	{
		UARTIntClear(UART2_BASE,UART_INT_RX);
		while(UARTCharsAvail(UART2_BASE))
		{
			Char = UARTCharGetNonBlocking(UART2_BASE);
			_io_InputBufWrite(&Jetsonio, &Char, 1);
		}
	}
#endif
}
#endif
/*
 * @brief:UART3 receive interrupt handler.
 * */
void UART3_IRQHandler(void)
{
	uint32_t IntStatus = UARTIntStatus(UART3_BASE, true);
	uint8_t Char;
	if(IntStatus&UART_INT_RX)
	{
		UARTIntClear(UART3_BASE,UART_INT_RX);
		while(UARTCharsAvail(UART3_BASE))
		{
			Char = UARTCharGetNonBlocking(UART3_BASE);
			UARTCharPutNonBlocking(UART3_BASE,Char);
		}
	}
}

/*
 * @brief:UART4 receive interrupt handler.
 * */
void UART4_IRQHandler(void)
{
	uint32_t IntStatus = UARTIntStatus(UART4_BASE, true);
	uint8_t Char;
	if(IntStatus&UART_INT_RX)
	{
		UARTIntClear(UART4_BASE,UART_INT_RX);
		while(UARTCharsAvail(UART4_BASE))
		{
			Char = UARTCharGet(UART4_BASE);
			UARTCharPut(UART4_BASE,Char);
		}
	}
	if((UARTRxErrorGet(UART4_BASE)&UART_RXERROR_OVERRUN)==UART_RXERROR_OVERRUN)
	{
		UARTRxErrorClear(UART4_BASE);
		UARTCharGetNonBlocking(UART4_BASE);
	}
}

void I2C8_IntHandler(void)
{
	// I2CMIntHandler(&ICM20602_dev.I2CInstance);
	I2CMIntHandler(&I2C8Inst);
}

/*************************************************************************************/
/*                             Auxilary Function                                     */
/*************************************************************************************/

/*
 * @brief:GPIO external interrupt handler for left front motor encoder phase A.
 * */
void LFEA_Process(void)
{
	GPIOIntClear(EncA_LeftFront_GPIO_Port, EncA_LeftFront_IntPin);
	uint8_t LeftFrontEncAState = ((EncA_LeftFront_Pin & GPIOPinRead(EncA_LeftFront_GPIO_Port, EncA_LeftFront_Pin))!=0);
	uint8_t LeftFrontEncBState = ((EncB_LeftFront_Pin & GPIOPinRead(EncB_LeftFront_GPIO_Port, EncB_LeftFront_Pin))!=0);
	
	if(LeftFrontEncAState ^ LeftFrontEncBState) // phase A and phase B are not in the same state
	{
		LeftFrontMotor.EncSource--;
	}
	else // phase A and phase B are in the same state
	{
		LeftFrontMotor.EncSource++;
	}
}

/*
 * @brief:GPIO external interrupt handler for left front motor encoder phase B.
 * */
void LFEB_Process(void)
{
	GPIOIntClear(EncB_LeftFront_GPIO_Port, EncB_LeftFront_IntPin);
	uint8_t LeftFrontEncAState = ((EncA_LeftFront_Pin & GPIOPinRead(EncA_LeftFront_GPIO_Port, EncA_LeftFront_Pin))!=0);
	uint8_t LeftFrontEncBState = ((EncB_LeftFront_Pin & GPIOPinRead(EncB_LeftFront_GPIO_Port, EncB_LeftFront_Pin))!=0);
	
	if(LeftFrontEncAState ^ LeftFrontEncBState) // phase A and phase B are not in the same state
	{
		LeftFrontMotor.EncSource++;
	}
	else // phase A and phase B are in the same state
	{
		LeftFrontMotor.EncSource--;
	}
}

/*
 * @brief:GPIO external interrupt handler for left rear motor encoder phase A.
 * */
void LREA_Process(void)
{
	GPIOIntClear(EncA_LeftRear_GPIO_Port, EncA_LeftRear_IntPin);
	uint8_t LeftRearEncAState = ((EncA_LeftRear_Pin & GPIOPinRead(EncA_LeftRear_GPIO_Port, EncA_LeftRear_Pin))!=0);
	uint8_t LeftRearEncBState = ((EncB_LeftRear_Pin & GPIOPinRead(EncB_LeftRear_GPIO_Port, EncB_LeftRear_Pin))!=0);
	
	if(LeftRearEncAState ^ LeftRearEncBState) // phase A and phase B are not in the same state
	{
		LeftRearMotor.EncSource--;
	}
	else // phase A and phase B are in the same state
	{
		LeftRearMotor.EncSource++;
	}
}

/*
 * @brief:GPIO external interrupt handler for left rear motor encoder phase B.
 * */
void LREB_Process(void)
{
	GPIOIntClear(EncB_LeftRear_GPIO_Port, EncB_LeftRear_IntPin);
	uint8_t LeftRearEncAState = ((EncA_LeftRear_Pin & GPIOPinRead(EncA_LeftRear_GPIO_Port, EncA_LeftRear_Pin))!=0);
	uint8_t LeftRearEncBState = ((EncB_LeftRear_Pin & GPIOPinRead(EncB_LeftRear_GPIO_Port, EncB_LeftRear_Pin))!=0);

	if(LeftRearEncAState ^ LeftRearEncBState) // phase A and phase B are not in the same state
	{
		LeftRearMotor.EncSource++;
	}
	else // phase A and phase B are in the same state
	{
		LeftRearMotor.EncSource--;
	}
}

/*
 * @brief:GPIO external interrupt handler for right front motor encoder phase A.
 * */
void RFEA_Process(void)
{
	GPIOIntClear(EncA_RightFront_GPIO_Port, EncA_RightFront_IntPin);
	uint8_t RightFrontEncAState = ((EncA_RightFront_Pin & GPIOPinRead(EncA_RightFront_GPIO_Port, EncA_RightFront_Pin))!=0);
	uint8_t RightFrontEncBState = ((EncB_RightFront_Pin & GPIOPinRead(EncB_RightFront_GPIO_Port, EncB_RightFront_Pin))!=0);

	if(RightFrontEncAState ^ RightFrontEncBState) // phase A and phase B are not in the same state
	{
		RightFrontMotor.EncSource--;
	}
	else // phase A and phase B are in the same state
	{
		RightFrontMotor.EncSource++;
	}
}

/*
 * @brief:GPIO external interrupt handler for right front motor encoder phase B.
 * */
void RFEB_Process(void)
{
	GPIOIntClear(EncB_RightFront_GPIO_Port, EncB_RightFront_IntPin);
	uint8_t RightFrontEncAState = ((EncA_RightFront_Pin & GPIOPinRead(EncA_RightFront_GPIO_Port, EncA_RightFront_Pin))!=0);
	uint8_t RightFrontEncBState = ((EncB_RightFront_Pin & GPIOPinRead(EncB_RightFront_GPIO_Port, EncB_RightFront_Pin))!=0);
	
	if(RightFrontEncAState ^ RightFrontEncBState) // phase A and phase B are not in the same state
	{
		RightFrontMotor.EncSource++;
	}
	else // phase A and phase B are in the same state
	{
		RightFrontMotor.EncSource--;
	}
}

/*
 * @brief:GPIO external interrupt handler for right rear motor encoder phase A.
 * */
void RREA_Process(void)
{
	GPIOIntClear(EncA_RightRear_GPIO_Port, EncA_RightRear_IntPin);
	uint8_t RightRearEncAState = ((EncA_RightRear_Pin & GPIOPinRead(EncA_RightRear_GPIO_Port, EncA_RightRear_Pin))!=0);
	uint8_t RightRearEncBState = ((EncB_RightRear_Pin & GPIOPinRead(EncB_RightRear_GPIO_Port, EncB_RightRear_Pin))!=0);

	if(RightRearEncAState ^ RightRearEncBState) // phase A and phase B are not in the same state
	{
		RightRearMotor.EncSource--;
	}
	else // phase A and phase B are in the same state
	{
		RightRearMotor.EncSource++;
	}
}

/*
 * @brief:GPIO external interrupt handler for right rear motor encoder phase B.
 * */
void RREB_Process(void)
{
	GPIOIntClear(EncB_RightRear_GPIO_Port, EncB_RightRear_IntPin);
	uint8_t RightRearEncAState = ((EncA_RightRear_Pin & GPIOPinRead(EncA_RightRear_GPIO_Port, EncA_RightRear_Pin))!=0);
	uint8_t RightRearEncBState = ((EncB_RightRear_Pin & GPIOPinRead(EncB_RightRear_GPIO_Port, EncB_RightRear_Pin))!=0);
	
	if(RightRearEncAState ^ RightRearEncBState) // phase A and phase B are not in the same state
	{
		RightRearMotor.EncSource++;
	}
	else // phase A and phase B are in the same state
	{
		RightRearMotor.EncSource--;
	}
}

