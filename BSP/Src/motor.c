/**
 * @file motor.c
 * @author pansamic
 * @brief 
 * @version 0.1
 * @date 2023-07-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <motor.h>
#include <pwm.h>
#include <gpio.h>
/****************************************************
 * @brief: This is a macro to transform realistic
 *     speed to pwm generating timer's counter compare
 *     value.
 * */
#define AngVel2TimPWMCnt(VelocityVal) ((VelocityVal)/(MOTOR_MAX_VELOCITY))*(MOTOR_PWM_PERIOD)
#define FittingFunc_1(x) x
#define FittingFunc_2(x) (8*atan(x))

void MotorVelocityControl    (DCMotor *Motor);
void MotorAngleControl       (DCMotor *Motor);
void SetMotorRotateForward   (DCMotor *Motor);
void SetMotorRotateBackward  (DCMotor *Motor);
void StopMotor               (DCMotor *Motor);

void DCMotor_Init(DCMotor *Motor, uint8_t RotateDirectionReverse, uint32_t PWM_Module, uint32_t PWM_Channel, uint32_t LogicPort1, uint32_t LogicPin1, uint32_t LogicPort2, uint32_t LogicPin2)
{
	Motor->RotateDirectionReverse = RotateDirectionReverse;

	/* Set all Velocity related variables to zero, in avoid of unspecified */
    PID_Init(&(Motor->VelocityController), 0.8f, 0.03f, 0.0f);
	Motor->AdjustedVelocity = 0.0f;
	Motor->CurrentVelocity = 0.0f;
	Motor->TargetVelocity = 0.0f;

    /* Set all angle related variables to zero, in avoid of unspecified */
    PID_Init(&(Motor->PositionController), 0.3f, 0.0f, 0.03f);
	Motor->AdjustedAngle = 0.0f;
	Motor->CurrentAngle = 0.0f;
	Motor->TargetAngle = 0.0f;

    Motor->EncSource = 0;
    Motor->CurrentCount = 0;

    Motor->RotationControlGPIOPort1 = LogicPort1;
    Motor->RotationControlGPIOPin1 =  LogicPin1;
    Motor->RotationControlGPIOPort2 = LogicPort2;
    Motor->RotationControlGPIOPin2 =  LogicPin2;

    Motor->PWMGeneratingTimer = PWM_Module;
    Motor->PWMGeneratingTimerChannel = PWM_Channel;

	Motor->RunningMode = VELOCITY_CONTROL;
}


/*****************************************************************************************************
 * @name:AdjustMotor
 * @brief:integration of position PID control and velocity PID control.
 * @params:
 *     1.Motor: pointer of 'DCMotor' struct.
 * @retval: none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void AdjustMotor( DCMotor *Motor )
{
	if(Motor->RunningMode==POSITION_CONTROL)
	{
		AdjustMotorAngle(Motor);
	}
	else if(Motor->RunningMode==VELOCITY_CONTROL)
	{
		AdjustMotorVelocity(Motor);
	}
}


void SetMotorVelocity(DCMotor* Motor, float Velocity)
{
	/* set the DC-motor run in velocity controlling mode */
	Motor->RunningMode = VELOCITY_CONTROL;

	/* constrain speed value */
	if(Velocity>MOTOR_MAX_VELOCITY)
	{
		Motor->TargetVelocity = MOTOR_MAX_VELOCITY;
	}
	else if(Velocity<(-MOTOR_MAX_VELOCITY))
	{
		Motor->TargetVelocity = -MOTOR_MAX_VELOCITY;
	}
	else
	{
		Motor->TargetVelocity = Velocity;
	}
}


/*****************************************************************************************************
 * @name:AdjustMotorVelocity
 * @brief: update DC motor speed and apply the adjusted speed to DC motor driver.
 * @params:
 *     1.Motor:A pointer to 'DCMotor' instance.
 * @retval:none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void AdjustMotorVelocity(DCMotor* Motor)
{
	int32_t TimerCounterLoadVal;
	MotorVelocityControl(Motor);
	TimerCounterLoadVal = AngVel2TimPWMCnt(Motor->AdjustedVelocity);

	if(Motor->RotateDirectionReverse==1)
	{
		SetMotorPWM(Motor, -TimerCounterLoadVal);
	}
	else
	{
		SetMotorPWM(Motor, TimerCounterLoadVal);
	}
}

/*****************************************************************************************************
 * @name:MotorVelocityControl
 * @brief:This function changes 'AdjustedVelocity' attribute of 'DCMotor' model. 'AdjustedVelocity' attribute
 *     is actually the attribute related to PWM compare value registers directly, because PWM compare
 *     value is calculated by 'AdjustedVelocity'.
 *     This function only changes 'AdjustedVelocity' and let 'AdjustMotorVelocity()' function to execute
 *     PWM compare value register.
 * @params:
 *     1.Motor:A pointer of 'DCMotor' instance.
 * @retval:none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void MotorVelocityControl(DCMotor *Motor)
{
	/* update encoder count number of encoder model, actually read compare counts of timer */
	Motor->CurrentCount = (short)Motor->EncSource;
    Motor->EncSource = 0;

	/* velocity control algorithm will calculate a proper value and store it in DCMotor model */
	Motor->CurrentVelocity = (float)
			                 (((float)Motor->CurrentCount)*6.2831853f)/
						     (((float)PULSE_PER_ROUND)*
	                         ((float)ENCODER_UPDATE_INTERVAL/1000));

	Motor->CurrentAngle += 6.283186f * Motor->CurrentCount/PULSE_PER_ROUND;

	PID_Update(&(Motor->VelocityController), Motor->TargetVelocity, Motor->CurrentVelocity);
	Motor->AdjustedVelocity = Motor->VelocityController.OutputVal;

	/* constrain the value of velocity */
	if(Motor->AdjustedVelocity>MOTOR_MAX_VELOCITY)
	{
		Motor->AdjustedVelocity=MOTOR_MAX_VELOCITY;
	}
	else if(Motor->AdjustedVelocity<(-MOTOR_MAX_VELOCITY))
	{
		Motor->AdjustedVelocity=-MOTOR_MAX_VELOCITY;
	}
}


/*****************************************************************************************************
 * @name:SetMotorAngle
 * @brief: ONLY seperately SET the target angle of one motor, but not update motor angle. If you want
 *  to update motor speed, you have to call 'DCMotor_AdjustAngle' function periodically.
 * @params:
 *     1.Motor:A pointer to 'DCMotor' instance.
 *     2.VelocityVal: speed, unit:cm/s.
 * @retval: none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void SetMotorAngle(DCMotor* Motor, float Angle)
{
	Motor->RunningMode = POSITION_CONTROL;
	Motor->TargetAngle = Angle;
}


/*****************************************************************************************************
 * @name:AdjustMotorAngle
 * @brief: update DC motor position and apply the adjusted position to DC motor driver.
 * @params:
 *     1.Motor:A pointer to 'DCMotor' instance.
 * @retval:none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void AdjustMotorAngle(DCMotor* Motor)
{
	float VelPIDLoopInput;

	MotorAngleControl(Motor);

	VelPIDLoopInput = FittingFunc_2(Motor->AdjustedAngle);

	/* constrain speed value */
	if(VelPIDLoopInput>MOTOR_MAX_VELOCITY)
	{
		Motor->TargetVelocity = MOTOR_MAX_VELOCITY;
	}
	else if(VelPIDLoopInput<(-MOTOR_MAX_VELOCITY))
	{
		Motor->TargetVelocity = -MOTOR_MAX_VELOCITY;
	}
	else
	{
		Motor->TargetVelocity = VelPIDLoopInput;
	}

	AdjustMotorVelocity(Motor);
}

/*****************************************************************************************************
 * @name:MotorAngleControl
 * @brief:This function changes 'AdjustedAngle' attribute of 'DCMotor' model. 'AdjustedAngle' attribute
 *     is actually the attribute related to encoder counting value directly.
 *     This function only changes 'AdjustedAngle' and let 'DCMotor_AdjustAngle()' function to execute
 *     PWM compare value register.
 * @params:
 *     1.Motor:A pointer of 'DCMotor' instance.
 * @retval:none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void MotorAngleControl(DCMotor *Motor)
{
	/* update encoder count number of encoder model, actually read compare counts of timer */
	/* the following line doesn't use 'ENCODER_REMAIN_TIMCNT' macro because if the encoder on
	 * DC-motor is a GMR encoder which always has above 500 lines and generate above 30000
	 * signals to timer making a turn. it may easily cause timer overflow. so here take the
	 * method of incremental encoder count. */
	Motor->CurrentCount = (short)Motor->EncSource;
    
	/* velocity control algorithm will calculate a proper value and store it in DCMotor model */
	Motor->CurrentAngle += 6.283186f * Motor->CurrentCount/PULSE_PER_ROUND;

	PID_Update(&(Motor->PositionController), Motor->TargetAngle, Motor->CurrentAngle);
    Motor->AdjustedAngle = Motor->PositionController.OutputVal;
}



/*****************************************************************************************************
 * @name:
 * @brief:
 * @params:
 * @retval:none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void PID_Init(PID_t *PID_instance, float Kp, float Ki, float Kd)
{
	PID_instance->BeforeLastVelocityBias = 0;
	PID_instance->CurrentVelocityBias = 0;
	PID_instance->LastVelocityBias = 0;

	PID_instance->FeedBack = 0;
	PID_instance->InputVal = 0;
	PID_instance->OutputVal = 0;

	PID_instance->Kp = Kp;
	PID_instance->Ki = Ki;
	PID_instance->Kd = Kd;
}

/*****************************************************************************************************
 * @name:
 * @brief:
 * @params:
 * @retval:none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void PID_Update(PID_t *PID_instance, float Input, float Feedback)
{
	PID_instance->CurrentVelocityBias = Input - Feedback;
	PID_instance->OutputVal += ((PID_instance->Kp) *(PID_instance->CurrentVelocityBias-PID_instance->LastVelocityBias))+
                           ((PID_instance->Ki) *(PID_instance->CurrentVelocityBias))+
                           ((PID_instance->Kd) *(PID_instance->CurrentVelocityBias-2*PID_instance->LastVelocityBias+PID_instance->BeforeLastVelocityBias));
	PID_instance->BeforeLastVelocityBias = PID_instance->LastVelocityBias;
	PID_instance->LastVelocityBias = PID_instance->CurrentVelocityBias;
}

/*****************************************************************************************************
 * @name:SetMotorPWM
 * @brief:this function is the function most close to hardware because this function calls timer
 *     capture/compare counter register.
 * @params:
 *     1.Motor:pointer of DCMotor struct.
 *     2.TimerCounterLoadVal:set timer capture/compare register.
 * @retval:none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void SetMotorPWM(DCMotor* Motor, int32_t TimerCounterLoadVal)
{
	if(TimerCounterLoadVal==0)
	{
		PWMPulseWidthSet(Motor->PWMGeneratingTimer, Motor->PWMGeneratingTimerChannel, 0);
		StopMotor(Motor);
	}
	else if(TimerCounterLoadVal>0)
	{
		SetMotorRotateForward(Motor);
		PWMPulseWidthSet(Motor->PWMGeneratingTimer, Motor->PWMGeneratingTimerChannel, (uint32_t)TimerCounterLoadVal);
	}
	else
	{
		SetMotorRotateBackward(Motor);
		PWMPulseWidthSet(Motor->PWMGeneratingTimer, Motor->PWMGeneratingTimerChannel, (uint32_t)abs(TimerCounterLoadVal));
	}
}


/*****************************************************************************************************
 * @name:SetMotorRotateForward
 * @brief:TB6612 module has two logic pins which controls rotation direction of motor, so this function
 *     changes pin state to set motor rotate forward.
 * @params: 
 * @retval:none
 * @note: rotation logic is from TB6612 datasheet.
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void SetMotorRotateForward(DCMotor *Motor)
{
	GPIOPinWrite(Motor->RotationControlGPIOPort1,Motor->RotationControlGPIOPin1, ~Motor->RotationControlGPIOPin1);
	GPIOPinWrite(Motor->RotationControlGPIOPort2,Motor->RotationControlGPIOPin2, Motor->RotationControlGPIOPin2);
}

/*****************************************************************************************************
 * @name:SetMotorRotateBackward
 * @brief:TB6612 module has two logic pins which controls rotation direction of motor, so this function
 *     changes pin state to set motor rotate backward.
 * @params:
 * @retval:none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void SetMotorRotateBackward(DCMotor *Motor)
{
	GPIOPinWrite(Motor->RotationControlGPIOPort1,Motor->RotationControlGPIOPin1, Motor->RotationControlGPIOPin1);
	GPIOPinWrite(Motor->RotationControlGPIOPort2,Motor->RotationControlGPIOPin2, ~Motor->RotationControlGPIOPin2);
}

/*****************************************************************************************************
 * @name:StopMotor
 * @brief:stop motor by setting both logic pins to low level.
 * @params:
 * @retval: none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void StopMotor(DCMotor *Motor)
{
	GPIOPinWrite(Motor->RotationControlGPIOPort1,Motor->RotationControlGPIOPin1, ~Motor->RotationControlGPIOPin1);
	GPIOPinWrite(Motor->RotationControlGPIOPort2,Motor->RotationControlGPIOPin2, ~Motor->RotationControlGPIOPin2);
}

