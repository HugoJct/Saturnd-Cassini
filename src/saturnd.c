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

	struct pollfd pfd;		//fill the structure needed for poll down below
	memset(&pfd,0,sizeof(pfd));
	pfd.fd = req_fd;
	pfd.events = POLLIN;

	while(1) {
		poll(&pfd,1,-1);			//wait for the client to write on the pipe
		if(pfd.revents & POLLIN) {
			int ret = read_request(req_fd);		//read the request written
			if(ret == -1)				//error detected
				goto error;
			else if (ret == 1)			//daemon kill request detected
				goto terminate;
		}
	}

terminate:	//if the daemon must terminate
	close(req_fd);
	return EXIT_SUCCESS;
	
error:		//if an error in encountered
	close(req_fd);
	return EXIT_FAILURE;
}
