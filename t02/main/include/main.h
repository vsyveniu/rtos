#ifndef MAIN_H
# define MAIN_H

#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include <string.h>
#include <stdio.h>
#include "driver/ledc.h"
#include "freertos/semphr.h"
#include "driver/uart.h"
#include "esp_system.h"
#include "esp_console.h"
#include "commands.h"
#include "uart_utils_funcs.h"
#include "uart_console.h"
#include "dht11.h"
#include "defines.h"

typedef struct dht_data_struct {
    uint8_t   temperature;
    uint8_t   humidity;
    uint8_t  timestamp;
} dht_data_s;

#endif