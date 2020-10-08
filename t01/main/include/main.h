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
#include "defines.h"

#define LEDC_SPEED_MODE 		LEDC_HIGH_SPEED_MODE
#define LEDC_DUTY  				(1023)
#define LEDC_DUTY_RESOLUTION	LEDC_TIMER_10_BIT
#define LEDC_FREQENCY			(1000)

#endif