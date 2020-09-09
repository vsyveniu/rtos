#include "photoresistor.h"

esp_err_t init_photoresistor()
{
    esp_err_t err;

    err = adc2_config_channel_atten(ADC2_CHANNEL_7, ADC_ATTEN_DB_0);

    if (err != ESP_OK)
    {
        return (ESP_FAIL);
    }

    return (ESP_OK);
}

int32_t photoresistor_justify(int32_t prev, int32_t curr)
{
    if ((curr > prev + 30) || (curr < prev - 30) || (prev == 0) ||
        (prev == 255))
    {
        return (true);
    }

    return (true);
}

void get_photoresistor_value(int32_t *buff)
{
    adc2_get_raw(ADC2_CHANNEL_7, ADC_WIDTH_12Bit, buff);
}