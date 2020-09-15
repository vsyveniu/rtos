#include "main.h"
#include "defines.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"
#include "esp_vfs.h"
#include "esp_vfs_dev.h"

static QueueHandle_t uart0_queue;
xSemaphoreHandle mutexInput;
static TaskHandle_t send_data_to_oled = NULL;

void uart_event_task(void *pvParams){

    uart_event_t event;

    while (true){
        if(xQueueReceive(uart0_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
            //printf("%s\n", "queue");
            if(xSemaphoreTake(mutexInput, portMAX_DELAY))
            {
              //  printf("queueu + sem");
                
                 
              //  printf("%s\n", "uart event task queue");
             //xTaskNotify(send_data_to_oled, 0, eSetValueWithOverwrite);
              //uart_disable_rx_intr(UART_NUM_0);
              xSemaphoreGive(mutexInput);
            }
        }
    }
   
}


void cmd_instance_task(void *pvParams){

    uart_event_t event;
       char* prompt = "> ";
       int ret;

    while (true){
        if(xSemaphoreTake(mutexInput, portMAX_DELAY)) {
           // printf("%s\n", "instance");
            //xTaskNotifyWait(0xffffffff, 0, 0, portMAX_DELAY);
            //uart_enable_rx_intr(UART_NUM_0);
           // printf("%s\n", "get notificaation");
            ret = 0;            
            while (ret != 24)
            {
                //char *line;
                char *line = linenoise(prompt);
                //uart_read_bytes(UART_NUM_1, &line, 1024, 0);
                
                if (line == NULL)
                {
                    printf("%s\n", "line is null");
                   break;
                }
                printf("\n%s\n", line);
                //uart_write_bytes(UART_NUM_1, line, strlen(line));
                esp_console_run(line, &ret);
                linenoiseFree(line);
                //vTaskDelay(100/portTICK_PERIOD_MS);
            }
            //uart_enable_rx_intr(UART_NUM_0);
            xSemaphoreGive(mutexInput); 
        }
    }
   
}


void app_main(void)
{
    esp_console_config_t console_conf = {
        .max_cmdline_length = 256,
        .max_cmdline_args = 5,
        .hint_color = 37,
        .hint_bold = 1,
    };

         esp_console_dev_uart_config_t repl_uart_conf = {
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


/*     fflush(stdout);
    fsync(fileno(stdout));

    setvbuf(stdin, NULL, _IONBF, 0); */

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

/*       ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config));
       uart_set_pin(UART_NUM_1, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    ESP_ERROR_CHECK(
        uart_driver_install(UART_NUM_1, 1024, 0, 20, &uart0_queue, 0)); */
       
  

    //uart_isr_register(UART_NUM_0, &uart_interrupt_handler, 0, 0, NULL);

    //uart_enable_rx_intr(UART_NUM_0);
    //uart_disable_tx_intr(UART_NUM_0);

    //esp_vfs_dev_uart_use_driver(UART_NUM_1);

    /* Minicom, screen, idf_monitor send CR when ENTER key is pressed */
    // esp_vfs_dev_uart_port_set_rx_line_endings(UART_NUM_0,
    // ESP_LINE_ENDINGS_CR);
    /* Move the caret to the beginning of the next line on '\n' */
/*     esp_vfs_dev_uart_port_set_tx_line_endings(UART_NUM_1,
                                              ESP_LINE_ENDINGS_CRLF); */

    

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

    int probe_status = linenoiseProbe();
    if (probe_status) { 
        printf("\n"
               "Your terminal application does not support escape sequences.\n"
               "Line editing and history features are disabled.\n"
               "On Windows, try using Putty instead.\n");
        linenoiseSetDumbMode(1);
    }



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
    esp_console_repl_t repl;

    ESP_ERROR_CHECK(esp_console_new_repl_uart(&repl_uart_conf, &repl_conf, repl));

     ESP_ERROR_CHECK(esp_console_start_repl(&repl));
 
     mutexInput = xSemaphoreCreateMutex();
    xSemaphoreGive(mutexInput);
    //xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 1, NULL);
    xTaskCreate(cmd_instance_task, "cmd_instance_task", 2048, NULL, 1, &send_data_to_oled);
}
