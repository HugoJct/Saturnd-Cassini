#include "saturnd.h"

int main(int argc, char **argv) {
	
	printf("Saturnd launched\n");
	daemonize();

	pause();

	return EXIT_SUCCESS;
}


void daemonize(void) {
	
	switch(fork()) {			//fork to create the daemon
		case -1:	//error
			perror("Fork");
			break;
		case 0:		//child
			break;
		default:	//parent
			exit(0);		//let the parent terminate
			break;
	}
	setsid();				//create a session for the process

	int pid = getpid();			//write the process' pid in a file to be able to identify easily
	int pidFile = open("saturnd.pid",O_WRONLY | O_TRUNC | O_CREAT, 0644);
	dprintf(pidFile,"%d",pid);
	close(pidFile);
}
