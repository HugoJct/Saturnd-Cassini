#include "utils/send_responses.h"


int send_ls_response(int fd, struct Liste *listTaskHead){
        
        int16_t reptype = htobe16(SERVER_REPLY_OK);
        int32_t nbtasks = numberTask(listTaskHead);

	write(fd, &reptype, 2);
        write(fd, &nbtasks, 4);

        struct task *courant = listTaskHead->premier;
	while (courant != NULL){
	 uint64_t taskId = courant->id;

         struct timing *timing = courant->exec_times;
	 uint64_t minutes =  timing->minutes;
	 uint32_t hours = timing->hours;
	 uint8_t daysofweek = timing->daysofweek;

	 write(fd, &taskId, sizeof(taskId));
	 write(fd, &minutes, sizeof(minutes));
	 write(fd, &hours, sizeof(hours));
         write(fd, &daysofweek, sizeof(daysofweek));

	 char **cmd = courant->cmd;

	 for(int ag = 0; ag < sizeof(cmd); ag++){
	   uint32_t length = strlen(cmd[ag]);
           write(fd, &length, sizeof(length));
           write(fd, cmd[ag], be32toh(length));
	}

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
