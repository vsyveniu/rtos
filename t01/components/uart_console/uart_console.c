#include "uart_console.h"

static QueueHandle_t uart0_queue;
xSemaphoreHandle mutexInput;

void uart_event_task(void *pvParams){

    uart_event_t event;
    static BaseType_t xHigherPriorityTaskWoken;
    while (true){
        if(xQueueReceive(uart0_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
              vTaskResume(pvParams);
              xSemaphoreGiveFromISR(mutexInput, &xHigherPriorityTaskWoken);
        }
    }
}

 void cmd_instance_task(void *pvParams){
    char *prompt = "> ";
    uint8_t buff[256];
    memset(buff, 0, 256);
    char str[256];
    memset(str, '\0', 256);
    int i = 0;
    int j = 0;
    int ret = 0;
    int is_first = 1;
    esp_err_t console_ret = 0;

    while (true){
        if(xSemaphoreTake(mutexInput, portMAX_DELAY))
        {
            if (is_first)
            {
              uart_write_bytes(UART_NUMBER, prompt, strlen(prompt));
               is_first = 0;     
            }
            int rxlen;
            ret = 0;
            console_ret = 0;      
            rxlen = uart_read_bytes(UART_NUM_1, buff, 256, 20 / portTICK_RATE_MS);

            if((rxlen == 3 || rxlen == 6) && buff[0] == 27)
            {
              if(buff[2] == 68 && i > 0)
              { 
                uart_write_bytes(UART_NUM_1, buff, 3);
                if(i > 0)
                {
                  i--;
                }
              }
              if(buff[2] == 67 && i < strlen(str))
              { 
                uart_write_bytes(UART_NUM_1, buff, 3);
                i++;
              }
            }
            else if(buff[0] == 13)
            {
              console_ret = esp_console_run(str, &ret);
              if(console_ret == ESP_ERR_INVALID_ARG){
                uart_print_str(UART_NUMBER, "\n\rBitch please! What am i supposed to do?");
              }
              if(console_ret == ESP_ERR_NOT_FOUND){
                uart_print_str(UART_NUMBER, "\n\rCommand not found. Try to type not such a crap next time");
              }
              memset(str, '\0', 256);
              memset(buff, 0, 256);
              i = 0;
              uart_write_bytes(UART_NUMBER, "\n\r", 2);
              uart_write_bytes(UART_NUMBER, prompt, strlen(prompt));
              if(ret == 24)
              {
                is_first = 1;
                uart_print_str(UART_NUMBER, "\n\rType any shit to enter a REPL again\n\r");
                vTaskSuspend( NULL );
              }
            }
         
            else if (rxlen > 0)
            { 
                /*   if( buff[0] == 127)
                {
                  uart_write_bytes(UART_NUMBER, buff, 3);
                  //uart_write_bytes(UART_NUMBER, "\r", 1);
                  // uart_write_bytes(UART_NUMBER, str, 256);
                  i--;
                  str[i] = 0;
                  str[i - 1] = 0;
                } */
              if(buff[0] != 127)
              {
                j = 0;
                if(rxlen + strlen(str) > 255 && strlen(str) < 255)
                {
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

void uart_console_init()
{
   esp_console_config_t console_conf = {
        .max_cmdline_length = 256,
        .max_cmdline_args = 12,
        .hint_color = 37,
        .hint_bold = 1,
    };

    esp_console_init(&console_conf);

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

    TaskHandle_t xcmdHandle = NULL;
    mutexInput = xSemaphoreCreateBinary();
    xTaskCreate(cmd_instance_task, "cmd_instance_task", 4096, NULL, 2, &xcmdHandle);
    xTaskCreate(uart_event_task, "uart_event_task", 2048, xcmdHandle, 2, NULL);
    uart_print_str(UART_NUMBER, "\n\rType any shit to enter a REPL \n\r");
}
