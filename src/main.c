#include "stm32f10x.h"
#include "system_stm32f10x.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"

#include <stdio.h>

void periph_init();
int uart_send(int ch);

int main(void)
{
    uint16_t adc_val = 0;

    periph_init();

    volatile int i;

    // for printf
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("Hello from MC\n");

    for(;;)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_15);

        for (i = 0; i < 1000000; i++)
        {
            ;
        }

        adc_val = ADC_GetConversionValue(ADC1);

        ADC_SoftwareStartConvCmd(ADC1, ENABLE);

        printf("%d\n", adc_val);
//        uart_send('A');
//        fflush(stdout);
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

    GPIO_InitTypeDef GPIO_LED_init;

    GPIO_LED_init.GPIO_Pin = GPIO_Pin_15;
    GPIO_LED_init.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_LED_init.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB , &GPIO_LED_init);

    // init UART
    // PA2 UART2-TX
    // PA3 UART2-RX
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_UART_init;

    GPIO_UART_init.GPIO_Pin = GPIO_Pin_2;
    GPIO_UART_init.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_UART_init.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(GPIOA , &GPIO_UART_init);

    GPIO_UART_init.GPIO_Pin = GPIO_Pin_3;
    GPIO_UART_init.GPIO_Mode = GPIO_Mode_IPU;

    GPIO_Init(GPIOA , &GPIO_UART_init);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    USART_InitTypeDef USART2_Init;

    /* USARTx configured as follow:
            - BaudRate = 115200 baud
            - Word Length = 8 Bits
            - One Stop Bit
            - No parity
            - Hardware flow control disabled (RTS and CTS signals)
            - Receive and transmit enabled
    */
//    USART_StructInit(&USART2_Init);
//    USART2_Init.USART_BaudRate = 115200;

    USART2_Init.USART_BaudRate = 115200;
    USART2_Init.USART_WordLength = USART_WordLength_8b;
    USART2_Init.USART_StopBits = USART_StopBits_1;
    USART2_Init.USART_Parity = USART_Parity_No;
    USART2_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART2_Init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART2_Init);

    USART_Cmd(USART2, ENABLE);

    // init ADC
    GPIO_InitTypeDef GPIO_ADC_init;

    GPIO_ADC_init.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_ADC_init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;

    GPIO_Init(GPIOA , &GPIO_ADC_init);
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    
    
    
    
    
    // настроить все и вся в нужный режим, потом включать

    ADC_InitTypeDef ADC1_int;

    ADC1_int.ADC_Mode = ADC_Mode_Independent;
    ADC1_int.ADC_ScanConvMode = DISABLE;
    ADC1_int.ADC_ContinuousConvMode = DISABLE;
    ADC1_int.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;      // SWSTART
    ADC1_int.ADC_DataAlign = ADC_DataAlign_Right;
    ADC1_int.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC1_int);
    
    /* ADC1 regular channel14 configuration */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_28Cycles5);

    ADC_ExternalTrigConvCmd(ADC1, DISABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);  
    
    /* Enable ADC1 reset calibration register */   
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1))
        ;
    
    /* Start ADC1 calibration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1))
        ;
}


int uart_send(int ch)
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(USART2, (uint8_t) ch);

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
        ;

    return ch;
}
