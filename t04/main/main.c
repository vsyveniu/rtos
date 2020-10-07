#include "main.h"
#include "defines.h"
#include "driver/periph_ctrl.h"
#include <math.h>

/* #define PI                      (3.14159265)
#define SAMPLE_RATE             (36000)
#define WAVE_FREQ_HZ            (10000)
#define SAMPLE_PER_CYCLE        (SAMPLE_RATE/WAVE_FREQ_HZ) */        //44.1


void make_noise(int bits)
{
    int *samples_data = malloc(((bits+8)/16)*SAMPLE_PER_CYCLE*4);      //264.6
    unsigned int i, sample_val;
    double sin_float, triangle_float, triangle_step = (double) pow(2, bits) / SAMPLE_PER_CYCLE; //  5,8
    size_t i2s_bytes_write = 0;

    printf("\r\nTest bits=%d free mem=%d, written data=%d\n", bits, esp_get_free_heap_size(), ((bits+8)/16)*SAMPLE_PER_CYCLE*4);

    triangle_float = -(pow(2, bits)/2 - 1);  //127

    for(i = 0; i < SAMPLE_PER_CYCLE; i++) {
        sin_float = sin(i * 2 * PI / SAMPLE_PER_CYCLE); 
        printf("%f\n", sin_float);  
        if(sin_float >= 0)
            triangle_float += triangle_step;
        else
            triangle_float -= triangle_step;


        sin_float *= (pow(2, bits)/2 - 1);
                               
        if (bits == 16) {
            sample_val = 0;
            sample_val += (short)triangle_float;
            sample_val = sample_val << 16;
            sample_val += (short) sin_float;
            samples_data[i] = sample_val;
        } else if (bits == 24) { //1-bytes unused
            samples_data[i*2] = ((int) triangle_float) << 8;
            samples_data[i*2 + 1] = ((int) sin_float) << 8;
        }else {
            samples_data[i*2] = ((int) triangle_float);
            samples_data[i*2 + 1] = ((int) sin_float);
        }
    }

    i2s_set_clk(0, SAMPLE_RATE, bits, I2S_CHANNEL_MONO);
    //Using push
    // for(i = 0; i < SAMPLE_PER_CYCLE; i++) {
    //     if (bits == 16)
    //         i2s_push_sample(0, &samples_data[i], 100);
    //     else
    //         i2s_push_sample(0, &samples_data[i*2], 100);
    // }
    // or write
    i2s_write(0, samples_data, ((bits+8)/16)*SAMPLE_PER_CYCLE*4, &i2s_bytes_write, 100);

    free(samples_data);
}





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
    //i2s_set_sample_rates(i2s_num, 22050);

    
    int test_bits = 16;
    while (true)
    {
        make_noise(test_bits);
        vTaskDelay(1000/portTICK_RATE_MS);
        test_bits += 8;
        if(test_bits > 32)
            test_bits = 16;
    }
/*       esp_vfs_spiffs_conf_t config = {
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
    } */
 

}
