#include "stm32f10x.h"
#include "Motor.h"

float Kp = 5.0, Ki = 0.5, Kd = 0.0;   // PID参数（需要自己调）
static float integral = 0, last_error = 0;

extern int16_t Speed_encoder;  // 实际速度
extern int16_t TargetSpeed;    // 目标速度

void Speed_Control(void)
{
    // 1. 计算误差
    int16_t error = TargetSpeed - Speed_encoder;

    // 2. 积分
    integral += error;

    // 3. 微分
    float derivative = error - last_error;
    last_error = error;

    // 4. PID公式
    float output = Kp * error + Ki * integral + Kd * derivative;

    // 5. 限幅（电机PWM范围）
    if (output > 100) output = 100;
    if (output < -100) output = -100;

    // 6. 输出到电机
    Motor_SetSpeed((int16_t)output);
}
