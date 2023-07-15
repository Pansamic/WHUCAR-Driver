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
	GPIOPinConfigure(GPIO_PK4_M0PWM6);
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinTypePWM(GPIO_PORTK_BASE, GPIO_PIN_4);
	/*f
	* set to 1KHz PWM wave
	* (120MHZ) / (6000) == 20KHz
	* */
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
	PWMGenConfigure(PWM0_BASE,PWM_GEN_0,PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC); // 2 channel of PWM for Left Motors
	PWMGenConfigure(PWM0_BASE,PWM_GEN_1,PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC); // 2 channel of PWM for Right Motors
	PWMGenConfigure(PWM0_BASE,PWM_GEN_3,PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC); // 1 channel of PWM for Buzzer
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_0,SYSTEM_FREQUENCY/20000);
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_1,SYSTEM_FREQUENCY/20000);
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_3,SYSTEM_FREQUENCY/4000);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_0,0);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_1,0);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_2,0);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_3,0);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_6,0);
	PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	PWMGenEnable(PWM0_BASE, PWM_GEN_1);
	PWMGenEnable(PWM0_BASE, PWM_GEN_3);

    /* Turn on PWM output pins */
	PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT | PWM_OUT_3_BIT | PWM_OUT_6_BIT, true);
}
