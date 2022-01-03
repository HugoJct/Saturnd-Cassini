#ifndef TASKS_H
#define TASKS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include "utils/command_line.h"
#include "timing.h"

struct task {
	int id;
	char **cmd;
	struct timing *exec_times;
};

int create_task(struct timing *t, char **cmd, struct task *task);
struct timing *get_current_timing();
int task_should_run(struct task *ta);

#endif
