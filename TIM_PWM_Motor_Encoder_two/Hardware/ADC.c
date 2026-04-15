#include "stm32f10x.h"
#include "Delay.h"
#include "ADC.h"

/**
  * 函    数：ADC初始化（PA0 -> ADC1_CH0）
  */
void ADC_Battery_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);

    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));
}

/**
  * 函    数：读取单次ADC数值
  */
uint16_t ADC_Battery_GetValue(void)
{
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    return ADC_GetConversionValue(ADC1);
}

/**
  * 函    数：读取电池电压（分压比1:11）
  */
float ADC_Battery_GetVoltage(void)
{
    uint16_t adcValue = ADC_Battery_GetValue();
    float Vadc = (float)adcValue * 3.3f / 4095.0f; // 转换为实际电压
    float Vbat = Vadc * 11.0f;                     // 反推电池电压
    return Vbat;
}

/**
  * 函    数：多次采样平均电压
  */
float ADC_Battery_GetVoltageAvg(void)
{
    float sum = 0;
    for (uint8_t i = 0; i < 10; i++)
    {
        sum += ADC_Battery_GetVoltage();
        Delay_ms(10);
    }
    return sum / 10.0f;
}

/**
  * 函    数：电压转百分比（适用于12V铅酸/锂电系统）
  */
uint8_t ADC_Battery_GetPercent(float voltage)
{
    if (voltage <= 11.0f) return 0;
    if (voltage >= 12.6f) return 100;
    return (uint8_t)((voltage - 11.0f) / (12.6f - 11.0f) * 100);
}
