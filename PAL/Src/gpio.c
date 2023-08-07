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
#include <linux/gpio.h>
#include <string.h>
#include <errno.h>
#include <poll.h>
#include "gpio.h"
#include "gpiod.h"

const char* gpio_edge_name[]={"none","both","rising","falling"};

int gpio_set_edge_event(gpio_t* gpio, const char *edge);
int request_gpio_direction(gpio_t* gpio);
void gpio_close(gpio_t* gpio);

int gpio_init(gpio_t* gpio)
{
	int fd, ret = 0;

	fd = open(gpio->name, O_RDWR);
	if (fd == -1)
	{
		printf("Unable to open %s: %s\n", gpio->name, strerror(errno));
		ret = -1;
		goto exit;
	}
	gpio->fd = fd;

	ret = request_gpio_direction(gpio);
	if (ret == -1)
	{
		goto exit;
	}

	if (gpio->direction == OUT)
	{
		ret = set_gpio_value(gpio, gpio->value);
		if (ret == -1)
		{
			goto exit;
		}
	}
	else if (gpio->direction == IN)
	{
		ret = gpio_set_edge_event(gpio, gpio_edge_name[gpio->edge]);
		if (ret == -1)
		{
			goto exit;
		}
	}

exit:

	return ret;
}

int request_gpio_direction(gpio_t* gpio)
{
	int ret = 0;

	if (gpio->direction == OUT)
	{
		struct gpiohandle_request rq = 
		{
			.lineoffsets[0] = gpio->pin,
			.flags = GPIOHANDLE_REQUEST_OUTPUT,
			.lines = 1,
		};

		ret = ioctl(gpio->fd, GPIO_GET_LINEHANDLE_IOCTL, &rq);
		if (ret == -1)
		{
			perror("Unable to get line handle from ioctl.\n");
			return -1;
		}

		gpio->req_fd = rq.fd;
	}

	return ret;
}

int set_gpio_value(gpio_t* gpio, gpio_value_t io_value)
{
	int ret = 0;

	if(gpio->direction == OUT)
	{
		struct gpiohandle_data data = 
		{
			.values[0] = io_value,
		};

		ret = ioctl(gpio->req_fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
		if (ret == -1)
		{
			perror("Unable to set the line value using ioctl.\n");
			return -1;
		}
		
		gpio->value = io_value;
	}
	else
	{
		ret = -1;
	}

	return ret;
}

gpio_value_t get_gpio_value(gpio_t* gpio)
{
	int ret = 0;
	uint8_t value;

	if(gpio->direction == IN)
	{
		struct gpiohandle_data data;
		ret = ioctl(gpio->req_fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);
		if (ret == -1)
		{
			perror("Unable to get line using ioctl.\n");
		}
		
		value = data.values[0];
	}
	else
	{
		value = gpio->value;
	}

	gpio->value=value;

	return gpio->value;
}

/****************************************************************
 * gpio_set_edge_event
 ****************************************************************/

int gpio_set_edge_event(gpio_t* gpio, const char *edge)
{
	int ret = 0;
	struct gpioevent_request req;

	if (!strcmp(edge, gpio_edge_name[0]))
	{
		req.lineoffset = gpio->pin;
		req.handleflags = GPIOHANDLE_REQUEST_INPUT;
		req.eventflags = 0;

		ret = ioctl(gpio->fd, GPIO_GET_LINEEVENT_IOCTL, &req);
		if (ret == -1)
		{
			perror("Failed to set the NONE edges on the GPIO line.\n");
		}

		gpio->req_fd = req.fd;
	}
	else if (!strcmp(edge, gpio_edge_name[1]))
	{
		req.lineoffset = gpio->pin;
		req.handleflags = GPIOHANDLE_REQUEST_INPUT;
		req.eventflags = GPIOEVENT_REQUEST_BOTH_EDGES;

		ret = ioctl(gpio->fd, GPIO_GET_LINEEVENT_IOCTL, &req);
		if (ret == -1)
		{
			perror("Failed to set the both edges on the GPIO line.\n");
		}

		gpio->req_fd = req.fd;
	}
	else if (!strcmp(edge, gpio_edge_name[2]))
	{
		req.lineoffset = gpio->pin;
		req.handleflags = GPIOHANDLE_REQUEST_INPUT;
		req.eventflags = GPIOEVENT_REQUEST_RISING_EDGE;

		ret = ioctl(gpio->fd, GPIO_GET_LINEEVENT_IOCTL, &req);
		if (ret == -1)
		{
			perror("Failed to set the rising edges on the GPIO line.\n");
		}

		gpio->req_fd = req.fd;
	}
	else if (!strcmp(edge, gpio_edge_name[3]))
	{
		req.lineoffset = gpio->pin;
		req.handleflags = GPIOHANDLE_REQUEST_INPUT;
		req.eventflags = GPIOEVENT_REQUEST_FALLING_EDGE;

		ret = ioctl(gpio->fd, GPIO_GET_LINEEVENT_IOCTL, &req);
		if (ret == -1)
		{
			perror("Failed to set the falling edges on the GPIO line.\n");
		}

		gpio->req_fd = req.fd;
	}
	else
	{
		ret = -1;
	}

	return ret;
}

int gpio_enable_interrupt(gpio_t* gpio)
{
	char * str = "RISING";
	gpio_set_edge_event(gpio, (const char*)str);
	
	return 0;
}

int gpio_disable_interrupt(gpio_t* gpio)
{
	char * str = "NONE";
	gpio_set_edge_event(gpio, (const char*)str);
	
	return 0;
}

/****************************************************************
 * gpio_close
 ****************************************************************/

void gpio_close(gpio_t* gpio)
{
	close(gpio->fd);
}

