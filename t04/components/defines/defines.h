#ifndef DEFINES_H
# define DEFINES_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#define UART_NUMBER             UART_NUM_1
#define UART_TX_PIN             17
#define UART_RX_PIN             16
#define EN_AMP                  23
#define PI                      (3.14159265)
#define SAMPLE_RATE             (44100)
#define WAVE_FREQ_HZ            (1000)
#define SAMPLE_PER_CYCLE        (SAMPLE_RATE/WAVE_FREQ_HZ)


#endif