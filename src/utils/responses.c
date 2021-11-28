#include "utils/responses.h"
#include "timing-text-io.h"

int read_rm_resp(int fd) {
	return -1;
}

int read_cr_resp(int fd) {
	return -1;
}

int read_ls_resp(int fd) {
	return -1;
}

int read_tx_resp(int fd) {
	uint16_t code;
	uint32_t exec_number;
	uint64_t runtime;
	uint16_t exitcode;
	uint16_t error_code; 

	int res = 0;

	res = read(fd,&code,2);
	assert(res == 2);
	code = be16toh(code);
	
	switch(code) {
		case 0x4552:	//ERROR
			res = read(fd,&error_code,2);
			assert(res == 2);
			printf("Error: There is no task with that ID\n");
			goto error;
		case 0x4F4b:	//OK
			break;
	}

	res = read(fd,&exec_number,4);
	assert(res == 4);
	exec_number = be32toh(exec_number);
	printf("This task was executed %d times: \n",exec_number);

	for(int i=0;i<exec_number;i++) {
		res = read(fd,&runtime,8);
		assert(res == 8);
		
		res = read(fd,&exitcode,2);
		assert(res == 2);

		time_t time = be32toh(runtime);
		char buf[25];
		strftime(buf,25,"- %d/%m/%Y @ %H:%M",localtime(&time));
		printf("%s\n",buf);
		printf("\tWith exit code: %d\n",exitcode);

	}

error:
	return 0;
}
