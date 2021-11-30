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
	int count = 0;
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



		uint16_t REPTYPE;
		uint32_t NBTASKS;

		read(fd, &REPTYPE, sizeof(REPTYPE));
		read(fd, &NBTASKS, sizeof(NBTASKS));
		printf("%d,", be16toh(REPTYPE));
		printf("%li", be64toh(NBTASKS));

		for(int i = 0; i < be64toh(NBTASKS); i++){
			uint64_t minutes;
		  uint32_t hours;
		  uint8_t daysofweek;

			read(fd, &minutes, sizeof(minutes));
			read(fd, &hours, sizeof(hours));
			read(fd, &daysofweek, sizeof(daysofweek));

			uint32_t argc;
			read(fd, &argc, sizeof(argc));

			printf("%li\n", be64toh(minutes));
			printf("%i\n", be32toh(hours));
			printf("%i\n", be16toh(daysofweek));

			for(int ag = 0; ag < be32toh(argc); ag++){
				uint32_t length;
				read(fd, &length, sizeof(length));
				char *data;
				read(fd, &data, sizeof(data));
			}
		}
	return 0;
	}
