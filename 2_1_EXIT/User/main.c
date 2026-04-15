#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CountSensor.h"

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	CountSensor_Init();	//计数传感器初始化
	
	/*OLED显示*/	
	OLED_ShowString(1, 1, "Hello Fans!");	//1行1列显示字符串Hello Fans!
	while (1)
	{
		OLED_ShowString(2, 1, "Count:");	//2行1列显示字符串Count:
		OLED_ShowNum(2, 7, CountSensor_GetCount(), 4);	//2行1列显示计数值
	}
}
