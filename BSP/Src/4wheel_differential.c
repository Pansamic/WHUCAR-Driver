/**
 * @file 4wheel_differential.c
 * @author pansamic
 * @brief 
 * @version 0.1
 * @date 2023-07-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <4wheel_differential.h>
#include <motor.h>

extern DCMotor LeftFrontMotor;
extern DCMotor LeftRearMotor;
extern DCMotor RightFrontMotor;
extern DCMotor RightRearMotor;

void Car_VelocityControl(void);
void Car_DistanceControl(void);
void Car_AngleControl(void);


void Car_Adjust(void)
{
	float CarLeftDisplace = 0;
	float CarRightDisplace = 0;
	float CarAngleDisplace = 0;
    switch(Car.RunningMode)
    {
        case CARMODE_VELOCITY:
            Car_AdjustVelocity();
			/* since encoder count value will be cleared when car running in
			 * VELOCITY mode, we can just fetch the current encoder value
			 * and calculate the distance and velocity of both left and right
			 * side of car */
			CarLeftDisplace  = (float)(LeftFrontMotor.CurrentCount + LeftRearMotor.CurrentCount) / 2 * 3.1416f * WHEEL_DIAMETER / (PULSE_PER_ROUND);
			CarRightDisplace = (float)(RightFrontMotor.CurrentCount + RightRearMotor.CurrentCount) / 2 * 3.1416f * WHEEL_DIAMETER / (PULSE_PER_ROUND);
			CarAngleDisplace = (float)(CarRightDisplace-CarLeftDisplace)/X_AXIS_WHELL_DISTANCE;
			Car.CurrentYaw += CarAngleDisplace;
			Car.CurrentxAxisDistance += (CarRightDisplace+CarLeftDisplace)/2*cos(Car.CurrentYaw+CarAngleDisplace/2);
			Car.CurrentyAxisDistance += (CarRightDisplace+CarLeftDisplace)/2*sin(Car.CurrentYaw+CarAngleDisplace/2);
            break;
        case CARMODE_DISTANCE:
			/* since encoder count won't ve clear when car is running in distance mode,
			 * we must save the old encoder value first, update encoder and substract
			 * old count value to calculate the displacement in a short time period. */
			CarLeftDisplace = ((float)(LeftFrontMotor.CurrentCount+LeftRearMotor.CurrentCount)/2.0f);
			CarRightDisplace = ((float)(RightFrontMotor.CurrentCount+RightRearMotor.CurrentCount)/2.0f);
            Car_AdjustDistance();
			CarLeftDisplace = (((float)(LeftFrontMotor.CurrentCount+LeftRearMotor.CurrentCount)/2.0f) - CarLeftDisplace) / 2 * 3.1416f * WHEEL_DIAMETER / (PULSE_PER_ROUND);
			CarRightDisplace = (((float)(RightFrontMotor.CurrentCount+RightRearMotor.CurrentCount)/2.0f) - CarRightDisplace) / 2 * 3.1416f * WHEEL_DIAMETER / (PULSE_PER_ROUND);
			CarAngleDisplace = (CarRightDisplace-CarLeftDisplace)/X_AXIS_WHELL_DISTANCE;
			Car.CurrentYaw += CarAngleDisplace;
			Car.CurrentxAxisDistance += (CarRightDisplace+CarLeftDisplace)/2*cos(Car.CurrentYaw+CarAngleDisplace/2);
			Car.CurrentyAxisDistance += (CarRightDisplace+CarLeftDisplace)/2*sin(Car.CurrentYaw+CarAngleDisplace/2);
            break;
        case CARMODE_ANGLE:
            Car_AdjustAngle();
            break;
        default:
            break;
    }
}
/*****************************************************************************************************
 * @name:Car_SetVelocity
 * @brief:set target velocity of car to input value.
 * @params:
 *     1.Car:Pointer of 'CarType_t' instance.
 *     2.LeftVel:velocity of left side of 4 wheel differential car.
 *     3.RightVel:velocity of right side of 4 wheel differential car.
 * @retval:none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void Car_SetVelocity(float LeftVel, float RightVel)
{
	Car.TargetLeftVelocity = LeftVel;
	Car.TargetRightVelocity = RightVel;
	Car.RunningMode = CARMODE_VELOCITY;
}

/*****************************************************************************************************
 * @name:Car_AdjustVelocity
 * @brief:Adjust attitude and apply velocity to wheels.
 * @params:
 *     1.Car:Pointer of 'CarType_t' instance.
 * @retval:none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void Car_AdjustVelocity(void)
{
	/* caution:this is the velocity of wheel ranther than motor,
	 * so its unit is cm/s rather than rad/s */
	float LeftWheelAngularVelocity   = 0;  // unit:rad/s
	float RightWheelAngularVelocity  = 0;  // unit:rad/s

	/* closed-loop control */
	Car_VelocityControl();

	LeftWheelAngularVelocity  = LinVelToAngVel(WHEEL_RADIUS,Car.AdjustedLeftVelocity);
	RightWheelAngularVelocity = LinVelToAngVel(WHEEL_RADIUS,Car.AdjustedRightVelocity);

	SetMotorVelocity(&LeftFrontMotor, LeftWheelAngularVelocity );
	SetMotorVelocity(&LeftRearMotor,  LeftWheelAngularVelocity );
	SetMotorVelocity(&RightFrontMotor,RightWheelAngularVelocity );
	SetMotorVelocity(&RightRearMotor, RightWheelAngularVelocity );

	AdjustMotorVelocity(&LeftFrontMotor );
	AdjustMotorVelocity(&LeftRearMotor  );
	AdjustMotorVelocity(&RightFrontMotor);
	AdjustMotorVelocity(&RightRearMotor );
}

/*****************************************************************************************************
 * @name:Car_VelocityControl
 * @brief:Adjust the attitude and motion with attitude sensor.
 * @params:
 *     1.Car:Pointer of 'CarType_t' instance.
 * @retval:none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void Car_VelocityControl(void)
{
	/* The driver for IMU has not yet been developed yet,
	 * so velocity control has nothing to do */
	Car.AdjustedLeftVelocity  = Car.TargetLeftVelocity;
	Car.AdjustedRightVelocity = Car.TargetRightVelocity;
}

/*****************************************************************************************************
 * @name:Car_SetDistance
 * @brief:set target distance of each side(left and right) of car.
 * @params:
 *     1.Car:Pointer of 'CarType_t' instance.
 *     2.LeftDis:distance of left side of car.
 *     3.RightDis:distance of right side of car.
 * @retval:none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void Car_SetDistance(float LeftDis, float RightDis)
{
	Car.TargetLeftDistance += LeftDis;
	Car.TargetRightDistance += RightDis;
	Car.RunningMode = CARMODE_DISTANCE;
}
/*****************************************************************************************************
 * @name:Car_AdjustDistance
 * @brief:Adjust the running distance of each side(left and right) of DC-motors.
 * @params:
 *     1.Car:Pointer of 'CarType_t' instance.
 * @retval:none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void Car_AdjustDistance(void)
{
	/* caution:this is the velocity of wheel ranther than motor,
	 * so its unit is cm/s rather than rad/s */
	float LeftWheelAngle   = 0;  // unit:rad/s
	float RightWheelAngle  = 0;  // unit:rad/s

	/* closed-loop control */
	Car_DistanceControl();

	LeftWheelAngle  = DisToAngle( WHEEL_RADIUS,Car.AdjustedLeftDistance);
	RightWheelAngle = DisToAngle( WHEEL_RADIUS,Car.AdjustedRightDistance);

	SetMotorAngle(&LeftFrontMotor, LeftWheelAngle );
	SetMotorAngle(&LeftRearMotor,  LeftWheelAngle );
	SetMotorAngle(&RightFrontMotor,RightWheelAngle);
	SetMotorAngle(&RightRearMotor, RightWheelAngle);

	AdjustMotorAngle(&LeftFrontMotor );
	AdjustMotorAngle(&LeftRearMotor  );
	AdjustMotorAngle(&RightFrontMotor);
	AdjustMotorAngle(&RightRearMotor );
}

/*****************************************************************************************************
 * @name:Car_DistanceControl
 * @brief:Adjust left and right distance with attitude sensor.
 * @params:
 *     1.Car:Pointer of 'CarType_t' instance.
 * @retval:none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void Car_DistanceControl(void)
{
	/* The driver for IMU has not yet been developed yet......
	 * so velocity control has nothing to do */
	Car.AdjustedLeftDistance  = Car.TargetLeftDistance;
	Car.AdjustedRightDistance = Car.TargetRightDistance;
}

void Car_SetAngle(float Pitch, float Roll, float Yaw)
{
    Car.TargetPitch = Pitch;
    Car.TargetRoll  = Roll;
    Car.TargetYaw   = Yaw;
    Car.RunningMode = CARMODE_ANGLE;
}

void Car_AdjustAngle(void)
{

}

void Car_AngleControl(void)
{

}
