idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS "include"
                        REQUIRES defines console commands uart_console uart_utils_funcs oled
                      )