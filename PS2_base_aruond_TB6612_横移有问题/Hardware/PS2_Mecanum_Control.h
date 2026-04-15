#ifndef __PS2_MECANUM_CONTROL_H
#define __PS2_MECANUM_CONTROL_H

#include "stm32f10x.h"   // 修复 uint8_t 未定义

void PS2_Mecanum_Control(uint8_t PS2_LX, uint8_t PS2_LY, uint8_t PS2_RX);

#endif