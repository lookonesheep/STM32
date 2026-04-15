#include "PWM.h"
#include "stm32f10x.h"

#define PWM_PERIOD  3599

void PWM_Init(void)
{
    // 时钟使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;

    // ========================= PWM 引脚（复用）=========================
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    // PA6  TIM3_CH1
    // PA7  TIM3_CH2
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // PB0  TIM3_CH3
    // PB1  TIM3_CH4
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // ========================= 方向引脚（普通IO，绝对不占用定时器）=========================
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    // A 电机：PA4、PA5
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // B 电机：PB5、PB6
    // C 电机：PB7、PB8
    // D 电机：PB9、PB10
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 默认低电平
    GPIO_ResetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5);
    GPIO_ResetBits(GPIOB, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);

    // 定时器配置
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_TimeBaseStruct.TIM_Period = PWM_PERIOD;
    TIM_TimeBaseStruct.TIM_Prescaler = 0;
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);

    // PWM 模式
    TIM_OCInitTypeDef TIM_OCStruct;
    TIM_OCStructInit(&TIM_OCStruct);
    TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_Pulse = 0;
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM3, &TIM_OCStruct);
    TIM_OC2Init(TIM3, &TIM_OCStruct);
    TIM_OC3Init(TIM3, &TIM_OCStruct);
    TIM_OC4Init(TIM3, &TIM_OCStruct);

    TIM_Cmd(TIM3, ENABLE);
}

void PWM_SetCompare(TIM_TypeDef *TIMx, uint8_t ch, uint16_t val)
{
    if(val > PWM_PERIOD) val = PWM_PERIOD;
    switch(ch)
    {
        case 1: TIM_SetCompare1(TIMx, val); break;
        case 2: TIM_SetCompare2(TIMx, val); break;
        case 3: TIM_SetCompare3(TIMx, val); break;
        case 4: TIM_SetCompare4(TIMx, val); break;
    }
}