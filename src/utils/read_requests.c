#include "utils/read_requests.h"

int read_request(int fd) {
	char buf[PIPE_BUF];
	int ret = read(fd,buf,PIPE_BUF);
	if(ret < 0)
		return -1;
	uint16_t opcode;
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
			//respond to cassini before terminating
			return 1;
	}

	return 0;
}

