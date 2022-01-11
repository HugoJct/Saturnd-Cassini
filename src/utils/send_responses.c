#include "utils/send_responses.h"


int send_ls_response(int fd, struct Liste *listTaskHead){
        
        int16_t reptype = htobe16(SERVER_REPLY_OK);
        int32_t nbtasks = numberTask(listTaskHead);

	write(fd, &reptype, 2);
        write(fd, &nbtasks, 4);

        printf("oui\n");
        struct task *courant = listTaskHead->premier;
        printf("oui 1 \n");

	while (courant != NULL){
	 uint64_t taskId = courant->id;
        printf("oui non ed \n");

         struct timing *timing = courant->exec_times;
	 uint64_t minutes =  timing->minutes;
	 uint32_t hours = timing->hours;
	 uint8_t daysofweek = timing->daysofweek;

	 write(fd, &taskId, sizeof(taskId));
	 write(fd, &minutes, sizeof(minutes));
	 write(fd, &hours, sizeof(hours));
         write(fd, &daysofweek, sizeof(daysofweek));
        printf("oui non edzdzdzd \n");

	 char **cmd = courant->cmd;
        printf("oui non edzdzdzddzzddzsvrf2 \n");

        printf("taille cmd %ld", sizeof(cmd));
	 for(int ag = 0; ag < 1; ag++){
           printf("on entre pour %d\n", ag);
	   uint32_t length = strlen(cmd[ag]);
           printf("taille %d\n", length);
           int rep;
           rep = write(fd, &length, sizeof(length));
           printf("rep %d \n", rep);
           rep = write(fd, &cmd[ag], be32toh(length));
           printf("rep  %d \n", rep);
	}
        printf("oui non \n");

        courant = courant->next;
        } 
        return 0;
}


int send_cr_response(int fd, int task_id){
	char buf[10];
	uint16_t opcode = htobe16(SERVER_REPLY_OK);
	uint64_t id = htobe64(task_id);
	memcpy(buf,&opcode,2);
	memcpy(buf+2,&id,8);

	int ret = write(fd,buf,10);
	if(ret != 10)
		return -1;
        return 0; 
}

int send_rm_response(int fd, int reponse){

        int rep;
	uint16_t ok = htobe16(SERVER_REPLY_OK);
	uint16_t err = htobe16(SERVER_REPLY_ERROR);
        switch (reponse)
        {
        case -1:
                rep = write(fd, &err, 2);
                break;
        default:
                rep = write(fd, &ok,2);
                break;
        
}
        return rep;
}

int send_tx_response(int fd){
        //TODO
        return 0;
}

int send_so_response(int fd){
        //TODO
        return 0;
}

int send_se_response(int fd){
        //TODO
        return 0;
}

int send_tm_response(int fd) {
        uint16_t opcode = htobe16(SERVER_REPLY_OK);
        int ret = write(fd,&opcode,2);
        if(ret != 2)
                return -1; 
        return 0;
}
