#include <stdint.h>
#include "lpuart.h"
#include <string.h>
#include "pcc.h"
#include "nvic.h"
#include "clock.h"
#include "port.h"

void init_lpuart(void)
{
	/* source clock */
	set_spll_sysclk();
/*-----------------------------setting Tx/Rx pin ------------------------------- */	
	/* enable pcc portC clock gate */
	PCC->PCC_PORTC.PCC_PORT.CGC = BIT_SET;
	/* set alternate function pin */
	PORT_C->PORT_PCR6.PORT_PIN_t.MUX = ALTERNATIVE_2; /* select alternative 2 */
	PORT_C->PORT_PCR7.PORT_PIN_t.MUX = ALTERNATIVE_2;
	
/*------------------------------------------------------------------------------*/

/*---------------------------- select srouce Lpuart ----------------------------*/	
	/* disable pcc lpuart clock */
	PCC->PCC_LPUART1.PCC_LPUART.CGC = BIT_CLEAR;
	/* set selection clock */
	PCC->PCC_LPUART1.PCC_LPUART.PCS = CLOCK_OPTION3;
	/* enable pcc lpuart clock gate */
	PCC->PCC_LPUART1.PCC_LPUART.CGC = BIT_SET;

/*-------------------------------------------------------------------------------*/
	
/*--------------------------- setting baud rate ---------------------------------*/	
	/* set modulo devide rate */
	/* setting baud rate = 38400 */
	LPUART1->LPUART_BAUD.LPUART_BAUD_t.LPUART_BAUD_SBR = 0x3F;
	/* choose oversampling ratio */
	/* set oversampling = 15 */
	LPUART1->LPUART_BAUD.LPUART_BAUD_t.LPUART_BAUD_OSR = OVERSAMPLING_RATIO_15;
/*--------------------------------------------------------------------------------*/
	
/*--------------------------- setting frame---------------------------------------*/	
	/* stop bit number is 1-bit */
	LPUART1->LPUART_BAUD.LPUART_BAUD_t.LPUART_BAUD_SBNS = BIT_CLEAR;	
	/* set data character number is 9-bit  */
	LPUART1->LPUART_CTRL.LPUART_CTRL_t.LPUART_CTRL_M = BIT_CLEAR;
	/* enable parity bit */
	LPUART1->LPUART_CTRL.LPUART_CTRL_t.LPUART_CTRL_PE = BIT_SET;
	/* select parity type is even */
	LPUART1->LPUART_CTRL.LPUART_CTRL_t.LPUART_CTRL_PT = BIT_CLEAR;
/*---------------------------------------------------------------------------------*/

/*-------------------------- enable transmitter & receiver ------------------------*/
	/* enable transmiiter */
	LPUART1->LPUART_CTRL.LPUART_CTRL_t.LPUART_CTRL_TE = BIT_SET;
	/* enable receiver */
	LPUART1->LPUART_CTRL.LPUART_CTRL_t.LPUART_CTRL_RE = BIT_SET;

/*---------------------------------------------------------------------------------*/
			
}

void init_lpuart_interrupt(void)
{
	/* enable nvic lpuart */
	set_enable_nvic_lpuart_transmit();
	
/*-----------------------------setting Tx/Rx pin ------------------------------- */	
	/* enable pcc portC clock gate */
	PCC->PCC_PORTC.PCC_PORT.CGC = BIT_SET;
	/* set alternate function pin */
	PORT_C->PORT_PCR6.PORT_PIN_t.MUX = ALTERNATIVE_2; /* select alternative 2 */
	PORT_C->PORT_PCR7.PORT_PIN_t.MUX = ALTERNATIVE_2;
	
/*------------------------------------------------------------------------------*/

/*---------------------------- select srouce Lpuart ----------------------------*/		
	/* disable pcc lpuart clock */
	PCC->PCC_LPUART1.PCC_LPUART.CGC = BIT_CLEAR;
	/* set selection clock */
	PCC->PCC_LPUART1.PCC_LPUART.PCS = CLOCK_OPTION6;
	/* enable pcc lpuart clock gate */
	PCC->PCC_LPUART1.PCC_LPUART.CGC = BIT_SET;
/*------------------------------------------------------------------------------*/


/*--------------------------- setting baud rate ---------------------------------*/	
	/* set modulo devide rate */
	/* baud rate: 38400, 8 bit data, even parity, 1 stop bit */
	LPUART1->LPUART_BAUD.LPUART_BAUD_t.LPUART_BAUD_SBR = 0x3F;
	/* choose oversampling ratio */
	LPUART1->LPUART_BAUD.LPUART_BAUD_t.LPUART_BAUD_OSR = OVERSAMPLING_RATIO_15;
/*--------------------------------------------------------------------------------*/	


/*--------------------------- setting frame---------------------------------------*/	

	/* stop bit number */
	LPUART1->LPUART_BAUD.LPUART_BAUD_t.LPUART_BAUD_SBNS = BIT_CLEAR;	
	/* set data character number is 9-bit  */
	LPUART1->LPUART_CTRL.LPUART_CTRL_t.LPUART_CTRL_M = BIT_SET; 
	/* enable parity bit */
	LPUART1->LPUART_CTRL.LPUART_CTRL_t.LPUART_CTRL_PE = BIT_SET;
	/* select parity type is even */
	LPUART1->LPUART_CTRL.LPUART_CTRL_t.LPUART_CTRL_PT = BIT_CLEAR;
/*---------------------------------------------------------------------------------*/

/*--------------------------- enable receiver interrupt -------------------------- */
	/* receive interrupt enable */
	LPUART1->LPUART_CTRL.LPUART_CTRL_t.LPUART_CTRL_RIE = BIT_SET;
/*---------------------------------------------------------------------------------*/

/*-------------------------- enable transmitter & receiver ------------------------*/
	/* enable transmiiter */
	LPUART1->LPUART_CTRL.LPUART_CTRL_t.LPUART_CTRL_TE = BIT_SET;
	/* enable receiver */
	LPUART1->LPUART_CTRL.LPUART_CTRL_t.LPUART_CTRL_RE = BIT_SET;

/*---------------------------------------------------------------------------------*/

}


void transmitter_1_character(unsigned char character)
{
	/* wait for transmit buffer to be empty  */
	while(LPUART1->LPUART_STAT.LPUART_STAT_t.LPUART_STAT_TDRE == 0){}
	
	/* send data */	
	LPUART1->LPUART_DATA.LPUART_DATA_t.LPUART_DATA_R1T1 = character;
}

void transmitter_1_string(volatile unsigned char character[])
{
	uint32_t i = 0;
	/* wait for transmit buffer to be empty  */
	while(LPUART1->LPUART_STAT.LPUART_STAT_t.LPUART_STAT_TDRE == 0){}
	
	/* send data */	
	
	while(character[i] != '\0' )
	{
		LPUART1->LPUART_DATA.LPUART_DATA_t.LPUART_DATA_R1T1 = character[i];
		i++;
	}
}

char receive_data(void)
{
	char receive_data;
	/* wait for buffer is full */
	while(LPUART1->LPUART_STAT.LPUART_STAT_t.LPUART_STAT_RDRF == 1){}
	
	/* receive data */	
	receive_data = LPUART1->LPUART_DATA.LPUART_DATA_t.LPUART_DATA_R1T1;

	return receive_data;
}

