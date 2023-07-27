#include <main.h>
#include <tv_timer.h>
#include <tv_gpio.h>
#include <hw_ints.h>
#include <timer.h>
#include <mdp.h>
//*****************************************************************************
//
// The interrupt handler for the second timer interrupt.
//
//*****************************************************************************
#define SERVO_PERIOD 20000//us
#define SERVO_MIN 4//us
//--------0.05ms-50us------//
//--------period 20ms-------//
extern float sg0_angle;
extern float sg1_angle;
extern float sg2_angle;
int16_t T0_flag=0;
void Timer0_Handler(void)
{
    // Clear the timer interrupt.
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterDisable();
    T0_flag++;
	if(T0_flag>=SERVO_PERIOD/SERVO_MIN)//5000
        T0_flag=0;
    // 0.5ms - 2.5ms / 20ms == 2.5 ~ 12.5 / 100//125~625
    if(T0_flag<125+(int)sg0_angle) MDP_GPIO_High(GPIO_PORTQ_BASE,GPIO_PIN_2);
    else MDP_GPIO_Low(GPIO_PORTQ_BASE,GPIO_PIN_2);

    if(T0_flag<125+(int)sg1_angle) MDP_GPIO_High(GPIO_PORTD_BASE,GPIO_PIN_4);
    else MDP_GPIO_Low(GPIO_PORTD_BASE,GPIO_PIN_4);

    if(T0_flag<375+(int)sg2_angle) MDP_GPIO_High(GPIO_PORTE_BASE,GPIO_PIN_4);
    else MDP_GPIO_Low(GPIO_PORTE_BASE,GPIO_PIN_4);
    IntMasterEnable();
}

int16_t flag_T1=0;
void Timer1_Handler(void)
{
    // Clear the timer interrupt.
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterDisable();
    flag_T1++;
	if(flag_T1>=2)
        flag_T1=0;
    if(flag_T1<1){//0 1 2 
        MDP_GPIO_High(LEDG_GPIO_Port,LEDG_Pin);
        //MDP_GPIO_High(LEDR_GPIO_Port,LEDR_Pin);
        //MDP_GPIO_High(LEDB_GPIO_Port,LEDB_Pin);

    }
    else{
        MDP_GPIO_Low(LEDG_GPIO_Port,LEDG_Pin);
        //MDP_GPIO_Low(LEDR_GPIO_Port,LEDR_Pin);
        //MDP_GPIO_Low(LEDB_GPIO_Port,LEDB_Pin);
        //MDP_GPIO_Low(GPIO_PORTD_BASE, GPIO_PIN_4);
    }
    IntMasterEnable();
}
//*****************************************************************************
//
// Init
//
//*****************************************************************************
void TV_TIMER0_Init(uint32_t Clock){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    // Enable processor interrupts.
    IntMasterEnable();
    // Configure the two 32-bit periodic timers.
    TimerConfigure(TIMER0_BASE,  TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, Clock);
    
    // Setup the interrupts for the timer timeouts.
	IntRegister(INT_TIMER0A, Timer0_Handler);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT );
    
    // Enable the timers.
    TimerEnable(TIMER0_BASE, TIMER_A);
}
void TV_TIMER1_Init(uint32_t Clock){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    // Enable processor interrupts.
    IntMasterEnable();
    // Configure the two 32-bit periodic timers.
    TimerConfigure(TIMER1_BASE,  TIMER_CFG_PERIODIC );
    TimerLoadSet(TIMER1_BASE, TIMER_A, Clock);
    // Setup the interrupts for the timer timeouts.
	IntRegister(INT_TIMER1A, Timer1_Handler);
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    // Enable the timers.
    TimerEnable(TIMER1_BASE, TIMER_A);
}
void TV_TIMER3A_PWM_Init(uint32_t Load){//16 bits

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    GPIOPinConfigure(GPIO_PD4_T3CCP0);
    //GPIOPinConfigure(GPIO_PM2_T3CCP0);
    GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_4);
    //GPIOPinTypeTimer(GPIO_PORTM_BASE, GPIO_PIN_2);
    TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
    TimerLoadSet(TIMER3_BASE, TIMER_A, Load);//
    TimerMatchSet(TIMER3_BASE, TIMER_A, 0.075*TimerLoadGet(TIMER3_BASE, TIMER_A));
    TimerEnable(TIMER3_BASE, TIMER_A);
}
void TV_TIMER3A_PWM_SetDuty(float Set){
    TimerMatchSet(TIMER3_BASE, TIMER_A, Set*TimerLoadGet(TIMER3_BASE, TIMER_A));
}
void TV_TIMER3B_PWM_Init(uint32_t Load){//16 bits
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    GPIOPinConfigure(GPIO_PD5_T3CCP1);
    //GPIOPinConfigure(GPIO_PM2_T3CCP0);
    GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_5);
    //GPIOPinTypeTimer(GPIO_PORTM_BASE, GPIO_PIN_2);
    TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM);
    TimerLoadSet(TIMER3_BASE, TIMER_B, Load);//
    TimerMatchSet(TIMER3_BASE, TIMER_B, 0.075*TimerLoadGet(TIMER3_BASE, TIMER_B));
    TimerEnable(TIMER3_BASE, TIMER_B);
}
void TV_TIMER3B_PWM_SetDuty(float Set){
    TimerMatchSet(TIMER3_BASE, TIMER_B, Set*TimerLoadGet(TIMER3_BASE, TIMER_B));
}