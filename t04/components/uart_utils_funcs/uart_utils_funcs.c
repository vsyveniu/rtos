#include "uart_utils_funcs.h"

void uart_print_str(int uart_num, char *str){
    uart_write_bytes(uart_num, str, strlen(str));
}