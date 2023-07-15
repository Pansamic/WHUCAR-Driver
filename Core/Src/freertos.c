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
#include <icm20602.h>
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
TaskHandle_t Task_AdjustCar;
TaskHandle_t TaskHandle_KeyDetect;
TaskHandle_t TaskHandle_LEDBlink;
TaskHandle_t TaskHandle_K210ioProcess;
TimerHandle_t TimerHandle_UpdateIMU;

void AdjustCar(void * argument);
void KeyDetect(void * argument);
void LEDBlink(void * argument);
void Jetsonio_Process(void * argument);
void UpdateIMU(TimerHandle_t xTimer);

void TV_FREERTOS_Init(void)
{
    xTaskCreate(AdjustCar, "Task_AdjustCar", 512, NULL, 3, &Task_AdjustCar);
	xTaskCreate(KeyDetect, "Task_KeyDetect", 128, NULL, 2, &TaskHandle_KeyDetect);
	xTaskCreate(LEDBlink, "Task_LEDBlink", 128, NULL, 2, &TaskHandle_LEDBlink);
	xTaskCreate(Jetsonio_Process, "Task_JetsonioProcess", 256, NULL, 2, &TaskHandle_K210ioProcess);
	// TimerHandle_UpdateIMU = xTimerCreate("Timer_UpdateIMU", 100, pdTRUE, NULL, UpdateIMU);
	portENABLE_INTERRUPTS();
	IntMasterEnable();
	// xTimerStart(TimerHandle_UpdateIMU, 0);
}

void AdjustCar(void * argument)
{
	TickType_t xLastWakeTime;
	/* delay time must equal to Motor.Encoder.Interval */
	const TickType_t xFrequency = 10;
	xLastWakeTime = xTaskGetTickCount();

  	/* Infinite loop */
  	for(;;)
  	{
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
		Car_Adjust();
//		printf("%.3f\n",LeftFrontMotor.CurrentAngle);
		// AdjustMotor(&LeftFrontMotor);
		// AdjustMotor(&LeftRearMotor);
		// AdjustMotor(&RightFrontMotor);
		// AdjustMotor(&RightRearMotor);
		// printf("%d, %d, %d, %d\r\n",LeftFrontMotor.EncSource, LeftRearMotor.EncSource, RightFrontMotor.EncSource, RightRearMotor.EncSource);
		// printf("%d\r\n",EncSource_LeftFrontMotor);
  	}
}

void KeyDetect(void * argument)
{
	for(;;)
	{
		vTaskDelay(200);
		if(!GPIOPinRead(KEY0_GPIO_Port,	KEY0_Pin))
		{
			while(!GPIOPinRead(KEY0_GPIO_Port,KEY0_Pin))
			{}
			Car_SetDistance(10.0f, 10.0f);
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
void Jetsonio_Process(void * argument)
{
	for(;;)
	{
		vTaskDelay(10);
		_io_InputProcess(&Jetsonio);
		_io_OutputProcess(&Jetsonio);
	}

}
void UpdateIMU(TimerHandle_t xTimer)
{
	configASSERT(xTimer);
	ICM20602_Update();
	printf("%f\r\n", ICM20602_dev.Yaw);
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	UARTCharPut(UART4_BASE, 'F');
}


void vApplicationMallocFailedHook( void )
{
	UARTCharPut(UART4_BASE, 'M');
}

