#include "main.h"
#include "defines.h"
#include "driver/periph_ctrl.h"
#include "soc/timer_group_struct.h"
#include <sys/time.h>
#include "audio_table.h"

static TaskHandle_t notify_dht_change = NULL;
static TaskHandle_t notify_time_change = NULL;
static TaskHandle_t notify_alarm_ring = NULL;

void x_task_dht()
{
    uint8_t dht_buff[5];
    memset(dht_buff, 0, 5);
    dht_data_s dht_data[1];
    dht_data_s dht_data_peek[1];
    dht_data_peek->temperature = 0;
    dht_data_peek->humidity = 0;

    while (true)
    {
        if (xSemaphoreTake(dht_peek_mutex, portMAX_DELAY))
        {
            get_DHT_data(dht_buff);
            dht_data->temperature =  dht_buff[2];
            dht_data->humidity =  dht_buff[0];
            xQueuePeek(dht_queue, &dht_data_peek, 10);
            if(dht_data_peek->temperature != dht_data->temperature || dht_data_peek->humidity != dht_data->humidity)
            {
                xQueueOverwrite(dht_queue, &dht_data);
                xTaskNotify(notify_dht_change, 1, eSetValueWithOverwrite);
            }
            xSemaphoreGive(dht_peek_mutex);
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS); 

    }
}

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
            timer_set_counter_value(TIMER_GROUP_0, TIMER_1, 0);
           timer_set_alarm_value(TIMER_GROUP_0, TIMER_0,  1 * TIMER_SCALE);
       }
       else
       {
            hours = (time_val) / 3600;
            minutes = (time_val - (3600 * hours)) / 60;
            seconds_show = time_val - (3600 * hours) - (minutes * 60);
       }
       if(xSemaphoreTake(oled_mutex, portMAX_DELAY))
       {
            str = make_time_str(hours, minutes, seconds_show);
            if(display_str_fat_row_2(str, 0, 8, 1, 2)){
                printf("%s\n", "string is too big");
            }
            xSemaphoreGive(oled_mutex);
            free(str); 
       }
    }
}

void v_task_display_dht()
{
    dht_data_s dht_data_receive[1];
    char temperature_buff[17];
    char humidity_buff[16];
    memset(temperature_buff, 0, 17);
    memset(humidity_buff, 0, 16);
    uint32_t dht_changed = 0;

    while (true)
    {
        xTaskNotifyWait(0xffffffff, 0, &dht_changed, portMAX_DELAY);
        xQueuePeek(dht_queue, &dht_data_receive, 10);
    
        sprintf(temperature_buff, "Temperature %dC", dht_data_receive->temperature);
        sprintf(humidity_buff, "Humidity %u%%", dht_data_receive->humidity);
        if(xSemaphoreTake(oled_mutex, portMAX_DELAY))
        {
            display_str(temperature_buff, 5, 1, 7);
            display_str(humidity_buff, 7, 1, 7);
            dht_changed = 0;
            xSemaphoreGive(oled_mutex);
        }
    }
}

void v_task_alarm_ring()
{
    uint32_t timer_val = 0;
    uint64_t alarm_val = 0;
    size_t i2s_bytes_write = 0;

    while (true)
    {
        xTaskNotifyWait(0xffffffff, 0, &timer_val, portMAX_DELAY);
        if(xQueuePeek(alarm_queue, &alarm_val, 10))
        {
            if(timer_val == alarm_val)
            {
                i2s_start(0);
                i2s_write(0, audio_table, sizeof(audio_table), &i2s_bytes_write, 100);
                i2s_stop(0);
            }
        }
    }
}

void v_task_make_noise()
{
    size_t i2s_bytes_write = 0;

    while (true)
    {
      i2s_write(0, audio_table, sizeof(audio_table), &i2s_bytes_write, 100);
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
    xTaskNotifyFromISR(notify_alarm_ring, timer_val / 1000000, eSetValueWithOverwrite, 0);
    timer_spinlock_give(TIMER_GROUP_0);
}


void IRAM_ATTR timer_alarm_handle(void *param)
{
    timer_spinlock_take(TIMER_GROUP_0);

    uint64_t timer_val = timer_group_get_counter_value_in_isr(TIMER_GROUP_0, TIMER_1);
    timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_1);
    uint64_t next_alarm = timer_val  + ( 10 * TIMER_SCALE);
    timer_group_set_alarm_value_in_isr(TIMER_GROUP_0, TIMER_1, next_alarm);
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, TIMER_1);

    xTaskNotifyFromISR(notify_alarm_ring, 1, eSetValueWithOverwrite, 0);
    timer_spinlock_give(TIMER_GROUP_0);
}

void app_main(void)
{
    esp_err_t err;

    gpio_set_direction(DHT_POWER, GPIO_MODE_OUTPUT);	
	gpio_set_level(DHT_POWER, 1);
    gpio_set_direction(EN_OLED, GPIO_MODE_OUTPUT);
	gpio_set_level(EN_OLED, 1);
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    err = uart_console_init();
    if(err != ESP_OK){
        printf("%s\n", "couldn't init uart console. Have to reboot");
        esp_restart();
    }
    
    register_cmnd_set();

    err = init_oled();
    if(err != ESP_OK)
    {
        printf("%s\n", "couldn't initiate oled");
    }
    clear_oled();

    timer_config_t clock_timer_conf = {
		.counter_en = false,
		.counter_dir = TIMER_COUNT_UP,
        .alarm_en = TIMER_ALARM_EN,
        .intr_type = TIMER_INTR_LEVEL,
		.auto_reload = false,
		.divider = TIMER_DIVIDER,
    };

    err = timer_init(TIMER_GROUP_0, TIMER_0, &clock_timer_conf);
    if(err != ESP_OK)
    {
        printf("%s\n", "couldn't initiate timer");
    }

    err = timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 1 * TIMER_SCALE);
    if(err != ESP_OK)
    {
        printf("%s\n", "couldn't set timer alarm");
    }

    err = timer_isr_register(TIMER_GROUP_0, TIMER_0, timer_intr_handle, (void *)0, ESP_INTR_FLAG_IRAM, NULL);
    if(err != ESP_OK)
    {
        printf("%s\n", "couldn't initiate timers interrupt");
    }

    err = i2s_setup();
    if(err != ESP_OK){
        printf("%s\n", esp_err_to_name(err));
    }
    
    dht_queue = xQueueCreate( 1, sizeof( dht_data_s) );
    alarm_queue = xQueueCreate( 1, sizeof(uint64_t) );
    
    dht_peek_mutex = xSemaphoreCreateMutex();
    oled_mutex = xSemaphoreCreateMutex();

    xTaskCreate(x_task_dht, "get dht data task", 2048, NULL, 1, NULL);
    xTaskCreate(v_task_display_dht, "display himidity and temperature on change", 2048, NULL, 1, &notify_dht_change);
    xTaskCreate(x_task_oled_time, "push dht data tostatic buffer", 4096, NULL, 1, &notify_time_change);
    xTaskCreate(v_task_alarm_ring, "play alarm tone", 2048, NULL, 1, &notify_alarm_ring);
    xTaskCreate(v_task_make_noise, "play some music", 2048, NULL, 1, NULL);

    char *str;

    str = make_time_str(0, 0, 0);

    if(display_str_fat_row_2(str, 0, 8, 1, 2))
    {
        printf("%s\n", "string is too big");
    }
    free(str); 
    
    timer_start(TIMER_GROUP_0, TIMER_0);
 
}
