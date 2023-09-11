/**
* @file i2c.c
* @brief This implementation file contains the code for i2c interface.
*
* @author Jimmy Chea
* @date 30/03/2023
*/

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include "i2c.h"


/**
 *  A detailed description of the i2c_open function
 *
 *  This function is to open the i2c device under /dev/i2c-* and return file descriptor
 *
 *  @param \*device - input the device name "/dev/i2c-*"
 *  @return - file descriptor on success, else return -1 on error
 */
int i2c_open(const char *device)
{
    int fd = open(device, O_RDWR);
    if (fd < 0)
    {
        printf("ERROR: open(%d) failed.\n", fd);
    }

    return fd;
}

/**
 *  A detailed description of the i2c_close function
 *
 *  This function is to close the i2c device based on the file descriptor
 *
 *  @param fd - file descriptor
 *  @return - none
 */
void i2c_close(int fd)
{
    close(fd);
}

int i2c_write(int fd, uint8_t addr, uint8_t new_reg_value)
{
    struct i2c_msg msg;
    struct i2c_rdwr_ioctl_data data;
    uint8_t buf;
    int ret;

    buf = new_reg_value;

    msg.addr = (__u16)addr;
    msg.flags = 0;
    msg.len = 1;
    msg.buf = &buf;

    data.msgs = &msg;
    data.nmsgs = 1;

    ret = ioctl(fd, I2C_RDWR, &data);
    if (ret < 0)
    {
        printf("Error: unable to write data. ret = %d\n", ret);
    }

    return ret;
}

uint8_t i2c_read(int fd, uint8_t addr)
{
    struct i2c_msg msg;
    struct i2c_rdwr_ioctl_data data;
    uint8_t reg_value;
    int ret;

    msg.addr = (__u16)addr;
    msg.flags = I2C_M_RD;
    msg.len = 1;
    msg.buf = &reg_value;

    data.msgs = &msg;
    data.nmsgs = 1;

    ret = ioctl(fd, I2C_RDWR, &data);
    if (ret < 0)
    {
        printf("ERROR: unable to read data\n");
        return ret;
    }

    return reg_value;
}