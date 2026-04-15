#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "LED.h"

/*
 * 函    数：LED初始化
 * 参    数：无
 * 返 回 值：无
 */
void LED_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启GPIOB的时钟

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); // 将PB0和PB1和PB10引脚初始化为推挽输出
}
/*
 * @brief   绿色LED亮灭函数
 * @param   无
 */
void LED_Green_status(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_1); // 设置LED_BLUE引脚为高电平
    Delay_s(3);
    GPIO_ResetBits(GPIOB, GPIO_Pin_1); // 设置LED_BLUE引脚为低电平
}
/*
@brief   红色LED亮灭函数
@param   无
*/
void LED_Red_status(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_0); // 设置LED_RED引脚为高电平
    Delay_s(3);
    GPIO_ResetBits(GPIOB, GPIO_Pin_0); // 设置LED_RED引脚为低电平
}
/*
@brief   黄色LED亮灭函数
@param   无
*/
void LED_Yellow_status(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_10); // 设置LED_BLUE引脚为低电平
    GPIO_SetBits(GPIOB, GPIO_Pin_10);   // 设置LED_GREEN引脚为高电平
    Delay_s(1);
    GPIO_ResetBits(GPIOB, GPIO_Pin_10);
    Delay_s(1);
    GPIO_SetBits(GPIOB, GPIO_Pin_10);
    Delay_s(1);
    GPIO_ResetBits(GPIOB, GPIO_Pin_10);
}