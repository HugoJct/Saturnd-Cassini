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


int create_commandline(struct commandline *command, int argc, char *argv[]);
/*
  Cette fonction créer une structure command line à partir d'un int argc et un tableau de tableau de string (pointeur)
  return 0 si tout c'est bien passé ou -1 en cas d'echec
*/
