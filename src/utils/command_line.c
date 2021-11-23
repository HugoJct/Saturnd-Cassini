#include "utils/command_line.h"
#include "utils/custom_string.h"
#include <assert.h>


int create_commandline(struct commandline *command, int argc, char *argv[]){

  if(argv == NULL){
    return -1;
  }
  int rep;
  struct custom_string *stringcustom;
  struct custom_string **p = malloc(sizeof(struct custom_string)*argc);
  assert(p != NULL);

  for (int i = 0; i < argc; i++){
    stringcustom = malloc(sizeof(struct custom_string));
    assert(stringcustom != NULL);

    rep = create_custom_string(stringcustom , argv[i]);
    printf("%s\n", "TEST");

    if(rep == -1){
      return -1;
    }

    p[i] = stringcustom;
    printf("%s est une entrée \n", p[i]->data);
  }
  command->ARGC = htobe32(argc);
  command->ARGV = p;

  return 0;
}
