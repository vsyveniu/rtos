#ifndef COMMANDS_H
# define COMMANDS_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_console.h"
#include "esp_err.h"
#include "uart_utils_funcs.h"
#include "driver/ledc.h"
#include "defines.h"
#include "blinker.h"

int     cmd_handle(int argc, char **argv);
int     cmd_exit();

#endif
