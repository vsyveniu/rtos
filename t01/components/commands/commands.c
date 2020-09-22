#include "commands.h"
#include "argtable3/argtable3.h"

int cmd_handle(int argc, char** argv)
{

      struct arg_lit *f, *help, *on, *off, *pulse;
        struct arg_int *scal;
        struct arg_end *end;

      void *argtable[] = {
            on = arg_litn("1", NULL, 0, 1, "the -b option"),
            off = arg_litn("2", NULL, 0, 1, "the -b option"),
            pulse = arg_litn("3", NULL, 0, 1, "the -b option"),
            scal = arg_intn(NULL,"1", "<n>", 0, 1, "foo value"),
            end = arg_end(20),

    };

    printf("%s\n", "command handle fuck");

    printf("argnums   %d \n", argc);

    int nerrors;
    nerrors = arg_parse(argc,argv,argtable);

   /*    if (help->count > 0){
          printf("%s\n", "HELP!");
      } */

    printf("errs %d\n", nerrors);    
    printf("on count %d\n", on->count);        
    printf("scalar count %d\n", scal->count);        

    gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_1, 1);

        while (argc > 0){
        printf("argvs   %s \n", *argv);
        argc--;
        argv++;
    }
    return 42;
}

int cmd_exit()
{
    printf("%s\n", "console exit");

    //ESP_ERROR_CHECK(esp_console_deinit());

    return 24;
}