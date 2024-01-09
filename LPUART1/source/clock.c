#include <stdint.h>
#include "common.h"
#include "clock.h"
#include "lpuart.h"

void set_firc_sysclk(void)
{
	/* wait till LK is 0	*/
	while(SCG_FIRC->FIRC_CSR.SCG_FIRCCSR_t.LK == BIT_SET){}
			
	/* disable firc clock */
	SCG_FIRC->FIRC_CSR.SCG_FIRCCSR_t.FIRCEN = BIT_CLEAR;

	/* enable peripheral firc div2 */	
	SCG_FIRC->FIRC_DIV.SCG_FIRCDIV_t.FIRCDIV2 = SCG_DIV2;
	
	/* enable firc and waiting till it is stable */
	SCG_FIRC->FIRC_CSR.SCG_FIRCCSR_t.FIRCEN = BIT_SET;
	while(SCG_FIRC->FIRC_CSR.SCG_FIRCCSR_t.FIRCVLD == BIT_CLEAR){}
	
	/* select clock source and configuration for RCCR from firc */
	SCG_RCCR->RCCR.REG |= ((SCG_FIRC_CLK << 24) | (SCG_DIVCORE1 << 16) | (SCG_DIVBUS2 << 4));
	
	/* check clock just configured between RCCR and SIRC */		
		while(SCG_RCCR->RCCR.REG == SCG_FIRC->FIRC_CSR.REG){}

}


void set_sirc_sysclk (void)
{
	/* wait till LK is */
	while(SCG_SIRC->SIRC_CSR.SCG_SIRCCSR_t.LK == BIT_SET){}
	
	/* check sirc is enable, if the sirc is enable, disable sirc */
	if(SCG_SIRC->SIRC_CSR.SCG_SIRCCSR_t.SIRCEN == BIT_SET)
	{
		SCG_SIRC->SIRC_CSR.SCG_SIRCCSR_t.SIRCEN = BIT_CLEAR;
	}
	else{/* do not something */}
	
	/* set frequency range for sirc */
	SCG_SIRC->SIRC_CFG.SCG_SIRCCFG_t.RANGE = BIT_SET;
	
	/* enable peripheral sirc div2 */
	SCG_SIRC->SIRC_DIV.SCG_SIRCDIV_t.SIRCDIV2 = SCG_DIV2;
	
	/* enable sirc and waiting till it is stable */
	SCG_SIRC->SIRC_CSR.SCG_SIRCCSR_t.SIRCEN = BIT_SET;
	while(SCG_SIRC->SIRC_CSR.SCG_SIRCCSR_t.SIRCVLD == BIT_SET){}
		
	/* select clock source and configuration for RCCR from sirc */
	SCG_RCCR->RCCR.REG |= ((SCG_SIRC_CLK << 24) | (SCG_DIVCORE1 << 16) | (SCG_DIVBUS2 << 4));
	
	/* check clock just configured between RCCR and SIRC */		
	while(SCG_RCCR->RCCR.REG != SCG_SIRC->SIRC_CSR.REG){}	
}

void set_sosc_sysclk(void)
{
	/* wait till lk is 0 */
	while(SCG_SOSC->SOSC_CSR.SCG_SOSCCSR_t.LK == BIT_SET){}
	
	
	/* check sirc is enable and disable */
	if(SCG_SOSC->SOSC_CSR.SCG_SOSCCSR_t.SOSCEN == BIT_SET)
	{
		SCG_SOSC->SOSC_CSR.SCG_SOSCCSR_t.SOSCEN = BIT_CLEAR;
	}
	else{/* DO NOT SOMETHING */}
	
	/* SET EREFS FOR SOSC */
	SCG_SOSC->SOSC_CFG.SCG_SOSCCFG_t.ERRES = BIT_SET;
	
	/* SET HIGH GAIN FOR SOSC */
	SCG_SOSC->SOSC_CFG.SCG_SOSCCFG_t.HIGH_GAIN = BIT_SET;
	
	/* SET RANGE FOR THE SOSC */
	SCG_SOSC->SOSC_CFG.SCG_SOSCCFG_t.RANGE = SCG_SOSC_MEDIUM_RANGE;
	
	/* enable peripheral sosc div2 */
	SCG_SOSC->SOSC_DIV.SCG_SOSCDIV_t.SOSCDIV2 = SCG_DIV2;
	
	/* enable sosc and waiting it is till stable */
	SCG_SOSC->SOSC_CSR.SCG_SOSCCSR_t.SOSCEN = BIT_SET;
	while(SCG_SOSC->SOSC_CSR.SCG_SOSCCSR_t.SOSCVLD == BIT_CLEAR){}
			
	/* select clock source for RCCR from SOSC */
	SCG_RCCR->RCCR.REG |= ((SCG_SOSC_CLK << 24) | (SCG_DIVCORE1 << 16) | (SCG_DIVBUS2 << 4));
		
	/* check clock just configured between RCCR and SOSC */	
	while(SCG_RCCR->RCCR.REG != SCG_SOSC->SOSC_CSR.REG){}
		
}

void set_spll_sysclk(void)
{
/*-------------------- configure SOSC clock source -----------------------*/
	//set_sosc_sysclk();
	/* wait till lk is 0 */
	while(SCG_SOSC->SOSC_CSR.SCG_SOSCCSR_t.LK == BIT_SET){}
	
	/* check sirc is enable and disable */
	if(SCG_SOSC->SOSC_CSR.SCG_SOSCCSR_t.SOSCEN == BIT_SET)
	{
		SCG_SOSC->SOSC_CSR.SCG_SOSCCSR_t.SOSCEN = BIT_CLEAR;
	}
	else{/* DO NOT SOMETHING */}
	
	/* SET EREFS FOR SOSC */
	SCG_SOSC->SOSC_CFG.SCG_SOSCCFG_t.ERRES = BIT_SET;
	
	/* SET HIGH GAIN FOR SOSC */
	SCG_SOSC->SOSC_CFG.SCG_SOSCCFG_t.HIGH_GAIN = BIT_SET;
	
	/* SET RANGE FOR THE SOSC */
	SCG_SOSC->SOSC_CFG.SCG_SOSCCFG_t.RANGE = SCG_SOSC_MEDIUM_RANGE;
	
	/* enable sosc and waiting it is till stable */
	SCG_SOSC->SOSC_CSR.SCG_SOSCCSR_t.SOSCEN = BIT_SET;
	while(SCG_SOSC->SOSC_CSR.SCG_SOSCCSR_t.SOSCVLD == BIT_CLEAR){}
/*------------------------------------------------------------------------*/

/*-------------------------- configure SPLL clock source -----------------*/		
	/* wait till lk is 0 */
	while(SCG_SPLL->SPLL_CSR.SCG_SPLLCSR_t.LK == BIT_SET){}
	
	/* disable SPLL clock */
	SCG_SPLL->SPLL_CSR.SCG_SPLLCSR_t.SPLLEN = BIT_CLEAR;
	
	/* configure input clock source is sosc */
	SCG_SPLL->SPLL_CFG.SCG_SPLLCFG_t.SOURCE = BIT_CLEAR;
	
	/* set prediv for SPLL */
	SCG_SPLL->SPLL_CFG.SCG_SPLLCFG_t.PREDIV = SCG_SPLL_PREDIV1;
	
	/* set multiply for SPLL */
	SCG_SPLL->SPLL_CFG.SCG_SPLLCFG_t.MULT = SCG_SPLL_MULT18;
	
	SCG_SPLL->SPLL_DIV.SCG_SPLLDIV_t.SIRCDIV2 = SCG_DIV2; /* spll_div2 = 36MHz */
	
	/* enable SPLL and waiting it is till stable */
	SCG_SPLL->SPLL_CSR.SCG_SPLLCSR_t.SPLLEN = BIT_SET;
	while(SCG_SPLL->SPLL_CSR.SCG_SPLLCSR_t.SPLLCVLD == BIT_CLEAR){}
		
	/* select clock source for RCCR from SPLL */
	SCG_RCCR->RCCR.SCG_CCR_t.DIVCORE = SCG_DIVCORE1; /* core_clock = 72MHz */
	SCG_RCCR->RCCR.SCG_CCR_t.DIVBUS = SCG_DIVBUS2; /* bus_clock = 36MHz */	
	SCG_RCCR->RCCR.SCG_CCR_t.DIVSLOW = SCG_DIVSLOW3; /* flash clock = 24Mh	*/	
	SCG_RCCR->RCCR.SCG_CCR_t.SCS = SCG_CLKOUT_SPLL_CLK;	
		
	/* check clock just configured between RCCR and SPLL */	
	while(SCG_RCCR->RCCR.REG == SCG_SPLL->SPLL_CSR.REG){}
	
/*---------------------------------------------------------------------*/	
}




