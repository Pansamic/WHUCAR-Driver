/**
 * @file tv_adc.c
 * @author darkbe
 * @brief 
 * @version 0.1
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <main.h>
#include <tv_adc.h>
#include <tv_gpio.h>
#include <hw_ints.h>

void TV_ADC0_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(ADC0_BASE);

    ADCSequenceConfigure(ADC0_BASE, ADC_SEQUENCE_2, ADC_TRIGGER_PROCESSOR, 0);
    ADCHardwareOversampleConfigure(ADC0_BASE, 5);
    ADCSequenceStepConfigure(ADC0_BASE, ADC_SEQUENCE_2, 0, ADC_CTL_CH0 );
	//ADCSequenceStepConfigure(ADC0_BASE, ADC_SEQUENCE_2, 1, ADC_CTL_CH1 );
	//DCSequenceStepConfigure(ADC0_BASE, ADC_SEQUENCE_2, 2, ADC_CTL_CH2 );
    ADCSequenceStepConfigure(ADC0_BASE, ADC_SEQUENCE_2, 1, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceEnable(ADC0_BASE, ADC_SEQUENCE_2); // 使能一个采样序列
    ADCIntClear(ADC0_BASE, ADC_SEQUENCE_2);       // 清除采样序列中断源
}
 
/************************************************************************
 * @brief   获取ADC0的转化采样值。
 * @return  ADC采样值[4]
 ***********************************************************************/
void TV_ADC0_Read(uint32_t* ADCValue)
{
    ADCProcessorTrigger(ADC0_BASE, ADC_SEQUENCE_2);         // 触发获取端口采样
    while(!ADCIntStatus(ADC0_BASE, ADC_SEQUENCE_2, false)); // 等待采样结束
    ADCIntClear(ADC0_BASE, ADC_SEQUENCE_2);                 // 清除ADC采样中断标志
    ADCSequenceDataGet(ADC0_BASE, ADC_SEQUENCE_2, ADCValue);// 读取ADC采样值
}
