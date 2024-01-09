#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include <stdint.h>
#include "common.h"

typedef union
{
	struct
	{
		RW uint32_t ENABLE  	: 1;
		RW uint32_t TICKINT 	: 1;
		RW uint32_t CLKSOURCE : 1;
		RW uint32_t RES1 			: 13;
		RW uint32_t COUNTFLAG : 1;
		RW uint32_t	RES2			: 15;
	}FIELD_CSR;

	struct
	{
		RW uint32_t RELOAD 		: 24;
		uint32_t RES1			 		: 8;	
	}SYST_RVR;

	struct
	{
		RW uint32_t CURRENT 	: 32;
	}FIELD_CVR;

	struct
	{
		RO uint32_t TENMS 		: 24;
		RO uint32_t RES   		: 6;
		RO uint32_t SKEW			: 1;
		RO uint32_t NOREF 		: 1;
	}FIELD_CALIB;
}SYST_LAYOUT_t;

typedef struct
{
	IO SYST_LAYOUT_t SYST_CSR;
	IO SYST_LAYOUT_t SYST_RVR;
	IO SYST_LAYOUT_t SYST_CVR;
	IO SYST_LAYOUT_t SYST_CALIB;
}SYSR_t;

/* define systick base address */
#define SYST_BASE							(0xE000E010)

/* define systick base */
#define SYST									((SYSR_t *) SYST_BASE)

#define CORE_CLOCK 		 72000000
#define RELOAD_TIME 	 0.001
#define RELOAD_VALUE   ((CORE_CLOCK * RELOAD_TIME) - 1)

static unsigned int delay_counter;

void delay_ms(void);
void delay(uint32_t  time_delay);

#endif /* !__SYSTICK_H__ */
