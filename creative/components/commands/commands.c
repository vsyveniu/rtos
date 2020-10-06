#include "commands.h"
#include "argtable3/argtable3.h"

struct arg_lit *first = NULL;
struct arg_lit *second = NULL;
struct arg_lit *third = NULL;
struct arg_lit *all = NULL;
struct arg_rex *val = NULL;
struct arg_end *end = NULL;

void handle_led_on(){

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

void handle_led_off()
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

int cmd_led_on(int argc, char** argv)
{
      int8_t nerrors = 0;

      void *argtable[] = {
          first = arg_litn("1", NULL, 0, 1, "the -1 option"),
          second = arg_litn("2", NULL, 0, 1, "the -2 option"),
          third = arg_litn("3", NULL, 0, 1, "the -3 option"),
          all = arg_litn("a", "all", 0, 1, "the -all option"),
          end = arg_end(20),
      };

      nerrors = arg_parse(argc,argv,argtable);

      if(nerrors > 0)
      {
          uart_print_str(UART_NUMBER, "\n\rarguments line error\n");
          arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
          return 0;

      }
      if(!first->count && !second->count && !third->count && !all->count)
      {
          uart_print_str(UART_NUMBER, "\n\rYou have to specify arguments!");
          arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
          return 0;
      }

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));

    handle_led_on();
    return 0;
}

int cmd_led_off(int argc, char** argv)
{
    int8_t nerrors = 0;

      void *argtable[] = {
          first = arg_litn("1", NULL, 0, 1, "the -1 option"),
          second = arg_litn("2", NULL, 0, 1, "the -2 option"),
          third = arg_litn("3", NULL, 0, 1, "the -3 option"),
          all = arg_litn("a", "all", 0, 1, "the -all option"),
          end = arg_end(20),
      };

      nerrors = arg_parse(argc,argv,argtable);

      if(nerrors > 0)
      {
          uart_print_str(UART_NUMBER, "\n\rarguments line error\n");
          arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
          return 0;

      }
      if(!first->count && !second->count && !third->count && !all->count)
      {
          uart_print_str(UART_NUMBER, "\n\rYou have to specify arguments!");
          arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
          return 0;
      }

    handle_led_off();
    return 0;
}

int cmd_exit()
{
    return 1;
}