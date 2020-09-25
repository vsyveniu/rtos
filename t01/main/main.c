#include "main.h"
#include "defines.h"
#include "argtable3/argtable3.h"
#include "esp_vfs.h"
#include "esp_vfs_dev.h"
#include "esp_vfs_fat.h"
#include "esp_log.h"

static QueueHandle_t uart0_queue;
xSemaphoreHandle mutexInput;
struct arg_lit *f;
struct arg_str *on, *off, *pulse;
struct arg_int *scal;
struct arg_end *end;

void uart_event_task(void *pvParams){

    uart_event_t event;
    static BaseType_t xHigherPriorityTaskWoken;
    //static int is_first_launch = 1;

    while (true){
        if(xQueueReceive(uart0_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
              vTaskResume(pvParams);
              xSemaphoreGiveFromISR(mutexInput, &xHigherPriorityTaskWoken);
        }
    }
   
}



/* void uart_print_str(int uart_num, char *str){
    uart_write_bytes(uart_num, str, strlen(str));
} */

 void cmd_instance_task(void *pvParams){
     char *prompt = "> ";
     
                     uint8_t buff[256];
             memset(buff, 0, 256);
            char str[256];
             //str = (char *)malloc(sizeof(char) * 256);
             memset(str, '\0', 256);
               int i = 0;
             int j = 0;
             int ret = 0;
             size_t len = 0;
             int is_first = 1;

    while (true){
        if(xSemaphoreTake(mutexInput, portMAX_DELAY)) {
            if (is_first)
            {
              uart_write_bytes(UART_NUMBER, prompt, strlen(prompt));
               is_first = 0;     
            }
          //  
            //xTaskNotifyWait(0xffffffff, 0, 0, portMAX_DELAY);
            //uart_enable_rx_intr(UART_NUM_0);
            printf("%s\n", "start loop");
             //uint8_t *buff = (uint8_t *) malloc(1);
            // uint8_t buff[256];


           
             int rxlen;
             ret = 0;      
            //while (true)
            //{
                rxlen = uart_read_bytes(UART_NUM_1, buff, 256, 20 / portTICK_RATE_MS);
               // len = uart_get_buffered_data_len(UART_NUMBER, &len);
                     if((rxlen == 3 || rxlen == 6) && buff[0] == 27){
                        printf("%s\n", "fuck");
                        printf("%d\n", rxlen);
                        printf("%d\n", buff[0]);
                        printf("%d\n", buff[1]);
                        printf("%d\n", buff[2]);

                         printf("\n%d rx_buff len\n", rxlen);
                        printf("\n%d len\n", len); 
                      if(buff[2] == 68 && i > 0)
                        { 
                            uart_write_bytes(UART_NUM_1, buff, 3);
                            if(i > 0)
                                i--;
                            printf("\n%d i\n", i); 
                        }
                        if(buff[2] == 67 && i < strlen(str))
                        { 
                            uart_write_bytes(UART_NUM_1, buff, 3);
                            i++;
                            printf("\n%d i\n", i); 
                        }
                        /* if(buff[2] == 65 || buff[2] == 66)
                        { 
                            uart_write_bytes(UART_NUM_1, buff, 3);
                        } */
                    }
                    else if(buff[0] == 13){
                        printf("not parsed %s\n", str);
                      /*    char **argv;
                        int argc = 0;
                        argc = esp_console_split_argv(str, argv, 5);
                        printf(" argc %d\n", argc);
                        while (argc > 0){
                            printf("argvs   %s \n", *argv);
                            argc--;
                            argv++;
                        }*/
                        printf("parsed %s\n", str); 
                        esp_console_run(str, &ret);
                        printf("ret after command %d\n", ret);
                        memset(str, '\0', 256);
                        memset(buff, 0, 256);
                        i = 0;
            
                       // free(str);
                        uart_write_bytes(UART_NUMBER, "\n\r", 2);
                        uart_write_bytes(UART_NUMBER, prompt, strlen(prompt));
                        if(ret == 24)
                        {
                            is_first = 1;
                            uart_print_str(UART_NUMBER, "\n\rType any shit to enter a REPL again\n\r");
                            vTaskSuspend( NULL );
                        }  
                    }
                    else if (rxlen > 0){ 
                      /*   if( buff[0] == 127)
                        {
                            uart_write_bytes(UART_NUMBER, buff, 3);
                            //uart_write_bytes(UART_NUMBER, "\r", 1);
                           // uart_write_bytes(UART_NUMBER, str, 256);
                            i--;
                            str[i] = 0;
                            str[i - 1] = 0;
                            printf("\n%d rtrtrtbi\n", i);
                            
                        } */
                        if(buff[0] != 127)
                        {
                            j = 0;
                            if(rxlen + strlen(str) > 255 && strlen(str) < 255)
                            {
                                printf("%s\n", "bitch");
                                uart_write_bytes(UART_NUMBER, buff, 255 - strlen(str));
                                while(i < 255)
                                {
                                    str[i] = buff[j];
                                    i++;
                                    j++;
                                }
                                
                            }
                            else if(i < 255)
                            {
                                printf("%s\n", "BITCH");
                                uart_write_bytes(UART_NUMBER, buff, rxlen);
                                while (j < rxlen)
                                {
                                    str[i] = buff[j];
                                    i++;
                                    j++;
                                  
                                }
                            }
                        }
                    }
                    memset(buff, 0, 256);
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
   
} 

void app_main(void)
{
    esp_console_config_t console_conf = {
        .max_cmdline_length = 256,
        .max_cmdline_args = 12,
        .hint_color = 37,
        .hint_bold = 1,
    };

    esp_console_init(&console_conf);

    esp_console_cmd_t esp_comm = {
        .command = "led",
        .help = "fucking help",
        .hint = "fucking hint",
        .func = &cmd_handle,
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

    ESP_ERROR_CHECK(uart_param_config(UART_NUMBER, &uart_config));
       uart_set_pin(UART_NUMBER, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    ESP_ERROR_CHECK(
        uart_driver_install(UART_NUMBER, 1024, 0, 20, &uart0_queue, 0)); 

    esp_console_cmd_register(&esp_comm);
    esp_console_cmd_register(&cmd_exit_conf);

    TaskHandle_t xcmdHandle = NULL;
    inputQueue = xQueueCreate(256, sizeof(int8_t));
    mutexInput = xSemaphoreCreateBinary();
    xTaskCreate(cmd_instance_task, "cmd_instance_task", 4096, NULL, 2, &xcmdHandle);
    xTaskCreate(uart_event_task, "uart_event_task", 2048, xcmdHandle, 2, &send_data_to_oled);
    uart_print_str(UART_NUMBER, "\n\rType any shit to enter a REPL \n\r");
}
