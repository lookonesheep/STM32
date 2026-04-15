#include "PS2_Mecanum_Control.h"
#include "Motor.h"
#include <math.h>

// 电机ID定义
#define MOTOR_A     1   // 左前
#define MOTOR_B     2   // 右前
#define MOTOR_C     3   // 左后
#define MOTOR_D     4   // 右后

// 车体结构参数
#define WHEEL_HALF      9.25f   // 左右轮距一半
#define AXLE_HALF       9.0f    // 前后轴距一半
#define ROTATE_GAIN     0.055f  // 旋转灵敏度

/********************************************************
 * 函数：MapPS2Value
 * 功能：将PS2的0~255映射为 -100 ~ +100
 ********************************************************/
int16_t MapPS2Value(uint8_t val)
{
    int16_t temp = (int16_t)val - 128;
    return (temp * 100) / 128;
}

/********************************************************
 * 函数：PS2_Mecanum_Control
 * 功能：PS2控制4轮麦克纳姆运动
 * LX：左右横移
 * LY：前后
 * RX：旋转
 ********************************************************/
void PS2_Mecanum_Control(uint8_t PS2_LX, uint8_t PS2_LY, uint8_t PS2_RX)
{
    // 摇杆值映射为速度值
    int16_t x =  MapPS2Value(PS2_LX);
    int16_t y = -MapPS2Value(PS2_LY);
    int16_t r =  MapPS2Value(PS2_RX);

    // 旋转分量
    float rotate = r * ((WHEEL_HALF + AXLE_HALF) * ROTATE_GAIN);

    // 麦克纳姆轮4轮速度解算
    float vA =  y + x + rotate;
    float vB = -y + x - rotate;
    float vC =  y + x - rotate;
    float vD = -y + x + rotate;

    // 限幅归一化（防止超100）
    float maxNum = fabs(vA);
    maxNum = (maxNum < fabs(vB)) ? fabs(vB) : maxNum;
    maxNum = (maxNum < fabs(vC)) ? fabs(vC) : maxNum;
    maxNum = (maxNum < fabs(vD)) ? fabs(vD) : maxNum;

    if(maxNum > 100.0f)
    {
        vA = vA * 100.0f / maxNum;
        vB = vB * 100.0f / maxNum;
        vC = vC * 100.0f / maxNum;
        vD = vD * 100.0f / maxNum;
    }

    // 输出到4个电机
    Motor_SetSpeed(MOTOR_A, (int16_t)vA);
    Motor_SetSpeed(MOTOR_B, (int16_t)vB);
    Motor_SetSpeed(MOTOR_C, (int16_t)vC);
    Motor_SetSpeed(MOTOR_D, (int16_t)vD);
}