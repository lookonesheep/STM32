#include <stdio.h>
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "pstwo.h"
#include "OLED.h"
#include "PWM.h"
#include "Motor.h"
#include "PS2_Mecanum_Control.h"

// PS2手柄数据变量
int PS2_LX, PS2_LY, PS2_RX, PS2_RY, PS2_KEY;

/*
电机	方向引脚 1	方向引脚 2	PWM 引脚	定时器通道	功能
左前 A	PA4	PA5	PB1	TIM3_CH4	2 方向 + 1PWM
右前 B	PB0	PB3	PA6	TIM3_CH1	2 方向 + 1PWM
左后 C	PB4	PB5	PA7	TIM3_CH2	2 方向 + 1PWM
右后 D	PB6	PB7	PB0	TIM3_CH3	2 方向 + 1PWM
*/

int main(void)
{
    // 系统时钟初始化（72MHz）
    SystemInit();

    // 串口初始化（调试用）
    uart_init(9600);
    Delay_ms(500);

    // PS2手柄初始化
    PS2_Init();
    PS2_SetInit();
    Delay_ms(500);

    // OLED屏初始化
    OLED_Init();

    // PWM + 所有电机GPIO初始化
    PWM_Init();

    // 电机全部停止（防止开机抖动）
    Motor_StopAll();

    //OLED_ShowString(1,1,"Mecanum Ready");

    while (1)
    {
        // 读取手柄4个轴数据
        PS2_LX = PS2_AnologData(PSS_LX);
        PS2_LY = PS2_AnologData(PSS_LY);
        PS2_RX = PS2_AnologData(PSS_RX);
        PS2_RY = PS2_AnologData(PSS_RY);
        PS2_KEY = PS2_DataKey();

        // OLED显示实时摇杆值
        OLED_ShowString(1,1,"LX:");  OLED_ShowNum(1,4,PS2_LX,3);
        OLED_ShowString(2,1,"LY:");  OLED_ShowNum(2,4,PS2_LY,3);
        OLED_ShowString(3,1,"RX:");  OLED_ShowNum(3,4,PS2_RX,3);
        OLED_ShowString(4,1,"RY:");  OLED_ShowNum(4,4,PS2_RY,3);

        // 麦克纳姆轮核心控制（4轮全控）
        PS2_Mecanum_Control(PS2_LX, PS2_LY, PS2_RX);

        Delay_ms(10);
    }
}