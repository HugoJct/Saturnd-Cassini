#include "saturnd.h"

int main(int argc, char **argv) {
	
	//The informations that can be sent with a request
	uint16_t opcode = 0;

	daemonize();

	char *username = getlogin();
	char path[strlen("/tmp/") + strlen(username) + strlen("/saturnd/pipes/saturnd-request-pipe") + 1]; 
	sprintf(path,"%s%s%s","/tmp/",username,"/saturnd/pipes/saturnd-request-pipe");

	int req_fd = open(path,O_RDONLY);
	if(req_fd < 0)
		goto error;

	struct pollfd pfd;
	memset(&pfd,0,sizeof(pfd));
	pfd.fd = req_fd;
	pfd.events = POLLIN;

	while(1) {
		poll(&pfd,1,-1);
		if(pfd.revents & POLLIN) {

			char buf[PIPE_BUF];
			int ret = read(req_fd,buf,PIPE_BUF);
			if(ret < 0)
				goto error;
			memcpy(&opcode,buf,2);	
			opcode = be16toh(opcode);
			switch(opcode) {
				case CLIENT_REQUEST_LIST_TASKS:
					break;
				case CLIENT_REQUEST_CREATE_TASK:
					break;
				case CLIENT_REQUEST_REMOVE_TASK:
					break;
				case CLIENT_REQUEST_GET_TIMES_AND_EXITCODES:
					break;
				case CLIENT_REQUEST_GET_STDOUT:
					break;
				case CLIENT_REQUEST_GET_STDERR:
					break;
				case CLIENT_REQUEST_TERMINATE:
					goto terminate;
			}
		}
	}

terminate: 
	close(req_fd);
	return EXIT_SUCCESS;
	
error:
	close(req_fd);
	return EXIT_FAILURE;
}
