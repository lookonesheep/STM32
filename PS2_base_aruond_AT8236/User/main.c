#include <stdio.h>
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "pstwo.h"
#include "OLED.h"
#include "PWM.h"
#include "Motor.h"
#include "PS2_Mecanum_Control.h"

//------------接线说明---------------
// PS2接收器--------STM32F1主板1引脚

//    VCC     -----------    V3.3
//    GND     -----------    GND
//    DI、DAT      --------- PA3
//    DO、CMD      ----------PA2
//    CS      -----------    PA1
//    CLK     -----------    PA0

int PS2_LX, PS2_LY, PS2_RX, PS2_RY, PS2_KEY;

int16_t map_0_255_to_minus100_100(uint8_t input)
{
    return (int16_t)((int32_t)input * 200 / 255 - 100);
}

int main(void)
{
    SystemInit();    //======配置系统时钟为72M
    uart_init(9600); //======串口初始化
    Delay_ms(500);   //======延时等待初始化稳定
    PS2_Init();      //======ps2驱动端口初始化
    PS2_SetInit();   //======ps2配置初始化,配置“红绿灯模式”，并选择是否可以修改
    Delay_ms(500);
    OLED_Init();  //======OLED显示屏初始化
    Motor_Init(); // 电机初始化
    while (1) {
        PS2_LX  = PS2_AnologData(PSS_LX);
        PS2_LY  = PS2_AnologData(PSS_LY);
        PS2_RX  = PS2_AnologData(PSS_RX);
        PS2_RY  = PS2_AnologData(PSS_RY);
        PS2_KEY = PS2_DataKey();
        printf("%d     PS2_LX:", PS2_LX);
        printf("%d     PS2_LY:", PS2_LY);
        printf("%d     PS2_RX:", PS2_RX);
        printf("%d     PS2_RY:", PS2_RY);
        printf("%d \r\nPS2_KEY:", PS2_KEY);

        OLED_ShowString(1, 1, "PS2_LX:");
        OLED_ShowString(2, 1, "PS2_LY:");
        OLED_ShowString(3, 1, "PS2_RX:");
        OLED_ShowString(4, 1, "PS2_RY:");

        OLED_ShowNum(1, 8, PS2_LX, 3);
        OLED_ShowNum(2, 8, PS2_LY, 3);
        OLED_ShowNum(3, 8, PS2_RX, 3);
        OLED_ShowNum(4, 8, PS2_RY, 3);

        PS2_Mecanum_Control(PS2_LX, PS2_LY, PS2_RX);

        // Delay_ms(10);
    }
}