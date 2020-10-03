#include "timer_utils.h"


char *make_time_str(uint8_t hours, uint8_t minutes, uint8_t seconds){
    char *str;
    char seconds_str[5];
    char minutes_str[5];
    char hours_str[5];
    memset(seconds_str, 0, 5);
    memset(minutes_str, 0, 5);
    memset(hours_str, 0, 5);

    str = (char*)calloc(15, sizeof(char));
    if(!str)
    {
      return 0;
    }

    if(seconds < 10)
    {
        sprintf(seconds_str, "0%u", seconds);
    }
    else
    {
        sprintf(seconds_str, "%u", seconds);
    }
    if(minutes < 10)
    {
        sprintf(minutes_str, "0%u", minutes);
    }
    else
    {
        sprintf(minutes_str, "%u", minutes);
    }
    if(hours < 10)
    {
        sprintf(hours_str, "0%u", hours);
    }
    else
    {
        sprintf(hours_str, "%u", hours);
    }

    sprintf(str, "%s:%s:%s", hours_str, minutes_str, seconds_str);

    return (str);
}