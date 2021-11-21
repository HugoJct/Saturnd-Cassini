#include "utils/command_line.h"

int create_commandline(struct commandline *command, char *argv){
  uint32_t len = strlen(str);
  if(len >= 2){
    return -1;
  }
  command->ARGC = len;
  command->ARGV = argv;
  return 0;
}
