#include <stdint.h>
#include "lpuart.h"
#include "pcc.h"
#include "clock.h"
#include "common.h"
#include "gpio.h"
#include "nvic.h"
#include "port.h"
#include "lpit.h"
#include "lpspi.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

/* time: 12h-52m-36s */
/* alamr: 13h-30m */
/* day: 15.12.2023 */
static uint16_t hour = 12, minute = 52, second = 36, alarm_hour = 13, alarm_minute = 30;
static uint16_t day = 15, month = 12, year = 2023;
static uint8_t hour1, hour2, minute1, minute2, second1, second2, alarm_hour1, alarm_hour2, alarm_minute1, alarm_minute2;
static uint8_t day1, day2, month1, month2, year1, year2, year3, year4;


void display_(void);
uint16_t isleapyear(uint16_t year_in);
void process_alarm(void);

int main(void)
{
	set_spll_sysclk();
	//set_enable_nvic_timer();
/*------------------------------ set PORT D --------------------------------*/	
	/* enable clock port D clock gate */
	PCC->PCC_PORTD.PCC_PORT.CGC = BIT_SET;
	/* configure the alternative function for PTD16 as GPIO */
	/* disable pull up/down is default value */
	PORT_D->PORT_PCR16.PORT_PIN_t.MUX = MUX_GPIO;
	/* set the PTD 16 as the output pin */
	GPIO_D->PDDR.FIELD.P16 = BIT_SET;
	/* start led green is off */
	GPIO_D->PTOR.FIELD.P16 = BIT_SET;
/*---------------------------------------------------------------------------*/

/*----------------------------- set PORT C ----------------------------------*/	
	/* enable clock port C clock gate */
	PCC->PCC_PORTC.PCC_PORT.CGC = BIT_SET;
	/* configure the alternative function for PTC 12 and PTC 13 as GPIO */
	PORT_C->PORT_PCR12.PORT_PIN_t.MUX = MUX_GPIO;
	PORT_C->PORT_PCR13.PORT_PIN_t.MUX = MUX_GPIO;
	/* set PTC 12 and PTC 13 as the input pin */
	GPIO_C->PDDR.FIELD.P12 = BIT_CLEAR;
	GPIO_C->PDDR.FIELD.P13 = BIT_CLEAR;
	/* set interrupt port C, rising edge */
	set_enable_nvic(NVIC_IRQn_PORTC);
	PORT_C->PORT_PCR12.PORT_PIN_t.IRQC = RISING_EDGE;
	PORT_C->PORT_PCR13.PORT_PIN_t.IRQC = RISING_EDGE;
/*----------------------------------------------------------------------------*/	
	
/*----------- the functions to communicaion to the devices -------------------*/
	/* initialize uart */
	init_lpuart_interrupt();
	/* initialize spi */
	init_lpspi();
	/* initialize lpit */
	timer_32_bit();
	
	/* configuration spi to transfer data to the devices  */
	/* shutdown mode is normal operation */
	transmit_data(shutdown_reg, normal_operation);
	/* display number leds */
	transmit_data(scan_limit, scan_limit_digit);
	/* select decode mode */
	transmit_data(decode_mode, decode_all);
	/* select mode intensity display */
	transmit_data(intensity, intensity_format);
	/* display test*/
	transmit_data(display_mode, display_test);
/*----------------------------------------------------------------------------*/	

	while(1)
	{
		
		process_time();
		display_();
		process_alarm();
		proccess_rxlpuart();
	}
	
}

void proccess_rxlpuart(void)
{
	if(LPUART1->LPUART_STAT.LPUART_STAT_t.LPUART_STAT_IDLE == BIT_SET)
	{
		LPUART1->LPUART_STAT.LPUART_STAT_t.LPUART_STAT_IDLE = BIT_SET;
		char sign1, sign2;
		uint32_t receive1 = 0;
		uint32_t receive2 = 0;
		uint32_t receive3 = 0;
		if(rxlpuart[1] == '-' || rxlpuart[2] == '-')
		{
			sscanf(rxlpuart, "%d%c%d%c%d", &receive1, &sign1, &receive2, &sign2, &receive3);
			hour 	 = (uint16_t) receive1;
			minute = (uint16_t) receive2;
			second = (uint16_t) receive3;
		}
		else if(rxlpuart[1] == '.' || rxlpuart[2] == '.')
		{
			sscanf(rxlpuart, "%d%c%d%c%d", &receive1, &sign1, &receive2, &sign2, &receive3);
			day = (uint16_t) receive1;
			month = (uint16_t) receive2;
			year = (uint16_t) receive3;
		}
		else if(rxlpuart[1] == ':' || rxlpuart[2] == ':')
		{
			sscanf(rxlpuart, "%d%c%d%c%d", &receive1, &sign1, &receive2, &sign2, &receive3);
			alarm_hour = (uint16_t) receive1;
			alarm_minute = (uint16_t)receive2;
			alarmonof = (uint16_t)receive3;
		}
		else
		{
			index = 0;
			memset(rxlpuart, 0, sizeof(rxlpuart));
		}
	}
}

uint16_t isleapyear(uint16_t year_in)
{
	return((year_in% 4 == 0 && year_in % 100 != 0) || (year_in % 400 == 0))? 28 : 29;
}

void process_time(void)
{
	second++;
	if(second > 59)
	{
		minute++;
		second = 0;
	}
	if(minute > 59)
	{
		hour++;
		minute = 0;
	}
	if(hour > 23)
	{
		day++;
		hour = 0;
		minute = 0;
		second = 0;
	}
	
	if(month == 2)
	{
		dayinmonth = (uint16_t)isleapyear(year);
	}
	else if(month == 1 || month == 1 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
	{
		dayinmonth = 31;
	}
	else
	{
		dayinmonth = 30;
	}
	
	if(day > dayinmonth)
	{
		month ++;
		day = 0;
	}
	if(month > 12)
	{
		year++;
		month = 1;
		day = 1;
	}
}

void display_(void)
{
	if(mode_display == time_display)
	{
		second1 = second % 10;
		second2 = (uint8_t)second / 10;
		minute1 = minute % 10;
		minute2 = (uint8_t)minute / 10;
		hour1 = hour % 10;
		hour2 = (uint8_t) hour/ 10;
		transmit_data(digit_0,second1);
		transmit_data(digit_1, second2);
		transmit_data(digit_2, 0x1);
		transmit_data(digit_3, minute1);
		transmit_data(digit_4, minute2);
		transmit_data(digit_5, 0x1);
		transmit_data(digit_6, hour1);
		transmit_data(digit_7, hour2);
	}
	else if(mode_display == date_display)
	{
		day1 = day % 10;
		day2 =(uint8_t) day / 10;
		month1 = month % 10;
		month2 = (uint8_t)month / 10;
		year1 = year % 10;
		year2 = (year / 100) % 10;
		year3 = (year % 1000) / 100;
		year4 = year / 1000;
		transmit_data(digit_0, day1);
		transmit_data(digit_1, (day2 | (1 << 7u)));
		transmit_data(digit_2, month1);
		transmit_data(digit_3, (month2 | (1 << 7u)));
		transmit_data(digit_4, year1);
		transmit_data(digit_5, year2);
		transmit_data(digit_6, year3);
		transmit_data(digit_7, year4);
	}
	else if(mode_display == alarm_display)
	{
		alarm_minute1 = alarm_minute % 10;
		alarm_minute2 =(uint8_t) alarm_minute / 10;
		alarm_hour1 = alarm_hour % 10;
		alarm_hour2 =(uint8_t) alarm_hour / 10;
		transmit_data(digit_0, 0x0);
		transmit_data(digit_1, 0x0);
		transmit_data(digit_2, 0x1);
		transmit_data(digit_3, alarm_minute1);
		transmit_data(digit_4, alarm_minute2);
		transmit_data(digit_5, 0x1);
		transmit_data(digit_6, alarm_hour1);
		transmit_data(digit_7, alarm_hour2);
	}
	else
	{
		transmit_data(digit_0, 0);
		transmit_data(digit_1, 0);
		transmit_data(digit_2, 0);
		transmit_data(digit_3, 0);
		transmit_data(digit_4, 0);
		transmit_data(digit_5, 0);
		transmit_data(digit_6, 0);
		transmit_data(digit_7, 0);
	}
}

void process_alarm(void)
{
	if(alarmonof == 1)
	{
		if(alarm_hour == hour && alarm_minute == minute)
		{
			GPIO_D->PTOR.FIELD.P16 = BIT_SET;
		}
	}
}


