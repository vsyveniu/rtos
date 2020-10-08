#include "commands_handle.h"
#include "defines.h"
#include "driver/periph_ctrl.h"
#include "soc/timer_group_struct.h"
#include <sys/time.h>
#include "sntp.h"
#include "argtable3/argtable3.h"


void handle_led_on(struct arg_lit *first, struct arg_lit *second, struct arg_lit *third, struct arg_lit *all){

      if(first->count){
        gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_1, 1);
      }
      
      if(second->count == 1){
        gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_2, 1);
      }

      if(third->count == 1){
        gpio_set_direction(LED_3, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_3, 1);
      }

      if(all->count == 1)
      {
        gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
        gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);
        gpio_set_direction(LED_3, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_1, 1);
        gpio_set_level(LED_2, 2);
        gpio_set_level(LED_3, 3);
      }
}

void handle_led_off(struct arg_lit *first, struct arg_lit *second, struct arg_lit *third, struct arg_lit *all)
{
      if(first->count == 1){
        gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_1, 0);
      }

      if(second->count == 1){
        gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_2, 0);
      }

      if(third->count == 1){
        gpio_set_direction(LED_3, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_3, 0);
      }

      if(all->count == 1)
      {
        gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
        gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);
        gpio_set_direction(LED_3, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_1, 0);
        gpio_set_level(LED_2, 0);
        gpio_set_level(LED_3, 0);
      }
}

void handle_time(struct arg_rex *val)
{
  uint64_t hours = 0;
  uint64_t minutes = 0;
  uint64_t seconds = 0;
  uint64_t timer_val = 0;

  hours = atoi(*val->sval);
  *val->sval+=3;
  minutes = atoi(*val->sval);
  *val->sval+=3;
  seconds = atoi(*val->sval);

  timer_val = ((hours * 3600) + minutes * 60 + seconds) * 1000000;

  timer_set_counter_value(TIMER_GROUP_0, TIMER_0, timer_val);
  timer_set_alarm_value(TIMER_GROUP_0, TIMER_0,  timer_val + ( 1 * TIMER_SCALE));

}


void handle_alarm(struct arg_rex *alarm_set)
{
  esp_err_t err;

  uint64_t hours = 0;
  uint64_t minutes = 0;
  uint64_t seconds = 0;
  uint64_t alarm_val = 0;
  UBaseType_t  is_filled = 0;

  hours = atoi(*alarm_set->sval);
  *alarm_set->sval+=3;
  minutes = atoi(*alarm_set->sval);
  *alarm_set->sval+=3;
  seconds = atoi(*alarm_set->sval);

  alarm_val = ((hours * 3600) + minutes * 60 + seconds);

  double time = 0;

  is_filled = uxQueueMessagesWaiting(alarm_queue);

  if(!is_filled)
  {
    xQueueSend(alarm_queue, &alarm_val, 10);
  }
  else
  {
    xQueueOverwrite(alarm_queue, &alarm_val);
  }

}

