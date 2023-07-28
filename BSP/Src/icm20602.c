/*****************************************************************************
THIS PROGRAM IS FREE SOFTWARE. YOU CAN REDISTRIBUTE IT AND/OR MODIFY IT
UNDER THE TERMS OF THE GNU GPLV3 AS PUBLISHED BY THE FREE SOFTWARE FOUNDATION.

Copyright (C), 2022-2023, pansamic(Wang GengJie) pansamic@foxmail.com

Filename:    icm20602.c
Author:      Pansamic
Version:     1.0
Create date: 2023.6.20
Description: This file contains all supported peripherals.
Others:      none

History:
1. <author>    <date>                  <desc>
   pansamic  2023.6.20           create v1.0 version.
*****************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif
#include <stdio.h>
#include <math.h>
#include <main.h>
#include <tv_i2c.h>
#include <hw_ints.h>
#include <hw_memmap.h>
#include <icm20602.h>


/*****************************************************************************************
 *                                                                                       *
 *                                      MACROS                                           *
 *                                                                                       *
 *****************************************************************************************/
#define ICM20602_WriteByte(Reg,Value)\
    I2C8_WriteByte(ICM20602_slave_addr,Reg,Value)
#define ICM20602_Write(Reg,pData,Length)\
    I2C8_Write(ICM20602_slave_addr,Reg,pData,Length)
#define ICM20602_ReadByte(Reg)\
    I2C8_ReadByte(ICM20602_slave_addr,Reg)
#define ICM20602_Read(Reg,pBuf,Length)\
    I2C8_Read(ICM20602_slave_addr,Reg,pBuf,Length)


/*****************************************************************************************
 *                                                                                       *
 *                                      CONST                                            *
 *                                                                                       *
 *****************************************************************************************/
// Acc Full Scale Range  +-2G 4G 8G 16G 
enum Ascale
{
    AFS_2G=0,  
    AFS_4G,
    AFS_8G,
    AFS_16G
};

// Gyro Full Scale Range +-250 500 1000 2000 Degrees per second
enum Gscale
{
    GFS_250DPS=0,   
    GFS_500DPS,
    GFS_1000DPS,
    GFS_2000DPS
};

/*****************************************************************************************
 *                                                                                       *
 *                                      VARIABLE                                         *
 *                                                                                       *
 *****************************************************************************************/


/*****************************************************************************************
 *                                                                                       *
 *                                    DECLARATION                                        *
 *                                                                                       *
 *****************************************************************************************/
void ICM20602_HardwareInit( void );
void ICM20602_whoAmI( void );
void ICM20602_StaticCallibration(void);
float Kalman_Filter_x(float Angle,float AngVelocity);
float Kalman_Filter_y(float Angle,float AngVelocity);

/*****************************************************************************************
 *                                                                                       *
 *                                     FUNCTION                                          *
 *                                                                                       *
 *****************************************************************************************/
/*****************************************************************************************************
 * @name:
 * @brief:
 * @params: none
 * @retval: none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void Add_ICM20602(void)
{
	/**********************************************/
	/*        MPU6050 STRUCT INITIALIZATION       */
	/**********************************************/

	/* Accelerometer configuration */
	ICM20602_dev.Accel_X_RAW = 0;
	ICM20602_dev.Accel_Y_RAW = 0;
	ICM20602_dev.Accel_Z_RAW = 0;
	ICM20602_dev.Ax = 0;
	ICM20602_dev.Ay = 0;
	ICM20602_dev.Az = 0;
	ICM20602_dev.VelocityX = 0;
	ICM20602_dev.VelocityY = 0;
	ICM20602_dev.VelocityZ = 0;

	/* Gyro configuration */
	ICM20602_dev.Gyro_X_RAW = 0;
	ICM20602_dev.Gyro_Y_RAW = 0;
	ICM20602_dev.Gyro_Z_RAW = 0;
	ICM20602_dev.Gx = 0;
	ICM20602_dev.Gy = 0;
	ICM20602_dev.Gz = 0;
	ICM20602_dev.AngleX = 0;
	ICM20602_dev.AngleY = 0;
	ICM20602_dev.AngleZ = 0;
	ICM20602_dev.Temperature = 0;

	ICM20602_HardwareInit();
	printf("ICM20602 init done.\r\n");
    SysCtlDelay(120000);
	printf("ICM20602 callibration start.\r\n");
    ICM20602_StaticCallibration();
	printf("ICM20602 callibration done.\r\n");
}
/*****************************************************************************************************
 * @name:ICM20602_whoAmI
 * @brief:Communication test: WHO_AM_I register should return 0x12.
 * @params: none
 * @retval: none
 * @author: Wang Geng Jie
 * @note: WHO_AM_I register always return other value, but that doesn't influence the function 
 *        of ICM20602. You can read correct value from output register. 
 *****************************************************************************************************/
void ICM20602_whoAmI( void )
{
    uint8_t RegVal = ICM20602_ReadByte(ICM20602_WHO_AM_I);
    
    if(RegVal==0x12)
    {
        printf("ICM20602 is online, Identification code:%x.",RegVal);
    }
    else
    {
        printf("ICM20602 is offline, Identification code:%x, expect 0x12.",RegVal);
    }
}
/*****************************************************************************************************
 * @name: ICM20602_HardwareInit
 * @brief: Write bytes to some configuration registers and initializes ICM20602 hardware.
 * @params: none
 * @retval: none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void ICM20602_HardwareInit( void )
{
    /* Reset ICM20602 */
    ICM20602_WriteByte(ICM20602_PWR_MGMT_1,0x80);
    SysCtlDelay(12000000);

    /* CLK_SEL=0 internal 20MHz, TEMP_DIS=0, SLEEP=0 */
    ICM20602_WriteByte(ICM20602_PWR_MGMT_1,0x01);

    /* Enable Acc & Gyro */
    ICM20602_WriteByte(ICM20602_PWR_MGMT_2,0x00);

    /* 176Hz set TEMP_OUT_L, DLPF=3 (Fs=1KHz):0x03 */
    ICM20602_WriteByte(ICM20602_CONFIG,0x01);

    /*  SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
     *  Where INTERNAL_SAMPLE_RATE = 1 kHz */
    ICM20602_WriteByte(ICM20602_SMPLRT_DIV,0x00);

    /* Average of 8 samples | Accelerometer Low Pass Filter 21.2Hz */
    ICM20602_WriteByte(ICM20602_ACCEL_CONFIG2,0x14);
	
    ICM20602_SetAccRange(AFS_2G);
    ICM20602_SetGyroRange(GFS_1000DPS);
}

void ICM20602_StaticCallibration(void)
{
    int32_t GyroXSum = 0;
    int32_t GyroYSum = 0;
	int32_t GyroZSum = 0;
    uint8_t Buffer[6];

    for(uint16_t i=0 ; i<10000 ; i++)
    {
        ICM20602_Read(ICM20602_GYRO_XOUT_H, Buffer, 6);
        GyroXSum += (int16_t)((Buffer[0]<<8) | Buffer[1]);
        GyroYSum += (int16_t)((Buffer[2]<<8) | Buffer[3]);
        GyroZSum += (int16_t)((Buffer[4]<<8) | Buffer[5]);
        SysCtlDelay(12000);
    }
    ICM20602_dev.GOffsetX = (float)GyroXSum/10000.0f;
    ICM20602_dev.GOffsetY = (float)GyroYSum/10000.0f;
    ICM20602_dev.GOffsetZ = (float)GyroZSum/10000.0f;
}

/*****************************************************************************************************
 * @name: ICM20602_Update
 * @brief: Read data from ICM20602, calculate attitude angle with kalman filter and update IMU data.
 * @params: 1.TimeInterval: time interval between two updates.
 * @retval: none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void ICM20602_Update(void)
{
    uint8_t Buffer[14];
    float AngleX = 0;
    float AngleY = 0;
    ICM20602_Read(ICM20602_ACCEL_XOUT_H, Buffer, 14);
    ICM20602_dev.Accel_X_RAW = (Buffer[0]<<8) | Buffer[1];
    ICM20602_dev.Accel_Y_RAW = (Buffer[2]<<8) | Buffer[3];
    ICM20602_dev.Accel_Z_RAW = (Buffer[4]<<8) | Buffer[5];
    ICM20602_dev.Temperature = ((Buffer[6]<<8) | Buffer[7])/TEMP_SENSITIVITY + ROOM_TEMP_OFFSET;
    ICM20602_dev.Gyro_X_RAW = (Buffer[8]<<8) | Buffer[9];
    ICM20602_dev.Gyro_Y_RAW = (Buffer[10]<<8) | Buffer[11];
    ICM20602_dev.Gyro_Z_RAW = (Buffer[12]<<8) | Buffer[13];

    ICM20602_dev.Ax = ((float)ICM20602_dev.Accel_X_RAW * ICM20602_dev.AccelResolution * IMU_ONE_G - ELLIPSOID_CALIBRATION_X_OFFSET)*ELLIPSOID_CALIBRATION_X_SCALE;
    ICM20602_dev.Ay = ((float)ICM20602_dev.Accel_Y_RAW * ICM20602_dev.AccelResolution * IMU_ONE_G - ELLIPSOID_CALIBRATION_Y_OFFSET)*ELLIPSOID_CALIBRATION_Y_SCALE;
    ICM20602_dev.Az = ((float)ICM20602_dev.Accel_Z_RAW * ICM20602_dev.AccelResolution * IMU_ONE_G - ELLIPSOID_CALIBRATION_Z_OFFSET)*ELLIPSOID_CALIBRATION_Z_SCALE;
    ICM20602_dev.Gx = ((float)ICM20602_dev.Gyro_X_RAW - ICM20602_dev.GOffsetX )* ICM20602_dev.GyroResolution;
    ICM20602_dev.Gy = ((float)ICM20602_dev.Gyro_Y_RAW - ICM20602_dev.GOffsetY )* ICM20602_dev.GyroResolution;
    ICM20602_dev.Gz = ((float)ICM20602_dev.Gyro_Z_RAW - ICM20602_dev.GOffsetZ )* ICM20602_dev.GyroResolution;

	/* AngelX and AngleY and AngleZ are degree */
    AngleX = atan2(ICM20602_dev.Ay,ICM20602_dev.Az)*RAD_TO_DEG;
    AngleY = atan2(ICM20602_dev.Ax,ICM20602_dev.Az)*RAD_TO_DEG;

    /* the following variables are in degree */
    ICM20602_dev.AngleX = Kalman_Filter_x(AngleX, ICM20602_dev.Gx);
    ICM20602_dev.AngleY = Kalman_Filter_y(AngleY, ICM20602_dev.Gy);
    ICM20602_dev.AngleZ += ICM20602_dev.Gz * IMU_UPDATE_INTERVAL / 1000.0f;

    ICM20602_dev.VelocityX = ICM20602_dev.Ax * IMU_UPDATE_INTERVAL / 1000.0f;
    ICM20602_dev.VelocityY = ICM20602_dev.Ay * IMU_UPDATE_INTERVAL / 1000.0f;
    ICM20602_dev.VelocityZ = ICM20602_dev.Az * IMU_UPDATE_INTERVAL / 1000.0f;
}
/*****************************************************************************************
 *                                                                                       *
 *                              Accelerometer Functions                                  *
 *                                                                                       *
 *****************************************************************************************/


/*****************************************************************************************************
 * @name: ICM20602_SetAccRange
 * @brief: Calculates Accelerometer resolution
 * @params: 1.Range: 0=+-2g,1=+-4g,2=+-8g,3=+-16g
 * @retval: none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void ICM20602_SetAccRange(uint8_t Range)
{
    switch(Range)
    {
        case AFS_2G:
            ICM20602_dev.AccelResolution = 4.0/32768.0;
            ICM20602_dev.AccelRange = AFS_2G;
            break;
        case AFS_4G:
            ICM20602_dev.AccelResolution = 8.0/32768.0;
            ICM20602_dev.AccelRange = AFS_4G;
            break;
        case AFS_8G:
            ICM20602_dev.AccelResolution = 16.0/32768.0;
            ICM20602_dev.AccelRange = AFS_8G;
            break;
        case AFS_16G:
            ICM20602_dev.AccelResolution = 32.0/32768.0;
            ICM20602_dev.AccelRange = AFS_16G;
            break;
    }
    /* bit[4:3] 0=+-2g,1=+-4g,2=+-8g,3=+-16g, ACC_HPF=On (5Hz) */
    ICM20602_WriteByte(ICM20602_ACCEL_CONFIG,ICM20602_dev.AccelRange<<3);
}
/*****************************************************************************************
 *                                                                                       *
 *                                    GYRO Functions                                     *
 *                                                                                       *
 *****************************************************************************************/
/*****************************************************************************************************
 * @name:
 * @brief: Calculates Gyro resolution
 * @params: none
 * @retval: none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void ICM20602_SetGyroRange(uint8_t Range)
{
    switch(Range)
    {
        case GFS_250DPS:
            ICM20602_dev.GyroResolution = 250.0/32768.0;
            ICM20602_dev.GyroRange = GFS_250DPS;
            break;
        case GFS_500DPS:
            ICM20602_dev.GyroResolution = 500.0/32768.0;
            ICM20602_dev.GyroRange = GFS_500DPS;
            break;
        case GFS_1000DPS:
            ICM20602_dev.GyroResolution = 1000.0/32768.0;
            ICM20602_dev.GyroRange = GFS_1000DPS;
            break;
        case GFS_2000DPS:
            ICM20602_dev.GyroResolution = 2000.0/32768.0;
            ICM20602_dev.GyroRange = GFS_2000DPS;
            break;
    }
    ICM20602_WriteByte(ICM20602_GYRO_CONFIG,ICM20602_dev.GyroRange<<3); // bit[4:3] 0=+-250d/s,1=+-500d/s,2=+-1000d/s,3=+-2000d/s
}

float Kalman_Filter_x(float Angle,float AngVelocity)		
{
	//static float angle_dot;
	static float angle;
	float Q_angle=0.001; 		// 过程噪声的协方差
	float Q_gyro=0.003;			// 0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
	float R_angle=0.5;			// 测量噪声的协方差 既测量偏差
	char  C_0 = 1;
	static float Q_bias, Angle_err;
	static float PCt_0, PCt_1, E;
	static float K_0, K_1, t_0, t_1;
	static float Pdot[4] ={0,0,0,0};
	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };
	angle+=(AngVelocity - Q_bias) * IMU_UPDATE_INTERVAL/1000; //先验估计
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分
	Pdot[1]=-PP[1][1];
	Pdot[2]=-PP[1][1];
	Pdot[3]=Q_gyro;
	PP[0][0] += Pdot[0] * IMU_UPDATE_INTERVAL/1000;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * IMU_UPDATE_INTERVAL/1000;   // 先验估计误差协方差
	PP[1][0] += Pdot[2] * IMU_UPDATE_INTERVAL/1000;
	PP[1][1] += Pdot[3] * IMU_UPDATE_INTERVAL/1000;
	Angle_err = Angle - angle;	//zk-先验估计
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	angle	+= K_0 * Angle_err;	   //后验估计
	Q_bias	+= K_1 * Angle_err;	 //后验估计
	//angle_dot   = Gyro - Q_bias;	//输出值(后验估计)的微分=角速度
	return angle;
}
float Kalman_Filter_y(float Angle,float AngVelocity)		
{
	//static float angle_dot;
	static float angle;
	float Q_angle=0.001; 		// 过程噪声的协方差
	float Q_gyro=0.003;			// 0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
	float R_angle=0.5;			// 测量噪声的协方差 既测量偏差
	char  C_0 = 1;
	static float Q_bias, Angle_err;
	static float PCt_0, PCt_1, E;
	static float K_0, K_1, t_0, t_1;
	static float Pdot[4] ={0,0,0,0};
	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };
	angle+=(AngVelocity - Q_bias) * IMU_UPDATE_INTERVAL/1000; //先验估计
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分
	Pdot[1]=-PP[1][1];
	Pdot[2]=-PP[1][1];
	Pdot[3]=Q_gyro;
	PP[0][0] += Pdot[0] * IMU_UPDATE_INTERVAL/1000;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * IMU_UPDATE_INTERVAL/1000;   // 先验估计误差协方差
	PP[1][0] += Pdot[2] * IMU_UPDATE_INTERVAL/1000;
	PP[1][1] += Pdot[3] * IMU_UPDATE_INTERVAL/1000;
	Angle_err = Angle - angle;	//zk-先验估计
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;

	angle	+= K_0 * Angle_err;	   //后验估计
	Q_bias	+= K_1 * Angle_err;	 //后验估计
	//angle_dot   = Gyro - Q_bias;	//输出值(后验估计)的微分=角速度
	return angle;
}

#ifdef __cplusplus
}
#endif
