#include "utils/requests.h"

int send_ls_req(int fd) {
	uint16_t op_code = htobe16(CLIENT_REQUEST_LIST_TASKS);
	int count = write(fd,&op_code,sizeof(op_code));
	return count > 0 ? 0 : -1;	
}

int send_tm_req(int fd) {
	uint16_t op_code = htobe16(CLIENT_REQUEST_TERMINATE);
	int count = write(fd,&op_code,sizeof(op_code));
	return count > 0 ? 0 : -1;	
}

int send_stdout_req(int fd, uint64_t taskid) {
	uint16_t op_code = htobe16(CLIENT_REQUEST_GET_STDOUT);
	// BUFF
	// filling
	// writing
	// int count = something
	// return
}