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
#include "timing-text-io.h"

struct task {
	int id;
	char **cmd;
	struct timing *exec_times;
	struct task *next;
	
};

int create_task(struct task *listTaskHead, struct timing *t, char **cmd, struct task *exec_times);
int delete_task(struct task *listTaskHead, int taskid);
void addListByTiming(struct task *listTaskHead, struct task *task);
void printList(struct task *listTaskHead);
struct timing *get_current_timing();

#endif
