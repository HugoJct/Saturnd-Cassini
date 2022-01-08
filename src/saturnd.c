#include "saturnd.h"

int self_pipe[2];

void handler(int sig) {
	write(self_pipe[1],"a",1);	//if a child terminated, write a byte into the self pipe to wake poll() up
}

int main(int argc, char **argv) {
	
	//daemonize();

	char *username = getlogin();
	
	char path[strlen("/tmp/") + strlen(username) + strlen("/saturnd/pipes/saturnd-request-pipe") + 1]; 
	sprintf(path,"%s%s%s","/tmp/",username,"/saturnd/pipes/saturnd-request-pipe");

	int req_fd = open(path,O_RDONLY);		//open the requet pipe
	if(req_fd < 0)
		goto error;

	struct pollfd pfd[2];		//fill the structure needed for poll down below
	
	memset(&pfd[0],0,sizeof(struct pollfd));	//filling for the request pipe
	pfd[0].fd = req_fd;
	pfd[0].events = POLLIN;

	pipe(self_pipe);
	memset(&pfd[1],0,sizeof(struct pollfd));	//creating pollfd for the self pipe
	pfd[1].fd = self_pipe[0];
	pfd[1].events = POLLIN;

	struct sigaction sa;				//creating sigaction to handle childs terminating 
	memset(&sa,0,sizeof(struct sigaction));
	sa.sa_handler = handler;			

	sigaction(SIGCHLD,&sa,NULL);			//applying the sigaction

	while(1) {
		poll(pfd,2,-1);			//wait for the client to write on the pipe

		if(pfd[0].revents & POLLIN) {
			int ret = read_request(req_fd);		//read the request written
			if(ret == -1)				//error detected
				goto error;
			else if (ret == 1)			//daemon kill request detected
				goto terminate;
		}

		if(pfd[1].revents & POLLIN) {		//if a child has terminated
			char tmp[PIPE_BUF];
			read(self_pipe[0],&tmp,PIPE_BUF);	//empty the self pipe
			wait(NULL);							//collect the child status
		}

	}

terminate:					//if the daemon must terminate
	close(self_pipe[0]);
	close(self_pipe[1]);
	close(req_fd);
	printf("terminé");
	return EXIT_SUCCESS;
	
error:						//if an error in encountered
	close(self_pipe[0]);
	close(self_pipe[1]);
	close(req_fd);
	printf("terminé avec failure");

	return EXIT_FAILURE;

}
