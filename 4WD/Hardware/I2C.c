#include "i2c.h"
#include "stm32f10x.h"

#define I2C_SPEED       100000     // I2C 通信速率 100kHz
#define I2C_OWN_ADDR    0x00       // STM32 自身地址（主机模式可随意设置）

/**
  * @brief  I2C1 初始化
  * @note   使用 PB6(SCL), PB7(SDA)
  */
void I2C1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef  I2C_InitStructure;

    /* 开启 GPIOB 和 I2C1 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    /* 配置 PB6, PB7 为开漏复用输出 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* I2C 外设配置 */
    I2C_DeInit(I2C1);
    I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = I2C_OWN_ADDR;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitStructure);

    /* 使能 I2C1 */
    I2C_Cmd(I2C1, ENABLE);
}

/**
  * @brief  I2C 写一个字节到指定寄存器
  */
void I2C_WriteReg(uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
    /* 等待总线空闲 */
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    /* 起始信号 */
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    /* 发送设备地址 + 写方向 */
    I2C_Send7bitAddress(I2C1, devAddr<<1, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* 发送寄存器地址 */
    I2C_SendData(I2C1, regAddr);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* 发送数据 */
    I2C_SendData(I2C1, data);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* 停止信号 */
    I2C_GenerateSTOP(I2C1, ENABLE);
}

/**
  * @brief  I2C 读寄存器（多字节）
  */
void I2C_ReadReg(uint8_t devAddr, uint8_t regAddr, uint8_t *buf, uint8_t len)
{
    uint8_t i;

    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    /* 起始信号（写） */
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    /* 发送设备地址（写） */
    I2C_Send7bitAddress(I2C1, devAddr<<1, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* 发送寄存器地址 */
    I2C_SendData(I2C1, regAddr);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* 重新起始信号（读） */
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    /* 发送设备地址（读） */
    I2C_Send7bitAddress(I2C1, devAddr<<1, I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    /* 循环接收数据 */
    for(i=0; i<len; i++)
    {
        if(i == (len-1))
        {
            /* 最后一个字节 -> 关闭应答并发送停止信号 */
            I2C_AcknowledgeConfig(I2C1, DISABLE);
            I2C_GenerateSTOP(I2C1, ENABLE);
        }
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
        buf[i] = I2C_ReceiveData(I2C1);
    }

    /* 重新打开 ACK，方便下次接收 */
    I2C_AcknowledgeConfig(I2C1, ENABLE);
}
