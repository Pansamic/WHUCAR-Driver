#ifndef _TV_TIMER_H_
#define _TV_TIMER_H_
#ifdef __cplusplus
extern "C"{
#endif

void TV_TIMER0_Init(uint32_t Clock);
void TV_TIMER1_Init(uint32_t Clock);
void TV_TIMER3A_PWM_Init(uint32_t Load);
void TV_TIMER3A_PWM_SetDuty(float Set);
void TV_TIMER3B_PWM_Init(uint32_t Load);
void TV_TIMER3B_PWM_SetDuty(float Set);

#ifdef __cplusplus
}
#endif
#endif /* _TV_TIMER_H_ */