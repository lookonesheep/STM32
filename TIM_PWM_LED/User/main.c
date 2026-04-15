#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "PWM.h"

uint8_t i = 0;

int main(void)
{
    /*模块初始化*/
    OLED_Init();  // OLED初始化
    PWM_Init();    // PWM初始化    

    /*OLED显示*/
    OLED_ShowString(1, 1, "Hello Fans!"); // 1行1列显示字符串Hello Fans!
    OLED_ShowString(2, 1, "PWM Is:"); // 2行1列显示字符串PWM Is:

    while (1) {

        /*PWM输出*/
        for (i = 0; i < 100; i++) {
            PWM_SetCompare1(i);
            Delay_ms(20);
            OLED_ShowNum(2, 8, i, 2); // 2行8列显示数字i
        }
        for (i = 100; i > 0; i--) {
            PWM_SetCompare1(i);
            Delay_ms(20);
            OLED_ShowNum(2, 8, i, 2); // 2行8列显示数字i
        }
        
    }
}
