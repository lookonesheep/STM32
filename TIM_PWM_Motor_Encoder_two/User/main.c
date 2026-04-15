#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"
#include "PWM.h"
#include "Motor.h"
#include "Key.h"
#include "PID.h"
#include "ADC.h"

int16_t Speed_encoder; // 定义速度变量
int8_t Speed;          // 定义速度变量
uint8_t KeyNum;        // 定义按键变量
float voltage;
uint8_t percent;
int16_t TargetSpeed = 0;     // 定义目标速度（全局变量）
int16_t Speed_encoder = 0;   // 定义实际速度（编码器反馈

int main(void)
{
    /*模块初始化*/
    OLED_Init();    // OLED初始化
    Timer_Init();   // 定时器初始化
    Encoder_Init(); // 编码器初始化
    Motor_Init();   // 电机初始化
    Key_Init();     // 按键初始化
    ADC_Battery_Init();

    /*显示静态字符串*/
    OLED_ShowString(1, 1, "Encoder:"); // 1行1列显示字符串Speed:
    OLED_ShowString(2, 1, "Duty:");    //
    OLED_ShowString(3, 1, "BatV:");

    while (1) {
        OLED_ShowSignedNum(1, 9, Encoder_Get(), 5); // 不断刷新显示编码器测得的最新速度

        KeyNum = Key_GetNum(); // 获取按键键码
        if (KeyNum == 1)       // 按键1按下
        {
            Speed += 20;     // 速度变量自增20
            if (Speed > 100) // 速度变量超过100后
            {
                Speed = -100; // 速度变量变为-100
                              // 此操作会让电机旋转方向突然改变，可能会因供电不足而导致单片机复位
                              // 若出现了此现象，则应避免使用这样的操作
            }
        }
        Motor_SetSpeed(Speed);              // 设置直流电机的速度为速度变量
        OLED_ShowSignedNum(2, 7, Speed, 3); // OLED显示速度变量

        voltage = ADC_Battery_GetVoltageAvg();     // 平均电压

        OLED_ShowNum(3, 6, voltage, 3); // 显示电压
        OLED_ShowString(3, 9, "V");          // 单位
    }
}

/**
 * 函    数：TIM2中断函数
 * 参    数：无
 * 返 回 值：无
 * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
 *           函数名为预留的指定名称，可以从启动文件复制
 *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
 */
void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) // 判断是否是TIM4的更新事件触发的中断
    {
        float temp    = (float)Encoder_Get();                     // 每隔固定时间段读取一次编码器计数增量值，即为速度值
        Speed_encoder = (int16_t)(temp / 390.0f * 60.0f / 0.05f); // 将编码器计数增量值除以3，再除以48，得到的结果即为速度值
        // Speed_Control();  // 闭环控制
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update); // 清除TIM2更新事件的中断标志位
                                                    // 中断标志位必须清除
                                                    // 否则中断将连续不断地触发，导致主程序卡死
    }
}
