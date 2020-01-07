/*	2017.07.28.1643.EST
	The idea here is to try and write a memory-safe trim and tokenizer in ANSI C.
	I got bored after a few minutes of toying with it... incomplete */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Tokens {
	char **data;
	size_t count;
} Tokens;

void freeTokens(Tokens *tokens) {
	int d;
	for(d = 0;d < tokens->count;d++)
		free(tokens->data[d]);
	free(tokens->data);
	free(tokens);
	tokens = NULL;
}

/* ANSI C doensn't support strdup by default, therefor we make our own */
/* Feel free to remove this and use regular strdup if you wan */
char *custom_strdup(const char *data) {
	char *retVal = calloc(strlen(data) + 1, sizeof(char));
	strncpy(retVal, data, strlen(data));
	return retVal;
}

char *trim(char *data) {
	char *begin = data;
	char *end = data + strlen(data);

	while(isspace(*begin) && begin < end) begin++;
	printf("%x\n", *begin);
	printf("%d\n", isspace(*begin));
	while(isspace(*end) && end > begin) end--;

	unsigned int size = end - begin;
	memmove(data, begin, size);
	data[size] = 0;

	return data;
}

Tokens *tokens(const char *data, const char *delims) {
	Tokens *retVal = calloc(1, sizeof(Tokens));
	char *tmp = custom_strdup(data);

	char *token = strtok(tmp, delims);
	while(token != NULL) {
		retVal->data = realloc(retVal->data, (retVal->count + 1) * sizeof(char*));
		retVal->data[retVal->count++] = custom_strdup(trim(token));
		token = strtok(NULL, delims);
	}

	free(tmp);
	return retVal;
}

void main() {
	Tokens *stuff = tokens(" a ,		     b  	   , 		 c    		", ",");
	int d;
	for(d = 0;d < stuff->count;d++)
		printf("Token[%d]: |%s|\n", d, stuff->data[d]);

	freeTokens(stuff);
}
