#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Encoder.h"

int16_t count = 0;

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	Encoder_Init();	//计数传感器初始化
	
	/*OLED显示*/	
	OLED_ShowString(1, 1, "Hello Fans!");	//1行1列显示字符串Hello Fans!
	while (1)
	{
		count += Encoder_Count_Get();	//获取计数值并累加
		OLED_ShowString(2, 1, "Count:");	//2行1列显示字符串Count:
		OLED_ShowSignedNum(2, 7, count, 5);	//2行7列显示计数值
	}
}
