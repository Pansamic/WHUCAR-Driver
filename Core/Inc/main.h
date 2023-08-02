/**
 * @file main.h
 * @author pansamic (wanggengjie@ieee.org)
 * @brief Pin define and include files
 * @version 0.1
 * @date 2023-06-02
 * 
 * @copyright Copyright (c) 2023 ALL RIGHTS RESERVED FOR PANSAMIC
 * 
 */
#ifndef _MAIN_H_
#define _MAIN_H_
#ifdef __cplusplus
extern "C"{
#endif
/* standard C library */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Tiva hardware */
#include <pin_map.h>
#include <hw_memmap.h>

/* Tiva module */
#include <sysctl.h>
#include <uart.h>
#include <gpio.h>
#include <pwm.h>
#include <i2c.h>
#include <fpu.h>
#include <interrupt.h>
#include <adc.h>

/* User Peripheral */
#include <motor.h>
#include <4wheel_differential.h>
#include <mdp_io.h>
#include <tf_luna.h>
#include <servo.h>

#define SYSTEM_FREQUENCY 120000000

/*****************************************************/
/*                        LED                        */
/*****************************************************/
#define LEDR_Pin GPIO_PIN_7
#define LEDR_GPIO_Port GPIO_PORTC_BASE
#define LEDG_Pin GPIO_PIN_3
#define LEDG_GPIO_Port GPIO_PORTK_BASE
#define LEDB_Pin GPIO_PIN_2
#define LEDB_GPIO_Port GPIO_PORTK_BASE


/*****************************************************/
/*                       Motor                       */
/*****************************************************/
#define LFMotor_RotateDirectionCtrl1_Pin GPIO_PIN_5
#define LFMotor_RotateDirectionCtrl1_GPIO_Port GPIO_PORTC_BASE
#define LFMotor_RotateDirectionCtrl2_Pin GPIO_PIN_6
#define LFMotor_RotateDirectionCtrl2_GPIO_Port GPIO_PORTC_BASE

#define LRMotor_RotateDirectionCtrl1_Pin GPIO_PIN_4
#define LRMotor_RotateDirectionCtrl1_GPIO_Port GPIO_PORTC_BASE
#define LRMotor_RotateDirectionCtrl2_Pin GPIO_PIN_3
#define LRMotor_RotateDirectionCtrl2_GPIO_Port GPIO_PORTH_BASE 

#define RFMotor_RotateDirectionCtrl1_Pin GPIO_PIN_1
#define RFMotor_RotateDirectionCtrl1_GPIO_Port GPIO_PORTH_BASE
#define RFMotor_RotateDirectionCtrl2_Pin GPIO_PIN_2
#define RFMotor_RotateDirectionCtrl2_GPIO_Port GPIO_PORTH_BASE

#define RRMotor_RotateDirectionCtrl1_Pin GPIO_PIN_0
#define RRMotor_RotateDirectionCtrl1_GPIO_Port GPIO_PORTH_BASE
#define RRMotor_RotateDirectionCtrl2_Pin GPIO_PIN_3
#define RRMotor_RotateDirectionCtrl2_GPIO_Port GPIO_PORTQ_BASE


/*****************************************************/
/*                    Encoder                        */
/*****************************************************/
#define EncA_LeftFront_Pin GPIO_PIN_6
#define EncA_LeftFront_GPIO_Port GPIO_PORTQ_BASE
#define EncA_LeftFront_IntPin GPIO_INT_PIN_6
#define EncA_LeftFront_GPIO_IntSource INT_GPIOQ

#define EncB_LeftFront_Pin GPIO_PIN_5
#define EncB_LeftFront_GPIO_Port GPIO_PORTQ_BASE
#define EncB_LeftFront_IntPin GPIO_INT_PIN_5
#define EncB_LeftFront_GPIO_IntSource INT_GPIOQ

#define EncA_LeftRear_Pin GPIO_PIN_6
#define EncA_LeftRear_GPIO_Port GPIO_PORTG_BASE
#define EncA_LeftRear_IntPin GPIO_INT_PIN_6
#define EncA_LeftRear_GPIO_IntSource INT_GPIOG

#define EncB_LeftRear_Pin GPIO_PIN_7
#define EncB_LeftRear_GPIO_Port GPIO_PORTG_BASE
#define EncB_LeftRear_IntPin GPIO_INT_PIN_7
#define EncB_LeftRear_GPIO_IntSource INT_GPIOG

#define EncA_RightFront_Pin GPIO_PIN_4
#define EncA_RightFront_GPIO_Port GPIO_PORTG_BASE
#define EncA_RightFront_IntPin GPIO_INT_PIN_4
#define EncA_RightFront_GPIO_IntSource INT_GPIOG

#define EncB_RightFront_Pin GPIO_PIN_5
#define EncB_RightFront_GPIO_Port GPIO_PORTG_BASE
#define EncB_RightFront_IntPin GPIO_INT_PIN_5
#define EncB_RightFront_GPIO_IntSource INT_GPIOG

#define EncA_RightRear_Pin GPIO_PIN_3
#define EncA_RightRear_GPIO_Port GPIO_PORTG_BASE
#define EncA_RightRear_IntPin GPIO_INT_PIN_3
#define EncA_RightRear_GPIO_IntSource INT_GPIOG

#define EncB_RightRear_Pin GPIO_PIN_2
#define EncB_RightRear_GPIO_Port GPIO_PORTG_BASE
#define EncB_RightRear_IntPin GPIO_INT_PIN_2
#define EncB_RightRear_GPIO_IntSource INT_GPIOG


/*****************************************************/
/*                       Key                         */
/*****************************************************/
#define KEY0_Pin GPIO_PIN_2
#define KEY0_GPIO_Port GPIO_PORTE_BASE
#define KEY1_Pin GPIO_PIN_1
#define KEY1_GPIO_Port GPIO_PORTE_BASE
#define KEY2_Pin GPIO_PIN_0
#define KEY2_GPIO_Port GPIO_PORTE_BASE

#define KEYBOARD_OutPort GPIO_PORTL_BASE
#define KEYBOARD_InPort GPIO_PORTM_BASE

/*****************************************************/
/*                       ADC                         */
/*****************************************************/
#define BATTERY_ADC_Pin GPIO_PIN_3
#define BATTERY_ADC_GPIO_Port GPIO_PORTE_BASE

/*****************************************************/
/*                      OLED                         */
/*****************************************************/
#define OLED_DC_GPIO_Port GPIO_PORTD_BASE
#define OLED_DC_Pin GPIO_PIN_0
#define OLED_MOSI_GPIO_Port GPIO_PORTD_BASE
#define OLED_MOSI_Pin GPIO_PIN_1
#define OLED_RST_GPIO_Port GPIO_PORTD_BASE
#define OLED_RST_Pin GPIO_PIN_2
#define OLED_SCLK_GPIO_Port GPIO_PORTD_BASE
#define OLED_SCLK_Pin GPIO_PIN_3

// sensor
#define IR_SENSOR_GPIO_Port GPIO_PORTQ_BASE
#define IR_SENSOR_CLK_Pin GPIO_PIN_0
#define IR_SENSOR_DAT_Pin GPIO_PIN_2

extern DCMotor LeftFrontMotor;
extern DCMotor LeftRearMotor;
extern DCMotor RightFrontMotor;
extern DCMotor RightRearMotor;
extern Car_t Car;

extern MDP_io Jetsonio;
extern MDP_io K210io;
extern TFLuna tfluna;
extern Servo_t Servo1;
extern Servo_t Servo2;

#ifdef __cplusplus
}
#endif
#endif /* _MAIN_H_ */
