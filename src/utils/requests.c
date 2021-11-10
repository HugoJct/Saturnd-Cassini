#include "utils/requests.h"

int send_ls_req(int fd) {
	uint16_t op_code = htobe16(CLIENT_REQUEST_LIST_TASKS);
	int count = write(fd,&op_code,sizeof(op_code));
	return count;
}
