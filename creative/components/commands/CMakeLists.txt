idf_component_register(SRCS "commands.c"
                       INCLUDE_DIRS "include"
                       REQUIRES defines console uart_utils_funcs timer_utils)