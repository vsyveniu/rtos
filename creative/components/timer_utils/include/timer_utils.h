#ifndef TIMER_UTILS_H
# define TIMER_UTILS_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_err.h"
#include <unistd.h>
#include "defines.h"

char *make_time_str(uint8_t hours, uint8_t minutes, uint8_t seconds);

#endif
