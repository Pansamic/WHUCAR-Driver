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

extern IRS_t IRS_dev;

extern uint8_t detection;
extern uint8_t signal;
uint8_t detectionCd=0;
uint8_t action=0;
uint8_t actionCd=0;

uint8_t running=0;
uint8_t state=0;
uint8_t speed=0;
uint8_t defaultSpeed=0;

/***********************************************************/
/*                 FreeRTOS Tasks Handles                  */
/***********************************************************/
TimerHandle_t TimerHandle_AdjustCar;
TimerHandle_t TimerHandle_UpdateIMU;
TaskHandle_t TaskHandle_CustomApp;
TaskHandle_t TaskHandle_KeyDetect;
TaskHandle_t TaskHandle_LEDBlink;
TaskHandle_t TaskHandle_AdjustServo;

TaskHandle_t TaskHandle_IRS;
TaskHandle_t TaskHandle_OLED;
TaskHandle_t TaskHandle_Controller;



#if USE_JETSON_UART
TaskHandle_t TaskHandle_JetsonioProcess;
#endif
#if USE_TFLUNA_UART
TaskHandle_t TaskHandle_TFLunaProcess;
#endif


/***********************************************************/
/*                 FreeRTOS Tasks Function                 */
/***********************************************************/
void AdjustCar(TimerHandle_t xTimer);
void UpdateIMU(TimerHandle_t xTimer);
void CustomApp(void * argument);
void KeyDetect(void * argument);
void LEDBlink(void * argument);
void AdjustServo(void * argument);

void GetIRS(void * argument);
void OLEDDisplay(void * argument);
void MainControl(void * argument);

#if USE_JETSON_UART
void Jetsonio_Process(void * argument);
#endif
#if USE_TFLUNA_UART
void TFLuna_Process(void * argument);
#endif

void TV_FREERTOS_Init(void)
{
	TimerHandle_AdjustCar = xTimerCreate("Timer_AdjustCar", ENCODER_UPDATE_INTERVAL, pdTRUE, NULL, AdjustCar);
	TimerHandle_UpdateIMU = xTimerCreate("Timer_UpdateIMU", IMU_UPDATE_INTERVAL, pdTRUE, NULL, UpdateIMU);
	//xTaskCreate(CustomApp, "Task_CustomApp", 256, NULL, 2, &TaskHandle_CustomApp);
	xTaskCreate(KeyDetect, "Task_KeyDetect", 128, NULL, 1, &TaskHandle_KeyDetect);
	xTaskCreate(LEDBlink, "Task_LEDBlink", 128, NULL, 1, &TaskHandle_LEDBlink);
	xTaskCreate(AdjustServo, "Task_AdjustServo", 128, NULL, 2, &TaskHandle_AdjustServo);
	xTaskCreate(GetIRS, "Task_GetIRS", 128, NULL, 1, &TaskHandle_IRS);
	xTaskCreate(OLEDDisplay, "Task_OLEDDisplay", 256, NULL, 2, &TaskHandle_OLED);
	
	xTaskCreate(MainControl, "Task_Control", 256, NULL, 2, &TaskHandle_Controller);


#if USE_JETSON_UART
	xTaskCreate(Jetsonio_Process, "Task_JetsonioProcess", 256, NULL, 2, &TaskHandle_JetsonioProcess);
#endif
#if USE_TFLUNA_UART
	xTaskCreate(TFLuna_Process, "Task_TFLunaProcess", 256, NULL, 2, &TaskHandle_TFLunaProcess);
#endif
	portENABLE_INTERRUPTS();
	IntMasterEnable();
	xTimerStart(TimerHandle_AdjustCar, 0);
	xTimerStart(TimerHandle_UpdateIMU, 0);
}
void CustomApp(void * argument)
{
	for(;;)
	{
		vTaskDelay(100);
		// printf("%.2f\r\n", ICM20602_dev.AngleX);
	}
}
void AdjustCar(TimerHandle_t xTimer)
{
	Car_Adjust();	
}
void UpdateIMU(TimerHandle_t xTimer)
{
	configASSERT(xTimer);
	ICM20602_Update();
	//SetServoAngle(&Servo2, -ICM20602_dev.AngleZ);
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
			SetServoAngle(&Servo1, Servo1.TargetAngle+0.5f);
				SetServoAngle(&Servo2, Servo2.TargetAngle+0.5f);
		}
		if(!GPIOPinRead(KEY1_GPIO_Port,	KEY1_Pin))
		{
			while(!GPIOPinRead(KEY1_GPIO_Port,KEY1_Pin))
			{}
			printf("key1 pressed\r\n");
			SetServoAngle(&Servo1, Servo1.TargetAngle-0.5f);
				SetServoAngle(&Servo2, Servo2.TargetAngle-0.5f);
		}
		if(!GPIOPinRead(KEY2_GPIO_Port,	KEY2_Pin))
		{
			//while(!GPIOPinRead(KEY1_GPIO_Port,KEY1_Pin))
			//{}
			printf("key2 pressed\r\n");
			SetServoAngle(&Servo1, Servo1.TargetAngle-0.05f);
			vTaskDelay(50);
		}
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

void AdjustServo(void * argument)
{
	for(;;)
	{
		vTaskDelay(10);
		AdjustServoAngle(&Servo1);
		AdjustServoAngle(&Servo2);
	}
}

void GetIRS(void * argument){
	for(;;){
		vTaskDelay(10);
		IRS_Update();
		
	}
}

void OLEDDisplay(void * argument){
	for(;;){
		vTaskDelay(100);
		OLED_Clear();
		/* Line 1 */
		OLED_ShowString(0,0,"Yaw:");
		OLED_ShowIntNum(32,0,(int)(ICM20602_dev.AngleZ*100),5);
		/* Line 2 */
		OLED_ShowString(0,2,"X:");
		OLED_ShowIntNum(16,2,(int)(Car.CurrentxAxisDistance*10),5);
		OLED_ShowString(64,2,"Y:");
		OLED_ShowIntNum(80,2,(int)(Car.CurrentyAxisDistance*10),5);
		/* Line 3 */
		OLED_ShowString(0,4,"S1:");
		OLED_ShowIntNum(24,4,(int)Servo1.CurrentAngle,4);
		OLED_ShowString(64,4,"S2:");
		OLED_ShowIntNum(88,4,(int)Servo2.CurrentAngle,4);

	}
}

//------------


void Start(){
	running=1;
}
// 0 0 0 0 1 0 0 0 base on IRS
void TrackLine(){
	if(IRS_dev.IRS_data[3]==0&&IRS_dev.IRS_data[5]!=0) ;
	else if(IRS_dev.IRS_data[3]!=0&&IRS_dev.IRS_data[5]==0) ;
	if(IRS_dev.IRS_data[2]==0&&IRS_dev.IRS_data[6]!=0) ;
	else if(IRS_dev.IRS_data[2]!=0&&IRS_dev.IRS_data[6]==0) ;
}

void DoingAction(){
	switch (action)
	{
	case 1:
		Car_SetVelocity(10,10);
		break;
	case 2:
		Car_SetVelocity(-10,10);
		break;
	case 3:
		Car_SetVelocity(10,-10);
		break;
	case 4:
		Car_SetVelocity(0,0);
	default:
		break;
	}
}


//------------

void MainControl(void * argument){
	for(;;){
		vTaskDelay(10);
		if(detectionCd>0) detectionCd--;
		if(actionCd>0) actionCd--;

		if(running==1){
			speed=defaultSpeed;
			if(actionCd>0){
				DoingAction();
			}
			else {
				if(detection==1&&detectionCd==0){// example when state=1,detection occured then set
					if(state==1){
						detectionCd=100;//1 s
						action=1;
						actionCd=100;
					}
					
				}
				Car_SetVelocity(speed+signal,speed-signal);
			}
		}
		else {
			Car_SetVelocity(0,0);
		}
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
