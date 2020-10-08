#include "main.h"
#include "defines.h"
#include "driver/periph_ctrl.h"
#include <math.h>
#include "audio_table.h"
#include "driver/i2s.h"


void v_task_make_noise()
{
    size_t i2s_bytes_write = 0;

    while (true)
    {
      i2s_write(0, audio_table, sizeof(audio_table), &i2s_bytes_write, 100);
    }

}


void app_main(void)
{
    dac_output_enable(DAC_CHANNEL_1);

    uart_console_init();

    esp_console_cmd_t cmd_en_noise_conf = {
        .command = "noise-on",
        .func = &cmd_noise_on,
    };

    esp_console_cmd_t cmd_dis_noise_conf = {
        .command = "noise-off",
        .func = &cmd_noise_off,
    };

    esp_console_cmd_t cmd_exit_conf = {
        .command = "exit",
        .func = &cmd_exit,
    };

    esp_console_cmd_register(&cmd_en_noise_conf);
    esp_console_cmd_register(&cmd_dis_noise_conf);
    esp_console_cmd_register(&cmd_exit_conf);

   static const int i2s_num = 0;

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

    i2s_driver_install(i2s_num, &i2s_config, 0, NULL); 
    i2s_set_pin(i2s_num, NULL);


    i2s_stop(0);
    xTaskCreate(v_task_make_noise, "play some music", 2048, NULL, 1, NULL);
  
}
