#include "commands.h"
#include "argtable3/argtable3.h"

struct arg_lit *first = NULL;
struct arg_lit *second = NULL;
struct arg_lit *third = NULL;
struct arg_lit *all = NULL;
struct arg_int *freq = NULL;
struct arg_end *end = NULL;


int cmd_time(int argc, char** argv)
{
  return 0;
}

int cmd_exit()
{
    return 1;
}