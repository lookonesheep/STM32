#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

void Motor_SetSpeed(int8_t motorID, int16_t speed);
void Motor_StopAll(void);

#endif