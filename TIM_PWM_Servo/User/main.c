#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"

uint8_t i = 0;

int main(void)
{
    /*模块初始化*/
    OLED_Init();  // OLED初始化
    Servo_Init();  // 舵机初始化


    /*OLED显示*/
    OLED_ShowString(1, 1, "Hello Fans!"); // 1行1列显示字符串Hello Fans!
    OLED_ShowString(2, 1, "Servo angle:"); // 2行1列显示字符串Servo angle:
    while (1) {
        for (i = 0; i < 180; i++) {
            Servo_SetAngle(i);
            Delay_ms(5);
            OLED_ShowNum(2, 13, i, 3); // 2行13列显示数字i
        }
        for (i = 180; i > 0; i--) {
            Servo_SetAngle(i);
            Delay_ms(5);
            OLED_ShowNum(2, 13, i, 3); // 2行13列显示数字i
        }
    }
}
