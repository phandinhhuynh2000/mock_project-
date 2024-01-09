#include <stdint.h>
#include "lpspi.h"
#include "common.h"
#include "clock.h"
#include "port.h"
#include "pcc.h"
#include "lpit.h"


void init_lpspi(void)
{
		
	/* set interrupt for lpspi */
	
/*---------------------- setting SCK/PCS/SOUT/SIN pin ---------------------------------------------------*/
	/* enablde pcc port E clock gate */
	PCC->PCC_PORTE.PCC_PORT.CGC = BIT_SET; 
	/* clock port E for SOUT, SIN, SCK, PCS in altinative 2  */
	/* disable pull up/down is default value */
	PORT_E->PORT_PCR0.PORT_PIN_t.MUX = ALTERNATIVE_2; /* selec port E pcr0 is SCK */
	PORT_E->PORT_PCR1.PORT_PIN_t.MUX = ALTERNATIVE_2; /* selec port E pcr0 is SIN */
	PORT_E->PORT_PCR2.PORT_PIN_t.MUX = ALTERNATIVE_2; /* selec port E pcr0 is SOUT */
	PORT_E->PORT_PCR6.PORT_PIN_t.MUX = ALTERNATIVE_2; /* selec port E pcr0 is PCS2 */
/* -------------------------------------------------------------------------------------------------------*/	
	
/*---------------------- select source lpspi -------------------------------------------------------------*/
	/* disable pcc lpspi clock gate */
	PCC->PCC_LPSPI0.PCC_LPSPI.CGC = BIT_CLEAR;
	/* selection source clock */
	PCC->PCC_LPSPI0.PCC_LPSPI.PCS = CLOCK_OPTION6; /* SPLLDIV2_clk */
	/* enable pcc lpspi clock gate */
	PCC->PCC_LPSPI0.PCC_LPSPI.CGC = BIT_SET;
/*--------------------------------------------------------------------------------------------------------*/	
	
/* --------------------- setting clock --------------------------------------------------------------------*/
	/* set device ratio of the SCK pin */
	LPSPI0->CCR.CCR_t.SCKDIV = 0x1;
/*----------------------------------------------------------------------------------------------------------*/	

/*------------------ set transmit command register--------------------------------------------------------- */
LPSPI0->TCR.REG = 0x0200000F; /* pcs: pcs2, prescale: div1, cpol = 0, cpha = 0 */
/*----------------------------------------------------------------------------------------------------------*/

/*---------------------- setting transmit/receive FIFO ------------------------------------------------------*/
  /* set receive FIFO watermark */
	LPSPI0->FCR.FCR_t.RXWATER = 0u;
	/* set transmit FIFO watermark */
	LPSPI0->FCR.FCR_t.TXWATER = 0u;
	/* set FIFO underun */
	LPSPI0->CFGR1.CFGR1_t.NOSTALL = BIT_SET;
/*------------------------------------------------------------------------------------------------------------*/

/*------------------------ configures LPSPI mode ------------------------------------------------------------ */
	/* set master mode */
	LPSPI0->CFGR1.CFGR1_t.MASTER = BIT_SET;
	/* debug enable */
	LPSPI0->CR.CR_t.DBGEN = BIT_SET;
/*-------------------------------------------------------------------------------------------------------------*/

	/* enable module */
	LPSPI0->CR.CR_t.MEN = BIT_SET;
	
}

void transmit_data(uint8_t addr, uint8_t data)
{
	uint16_t temp;
	temp = (uint16_t)((addr << 8u) | data);
	while(LPSPI0->SR.SR_t.TDF == BIT_CLEAR){}
	LPSPI0->TDR.REG_TDR = temp;
	return;
}





