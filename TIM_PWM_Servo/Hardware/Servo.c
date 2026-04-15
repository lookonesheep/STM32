#include "stm32f10x.h"
#include "PWM.h"

/*
@brief  舵机初始化函数
@param  无
@retval 无
*/
void Servo_Init(void)
{
    PWM_Init();
}

/*
@brief  设置舵机角度
@param  Angle:舵机角度，单位°
@retval 无
*/

void Servo_SetAngle(float Angle)
{
    float CompareValue;
    CompareValue = (Angle / 180 * 2000) + 500;
    PWM_SetCompare2(CompareValue);
}
