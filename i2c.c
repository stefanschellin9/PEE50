/*
 * i2c.c
 *
 *  Created on: 12 mei 2021
 *      Author: stefa
 */

#include <stdint.h>
#include <ti/drivers/I2C.h>

int8_t cc3220_i2c_init(I2C_Params i2cParams)
{
    I2C_init();
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    return 0;
}

I2C_Handle cc3220_i2c_open(I2C_Params i2cParams)
{
    I2C_Handle i2c = I2C_open(0, &i2cParams);
    if (i2c == NULL) {
        return -1;
    }
    return 0;
}

int8_t cc3220_i2c_close(I2C_Handle i2c)
{
    I2C_close(i2c);
    return 0;
}

int8_t cc3220_i2c_write(I2C_Handle i2c_handle,
                               I2C_Transaction i2c_transaction,
                               uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
    uint8_t write_buffer[] = {regAddr, data};

    i2c_transaction.slaveAddress = devAddr;

    i2c_transaction.writeBuf   = write_buffer;
    i2c_transaction.writeCount = sizeof(write_buffer);
    i2c_transaction.readBuf    = NULL;
    i2c_transaction.readCount  = 0;

    if(!I2C_transfer(i2c_handle, &i2c_transaction))
    {
        return -1;
    }
    return 0;
}

int8_t cc3220_i2c_read(I2C_Handle i2c_handle,
                              I2C_Transaction i2c_transaction,
                              uint8_t devAddr, uint8_t regAddr, uint8_t *read_buffer)
{
    uint8_t write_buffer[] = {regAddr};

    i2c_transaction.slaveAddress = devAddr;

    i2c_transaction.writeBuf   = write_buffer;
    i2c_transaction.writeCount = sizeof(regAddr);
    i2c_transaction.readBuf    = read_buffer;
    i2c_transaction.readCount  = sizeof(read_buffer);

    if(!I2C_transfer(i2c_handle, &i2c_transaction))
    {
        return -1;
    }
    return 0;
}
