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
#include <tv_uart.h>
TimerHandle_t TimerHandle_AdjustCar;
TaskHandle_t TaskHandle_KeyDetect;
TaskHandle_t TaskHandle_LEDBlink;
#if USE_JETSON_UART
TaskHandle_t TaskHandle_JetsonioProcess;
#endif
TimerHandle_t TimerHandle_UpdateIMU;

void AdjustCar(TimerHandle_t xTimer);
void KeyDetect(void * argument);
void LEDBlink(void * argument);
#if USE_JETSON_UART
void Jetsonio_Process(void * argument);
#endif
void UpdateIMU(TimerHandle_t xTimer);

void TV_FREERTOS_Init(void)
{
	TimerHandle_AdjustCar = xTimerCreate("Timer_AdjustCar", ENCODER_UPDATE_INTERVAL, pdTRUE, NULL, AdjustCar);
	xTaskCreate(KeyDetect, "Task_KeyDetect", 128, NULL, 2, &TaskHandle_KeyDetect);
	xTaskCreate(LEDBlink, "Task_LEDBlink", 128, NULL, 2, &TaskHandle_LEDBlink);
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
	// printf("X:%.4f | Y:%.4f\r\n", Car.CurrentxAxisVelocity, Car.CurrentyAxisVelocity);
	// printf("X:%.2fcm | Y:%.2fcm | Angle:%.2frad\r\n", Car.CurrentxAxisDistance, Car.CurrentyAxisDistance, Car.CurrentYaw);
	// printf("%.3f | %.3f\r\n", LeftFrontMotor.CurrentAngle, LeftFrontMotor.CurrentVelocity);

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
			Car_SetDistance(10.0f, 80.0f);
		}
		if(!GPIOPinRead(KEY1_GPIO_Port,	KEY1_Pin))
		{
			while(!GPIOPinRead(KEY1_GPIO_Port,KEY1_Pin))
			{}
			Car_SetVelocity(5.0f, 5.0f);
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
	printf("%.5f,%.5f,%.5f\n", ICM20602_dev.AngleX, ICM20602_dev.AngleY, ICM20602_dev.AngleZ);
	// printf("%.5f,%.5f,%.5f\n", ICM20602_dev.Ax, ICM20602_dev.Ay, ICM20602_dev.Az);
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	UARTCharPut(UART4_BASE, 'F');
}


void vApplicationMallocFailedHook( void )
{
	UARTCharPut(UART4_BASE, 'M');
}

