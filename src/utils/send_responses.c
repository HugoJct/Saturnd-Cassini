#include "utils/send_responses.h"


int send_ls_response(int fd){
        //TODO
        return 0;
}

int send_cr_response(int fd){
        //TODO
        return 0;
}

int send_rm_response(int fd){
        //TODO
        return 0;
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
