idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS "include"
                        REQUIRES defines console commands fatfs nvs_flash uart_utils_funcs blinker
                      )