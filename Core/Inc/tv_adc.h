#ifndef _TV_ADC_H_
#define _TV_ADC_H_
#ifdef __cplusplus
extern "C"{
#endif

#define ADC_SEQUENCE_0 0
#define ADC_SEQUENCE_1 1
#define ADC_SEQUENCE_2 2
#define ADC_SEQUENCE_3 3

void TV_ADC0_Init(void);
void TV_ADC0_Read(uint32_t* ADCValue);

#ifdef __cplusplus
}
#endif
#endif
