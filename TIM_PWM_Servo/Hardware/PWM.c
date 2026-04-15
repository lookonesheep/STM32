#include "stm32f10x.h"

void PWM_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启APB1_TIM2时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启APB2_GPIOA时钟

    GPIO_InitTypeDef GPIO_InitStructure; // 定义GPIO初始化结构体
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; // 设置GPIO_Pin_0为输出模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 设置GPIO_Pin_0为推挽输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置GPIO_Pin_0的速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIO_Pin_0

    TIM_InternalClockConfig(TIM2); // 设置时基单元时钟源为内部时钟

    /*
    计数器溢出频率：CK_CNT_OV = CK_CNT / (ARR + 1)
                             = CK_PSC / (PSC + 1) / (ARR + 1)
                    本次示例 = 72000000 / 20000 / 72 = 50Hz
                             说明：CK_CNT为系统时钟72MHz，CK_CNT_OV的溢出频率为50Hz
            
    ***PSC预分频和ARR自动重装器的取值不是唯一的：
    1.可以PSC预分频给少点，ARR自动重装器的值可以大一些，这样可以以较高的频率计数，提高计数器的精度。
    2.也可以PSC预分频给多点，ARR自动重装器的值可以小一些，这样可以以较低的频率计数。
    */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  // 配置时基单元参数
    TIM_TimeBaseStructure.TIM_Period = 20000 - 1;   // ARR自动重装器的值
                                                    // 单位为计数器个数,范围为0-65535
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // PSC预分频器的值（50的频率）/
                                                    // 单位为分频因子，范围为0-65535
    TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; // 重复计数器的值，单位为计数器个数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);  // 初始化时基单元

    TIM_OCInitTypeDef TIM_OCInitStructure;  // 配置输出比较单元参数
    TIM_OCStructInit(&TIM_OCInitStructure);// 初始化结构体，防止未使用变量导致异常问题

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // 设置比较输出模式为PWM1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 使能比较输出
    TIM_OCInitStructure.TIM_Pulse = 0; // 设置CCR
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 设置比较输出极性为高
    TIM_OC2Init(TIM2, &TIM_OCInitStructure); // TIM2_CH2输出比较初始化

    TIM_Cmd(TIM2, ENABLE); // 使能定时器
}

void PWM_SetCompare2(uint16_t CompareValue)
{
    TIM_SetCompare2(TIM2, CompareValue); // 设置比较输出2的值
}

/*
@brief 定时器中断服务函数
@param 无
@retval 无

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) //检查是否有更新中断发生
    {
        //处理定时器中断
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清除更新中断标志位
    }
}

*/