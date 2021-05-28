/*
 * i2c.c
 *
 *  Created on: 12 mei 2021
 *      Author: stefa
 */

#include <stdint.h>
#include <ti/drivers/I2C.h>
#include "pee50_i2c.h"

I2C_Handle      i2c_handle;
I2C_Params      i2c_params;
I2C_Transaction i2c_transaction;

int8_t cc3220_i2c_init()
{
    I2C_init();
    I2C_Params_init(&i2c_params);
    i2c_params.bitRate = I2C_400kHz;
}

int8_t cc3220_i2c_open()
{
    i2c_handle = I2C_open(0, &i2c_params);
    if(i2c_handle == NULL) {
        return -1;
    }
    return 0;
}

int8_t cc3220_i2c_close()
{
    I2C_close(i2c_handle);
    return 0;
}

int8_t cc3220_i2c_write_8bit(uint8_t devAddr, uint8_t regAddr, int8_t write_data)
{
    uint8_t write_buffer[] = {regAddr, write_data};

    i2c_transaction.slaveAddress = devAddr;

    i2c_transaction.writeBuf   = write_buffer;
    i2c_transaction.writeCount = 2;
    i2c_transaction.readBuf    = NULL;
    i2c_transaction.readCount  = 0;

    if(!I2C_transfer(i2c_handle, &i2c_transaction))
    {
        return -1;
    }
    return 0;
}

int8_t cc3220_i2c_read_8bit(uint8_t devAddr, uint8_t regAddr, int8_t *read_data)
{
    uint8_t write_buffer[] = {regAddr};
    uint8_t read_buffer[1];

    i2c_transaction.slaveAddress = devAddr;

    i2c_transaction.writeBuf   = write_buffer;
    i2c_transaction.writeCount = 1;
    i2c_transaction.readBuf    = read_buffer;
    i2c_transaction.readCount  = 1;

    if(!I2C_transfer(i2c_handle, &i2c_transaction))
    {
        return -1;
    }
    return 0;
}

int8_t cc3220_i2c_write_16bit(uint8_t devAddr, uint8_t regAddr, int16_t write_data)
{
    uint8_t data_low = ((uint8_t)write_data >> 0) & 0xFF;
    uint8_t data_high = ((uint8_t)write_data >> 8) & 0xFF;

    uint8_t write_buffer[] = {regAddr, data_high, data_low};

    i2c_transaction.slaveAddress = devAddr;

    i2c_transaction.writeBuf   = write_buffer;
    i2c_transaction.writeCount = 3;
    i2c_transaction.readBuf    = NULL;
    i2c_transaction.readCount  = 0;

    if(!I2C_transfer(i2c_handle, &i2c_transaction))
    {
        return -1;
    }
    return 0;
}

int8_t cc3220_i2c_read_16bit(uint8_t devAddr, uint8_t regAddr, int16_t *read_data)
{
    uint8_t write_buffer[] = {regAddr};
    uint8_t read_buffer[2];

    i2c_transaction.slaveAddress = devAddr;

    i2c_transaction.writeBuf   = write_buffer;
    i2c_transaction.writeCount = 1;
    i2c_transaction.readBuf    = read_buffer;
    i2c_transaction.readCount  = 2;

    if(!I2C_transfer(i2c_handle, &i2c_transaction))
    {
        return -1;
    }

    *read_data = (read_buffer[0] << 8) & 0xFFFF;
    *read_data |= read_buffer[1];
    return 0;
}
