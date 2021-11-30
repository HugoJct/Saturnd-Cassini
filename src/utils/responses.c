#include "utils/responses.h"
#include "timing-text-io.h"
#include "utils/command_line.h"
#include "timing.h"

int read_rm_resp(int fd) {
	return -1;
}

int read_cr_resp(int fd) {

	uint16_t REPTYPE;
	uint64_t TASKID;
	int count;
	count += read(fd, &REPTYPE, sizeof(REPTYPE));
	count += read(fd, &TASKID, sizeof(TASKID));

	if(count != (sizeof(REPTYPE) + sizeof(TASKID))){
		return -1; //on a pas tout lu;
	}

	printf("%d,", be16toh(REPTYPE));
	printf("%ld", be64toh(TASKID));
	return count;
}

int read_ls_resp(int fd) {
	return 0;
	}
