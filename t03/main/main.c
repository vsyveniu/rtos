#include "main.h"
#include "defines.h"
#include "driver/periph_ctrl.h"
#include "soc/timer_group_struct.h"

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
        .alarm_en = TIMER_ALARM_EN,
        .intr_type = TIMER_INTR_LEVEL,
		.auto_reload = false,
		.divider = TIMER_DIVIDER,
    };

    err = timer_init(TIMER_GROUP_0, 0, &clock_timer_conf);
    if(err != ESP_OK)
    {
        printf("%s\n", "couldn't initiate timer");
    }
   // timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);

    //timer_set_alarm(TIMER_GROUP_0, TIMER_0, TIMER_ALARM_EN);

    err = timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 1 * TIMER_SCALE);
    if(err != ESP_OK)
    {
        printf("%s\n", "couldn't set timer alarm");
    }

    err = timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    if(err != ESP_OK)
    {
        printf("%s\n", "couldn't enable timer interrupt");
    }

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
 
    xTaskCreate(x_task_oled_time, "push dht data tostatic buffer", 4096, NULL, 1, &notify_time_change);

    timer_start(TIMER_GROUP_0, TIMER_0);
}
