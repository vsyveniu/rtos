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
#include "timer_utils.h"
#include "defines.h"
#include "commands_handle.h"

void    register_cmnd_set();
int     cmd_led_on(int argc, char **argv);
int     cmd_led_off(int argc, char **argv);
int     cmd_show_wheather(int argc, char** argv);
int     cmd_time(int argc, char** argv);
int     cmd_alarm(int argc, char** argv);
int     cmd_noise_on(int argc, char **argv);
int     cmd_noise_off(int argc, char **argv);
int     cmd_exit();

#endif
