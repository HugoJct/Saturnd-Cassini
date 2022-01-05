#include "utils/send_responses.h"


int send_ls_response(int fd){
        //TODO
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
        switch (reponse)
        {
        case -1:
                rep = write(&fd, sizeof(SERVER_REPLY_ERROR), SERVER_REPLY_ERROR);
                break;
        default:
                rep = write(&fd, sizeof(SERVER_REPLY_OK), SERVER_REPLY_OK);
        break;
        
        return rep;
}
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
