#include "main.h"
#include "defines.h"
#include "driver/periph_ctrl.h"
#include "soc/timer_group_struct.h"


void x_task_oled_time()
{
    uint8_t hours = 0;
    uint8_t minutes = 0;
    uint seconds = 0;
    uint8_t seconds_show = 0;
    uint32_t time_val = 0;
    char str[15];
    char secs[5];
    char mins[5];
    char hors[5];
    memset(str, 0, 15);
    memset(secs, 0, 5);
    memset(mins, 0, 5);
    memset(hors, 0, 5);

    while (true)
    {
        xTaskNotifyWait(0xffffffff, 0, &time_val, portMAX_DELAY);
        //printf("%d\n", time_val);

       if(time_val == 86400)
       {
           hours = 0;
           minutes = 0;
           seconds_show = 0;
       }
       else
       {
            hours = (time_val) / 3600;
            minutes = (time_val - (3600 * hours)) / 60;
            seconds_show = time_val - (3600 * hours) - (minutes * 60);
       }
        if(seconds_show < 10)
        {
            sprintf(secs, "0%u", seconds_show);
        }
        else{
            sprintf(secs, "%u", seconds_show);
        }
        if(minutes < 10)
        {
            sprintf(mins, "0%u", minutes);
        }
        else{
            sprintf(mins, "%u", minutes);
        }
        if(hours < 10)
        {
            sprintf(hors, "0%u", hours);
        }
        else{
            sprintf(hors, "%u", hours);
        }
        //sprintf(str, "%u:%u:0%u", hours, minutes, seconds_show);
        sprintf(str, "%s:%s:%s", hors, mins, secs);
        display_str(str, 3, 0, 7);
    }
}

void IRAM_ATTR timer_intr_handle(void *param)
{
    timer_spinlock_take(TIMER_GROUP_0);
    uint64_t timer_val = timer_group_get_counter_value_in_isr(TIMER_GROUP_0, TIMER_0);
    timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, 0);
    uint64_t next_alarm = timer_val + ( 1 * TIMER_SCALE);
    timer_group_set_alarm_value_in_isr(TIMER_GROUP_0, TIMER_0, next_alarm);
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, 0);

    xTaskNotifyFromISR(notify_time_change, timer_val / 1000000, eSetValueWithOverwrite, 0);
    timer_spinlock_give(TIMER_GROUP_0);
}

void app_main(void)
{
    esp_err_t err;

    uart_console_init();
   
    esp_console_cmd_t cmd_set_time_conf = {
        .command = "time",
        .func = &cmd_time,
    };

    esp_console_cmd_t cmd_exit_conf = {
        .command = "exit",
        .func = &cmd_exit,
    };

    esp_console_cmd_register(&cmd_set_time_conf);
    esp_console_cmd_register(&cmd_exit_conf);

    timer_config_t clock_timer_conf = {
		.counter_en = false,
		.counter_dir = TIMER_COUNT_UP,
        // .alarm_en = TIMER_ALARM_EN,
        .intr_type = TIMER_INTR_LEVEL,
		.auto_reload = false,
		.divider = TIMER_DIVIDER,
    };

    err = timer_init(TIMER_GROUP_0, 0, &clock_timer_conf);
    if(err != ESP_OK)
    {
        printf("%s\n", "couldn't initiate timer");
    }
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);

    timer_set_alarm(TIMER_GROUP_0, TIMER_0, TIMER_ALARM_EN);

    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 1 * TIMER_SCALE);

      timer_enable_intr(TIMER_GROUP_0, TIMER_0);

    err = timer_isr_register(TIMER_GROUP_0, TIMER_0, timer_intr_handle, (void *)0, ESP_INTR_FLAG_IRAM, NULL);
    if(err != ESP_OK)
    {
        printf("%s\n", "couldn't initiate timers interrupt");
    }

    err = init_oled();
    if(err != ESP_OK){
        printf("%s\n", "couldn't initiate oled");
    }
    clear_oled();
 
    //xTaskCreate(x_task_update_time, "get dht data task", 2048, NULL, 1, NULL);
    xTaskCreate(x_task_oled_time, "push dht data tostatic buffer", 4096, NULL, 1, &notify_time_change);

    timer_start(TIMER_GROUP_0, TIMER_0);
}
