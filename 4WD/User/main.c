#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "I2C.h"

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	Motor_Init();		//电机初始化

	int32_t lastCount = 0;
	
	/*OLED显示*/
	OLED_ShowChar(1, 1, 'A');				//1行1列显示字符A
	
	while (1)
	{
		/* 让电机正转半速 */
        Motor_SetPWM(50);
        Delay_ms(500);

        /* 读取编码器脉冲，计算转速 */
        int32_t cnt = Motor_ReadEncoder();
        int32_t speed = cnt - lastCount; // 脉冲差值 = 速度
        lastCount = cnt;

        /* TODO: 可加上 USART 打印 speed */

        /* 让电机反转半速 */
        Motor_SetPWM(-50);
        Delay_ms(500);
	}
}
