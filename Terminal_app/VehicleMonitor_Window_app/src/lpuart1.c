/*******************************************************************************
* Include
*******************************************************************************/
#include "lpuart1.h"
/*******************************************************************************
* Define
*******************************************************************************/
#define FEATURE_INTERRUPT_IRQ_MAX (FTM3_Ovf_Reload_IRQn)
extern uint32_t __VECTOR_RAM[((uint32_t)(FEATURE_INTERRUPT_IRQ_MAX)) + 16U + 1U];
/*******************************************************************************
* Global variable
*******************************************************************************/
volatile char g_temp;
volatile callback g_callback_func;
/*******************************************************************************
* Prototype
*******************************************************************************/
void cacul_baudrate(uint32_t clock, uint32_t baud, uint16_t *OSR, uint16_t *SBR);

void enable_irq(IRQn_Type irqNumber);

void LPUART1_RxTx_IRQHandler(void);

void send_character(char character);

/*******************************************************************************
* Code
*******************************************************************************/
void init_uart(const uint32_t _baud_rate)
{
	uint16_t OSR = 0;
	uint16_t SBR = 0;

	/* enable port C for UART (RX - PTC6, TX - PTC7) */
	IP_PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC(1);
	/* PTC6 ~ RX */
	IP_PORTC->PCR[6] |= PORT_PCR_MUX(2); /*ATL2*/
	/* PTC7 ~ TX */
	IP_PORTC->PCR[7] |= PORT_PCR_MUX(2); /*ATL2*/

	/* choose clock for UART is FIRC */
	IP_PCC->PCCn[PCC_LPUART1_INDEX] &= ~PCC_PCCn_CGC_MASK;
	IP_PCC->PCCn[PCC_LPUART1_INDEX] |= PCC_PCCn_PCS(3);
	IP_PCC->PCCn[PCC_LPUART1_INDEX] |= PCC_PCCn_CGC_MASK;

	/* FIRCDIV2 divider by 1 */
	IP_SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(1);

	/*disable TX, RX*/
	IP_LPUART1->CTRL &= ~LPUART_CTRL_RE_MASK;
	IP_LPUART1->CTRL &= ~LPUART_CTRL_TE_MASK;

	/* select 8 bit data*/
	IP_LPUART1->BAUD &= ~LPUART_BAUD_M10_MASK;   /*M10*/
	IP_LPUART1->CTRL &= ~LPUART_CTRL_M7_MASK;    /*M7*/
	IP_LPUART1->CTRL &= ~LPUART_CTRL_M_MASK;     /*M*/

	/* select none parity */
	IP_LPUART1->CTRL &= ~LPUART_CTRL_PE_MASK;

	/*LSB first*/
	IP_LPUART1->STAT &= ~LPUART_STAT_MSBF_MASK;

	/*receive not inverted*/
	IP_LPUART1->STAT &= ~LPUART_STAT_RXINV_MASK;

	/*NO inverted*/
	IP_LPUART1->CTRL &= ~LPUART_CTRL_TXINV_MASK;

	/* select 1 bit stop */
	IP_LPUART1->BAUD &= ~LPUART_BAUD_SBNS_MASK;

	/* configuration baud rate */
	/*Using clock = FIRC = 48Mhz*/
	cacul_baudrate(48000000, _baud_rate, &OSR, &SBR);
	IP_LPUART1->BAUD &= ~LPUART_BAUD_OSR_MASK;
	IP_LPUART1->BAUD |= LPUART_BAUD_OSR(31);

	IP_LPUART1->BAUD &= ~LPUART_BAUD_SBR_MASK;
	IP_LPUART1->BAUD |= LPUART_BAUD_SBR(156);

	/* enable receive */
	IP_LPUART1->CTRL |= LPUART_CTRL_RE(1);

	/* enable transmit */
	IP_LPUART1->CTRL |= LPUART_CTRL_TE(1);

	/* enable interrupt */
	IP_LPUART1->CTRL |= LPUART_CTRL_RIE(1);

	enable_irq(LPUART1_RxTx_IRQn);

	/*register handle*/
	uint32_t * pVectorRam = (uint32_t *)__VECTOR_RAM;
	pVectorRam[((int32_t)LPUART1_RxTx_IRQn) + 16] = (uint32_t)(&LPUART1_RxTx_IRQHandler);
}

void LPUART1_send_character(char character)
{
	while((IP_LPUART1->STAT & LPUART_STAT_TDRE_MASK)>>LPUART_STAT_TDRE_SHIFT==0);
	/* Wait for transmit buffer to be empty */
	IP_LPUART1->DATA = character;              /* Send data */
}

void enable_irq(IRQn_Type irqNumber)
{
	S32_NVIC->ISER[(uint32_t)(irqNumber) >> 5U] = (uint32_t)(1UL << ((uint32_t)(irqNumber) & (uint32_t)0x1FU));
}

void cacul_baudrate(uint32_t clock, uint32_t baud,
                   uint16_t *OSR, uint16_t *SBR)
{
    uint32_t best_err = UINT32_MAX;
    uint32_t best_osr = 0, best_sbr = 0;

    for (uint32_t osr_reg = 31; osr_reg >= 4; osr_reg--)
    {
        uint32_t osr = osr_reg + 1;

        uint64_t denom = (uint64_t)baud * osr;
        uint32_t sbr = clock / denom;
        if (sbr == 0 || sbr > 0x1FFF) continue;

        for (uint32_t s = sbr; s <= sbr + 1; s++)
        {
            uint32_t actual = clock / (s * osr);
            uint32_t err = (baud > actual) ? (baud - actual) : (actual - baud);

            if (err < best_err)
            {
                best_err = err;
                best_osr = osr_reg;
                best_sbr = s;
            }
        }
    }

    *OSR = best_osr;
    *SBR = best_sbr;
}


void LPUART1_RxTx_IRQHandler(void)
{
	while((IP_LPUART1->STAT & LPUART_STAT_RDRF_MASK)>>LPUART_STAT_RDRF_SHIFT==0);
	/* Wait for received buffer to be full */
	g_temp= IP_LPUART1->DATA;            /* Read received data*/
	if(g_temp != '\0')
	{
		g_callback_func(g_temp);

	}
}

void register_take_data_from_uart(callback _callback_func)
{
	if(_callback_func != NULL)
	{
		g_callback_func = _callback_func;
	}
}

void LPUART1_print_number(uint32_t value)
{
    char buf[10];
    int i = 0;

    if (value == 0) {
    	LPUART1_send_character('0');
        return;
    }

    while (value > 0) {
        buf[i++] = (value % 10) + '0';
        value /= 10;
    }

    while (i--) {
    	LPUART1_send_character(buf[i]);
    }
}

void LPUART1_send_string(const char *data_string)
{
    uint32_t i = 0;
    while (data_string[i] != '\0') {
    	LPUART1_send_character(data_string[i]);
        i++;
    }
}

void LPUART1_send_string_v2(char *data_string, uint16_t length)
{
	int index = 0;
	for(index = 0; index < length; index++)
	{
		LPUART1_send_character(data_string[index]);
	}
}
/*******************************************************************************
* EOF
*******************************************************************************/

