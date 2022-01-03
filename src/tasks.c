#include "tasks.h"

int create_task(struct timing *t, char **cmd) {
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
	

	return highest+1;
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
