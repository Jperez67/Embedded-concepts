#include <stdint.h>
#include <stdio.h>
#include "driver/gpo.h"

const unint8_t DHT_11_DATA_PIN = 4;

void app_main(void)
{
    // Initialize the DHT11 sensor
    gpio_set_direction(DHT_11_DATA_PIN, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_level(DHT_11_DATA_PIN, 0); // sending starting signal from MCU to DHT11 sensor
    gpio_rom_delay_us(30);
    gpio_set_level(DHT_11_DATA_PIN, 1);
    gpio_rom_delay_us(40); // not sure if this is needed

    gpio_get_level(DHT_11_DATA_PIN); // waiting for DHT11 response signal from sensor to MCU
}