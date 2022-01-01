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
				int out = open("saturnd.out",O_WRONLY | O_CREAT | O_TRUNC, 0644);
				int padding = 15;
				
				uint32_t argc;
				memcpy(&argc,buf+padding,4);
				padding += 4;
				argc = be32toh(argc);

				char **argv;
				argv = malloc(sizeof(char*) * argc);

				for(int i=0;i<argc;i++) {
					uint32_t arg_len;
					memcpy(&arg_len,buf+padding,4);
					padding += 4;
					arg_len = be32toh(arg_len);
					
					argv[i] = malloc(arg_len+1);

					char arg_tmp[arg_len];
					memcpy(arg_tmp,buf+padding,arg_len);
					padding += arg_len;
					memcpy(argv[i],arg_tmp,arg_len);
				}

				for(int i=0;i<argc;i++)
					dprintf(out,"%s ",argv[i]);
				dprintf(out,"\n");

				close(out);
			}
			send_cr_response(res_fd);	//not yet written
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

