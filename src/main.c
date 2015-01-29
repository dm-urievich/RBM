#include "stm32f10x.h"
#include "system_stm32f10x.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

void periph_init();

int main(void)
{
    periph_init();

    volatile int i;

    for(;;)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_15);

        for (i = 0; i < 1000000; i++)
        {
            ;
        }

        GPIO_ResetBits(GPIOB, GPIO_Pin_15);

        for (i = 0; i < 1000000; i++)
        {
            ;
        }
    }

    return 0;
}

void periph_init()
{
    // init GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB , &GPIO_InitStructure);

    // init UART


    // init ADC
}
