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

int read_stdout_resp(int fd) {
	return -1;
}

int read_stderr_resp(int fd) {

	uint16_t code;
	uint16_t error_code; 
	struct custom_string output;

	int res = 0;
	res = read(fd, &code, 2);
	assert(res == 2);
	
	code = be16toh(code);
	int output_length = be32toh(output.length);

	switch(code) {
		case 0x4552:	//ERROR
			res = read(fd,&error_code,2);
			assert(res == 2);
			eval_error_type(fd, error_code);
			break;
		case 0x4F4b:	//OK
			res = read(fd, &output.length, sizeof(uint32_t));
			assert(res == sizeof(uint32_t));
			output.data = malloc(output_length);
			res = read(fd, output.data, output_length);
			assert(res == output_length);
			print_custom_string(&output);
			break;
	}
	free(output.data);
	return 0;
}

void eval_error_type(int fd, uint16_t error_code) {
	switch(error_code) {
		// two error cases
		case 0x4e46: 
			printf("Error: There is no task with that ID\n");
			break;
		case 0x4e52:
			printf("Error: this task hasn't been executed\n");
			break;
		// OK case 
		case 0x4F4b: break; 
	}
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
