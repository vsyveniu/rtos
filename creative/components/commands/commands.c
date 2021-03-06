#include "commands.h"
#include "argtable3/argtable3.h"
#include "driver/i2s.h"

struct arg_lit* first = NULL;
struct arg_lit* second = NULL;
struct arg_lit* third = NULL;
struct arg_lit* all = NULL;
struct arg_rex* val = NULL;
struct arg_rex* alarm_set = NULL;
struct arg_end* end = NULL;

void register_cmnd_set()
{
    esp_console_cmd_t cmd_led_on_conf = {
        .command = "led-on",
        .func = &cmd_led_on,
    };
    esp_console_cmd_t cmd_led_off_conf = {
        .command = "led-off",
        .func = &cmd_led_off,
    };

    esp_console_cmd_t cmd_noise_on_conf = {
        .command = "noise-on",
        .func = &cmd_noise_on,
    };
    esp_console_cmd_t cmd_noise_off_conf = {
        .command = "noise-off",
        .func = &cmd_noise_off,
    };

    esp_console_cmd_t cmd_show_wheather_conf = {
        .command = "wheather",
        .func = &cmd_show_wheather,
    };

    esp_console_cmd_t cmd_set_time_conf = {
        .command = "time",
        .func = &cmd_time,
    };

    esp_console_cmd_t cmd_set_alarm_conf = {
        .command = "alarm",
        .func = &cmd_alarm,
    };

    esp_console_cmd_t cmd_help_conf = {
        .command = "help",
        .func = &cmd_help,
    };

    esp_console_cmd_t cmd_exit_conf = {
        .command = "exit",
        .func = &cmd_exit,
    };

    esp_console_cmd_register(&cmd_led_on_conf);
    esp_console_cmd_register(&cmd_led_off_conf);
    esp_console_cmd_register(&cmd_show_wheather_conf);
    esp_console_cmd_register(&cmd_set_time_conf);
    esp_console_cmd_register(&cmd_set_alarm_conf);
    esp_console_cmd_register(&cmd_noise_on_conf);
    esp_console_cmd_register(&cmd_noise_off_conf);
    esp_console_cmd_register(&cmd_help_conf);
    esp_console_cmd_register(&cmd_exit_conf);
}

int cmd_led_on(int argc, char** argv)
{
    int8_t nerrors = 0;

    void* argtable[] = {
        first = arg_litn("1", NULL, 0, 1, "the -1 option"),
        second = arg_litn("2", NULL, 0, 1, "the -2 option"),
        third = arg_litn("3", NULL, 0, 1, "the -3 option"),
        all = arg_litn("a", "all", 0, 1, "the -all option"),
        end = arg_end(20),
    };

    nerrors = arg_parse(argc, argv, argtable);

    if (nerrors > 0)
    {
        uart_print_str(UART_NUMBER, "\n\rarguments line error\n");
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 0;
    }
    if (!first->count && !second->count && !third->count && !all->count)
    {
        uart_print_str(UART_NUMBER, "\n\rYou have to specify arguments!");
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 0;
    }

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));

    handle_led_on(first, second, third, all);
    return 0;
}

int cmd_led_off(int argc, char** argv)
{
    int8_t nerrors = 0;

    void* argtable[] = {
        first = arg_litn("1", NULL, 0, 1, "the -1 option"),
        second = arg_litn("2", NULL, 0, 1, "the -2 option"),
        third = arg_litn("3", NULL, 0, 1, "the -3 option"),
        all = arg_litn("a", "all", 0, 1, "the -all option"),
        end = arg_end(20),
    };

    nerrors = arg_parse(argc, argv, argtable);

    if (nerrors > 0)
    {
        uart_print_str(UART_NUMBER, "\n\rarguments line error");
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 0;
    }
    if (!first->count && !second->count && !third->count && !all->count)
    {
        uart_print_str(UART_NUMBER, "\n\rYou have to specify arguments!");
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 0;
    }

    handle_led_off(first, second, third, all);
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return 0;
}

int cmd_show_wheather(int argc, char** argv)
{
    if (argc > 1)
    {
        uart_print_str(UART_NUMBER, "\n\rType command without options");

        return 0;
    }
    dht_data_s dht_data_peek[1];

    if (xSemaphoreTake(dht_peek_mutex, portMAX_DELAY))
    {
        xQueuePeek(dht_queue, &dht_data_peek, 10);

        char dht_str[35];
        memset(dht_str, 0, 35);

        sprintf(dht_str, "Temperature %dC  Humidity %u%%",
                dht_data_peek->temperature, dht_data_peek->humidity);

        uart_print_str(UART_NUMBER, "\n\r");
        uart_print_str(UART_NUMBER, dht_str);

        xSemaphoreGive(dht_peek_mutex);
    }

    return 0;
}

int cmd_time(int argc, char** argv)
{
    int nerrors = 0;

    void* argtable[] = {
        val = arg_rex1("val", "value",
                       "(?:[01]\\d|2[0123]):(?:[012345]\\d):(?:[012345]\\d)",
                       "<n>", 0, "the regular expression option"),
        end = arg_end(20),
    };

    nerrors = arg_parse(argc, argv, argtable);
    if (nerrors > 0)
    {
        uart_print_str(UART_NUMBER, "\n\rarguments line error");
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 0;
    }

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));

    handle_time(val);

    return 0;
}

int cmd_alarm(int argc, char** argv)
{
    int nerrors = 0;

    void* argtable[] = {
        alarm_set =
            arg_rex1("val", "value",
                     "(?:[01]\\d|2[0123]):(?:[012345]\\d):(?:[012345]\\d)",
                     "<n>", 0, "the regular expression option"),
        end = arg_end(20),
    };

    nerrors = arg_parse(argc, argv, argtable);
    if (nerrors > 0)
    {
        uart_print_str(UART_NUMBER, "\n\rarguments line error");
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 0;
    }

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    handle_alarm(alarm_set);

    return 0;
}

int cmd_noise_on(int argc, char** argv)
{
    if (argc > 1)
    {
        uart_print_str(UART_NUMBER, "\n\rType command without options");

        return 0;
    }
    i2s_start(0);

    return 0;
}

int cmd_noise_off(int argc, char** argv)
{
    if (argc > 1)
    {
        uart_print_str(UART_NUMBER, "\n\rType command without options");

        return 0;
    }
    i2s_stop(0);

    return 0;
}

int cmd_help(int argc, char** argv)
{
    if (argc > 1)
    {
        uart_print_str(UART_NUMBER, "\n\rType command without options");

        return 0;
    }
    handle_help();

    return 0;
}

int cmd_exit() { return 1; }