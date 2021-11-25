#ifndef RESPONSES_H
#define RESPONSES_H

/*
 *	Maybe some unused include, juste a copy of requests.h includes here
 */

#include <unistd.h>
#include <endian.h>
#include <stdlib.h>
#include <stdint.h>
#include "utils/custom_string.h"
#include "utils/command_line.h"
#include "timing.h"
#include "timing-text-io.h"
#include <stdio.h>
#include <string.h>

/*
 *	All request functions return 0 on success and -1 on error
 */

int send_rm_resp(int fd);
int send_cr_resp(int fd);
int send_ld_resp(int fd);

#endif