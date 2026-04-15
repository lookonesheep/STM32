#include "motor.h"
#include "i2c.h"

/**
  * @brief  电机模块初始化
  * @note   实际上就是初始化 I2C
  */
void Motor_Init(void)
{
    I2C1_Init();
}

/**
  * @brief  设置电机转速（开环 PWM 控制）
  * @param  pwm: -100 ~ 100 (负数 = 反转，正数 = 正转)
  */
void Motor_SetPWM(int8_t pwm)
{
    I2C_WriteReg(MOTOR_I2C_ADDR, 0x1F, (uint8_t)pwm);
}

/**
  * @brief  读取电机编码器累计脉冲值
  * @retval 脉冲数（int32，可能正负）
  */
int32_t Motor_ReadEncoder(void)
{
    uint8_t data[4];
    I2C_ReadReg(MOTOR_I2C_ADDR, 0x3C, data, 4);

    /* 将4字节拼接成有符号32位整数 */
    int32_t val = (data[3]<<24) | (data[2]<<16) | (data[1]<<8) | data[0];
    return val;
}
