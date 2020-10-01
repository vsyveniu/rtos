#include "main.h"
#include "defines.h"

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
     dht_data_s dht_data_curr[1];
     dht_data_s dht_data_replace[1];

    while (true)
    {
        if(xQueueReceive(dht_queue, &dht_data_receive, 10))
        {
                if(dht_log[0].temperature != dht_data_receive->temperature || dht_log[0].humidity != dht_data_receive->humidity)
                {
                    timer_start(TIMER_GROUP_0, 0);
                    timer_start(TIMER_GROUP_0, 1);

                    double last_item_time = 0;
                    double common_time = 0;
                    timer_get_counter_time_sec(TIMER_GROUP_0, 0, &last_item_time);
                    timer_get_counter_time_sec(TIMER_GROUP_0, 1, &common_time);
                
                    dht_data_replace->temperature = dht_log[0].temperature;
                    dht_data_replace->humidity = dht_log[0].humidity;
                    dht_data_replace->timestamp = dht_log[0].timestamp;

                    for(int j = 1; j < 60; j++)
                    {
                        dht_data_curr->temperature = dht_log[j].temperature;
                        dht_data_curr->humidity = dht_log[j].humidity;
                        dht_data_curr->timestamp = dht_log[j].timestamp;
                        
                        dht_log[j].temperature = dht_data_replace->temperature;
                        dht_log[j].humidity = dht_data_replace->humidity;
                        dht_log[j].timestamp = dht_data_replace->timestamp;
                        
                        dht_data_replace->temperature = dht_data_curr->temperature;
                        dht_data_replace->humidity = dht_data_curr->humidity; 
                        dht_data_replace->timestamp = dht_data_curr->timestamp; 
                    }

                    dht_log[0].temperature = dht_data_receive->temperature;
                    dht_log[0].humidity = dht_data_receive->humidity;
                    dht_log[0].timestamp = common_time;
                    timer_set_counter_value(TIMER_GROUP_0, 0, 0);

                }
        }
    }
}

void app_main(void)
{
    gpio_set_direction(DHT_POWER, GPIO_MODE_OUTPUT);	
	gpio_set_level(DHT_POWER, 1);
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    uart_console_init();
   
    esp_console_cmd_t cmd_dhtlog_conf = {
        .command = "dhtlog",
        .func = &cmd_dhtlog,
    };

    esp_console_cmd_t cmd_exit_conf = {
        .command = "exit",
        .func = &cmd_exit,
    };

    esp_console_cmd_register(&cmd_dhtlog_conf);
    esp_console_cmd_register(&cmd_exit_conf);

    timer_config_t dht_timer_conf = {
        .alarm_en = false,
		.counter_en = false,
		.counter_dir = TIMER_COUNT_UP,
		.auto_reload = true,
		.divider = 16,
    };

    timer_config_t dht_update_timer_conf = {
        .alarm_en = false,
		.counter_en = false,
		.counter_dir = TIMER_COUNT_UP,
		.auto_reload = true,
		.divider = 16,
    };

    timer_init(TIMER_GROUP_0, 0, &dht_timer_conf);
    timer_init(TIMER_GROUP_0, 1, &dht_update_timer_conf);

    for (int j = 0; j < 60; j++){
        dht_log[j].temperature = 0;
        dht_log[j].humidity = 0;
        dht_log[j].timestamp = 0.00;
    }

    dht_queue = xQueueCreate( 60, sizeof( dht_data_s) );

    xTaskCreate(x_task_dht, "get dht data task", 2048, NULL, 1, NULL);
    xTaskCreate(x_task_buffer_push, "push dht data tostatic buffer", 2048, NULL, 1, NULL);

}
