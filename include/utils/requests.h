#ifndef REQUESTS_H
#define REQUESTS_H

#include "client-request.h"
#include <unistd.h>
#include <endian.h>
#include <stdlib.h>
#include <stdint.h>

int send_ls_req(int fd);	//to list the tasks
int send_tm_req(int fd);	//to terminate the daemon

#endif
