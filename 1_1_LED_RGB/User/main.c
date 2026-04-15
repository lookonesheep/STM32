#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"

int main(void)
{
    LED_Init();  // LED初始化
	while (1)
	{
		LED_Green_status(); // 绿灯常亮
        Delay_ms(10);
        LED_Yellow_status();//黄灯闪烁
        Delay_ms(10);
        LED_Red_status();//红灯常亮
		Delay_ms(100);
	}
}
