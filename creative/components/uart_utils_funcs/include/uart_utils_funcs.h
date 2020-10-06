#ifndef UART_UTILS_FUNCS_H
# define UART_UTILS_FUNCS_H

#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include "defines.h"
#include "esp_err.h"
#include "driver/uart.h"

void   uart_print_str(int uart_num, char *str);

#endif
