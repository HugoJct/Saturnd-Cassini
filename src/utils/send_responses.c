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

int send_tx_response(int fd, int reponse){
        
        int rep;
        switch (reponse)
        {
        case -1:
                uint16_t err = htobe16(SERVER_REPLY_ERROR);
	        uint16_t nf = htobe16(SERVER_REPLY_ERROR_NOT_FOUND);
                char buf[sizeof(err) + sizeof(nf)];
                memmove(buf, err, 2);
                memmove(buf+2, nf, 2);
                rep = write(fd, &buf, 4);
                break;
        default:
        	uint16_t ok = htobe16(SERVER_REPLY_OK);
                rep = write(fd, &ok,2);
                break;
        
        }
        return rep;
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
