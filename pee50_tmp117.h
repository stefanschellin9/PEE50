/*
 * pee50_tmp117.h
 *
 *  Created on: 26 mei 2021
 *      Author: stefanschellin9
 */

#ifndef TMP117_H_PEE50
#define TMP117_H_PEE50

#define tmp117_addr         0x48
#define DEVICE_ID_VALUE     0x0117
#define TMP117_RESOLUTION   0.0078125f



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

void binary_printf(int16_t data);

void binary_bit_write(int16_t *data, uint16_t bit_num, uint8_t state);

int8_t tmp117_begin();

float tmp117_read_temp_c();

int8_t tmp117_conv_mode_set(int8_t conv_bit);

int8_t tmp117_conv_cycle_set(int8_t cycle_bit);

int8_t tmp117_avg_mode_set(int8_t avg_bit);

int8_t tmp117_alert_mode_set(int8_t alert_bit);

int8_t tmp117_soft_reset();

int8_t tmp117_init();

int8_t tmp117_high_limit_set(int16_t high_lim);

int8_t tmp117_low_limit_set(int16_t low_lim);

int8_t tmp117_high_limit_get();

int8_t tmp117_low_limit_get();

int8_t tmp117_data_ready_check();

#endif /* TMP117_H_PEE50 */
