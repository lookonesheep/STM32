#include "stm32f10x.h"

int16_t Encoder_Count; //定义计数器变量

void Encoder_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure; //定义GPIO初始化结构体变量
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0); //配置外部中断线0
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1); //配置外部中断线1


    EXTI_InitTypeDef EXTI_InitStructure; //定义外部中断初始化结构体变量
    EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置抢占优先级组为2

    NVIC_InitTypeDef NVIC_InitStructure; //定义NVIC初始化结构体变量
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//由于选择PB0引脚，所以选择外部中断0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级0x01
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //响应优先级0x01
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//由于选择PB1引脚，所以选择外部中断1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级0x01
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //响应优先级0x02
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

int16_t Encoder_Count_Get(void)
{
    int16_t Temp;
    Temp = Encoder_Count;
    Encoder_Count = 0;
    return Temp;
}

void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) == SET) //判断外部中断14是否产生中断
    {
        //在此处编写中断处理程序
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) //判断P0引脚是否为低电平
        {
            if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) //判断P1引脚是否为低电平
                {
                Encoder_Count --; //计数器减1
                }
        }
    EXTI_ClearITPendingBit(EXTI_Line0); //清除中断标志位
    }
}

void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) == SET) //判断外部中断1是否产生中断
    {
        //在此处编写中断处理程序
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) //判断P0引脚是否为低电平
            {
                if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) //判断P1引脚是否为低电平
                    {
                    Encoder_Count ++; //计数器减1
                    }
            }
    EXTI_ClearITPendingBit(EXTI_Line1); //清除中断标志位
    }
}