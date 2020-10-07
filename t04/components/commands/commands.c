#include "commands.h"
#include "argtable3/argtable3.h"
#include "driver/i2s.h"


int cmd_noise_on(int argc, char **argv)
{
    if(argc > 1)
    {
      uart_print_str(UART_NUMBER, "\n\rType command without options. And don't fuck with me. No tricks. Or you will go to jail");

      return 0;
    }
    i2s_start(0);

    return 0;
}

int cmd_noise_off(int argc, char **argv)
{
    if(argc > 1)
    {
      uart_print_str(UART_NUMBER, "\n\rType command without options. And don't fuck with me. No tricks. Or you will go to jail");

      return 0;
    }
    i2s_stop(0);

    return 0;
}

int cmd_exit()
{
    return 1;
}