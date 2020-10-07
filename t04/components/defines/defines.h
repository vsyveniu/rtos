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
#define SAMPLE_RATE             (16000)
#define BITS_PER_SAMPLE         (16)
#define DMA_BUF_COUNT           (2)
#define DMA_BUF_LEN             (1024)

#endif