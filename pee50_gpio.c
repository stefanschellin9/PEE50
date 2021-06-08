/*
 * pee50_gpio.c
 *
 *  Created on: 3 jun. 2021
 *      Author: stefa
 */

#include <ti/drivers/GPIO.h>

#include "ti_drivers_config.h"

#include "pee50_gpio.h"

void gpio_init()
{
    GPIO_init();
    GPIO_setConfig(CONFIG_GPIO_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW);
}

void gpio_fan_on()
{
    GPIO_write(CONFIG_GPIO_0, CONFIG_LED_ON);
}

void gpio_fan_off()
{
    GPIO_write(CONFIG_GPIO_0, CONFIG_LED_OFF);
}

void gpio_led_toggle()
{
    GPIO_toggle(CONFIG_GPIO_2);
}

void gpio_schakelaar_on()
{
    GPIO_write(CONFIG_GPIO_1, CONFIG_LED_ON);
}

void gpio_schakelaar_off()
{
    GPIO_write(CONFIG_GPIO_1, CONFIG_LED_OFF);
}
