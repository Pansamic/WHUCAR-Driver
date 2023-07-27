/**
 * @file servo.h
 * @author pansamic
 * @brief servo motor control
 * @version 0.1
 * @date 2023-07-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __SERVO_H
#define __SERVO_H

#include <motor.h>

#define SERVO_180_DEGREE 1
#define SERVO_270_DEGREE 2
#define SERVO_360_DEGREE 3
#define SERVO_PWM_PERIOD 37500

/**
 * @brief Servo motor type definition
 * @member Type: SERVO_180_DEGREE, SERVO_270_DEGREE, SERVO_360_DEGREE
 * @member CurrentAngle: current angle of the servo motor
 * @member TargetAngle: target angle of the servo motor
 * @member AdjustedAngle: adjusted angle of the servo motor
 */
typedef struct ServoTypeDef
{
    uint8_t Type;
    uint32_t PWMBase;
    uint32_t PWMOut;
	float MinAngle;
	float MaxAngle;
    float CurrentAngle;
    float TargetAngle;
    float AdjustedAngle;
}Servo_t;

extern Servo_t Servo1;
extern Servo_t Servo2;

void Add_Servo(Servo_t *Servo, uint32_t PWM_Base, uint32_t PWM_Out, uint8_t Type, float MinAngle, float MaxAngle);
void SetServoAngle(Servo_t *Servo, float angle);
void AdjustServoAngle(Servo_t *Servo);

#endif
