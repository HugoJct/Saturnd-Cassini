#include "utils/command_line.h"
#include "utils/custom_string.h"
#include <assert.h>


int create_commandline(struct commandline *command, char *argv){

  if(argv == NULL || argv[0] == '\0'){
    return -1;
  }

  int rep;
  int i;

  const char * separator = " -";
  char *copyArgv = strdup(argv); //copy car strtok modifie la chaine, hors argv n'est pas directement accessible
  //TODO: ne pas oublier de free();
  char *strtoken = strtok(copyArgv, separator);

  struct custom_string *stringcustom;
  struct custom_string **p = malloc(sizeof(struct custom_string));
  assert(p != NULL);

  while (strtoken != NULL ) {

    stringcustom = malloc(sizeof(struct custom_string));
    assert(stringcustom != NULL);
    printf("%s\n", "prout");
    rep = create_custom_string(stringcustom ,strtoken);
    if(rep == -1){
      return -1;
    }


    p = realloc(p, sizeof(struct custom_string)+i);
    assert(p != NULL);
    p[i] = stringcustom;
    //printf("%s\n", p[i]->data);
    i++;
    strtoken = strtok ( NULL, separator);
  }
  command->ARGC = htobe32(i);
  command->ARGV = p;

  //Malloc
  free(copyArgv);
  return 0;
}
