/*******************************************************************************
* Include
*******************************************************************************/
#include "S32K144.h"
#include "clock_init.h"
/*******************************************************************************
* Code
*******************************************************************************/
void SOSC_Init(void)
{
	IP_SCG->SOSCCSR &= (~SCG_SOSCCSR_SOSCEN_MASK); /* disable sosc */
	/* config range, hgo, erefs */
	IP_SCG->SOSCCFG |= SCG_SOSCCFG_RANGE(3) | SCG_SOSCCFG_HGO(0) | SCG_SOSCCFG_EREFS(1);
	/*
	 * RANGE = 3   frequency = 8MHz
	 * HGO = 0     high gain
	 * erefs = 1   exteral clock XTAL
	 * */
	IP_SCG->SOSCDIV |= (SCG_SOSCDIV_SOSCDIV1(1) | SCG_SOSCDIV_SOSCDIV2(1)); /* SOSCDIV1 = SOSCDIV2 = 8 MHz */
	while(IP_SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK); /* Ensure SOSCCSR unlocked */

	/* enable sosc */
	IP_SCG->SOSCCSR |= SCG_SOSCCSR_LK(0) |           /* unlock register, register can be written*/
			        SCG_SOSCCSR_SOSCEN(1);        /* trigger sosc, enable oscillator */


	while(!(IP_SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK)); /* Wait for sys OSC clk valid */
}

void SPLL_Init(void)
{

	while(IP_SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK); /* Ensure SPLLCSR unlocked */
	IP_SCG->SPLLCSR = 0x00000000; /*
									SPLLEN = 0 : disable SPLL to config
									LK = 0 : register can be written
	 	 	 	 	 	 	 	*/

	IP_SCG->SPLLCFG |= SCG_SPLLCFG_PREDIV(0) |     /* PREDIV  = 0 */
			        SCG_SPLLCFG_MULT(0b11000);   /* MULT = 24 */
	/*     VCO_CLOCK = 8/(0+1) * (24+16) = 320MHz
	       SPLL_CLOCK = VCO_CLOCK/2 = 160*/


	IP_SCG->SPLLDIV = SCG_SPLLDIV_SPLLDIV1(0b010) |  /* SPLLDIV1 = SPLL_CLOCK/2 = 80MHz */
				   SCG_SPLLDIV_SPLLDIV2(0b011);  /* SPLLDIV2 = SPLL_CLOCK/4 = 40MHz */

	while(IP_SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK); /* Ensure SPLLCSR unlocked */

	IP_SCG->SPLLCSR = SCG_SPLLCSR_SPLLEN(1); /* SPLLEN=1: Enable SPLL */
	while (!(IP_SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK)); /* Wait for SPLL valid */
}
void NormalRUNmode(void)
{
	IP_SCG->RCCR  = SCG_RCCR_SCS(0b0110)   |          /* select SPLL source */
			     SCG_RCCR_DIVCORE(0b01) |          /* CORE_CLK = SPLL_CLK/2 = 80  */
				 SCG_RCCR_DIVBUS(0b01)  |          /* BUS_CLK = CORE_ClK/2 = 40MHz */
				 SCG_RCCR_DIVSLOW(0b10) ;          /* FLASH_CLK = CORE_CLK/3  */
	while(((IP_SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT) != 6){}
}
/*******************************************************************************
* EOF
*******************************************************************************/
