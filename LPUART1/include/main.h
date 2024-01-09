#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdint.h>

//handle the optimize -O0 checking for main argument
__asm(".global __ARM_use_no_argv");


#define time_display 	2
#define date_display  3
#define alarm_display 4
volatile static uint32_t mode_display;
volatile static uint16_t dayinmonth;
volatile static uint16_t alarmonof;

void process(void);
void display_(void);
void proccess_rxlpuart(void);


#endif /* !__MAIN_H__ */
