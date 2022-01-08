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

typedef struct Liste Liste;
struct Liste
{
    struct task *premier;
};

int create_task(struct Liste *listTaskHead, struct timing *t, char **cmd, struct task *exec_times);
int delete_task(struct Liste *listTaskHead, int taskid);
void addList(struct Liste *listTaskHead, struct task *task);
void printList(struct Liste *listTaskHead);
struct timing *get_current_timing();

#endif
