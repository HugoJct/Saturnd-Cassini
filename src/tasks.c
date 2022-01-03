#include "tasks.h"

int create_task(struct task *listTaskHead, struct timing *t, char **cmd, struct task *task) {
	int highest = -1;
		
	/* This is used to get the highest tasks ID available */
	/*		########################### 		*/
	DIR *dir = opendir("./tasks/");
	struct dirent *d;

	while((d = readdir(dir)) != NULL) {
		if(atoi(d->d_name) > highest)
			highest = atoi(d->d_name);
	}
	closedir(dir);
	/*		########################### 		*/

	//initializing the struct 
	task->id = highest+1;
	task->exec_times = t;
	task->cmd = cmd;
	task->next = NULL;

	char path[strlen("tasks/")+sizeof(int)];
	sprintf(path,"tasks/%d",highest+1);	//formatting directory path
	mkdir(path,0744);			//creating directory	(execution permission is required to create files inside a directory)

	char path_tmp[256];				//tmp char* to hold paths

	sprintf(path_tmp,"%s/cmd",path);		//formatting path
	int cmd_file_fd = open(path_tmp,O_WRONLY | O_CREAT,0644);	//creating file + opening it
	int i = 0;
	while(cmd[i] != NULL) {					//write all cmd args in the file
		dprintf(cmd_file_fd,"%s ",cmd[i]);
		i++;
	}
	close(cmd_file_fd);


	sprintf(path_tmp,"%s/exec_time",path);
	int exec_time_file_fd = open(path_tmp,O_WRONLY | O_CREAT,0644);
	char tmp[13];
	memcpy(tmp,&(t->minutes),8);
	memcpy(tmp+8,&(t->hours),4);
	memcpy(tmp+12,&(t->daysofweek),1);
	write(exec_time_file_fd,tmp,13);
	close(exec_time_file_fd);

	//only creating the files needed for later
	sprintf(path_tmp,"%s/stdout",path);
	int stdout_file_fd = open(path_tmp,O_RDONLY | O_CREAT,0644);
	close(stdout_file_fd);

	sprintf(path_tmp,"%s/stderr",path);
	int stderr_file_fd = open(path_tmp,O_RDONLY | O_CREAT,0644);
	close(stderr_file_fd);

	sprintf(path_tmp,"%s/times_exit-code",path);
	int te_file_fd = open(path_tmp,O_RDONLY | O_CREAT,0644);
	close(te_file_fd);
	

	if(listTaskHead == NULL){
		//si la liste est vide alors on l'ajoute en premier
		listTaskHead = task;
	}else{
		struct task *courant;
		while (courant->next != NULL)
		{
			struct task *targetTask = courant->next;

			//je sais que la condition n'est pas bonne mais c l'idée
			if(task->exec_times >  targetTask->exec_times ){
				
				//si le temps de notre task est supp à la task courante alors 
				//replace les elements
				courant->next = task;
				task->next = nextTask;
				break;
			}
			courant = courant->next;
		}
		courant->next = task;

	}

	return highest+1;
}

int delete_task(struct task *listTaskHead, int taskId){

	if(listTaskHead == NULL){
		return -1; // la liste est vide
	}else{
		struct task *courant;
		while (courant->next != NULL)
		{
			struct task *targetTask = courant->next;

			if(targetTask->id == taskId){
				courant->next = targetTask->next;
					//suprimer le fichier
					int rem = remove(strlen("tasks/") + strlen(targetTask->id));
					assert(rem >= 0);
					free(targetTask);
					return 0; // trouvé
			}

			//on avance
			courant = courant->next;
		}

	}
	return -1;
}


struct timing *get_current_timing() {

	int ti = time(NULL);
	int day = (ti / 86400) % 7 - 3;
	int hour = (ti / 3600) % 24 + 1;
	int minute = (ti / 60) % 60;

	uint8_t day_b = 0x00 | (0x01 << day);

	uint32_t hours_b = 0x00000000 | (0x00000001 << hour);
	hours_b = htobe32(hours_b);

	uint64_t minutes_b = 0x000000000000000 | (0x0000000000000001 << minute);
	minutes_b = htobe64(minutes_b);

	struct timing *t = malloc(sizeof(struct timing));
	t->minutes = minutes_b;
	t->hours = hours_b;
	t->daysofweek = day_b;

	return t;
}

int task_should_run(struct task *ta) {
	struct timing *now = get_current_timing();
	int boolean = 0;

	//Testing minutes
	if((ta->exec_times->minutes & now->minutes) == 0)
		boolean = 0;
	else 
		boolean = 1;	

	//Testing hours
	if((ta->exec_times->hours & now->hours) == 0)
		boolean = 0;
	else
		boolean = 1;	
	
	//Testing days
	if((ta->exec_times->daysofweek & now->daysofweek) == 0)
		boolean = 0;
	else
		boolean = 1;	
	
	free(now);
	return boolean;
}
