#ifndef COMMANDS_HANDLE_H
# define COMMANDS_HANDLE_H

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
#include "oled.h"
#include "timer_utils.h"
#include "dht11.h"
#include "freertos/semphr.h"
#include "defines.h"
#include "argtable3/argtable3.h"
#include "driver/i2s.h"

void IRAM_ATTR timer_alarm_handle(void *param);
void handle_alarm(struct arg_rex *alarm_set);
void handle_time(struct arg_rex *val);
void handle_led_on(struct arg_lit *first, struct arg_lit *second, struct arg_lit *third, struct arg_lit *all);
void handle_led_off(struct arg_lit *first, struct arg_lit *second, struct arg_lit *third, struct arg_lit *all);
void handle_help();

#endif