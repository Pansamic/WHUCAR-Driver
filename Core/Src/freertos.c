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
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <icm20602.h>
#include <servo.h>




TimerHandle_t TimerHandle_AdjustCar;
TimerHandle_t TimerHandle_UpdateIMU;
TaskHandle_t TaskHandle_KeyDetect;
TaskHandle_t TaskHandle_LEDBlink;
TaskHandle_t TaskHandle_AdjustServo;
#if USE_JETSON_UART
TaskHandle_t TaskHandle_JetsonioProcess;
#endif


void AdjustCar(TimerHandle_t xTimer);
void UpdateIMU(TimerHandle_t xTimer);
void KeyDetect(void * argument);
void LEDBlink(void * argument);
void AdjustServo(void * argument);
#if USE_JETSON_UART
void Jetsonio_Process(void * argument);
#endif


void TV_FREERTOS_Init(void)
{
	TimerHandle_AdjustCar = xTimerCreate("Timer_AdjustCar", ENCODER_UPDATE_INTERVAL, pdTRUE, NULL, AdjustCar);
	xTaskCreate(KeyDetect, "Task_KeyDetect", 128, NULL, 2, &TaskHandle_KeyDetect);
	xTaskCreate(LEDBlink, "Task_LEDBlink", 128, NULL, 2, &TaskHandle_LEDBlink);
	xTaskCreate(AdjustServo, "Task_AdjustServo", 128, NULL, 2, &TaskHandle_AdjustServo);
#if USE_JETSON_UART
	xTaskCreate(Jetsonio_Process, "Task_JetsonioProcess", 256, NULL, 2, &TaskHandle_JetsonioProcess);
#endif
	TimerHandle_UpdateIMU = xTimerCreate("Timer_UpdateIMU", IMU_UPDATE_INTERVAL, pdTRUE, NULL, UpdateIMU);
	portENABLE_INTERRUPTS();
	IntMasterEnable();
	xTimerStart(TimerHandle_AdjustCar, 0);
	xTimerStart(TimerHandle_UpdateIMU, 0);
}

void AdjustCar(TimerHandle_t xTimer)
{
	Car_Adjust();
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
			printf("key0 pressed\r\n");
			SetServoAngle(&Servo1, Servo1.TargetAngle+5.0f);
		}
		if(!GPIOPinRead(KEY1_GPIO_Port,	KEY1_Pin))
		{
			while(!GPIOPinRead(KEY1_GPIO_Port,KEY1_Pin))
			{}
			printf("key1 pressed\r\n");
			SetServoAngle(&Servo1, Servo1.TargetAngle-5.0f);
		}
	}
}

void LEDBlink(void * argument)
{
	for(;;)
	{
		GPIOPinWrite(LEDR_GPIO_Port,LEDR_Pin,~LEDR_Pin);
		vTaskDelay(500);
		GPIOPinWrite(LEDR_GPIO_Port,LEDR_Pin,LEDR_Pin);
		vTaskDelay(500);
	}
}

void AdjustServo(void * argument)
{
	for(;;)
	{
		vTaskDelay(10);
		AdjustServoAngle(&Servo1);
		AdjustServoAngle(&Servo2);
	}
}
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
void UpdateIMU(TimerHandle_t xTimer)
{
	configASSERT(xTimer);
	ICM20602_Update();
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	UARTCharPut(UART4_BASE, 'F');
}


void vApplicationMallocFailedHook( void )
{
	UARTCharPut(UART4_BASE, 'M');
}

