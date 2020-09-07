#include "main.h"

xSemaphoreHandle mutexSensor;
static TaskHandle_t receive_from_oled_ready = NULL;

void read_light_sensor(void *param)
{
    int32_t raw = 0;

    while (true)
    {
        adc2_get_raw(ADC2_CHANNEL_7, ADC_WIDTH_12Bit, &raw);
        xTaskNotify(receive_from_oled_ready, raw, eSetBits);
        printf("%s\n", "sent");
        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }    
}

void change_brightness(void *param)
{
    uint32_t raw = 0;

    while (true)
    {
       xTaskNotifyWait(0xffffffff, 0, &raw, portMAX_DELAY);
        printf("%s\n", "received");
       reconfigure_oled(2, 0x81, raw / 16);
    }    
}

void app_main()
{
    power_up();

    esp_err_t err;

    err = adc2_config_channel_atten(ADC2_CHANNEL_7, ADC_ATTEN_DB_0);

    if(err != ESP_OK)
    {
        printf("%s\n", "couldn't configure adc2 channel");
    }

    err = init_oled();

    if(err != ESP_OK)
    {
        printf("%s\n", "couldn't init oled");
    }

    fill_oled();

    

     xTaskCreate(&read_light_sensor, "read light sensor value", 2048, "task 1", 1, &receive_from_oled_ready);
     xTaskCreate(&change_brightness, "change oled brightness", 2048, "task 2", 1, NULL);
     

}
