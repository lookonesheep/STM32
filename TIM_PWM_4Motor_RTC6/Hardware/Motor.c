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

void Motor_SetSpeed(int16_t Speed) // -100 ~ +100
{
    if(Speed >= 0)
    {
        // 设置方向引脚正转
        PWM_SetCompare3(Speed * 3600 / 100);
        PWM_SetCompare4(0);
    }
    else
    {
        // 设置方向引脚反转
        PWM_SetCompare3(0);
        PWM_SetCompare4(-Speed * 3600 / 100);
    }
}




