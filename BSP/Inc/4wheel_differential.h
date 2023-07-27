/**
 * @file 4wheel_differential.h
 * @author pansamic
 * @brief 
 * @version 0.1
 * @date 2023-07-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __4WHEEL_DIFFERENTIAL_H__
#define __4WHEEL_DIFFERENTIAL_H__
#include <stdint.h>

#define CARMODE_VELOCITY 1
#define CARMODE_DISTANCE 2
#define CARMODE_ANGLE    3

#define WHEEL_DIAMETER   6.95f
#define WHEEL_RADIUS     (WHEEL_DIAMETER / 2.0f)
#define X_AXIS_WHELL_DISTANCE  16.5f
#define Y_AXIS_WHELL_DISTANCE  10.0f
#define DisToAngle(Radius,Displace) ((Displace)/(Radius))
#define LinVelToAngVel(Radius,LinearVel)  ((LinearVel)/(Radius))
#define AngVelToLinVel(Radius,AngularVel) ((Radius)*(AngularVel))
#define RAD_TO_DEG 57.29578f
#define DEG_TO_RAD 0.01745329f

typedef struct CarTypeDef
{
    uint8_t RunningMode;

    float TargetPitch;
    float TargetRoll;
    float TargetYaw;
    float AdjustedPitch;
    float AdjustedRoll;
    float AdjustedYaw;
    float CurrentPitch;
    float CurrentRoll;
    float CurrentYaw;


    float TargetPitchVelocity;
    float TargetRollVelocity;
    float TargetYawVelocity;
    float AdjustedPitchVelocity;
    float AdjustedRollVelocity;
    float AdjustedYawVelocity;
    float CurrentPitchVelocity;
    float CurrentRollVelocity;
    float CurrentYawVelocity;


    /**
     * @brief wheel velocity in car frame
     * 
     */
    float TargetLeftVelocity;
    float TargetRightVelocity;
    float AdjustedLeftVelocity;
    float AdjustedRightVelocity;
    float CurrentLeftVelocity;
    float CurrentRightVelocity;


    /**
     * @brief the velocity in world frame
     * 
     */
    float CurrentxAxisVelocity;
    float CurrentyAxisVelocity;
    float CurrentzAxisVelocity;

    /**
     * @brief the distance in world frame
     * 
     */
    float CurrentxAxisDistance;
    float CurrentyAxisDistance;
    float CurrentzAxisDistance;

    /**
     * @brief the distance in car frame
     * 
     */
    float TargetLeftDistance;
    float TargetRightDistance;
    float AdjustedLeftDistance;
    float AdjustedRightDistance;
    float CurrentLeftDistance;
    float CurrentRightDistance;

}Car_t;

extern Car_t Car;

void    Car_Adjust             ( void );

/* velocity */
void    Car_SetVelocity        ( float LeftVel, float RightVel );
void    Car_AdjustVelocity     ( void );

/* displace */
void    Car_SetDistance        ( float LeftDis, float RightDis );
void    Car_AdjustDistance     ( void );

/* angle */
void    Car_SetAngle           ( float Pitch, float Roll, float Yaw );
void    Car_AdjustAngle        ( void );

#endif
