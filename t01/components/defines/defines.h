#ifndef DEFINES_H
# define DEFINES_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#define LED_1 27
#define LED_2 26
#define LED_3 33
#define LEDC_SPEED_MODE 		LEDC_HIGH_SPEED_MODE
#define LEDC_DUTY  				(1023)
#define LEDC_DUTY_RESOLUTION	LEDC_TIMER_10_BIT
#define LEDC_FREQENCY			(1000)
#define UART_NUMBER             UART_NUM_1
#define UART_TX_PIN             17
#define UART_RX_PIN             16

#endif