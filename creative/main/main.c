#include "main.h"
#include "defines.h"
#include "driver/periph_ctrl.h"
#include "soc/timer_group_struct.h"
#include <sys/time.h>
#include "sntp.h"

static TaskHandle_t notify_time_change = NULL;

void x_task_oled_time()
{
    uint8_t hours = 0;
    uint8_t minutes = 0;
    uint8_t seconds_show = 0;
    uint32_t time_val = 0;
    char *str;

    while (true)
    {
        xTaskNotifyWait(0xffffffff, 0, &time_val, portMAX_DELAY);

       if(time_val == 86400)
       {
           hours = 0;
           minutes = 0;
           seconds_show = 0;
           timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
           timer_set_alarm_value(TIMER_GROUP_0, TIMER_0,  1 * TIMER_SCALE);
       }
       else
       {
            hours = (time_val) / 3600;
            minutes = (time_val - (3600 * hours)) / 60;
            seconds_show = time_val - (3600 * hours) - (minutes * 60);
       }
       str = make_time_str(hours, minutes, seconds_show);
       display_str(str, 3, 0, 7);
       free(str);
    }
}

void IRAM_ATTR timer_intr_handle(void *param)
{
    timer_spinlock_take(TIMER_GROUP_0);
    timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, 0);

    uint64_t timer_val = timer_group_get_counter_value_in_isr(TIMER_GROUP_0, TIMER_0);
    uint64_t next_alarm = timer_val + ( 1 * TIMER_SCALE);

    timer_group_set_alarm_value_in_isr(TIMER_GROUP_0, TIMER_0, next_alarm);
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, 0);

    xTaskNotifyFromISR(notify_time_change, timer_val / 1000000, eSetValueWithOverwrite, 0);
    timer_spinlock_give(TIMER_GROUP_0);
}


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

    struct timeval current_time;

    sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);

    while (true)
    {
        gettimeofday(&current_time, NULL);
    printf("seconds : %ld\n  micro seconds : %ld\n",
        current_time.tv_sec, current_time.tv_usec);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
