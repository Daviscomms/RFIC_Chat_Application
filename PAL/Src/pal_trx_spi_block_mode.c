
#include <stdbool.h>
#include <stdint.h>
#include "pal.h"
#include "pal_types.h"

#if (defined PAL_SPI_BLOCK_MODE) || (defined DOXYGEN)

/* === Externals ============================================================ */
extern At86rf215_Dev_t at86rf215_dev;

/* === Macros =============================================================== */


void pal_trx_write(uint16_t addr, uint8_t *data, uint16_t length)
{
	ENTER_TRX_REGION();
	spi_data_t message={
		.address=addr,
		.data=data,
		.len=length
	};
	spi_write(at86rf215_dev.spi,&message);
	LEAVE_TRX_REGION();
}


void pal_trx_read(uint16_t addr, uint8_t *data, uint16_t length)
{
	ENTER_TRX_REGION();
	spi_data_t message={
		.address=addr,
		.data=data,
		.len=length
	};
	spi_read(at86rf215_dev.spi,&message);
	LEAVE_TRX_REGION();
}


void pal_trx_reg_write(uint16_t addr, uint8_t data)
{
	ENTER_TRX_REGION();
	spi_reg_write(at86rf215_dev.spi,addr,data);
	LEAVE_TRX_REGION();
}


uint8_t pal_trx_reg_read(uint16_t addr)
{
	ENTER_TRX_REGION();
	uint8_t ret= spi_reg_read(at86rf215_dev.spi,addr);
	LEAVE_TRX_REGION();

	return ret;
}

uint8_t pal_trx_bit_read(uint16_t addr, uint8_t mask, uint8_t pos)
{
	ENTER_TRX_REGION();
	uint8_t ret=spi_reg_bit_read(at86rf215_dev.spi,addr,mask,pos);
	LEAVE_TRX_REGION();
	
	return ret;
} 

 
void pal_trx_bit_write(uint16_t addr, uint8_t mask, uint8_t pos, uint8_t new_value) 
{
	ENTER_TRX_REGION();
 	spi_reg_bit_write(at86rf215_dev.spi,addr,mask,pos,new_value);
	LEAVE_TRX_REGION();
}



#endif  /* #if (defined PAL_SPI_BLOCK_MODE) || (defined DOXYGEN) */


/* EOF */
