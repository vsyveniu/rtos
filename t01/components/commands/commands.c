#include "commands.h"
#include "argtable3/argtable3.h"

struct arg_lit *first, *second, *third, *all;
struct arg_int *freq;
struct arg_end *end;
static TaskHandle_t pulse_led1_handle = NULL;
static TaskHandle_t pulse_led2_handle = NULL;
static TaskHandle_t pulse_led3_handle = NULL;

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

printf("%s\n", "runtime");
      if(first->count == 1)
      {
        if(pulse_led1_handle)
        {
          vTaskDelete(pulse_led1_handle);
          pulse_led1_handle = NULL;
        }
        gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_1, 0);
        
      }
      if(second->count == 1)
      {
        printf("%s\n", "feck");
        if(pulse_led2_handle)
        {
          vTaskDelete(pulse_led2_handle);
          pulse_led2_handle = NULL;
        }
        gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_2, 0);
        
      }
      if(third->count == 1)
      {
        
        if(pulse_led3_handle)
        {
          vTaskDelete(pulse_led3_handle);
          pulse_led3_handle = NULL;
        }
        gpio_set_direction(LED_3, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_3, 0);
      }

      if(all->count == 1)
      {
        if(pulse_led1_handle)
        {
          vTaskDelete(pulse_led1_handle);
          pulse_led1_handle = NULL;
        }
        if(pulse_led2_handle)
        {
          vTaskDelete(pulse_led2_handle);
          pulse_led2_handle = NULL;
        }
        if(pulse_led3_handle)
        {
          vTaskDelete(pulse_led3_handle);
          pulse_led3_handle = NULL;
        }
        gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
        gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);
        gpio_set_direction(LED_3, GPIO_MODE_OUTPUT);
        gpio_set_level(LED_1, 0);
        gpio_set_level(LED_2, 0);
        gpio_set_level(LED_3, 0);
      }
}

/* void led_pulse_task(void *pvParams){

    ledc_timer_t timer_num = 0;
    uint16_t frequency = 0;
    ledc_channel_t channel = 0;
    int gpio = 0; 

  if(first->count == 1){
    timer_num = 1;
    frequency = 1000;
    channel = LEDC_CHANNEL_0;
    gpio = LED_1;  

    ledc_channel_setup(timer_num, frequency, channel, gpio);  
  }
  if(second->count == 1){
    timer_num = 2;
    frequency = 1000;
    channel = LEDC_CHANNEL_1;
    gpio = LED_2;  

    ledc_channel_setup(timer_num, frequency, channel, gpio);  
  }
  if(third->count == 1){
    timer_num = 3;
    frequency = 1000;
    channel = LEDC_CHANNEL_2;
    gpio = LED_3;  

    ledc_channel_setup(timer_num, frequency, channel, gpio);  
  }
  ledc_fade_func_install(0);
    while (true){
      printf("%s\n", "BITCH!!");
        ledc_set_fade_with_time(LEDC_SPEED_MODE, channel, LEDC_DUTY, 2000);
        ledc_fade_start(LEDC_SPEED_MODE, channel, LEDC_FADE_WAIT_DONE);
        ledc_set_fade_with_time(LEDC_SPEED_MODE, channel, 0, 2000);
        ledc_fade_start(LEDC_SPEED_MODE, channel, LEDC_FADE_WAIT_DONE);
    }

} */

void led1_pulse_task(void *pvParams){
    while (true){
      printf("%s\n", "BITCH!!1111");
 
         ledc_set_fade_with_time(LEDC_SPEED_MODE, LEDC_CHANNEL_0, LEDC_DUTY, 1000);
        ledc_fade_start(LEDC_SPEED_MODE, LEDC_CHANNEL_0, LEDC_FADE_WAIT_DONE);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        ledc_set_fade_with_time(LEDC_SPEED_MODE, LEDC_CHANNEL_0, 0, 1000);
        ledc_fade_start(LEDC_SPEED_MODE, LEDC_CHANNEL_0, LEDC_FADE_WAIT_DONE);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

}

void led2_pulse_task(void *pvParams){
    while (true){
      printf("%s\n", "BITCH!!222");

         ledc_set_fade_with_time(LEDC_SPEED_MODE, LEDC_CHANNEL_1, LEDC_DUTY, 1000);
        ledc_fade_start(LEDC_SPEED_MODE, LEDC_CHANNEL_1, LEDC_FADE_WAIT_DONE);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        ledc_set_fade_with_time(LEDC_SPEED_MODE, LEDC_CHANNEL_1, 0, 1000);
        ledc_fade_start(LEDC_SPEED_MODE, LEDC_CHANNEL_1, LEDC_FADE_WAIT_DONE);
        vTaskDelay(100 / portTICK_PERIOD_MS); 
    }

}

void led3_pulse_task(void *pvParams){
    while (true){
      printf("%s\n", "BITCH!!333");

         ledc_set_fade_with_time(LEDC_SPEED_MODE, LEDC_CHANNEL_2, LEDC_DUTY, 1000);
        ledc_fade_start(LEDC_SPEED_MODE, LEDC_CHANNEL_2, LEDC_FADE_WAIT_DONE);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        ledc_set_fade_with_time(LEDC_SPEED_MODE, LEDC_CHANNEL_2, 0, 1000);
        ledc_fade_start(LEDC_SPEED_MODE, LEDC_CHANNEL_2, LEDC_FADE_WAIT_DONE);
        vTaskDelay(100 / portTICK_PERIOD_MS); 
    }

}


void handle_led_pulse(){

  ledc_timer_t timer_num = 0;
  int32_t frequency = 0;
  ledc_channel_t channel = 0;
  int gpio = 0; 
  frequency = 1000;

  printf("freq %d\n", *freq->ival);

  if(*freq->ival){
    frequency = *freq->ival;
  }
 
  if(first->count == 1){
    timer_num = 1;
    channel = LEDC_CHANNEL_0;
    gpio = LED_1;  

    ledc_channel_setup(timer_num, frequency, channel, gpio);
    xTaskCreate(led1_pulse_task, "led1_pulse_task", 2048, NULL, 1, &pulse_led1_handle);  
  }
  if(second->count == 1){
    timer_num = 2;
    channel = LEDC_CHANNEL_1;
    gpio = LED_2;  

    ledc_channel_setup(timer_num, frequency, channel, gpio);
    xTaskCreatePinnedToCore(led2_pulse_task, "led2_pulse_task", 2048, NULL, 1, &pulse_led2_handle, 1);  
  }
  if(third->count == 1){
    timer_num = 3;
    channel = LEDC_CHANNEL_2;
    gpio = LED_3;  

    ledc_channel_setup(timer_num, frequency, channel, gpio);
    xTaskCreatePinnedToCore(led3_pulse_task, "led3_pulse_task", 2048, NULL, 1, &pulse_led3_handle, 1);  
  }
  
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
                freq = arg_int1("f", "freq", "<n>", "the -b option"),
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