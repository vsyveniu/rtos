#ifndef DEFINES_H
# define DEFINES_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "driver/timer.h"
#define LED_1                   27
#define LED_2                   26
#define LED_3                   33
#define UART_NUMBER             UART_NUM_1
#define UART_TX_PIN             17
#define UART_RX_PIN             16
#define DHT_PIN                 GPIO_NUM_4
#define DHT_POWER               GPIO_NUM_2
#define EN_OLED                 GPIO_NUM_32
#define I2C_SDA                 GPIO_NUM_21
#define I2C_SCL                 GPIO_NUM_22
#define OLED_ADDR               0x3c
#define TIMER_DIVIDER           80
#define TIMER_SCALE             (TIMER_BASE_CLK / TIMER_DIVIDER)

typedef struct dht_data_struct {
    uint8_t   temperature;
    uint8_t   humidity;
} dht_data_s;

QueueHandle_t dht_queue;
//dht_data_s dht_log[1];

#endif