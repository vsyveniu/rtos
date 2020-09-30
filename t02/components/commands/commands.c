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
/*     int i = 0;
    while(dht_log[i].temperature != 0 && dht_log[i].humidity != 0)
    {
      i++;
    } */
     int i = 0;
     double stored_time = 0;
    double time_replace = 0;

      
      

      double time = 0;
      double fresh_item_time = 0;
      timer_get_counter_time_sec(TIMER_GROUP_0, 1, &time);
      timer_get_counter_time_sec(TIMER_GROUP_0, 0, &fresh_item_time);

/*       printf("comm %f\n", time);

        dht_log[0].timestamp = fresh_item_time;
       i = 1;
      while (i < 60){
         
                    if(dht_log[i].timestamp != 0){

                        dht_log[i].timestamp += time;
                    }
                    i++;
                } 

    time_diff = 0; */
    int hours = 0;
    int minutes = 0;
    int seconds = 0;


 
    for(int j = 0; j < 60; j++) 
    {
     // printf("temp %d\n", dht_log[j].temperature);
         hours = (int)(time - dht_log[j].timestamp) / 3600;
         minutes = (int)(time - dht_log[j].timestamp) / 60;
         seconds = (int)(time - dht_log[j].timestamp) - (3600 * hours) - (minutes * 60));
      printf("temp %d hum %d     %d hours %d minutes %d seconds ago\n", dht_log[j].temperature, dht_log[j].humidity, hours, minutes, seconds);
    }

    printf("%s\n", "feck");

}


int cmd_dhtlog(int argc, char** argv)
{
    if(argc > 1){
      uart_print_str(UART_NUMBER, "\n\rCommand have no arguments!\n");
      return 0;
    }
    else{
      dht_log_print();
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