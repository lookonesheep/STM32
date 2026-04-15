#include "Motor.h"
#include "PWM.h"
#include <stdlib.h>

#define PWM_MAX     3599

/******** 左前 A ********/
#define AIN1_H()    GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define AIN1_L()    GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define AIN2_H()    GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define AIN2_L()    GPIO_ResetBits(GPIOA, GPIO_Pin_5)

/******** 右前 B ********/
#define BIN1_H()    GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define BIN1_L()    GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define BIN2_H()    GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define BIN2_L()    GPIO_ResetBits(GPIOB, GPIO_Pin_6)

/******** 左后 C ********/
#define CIN1_H()    GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define CIN1_L()    GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define CIN2_H()    GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define CIN2_L()    GPIO_ResetBits(GPIOB, GPIO_Pin_8)

/******** 右后 D ********/
#define DIN1_H()    GPIO_SetBits(GPIOB, GPIO_Pin_9)
#define DIN1_L()    GPIO_ResetBits(GPIOB, GPIO_Pin_9)
#define DIN2_H()    GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define DIN2_L()    GPIO_ResetBits(GPIOB, GPIO_Pin_10)

void Motor_SetSpeed(int8_t id, int16_t speed)
{
    uint16_t pwm = (uint16_t)(abs(speed) * PWM_MAX / 100);

    switch(id)
    {
        case 1: // A 左前
            speed>0 ? (AIN1_H(),AIN2_L()) : (AIN1_L(),AIN2_H());
            PWM_SetCompare(TIM3, 4, pwm);
            break;

        case 2: // B 右前
            speed>0 ? (BIN1_H(),BIN2_L()) : (BIN1_L(),BIN2_H());
            PWM_SetCompare(TIM3, 1, pwm);
            break;

        case 3: // C 左后
            speed>0 ? (CIN1_H(),CIN2_L()) : (CIN1_L(),CIN2_H());
            PWM_SetCompare(TIM3, 2, pwm);
            break;

        case 4: // D 右后
            speed>0 ? (DIN1_H(),DIN2_L()) : (DIN1_L(),DIN2_H());
            PWM_SetCompare(TIM3, 3, pwm);
            break;
    }
}

void Motor_StopAll(void)
{
    Motor_SetSpeed(1,0);
    Motor_SetSpeed(2,0);
    Motor_SetSpeed(3,0);
    Motor_SetSpeed(4,0);
}