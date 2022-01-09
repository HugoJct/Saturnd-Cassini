#ifndef SEND_RESPONSES_H
#define SEND_RESPONSES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "timing.h"
#include "timing-text-io.h"
#include "tasks.h"
#include "command_line.h"
#include "server-reply.h"

int send_ls_response(int fd, struct Liste *listTaskHead);
int send_cr_response(int fd, int task_id);
int send_rm_response(int fd, int response);
int send_tx_response(int fd);
int send_so_response(int fd);
int send_se_response(int fd);
int send_tm_response(int fd);

#endif
