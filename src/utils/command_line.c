#include "utils/command_line.h"
#include "utils/custom_string.h"
#include <assert.h>


int create_commandline(struct commandline *command, char *argv){

  if(argv == NULL || argv[0] == '\0'){
    return -1;
  }

  int rep;
  int i;

  char * strtoken = strtok (argv, " ");
  struct custom_string *stringcustom;

  while (strtoken != NULL ) {

    stringcustom = malloc(sizeof(struct custom_string));
    printf("test 3\n");

    assert(stringcustom != NULL);

    rep = create_custom_string(stringcustom ,strtoken);
    if(rep == -1){
      return -1;
    }
    struct custom_string **p = realloc(command->ARGV, sizeof(struct custom_string)+i);
    assert( p != NULL);
    command->ARGV = p;
    command->ARGV[i] = stringcustom;
    i++;
  }

  command->ARGC = htobe32(i);
  return 0;
}
