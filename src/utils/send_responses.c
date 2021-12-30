#include "utils/send_responses.h"

int send_tm_response(int fd) {
	uint16_t opcode = htobe16(SERVER_REPLY_OK);
	int ret = write(fd,&opcode,2);
	if(ret != 2)
		return -1;
	return 0;
}
