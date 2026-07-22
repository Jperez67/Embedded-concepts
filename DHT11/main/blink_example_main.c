#include <stdint.h>
#include <stdio.h>
#include "driver/gpio.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
const uint8_t DHT_11_DATA_PIN = 4;
uint8_t data[5] = {0};
uint8_t checksum = 0, data_valid = 0;
void app_main(void)
{
    while (1)
    {
        gpio_set_direction(DHT_11_DATA_PIN, GPIO_MODE_OUTPUT);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        // send start signal the DHT11 sensor
        gpio_set_level(DHT_11_DATA_PIN, 0);
        esp_rom_delay_us(18000); // wait for 18ms

        gpio_set_level(DHT_11_DATA_PIN, 1);
        esp_rom_delay_us(30);                                 // wait for 30us
        gpio_set_direction(DHT_11_DATA_PIN, GPIO_MODE_INPUT); // set pin to input to read data from DHT11 sensor

        // DHT Responds by pulling low for 80us and then high for 80us
        while (gpio_get_level(DHT_11_DATA_PIN) == 1)
            ; // waiting for DHT11 response signal from sensor to MCU
        esp_rom_delay_us(80);

        if (gpio_get_level(DHT_11_DATA_PIN) == 1) // if signal is still low after 80us, then the DHT11 sensor is responding incorrectly
        {
            printf("DHT11 sensor is responding\n");
        }
        else
        {
            printf("DHT11 sensor error\n");
        }
        esp_rom_delay_us(80); // waiting for data transmission to start from DHT11 sensor to MCU

        // now read the data from the DHT11 sensor
        for (int i = 0; i < 5; i++)
        {
            for (int j = 7; j >= 0; j--)
            {
                while (gpio_get_level(DHT_11_DATA_PIN) == 0)
                    ;                 // wait for pin to be pulled high to read data
                esp_rom_delay_us(40); // wait for 40us to see if it is a 0 or 1
                if (gpio_get_level(DHT_11_DATA_PIN) == 1)
                {
                    data[i] |= (1ull << j);
                }
                else
                {
                    data[i] &= ~(1ull << j);
                }

                // now to decypher the data from the DHT11 sensor
                //  now display the data on the console
            }
        }
        // now to check sum
        for (int i = 0; i < 4; i++)
        {
            checksum = data[i];
        }
        if (checksum == data[4])
        {
            printf("CheckSum: data is valid %d = %d \n", checksum, data[4]);
            data_valid = 1;
        }
        else
        {
            printf("CheckSum: data is not valid %d = %d\n", checksum, data[4]);
            data_valid = 0;
        }

        // print data if data is valid
        if (data_valid == 1)
        {
            printf("Humidity: %d.%d Temp: %d.%d\n", data[0], data[1], data[2], data[3]);
        }
    }
}