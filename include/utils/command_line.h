#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "custom_string.h"

struct commandline{
  uint32_t ARGC;
  struct custom_string **ARGV;
};


int create_commandline(struct commandline *command, char *argv);
