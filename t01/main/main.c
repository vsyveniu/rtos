#include "main.h"
#include "defines.h"

void app_main(void)
{
    uart_console_init();

    esp_console_cmd_t esp_comm = {
        .command = "led",
        .func = &cmd_handle,
    };

    esp_console_cmd_t cmd_exit_conf = {
        .command = "exit",
        .func = &cmd_exit,
    };

    esp_console_cmd_register(&esp_comm);
    esp_console_cmd_register(&cmd_exit_conf);
}
