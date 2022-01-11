#include "tasks.h"

int create_task(struct Liste *listTaskHead, struct timing *t, char **cmd, struct task *task) {
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
	task = malloc(sizeof(struct task));
	task->id = highest+1;
	task->exec_times = t;
	task->cmd = cmd;
	task->next = NULL;
	addList(listTaskHead, task);

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
	
	//printList(listTaskHead);

	return highest+1;
}

int delete_task(struct Liste *listTaskHead, int taskId){
	struct task *courant = listTaskHead->premier;

	if(courant == NULL){
		return -1; // la liste est vide
	}else{
		if(courant->id == taskId){
			deleteFileAndRep(taskId);
			listTaskHead->premier=courant->next;
			return 0;
		}

		while (courant->next != NULL)
		{
			struct task *targetTask = courant->next;

			if(targetTask->id == taskId){
					//suprimer le fichier
					deleteFileAndRep(taskId);
					courant->next = targetTask->next;
					return 0; // trouvé
			}
			//on avance
			courant = courant->next;
		} 

	}
	return -1;
}

void deleteFileAndRep(int taskId){
	char buf[strlen("tasks/") + sizeof(int)];
	sprintf(buf, "%s%d", "tasks/", taskId);
	int rem;
	char tmp[17];

	sprintf(tmp, "%s%s", buf, "/cmd");
	rem = unlink(tmp);

	sprintf(tmp, "%s%s", buf, "/exec_time");
	rem = unlink(tmp);
					
	sprintf(tmp, "%s%s", buf, "/stderr");
	rem = unlink(tmp);
					
	sprintf(tmp, "%s%s", buf, "/stdout");
	rem = unlink(tmp);
					
	sprintf(tmp, "%s%s", buf, "/times_exit-code");
	rem = unlink(tmp);

	rem = rmdir(buf); 
	assert(rem >= 0);
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

void from_disk_to_memory(struct Liste *listTaskHead){

	DIR *dir = opendir("./tasks/");
	struct dirent *d;
	
	int fd;
	
	while((d = readdir(dir)) != NULL) {
		struct task *taskDir = malloc(sizeof(struct task));
		int idDir = atoi(d->d_name);
		if(idDir > 0){

			taskDir->id = idDir;
			//Cmd 

			char path[20];
			sprintf(path, "%s/%d/%s", "tasks", idDir,"cmd");

			fd = open(path, O_RDONLY);
			int count = lseek(fd, 0, SEEK_END);

			char buffCmd[count];
			lseek(fd, 0, SEEK_SET);

			read(fd, &buffCmd, count);
			taskDir->cmd = arg_array_from_buf(buffCmd);

			//Exce times

			sprintf(path, "%s/%d/%s", "tasks", idDir,"exec_time");
			fd = open(path,O_RDONLY);
			
			uint64_t minutes;
  			uint32_t hours;
  			uint8_t daysofweek;
			
			read(fd, &minutes, sizeof(minutes));
			read(fd, &hours, sizeof(hours));
			read(fd, &daysofweek, sizeof(daysofweek));
			
			struct timing *exec_times = malloc(sizeof(struct timing));
			exec_times->daysofweek = daysofweek;
			exec_times->hours = hours;
			exec_times->minutes = minutes;
			taskDir->exec_times = exec_times; 
			//ajout à la liste
			addList(listTaskHead, taskDir);
		}
	}
	closedir(dir);
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

void addList(struct Liste *listTaskHead, struct task *task){
		struct task *courant = listTaskHead->premier;

		if(courant == NULL) {
			listTaskHead->premier = task;		
		}else{
			while (courant->next != NULL)courant = courant->next;
			courant->next = task;
		}		
}

void printList(struct Liste *listTaskHead){
	
		struct task *courant = listTaskHead->premier;
		if(courant == NULL){
			printf("Votre liste est vide \n");
			return;
		}

		while (courant != NULL)
		{
			printf("[task %d ] -> ", courant->id);
			courant = courant->next;
		}
		printf("| NULL ] Fin de liste \n");
}
