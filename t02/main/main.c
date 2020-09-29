#include "main.h"
#include "defines.h"

xSemaphoreHandle mutexSensor;

void x_task_dht()
{
    uint8_t dht_buff[5];
    memset(dht_buff, 0, 5);
    dht_data_s dht_data[1];
    UBaseType_t fuck = 0;

    dht_data_s *p_dht_log = NULL;
    p_dht_log = dht_queue;


    while (true)
    {
        get_DHT_data(dht_buff);
        dht_data->temperature =  dht_buff[2];
        dht_data->humidity =  dht_buff[0];
        xQueueSend(dht_queue, &dht_data, 10);
        
        
        //p_dht_log+;
       

/*         printf("%d receive temperature \n", p_dht_log->temperature);
        printf("%d receive humidity \n", p_dht_log->humidity); */
        
       // fuck = uxQueueMessagesWaiting(dht_queue);
        vTaskDelay(5000 / portTICK_PERIOD_MS); 

    }
}

void x_task_dht_update_timestamp()
{

    double stored_time = 0;
    double time_replace = 0;

    while (true)
    {
         if (xSemaphoreTake(mutexSensor, portMAX_DELAY))
            {
                double time = 0;
                double fresh_item_time = 0;
                timer_get_counter_time_sec(TIMER_GROUP_0, 1, &time);
                timer_get_counter_time_sec(TIMER_GROUP_0, 0, &fresh_item_time);

                dht_log[0].timestamp = fresh_item_time;

                for(int i = 1; i < 60; i++)
                {
                    if(dht_log[i].timestamp != 0)
                    {
                        dht_log[i].timestamp += time;
                        printf("%f\n", time);       
                    }
                    if(dht_log[i].timestamp == 0){
                        break;
                    }
                }
                xSemaphoreGive(mutexSensor);                
            }
            vTaskDelay(1000 / portTICK_PERIOD_MS);
           // timer_set_counter_value(TIMER_GROUP_0, 1, 0);
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

            if (xSemaphoreTake(mutexSensor, portMAX_DELAY))
            { 
                
                /*  timer_get_counter_time_sec(TIMER_GROUP_0, 0, &time);

                    printf("%d hours %d minutes %d seconds ago\n", (int)time / 3600, (int)time / 60, (int)time);
                */

                if(dht_log[0].temperature != dht_data_receive->temperature || dht_log[0].humidity != dht_data_receive->humidity)
                {
                    timer_start(TIMER_GROUP_0, 0);
                    timer_start(TIMER_GROUP_0, 1);

                    double last_item_time = 0;
                    double common_time = 0;
                    timer_get_counter_time_sec(TIMER_GROUP_0, 0, &last_item_time);
                    timer_get_counter_time_sec(TIMER_GROUP_0, 1, &common_time);
                    time_diff += common_time;
                   
                     
                    dht_data_replace->temperature = dht_log[0].temperature;
                    dht_data_replace->humidity = dht_log[0].humidity;
                    dht_data_replace->timestamp = dht_log[0].timestamp;

                    for(int j = 1; j < 60; j++)
                    {
                        dht_data_curr->temperature = dht_log[j].temperature;
                        dht_data_curr->humidity = dht_log[j].humidity;
                        dht_data_curr->timestamp = dht_log[j].timestamp;
                        
                        dht_log[j].temperature =  dht_data_replace->temperature;
                        dht_log[j].humidity = dht_data_replace->humidity;
                        dht_log[j].timestamp = dht_data_replace->timestamp;
                        
                        dht_data_replace->temperature = dht_data_curr->temperature;
                        dht_data_replace->humidity = dht_data_curr->humidity; 
                        dht_data_replace->timestamp = dht_data_curr->timestamp; 
                    }

                    int i = 2;
                        while (i < 60){
                            
                            if(dht_log[i].timestamp != 0){

                                dht_log[i].timestamp += common_time;
                            }
                        i++;
                     } 
                     timer_set_counter_value(TIMER_GROUP_0, 1, 0);

                    dht_log[0].temperature = dht_data_receive->temperature;
                    dht_log[0].humidity = dht_data_receive->humidity;
                    dht_log[0].timestamp = last_item_time;

                    timer_set_counter_value(TIMER_GROUP_0, 0, 0);

                    
                }
                 xSemaphoreGive(mutexSensor);
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
   
    esp_console_cmd_t esp_comm = {
        .command = "dhtlog",
        .func = &cmd_dhtlog,
    };

    esp_console_cmd_t cmd_exit_conf = {
        .command = "exit",
        .func = &cmd_exit,
        .argtable = NULL,
    };

    esp_console_cmd_register(&esp_comm);
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
    //dht_data_s *dht_data;

    //dht_data = (dht_data_s *)malloc(sizeof(dht_data_s));


   
    //dht_data_s dht_log[60];
   

    for (int j = 0; j < 60; j++){
        dht_log[j].temperature = 0;
        dht_log[j].humidity = 0;
        dht_log[j].timestamp = 0.00;
    }
    time_diff = 0;

    

    
    

    

    dht_queue = xQueueCreate( 60, sizeof( dht_data_s) );

    //dht_timer = xTimerCreate("DHT timestamp", portMAX_DELAY, pdFALSE, NULL, NULL);

    mutexSensor = xSemaphoreCreateMutex();

    xTaskCreate(x_task_dht, "get dht data task", 2048, NULL, 1, NULL);
    xTaskCreate(x_task_buffer_push, "push dht data tostatic buffer", 2048, NULL, 1, NULL);
   // xTaskCreate(x_task_dht_update_timestamp, "update timestamps values", 2048, NULL, 1, NULL);


        
/*         dht_data->temperature =  dht_buff[2];
        dht_data->humidity =  dht_buff[0];
        dht_log[i].temperature = dht_buff[2];
        dht_log[i].humidity = dht_buff[0]; */
        
     /*    printf("%d temperature \n", dht_buff[2]);
        printf("%d humidity \n", dht_buff[0]); */
/*         printf("%d temperature \n", dht_data->temperature);
        printf("%d humidity \n", dht_data->humidity);
        printf("\n-----------\n");
        vTaskDelay(2000 / portTICK_PERIOD_MS); */
        
      
   //     printf("%d\n", i);

/*         printf("\n-----------\n");
         printf("%d log temperature \n", p_dht_log->temperature);
        printf("%d log humidity \n", p_dht_log->humidity);
        printf("\n-----------\n");
        p_dht_log--; */

/*             printf("\n-----------\n");
         printf("%d log temperature \n", dht_log[i].temperature);
        printf("%d log humidity \n", dht_log[i].humidity);
        printf("\n-----------\n"); */
        

/*         for (int k  = 60; k > 0; k--){
         printf("temperature %d  k %d\n", dht_log[k].temperature, k);
         printf("humidity    %d  k %d\n", dht_log[k].humidity, k);
        }
 */
      //  i++;    

}
