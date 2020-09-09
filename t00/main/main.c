#include "main.h"

xSemaphoreHandle mutexSensor;
static TaskHandle_t send_data_to_oled = NULL;

void read_light_sensor(void* param)
{
    int32_t raw = 0;
    int32_t prev = 0;

    while (true)
    {
        if (xSemaphoreTake(mutexSensor, portMAX_DELAY))
        {
            get_photoresistor_value(&raw);
            if (photoresistor_justify(prev, raw))
            {
                xTaskNotify(send_data_to_oled, raw, eSetValueWithOverwrite);
                prev = raw;
                xSemaphoreGive(mutexSensor);
            }
            else
            {
                xTaskNotify(send_data_to_oled, prev, eSetValueWithOverwrite);
                xSemaphoreGive(mutexSensor);
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void change_brightness(void* param)
{
    uint32_t raw = 0;
    static uint32_t prev = 0;

    while (true)
    {
        if (xSemaphoreTake(mutexSensor, portMAX_DELAY))
        {
            xTaskNotifyWait(0xffffffff, 0, &raw, portMAX_DELAY);
            uint32_t val = raw / 16;
            if (raw != prev)
            {
                reconfigure_oled(2, 0x81, val);
                fill_oled();
                prev = val;
                vTaskDelay(500 / portTICK_PERIOD_MS);
            }
            xSemaphoreGive(mutexSensor);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    power_up();

    esp_err_t err;

    err = init_photoresistor();

    if (err != ESP_OK)
    {
        printf("%s\n", "couldn't configure photoresistor channel");
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
                &send_data_to_oled);
}
