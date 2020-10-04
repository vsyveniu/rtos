#include "main.h"
#include "defines.h"
#include "driver/periph_ctrl.h"

void app_main(void)
{
  esp_err_t err;
  
   gpio_set_direction(EN_AMP, GPIO_MODE_OUTPUT);
    gpio_set_level(EN_AMP, 1);
    dac_output_enable(DAC_CHANNEL_1);

   static const int i2s_num = 0;

    static const i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
        .sample_rate = 44100,
        .bits_per_sample = 16,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false
    };

    i2s_driver_install(i2s_num, &i2s_config, 0, NULL); 
    i2s_set_pin(i2s_num, NULL);
    i2s_set_sample_rates(i2s_num, 22050);

 
      esp_vfs_spiffs_conf_t config = {
        .base_path = "/spiffs",
        .partition_label = "storage",
        .max_files = 1,
        .format_if_mount_failed = true,
    };
    err = esp_vfs_spiffs_register(&config);
    if(err != ESP_OK){
      printf("dsff");
    }

    int fuck;

    fuck = esp_spiffs_mounted("storage");

    printf("%d\n", fuck);
    //ize_t


      FILE *file = fopen("/spiffs/sub/truba.mp3", "rb");
    
    long numbytes;

    if(file ==NULL)
    {
      printf("%s\n", "could not open file");
    }
    else 
    {
      printf("%s\n", "fuck");
      fseek(file, 0L, SEEK_END);
      numbytes = ftell(file);
      printf("Reading high score: %ld\n", numbytes);
      char *buffer;

      buffer = (char *)malloc(273344);
      if(buffer != NULL){
        printf("%s\n", "K");
      }

      fread(&buffer, sizeof(char), numbytes, file);

      fclose(file);

      printf("%d\n", strlen(buffer));
    }
 

}
