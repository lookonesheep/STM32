#include "stm32f10x.h"

void Timer_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启APB1_TIM2时钟

    TIM_InternalClockConfig(TIM2); // 设置时基单元时钟源为内部时钟

    /*
    计数器溢出频率：CK_CNT_OV = CK_CNT / (ARR + 1)
                             = CK_PSC / (PSC + 1) / (ARR + 1)
                    本次示例 = 72000000 / (7200 + 1) / (10000 + 1)
                             说明：CK_CNT为系统时钟72MHz，CK_CNT_OV的溢出频率为1Hz
                                  即计数器每秒溢出一次。
    ***PSC预分频和ARR自动重装器的取值不是唯一的：
    1.可以PSC预分频给少点，ARR自动重装器的值可以大一些，这样可以以较高的频率计数，提高计数器的精度。
    2.也可以PSC预分频给多点，ARR自动重装器的值可以小一些，这样可以以较低的频率计数。
    */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  // 配置时基单元参数
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;   // ARR自动重装器的值
                                                    // 单位为计数器个数,范围为0-65535
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1; // PSC预分频器的值（10K的频率）/
                                                    // 单位为分频因子，范围为0-65535
    TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; // 重复计数器的值，单位为计数器个数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);  // 初始化时基单元

    TIM_ClearFlag(TIM2, TIM_FLAG_Update); // 清除更新中断标志位

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 使能更新中断,开启了更新中断到NVIC的通路

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置NVIC的优先级分组为2

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM2_IRQn; // 定时器在NVIC中的通道号
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;         // 设置抢占优先级为0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;         // 设置响应优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;    // 使能定时器的NVIC通路
    NVIC_Init(&NVIC_InitStructure);                                   // 使能TIM2的NVIC通路

    TIM_Cmd(TIM2, ENABLE); // 使能定时器
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