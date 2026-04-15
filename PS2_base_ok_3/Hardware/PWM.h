#ifndef __PWM_H
#define __PWM_H

void PWM_Init(void);
void PWM_SetCompare(TIM_TypeDef *TIMx, uint8_t channel, uint16_t CompareValue);




#endif // !__PWM_H
