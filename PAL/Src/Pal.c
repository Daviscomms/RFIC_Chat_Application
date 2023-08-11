#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include "pal.h"
#include <time.h>
#include <sys/time.h>
#include <string.h> 
#include <signal.h>
#include "timer.h"

/* === Externals ============================================================ */
extern At86rf215_Dev_t at86rf215_dev;
struct timeval start;


retval_t pal_init(void){
	if(-1==spi_init(at86rf215_dev.spi)){
		return FAILURE;
	}
	if(-1==gpio_init(at86rf215_dev.gpio_irq)){
		return FAILURE;
	}
	if(-1==gpio_init(at86rf215_dev.gpio_rest)){
		return FAILURE;
	}
	gettimeofday(&start,NULL);

	return MAC_SUCCESS;
}

void pal_timer_delay(uint16_t delay){//us
	struct timeval tv;
	tv.tv_sec=delay/1000000;
	tv.tv_usec=delay-tv.tv_sec*1000000;	
	select(0,NULL,NULL,NULL,&tv);

}

void TRX_RST_HIGH(){
	set_gpio_value(at86rf215_dev.gpio_rest,high);
}

void TRX_RST_LOW(){
	set_gpio_value(at86rf215_dev.gpio_rest,low);
}

gpio_value_t TRX_IRQ_GET(){
	return get_gpio_value(at86rf215_dev.gpio_irq);
}


void pal_get_current_time(uint32_t *current_time){
	struct  timeval  cur;
	gettimeofday(&cur,NULL);
	*current_time=(uint32_t)(1000000*(cur.tv_sec-start.tv_sec))+(cur.tv_usec-start.tv_usec);//us

}


retval_t pal_timer_start(timer_handle_t** timer_handler,
						 timer_instance_id_t timer_instance_id,
						 uint32_t timer_count,
						 timer_type_t timer_type,
						 FUNC_PTR(timer_cb),
						 void *param_cb){

	*timer_handler = fn_timer_create(timer_type, (FUNC_PTR())timer_cb, timer_instance_id);
	if (timer_handler == NULL)
	{
		perror("Failed to create timer.\n");
		return FAILURE;
	}

	fn_timer_start((*timer_handler), (timer_tick_ms_t)timer_count);

	return MAC_SUCCESS;
}

retval_t pal_timer_stop(timer_handle_t** timer_handler,
							timer_instance_id_t timer_instance_id)
{
	fn_timer_stop((*timer_handler));
	fn_timer_delete((*timer_handler));
	
	return MAC_SUCCESS;
}


void pal_enable_interrupt()
{
	gpio_enable_interrupt(at86rf215_dev.gpio_irq);
}

void pal_disable_interrupt()
{
	gpio_disable_interrupt(at86rf215_dev.gpio_irq);
}




