#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"

int8_t i = 0;

int main(void)
{
    /*模块初始化*/
    OLED_Init();  // OLED初始化
    Motor_Init();  // 舵机初始化


    /*OLED显示*/
    OLED_ShowString(1, 1, "Hello Fans!"); // 1行1列显示字符串Hello Fans!
    OLED_ShowString(2, 1, "Servo Ang:"); // 2行1列显示字符串Servo angle:
    //Motor_SetSpeed(0);
    while (1) {
        for (i = -100 ; i <= 100 ; i++) 
        {
            Motor_SetSpeed(i);
            OLED_ShowSignedNum(2, 11, i, 3); // 2行10列显示数字i
            Delay_ms(30); // 延时100ms
        }
        for (i = 100 ; i > -100 ; i--) 
        {
            Motor_SetSpeed(i);
            OLED_ShowSignedNum(2, 11, i, 3); // 2行10列显示数字i
            Delay_ms(30); // 延时100ms
        }
}
}
