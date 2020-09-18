#include "main.h"
#include "defines.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"
#include "esp_vfs.h"
#include "esp_vfs_dev.h"
#include "esp_vfs_fat.h"
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#define MOUNT_PATH "/data"
#define HISTORY_PATH MOUNT_PATH "/history.txt"


xQueueHandle inputQueue;
static QueueHandle_t uart0_queue;
xSemaphoreHandle mutexInput;
xSemaphoreHandle mutexCrutch;
static TaskHandle_t send_data_to_oled = NULL;

static void initialize_filesystem(void)
{
    static wl_handle_t wl_handle;
    const esp_vfs_fat_mount_config_t mount_config = {
            .max_files = 4,
            .format_if_mount_failed = true
    };
    esp_err_t err = esp_vfs_fat_spiflash_mount(MOUNT_PATH, "storage", &mount_config, &wl_handle);
    if (err != ESP_OK) {
            printf("%s\n", "fat is fucked");
        return;
    }
}

static void initialize_nvs(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}


void cmd_instance_task(void *pvParams)
{

    char* prompt = "> ";
    int ret;
    int crutch = 1;

    while (true){
        if(xSemaphoreTake( mutexInput, 20 / portTICK_RATE_MS) == pdTRUE) {
                if(crutch)
                {
                    //linenoiseClearScreen();
                    crutch = 0;
                }
               ret = 0;
                while (true)
                {
                    char *line = linenoise(prompt);
                    if (line == NULL)
                    {
                        printf("%s\n", "Are you dumb? What am i supposed to do?");
                        break;
                    }
                   /*  if (strlen(line) > 0) {
                        linenoiseHistoryAdd(line);
                    } */
                    //esp_console_split_argv(line, **argv, 3);
                    esp_console_run(line, &ret);
                    if(ret == 24)
                    {
                        printf("%s\n", "Type any shit to enter a REPL again");
                        linenoiseFree(line);
                        vTaskSuspend( NULL );
                        break;
                    }
                    //linenoiseHistorySave(HISTORY_PATH);
                    linenoiseFree(line);
                }
                crutch = 1;
        }
    }
   
}


void uart_event_task(void *pvParams){

    uart_event_t event;
    static BaseType_t xHigherPriorityTaskWoken;
    static int is_first_launch = 1;

    while (true){
        if(xQueueReceive(uart0_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
              vTaskResume(pvParams);
              xSemaphoreGiveFromISR(mutexInput, &xHigherPriorityTaskWoken);
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
    //initialize_nvs();
    //initialize_filesystem();
    //_GLOBAL_REENT->_stdin = fopen("/dev/uart/1", "r");
     //_GLOBAL_REENT->_stdout = fopen("/dev/uart/1", "w");

    fflush(stdout);
    fsync(fileno(stdout));

    setvbuf(stdin, NULL, _IONBF, 0); 

   // vTaskDelay(5000 / portTICK_PERIOD_MS);

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

    //linenoiseClearScreen();
  //  linenoiseSetMultiLine(1);
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

          printf("%s\n", "osdofsdof");
        printf("\n%s\n", line);
        esp_console_run(line, &ret);
        linenoiseFree(line);
        // vTaskDelay(200/portTICK_PERIOD_MS);
    } */

   // ESP_ERROR_CHECK(esp_console_new_repl_uart(&repl_uart_conf, &repl_conf, repl));

     //ESP_ERROR_CHECK(esp_console_start_repl(&repl));
     

    TaskHandle_t xcmdHandle = NULL;
    inputQueue = xQueueCreate(256, sizeof(int8_t));
    mutexInput = xSemaphoreCreateBinary();
    mutexCrutch = xSemaphoreCreateMutex();
    xTaskCreate(cmd_instance_task, "cmd_instance_task", 2048, NULL, 1, &xcmdHandle);
    xTaskCreate(uart_event_task, "uart_event_task", 2048, xcmdHandle, 1, &send_data_to_oled); 
    printf("%s\n", "Type any shit to enter a REPL"); 
}
