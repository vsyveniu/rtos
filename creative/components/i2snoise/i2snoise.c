#include "i2snoise.h"


int i2s_setup()
{
    esp_err_t err;

   const int i2s_num = 0;

   static const i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = BITS_PER_SAMPLE,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_MSB,
        .intr_alloc_flags = 0,
        .dma_buf_count = DMA_BUF_COUNT,
        .dma_buf_len = DMA_BUF_LEN,
        .use_apll = 1
    }; 

    err = i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
    if(err != ESP_OK)
    {
        return (err);
    } 
    err = i2s_set_pin(i2s_num, NULL);
    if(err != ESP_OK)
    {
        return (err);
    } 

    i2s_stop(0);

    return (ESP_OK);
}
