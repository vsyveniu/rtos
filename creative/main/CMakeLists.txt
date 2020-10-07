idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS "include"
                        REQUIRES defines console commands commands_handle uart_console uart_utils_funcs oled timer_utils dht11 lwip
                      )