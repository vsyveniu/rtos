#include "main.h"
#include "defines.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"
#include "esp_vfs.h"
#include "esp_vfs_dev.h"


xQueueHandle inputQueue;
static QueueHandle_t uart0_queue;
xSemaphoreHandle mutexInput;
static TaskHandle_t send_data_to_oled = NULL;


void uart_event_task(void *pvParams){

    uart_event_t event;

    while (true){
        if(xQueueReceive(uart0_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
           g
            if(xSemaphoreTake(mutexInput, portMAX_DELAY))
            {
               // printf("queueu + sem");
                
              //  printf("%s\n", "uart event task queue");
             
              //uart_disable_rx_intr(UART_NUM_0);
              xSemaphoreGive(mutexInput);
              xTaskNotify(send_data_to_oled, 0, eSetValueWithOverwrite);
            }
        }
    }
   
}


void cmd_instance_task(void *pvParams){

    uart_event_t event;
       char* prompt = "> ";
       int ret;

    
 
   // stdin = fopen("/dev/uart/1", "r");
    //stdout = fopen("/dev/uart/1", "w");


    while (true){
        if(xSemaphoreTake(mutexInput, portMAX_DELAY)) {
            xTaskNotifyWait(0xffffffff, 0, 0, portMAX_DELAY);
            //uart_enable_rx_intr(UART_NUM_0);
           // printf("%s\n", "get notificaation");
             //uint8_t buff[1];
             //uint8_t peekbuff[256];    
            //printf("%s\n", "gsdg");
           /*  int8_t line = uart_read_bytes(UART_NUM_1, buff, 1, 20 / portTICK_RATE_MS);
            len = uart_get_buffered_data_len(UART_NUM_1, &len);
            uart_write_bytes(UART_NUM_1, (char *)buff, 1);
 */
                while (true)
                {
                    //char *line;
                    char *line = linenoise(prompt);
                    if (line == NULL)
                    {
                        printf("%s\n", "line is null");
                        break;
                    }
                    
                    //printf("%s\n", line);
                    //uart_write_bytes(UART_NUM_1, (char *)buff, line);
                     esp_console_run(line, &ret);
                    linenoiseFree(line);
                    vTaskDelay(100/portTICK_PERIOD_MS);
                }
            //uart_enable_rx_intr(UART_NUM_0);
            xSemaphoreGive(mutexInput); 
        }
    }
   
}

/* void cmd_instance_task(void *pvParams){
    while (true){
       // if(xSemaphoreTake(mutexInput, portMAX_DELAY)) {
            xTaskNotifyWait(0xffffffff, 0, 0, portMAX_DELAY);
            //uart_enable_rx_intr(UART_NUM_0);
           // printf("%s\n", "get notificaation");
             //uint8_t *buff = (uint8_t *) malloc(1);
             uint8_t buff[1];
             uint8_t peekbuff[256];           
            //while (ret != 24)
           // {
                //char *line;
                //char *line = linenoise(prompt);
                int8_t line = uart_read_bytes(UART_NUM_1, buff, 1, 20 / portTICK_RATE_MS);
                printf("\n buff %u \n", buff[0]);
                xQueueSendToBack(inputQueue, buff, 20 / portTICK_RATE_MS);

               // printf("%s\n", line);
               printf("%u\n", uxQueueMessagesWaiting(inputQueue));
               for (int i = 0; i < uxQueueMessagesWaiting(inputQueue); i++){
                    xQueueReceive(inputQueue, &peekbuff, 20 / portTICK_RATE_MS);
                    uart_write_bytes(UART_NUM_1, peekbuff, i);
               } 

               



                 //free(buff);

               // esp_console_run(line, &ret);
                //linenoiseFree(line);
                //vTaskDelay(100/portTICK_PERIOD_MS);
            //}
            //uart_enable_rx_intr(UART_NUM_0);
         //   xSemaphoreGive(mutexInput); 
        //}
    }
   
} */


void app_main(void)
{

     /*    _GLOBAL_REENT->_stdin = fopen("/dev/uart/1", "r");
    _GLOBAL_REENT->_stdout = fopen("/dev/uart/1", "w");   */


    esp_console_config_t console_conf = {
        .max_cmdline_length = 256,
        .max_cmdline_args = 5,
        .hint_color = 37,
        .hint_bold = 1,
    };

 /*         esp_console_dev_uart_config_t repl_uart_conf = {
            .channel = UART_NUM_1,
            .baud_rate = 115200,
            .tx_gpio_num = 17,
            .rx_gpio_num = 16,
        }; 

         esp_console_repl_config_t repl_conf = {
            .max_history_len = 10,
            .history_save_path = NULL,
            .task_stack_size = 2048,
            .task_priority = 1,
        }; 
 */

    fflush(stdout);
    fsync(fileno(stdout)); 

    setvbuf(stdin, NULL, _IONBF, 0); 

    esp_console_init(&console_conf);

    esp_console_cmd_t esp_comm = {
        .command = "fuck",
        .help = "fucking help",
        .hint = "fucking hint",
        .func = &cmd_handle,
        .argtable = NULL,
    };

    esp_console_cmd_t cmd_exit_conf = {
        .command = "exit",
        .help = "exit help",
        .hint = "exit hint",
        .func = &cmd_exit,
        .argtable = NULL,
    };

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,

    };


       ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));
       uart_set_pin(UART_NUM_0, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    ESP_ERROR_CHECK(
        uart_driver_install(UART_NUM_0, 1024, 0, 20, &uart0_queue, 0)); 
       
   uart_enable_pattern_det_baud_intr(UART_NUM_0, '+', 3, 9, 0, 0);

    //uart_isr_register(UART_NUM_0, &uart_interrupt_handler, 0, 0, NULL);

  
   // esp_vfs_dev_uart_register();

    esp_vfs_dev_uart_use_driver(UART_NUM_0);

    /* Minicom, screen, idf_monitor send CR when ENTER key is pressed */
     esp_vfs_dev_uart_port_set_rx_line_endings(UART_NUM_0,
     ESP_LINE_ENDINGS_CR);
    /* Move the caret to the beginning of the next line on '\n' */
    esp_vfs_dev_uart_port_set_tx_line_endings(UART_NUM_0,
                                              ESP_LINE_ENDINGS_CRLF); 

    

    /*    int probe_status = linenoiseProbe();
        if (probe_status) {
            printf("\n"
                   "Your terminal application does not support escape
       sequences.\n" "Line editing and history features are disabled.\n" "On
       Windows, try using Putty instead.\n"); linenoiseSetDumbMode(1);
        }
     */
    esp_console_cmd_register(&esp_comm);
    esp_console_cmd_register(&cmd_exit_conf);

    linenoiseSetMultiLine(1);
    linenoiseAllowEmpty(false);
/* 
    int probe_status = linenoiseProbe();
    if (probe_status) { 
        printf("\n"
               "Your terminal application does not support escape sequences.\n"
               "Line editing and history features are disabled.\n"
               "On Windows, try using Putty instead.\n");
        linenoiseSetDumbMode(1);
    }
 */

  /*   while (true)
    {
        char* line = linenoise(prompt);
        if (line == NULL)
        {
            printf("%s\n", "line is null");
            break;
        }

        //  printf("%s\n", "osdofsdof");
        printf("\n%s\n", line);
        esp_console_run(line, &ret);
        linenoiseFree(line);
        // vTaskDelay(200/portTICK_PERIOD_MS);
    } */

   // ESP_ERROR_CHECK(esp_console_new_repl_uart(&repl_uart_conf, &repl_conf, repl));

     //ESP_ERROR_CHECK(esp_console_start_repl(&repl));
     //_GLOBAL_REENT->_stdin = fopen("/dev/uart/1", "r");
     //_GLOBAL_REENT->_stdout = fopen("/dev/uart/1", "w");
   

    inputQueue = xQueueCreate(256, sizeof(int8_t));
    mutexInput = xSemaphoreCreateMutex();
    xSemaphoreGive(mutexInput);
    xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 2, NULL);
    xTaskCreate(cmd_instance_task, "cmd_instance_task", 2048, NULL, 1, &send_data_to_oled);


}
