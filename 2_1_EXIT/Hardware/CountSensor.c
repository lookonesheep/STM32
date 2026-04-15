#include "stm32f10x.h"

uint16_t CountSensor_Count = 0; //计数器变量

void CountSensor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure; //定义GPIO初始化结构体变量
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14); //配置外部中断线

    EXTI_InitTypeDef EXTI_InitStructure; //定义外部中断初始化结构体变量
    EXTI_InitStructure.EXTI_Line = EXTI_Line14;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//上升沿和下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置抢占优先级组为2

    NVIC_InitTypeDef NVIC_InitStructure; //定义NVIC初始化结构体变量
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//由于选择P14引脚，所以选择外部中断15-10
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级0x01
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //子优先级0x01
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

uint16_t CountSensor_GetCount(void)
{
    return CountSensor_Count; //返回计数器变量的值
}

void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line14) == SET) //判断外部中断14是否产生中断
    {
        //在此处编写中断处理程序
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0) //判断P14引脚是否为低电平
        {
        CountSensor_Count++; //计数器加1
        }
    EXTI_ClearITPendingBit(EXTI_Line14); //清除中断标志位
    }
}