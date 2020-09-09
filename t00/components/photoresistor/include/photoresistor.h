#ifndef PHOTORESISTOR_H
# define PHOTORESISTOR_H

#include <driver/adc.h>
#include <stdio.h>

esp_err_t init_photoresistor();
void get_photoresistor_value(int32_t *buff);
int32_t photoresistor_justify(int32_t prev, int32_t curr);

#endif
