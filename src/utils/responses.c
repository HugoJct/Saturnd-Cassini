#include "utils/responses.h"
#include "timing-text-io.h"
#include "utils/command_line.h"
#include "timing.h"

int read_rm_resp(int fd) {
	return -1;
}

int read_cr_resp(int fd) {

	int count;
	uint16_t reptype;
	uint64_t taskId;
	char buf[sizeof(reptype) + sizeof(taskId)];
	count = read(fd, buf, sizeof(reptype)+sizeof(taskId));

	if(count != (sizeof(reptype) + sizeof(taskId))){
		return -1; //on a pas tout lu;
		}
	memmove(&reptype, buf, sizeof(reptype));
	memmove(&taskId, buf+sizeof(reptype), sizeof(taskId));
	printf("%ld", be64toh(taskId));
	return count;
}

int read_ls_resp(int fd) {

		uint16_t reptype;
		uint32_t nbtasks;

		read(fd, &reptype, 2);
		read(fd, &nbtasks, 4);

		for(int i = 0; i < be32toh(nbtasks); i++){

			uint64_t taskId;
			uint64_t minutes;
		  uint32_t hours;
		  uint8_t daysofweek;

			read(fd, &taskId, sizeof(taskId));
			read(fd, &minutes, sizeof(minutes));
			read(fd, &hours, sizeof(hours));
			read(fd, &daysofweek, sizeof(daysofweek));

			struct timing time= {.minutes=be64toh(minutes), .hours=be32toh(hours), .daysofweek=daysofweek};
			char buff[1024];

			timing_string_from_timing(buff, &time);

			uint32_t argc;
			read(fd, &argc, sizeof(argc));

			printf("%ld: ", be64toh(taskId));
			printf("%s ", buff);
			for(int ag = 0; ag < be32toh(argc); ag++){
				uint32_t length;
				read(fd, &length, sizeof(length));
				char data[be32toh(length)];
				read(fd, data, be32toh(length));
				printf("%.*s ", be32toh(length), data);
			}
			printf("\n");
	}
	return 0;
	}
