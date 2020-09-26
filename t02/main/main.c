#include "main.h"
#include "defines.h"

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

    //dht_data_s *dht_data;

    //dht_data = (dht_data_s *)malloc(sizeof(dht_data_s));

    dht_data_s *p_dht_log;

    dht_data_s dht_data[1];
    dht_data_s dht_log[60];
    dht_data_s dht_data_receive[1];

    for (int j = 0; j < 60; j++){
        dht_log[j].temperature = 0;
        dht_log[j].humidity = 0;
        dht_log[j].timestamp = 0;
    }

    p_dht_log = dht_log;

    dht_data->temperature = 0;
    dht_data->humidity = 0;
    dht_data->timestamp = 0;

    

    uint8_t dht_buff[5];
    memset(dht_buff, 0, 5);

    QueueHandle_t dht_queue;

    dht_queue = xQueueCreate( 10, sizeof( dht_data_s) );

    UBaseType_t fuck;

     int8_t i = 0;   

    while (true)
    {
        get_DHT_data(dht_buff);
        dht_data->temperature =  dht_buff[2];
        dht_data->humidity =  dht_buff[0];
        dht_log[i].temperature = dht_buff[2];
        dht_log[i].humidity = dht_buff[0];
        
     /*    printf("%d temperature \n", dht_buff[2]);
        printf("%d humidity \n", dht_buff[0]); */
        printf("%d temperature \n", dht_data->temperature);
        printf("%d humidity \n", dht_data->humidity);
        printf("\n-----------\n");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        xQueueSendToFront(dht_queue, &dht_data, 10);
        xQueuePeek(dht_queue, &dht_data_receive, 10);
        printf("%d\n", i);
/*         printf("%d receive temperature \n", dht_data_receive->temperature);
        printf("%d receive humidity \n", dht_data_receive->humidity);
        fuck = uxQueueMessagesWaiting(dht_queue);
         printf("%d mess waiting\n", fuck); */
/*         printf("\n-----------\n");
         printf("%d log temperature \n", p_dht_log->temperature);
        printf("%d log humidity \n", p_dht_log->humidity);
        printf("\n-----------\n");
        p_dht_log--; */

/*            printf("\n-----------\n");
         printf("%d log temperature \n", dht_log[i].temperature);
        printf("%d log humidity \n", dht_log[i].humidity);
        printf("\n-----------\n");
        p_dht_log--;  */

/*         for (int k  = 60; k > 0; k--){
         printf("temperature %d  k %d\n", dht_log[k].temperature, k);
         printf("humidity    %d  k %d\n", dht_log[k].humidity, k);
        }
 */
        i++;    
    }

}
