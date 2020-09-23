#include "commands.h"
#include "argtable3/argtable3.h"

struct arg_lit *first, *second, *third, *all;
struct arg_int *freq;
struct arg_end *end;

void handle_led_on(){

      if(first->count == 1){
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
      if(all->count == 1){
        gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
        gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);
        gpio_set_direction(LED_3, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_1, 1);
        gpio_set_level(LED_2, 1);
        gpio_set_level(LED_3, 1);
      }
}

void handle_led_off(){

      if(first->count == 1)
      {
        gpio_set_level(LED_1, 0);
      }
      if(second->count == 1)
      {
        gpio_set_level(LED_2, 0);
      }
      if(third->count == 1)
      {
        gpio_set_level(LED_3, 0);
      }
      if(all->count == 1)
      {
        gpio_set_level(LED_1, 0);
        gpio_set_level(LED_2, 0);
        gpio_set_level(LED_3, 0);
      }
}

void handle_led_pulse(){
  printf("%s\n", "feck");
  xTaskNotify(send_data_to_oled, )

}



int cmd_handle(int argc, char** argv)
{
  int nerrors;


      if(argv[1] && !strcmp(argv[1], "on"))
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
          return 0;
        }
        else if(nerrors == 1 && !first->count && !second->count && !third->count && !all->count)
        {
          uart_print_str(UART_NUMBER, "\n\rYou have to specify arguments!\n");
          return 0;
        }

        handle_led_on();

         arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));

         return 42;

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
          return 0;
        }
        else if(nerrors == 1 && !first->count && !second->count && !third->count && !all->count)
        {
          uart_print_str(UART_NUMBER, "\n\rYou have to specify arguments!\n");
          return 0;
        }

        handle_led_off();

         arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));

         return 42;

      }

      if(argv[1] && !strcmp(argv[1], "pulse"))
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
          return 0;
        }
        else if(nerrors == 1 && !first->count && !second->count && !third->count && !all->count)
        {
          
          uart_print_str(UART_NUMBER, "\n\rYou have to specify arguments!\n");
          return 0;
        }
        printf("%s\n", "here");
        handle_led_pulse();

         arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));

         return 42;

      }
      
      
/*       printf("errs %d\n", nerrors);        
      printf("scalar count %d\n", scal->count);        


          while (argc > 0){
          printf("argvs   %s \n", *argv);
          argc--;
          argv++;
      } */
     
    

    uart_print_str(UART_NUMBER, "\n\rUnrecognixed command\n");
    return 0;
}

int cmd_exit()
{
    printf("%s\n", "console exit");

    //ESP_ERROR_CHECK(esp_console_deinit());

    return 24;
}