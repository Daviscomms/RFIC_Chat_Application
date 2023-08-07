#include <fcntl.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <string.h>
#include <stdio.h>
#include "io_utils.h"
#include <linux/gpio.h>

int main(void)
{
    struct gpioevent_data evdata;

    gpio_t at86rf215_gpio_irq={
        .name="/dev/gpiochip5",
        .pin=8,
        .fd=-1,
        .req_fd=-1,
        .direction=IN,
        .edge=RISING,
        .value=low
    };

    printf("gpio init\n");
    gpio_init(&at86rf215_gpio_irq);

    int nfds = 1;
	struct pollfd fdset;
	int ret;

	while (1) {
		memset(&fdset, 0, sizeof(fdset));
      
		fdset.fd = at86rf215_gpio_irq.req_fd;
		fdset.events = POLLPRI;

		ret = poll(&fdset, nfds, -1);	  
		
		if (ret < 0) {
			perror("\npoll() failed!\n");
			break;
		}
		if (fdset.revents & POLLPRI) {
            printf("Interrupt event trigger.\n");
		}
	}

    return 0;
}
