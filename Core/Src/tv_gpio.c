/**
 * @file tv_gpio.c
 * @author pansamic (wanggengjie@ieee.org)
 * @brief GPIO initialization and customize functions
 * @version 0.1
 * @date 2023-06-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <main.h>
#include <hw_gpio.h>
#include <hw_types.h>
#include <hw_ints.h>
#include <interrupt.h>
#include <tm4c129_it.h>
#include <tv_gpio.h>

void TV_GPIO_Init(void)
{
    /* GPIO port enable */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
	
	
    /* GPIO mode configuration */
    GPIOPinTypeGPIOOutput(LEDR_GPIO_Port, LEDR_Pin);
	GPIOPadConfigSet(LEDR_GPIO_Port, LEDR_Pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    GPIOPinTypeGPIOOutput(LEDG_GPIO_Port, LEDG_Pin);
	GPIOPadConfigSet(LEDG_GPIO_Port, LEDG_Pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    GPIOPinTypeGPIOOutput(LEDB_GPIO_Port, LEDB_Pin);
	GPIOPadConfigSet(LEDB_GPIO_Port, LEDB_Pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);

    GPIOPinTypeGPIOOutput(LFMotor_RotateDirectionCtrl1_GPIO_Port, LFMotor_RotateDirectionCtrl1_Pin);
    GPIOPinTypeGPIOOutput(LFMotor_RotateDirectionCtrl2_GPIO_Port, LFMotor_RotateDirectionCtrl2_Pin);
    GPIOPinTypeGPIOOutput(LRMotor_RotateDirectionCtrl1_GPIO_Port, LRMotor_RotateDirectionCtrl1_Pin);
    GPIOPinTypeGPIOOutput(LRMotor_RotateDirectionCtrl2_GPIO_Port, LRMotor_RotateDirectionCtrl2_Pin);
    GPIOPinTypeGPIOOutput(RFMotor_RotateDirectionCtrl1_GPIO_Port, RFMotor_RotateDirectionCtrl1_Pin);
    GPIOPinTypeGPIOOutput(RFMotor_RotateDirectionCtrl2_GPIO_Port, RFMotor_RotateDirectionCtrl2_Pin);
    GPIOPinTypeGPIOOutput(RRMotor_RotateDirectionCtrl1_GPIO_Port, RRMotor_RotateDirectionCtrl1_Pin);
    GPIOPinTypeGPIOOutput(RRMotor_RotateDirectionCtrl2_GPIO_Port, RRMotor_RotateDirectionCtrl2_Pin);

    GPIOPinTypeGPIOInput(EncA_LeftFront_GPIO_Port, EncA_LeftFront_Pin);
    GPIOPinTypeGPIOInput(EncB_LeftFront_GPIO_Port, EncB_LeftFront_Pin);
    GPIOPinTypeGPIOInput(EncA_LeftRear_GPIO_Port, EncA_LeftRear_Pin);
    GPIOPinTypeGPIOInput(EncB_LeftRear_GPIO_Port, EncB_LeftRear_Pin);
    GPIOPinTypeGPIOInput(EncA_RightFront_GPIO_Port, EncA_RightFront_Pin);
    GPIOPinTypeGPIOInput(EncB_RightFront_GPIO_Port, EncB_RightFront_Pin);
    GPIOPinTypeGPIOInput(EncA_RightRear_GPIO_Port, EncA_RightRear_Pin);
    GPIOPinTypeGPIOInput(EncB_RightRear_GPIO_Port, EncB_RightRear_Pin);

    GPIOPinTypeGPIOInput(KEY0_GPIO_Port, KEY0_Pin);
    GPIOPinTypeGPIOInput(KEY1_GPIO_Port, KEY1_Pin);
    GPIOPinTypeGPIOInput(KEY2_GPIO_Port, KEY2_Pin);

    GPIOPinTypeGPIOOutput(OLED_DC_GPIO_Port, OLED_DC_Pin);
    GPIOPinTypeGPIOOutput(OLED_RST_GPIO_Port, OLED_RST_Pin);
    GPIOPinTypeGPIOOutput(OLED_SCLK_GPIO_Port, OLED_SCLK_Pin);
    GPIOPinTypeGPIOOutput(OLED_MOSI_GPIO_Port, OLED_MOSI_Pin);

    /* GPIO interrupt configuration */
    GPIOIntRegister(GPIO_PORTG_BASE, EXTI_GPIOG_IRQHandler);
    IntPrioritySet(INT_GPIOG, 3<<5);
    GPIOIntRegister(GPIO_PORTQ_BASE, EXTI_GPIOQ_IRQHandler);
    IntPrioritySet(INT_GPIOQ5, 3<<5);
    IntPrioritySet(INT_GPIOQ6, 3<<5);


    GPIOIntEnable(EncA_LeftFront_GPIO_Port, EncA_LeftFront_IntPin);
    GPIOIntTypeSet(EncA_LeftFront_GPIO_Port, EncA_LeftFront_Pin, GPIO_BOTH_EDGES);

    GPIOIntEnable(EncB_LeftFront_GPIO_Port, EncB_LeftFront_IntPin);
    GPIOIntTypeSet(EncB_LeftFront_GPIO_Port, EncB_LeftFront_Pin, GPIO_BOTH_EDGES);

    GPIOIntEnable(EncA_LeftRear_GPIO_Port, EncA_LeftRear_IntPin);
    GPIOIntTypeSet(EncA_LeftRear_GPIO_Port, EncA_LeftRear_Pin, GPIO_BOTH_EDGES);

    GPIOIntEnable(EncB_LeftRear_GPIO_Port, EncB_LeftRear_IntPin);
    GPIOIntTypeSet(EncB_LeftRear_GPIO_Port, EncB_LeftRear_Pin, GPIO_BOTH_EDGES);

    GPIOIntEnable(EncA_RightFront_GPIO_Port, EncA_RightFront_IntPin);
    GPIOIntTypeSet(EncA_RightFront_GPIO_Port, EncA_RightFront_Pin, GPIO_BOTH_EDGES);

    GPIOIntEnable(EncB_RightFront_GPIO_Port, EncB_RightFront_IntPin);
    GPIOIntTypeSet(EncB_RightFront_GPIO_Port, EncB_RightFront_Pin, GPIO_BOTH_EDGES);

    GPIOIntEnable(EncA_RightRear_GPIO_Port, EncA_RightRear_IntPin);
    GPIOIntTypeSet(EncA_RightRear_GPIO_Port, EncA_RightRear_Pin, GPIO_BOTH_EDGES);
 
    GPIOIntEnable(EncB_RightRear_GPIO_Port, EncB_RightRear_IntPin);
    GPIOIntTypeSet(EncB_RightRear_GPIO_Port, EncB_RightRear_Pin, GPIO_BOTH_EDGES);


}

