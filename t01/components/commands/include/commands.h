#ifndef COMMANDS_H
# define COMMANDS_H

#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include "defines.h"
#include "esp_console.h"
#include "esp_err.h"

int     cmd_handle(int argc, char **argv);
int     cmd_exit();

#endif