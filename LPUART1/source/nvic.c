#include <stdint.h>
#include "nvic.h"
#include "common.h"

void set_enable_nvic_timer(void)
{
	NVIC->ISER[NVIC_IRQn_LPIT0_ch0/32] = SET_BIT(NVIC->ISER[NVIC_IRQn_LPIT0_ch0/32], (NVIC_IRQn_LPIT0_ch0 % 32));
}

void set_enable_nvic_lpuart_transmit(void)
{
	NVIC->ISER[NVIC_IRQn_LUART1_Transmit/32] = SET_BIT(NVIC->ISER[NVIC_IRQn_LUART1_Transmit/32], (NVIC_IRQn_LUART1_Transmit%32));
}

void set_enable_nvic_lpuart_receive(void)
{
	NVIC->ISER[NVIC_IRQn_LUART1_Receive/32] = SET_BIT(NVIC->ISER[NVIC_IRQn_LUART1_Receive/32], (NVIC_IRQn_LUART1_Receive%32));
}

void set_enable_nvic(nvic_interrupt_id_t ID)
{
	NVIC->ISER[ID/32] = SET_BIT(NVIC->ISER[ID/32], (ID%32));
}

