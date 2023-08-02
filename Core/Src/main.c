/**
 * @file main.c
 * @author pansamic (wanggengjie@ieee.org)
 * @brief Car driver system main file
 * @version 0.1
 * @date 2023-06-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <string.h>
#include <hw_ints.h>
#include <main.h>
#include <tv_gpio.h>
#include <tv_uart.h>
#include <tv_i2c.h>
#include <tv_pwm.h>
#include <tv_dma.h>	
#include <tv_adc.h>
#include <icm20602.h>
#include <irs.h>
#include <FreeRTOS.h>
#include <task.h>
#include <mdp_io.h>
#include <i2cm_drv.h>
#include <oled.h>
#include <buzzer.h>

#include <feetchh/SCServo.h>

DCMotor LeftFrontMotor = {0};
DCMotor LeftRearMotor = {0};
DCMotor RightFrontMotor = {0};
DCMotor RightRearMotor = {0};
Car_t Car = {0};
ICM20602_t ICM20602_dev = {0};
IRS_t IRS_dev = {0};

Servo_t Servo1={0};
Servo_t Servo2={0};
/* trigger [0]:camera */
int key_index=0;
int signal=0;

#if USE_JETSON_UART
#define JETSON_OUTPUTBUF1_SIZE 64
#define JETSON_OUTPUTBUF2_SIZE 64
#define JETSON_INTPUTBUF_SIZE 256
uint8_t Jetsonio_OutputBuf1[JETSON_OUTPUTBUF1_SIZE];
uint8_t Jetsonio_OutputBuf2[JETSON_OUTPUTBUF2_SIZE];
uint8_t Jetsonio_InputBuf[JETSON_INTPUTBUF_SIZE];
uint8_t JetsonPackageContainer[16];
MDP_io Jetsonio;
const uint8_t JetsonPkgHead[2]={0xC8,0xFF};
const MDP_PkgFmt JetsonPkgFmt=
{
    1,               // enable package head
	JetsonPkgHead,   // use Jetson head sequence
    2,               // head sequence length equals two
    0,               // disable data check
    NULL,            // don't use check algorithm
    0,               // disable tail
    NULL,            // use null tail sequence
    0,               // tail sequence length equals 0
    0                // tail after check value. useless field, cuz check is disabled.
};
void Jetson_PackageProcess(MDP_io* ioDevice, uint8_t *PkgDst);
#endif 

void TV_FREERTOS_Init(void);

/******************************************************/
/******************************************************/
/*                    MAIN FUNCTION                   */
/******************************************************/
/******************************************************/
int main( void )
{
    /* SYSTEM CLOCK INIT 120MHZ */
    SysCtlClockFreqSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_CFG_VCO_240,SYSTEM_FREQUENCY);
	IntPriorityGroupingSet(3);
	FPULazyStackingEnable();
    /* PERIPHERAL INIT */
    TV_GPIO_Init();
    TV_UART_Init();
    TV_I2C_Init();
	TV_PWM_Init();
	TV_ADC0_Init();
    OLED_Init();
    OLED_ShowString(0,0,"Initializing...");

    SetBuzzer(600,1);
    SysCtlDelay(120000000);
    SetBuzzer(600,0);

	TV_FREERTOS_Init();

    vTaskStartScheduler();
    while(1)
    {
        
    }
}




/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    UARTCharPut(USB_UART, ch);
    return ch;
}

