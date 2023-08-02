/**
 * @file freertos.c
 * @author pansamic (wanggengjie@ieee.org)
 * @brief FreeRTOS tasks
 * @version 0.1
 * @date 2023-06-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <main.h>
#include <tv_uart.h>
#include <tv_gpio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <icm20602.h>
#include <irs.h>
#include <servo.h>
#include <oled.h>

extern int key_index;

/***********************************************************/
/*                 FreeRTOS Tasks Handles                  */
/***********************************************************/
TaskHandle_t TaskHandle_CustomApp;
TaskHandle_t TaskHandle_KeyDetect;
TaskHandle_t TaskHandle_LEDBlink;
TaskHandle_t TaskHandle_OLED;

/***********************************************************/
/*                 FreeRTOS Tasks Function                 */
/***********************************************************/
void KeyDetect(void * argument);
void LEDBlink(void * argument);
void OLEDDisplay(void * argument);

void TV_FREERTOS_Init(void)
{
	xTaskCreate(KeyDetect, "Task_KeyDetect", 128, NULL, 1, &TaskHandle_KeyDetect);
	xTaskCreate(LEDBlink, "Task_LEDBlink", 128, NULL, 1, &TaskHandle_LEDBlink);
	xTaskCreate(OLEDDisplay, "Task_OLEDDisplay", 256, NULL, 2, &TaskHandle_OLED);
	portENABLE_INTERRUPTS();
	IntMasterEnable();
}



void GetKeyBoard()
{
	for(int i=0;i<4;++i){
		SysCtlDelay(120000);
		GPIOPinWrite(KEYBOARD_OutPort,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4);
		GPIOPinWrite(KEYBOARD_OutPort,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4,~GPIO_PIN_1<<i);
		for(int j=0;j<4;++j){
			if(GPIOPinRead(KEYBOARD_InPort,GPIO_PIN_1<<j)==0){
				while (GPIOPinRead(KEYBOARD_InPort,GPIO_PIN_1<<j)==0)
				{}
				key_index=i*4+j+1;
			}
		}
	}
}

void KeyDetect(void * argument)
{
	for(;;)
	{
		vTaskDelay(30);
		if(!GPIOPinRead(KEY0_GPIO_Port,	KEY0_Pin))
		{
			while(!GPIOPinRead(KEY0_GPIO_Port,KEY0_Pin))
			{}
			key_index=0;
			printf("0");
		}
		if(!GPIOPinRead(KEY1_GPIO_Port,	KEY1_Pin))
		{
			while(!GPIOPinRead(KEY1_GPIO_Port,KEY1_Pin))
			{}
			key_index=1;
			printf("1");
		}
		if(!GPIOPinRead(KEY2_GPIO_Port,	KEY2_Pin))
		{
			while(!GPIOPinRead(KEY1_GPIO_Port,KEY1_Pin))
			{}
			key_index=2;
			printf("2");
		}
		GetKeyBoard();
	}
}

void LEDBlink(void * argument)
{
	for(;;)
	{
		SetLED(0,1,0);
		vTaskDelay(500);
		SetLED(0,0,0);
		vTaskDelay(500);
	}
}
extern int signal;
void OLEDDisplay(void * argument){
	for(;;){
		vTaskDelay(100);
		OLED_Clear();
		/* Line 1 */
		OLED_ShowString(0,0,"Yaw:");
		OLED_ShowIntNum(32,0,(int)(ICM20602_dev.AngleZ*100),5);
		/* Line 2 */
		OLED_ShowString(0,2,"Rec:");
		OLED_ShowIntNum(32,2,(int)signal,4);
		/* Line 3 */
		OLED_ShowString(0,4,"Key:");
		OLED_ShowIntNum(32,4,(int)key_index,4);


	}
}

//------------

#if USE_JETSON_UART
void Jetsonio_Process(void * argument)
{
	for(;;)
	{
		vTaskDelay(10);
		_io_InputProcess(&Jetsonio);
		_io_OutputProcess(&Jetsonio);
	}
}
#endif

#if USE_TFLUNA_UART
void TFLuna_Process(void * argument)
{
	for(;;)
	{
		vTaskDelay(20);
		_io_InputProcess(&tfluna.ioDevice);
		_io_OutputProcess(&tfluna.ioDevice);
	}
}
#endif


void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	UARTCharPut(UART4_BASE, 'F');
}

void vApplicationMallocFailedHook( void )
{
	UARTCharPut(UART4_BASE, 'M');
}
