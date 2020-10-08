#ifndef I2SNOISE_H
# define I2SNOISE_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_err.h"
#include <unistd.h>
#include "freertos/timers.h"
#include "driver/timer.h"
#include "defines.h"
#include "driver/i2s.h"

int i2s_setup();

#endif
