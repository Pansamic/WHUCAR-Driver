/**
 * @file tm4c129_it.h
 * @author pansamic (wanggengjie@ieee.org)
 * @brief interrupt handler header file
 * @version 0.1
 * @date 2023-06-02
 * 
 * @copyright Copyright (c) 2023 ALL RIGHTS RESERVED FOR PANSAMIC
 * 
 */
#ifndef _TM4C123_IT_H_
#define _TM4C123_IT_H_
#ifdef __cplusplus
extern "C"{
#endif

void EXTI_GPIOG_IRQHandler(void);
void EXTI_GPIOQ_IRQHandler(void);
// void EXTI_GPIOQ6_IRQHandler(void);
void UART0_IRQHandler(void);
void UART1_IRQHandler(void);
void UART2_IRQHandler(void);
void UART3_IRQHandler(void);
void UART4_IRQHandler(void);

void uDMA_IRQHandler(void);

void I2C8_IntHandler(void);
void I2C2_IntHandler(void);

#ifdef __cplusplus
}
#endif
#endif /* _TM4C123_IT_H_ */
