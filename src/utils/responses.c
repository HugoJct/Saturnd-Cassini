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

	res = read(fd,&code,2);		//read the response type
	assert(res == 2);
	code = be16toh(code);		//convert to the host endianness
	
	switch(code) {
		case 0x4552:	//ERROR
			res = read(fd,&error_code,2);				//empty the pipe
			assert(res == 2);
			printf("Error: There is no task with that ID\n");	//display an error message
			goto error;
		case 0x4F4b:	//OK
			break;
	}

	//If the task exists
	res = read(fd,&exec_number,4);		//read the number of time it was ran
	assert(res == 4);
	exec_number = be32toh(exec_number);
	printf("This task was executed %d times: \n",exec_number);	//print an output

	for(int i=0;i<exec_number;i++) {		//for each task
		res = read(fd,&runtime,8);		//read the time it was ran at
		assert(res == 8);
		
		res = read(fd,&exitcode,2);		//read its exitcode
		assert(res == 2);

		time_t time = be32toh(runtime);		
		char buf[25];
		strftime(buf,25,"- %d/%m/%Y @ %H:%M",localtime(&time));		//format the runtime to something readable
		printf("%s\n",buf);						
		printf("\tWith exit code: %d\n",exitcode);			

	}

error:
	return 0;
}
