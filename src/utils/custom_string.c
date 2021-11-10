#include "utils/custom_string.h"

int create_custom_string(struct custom_string *dest, char *str) {
	int len = strlen(str);
	if(len <= 0)
		return -1;
	dest->length = len;
	strcpy(dest->bytes,str);
	return 0;
}
