#ifndef READ_REQUESTS_H
#define READ_REQUESTS_h

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <endian.h>
#include <string.h>

#include "client-request.h"

//returns -1 if an error occured, 1 if the daemon must terminate, 0 otherwise
int read_request(int fd);

#endif
