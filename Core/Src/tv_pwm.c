/**
 * @file tv_pwm.c
 * @author pansamic (wanggengjie@ieee.org)
 * @brief 
 * @version 0.1
 * @date 2023-06-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <main.h>
#include <tv_pwm.h>

void TV_PWM_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

	GPIOPinConfigure(GPIO_PF0_M0PWM0);
	GPIOPinConfigure(GPIO_PF1_M0PWM1);
	GPIOPinConfigure(GPIO_PF2_M0PWM2);
	GPIOPinConfigure(GPIO_PF3_M0PWM3);
	GPIOPinConfigure(GPIO_PG0_M0PWM4);
	GPIOPinConfigure(GPIO_PG1_M0PWM5);
	GPIOPinConfigure(GPIO_PK4_M0PWM6);
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	GPIOPinTypePWM(GPIO_PORTK_BASE, GPIO_PIN_4);
	/*
	* system clock 120MHz. 64 division to 1.875MHz=1875000.
	* TB6612 need 10kHz to 20kHz, set peroid count to 200.
	* Servo need 50Hz, set peroid count to 37500.
	* Buzzer need 4kHz. set peroid count to 469.
	* */
	PWMClockSet(PWM0_BASE, PWM_SYSCLK_DIV_64);
	PWMGenConfigure(PWM0_BASE,PWM_GEN_0,PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC); // 2 channel of PWM for Left Motors
	PWMGenConfigure(PWM0_BASE,PWM_GEN_1,PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC); // 2 channel of PWM for Right Motors
	PWMGenConfigure(PWM0_BASE,PWM_GEN_2,PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC); // 2 channel of PWM for Right Motors
	PWMGenConfigure(PWM0_BASE,PWM_GEN_3,PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC); // 1 channel of PWM for Buzzer
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_0,200);
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_1,200);
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_2,37500);
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_3,469);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_0,100);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_1,100);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_2,100);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_3,100);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_4,37500*0.05);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_5,37500*0.05);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_6,469);
	PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	PWMGenEnable(PWM0_BASE, PWM_GEN_1);
	PWMGenEnable(PWM0_BASE, PWM_GEN_2);
	PWMGenEnable(PWM0_BASE, PWM_GEN_3);

    /* Turn on PWM output pins */
	PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT | PWM_OUT_3_BIT | PWM_OUT_4_BIT | PWM_OUT_5_BIT | PWM_OUT_6_BIT, true);
}
