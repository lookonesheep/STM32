#include "stm32f10x.h"
#include <math.h>   // fabs()
#include "Motor.h"


/******************************************************
 * 宏定义部分
 ******************************************************/

//==================== 电机 ID 定义 ====================
// 方便记忆，避免直接写数字
#define MOTOR_A_ID   1   // 左前轮
#define MOTOR_B_ID   2   // 右前轮
#define MOTOR_C_ID   3   // 左后轮
#define MOTOR_D_ID   4   // 右后轮

//==================== MEC 参数定义 ====================
// MEC_wheelspacing  : 车体左右轮间距的一半 (单位: cm 或 mm, 按你实际尺寸)
// MEC_axlespacing   : 车体前后轮间距的一半
// 例如: 车体左右轮距 = 20cm, 前后轮距 = 25cm
// 则 MEC_wheelspacing = 10, MEC_axlespacing = 12.5
#define MEC_wheelspacing   9.25f   // 左右半距
#define MEC_axlespacing    9.0f   // 前后半距
#define ROTATION_GAIN   0.055f   // 旋转灵敏度系数 (0.1~0.3比较合适)


/******************************************************
 * 工具函数部分
 ******************************************************/

//==================== 摇杆数值映射函数 ====================
// 功能: 将PS2手柄 0~255 的值映射到 -100 ~ +100
// 参数: val -> PS2输入 (0~255)
// 返回: -100 ~ +100 (电机速度百分比)
int16_t MapPS2Value(uint8_t val)
{
    // 以128为中点，小于128为负数，大于128为正数
    // (val-128) 范围：-128 ~ +127
    // 除以127 后 -> -1.0 ~ +1.0
    // 再乘以100 -> -100 ~ +100
    return (int16_t)( ((int)val - 128) * 65 / 127 );
}


/******************************************************
 * 麦克纳姆轮运动控制部分
 ******************************************************/

//==================== PS2 控制函数 ====================
// 功能: 使用PS2摇杆控制4个麦克纳姆轮的速度
// 参数: PS2_LX -> 左摇杆X (0~255) 控制小车横移
//       PS2_LY -> 左摇杆Y (0~255) 控制小车前后
//       PS2_RX -> 右摇杆X (0~255) 控制小车旋转
// 注意: 电机ID: 1=A左前, 2=B右前, 3=C左后, 4=D右后
void PS2_Mecanum_Control(uint8_t PS2_LX, uint8_t PS2_LY, uint8_t PS2_RX)
{
    //-------------------- 1. 摇杆数值映射 --------------------
    int16_t xSpeed = MapPS2Value(PS2_LX);   // 左摇杆X -> 横移速度
    int16_t ySpeed = -MapPS2Value(PS2_LY);  // 左摇杆Y -> 前后速度 (方向取反, 上推为前进)
    int16_t zSpeed = MapPS2Value(PS2_RX);   // 右摇杆X -> 旋转速度

    //-------------------- 2. 麦克纳姆轮运动解算 --------------------
    // 电机速度计算公式:
    // 前后速度ySpeed + 横移速度xSpeed ± 旋转速度zSpeed*车体参数
    float MOTOR_A =  ySpeed + xSpeed + zSpeed * ((MEC_wheelspacing + MEC_axlespacing) * ROTATION_GAIN); // 左前轮
    float MOTOR_B = -ySpeed + xSpeed - zSpeed * ((MEC_wheelspacing + MEC_axlespacing) * ROTATION_GAIN); // 右前轮
    float MOTOR_C =  ySpeed + xSpeed - zSpeed * ((MEC_wheelspacing + MEC_axlespacing) * ROTATION_GAIN); // 左后轮
    float MOTOR_D = -ySpeed + xSpeed + zSpeed * ((MEC_wheelspacing + MEC_axlespacing) * ROTATION_GAIN); // 右后轮

    //-------------------- 3. 归一化处理 (限幅) --------------------
    // 取4个电机速度中的最大值
    float maxVal = fabs(MOTOR_A);
    if(fabs(MOTOR_B) > maxVal) maxVal = fabs(MOTOR_B);
    if(fabs(MOTOR_C) > maxVal) maxVal = fabs(MOTOR_C);
    if(fabs(MOTOR_D) > maxVal) maxVal = fabs(MOTOR_D);

    // 如果超出100 (最大速度)，则按比例缩放到 [-100,100]
    if(maxVal > 100) {
        MOTOR_A = MOTOR_A * 100 / maxVal;
        MOTOR_B = MOTOR_B * 100 / maxVal;
        MOTOR_C = MOTOR_C * 100 / maxVal;
        MOTOR_D = MOTOR_D * 100 / maxVal;
    }

    //-------------------- 4. 输出到电机 --------------------
    // 调用你的电机驱动函数, 设置每个电机的PWM占空比
    Motor_SetSpeed(MOTOR_A_ID, (int16_t)MOTOR_A); // 左前
    Motor_SetSpeed(MOTOR_B_ID, (int16_t)MOTOR_B); // 右前
    Motor_SetSpeed(MOTOR_C_ID, (int16_t)MOTOR_C); // 左后
    Motor_SetSpeed(MOTOR_D_ID, (int16_t)MOTOR_D); // 右后
}
