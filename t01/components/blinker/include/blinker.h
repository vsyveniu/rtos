#ifndef BLINKER_H
# define BLINKER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include <driver/dac.h>
#include "defines.h"

void ledc_channel_setup(ledc_timer_t timer_num, int32_t frequency, ledc_channel_t channel, int gpio);

#endif
