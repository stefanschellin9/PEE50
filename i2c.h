/*
 * i2c.h
 *
 *  Created on: 12 mei 2021
 *      Author: stefanschellin9
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include <ti/drivers/I2C.h>

enum TMP117_Register
{
  TMP117_TEMP_RESULT = 0X00,
  TMP117_CONFIGURATION = 0x01,
  TMP117_T_HIGH_LIMIT = 0X02,
  TMP117_T_LOW_LIMIT = 0X03,
  TMP117_EEPROM_UL = 0X04,
  TMP117_EEPROM1 = 0X05,
  TMP117_EEPROM2 = 0X06,
  TMP117_TEMP_OFFSET = 0X07,
  TMP117_EEPROM3 = 0X08,
  TMP117_DEVICE_ID = 0X0F
};

int8_t cc3220_i2c_init(I2C_Params *i2cParams);
int8_t cc3220_i2c_open(I2C_Handle *i2c, I2C_Params i2cParams);
int8_t cc3220_i2c_close(I2C_Handle i2c);
int8_t cc3220_i2c_write(I2C_Handle i2c_handle,
                               I2C_Transaction i2c_transaction,
                               uint8_t devAddr, uint8_t regAddr, uint8_t data);
int8_t cc3220_i2c_read(I2C_Handle i2c_handle,
                              I2C_Transaction i2c_transaction,
                              uint8_t devAddr, uint8_t regAddr, uint8_t *buffer);
int8_t cc3220_LSM6DS3_step_counter_init();
int8_t cc3220_LSM6DS3_step_counter_read();

#endif /* I2C_H_ */
