#include "main.h"
#include "defines.h"
#include "driver/periph_ctrl.h"
#include "soc/timer_group_struct.h"

void app_main(void)
{
    esp_err_t err;

    err = uart_console_init();
    if(err != ESP_OK){
        printf("%s\n", "couldn't init uart console. Have to reboot");
        esp_restart();
    }
   
    esp_console_cmd_t cmd_led_on_conf = {
        .command = "ledon",
        .func = &cmd_ledon,
    };

     esp_console_cmd_t cmd_led_off_conf = {
        .command = "ledoff",
        .func = &cmd_ledoff,
    };

    esp_console_cmd_t cmd_exit_conf = {
        .command = "exit",
        .func = &cmd_exit,
    };

    esp_console_cmd_register(&cmd_led_on_conf);
    esp_console_cmd_register(&cmd_led_off_conf);
    esp_console_cmd_register(&cmd_exit_conf);

}
