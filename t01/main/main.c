#include "main.h"
#include "defines.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"
#include "esp_vfs.h"
#include "esp_vfs_dev.h"


static int cmd_handle(int argc, char **argv){

    printf("%s\n", "handle");

    return 0;
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
        .channel = UART_NUM_0,
        .baud_rate = 115200,
        .tx_gpio_num = -1,
        .rx_gpio_num = -1,
    };

    esp_console_repl_config_t repl_conf = {
        .max_history_len = 10,
        .history_save_path = NULL,
        .task_stack_size = 2048,
        .task_priority = 1,
    };

    esp_console_repl_t *repl = NULL;

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

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,

    };
  

    ESP_ERROR_CHECK( uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0) );
    ESP_ERROR_CHECK( uart_param_config(UART_NUM_0, &uart_config) );

    esp_vfs_dev_uart_use_driver(UART_NUM_0);


     /* Minicom, screen, idf_monitor send CR when ENTER key is pressed */
    //esp_vfs_dev_uart_port_set_rx_line_endings(UART_NUM_0, ESP_LINE_ENDINGS_CR);
    /* Move the caret to the beginning of the next line on '\n' */
    esp_vfs_dev_uart_port_set_tx_line_endings(UART_NUM_0, ESP_LINE_ENDINGS_CRLF);

   

   // int ret;

     
/*    int probe_status = linenoiseProbe();
    if (probe_status) {
        printf("\n"
               "Your terminal application does not support escape sequences.\n"
               "Line editing and history features are disabled.\n"
               "On Windows, try using Putty instead.\n");
        linenoiseSetDumbMode(1);
    }
 */
    esp_console_cmd_register(&esp_comm);

   linenoiseSetMultiLine(1);
    linenoiseAllowEmpty(false);
    char *prompt = "> ";


    while (true)
    {
        char* line = linenoise(prompt);
             if (line == NULL) {
            printf("%s\n", "line is null");
                break;
            }
 
      //  printf("%s\n", "osdofsdof");
        printf("\n%s\n", line);
        // esp_console_run(line, &ret);
         linenoiseFree(line);
         //vTaskDelay(200/portTICK_PERIOD_MS);
    }

   

    //esp_console_new_repl_uart(&repl_uart_conf, &repl_conf, &repl);

   // esp_console_start_repl(repl);

   

    

}
