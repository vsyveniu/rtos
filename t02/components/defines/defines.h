#ifndef DEFINES_H
# define DEFINES_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "driver/timer.h"
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
#define DHT_PIN GPIO_NUM_4
#define DHT_POWER GPIO_NUM_2

typedef struct dht_data_struct {
    uint8_t   temperature;
    uint8_t   humidity;
    double    timestamp;
} dht_data_s;

QueueHandle_t dht_queue;
dht_data_s dht_log[60];
TimerHandle_t dht_timer;
double time_diff;

#endif