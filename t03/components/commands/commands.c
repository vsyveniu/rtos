#include "commands.h"
#include "argtable3/argtable3.h"

struct arg_rex *val = NULL;
struct arg_end *end = NULL;


void handle_cmd_time()
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

int cmd_time(int argc, char** argv)
{
        int nerrors = 0;

        void *argtable[] = {
              val = arg_rex1("val", "value", "(?:[01]\\d|2[0123]):(?:[012345]\\d):(?:[012345]\\d)", "<n>", 0, "the regular expression option"),
              end = arg_end(20),
        };

        nerrors = arg_parse(argc,argv,argtable);
        if(nerrors > 0)
        {
          uart_print_str(UART_NUMBER, "\n\rarguments line error");
          arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
          return 0;
        }

        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        handle_cmd_time();

        return 0;
}

int cmd_exit()
{
    return 1;
}