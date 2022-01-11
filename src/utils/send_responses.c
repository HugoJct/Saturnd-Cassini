#include "utils/send_responses.h"
#include "server-reply.h"
#include "utils/custom_string.h"

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

int send_so_response(int fd, int fdTask) {
        /* declare variables */
        long numbytes;
        int count;
        uint16_t reptype;
        
        /* Get the number of bytes */
        numbytes = lseek(fdTask, 0L, SEEK_END);
         
        /* reset the file position to 
        the beginning of the file */
        lseek(fdTask, 0L, SEEK_SET);    
         
        /* grab sufficient memory for the 
        buffer to hold the file content */
        char bufferToWrite[numbytes+6]; 
        char bufferOutput[numbytes];
        
        /* OK str to uint16 */
        reptype = htobe16(SERVER_REPLY_OK);
        /* write reptype into response buffer (stdout) */
        memmove(bufferToWrite, &reptype, sizeof(reptype));

        /* read file related to fdTask into bufferOutput */
        read(fdTask, bufferOutput, numbytes);
        
        /* create custom string */
        struct custom_string str;
        create_custom_string(&str, bufferOutput);
        count = format_from_string(bufferToWrite+2, &str);

        assert(count != -1);

        /* copy all the text from task file into response buffer */

        // TODO : écrire le buffer sur le pipe réponse
        count = write(fd, bufferToWrite, sizeof(bufferToWrite));
        close(fd);
        close(fdTask);

        return count > 0 ? 0 : -1;
}

int send_se_response(int fd, char *task_path) {
        printf("%s\n", task_path);
        /* declare variables */
        int count;
        uint16_t reptype;
        uint16_t repErr;
        reptype = htobe16(SERVER_REPLY_ERROR);

        DIR *dir = opendir(task_path);

        /* get stderr path */
        char task_path_err[sizeof(task_path) + strlen("/stderr") + 1];
        sprintf(task_path_err,"%s%s",task_path,"/stderr");

        /* get exitcode path */
        char task_path_ex[sizeof(task_path) + strlen("/exitcode") + 1];
        sprintf(task_path_ex,"%s%s",task_path,"/exitcode");

        if (dir != NULL) {
                /* try to open stderr file */
                int task_fd = open(task_path_err, O_RDONLY);
                /* try to open exitcode file */
                int exit_code_fd = open(task_path_ex, O_RDONLY);
                /* get exitcode file length */
                long numbytes = lseek(exit_code_fd, 0L, SEEK_END);
                lseek(exit_code_fd, 0L, SEEK_SET);
                /* fill exitcode buffer with related values */
                char buff_exit[numbytes];
                read(exit_code_fd, buff_exit, numbytes);

                if (task_fd > -1 && atoi(buff_exit) != 0) {
                        return send_so_response(fd, task_fd); 
                } else {
                        repErr = htobe16(SERVER_REPLY_ERROR_NEVER_RUN);
                }
                close(task_fd);
                close(exit_code_fd);
        } else {
                repErr = htobe16(SERVER_REPLY_ERROR_NOT_FOUND);
        }
        char buffer[sizeof(reptype)+sizeof(repErr)];
        memmove(buffer, &reptype, sizeof(reptype));
        memmove(buffer+sizeof(reptype), &repErr, sizeof(repErr));
        
        printf("%s\n", buffer);

        count = write(fd, buffer, sizeof(buffer));

        close(fd);

        return count > 0 ? 0 : -1;
}

int send_tm_response(int fd) {
        uint16_t opcode = htobe16(SERVER_REPLY_OK);
        int ret = write(fd,&opcode,2);
        if(ret != 2)
                return -1; 
        return 0;
}
