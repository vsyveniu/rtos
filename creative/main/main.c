#include "main.h"
#include "defines.h"
#include "driver/periph_ctrl.h"
#include "soc/timer_group_struct.h"
#include <sys/time.h>
#include "sntp.h"

static TaskHandle_t notify_time_change = NULL;


xSemaphoreHandle mutexSensor;

void x_task_dht()
{
    uint8_t dht_buff[5];
    memset(dht_buff, 0, 5);
    dht_data_s dht_data[1];

    while (true)
    {
        get_DHT_data(dht_buff);
        dht_data->temperature =  dht_buff[2];
        dht_data->humidity =  dht_buff[0];
        xQueueSend(dht_queue, &dht_data, 10);

        vTaskDelay(5000 / portTICK_PERIOD_MS); 

    }
}

void x_task_buffer_push()
{
    dht_data_s dht_data_receive[1];

    while (true)
    {
        if(xQueueReceive(dht_queue, &dht_data_receive, 10))
        {
            if(dht_log[0].temperature != dht_data_receive->temperature || dht_log[0].humidity != dht_data_receive->humidity)
            {
                dht_log[0].temperature = dht_data_receive->temperature;
                dht_log[0].humidity = dht_data_receive->humidity;
            }
        }
    }
}

void app_main(void)
{
    esp_err_t err;

    gpio_set_direction(DHT_POWER, GPIO_MODE_OUTPUT);	
	gpio_set_level(DHT_POWER, 1);
    gpio_set_direction(EN_OLED, GPIO_MODE_OUTPUT);
	gpio_set_level(EN_OLED, 1);
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    err = uart_console_init();
    if(err != ESP_OK){
        printf("%s\n", "couldn't init uart console. Have to reboot");
        esp_restart();
    }
    esp_console_cmd_t cmd_led_on_conf = {
        .command = "led-on",
        .func = &cmd_ledon,
    };
     esp_console_cmd_t cmd_led_off_conf = {
        .command = "led-off",
        .func = &cmd_ledoff,
    };
    esp_console_cmd_t cmd_exit_conf = {
        .command = "exit",
        .func = &cmd_exit,
    };

    esp_console_cmd_register(&cmd_led_on_conf);
    esp_console_cmd_register(&cmd_led_off_conf);
    esp_console_cmd_register(&cmd_exit_conf);
/* 
    struct timeval current_time;

    sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);

    while (true)
    {
        gettimeofday(&current_time, NULL);
    printf("seconds : %ld\n  micro seconds : %ld\n",
        current_time.tv_sec, current_time.tv_usec);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    } */

    err = init_oled();
    if(err != ESP_OK){
        printf("%s\n", "couldn't initiate oled");
    }
    fill_oled();
    clear_oled();


    dht_queue = xQueueCreate( 1, sizeof( dht_data_s) );

    xTaskCreate(x_task_dht, "get dht data task", 2048, NULL, 1, NULL);
    xTaskCreate(x_task_buffer_push, "push dht data tostatic buffer", 2048, NULL, 1, NULL);

}
