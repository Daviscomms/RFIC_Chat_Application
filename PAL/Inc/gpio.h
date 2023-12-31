#ifndef GPIO_H
#define GPIO_H

#include "gpiod.h"


typedef enum gpio_edge_tag{
	NONE,
	BOTH,
	RISING,
	FALLING
}gpio_edge_t;

typedef enum gpio_direction_tag{
	IN,
	OUT
}gpio_direction_t;

typedef enum gpio_value_tag{
	low,
	high
}gpio_value_t;

typedef struct gpio_tag{
	char *name;
	int pin;
	int fd;
	int req_fd;
	gpio_direction_t direction;
	gpio_edge_t edge;
	gpio_value_t value;
}gpio_t;


int gpio_init(gpio_t* gpio);
int set_gpio_value(gpio_t* gpio,gpio_value_t io_value);
gpio_value_t  get_gpio_value(gpio_t* gpio);
int gpio_enable_interrupt(gpio_t* gpio);
int gpio_disable_interrupt(gpio_t* gpio);


#endif
