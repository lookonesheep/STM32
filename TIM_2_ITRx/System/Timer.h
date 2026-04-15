#ifndef __TIMER_H
#define __TIMER_H

void TIM2_IRQHandler(void);
void Timer_Init(void);
uint16_t Timer_GetCount(void);

#endif // !__TIMER_H