/**
 * @file servo.c
 * @author pansamic
 * @brief 
 * @version 0.1
 * @date 2023-07-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <hw_memmap.h>
#include <pwm.h>
#include <servo.h>

/*************************************************************************************
 * @brief Add a servo motor
 * 
 * @param Servo pointer to the servo motor
 * @param PWM_Base PWM base address
 * @param PWM_Out PWM output channel, e.g. PWM_OUT_4, PWM_OUT_5
 * @param Type SERVO_180_DEGREE, SERVO_270_DEGREE, SERVO_360_DEGREE
 *************************************************************************************/
void Add_Servo(Servo_t *Servo, uint32_t PWM_Base, uint32_t PWM_Out, uint8_t Type, float MinAngle, float MaxAngle)
{
    if(Servo==(void*)0)
    {
        printf("pass null pointer when initializing servo motor\r\n");
        return;
    }

    Servo->PWMBase = PWM_Base;
    Servo->PWMOut = PWM_Out;
    Servo->Type = Type;
    Servo->CurrentAngle = 0;
    Servo->TargetAngle = 0;
    Servo->AdjustedAngle = 0;
	switch(Type)
	{
		case SERVO_180_DEGREE:
		{
			if(MinAngle<-90) Servo->MinAngle = -90;
			else Servo->MinAngle = MinAngle;
			if(MaxAngle > 90) Servo->MaxAngle = 90;
			else Servo->MaxAngle = MaxAngle;
			break;
		}
		case SERVO_270_DEGREE:
		{
            if(MinAngle<-135) Servo->MinAngle = -135;
            else Servo->MinAngle = MinAngle;
            if(MaxAngle > 135) Servo->MaxAngle = 135;
            else Servo->MaxAngle = MaxAngle;
			break;
		}
		case SERVO_360_DEGREE:
		{
            if(MinAngle<-180) Servo->MinAngle = -180;
            else Servo->MinAngle = MinAngle;
            if(MaxAngle > 180) Servo->MaxAngle = 180;
            else Servo->MaxAngle = MaxAngle;
			break;
		}
		default:
			break;
	}
}

/*************************************************************************************
 * @brief Set the Servo Angle object
 * 
 * @param Servo pointer to the servo motor
 * @param angle unit:degree
 *************************************************************************************/
void SetServoAngle(Servo_t *Servo, float angle)
{
    if(angle > Servo->MaxAngle)
        angle = Servo->MaxAngle;
    else if(angle < Servo->MinAngle)
        angle = Servo->MinAngle;
    Servo->TargetAngle = angle;
}

/*************************************************************************************
 * @brief adjust servo with resistance to avoid damage when suddenly start.0.025~0.125 0.1 divided by 180/270
 * 
 * @param Servo pointer to the servo motor
 *************************************************************************************/
void AdjustServoAngle(Servo_t *Servo)
{
    Servo->AdjustedAngle = 0.5f * (Servo->TargetAngle - Servo->CurrentAngle);
    Servo->CurrentAngle = Servo->AdjustedAngle;
    uint16_t PulseWidth=0.025f;
    if(SERVO_180_DEGREE){
        PulseWidth = SERVO_PWM_PERIOD*(0.025f+(Servo->AdjustedAngle+90)/180.0f*0.1f);
    }
    else if(SERVO_270_DEGREE){
        PulseWidth = SERVO_PWM_PERIOD*(0.025f+(Servo->AdjustedAngle+135)/270.0f*0.1f);
    }
    else if(SERVO_360_DEGREE){
        PulseWidth = SERVO_PWM_PERIOD*(0.025f+(Servo->AdjustedAngle+180)/360.0f*0.1f);
    }
    PWMPulseWidthSet(Servo->PWMBase, Servo->PWMOut, PulseWidth);
}
