#ifndef __I2C_H
#define __I2C_H

#include "stm32f10x.h"

/**
  * @brief  I2C1 初始化函数
  */
void I2C1_Init(void);

/**
  * @brief  I2C 写寄存器
  * @param  devAddr: 设备地址
  * @param  regAddr: 寄存器地址
  * @param  data:    写入数据
  */
void I2C_WriteReg(uint8_t devAddr, uint8_t regAddr, uint8_t data);

/**
  * @brief  I2C 读寄存器（多字节）
  * @param  devAddr: 设备地址
  * @param  regAddr: 寄存器地址
  * @param  buf:     数据缓冲区
  * @param  len:     数据长度
  */
void I2C_ReadReg(uint8_t devAddr, uint8_t regAddr, uint8_t *buf, uint8_t len);

#endif
