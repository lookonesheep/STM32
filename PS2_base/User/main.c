#include <stdio.h>
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "pstwo.h"

//------------接线说明---------------
// PS2接收器--------STM32F1主板1引脚

//    VCC     -----------    V3.3
//    GND     -----------    GND
//    DI、DAT      --------- PA3
//    DO、CMD      ----------PA2
//    CS      -----------    PA1
//    CLK     -----------    PA0

int PS2_LX, PS2_LY, PS2_RX, PS2_RY, PS2_KEY;
int main(void)
{
    SystemInit();    //======配置系统时钟为72M
    uart_init(9600); //======串口初始化
    Delay_ms(1000);  //======延时等待初始化稳定
    PS2_Init();      //======ps2驱动端口初始化
    PS2_SetInit();   //======ps2配置初始化,配置“红绿灯模式”，并选择是否可以修改
    Delay_ms(1000);
    while (1) {
        PS2_LX  = PS2_AnologData(PSS_LX);
        PS2_LY  = PS2_AnologData(PSS_LY);
        PS2_RX  = PS2_AnologData(PSS_RX);
        PS2_RY  = PS2_AnologData(PSS_RY);
        PS2_KEY = PS2_DataKey();
        printf("%d     PS2_LX:", PS2_LX);
        printf("%d     PS2_LY:", PS2_LY);
        printf("%d     PS2_RX:", PS2_RX);
        printf("%d     PS2_RY:", PS2_RY);
        printf("%d \r\nPS2_KEY:", PS2_KEY);



        // switch (PS2_KEY) {
        //     case 5:
        //         LED2_TOGGLE;
        //         break;
        // }
        Delay_ms(500);
    }
}