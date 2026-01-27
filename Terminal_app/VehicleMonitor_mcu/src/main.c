/*******************************************************************************
* Include
*******************************************************************************/
#include "S32K144.h"
#include "clock_init.h"
#include "peripheral_init.h"
#include "lpuart1.h"
#include <stdbool.h>
#include <stdio.h>
/*******************************************************************************
* Define
*******************************************************************************/
#define PTD15 15 /* RED LED */
#define ADC_CHANNEL 12 /* ADC0 channel 12 - PTB0 */
/*******************************************************************************
* Global variable
*******************************************************************************/
volatile uint16_t g_adc_result = 0;  /* Store ADC conversion result */
volatile bool g_adc_ready = false;   /* Flag to indicate ADC data is ready */
/*******************************************************************************
* Code
*******************************************************************************/
/* Initialize NVIC interrupts for LPIT0 and ADC0 */
void NVIC_IRQ_Init(void)
{
    /* Clear pending interrupt for LPIT0 Channel 0 */
    S32_NVIC->ICPR[LPIT0_Ch0_IRQn / 32] = 1U << (LPIT0_Ch0_IRQn % 32);
    /* Enable LPIT0 Channel 0 interrupt */
    S32_NVIC->ISER[LPIT0_Ch0_IRQn / 32] = 1U << (LPIT0_Ch0_IRQn % 32);
    /* Set priority for LPIT0 interrupt */
    S32_NVIC->IP[LPIT0_Ch0_IRQn] = 0x05;

    /* Clear pending interrupt for ADC0 */
    S32_NVIC->ICPR[ADC0_IRQn / 32] |= (1U << (ADC0_IRQn % 32));
    /* Enable ADC0 interrupt */
    S32_NVIC->ISER[ADC0_IRQn / 32] |= (1U << (ADC0_IRQn % 32));
    /* Set priority for ADC0 interrupt */
    S32_NVIC->IP[ADC0_IRQn] = 0x06;

    /* Enable global interrupts */
    __asm volatile("cpsie i");
}

/* Convert ADC value to voltage and send via UART */
void send_adc_via_uart(uint16_t adc_value)
{
    /* Convert ADC value (0-4095) to voltage (0-5V) */
    float voltage = (adc_value / 4095.0f) * 5.0f;

    /* Buffer to store formatted string */
    char buffer[32];

    /* Format voltage value to 4 decimal places */
    sprintf(buffer, "%.4f\r\n", voltage);

    /* Send string through UART */
    LPUART1_send_string(buffer);
}

int main(void)
{
    /* Initialize system oscillator */
    SOSC_Init();
    /* Initialize PLL */
    SPLL_Init();
    /* Configure run mode */
    NormalRUNmode();
    /* Initialize GPIO ports */
    PORT_Init();
    /* Initialize UART with 9600 baud rate */
    init_uart(9600);
    /* Initialize ADC0 */
    ADC0_Init();
    /* Initialize LPIT0 timer for 100ms period */
    LPIT0_init_100ms();
    /* Enable interrupts */
    NVIC_IRQ_Init();

    /* Send startup messages */
    LPUART1_send_string("S32K144 ADC to UART\r\n");
    LPUART1_send_string("Sending ADC data every 100ms\r\n");

    /* Main loop */
    while(1)
    {
        /* Check if new ADC data is ready */
        if (g_adc_ready)
        {
            /* Clear the ready flag */
            g_adc_ready = false;
            /* Send ADC value via UART */
            send_adc_via_uart(g_adc_result);
            /* Toggle RED LED */
            IP_PTD->PTOR = (1U << PTD15);
        }
    }

    return 0;
}

/* ==================== INTERRUPT HANDLERS ==================== */

/* LPIT0 Channel 0 interrupt handler - triggers every 100ms */
void LPIT0_Ch0_IRQHandler(void)
{
    /* Clear timer interrupt flag */
    IP_LPIT0->MSR |= LPIT_MSR_TIF0_MASK;

    /* Clear pending interrupt in NVIC */
    S32_NVIC->ICPR[1] = 1 << (LPIT0_Ch0_IRQn % 32);

    /* Start ADC conversion on specified channel */
    convertAdcChan(ADC_CHANNEL);
}

/* ADC0 interrupt handler - called when conversion is complete */
void ADC0_IRQHandler(void)
{
    /* Read ADC conversion result (12-bit value) */
    g_adc_result = (uint16_t)(IP_ADC0->R[0] & 0xFFFFU);

    /* Disable ADC channel to stop conversion */
    IP_ADC0->SC1[0] |= ADC_SC1_ADCH(0x3F);

    /* Clear pending interrupt in NVIC */
    S32_NVIC->ICPR[ADC0_IRQn / 32] |= (1U << (ADC0_IRQn % 32));

    /* Set flag to indicate ADC data is ready */
    g_adc_ready = true;
}
/*******************************************************************************
* EOF
*******************************************************************************/
