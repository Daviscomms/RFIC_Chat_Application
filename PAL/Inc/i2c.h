/**
* @file i2c.h
* @brief This header file provides the i2c interfaces.
*
* @author Jimmy Chea
* @date 30/03/2023
*/

#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

int i2c_open(const char *device);

void i2c_close(int fd);

int i2c_write(int fd, uint8_t addr, uint8_t new_reg_value);

uint8_t i2c_read(int fd, uint8_t addr);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __I2C_H__*/