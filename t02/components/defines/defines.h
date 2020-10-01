#ifndef DEFINES_H
# define DEFINES_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "driver/timer.h"
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

#endif