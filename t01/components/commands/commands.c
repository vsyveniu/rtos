#include "commands.h"
#include "argtable3/argtable3.h"

struct arg_lit* first = NULL;
struct arg_lit* second = NULL;
struct arg_lit* third = NULL;
struct arg_lit* all = NULL;
struct arg_int* freq = NULL;
struct arg_int* freqhz = NULL;
struct arg_end* end = NULL;
static TaskHandle_t pulse_led1_handle = NULL;
static TaskHandle_t pulse_led2_handle = NULL;
static TaskHandle_t pulse_led3_handle = NULL;
static int led1_on = 0;
static int led2_on = 0;
static int led3_on = 0;
static int led1_pulse = 0;
static int led2_pulse = 0;
static int led3_pulse = 0;
static int freq_changed = 1;

void handle_led_on()
{
    if (first->count == 1 && !led1_pulse)
    {
        gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_1, 1);
        led1_on = 1;
    }
    if (second->count == 1 && !led2_pulse)
    {
        gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_2, 1);
        led2_on = 1;
    }
    if (third->count == 1 && !led3_pulse)
    {
        gpio_set_direction(LED_3, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_3, 1);
        led3_on = 1;
    }
}

void handle_led_off()
{
    if (first->count == 1)
    {
        if (pulse_led1_handle)
        {
            vTaskDelete(pulse_led1_handle);
            pulse_led1_handle = NULL;
        }
        gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_1, 0);
        led1_on = 0;
        led1_pulse = 0;
    }
    if (second->count == 1)
    {
        if (pulse_led2_handle)
        {
            vTaskDelete(pulse_led2_handle);
            pulse_led2_handle = NULL;
        }
        gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_2, 0);
        led2_on = 0;
        led2_pulse = 0;
    }
    if (third->count == 1)
    {
        if (pulse_led3_handle)
        {
            vTaskDelete(pulse_led3_handle);
            pulse_led3_handle = NULL;
        }
        gpio_set_direction(LED_3, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_3, 0);
        led3_on = 0;
        led3_pulse = 0;
    }

    if (all->count == 1)
    {
        if (pulse_led1_handle)
        {
            vTaskDelete(pulse_led1_handle);
            pulse_led1_handle = NULL;
        }
        if (pulse_led2_handle)
        {
            vTaskDelete(pulse_led2_handle);
            pulse_led2_handle = NULL;
        }
        if (pulse_led3_handle)
        {
            vTaskDelete(pulse_led3_handle);
            pulse_led3_handle = NULL;
        }
        gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
        gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);
        gpio_set_direction(LED_3, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_1, 0);
        gpio_set_level(LED_2, 0);
        gpio_set_level(LED_3, 0);
        led1_on = 0;
        led2_on = 0;
        led3_on = 0;
        led1_pulse = 0;
        led2_pulse = 0;
        led3_pulse = 0;
    }
}

void led1_pulse_task(void* pvParams)
{
    while (true)
    {
        ledc_set_fade_with_time(LEDC_SPEED_MODE, LEDC_CHANNEL_0, LEDC_DUTY,
                                1000);
        ledc_fade_start(LEDC_SPEED_MODE, LEDC_CHANNEL_0, LEDC_FADE_NO_WAIT);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        ledc_set_fade_with_time(LEDC_SPEED_MODE, LEDC_CHANNEL_0, 0, 1000);
        ledc_fade_start(LEDC_SPEED_MODE, LEDC_CHANNEL_0, LEDC_FADE_NO_WAIT);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void led2_pulse_task(void* pvParams)
{
    while (true)
    {
        ledc_set_fade_with_time(LEDC_SPEED_MODE, LEDC_CHANNEL_1, LEDC_DUTY,
                                1000);
        ledc_fade_start(LEDC_SPEED_MODE, LEDC_CHANNEL_1, LEDC_FADE_NO_WAIT);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        ledc_set_fade_with_time(LEDC_SPEED_MODE, LEDC_CHANNEL_1, 0, 1000);
        ledc_fade_start(LEDC_SPEED_MODE, LEDC_CHANNEL_1, LEDC_FADE_NO_WAIT);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void led3_pulse_task(void* pvParams)
{
    while (true)
    {
        ledc_set_fade_with_time(LEDC_SPEED_MODE, LEDC_CHANNEL_2, LEDC_DUTY,
                                1000);
        ledc_fade_start(LEDC_SPEED_MODE, LEDC_CHANNEL_2, LEDC_FADE_NO_WAIT);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        ledc_set_fade_with_time(LEDC_SPEED_MODE, LEDC_CHANNEL_2, 0, 1000);
        ledc_fade_start(LEDC_SPEED_MODE, LEDC_CHANNEL_2, LEDC_FADE_NO_WAIT);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void handle_led_pulse()
{
    ledc_timer_t timer_num = 0;
    uint32_t frequency = 1000;
    ledc_channel_t channel = 0;
    int gpio = 0;

    if (*freq->ival && !*freqhz->ival)
    {
        if (*freq->ival == 1)
        {
            frequency = 10;
        }

        if (*freq->ival == 2)
        {
            frequency = 42;
        }
        freq_changed = 1;
    }
    else
    {
        if (*freqhz->ival)
        {
            frequency = *freqhz->ival;
            freq_changed = 1;
        }
    }

    if (first->count == 1 && (freq_changed || !led1_pulse))
    {
        if (led1_on == 1)
        {
            timer_num = 1;
            channel = LEDC_CHANNEL_0;
            gpio = LED_1;

            ledc_channel_setup(timer_num, frequency, channel, gpio);
            xTaskCreate(led1_pulse_task, "led1_pulse_task", 2048, NULL, 1,
                        &pulse_led1_handle);
            led1_pulse = 1;
        }
        else
        {
            uart_print_str(UART_NUMBER, "\n\rLed must be turned on!");
        }
    }
    else if (first->count == 1 && !freq_changed && led1_on)
    {
        uart_print_str(UART_NUMBER, "\n\rLed is already pulsating");
    }
    else if (first->count && !led1_on)
    {
        uart_print_str(UART_NUMBER, "\n\rLed must be turned on!");
    }

    if (second->count == 1 && (freq_changed || !led2_pulse))
    {
        if (led2_on == 1)
        {
            timer_num = 2;
            channel = LEDC_CHANNEL_1;
            gpio = LED_2;

            ledc_channel_setup(timer_num, frequency, channel, gpio);
            xTaskCreate(led2_pulse_task, "led2_pulse_task", 2048, NULL, 1,
                        &pulse_led2_handle);
            led2_pulse = 1;
        }
        else
        {
            uart_print_str(UART_NUMBER, "\n\rLed must be turned on!");
        }
    }
    else if (second->count == 1 && !freq_changed && led2_on)
    {
        uart_print_str(UART_NUMBER, "\n\rLed is already pulsating");
    }
    else if (second->count && !led2_on)
    {
        uart_print_str(UART_NUMBER, "\n\rLed must be turned on!");
    }

    if (third->count == 1 && (freq_changed || !led3_pulse))
    {
        if (led3_on == 1)
        {
            timer_num = 3;
            channel = LEDC_CHANNEL_2;
            gpio = LED_3;

            ledc_channel_setup(timer_num, frequency, channel, gpio);
            xTaskCreate(led3_pulse_task, "led3_pulse_task", 2048, NULL, 1,
                        &pulse_led3_handle);
            led3_pulse = 1;
        }
        else
        {
            uart_print_str(UART_NUMBER, "\n\rLed must be turned on!");
        }
    }
    else if (third->count == 1 && !freq_changed && led3_on)
    {
        uart_print_str(UART_NUMBER, "\n\rLed is already pulsating");
    }
    else if (third->count && !led3_on)
    {
        uart_print_str(UART_NUMBER, "\n\rLed must be turned on!");
    }
    freq_changed = 0;
}

int cmd_handle(int argc, char** argv)
{
    int nerrors = 0;

    if (argv[1] && !strcmp(argv[1], "on"))
    {
        void* argtable[] = {
            first = arg_litn("1", NULL, 0, 1, "the -1 option"),
            second = arg_litn("2", NULL, 0, 1, "the -2 option"),
            third = arg_litn("3", NULL, 0, 1, "the -3 option"),
            end = arg_end(20),
        };
        nerrors = arg_parse(argc, argv, argtable);
        if (nerrors > 1)
        {
            uart_print_str(UART_NUMBER, "\n\rarguments line error");
            arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
            return 0;
        }
        if (nerrors == 1 && !first->count && !second->count && !third->count)
        {
            uart_print_str(UART_NUMBER, "\n\rYou have to specify arguments!");
            arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
            return 0;
        }

        if ((nerrors == 1 && argv[2] && strlen(argv[2]) > 2) ||
            (nerrors == 1 && argv[3] && strlen(argv[3]) > 2) ||
            (nerrors == 1 && argv[4] && strlen(argv[4]) > 2))
        {
            uart_print_str(UART_NUMBER, "\n\rarguments line error 2");
            arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
            return 0;
        }

        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        handle_led_on();

        return 0;
    }

    if (argv[1] && !strcmp(argv[1], "off"))
    {
        void* argtable[] = {
            first = arg_litn("1", NULL, 0, 1, "the -1 option"),
            second = arg_litn("2", NULL, 0, 1, "the -2 option"),
            third = arg_litn("3", NULL, 0, 1, "the -3 option"),
            all = arg_litn("a", NULL, 0, 1, "the --all option"),
            end = arg_end(20),

        };
        nerrors = arg_parse(argc, argv, argtable);
        if (nerrors > 1)
        {
            uart_print_str(UART_NUMBER, "\n\rarguments line error");
            arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
            return 0;
        }
        else if (nerrors == 1 && !first->count && !second->count &&
                 !third->count && !all->count)
        {
            uart_print_str(UART_NUMBER, "\n\rYou have to specify arguments!");
            arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
            return 0;
        }

        if ((nerrors == 1 && argv[2] && strlen(argv[2]) > 2) ||
            (nerrors == 1 && argv[3] && strlen(argv[3]) > 2) ||
            (nerrors == 1 && argv[4] && strlen(argv[4]) > 2))
        {
            uart_print_str(UART_NUMBER, "\n\rarguments line error 2");
            arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
            return 0;
        }

        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));

        handle_led_off();

        return 0;
    }
    if (argv[1] && !strcmp(argv[1], "pulse"))
    {
        void* argtable[] = {
            first = arg_litn("1", NULL, 0, 1, "the -1 option"),
            second = arg_litn("2", NULL, 0, 1, "the -2 option"),
            third = arg_litn("3", NULL, 0, 1, "the -3 option"),
            all = arg_litn("a", NULL, 0, 1, "the -a option"),
            freq = arg_int1("f", "freq", "<n>", "the -f option"),
            freqhz = arg_int1(NULL, "freqhz", "<n>", "the -fhz option"),
            end = arg_end(20),
        };

        *freq->ival = 0;
        *freqhz->ival = 0;
        nerrors = arg_parse(argc, argv, argtable);

        if (nerrors > 1)
        {
            uart_print_str(UART_NUMBER, "\n\rarguments line error");
            arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
            return 0;
        }
        else if (nerrors == 1 && !first->count && !second->count &&
                 !third->count && !all->count)
        {
            uart_print_str(UART_NUMBER, "\n\rYou have to specify arguments!");
            arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
            return 0;
        }

        int i = argc - 1;
        while (i > 1)
        {
            if (strlen(argv[i]) > 2 && !strstr(argv[i], "-f"))
            {
                uart_print_str(UART_NUMBER, "\n\rarguments line error");
                arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
                return 0;
            }
            i--;
        }

        if (*freq->ival != 0)
        {
            if (*freq->ival == 1 || *freq->ival == 2)
            {
                handle_led_pulse();
                arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
                return 0;
            }
            else
            {
                uart_print_str(UART_NUMBER,
                               "\n\rFrequency can be 1 or 2 (Gosh! Why?)");
                arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
                return 0;
            }
        }

        if (*freqhz->ival <= 78125)
        {
            handle_led_pulse();
            arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
            return 0;
        }
        else
        {
            uart_print_str(UART_NUMBER,
                           "\n\rFrequency can't be more than 78125");
            arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
            return 0;
        }

        handle_led_pulse();
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));

        return 0;
    }
    uart_print_str(
        UART_NUMBER,
        "\n\rCommand not found");
    return 0;
}

int cmd_exit() { return 1; }