#ifndef CUSTOM_STRING_H
#define CUSTOM_STRING_H

#define MAX_COMMAND_LENGTH 256

#include <string.h>
#include <stdint.h>
#include <stdlib.h>

struct custom_string {
	uint32_t length;
	char bytes[MAX_COMMAND_LENGTH];
};

int create_custom_string(struct custom_string *dest, char *str);

#endif
