#include "utils/read_requests.h"

int read_request(int fd) {
	char buf[PIPE_BUF];
	int ret = read(fd,buf,PIPE_BUF);
	if(ret < 0)
		return -1;
	uint16_t opcode;
	memcpy(&opcode,buf,2);
	opcode = be16toh(opcode);

	char *user = getlogin();
	char res_path[strlen("/tmp/") + strlen(user) + strlen("/saturnd/pipes/saturnd-reply-pipe") + 1];
	sprintf(res_path,"%s%s%s","/tmp/",user,"/saturnd/pipes/saturnd-reply-pipe");

	int res_fd = open(res_path,O_WRONLY);

	switch(opcode) {
		case CLIENT_REQUEST_LIST_TASKS:
			send_ls_response(res_fd);	//not yet written
			break;
		case CLIENT_REQUEST_CREATE_TASK:
			{
				char **cmd = arg_array_from_buf(buf+15);
				send_cr_response(res_fd);	//not yet written
				close(res_fd);

				int pid = fork();
				switch(pid) {
					case 0:
						execvp(cmd[0],cmd);
						break;
					default:
						return pid;	
				}
			}
			break;
		case CLIENT_REQUEST_REMOVE_TASK:
			send_rm_response(res_fd);	//not yet written
			break;
		case CLIENT_REQUEST_GET_TIMES_AND_EXITCODES:
			send_tx_response(res_fd);	//not yet written
			break;
		case CLIENT_REQUEST_GET_STDOUT:
			send_so_response(res_fd);	//not yet written
			break;
		case CLIENT_REQUEST_GET_STDERR:
			send_se_response(res_fd);	//not yet written
			break;
		case CLIENT_REQUEST_TERMINATE:
			send_tm_response(res_fd);
			close(res_fd);
			return 1;
	}

	close(res_fd);
	return 0;
}

