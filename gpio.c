/*
 * AT86RF215 test code with spi driver
 *
 * Copyright (c) 2017  Cisco, Inc.
 * Copyright (c) 2017  <binyao@cisco.com>
 *
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <string.h>
#include <errno.h>
#include <poll.h>
#include "gpio.h"
#include "gpiod.h"



const char* gpio_edge_name[]={"none","both","rising","falling"};

int gpio_set_dir(gpio_t* gpio, unsigned int pin, unsigned int out_flag, int out_value);
int gpio_set_edge(gpio_t* gpio, unsigned int pin, const char *edge);
void gpio_close(gpio_t* gpio);


int gpio_init(gpio_t* gpio)
{
	int ret = 0;

	gpio->gpio_chip = gpiod_chip_open(gpio->name);
	if (!(gpio->gpio_chip))
	{
		perror("Failed to open GPIO chip\n");
		return -1;
	}

	gpio->gpio_line = gpiod_chip_get_line(gpio->gpio_chip, gpio->pin);
	if (!(gpio->gpio_line))
	{
		perror("Failed to open GPIO line\n");
		gpiod_chip_close(gpio->gpio_chip);
		return -1;
	}

	struct gpiod_line_request_config gpio_config = 
	{
		.consumer = NULL,
		.request_type = GPIOD_LINE_REQUEST_EVENT_BOTH_EDGES,
		.flags = 0
	};
	ret = gpiod_line_request(gpio->gpio_line, &gpio_config, 0);
	if (ret == -1)
	{
		perror("Failed to set the configuration on the GPIO line.\n");
	}

	gpio->fd = gpiod_line_event_get_fd(gpio->gpio_line);

/*
	ret = gpio_set_edge(gpio, gpio->pin, gpio_edge_name[gpio->edge]);
	if (ret == -1)
	{
		perror("Failed to set the edge on the GPIO line.\n");
	}

	ret = gpio_set_dir(gpio, gpio->pin, gpio->direction, gpio->value);
	if (ret == -1)
	{
		perror("Failed to set the direction on the GPIO line.\n");
	}
	*/

	return ret;
}

int set_gpio_value(gpio_t* gpio,gpio_value_t io_value)
{
	int ret = 0;
	if(gpio->direction == OUT)
	{
		ret = gpiod_line_set_value(gpio->gpio_line, io_value);
		if (ret == -1)
		{
			perror("Failed to set the value on the GPIO line.\n");
		}
		else
		{
			gpio->value=io_value;
		}
	}
	else
	{
		ret = -1;
	}

	return ret;
}

gpio_value_t get_gpio_value(gpio_t* gpio)
{
	uint8_t value;

	if(gpio->direction == IN)
	{
		value = gpiod_line_get_value(gpio->gpio_line);
	}
	else
	{
		value = gpio->value;
	}

	gpio->value=value;

	return gpio->value;
}

/****************************************************************
 * gpio_set_direction
 0 in
 1 out
 ****************************************************************/
int gpio_set_dir(gpio_t* gpio, unsigned int pin, unsigned int out_flag, int out_value)
{
	int ret = 0;

	if (out_flag)
	{
		ret = gpiod_line_request_output(gpio->gpio_line, NULL, out_value);
		if (ret == -1)
		{
			perror("Failed to set the GPIO line to output direction.\n");
		}
	}
	else
	{
		ret = gpiod_line_request_input(gpio->gpio_line, NULL);
		if (ret == -1)
		{
			perror("Failed to set the GPIO line to input direction.\n");
		}
	}
	return ret;
}

/****************************************************************
 * gpio_set_edge
 ****************************************************************/

int gpio_set_edge(gpio_t* gpio, unsigned int pin, const char *edge)
{
	int ret = 0;

	if (!strcmp(edge, gpio_edge_name[0]))
	{
	}
	else if (!strcmp(edge, gpio_edge_name[1]))
	{
		ret = gpiod_line_request_both_edges_events(gpio->gpio_line, NULL);
		if (ret == -1)
		{
			perror("Failed to set the both edges on the GPIO line.\n");
		}
	}
	else if (!strcmp(edge, gpio_edge_name[2]))
	{
		ret = gpiod_line_request_rising_edge_events(gpio->gpio_line, NULL);
		if (ret == -1)
		{
			perror("Failed to set the rising edges on the GPIO line.\n");
		}
	}
	else if (!strcmp(edge, gpio_edge_name[3]))
	{
		ret = gpiod_line_request_falling_edge_events(gpio->gpio_line, NULL);
		if (ret == -1)
		{
			perror("Failed to set the falling edges on the GPIO line.\n");
		}
	}
	else
	{
		ret = -1;
	}

	return ret;
}

/****************************************************************
 * gpio_close
 ****************************************************************/

void gpio_close(gpio_t* gpio)
{
	close(gpio->fd);
	gpiod_line_release(gpio->gpio_line);
	gpiod_chip_close(gpio->gpio_chip);
}

