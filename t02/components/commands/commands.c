#include "commands.h"
#include "argtable3/argtable3.h"

struct arg_lit *first = NULL;
struct arg_lit *second = NULL;
struct arg_lit *third = NULL;
struct arg_lit *all = NULL;
struct arg_int *freq = NULL;
struct arg_end *end = NULL;


int cmd_dhtlog(int argc, char** argv)
{
    if(argc > 1){
      uart_print_str(UART_NUMBER, "\n\rYou have to specify arguments!\n");
    }
      /* int nerrors = 0;

      if(argv[1] && !strcmp(argv[1], "on"))
      {
          void *argtable[] = {
                first = arg_litn("1", NULL, 0, 1, "the -b option"),
                second = arg_litn("2", NULL, 0, 1, "the -b option"),
                third = arg_litn("3", NULL, 0, 1, "the -b option"),
                end = arg_end(20),
          };
        nerrors = arg_parse(argc,argv,argtable);
        if(nerrors > 1)
        {
          uart_print_str(UART_NUMBER, "\n\rarguments line error\n");
          arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
          return 0;
        }
        else if(nerrors == 1 && !first->count && !second->count && !third->count)
        {
         
          uart_print_str(UART_NUMBER, "\n\rYou have to specify arguments!\n");
          arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
          return 0;
        }

        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        handle_led_on();

         return 0;
      }

      if(argv[1] && !strcmp(argv[1], "off"))
      {
          void *argtable[] = {
                first = arg_litn("1", NULL, 0, 1, "the -b option"),
                second = arg_litn("2", NULL, 0, 1, "the -b option"),
                third = arg_litn("3", NULL, 0, 1, "the -b option"),
                all = arg_litn("a", "all", 0, 1, "the -b option"),
                end = arg_end(20),

          };
        nerrors = arg_parse(argc,argv,argtable);
        if(nerrors > 1){
          uart_print_str(UART_NUMBER, "\n\rarguments line error\n");
          arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
          return 0;
        }
        else if(nerrors == 1 && !first->count && !second->count && !third->count && !all->count)
        {
          uart_print_str(UART_NUMBER, "\n\rYou have to specify arguments!\n");
          arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
          return 0;
        }

        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));

        handle_led_off();

         return 0;

      }
      if(argv[1] && !strcmp(argv[1], "pulse"))
      {
          void *argtable[] = {
                first = arg_litn("1", NULL, 0, 1, "the -b option"),
                second = arg_litn("2", NULL, 0, 1, "the -b option"),
                third = arg_litn("3", NULL, 0, 1, "the -b option"),
                all = arg_litn("a", "all", 0, 1, "the -b option"),
                freq = arg_int1("f", "freq", "<n>", "the -b option"),
                end = arg_end(20),
          };

        *freq->ival = 0;
        nerrors = arg_parse(argc,argv,argtable);
        
        if(nerrors > 1){
          uart_print_str(UART_NUMBER, "\n\rarguments line error\n");
          arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
          return 0;
        }
        else if(nerrors == 1 && !first->count && !second->count && !third->count && !all->count)
        {
          uart_print_str(UART_NUMBER, "\n\rYou have to specify arguments!\n");
          arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
          return 0;
        }
        
        if(*freq->ival <= 78125)
        {
          handle_led_pulse();
        }
        else
        {
          uart_print_str(UART_NUMBER, "\n\rFrequency can't be more than 78125");
        }

        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));

         return 0;

      }
    uart_print_str(UART_NUMBER, "\n\rCommand not found. Try to type not such a crap like this time"); */
  return 0;
}

int cmd_exit()
{
    return 1;
}