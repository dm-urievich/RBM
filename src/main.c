#define STM32F10X_MD
#include "stm32f10x.h"
//#include "system_stm32f10x.h"

//#include "stm32f10x_gpio.h"

void SystemInit (void)
{

}

int main(void)
{

    RCC->APB2ENR	|= RCC_APB2ENR_IOPBEN;		// Подали такты на порт. Без этого работать не будет.

    // настройка PB15 на выход для мигания светодиодиком

    GPIOB->CRH &= ~GPIO_CRH_CNF15;		// Обнулили биты CNF3 т.к. после старта они настроены как (01)
                                        // режим Open Drain. А нам надо Push-Pull (00)

    GPIOB->CRH |= GPIO_CRH_MODE15_0 | GPIO_CRH_MODE15_1;		// Настроили порт на выход


    volatile int i;

    for(;;)
    {

        GPIOB->BSRR |= GPIO_BSRR_BS15;

        for (i = 0; i < 100000; i++)
        {
            ;
        }

        GPIOB->BSRR |= GPIO_BSRR_BR15;

        for (i = 0; i < 100000; i++)
        {
            ;
        }

    }

    return 0;
}

