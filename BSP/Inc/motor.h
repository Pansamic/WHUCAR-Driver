/**
 * @file motor.h
 * @author pansamic
 * @brief 
 * @version 0.1
 * @date 2023-07-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __MOTOR_H
#define __MOTOR_H
#include <stdint.h>

#define VELOCITY_CONTROL             1
#define POSITION_CONTROL             2
#define DCMOTOR_REVERSE_DIRECTION    1
#define DCMOTOR_DEFAULT_DIRECTION    0
/* unit : clock cycle */
#define MOTOR_PWM_PERIOD             6000 
#define MOTOR_MAX_VELOCITY           32.0f
#define PULSE_PER_ROUND              1560
#define ENCODER_UPDATE_INTERVAL      10 // unit:ms

#define ENCODER_REMAIN_TIMERCNT      (0)
#define ENCODER_CLEAR_TIMERCNT       (1)

#define ENCODER_REVERSE              (-1)
#define ENCODER_ADVANCE              (1)



typedef struct PIDTypeDef
{
	float Kp;
	float Ki;
	float Kd;
	float CurrentVelocityBias;
	float LastVelocityBias;
	float BeforeLastVelocityBias;
	float InputVal;
	float OutputVal;
	float FeedBack;

}PID_t;

typedef struct DCMotortypeDef
{
	PID_t     VelocityController;
	float     CurrentVelocity;
	float     TargetVelocity;
	float     AdjustedVelocity;

	PID_t     PositionController;
	float     CurrentAngle;
	float     TargetAngle;
	float     AdjustedAngle;

	uint16_t  EncSource;
	short     CurrentCount;

	uint8_t   RotateDirectionReverse;
	uint8_t   RunningMode;

	uint32_t  RotationControlGPIOPin1;
	uint32_t  RotationControlGPIOPort1;
	uint32_t  RotationControlGPIOPin2;
	uint32_t  RotationControlGPIOPort2;
	uint32_t  PWMGeneratingTimer;
	uint32_t  PWMGeneratingTimerChannel;
}DCMotor;


void        DCMotor_Init                (DCMotor *Motor, uint8_t RotateDirectionReverse, uint32_t PWM_Module, uint32_t PWM_Channel, uint32_t LogicPort1, uint32_t LogicPin1, uint32_t LogicPort2, uint32_t LogicPin2);
void        PID_Init                    (PID_t *PID_instance, float Kp, float Ki, float Kd);
void        PID_Update                  (PID_t *PID_instance, float Input, float Feedback);
void        SetMotorVelocity            (DCMotor *Motor, float Velocity);
void        SetMotorAngle               (DCMotor *Motor, float Angle);
void        AdjustMotor                 (DCMotor *Motor);
void        AdjustMotorVelocity         (DCMotor *Motor);
void        AdjustMotorAngle            (DCMotor *Motor);
void        SetMotorPWM                 (DCMotor *Motor, int32_t TimerCounterLoadVal);


#endif
