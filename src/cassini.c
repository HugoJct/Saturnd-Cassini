#include "cassini.h"
#include "utils/command_line.h"

const char usage_info[] = "\
   usage: cassini [OPTIONS] -l -> list all tasks\n\
      or: cassini [OPTIONS]    -> same\n\
      or: cassini [OPTIONS] -q -> terminate the daemon\n\
      or: cassini [OPTIONS] -c [-m MINUTES] [-H HOURS] [-d DAYSOFWEEK] COMMAND_NAME [ARG_1] ... [ARG_N]\n\
          -> add a new task and print its TASKID\n\
             format & semantics of the \"timing\" fields defined here:\n\
             https://pubs.opengroup.org/onlinepubs/9699919799/utilities/crontab.html\n\
             default value for each field is \"*\"\n\
      or: cassini [OPTIONS] -r TASKID -> remove a task\n\
      or: cassini [OPTIONS] -x TASKID -> get info (time + exit code) on all the past runs of a task\n\
      or: cassini [OPTIONS] -o TASKID -> get the standard output of the last run of a task\n\
      or: cassini [OPTIONS] -e TASKID -> get the standard error\n\
      or: cassini -h -> display this message\n\
\n\
   options:\n\
     -p PIPES_DIR -> look for the pipes in PIPES_DIR (default: /tmp/<USERNAME>/saturnd/pipes)\n\
";

int main(int argc, char * argv[]) {
  errno = 0;

  char * minutes_str = "*";
  char * hours_str = "*";
  char * daysofweek_str = "*";
  char * pipes_directory = NULL;

  uint16_t operation = CLIENT_REQUEST_LIST_TASKS;
  uint64_t taskid;

  int opt;
  char * strtoull_endp;
  while ((opt = getopt(argc, argv, "hlcqm:H:d:p:r:x:o:e:")) != -1) {
    switch (opt) {
    case 'm':
      minutes_str = optarg;
      break;
    case 'H':
      hours_str = optarg;
      break;
    case 'd':
      daysofweek_str = optarg;
      break;
    case 'p':
      pipes_directory = strdup(optarg);
      if (pipes_directory == NULL) goto error;
      break;
    case 'l':
      operation = CLIENT_REQUEST_LIST_TASKS;
      break;
    case 'c':
      operation = CLIENT_REQUEST_CREATE_TASK;
      break;
    case 'q':
      operation = CLIENT_REQUEST_TERMINATE;
      break;
    case 'r':
      operation = CLIENT_REQUEST_REMOVE_TASK;
      taskid = strtoull(optarg, &strtoull_endp, 10);
      if (strtoull_endp == optarg || strtoull_endp[0] != '\0') goto error;
      break;
    case 'x':
      operation = CLIENT_REQUEST_GET_TIMES_AND_EXITCODES;
      taskid = strtoull(optarg, &strtoull_endp, 10);
      if (strtoull_endp == optarg || strtoull_endp[0] != '\0') goto error;
      break;
    case 'o':
      operation = CLIENT_REQUEST_GET_STDOUT;
      taskid = strtoull(optarg, &strtoull_endp, 10);
      if (strtoull_endp == optarg || strtoull_endp[0] != '\0') goto error;
      break;
    case 'e':
      operation = CLIENT_REQUEST_GET_STDERR;
      taskid = strtoull(optarg, &strtoull_endp, 10);
      if (strtoull_endp == optarg || strtoull_endp[0] != '\0') goto error;
      break;
    case 'h':
      printf("%s", usage_info);
      return 0;
    case '?':
      fprintf(stderr, "%s", usage_info);
      goto error;
    }
  }

  // ouverture du pipe des requêtes
  int pipe_req = open("run/pipes/saturnd-request-pipe",O_WRONLY);
  assert(pipe_req >= 0);

  int ret;
  char **new_argv = &argv[optind]; //On initialise new_argv à l'adresse du premier élément de argv qui définit la commande
  int new_argc = argc-optind; //Le nombre d'arguments que possede notre fonction à traiter par le démon

 switch(operation) {
  	case CLIENT_REQUEST_LIST_TASKS:
  		ret = send_ls_req(pipe_req);
  		assert(ret >= 0);
  		break;
  	case CLIENT_REQUEST_CREATE_TASK:
		ret = send_cr_req(pipe_req,minutes_str,hours_str,daysofweek_str,new_argc,new_argv);
		assert(ret >= 0);
  		break;
  	case CLIENT_REQUEST_TERMINATE:
  		ret = send_tm_req(pipe_req);
  		assert(ret >= 0);
  		break;
  	case CLIENT_REQUEST_REMOVE_TASK:
      ret = send_rm_req(pipe_req, taskid);
      assert(ret >= 0);
  		break;
  	case CLIENT_REQUEST_GET_TIMES_AND_EXITCODES:
      ret = send_tx_req(pipe_req, taskid);
      assert(ret >= 0);
  		break;
  	case CLIENT_REQUEST_GET_STDOUT:
		ret = send_stdout_req(pipe_req, taskid);
		assert(ret >= 0);
  		break;
  	case CLIENT_REQUEST_GET_STDERR:
		ret = send_stder_req(pipe_req, taskid);
		assert(ret >= 0);
  		break;
 }

  // ouverture du pipe de réponses
  int pipe_reply = open("run/pipes/saturnd-reply-pipe",O_WRONLY);
  assert(pipe_reply >= 0); 

  return EXIT_SUCCESS;

 error:
  if (errno != 0) perror("main");
  free(pipes_directory);
  pipes_directory = NULL;
  return EXIT_FAILURE;
  }
