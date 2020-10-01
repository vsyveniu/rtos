#include "commands.h"
#include "argtable3/argtable3.h"

struct arg_lit *first = NULL;
struct arg_lit *second = NULL;
struct arg_lit *third = NULL;
struct arg_lit *all = NULL;
struct arg_int *freq = NULL;
struct arg_end *end = NULL;

void dht_log_print()
{
    double time = 0;
    timer_get_counter_time_sec(TIMER_GROUP_0, 1, &time);
    int hours = 0;
    int minutes = 0;
    int seconds = 0;

    char str[95];
    memset(str, 0, 95);

    for(int i = 0; i < 60; i++) 
    {
      if(dht_log[i].temperature != 0 || dht_log[i].humidity != 0)
      {
        hours = (int)(time - dht_log[i].timestamp) / 3600;
        minutes = (int)(time - dht_log[i].timestamp) / 60;
        seconds = (int)(time - dht_log[i].timestamp) - (3600 * hours) - (minutes * 60);

        sprintf(str, "\n\rTemperature: %dС Humidity: %d%%     %d hours  %d  minutes  %d seconds ago",  dht_log[i].temperature, dht_log[i].humidity, hours, minutes, seconds);
        uart_print_str(UART_NUMBER, str);
      }
    }
}

int cmd_dhtlog(int argc, char** argv)
{
    if(argc > 1)
    {
      uart_print_str(UART_NUMBER, "\n\rCommand have no arguments!\n");
      return 0;
    }
    else
    {
      dht_log_print();
      return 0;
    }
}

int cmd_exit()
{
    return 1;
}