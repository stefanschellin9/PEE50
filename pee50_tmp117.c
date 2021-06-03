/*
 * pee50_tmp117.c
 *
 *  Created on: 26 mei 2021
 *      Author: stefanschellin9
 */

#include <stdio.h>
#include <stdint.h>

#include "pee50_i2c.h"
#include "pee50_tmp117.h"

void binary_printf(int16_t data)
{
    unsigned int mask = 1<<((sizeof(int)<<2)-1);

    while(mask) {
        if(mask & 0x8888) {
            printf(" ");
        }
        printf("%d", (data & mask ? 1 : 0));
        mask >>= 1;
    }
    printf("\n");
}

void binary_bit_write(int16_t *data, uint16_t bit_num, uint8_t state)
{
    if(state) {
        *data |= 1 << bit_num;
    } else {
        *data &= ~(1 << bit_num);
    }
}

int8_t tmp117_begin()
{
    int16_t data;

    cc3220_i2c_init();
    cc3220_i2c_open();
    cc3220_i2c_read_16bit((uint8_t)tmp117_addr,
                          (uint8_t)TMP117_DEVICE_ID, &data);

    cc3220_i2c_close();
    if(data != (uint16_t)DEVICE_ID_VALUE ) {
        return -1;
    }
    return 0;
}

void tmp117_read_temp_c(void *celsius)
{
    int16_t data;
    float finalTempC;

    cc3220_i2c_init();
    cc3220_i2c_open();
    cc3220_i2c_read_16bit((uint8_t)tmp117_addr,
                          (uint8_t)TMP117_TEMP_RESULT, &data);
    cc3220_i2c_close();

    *(float *)celsius = (data*TMP117_RESOLUTION);
}

int8_t tmp117_conv_mode_set(int8_t conv_bit)
{
    int16_t data;

    cc3220_i2c_init();
    cc3220_i2c_open();
    cc3220_i2c_read_16bit((uint8_t)tmp117_addr,
                          (uint8_t)TMP117_CONFIGURATION, &data);

    //binary_printf(data);
    binary_bit_write(&data, 10, conv_bit & 1);
    binary_bit_write(&data, 11, conv_bit & 2);
    //binary_printf(data);

    cc3220_i2c_write_16bit((uint8_t)tmp117_addr,
                           (uint8_t)TMP117_CONFIGURATION, data);

    cc3220_i2c_close();
    return 0;
}

int8_t tmp117_conv_cycle_set(int8_t cycle_bit)
{
    int16_t data;

    cc3220_i2c_init();
    cc3220_i2c_open();
    cc3220_i2c_read_16bit((uint8_t)tmp117_addr,
                          (uint8_t)TMP117_CONFIGURATION, &data);

    //binary_printf(data);
    binary_bit_write(&data, 7, cycle_bit & 1);
    binary_bit_write(&data, 8, cycle_bit & 2);
    binary_bit_write(&data, 9, cycle_bit & 4);
    //binary_printf(data);

    cc3220_i2c_write_16bit((uint8_t)tmp117_addr,
                           (uint8_t)TMP117_CONFIGURATION, data);

    cc3220_i2c_close();
    return 0;
}

int8_t tmp117_avg_mode_set(int8_t avg_bit)
{
    int16_t data;

    cc3220_i2c_init();
    cc3220_i2c_open();
    cc3220_i2c_read_16bit((uint8_t)tmp117_addr,
                             (uint8_t)TMP117_CONFIGURATION, &data);

    //binary_printf(data);
    binary_bit_write(&data, 5, avg_bit & 1);
    binary_bit_write(&data, 6, avg_bit & 2);
    //binary_printf(data);

    cc3220_i2c_write_16bit((uint8_t)tmp117_addr,
                           (uint8_t)TMP117_CONFIGURATION, data);

    cc3220_i2c_close();
    return 0;
}

int8_t tmp117_alert_mode_set(int8_t alert_bit)
{
    int16_t data;

    cc3220_i2c_init();
    cc3220_i2c_open();
    cc3220_i2c_read_16bit((uint8_t)tmp117_addr,
                          (uint8_t)TMP117_CONFIGURATION, &data);

    //binary_printf(data);
    binary_bit_write(&data, 4, alert_bit & 1);
    //binary_printf(data);

    cc3220_i2c_write_16bit((uint8_t)tmp117_addr,
                           (uint8_t)TMP117_CONFIGURATION, data);

    cc3220_i2c_close();
    return 0;
}

int8_t tmp117_soft_reset()
{
    int16_t data;

    cc3220_i2c_init();
    cc3220_i2c_open();
    cc3220_i2c_read_16bit((uint8_t)tmp117_addr,
                          (uint8_t)TMP117_CONFIGURATION, &data);

    //binary_printf(data);
    binary_bit_write(&data, 1, 1);
    //binary_printf(data);

    cc3220_i2c_write_16bit((uint8_t)tmp117_addr,
                           (uint8_t)TMP117_CONFIGURATION, data);

    cc3220_i2c_close();
    return 0;
}

int8_t tmp117_init()
{
    tmp117_begin();
    tmp117_avg_mode_set(0b01);
    tmp117_conv_mode_set(0b00);
    tmp117_alert_mode_set(0b01);
    return 0;
}

int8_t tmp117_high_limit_set(int16_t high_lim)
{
    cc3220_i2c_init();
    cc3220_i2c_open();

    int16_t final_limit = high_lim / TMP117_RESOLUTION;

    printf("%d ",final_limit);
    binary_printf(final_limit);

    cc3220_i2c_write_16bit((uint8_t)tmp117_addr,
                           (uint8_t)TMP117_T_HIGH_LIMIT, final_limit);

    cc3220_i2c_close();
    return 0;
}

int8_t tmp117_low_limit_set(int16_t low_lim)
{
    cc3220_i2c_init();
    cc3220_i2c_open();

    int16_t final_limit = low_lim / TMP117_RESOLUTION;

    printf("%d ",final_limit);
    binary_printf(final_limit);

    cc3220_i2c_write_16bit((uint8_t)tmp117_addr,
                           (uint8_t)TMP117_T_LOW_LIMIT, final_limit);


    cc3220_i2c_close();
    return 0;
}


int8_t tmp117_high_limit_get()
{
    int16_t data;

    cc3220_i2c_init();
    cc3220_i2c_open();
    cc3220_i2c_read_16bit((uint8_t)tmp117_addr,
                          (uint8_t)TMP117_T_HIGH_LIMIT, &data);
    cc3220_i2c_close();

    int16_t final_limit = data * TMP117_RESOLUTION;
    printf("high %d ", final_limit);

    binary_printf(data);
    return 0;
}

int8_t tmp117_low_limit_get()
{
    int16_t data;

    cc3220_i2c_init();
    cc3220_i2c_open();
    cc3220_i2c_read_16bit((uint8_t)tmp117_addr,
                          (uint8_t)TMP117_T_LOW_LIMIT, &data);
    cc3220_i2c_close();

    int16_t final_limit = data * TMP117_RESOLUTION;
    printf("low %d ", final_limit);

    binary_printf(data);

    return 0;
}

int8_t tmp117_data_ready_check()
{
    int16_t data;

    cc3220_i2c_init();
    cc3220_i2c_open();
    cc3220_i2c_read_16bit((uint8_t)tmp117_addr,
                          (uint8_t)TMP117_CONFIGURATION, &data);

    cc3220_i2c_close();

    if(data & 1 << 13) {
        return 0;
    } else {
        return -1;
    }
}
