#include "utils/command_line.h"
#include "utils/custom_string.h"
#include <assert.h>


int create_commandline(struct commandline *command, int argc, char *argv[]){

  if(argv == NULL){ //TODO: verifier arguments vide
    return -1;
  }

  int rep; //retient les erreurs possibles
  struct custom_string *stringcustom; //creation d'un object custom_string
  struct custom_string **p = malloc(sizeof(struct custom_string)*argc); // création d'un tableau de custom_string
  assert(p != NULL);

  for (int i = 0; i < argc; i++){ //pour tous les agurments présent dans argv
    stringcustom = malloc(sizeof(struct custom_string)); //allocation en mémoire
    assert(stringcustom != NULL);
    rep = create_custom_string(stringcustom , argv[i]); //creation d'un string_custom à partir d'un élément du tableau argv (string basique vers string custom)
    if(rep == -1){
      return -1;
    }
    p[i] = stringcustom;
  }
  //initlisation de la structure
  command->ARGC = htobe32(argc);
  command->ARGV = p;

  return 0;
}
