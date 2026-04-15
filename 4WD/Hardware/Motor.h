#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

/* 电机驱动模块 I2C 地址（需要根据模块确认，假设为0x30） */
#define MOTOR_I2C_ADDR  0x30

void Motor_Init(void);
void Motor_SetPWM(int8_t pwm);
int32_t Motor_ReadEncoder(void);

#endif
