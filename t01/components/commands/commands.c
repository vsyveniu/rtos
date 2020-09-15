#include "commands.h"

int cmd_handle(int argc, char** argv)
{
    printf("%s\n", "command handle fuck");

    gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_1, 1);

    return 42;
}

int cmd_exit()
{
    printf("%s\n", "console exit");

    //ESP_ERROR_CHECK(esp_console_deinit());

    return 24;
}