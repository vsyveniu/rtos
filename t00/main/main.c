#include "main.h"

xSemaphoreHandle mutexSensor;
static TaskHandle_t receive_from_oled_ready = NULL;

void read_light_sensor(void *param)
{
    int32_t raw = 0;

    while (true)
    {
        if (xSemaphoreTake(mutexSensor, 1000 / portTICK_PERIOD_MS))
        {
            adc2_get_raw(ADC2_CHANNEL_7, ADC_WIDTH_12Bit, &raw);
            xTaskNotify(receive_from_oled_ready, raw, eSetValueWithOverwrite);
            xSemaphoreGive(mutexSensor);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void change_brightness(void *param)
{
    uint32_t raw = 0;
    static uint32_t prev = 0;

    while (true)
    {
        if (xSemaphoreTake(mutexSensor, 1000 / portTICK_PERIOD_MS))
        {
            xTaskNotifyWait(0xffffffff, 0, &raw, portMAX_DELAY);
            // printf("%d\n", raw);
            printf("%d conf val \n", raw / 16);
            printf("%d prev val \n", prev);
            uint32_t val = raw / 16;
            if ((prev == 0) || (val == 0) || (val == 255) || ((val) > (prev + 20)) || ((val) < (prev - 20)))
            {
                reconfigure_oled(2, 0x81, raw / 16);
                fill_oled();
                prev = val;
            }
            vTaskDelay(100 / portTICK_PERIOD_MS);
            xSemaphoreGive(mutexSensor);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    power_up();

    esp_err_t err;

    err = adc2_config_channel_atten(ADC2_CHANNEL_7, ADC_ATTEN_DB_0);

    if (err != ESP_OK)
    {
        printf("%s\n", "couldn't configure adc2 channel");
        esp_restart();
    }

    err = init_oled();

    if (err != ESP_OK)
    {
        printf("%s\n", "couldn't init oled");
        esp_restart();
    }

    fill_oled();

    mutexSensor = xSemaphoreCreateMutex();

    xTaskCreate(&read_light_sensor, "read light sensor value", 2048, "task 1",
                1, NULL);
    xTaskCreate(&change_brightness, "change oled brightness", 2048, "task 2", 1,
                &receive_from_oled_ready);
}
