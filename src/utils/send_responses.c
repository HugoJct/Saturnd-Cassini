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

int send_rm_response(int fd){
        //TODO
        return 0;
}

int send_tx_response(int fd){
        //TODO
        return 0;
}

int send_so_se_response(int fd) {
        /* declare a file pointer */
        char    *buffer;
        long    numbytes;
         
        /* Get the number of bytes */
        numbytes = lseek(fd, 0L, SEEK_END);
         
        /* reset the file position to 
        the beginning of the file */
        lseek(fd, 0L, SEEK_SET);    
         
        /* grab sufficient memory for the 
        buffer to hold the file content */
        buffer = (char*)calloc(numbytes, sizeof(char)); 
         
        /* buffer error */
        if(buffer == NULL) {
            return 1;
        }
         
        /* copy all the text into the buffer */
        int res = read(fd, buffer, numbytes);
        
        if (res == -1) {
                
        }

        close(fd);
        
        free(buffer);

        return 0;
}

int send_tm_response(int fd) {
        uint16_t opcode = htobe16(SERVER_REPLY_OK);
        int ret = write(fd,&opcode,2);
        if(ret != 2)
                return -1; 
        return 0;
}
