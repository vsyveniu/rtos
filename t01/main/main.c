#include "main.h"
#include "defines.h"
#include "argtable3/argtable3.h"
#include "esp_vfs.h"
#include "esp_vfs_dev.h"
#include "esp_vfs_fat.h"
#include "esp_log.h"

xQueueHandle inputQueue;
static QueueHandle_t uart0_queue;
xSemaphoreHandle mutexInput;
xSemaphoreHandle mutexCrutch;
static TaskHandle_t send_data_to_oled = NULL;


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

 void cmd_instance_task(void *pvParams){
     char *prompt = "> ";

    while (true){
        if(xSemaphoreTake(mutexInput, portMAX_DELAY)) {
            uart_write_bytes(UART_NUMBER, prompt, strlen(prompt));
            //xTaskNotifyWait(0xffffffff, 0, 0, portMAX_DELAY);
            //uart_enable_rx_intr(UART_NUM_0);
            printf("%s\n", "get notificaation");
             //uint8_t *buff = (uint8_t *) malloc(1);
            // uint8_t buff[256];
             uint8_t buff[3];
             memset(buff, 0, 3);
             char *str;
             str = (char *)malloc(sizeof(char) * 256);
             memset(str, 0, 256);
             int i = 0;
             int ret = 0;
             size_t len = 0;
             int rxlen;      
            while (true)
            {
                rxlen = uart_read_bytes(UART_NUM_1, buff, 3, 20 / portTICK_RATE_MS);
                len = uart_get_buffered_data_len(UART_NUMBER, &len);

                    if(buff[0] == 97){
                        vTaskSuspend( NULL );
                        break;    
                    }
                    else if(rxlen == 3){
                        printf("%s\n", "fuck");
                        printf("%d\n", rxlen);
                        printf("%d\n", buff[0]);
                        printf("%d\n", buff[1]);
                        printf("%d\n", buff[2]);
                    /*  if(buff[2] == 68 || buff[2] == 67)
                        { */
                            uart_write_bytes(UART_NUM_1, buff, 3);
                            i--;
                    // }
                    }
                    else if(buff[0] == 13){
                        printf("%s\n", str);
                        esp_console_run(str, &ret);
                        memset(str, 0, 256);
                        free(str);
                        uart_write_bytes(UART_NUMBER, "\n\r", 2);
                        break;     
                    }
                    else if (rxlen > 0){ 
                        if( buff[0] == 127 && i > 0)
                        {
                            uart_write_bytes(UART_NUM_1, buff, 1);
                            str[i] = 0;
                            str[i - 1] = 0;
                            i--;
                            printf("\n%d i\n", i);
                        }
                        else if(buff[0] != 127)
                        {
                            uart_write_bytes(UART_NUM_1, buff, 1);
                            str[i] = buff[0];
                            i++;
                        }
                        

                        printf("%d\n", buff[0]);
                        printf("\n%d rx_buff len\n", rxlen);
                        printf("\n%d rx_len\n", len);
                        
                        if(ret == 24)
                        {
                            printf("%s\n", "Type any shit to enter a REPL again");
                            //free(buff);
                            vTaskSuspend( NULL );
                            break;
                        }
                    } 

                vTaskDelay(20 / portTICK_PERIOD_MS);
            

            }
            //uart_enable_rx_intr(UART_NUM_0);
         //   xSemaphoreGive(mutexInput); 
        }
    }
   
} 


void app_main(void)
{
 
    esp_console_config_t console_conf = {
        .max_cmdline_length = 42,
        .max_cmdline_args = 5,
        .hint_color = 37,
        .hint_bold = 1,
    };


    esp_console_init(&console_conf);

    esp_console_cmd_t esp_comm = {
        .command = "fuck",
        .help = "fucking help",
        .hint = "fucking hint",
        .func = &cmd_handle,
        .argtable = "arg_int",
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
       
    esp_vfs_dev_uart_use_driver(UART_NUMBER);

    /* Minicom, screen, idf_monitor send CR when ENTER key is pressed */
     esp_vfs_dev_uart_port_set_rx_line_endings(UART_NUMBER,
     ESP_LINE_ENDINGS_CR);
    /* Move the caret to the beginning of the next line on '\n' */
    esp_vfs_dev_uart_port_set_tx_line_endings(UART_NUMBER,
                                              ESP_LINE_ENDINGS_CRLF); 
    
 /* 
    int probe_status = linenoiseProbe();
        if (probe_status) {
            linenoiseSetDumbMode(1);
        } 
 */
    esp_console_cmd_register(&esp_comm);
    esp_console_cmd_register(&cmd_exit_conf);


   // ESP_ERROR_CHECK(esp_console_new_repl_uart(&repl_uart_conf, &repl_conf, repl));

     //ESP_ERROR_CHECK(esp_console_start_repl(&repl));
     

    TaskHandle_t xcmdHandle = NULL;
    inputQueue = xQueueCreate(256, sizeof(int8_t));
    mutexInput = xSemaphoreCreateBinary();
    mutexCrutch = xSemaphoreCreateMutex();
    xTaskCreate(cmd_instance_task, "cmd_instance_task", 2048, NULL, 1, &xcmdHandle);
    xTaskCreate(uart_event_task, "uart_event_task", 2048, xcmdHandle, 1, &send_data_to_oled); 
}
