#include "main.h"
#include "defines.h"
#include "driver/periph_ctrl.h"
#include "soc/timer_group_struct.h"


void x_task_oled_time()
{
    uint32_t time_val = 0;

    while (true)
    {
        xTaskNotifyWait(0xffffffff, 0, &time_val, portMAX_DELAY);
        printf("%d\n", time_val);
    }
}

void IRAM_ATTR timer_intr_handle(void *param)
{
    timer_spinlock_take(TIMER_GROUP_0);
    uint32_t timer_val = timer_group_get_counter_value_in_isr(TIMER_GROUP_0, TIMER_0);
    timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, 0);
    timer_counter_value += (uint32_t) (1.00 * TIMER_SCALE);
    
    timer_group_set_alarm_value_in_isr(TIMER_GROUP_0, TIMER_0, timer_val);
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, 0);

    xTaskNotifyFromISR(notify_time_change, timer_val, eSetValueWithOverwrite, 0);
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
		.auto_reload = false,
		.divider = TIMER_DIVIDER,
    };

    err = timer_init(TIMER_GROUP_0, 0, &clock_timer_conf);
    if(err != ESP_OK)
    {
        printf("%s\n", "couldn't initiate timer");
    }
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0x00000000ULL);

    timer_set_alarm(TIMER_GROUP_0, TIMER_0, TIMER_ALARM_EN);

    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 1.00 * TIMER_SCALE);

      timer_enable_intr(TIMER_GROUP_0, TIMER_0);

    err = timer_isr_register(TIMER_GROUP_0, TIMER_0, timer_intr_handle, (void *)0, ESP_INTR_FLAG_IRAM, NULL);
    if(err != ESP_OK)
    {
    printf("%s\n", "couldn't initiate timers im=nterrupt");
    }

 
    //xTaskCreate(x_task_update_time, "get dht data task", 2048, NULL, 1, NULL);
    xTaskCreate(x_task_oled_time, "push dht data tostatic buffer", 2048, NULL, 1, &notify_time_change);

    timer_start(TIMER_GROUP_0, TIMER_0);
}
