#include <stdint.h>
#include "port.h"
#include "gpio.h"
#include "common.h"
#include "nvic.h"
#include "lpit.h"
#include "lpuart.h"
#include "main.h"
#include "lpspi.h"

volatile static uint32_t time_alarm = 0;
volatile static uint32_t time_second = 0;
volatile static uint8_t intensity_lv;



void PORTD_IRQHandler(void)
{
	
	GPIO_D->PTOR.FIELD.P16 = BIT_SET;
}

void LPIT0_Ch0_IRQHandler(void)
{
	if(LPIT->MSR.LPIT_MSR_t.TIF0 == BIT_SET)
	{
		LPIT->MSR.LPIT_MSR_t.TIF0 = BIT_SET;
		if((time_alarm == 20) || (PORT_C->PORT_PCR12.PORT_PIN_t.ISF = 1) || (PORT_C->PORT_PCR13.PORT_PIN_t.ISF = 1))
		{
			LPIT->TCTRL0.LPIT_TCTRLn_t.T_EN = BIT_CLEAR;
			GPIO_D->PTOR.FIELD.P16 = BIT_SET;
			time_alarm = 0;
		}
		else
		{
		LPIT->MSR.LPIT_MSR_t.TIF0 = BIT_SET;
		GPIO_D->PTOR.FIELD.P16 = BIT_SET;
		time_alarm++;
		}
		/* delay 1 second when in mode time_display */
		if(time_second == 2)
		{
			LPIT->TCTRL0.LPIT_TCTRLn_t.T_EN = BIT_CLEAR;
			time_second = 0;
		}
		else
		{
			LPIT->TCTRL0.LPIT_TCTRLn_t.T_EN = BIT_SET;
			time_second++;
		}
	}
}


void PORTC_IRQHandler(void)
{
	if(PORT_C->PORT_PCR12.PORT_PIN_t.ISF == BIT_SET)
	{
		if(mode_display == time_display)
		{
			mode_display = alarm_display;
		}
		else if(mode_display == alarm_display)
		{
			mode_display = date_display;
		}
		else if(mode_display == date_display)
		{
			mode_display = time_display;
		}
		else {}
	}	
	PORT_C->PORT_PCR13.PORT_PIN_t.ISF = BIT_SET;
	
	if(PORT_C->PORT_PCR13.PORT_PIN_t.ISF == BIT_SET)
	{
		if(mode_display == alarm_display)
		{
			if(alarmonof == 1)
			{
				alarmonof = 0;
			}
			else if (alarmonof == 0)
			{
				alarmonof = 1;
			}
		}
		else 
		{
			switch(intensity_lv)
			{
				case intensity_1:
				{
					mode_display = time_display;
					intensity_lv = intensity_1;
					transmit_data(intensity, intensity_lv);
					break;
				}
				case intensity_2:
				{
					intensity_lv = intensity_2;
					transmit_data(intensity, intensity_lv);
					break;
				}
				case intensity_3:
				{
					intensity_lv = intensity_3;
					transmit_data(intensity, intensity_lv);
					break;
				}
				case intensity_4:
				{
					intensity_lv = intensity_4;
					transmit_data(intensity, intensity_lv);
					break;
				}
				case intensity_5:
				{
					intensity_lv = intensity_5;
					transmit_data(intensity, intensity_lv);
					break;
				}
			}
		}
	}
}

void LPUART1_RxTx_IRQHandler (void)
{
	if(LPUART1->LPUART_STAT.LPUART_STAT_t.LPUART_STAT_RDRF == BIT_SET)
	{
		receive =(char) LPUART1->LPUART_DATA.REG;
		rxlpuart[index++] = receive;
	}
}




