#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct commandline{
  uint32_t ARGC;
  char *ARGV;
}


int create_commandline(struct *commandline command);
