#include "commands.h"
#include "argtable3/argtable3.h"

struct arg_rex *val = NULL;
struct arg_end *end = NULL;


void handle_cmd_time()
{
  int hours = 0;
  int minutes = 0;
  int seconds = 0;

  char *tock;
  char str[8];
  char *ps;
  ps = str;

  strcpy(str, *val->sval);
  hours = atoi(*val->sval);
  *val->sval+=3;
  minutes = atoi(*val->sval);
  *val->sval+=3;
  seconds = atoi(*val->sval);
  //printf("%s\n", *val->sval);
  printf("%d\n", hours);
  printf("%d\n", minutes);
  printf("%d\n", seconds);

}

int cmd_time(int argc, char** argv)
{
        int nerrors = 0;

        void *argtable[] = {
              val = arg_rex1("val", "value", "(?:[01]\\d|2[0123]):(?:[012345]\\d):(?:[012345]\\d)", "<n>", 0, "the regular expression option"),
              //val = arg_rex1("val", "value", "[0-5]", "<rex>", 0, "the regular expression option"),
              end = arg_end(20),
        };

        nerrors = arg_parse(argc,argv,argtable);
        printf(" errs %d\n", nerrors);
        if(nerrors > 0)
        {
          uart_print_str(UART_NUMBER, "\n\rarguments line error\n");
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