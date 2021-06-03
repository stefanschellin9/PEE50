/*
 * i2c.h
 *
 *  Created on: 12 mei 2021
 *      Author: stefanschellin9
 */

#ifndef I2C_H_PEE50
#define I2C_H_PEE50

#include <stdint.h>
#include <ti/drivers/I2C.h>

/****************************** cc3220_i2c_init ******************************/
/* initialise i2c */
int8_t cc3220_i2c_init();

/****************************** cc3220_i2c_open ******************************/
/* open the i2c for reading and writing */
int8_t cc3220_i2c_open();

/****************************** cc3220_i2c_close *****************************/
/* close the i2c for reading and writing */
int8_t cc3220_i2c_close();

/****************************** cc3220_i2c_write *****************************/
/* write to an 8 bit register of an i2c device */
int8_t cc3220_i2c_write_8bit(uint8_t devAddr, uint8_t regAddr, int8_t write_data);

/****************************** cc3220_i2c_read ******************************/
/* read from an 8 bit register of an i2c device */
int8_t cc3220_i2c_read_8bit(uint8_t devAddr, uint8_t regAddr, int8_t *read_data);

/****************************** cc3220_i2c_write *****************************/
/* write to an 16 bit register of an i2c device */
int8_t cc3220_i2c_write_16bit(uint8_t devAddr, uint8_t regAddr, int16_t write_data);

/****************************** cc3220_i2c_read ******************************/
/* read from an 16 bit register of an i2c device */
int8_t cc3220_i2c_read_16bit(uint8_t devAddr, uint8_t regAddr, int16_t *read_data);

#endif /* I2C_H_PEE50 */
