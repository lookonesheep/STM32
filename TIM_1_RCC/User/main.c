#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"

uint16_t count = 0;

int main(void)
{
    /*模块初始化*/
    OLED_Init();  // OLED初始化
    Timer_Init(); // 定时器2初始化

    /*OLED显示*/
    OLED_ShowString(1, 1, "Hello Fans!"); // 1行1列显示字符串Hello Fans!
    OLED_ShowString(2, 1, "Count:");      // 2行1列显示字符串Count:
    OLED_ShowString(3, 1, "Num:");        // 3行1列显示字符串Num:

    while (1) {
        OLED_ShowNum(2, 7, count, 5);                // 2行6列显示count值，5位宽
        OLED_ShowNum(3, 5, TIM_GetCounter(TIM2), 5); // 3行1列显示定时器2计数值，5位宽
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) // 检查是否有更新中断发生
    {
        // 处理定时器中断
        count++;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除更新中断标志位
    }
}