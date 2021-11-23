#include "utils/custom_string.h"

int create_custom_string(struct custom_string *dest, char *str) {
	uint32_t len = strlen(str);					//get str length
	if(len <= 0)							//check for error
		return -1;
	dest->length = htobe32(len);					//write length into struct
	dest->data = malloc(len);						//write the bytes of the char array
	assert(dest->data != NULL);
	strncpy(dest->data,str,len);

	return 0;
}

int format_from_string(char *dest, struct custom_string *str) {
	memmove(dest,&str->length,sizeof(uint32_t));			//write the length of the string
	if(dest == NULL)						//check for error
		return -1;
	strncpy(dest+4,str->data,str->length);					//write the string data 
	if(dest+4 == NULL)						//check for error
		return -1;
	return 0;
}
