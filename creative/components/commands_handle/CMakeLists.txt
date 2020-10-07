idf_component_register(SRCS "commands_handle.c"
                       INCLUDE_DIRS "include"
                       REQUIRES defines console uart_utils_funcs timer_utils main)