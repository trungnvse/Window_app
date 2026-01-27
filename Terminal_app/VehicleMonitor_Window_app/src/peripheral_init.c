/*******************************************************************************
* Include
*******************************************************************************/
#include "S32K144.h"
#include "peripheral_init.h"
/*******************************************************************************
* Code
*******************************************************************************/
void PORT_Init(void)
{
    /* Enable clock for Port D */
    IP_PCC->PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC_MASK;

    /* Configure PTD15 as GPIO (RED LED) */
    IP_PORTD->PCR[15] = PORT_PCR_MUX(0b001);
    IP_PTD->PDDR |= (1 << 15); /* Output */
    IP_PTD->PSOR = (1U << 15); /* Turn off LED (active low) */

    /* Enable clock for Port B (for ADC) */
    IP_PCC->PCCn[PCC_PORTB_INDEX] = PCC_PCCn_CGC_MASK;

    /* Configure PTB0 as ADC input (no need to set MUX, default is analog) */
    IP_PORTB->PCR[0] = 0; /* Analog mode */
}

void convertAdcChan(uint8_t adcChan)
{
    /* Clear prior ADCH bits */
    IP_ADC0->SC1[0] &= ~ADC_SC1_ADCH_MASK;

    /* Start conversion with interrupt enabled */
    IP_ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(adcChan);
}

void LPIT0_init_100ms(void)
{
    /* Disable clock to change settings */
    IP_PCC->PCCn[PCC_LPIT_INDEX] &= (~PCC_PCCn_CGC_MASK);

    /* Clock Source = SPLL2_DIV2_CLK = 40MHz */
    IP_PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_PCS(0b110) | PCC_PCCn_CGC_MASK;

    /* Enable LPIT module */
    IP_LPIT0->MCR |= LPIT_MCR_M_CEN_MASK;

    /* Stop timer in Debug mode */
    IP_LPIT0->MCR &= ~LPIT_MCR_DBG_EN_MASK;

    /* Enable Timer Interrupt for Channel 0 */
    IP_LPIT0->MIER = LPIT_MIER_TIE0_MASK;

    /* Set timer value for 100ms
     * Clock = 40MHz (SPLL2_DIV2_CLK)
     * Period = 100ms = 0.1s
     * TVAL = 40,000,000 * 0.1 = 4,000,000
     */
    IP_LPIT0->TMR[0].TVAL = 4000000;

    /* Configure timer:
     * CHAIN = 0 (not chained)
     * MODE = 0 (Periodic Counter)
     * TSOI = 0 (does not stop after timeout)
     * TSOT = 0 (does not start on trigger)
     */
    IP_LPIT0->TMR[0].TCTRL &= ~(LPIT_TMR_TCTRL_CHAIN_MASK |
                             LPIT_TMR_TCTRL_MODE_MASK |
                             LPIT_TMR_TCTRL_TSOI_MASK |
                             LPIT_TMR_TCTRL_TSOT_MASK);

    /* Start timer */
    IP_LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK;
}

void ADC0_Init(void)
{
    /* Enable clock for ADC0 */
    IP_PCC->PCCn[PCC_ADC0_INDEX] &= ~PCC_PCCn_CGC_MASK;
    IP_PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_CGC_MASK |
                                  PCC_PCCn_PCS(0b001); /* SOSCDIV2 = 8MHz */

    /* Disable ADC module first */
    IP_ADC0->SC1[0] = ADC_SC1_AIEN_MASK |    /* Enable interrupt */
                   ADC_SC1_ADCH(0x3F);    /* Disable module */

    /* Configure ADC:
     * MODE = 0b01: 12-bit resolution
     * ADICLK = 0: Input clock = ALTCLK1 = SOSCDIV2
     * ADIV = 0: Prescaler = 1
     */
    IP_ADC0->CFG1 = ADC_CFG1_MODE(1) | ADC_CFG1_ADICLK(0);

    /* Sample time = 13 ADC clocks */
    IP_ADC0->CFG2 = ADC_CFG2_SMPLTS(12);

    /* Configure conversion:
     * ADCO = 0: One conversion
     * AVGE = 1: Hardware average enabled
     * AVGS = 0b01: 8 samples averaged
     */
    IP_ADC0->SC3 = ADC_SC3_ADCO(0) |
                ADC_SC3_AVGE_MASK |
                ADC_SC3_AVGS(0b01);

    /* Configure trigger:
     * ADTRG = 0: Software trigger
     * DMAEN = 0: DMA disabled
     * ACFE = 0: Compare function disabled
     * REFSEL = 0: VREFH and VREFL
     */
    IP_ADC0->SC2 &= ~(ADC_SC2_ADTRG_MASK |
                   ADC_SC2_DMAEN_MASK |
                   ADC_SC2_ACFE_MASK |
                   ADC_SC2_REFSEL_MASK);
}
/*******************************************************************************
* EOF
*******************************************************************************/
