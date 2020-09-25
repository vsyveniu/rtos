#include "main.h"
#include "defines.h"
#include "esp_log.h"

void app_main(void)
{
    uart_console_init();
   
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

    esp_console_cmd_register(&esp_comm);
    esp_console_cmd_register(&cmd_exit_conf);
}
