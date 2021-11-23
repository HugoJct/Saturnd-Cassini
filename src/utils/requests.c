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
	uint64_t big_taskid = htobe64(taskid);
	// BUFF
	char buf[sizeof(taskid)+sizeof(op_code)]; // 16/8 + 64/8
	// filling
	memmove(buf, &op_code, sizeof(big_taskid));
	memmove(buf+sizeof(big_taskid), &big_taskid, sizeof(op_code));
	// writing
	int count = write(fd, &buf, sizeof(buf));
	//print buf
	printf("%s\n", buf);
	// return
	return count > 0 ? 0 : -1;
}

/* int send_stder_req(int fd, uint64_t taskid) {
	uint16_t op_code = htobe16(CLIENT_REQUEST_GET_STDER);
	// BUFF

	// filling
	// writing
	// int count = something
	// return
} */