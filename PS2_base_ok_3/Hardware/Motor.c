#include "stm32f10x.h"
#include "PWM.h"

/*
@brief  电机初始化函数
@param  无
@retval 无
*/
void Motor_Init(void)
{
    PWM_Init();
}

/*
@brief  设置电机速度
@param  Speed:电机速度，单位rpm
@retval 无
*/

// void Motor_SetSpeed(int16_t Speed) // -100 ~ +100
// {
//     if(Speed >= 0)
//     {
//         // 设置方向引脚正转
//         PWM_SetCompare3(Speed * 3600 / 100);
//         PWM_SetCompare4(0);
//     }
//     else
//     {
//         // 设置方向引脚反转
//         PWM_SetCompare3(0);
//         PWM_SetCompare4(-Speed * 3600 / 100);
//     }
// }

/*
@brief  设置电机速度
@param  MotorID:电机 ID，1~4
@param  Speed:电机速度，单位rpm
@retval 无
*/
void Motor_SetSpeed(uint8_t MotorID, int16_t Speed)
{
    TIM_TypeDef *TIMx;
    uint8_t ch_forward, ch_backward;
    uint16_t ccr_value;

    // 限幅
    if (Speed > 100) Speed = 100;
    if (Speed < -100) Speed = -100;

    // 计算 CCR
    ccr_value = (uint16_t)(Speed * TIM3->ARR / 100);

    // 电机通道映射
    switch (MotorID) {
        case 1:
            TIMx        = TIM4;
            ch_forward  = 1;
            ch_backward = 2;
            break;
        case 2:
            TIMx        = TIM4;
            ch_forward  = 3;
            ch_backward = 4;
            break;
        case 3:
            TIMx        = TIM3;
            ch_forward  = 1;
            ch_backward = 2;
            break;
        case 4:
            TIMx        = TIM3;
            ch_forward  = 3;
            ch_backward = 4;
            break;
        default:
            return; // 错误 ID
    }

    // 先置零再输出，避免 H 桥短路
    PWM_SetCompare(TIMx, ch_forward, 0);
    PWM_SetCompare(TIMx, ch_backward, 0);

    if (Speed >= 0)
        PWM_SetCompare(TIMx, ch_forward, ccr_value);
    else
        PWM_SetCompare(TIMx, ch_backward, -ccr_value);
}
