#include "utils/requests.h"
#include "timing-text-io.h"

int send_ls_req(int fd) {
	uint16_t op_code = htobe16(CLIENT_REQUEST_LIST_TASKS);
	int count = write(fd,&op_code,sizeof(op_code));
	return count > 0 ? 0 : -1;	
}

int send_cr_req(int fd, char *minutes, char *hours, char *days, int argc, char **argv) {

	uint16_t op_code = htobe16(CLIENT_REQUEST_CREATE_TASK);

	struct commandline cmd;					//create a commandline and initialize it with the given command
	int err = create_commandline(&cmd,argc,argv);
	assert(err == 0);

	struct timing t;					//create a timing and fill it with the specified fields
	err = timing_from_strings(&t,minutes,hours,days);
	assert(err == 0);

	int buf_size = sizeof(uint16_t)/*op_code*/ + sizeof(uint64_t)/*minutes*/ + sizeof(uint32_t)/*hours*/ + sizeof(uint8_t)/*days*/ + sizeof(uint32_t)/*argc*/;
			
	for(int i=0;i<argc;i++) {
		buf_size += sizeof(uint32_t) + strlen(argv[i]);		//add the size of each string to the buffer size
	}

	char buf[buf_size];
	
	int p = 0;					//write the op code
	memmove(buf,&op_code,2);
	p+=2;

	uint64_t minutesbe = htobe64(t.minutes);	//write the minutes field
	memmove(buf+p,&minutesbe,8);
	p+= 8;

	uint32_t hoursbe = htobe32(t.hours);		//write the hours field
	memmove(buf+p,&hoursbe,4);
	p+=4;

	memmove(buf+p,&t.daysofweek,1);			//write the days field
	p+=1;

	uint32_t arg_count = htobe32(argc);		//write the argc field
	memmove(buf+p,&arg_count,4);
	p+=4;

	for(int i=0;i<argc;i++) {			//write the string fields
		p += format_from_string(buf+p,cmd.ARGV[i]);		//for each string in cmd write it in buf and add the size to p
		free(cmd.ARGV[i]->data);				//free the string data
		free(cmd.ARGV[i]);					//free the string	
	}
	free(cmd.ARGV);							//free the string array

	int count = write(fd,buf,p);					//write the buffer

	return count > 0 ? 0 : -1;
}

int send_tm_req(int fd) {
	uint16_t op_code = htobe16(CLIENT_REQUEST_TERMINATE);
	int count = write(fd,&op_code,sizeof(op_code));
	return count > 0 ? 0 : -1;	
}
