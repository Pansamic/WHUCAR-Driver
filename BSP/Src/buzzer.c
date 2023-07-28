#include "buzzer.h"
#include <tv_pwm.h>
#include <main.h>

void SetBuzzer(uint16_t freq,uint8_t on){
    if(on){
        PWMGenPeriodSet(PWM0_BASE,PWM_GEN_3,1875000/(uint32_t)freq);
        PWMPulseWidthSet(PWM0_BASE,PWM_OUT_6,1875000/(uint32_t)freq/2);
    }
    else {
        PWMGenPeriodSet(PWM0_BASE,PWM_GEN_3,1875000/(uint32_t)freq);
        PWMPulseWidthSet(PWM0_BASE,PWM_OUT_6,1875000/(uint32_t)freq);
    }
}




