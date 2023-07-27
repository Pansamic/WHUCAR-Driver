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
#include <math.h>
#include <4wheel_differential.h>
#include <motor.h>
#include <icm20602.h>

extern DCMotor LeftFrontMotor;
extern DCMotor LeftRearMotor;
extern DCMotor RightFrontMotor;
extern DCMotor RightRearMotor;

void Car_VelocityControl(void);
void Car_DistanceControl(void);
void Car_AngleControl(void);


void Car_Adjust(void)
{
	float CarLeftDisplace = (LeftFrontMotor.CurrentAngle + LeftRearMotor.CurrentAngle) / 2 * WHEEL_RADIUS;
	float CarRightDisplace = (RightFrontMotor.CurrentAngle + RightRearMotor.CurrentAngle) / 2 * WHEEL_RADIUS;
	float CarAngleDisplace = 0;
    switch(Car.RunningMode)
    {
        case CARMODE_VELOCITY:
            Car_AdjustVelocity();
            break;
        case CARMODE_DISTANCE:
            Car_AdjustDistance();
            break;
        case CARMODE_ANGLE:
            Car_AdjustAngle();
            break;
        default:
            break;
    }
	/**
	 * calculate pose of car by odometry. We found that odometry is not accurate enough on 4 wheel differentialcar model.
	 * 
	 */
	// CarLeftDisplace  = ((LeftFrontMotor.CurrentAngle + LeftRearMotor.CurrentAngle) / 2 * WHEEL_RADIUS) - CarLeftDisplace;
	// CarRightDisplace = ((RightFrontMotor.CurrentAngle + RightRearMotor.CurrentAngle) / 2 * WHEEL_RADIUS) - CarRightDisplace;
	// CarAngleDisplace = (CarRightDisplace-CarLeftDisplace)/X_AXIS_WHELL_DISTANCE*0.32505176f;
	// CarAngleDisplace = (CarRightDisplace-CarLeftDisplace)/2;
	// Car.CurrentYaw += CarAngleDisplace;
	// Car.CurrentxAxisDistance += (CarRightDisplace+CarLeftDisplace)/2*cosf(Car.CurrentYaw+CarAngleDisplace/2);
	// Car.CurrentyAxisDistance += (CarRightDisplace+CarLeftDisplace)/2*sinf(Car.CurrentYaw+CarAngleDisplace/2);

	/**
	 * @brief calculate pose of car by IMU(ICM20602)
	 * 
	 */
	CarLeftDisplace  = ((LeftFrontMotor.CurrentAngle + LeftRearMotor.CurrentAngle) / 2 * WHEEL_RADIUS) - CarLeftDisplace;
	CarRightDisplace = ((RightFrontMotor.CurrentAngle + RightRearMotor.CurrentAngle) / 2 * WHEEL_RADIUS) - CarRightDisplace;
	
	Car.CurrentPitch = ICM20602_dev.AngleX * DEG_TO_RAD;
	Car.CurrentRoll = ICM20602_dev.AngleY * DEG_TO_RAD;
	Car.CurrentYaw = ICM20602_dev.AngleZ * DEG_TO_RAD;

	Car.CurrentxAxisDistance += (CarRightDisplace+CarLeftDisplace)/2*cosf(Car.CurrentYaw);
	Car.CurrentyAxisDistance += (CarRightDisplace+CarLeftDisplace)/2*sinf(Car.CurrentYaw);

	Car.CurrentxAxisVelocity = ICM20602_dev.VelocityX * 1000.0f;
	Car.CurrentyAxisVelocity = ICM20602_dev.VelocityY * 1000.0f;
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
