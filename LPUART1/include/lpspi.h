#ifndef __LPSPI_H__
#define __LPSPI_H__

#include <stdint.h>
#include "common.h"

/* control register */
typedef union
{
	uint32_t REG;
	struct
	{
		RW uint32_t	MEN		: 1;  /* module enable */
		RW uint32_t RST		: 1; /* software reset */
		RW uint32_t DOZEN	: 1; /* doze mode enable */
		RW uint32_t DBGEN	: 1; /* debug enable */
		uint32_t		RES0	: 4;
		WO uint32_t RTF		: 1; /* reset transmit FIFO */
		WO uint32_t RRF		: 1; /* reset receive FIFO */
		uint32_t		RES1	: 22;
	}CR_t;
}LPSPI_CR_t;

/* status register */
typedef union
{
	uint32_t REG;
	struct
	{
		RO 	uint32_t	TDF		: 1; /* transmit data flag */
		RO 	uint32_t 	RDF		: 1; /* receive data flag */
		uint32_t 			RES0	: 6;
		W1C uint32_t 	WCF		: 1; /* word complete flag */
		W1C	uint32_t 	FCF		: 1; /* frame complete flag */
		W1C uint32_t	TCF		: 1; /* transfer complete flag */
		W1C uint32_t	TEF		: 1; /* transmit error flag */
		W1C uint32_t	REF		: 1; /* receive error flag */
		W1C uint32_t	DMF		: 1; /* data match flag */
		uint32_t			RES1	: 10;
		RO	uint32_t	MBF		: 1; /* module busy flag */
		uint32_t			RES2	: 7;
	}SR_t;
}LPSPI_SR_t;

/* interrupt enbale register */
typedef union
{
	uint32_t REG;
	struct
	{
		RW uint32_t MASTER	: 1; /* master mode */
		RW uint32_t SAMPLE	: 1; /* sample point */
		RW uint32_t AUTOPCS	: 1; /* automatic PCS */
		RW uint32_t NOSTALL	: 1; /* no stall */
		uint32_t		RES0		: 4;
		RW uint32_t	PCSPOL	: 4; /* peripheral chip select polarity */
		uint32_t		RES1		: 4;
		RW uint32_t	MATCFG	: 3; /* match congfiguration */
		uint32_t		RES2		: 5;
		RW uint32_t	PINCFG	: 2; /* pin configuration */
		RW uint32_t	OUTCFG	: 1; /* output configuration */
		RW uint32_t	PCSCFG	: 1; /* peripheral chip selec configuration */
		uint32_t		RES3		: 4;
	}CFGR1_t;
}LPSPI_CFGR1_t;

/* clock configuration register */
typedef union
{
	uint32_t REG;
	struct
	{
		RW uint32_t	SCKDIV	: 8; /* SCK devider */
		RW uint32_t	DBT			: 8; /* delay between transfer */
		RW uint32_t	PSCSCK	: 8; /* PCS-to-SCK delay */
		RW uint32_t SCKPCS	: 8; /* SCK-to-PCS delay */
	}CCR_t;
}LPSPI_CCR_t;

/* FIFO control register */
typedef union
{
	uint32_t REG;
	struct
	{
		RW uint32_t	TXWATER	: 2; /* transmit FIFO watermark */
		uint32_t		RES0		: 14;
		RW uint32_t	RXWATER	: 2; /* receive FIFO watermart */
		uint32_t		RES1		: 14;
	}FCR_t;
}LPSPI_FCR_t;

/* transmit command register */
typedef union
{
	uint32_t REG;
	struct
	{
		RW uint32_t FRAMESZ		: 12; /* frame size */
		uint32_t		RES0			: 4;
		RW uint32_t WIDTH			: 2; /* transfer width */
		RW uint32_t TXMSK			: 1; /* transmit data mask */
		RW uint32_t	RXMSK			: 1; /* receive data mask */
		RW uint32_t	CONTC			: 1; /* continuing command */
		RW uint32_t	CONT			: 1; /* continuous transfer */
		RW uint32_t BYSW			: 1; /* byte swap	*/
		RW uint32_t LSBF			: 1; /* LSB first */
		RW uint32_t PCS				: 2; /* peripheral chip select */
		uint32_t		RES1			: 1;
		RW uint32_t	PRESCALE	: 3; /* prescaler value */
		RW uint32_t	CPHA			: 1; /* clock phase */
		RW uint32_t	CPOL			: 1; /* clock polarity */
		
	}TCR_t;
}LPSPI_TCR_t;

typedef union
{
	uint32_t REG_TDR;
}LPSPI_TDR_t;

/* type LPSPI  */
typedef struct
{
	IO uint32_t				VERID;
	IO uint32_t				PARAM;
	uint32_t					RES0[2];
	IO LPSPI_CR_t			CR;
	IO LPSPI_SR_t			SR;
	IO uint32_t				IER;
	IO uint32_t				DER;
	IO uint32_t				CFGR0;
	IO LPSPI_CFGR1_t	CFGR1;
	uint32_t					RES1[2];
	IO uint32_t				DMR0;
	IO uint32_t				DMR1;
	uint32_t					RES2[2];
	IO LPSPI_CCR_t		CCR;
	uint32_t					RES3[5];
	IO LPSPI_FCR_t		FCR;
	IO uint32_t				FSR;
	IO LPSPI_TCR_t		TCR;
	IO LPSPI_TDR_t		TDR;
	IO uint32_t				RSR;
	IO uint32_t				RDR;
}LPSPI_Type_t;

/* define LPSPIn base address */
#define	LPSPI_0_base		(0x4002C000)
#define LPSPI_1_base		(0x4002D000)
#define	LPSPI_2_base		(0x4002E000)

/* define LPSPIn  */
#define LPSPI0					((LPSPI_Type_t *) LPSPI_0_base)
#define LPSPI1					((LPSPI_Type_t *) LPSPI_1_base)
#define LPSPI2					((LPSPI_Type_t *)	LPSPI_2_base)

typedef enum
{
	SIN_0 	= 0u, /*  input data and SOUT is used for output data */
	SIN_1 	= 1u, /* input and output data */
	SOUT_0	= 3u, /*  input and output data */
	SOUT_1	= 4u	/* input data and SIN is used for output data */
	
}PINCFG_value_t;

typedef enum
{
	DISABLE	= 0u, /* Match is disabled */
	RES0		= 1u, /* Reserved */
	MATCH1	= 2u, /* 1st data word = MATCH0 + MATCH1 */ 
	MATCH2	= 3u, /* any data word = MATCH0 + MATCH1 */
	MATCH3	= 4u, /* (1st data word = MATCH0) * (2nd data word = MATCH1) */
	MATCH4	= 5u, /* (any data word = MATCH0) * (next data word = MATCH1) */
	MATCH5	= 6u, /* (1st data word * MATCH1) = (MATCH0 * MATCH1) */
	MATCH6	= 7u, /* (any data word * MATCH1) = (MATCH0 * MATCH1) */
}MATCFG_value_t;

typedef enum
{
	DIVIDE_BY_1		= 0u,
	DEVIDE_BY_2		= 1u,
	DEVIDE_BY_4		= 2u,
	DEVIDE_BY_8		= 3u,
	DEVIDE_BY_16	= 4u,
	DEVIDE_BY_32	= 5u,
	DEVIDE_BY_64	= 6u,
	DEVIDE_BY_128	= 7u
}PRESCALE_value_t;

typedef enum
{
	Transfer_LPSPI_PCS0	= 0u,
	Transfer_LPSPI_PCS1	= 1u,
	Transfer_LPSPI_PCS2	= 2u,
	Transfer_LPSPI_PCS3	= 3u	
}PCS_value_t;

typedef enum
{
	TRANSFER_1_BIT	= 0u,
	TRANSFER_2_BIT	= 1u,
	TRANSFER_4_BIT	= 3u,
	_RES0						= 4u
}WIDTH_BIT_t;

#define decode_mode				0x09
#define intensity					0x0A
#define scan_limit				0x0B
#define	display_mode			0x0F
#define shutdown_reg			0x0C
#define digit_0						0x1
#define digit_1						0x2
#define digit_2						0x3
#define digit_3						0x4
#define digit_4						0x5
#define digit_5						0x6
#define digit_6						0x7
#define digit_7						0x8

#define intensity_format	0x4u
#define scan_limit_digit	0x07u
#define display_test			0x00u
#define decode_all				0xFF
#define normal_operation	0x1u
#define	data_1						0x3u

#define intensity_1				0x1
#define intensity_2				0x3
#define intensity_3				0x5
#define intensity_4				0x7
#define intensity_5				0x8

			
/* initialize the spi */
void init_lpspi(void);
/* transfer data  */
void transmit_data(uint8_t addr, uint8_t data);
/* configuration spi to transfer data to the devices  */
void configure_lpspi(void);
void process_time(void);


#endif 
