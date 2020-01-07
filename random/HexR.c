/*	HexR 1.0, Parad0x13 of Keikaku Studios
1.1 -	Added some auto-open functionality
		Added Big Endian, Little Endian reading functionality
		Added some Byte/Word reading functionality
		Added different view modes
		Modularized code a bit
1.2 -	Commented out fread error, was causing things not to load
*/
#include <stdio.h>

#define false 0
#define true 1

#define LITTLE 0
#define BIG 1
#define RAW BIG
int Endianness = LITTLE;

#define BYTE_SIZE 8
#define WORD_SIZE 16
int AtomicSize = WORD_SIZE;

#define HEX_AND_BINARY		100
#define FULL_HEX_ONLY		101
#define PARTIAL_HEX_ONLY	102
#define BINARY_ONLY			103
int viewMode = PARTIAL_HEX_ONLY;

FILE *openFile(char *fileName);
long lSize;
void getDump();
void viewDump();
void logByte(unsigned char byte);
void logWord(unsigned short word);

unsigned char *buffer;
FILE *input;

int main(int argc, char* argv[]) {
	char *fileName;
	printf("--== Parad0x13's HexR reader ==--\n");
	/* Process arguements */
	if(argc > 3 || argc < 2) {
		printf("useage: %s input\n", argv[0]);
		fileName = "file.bin";
		printf("Will attempt to open %s\n\n", fileName);
	}
	else fileName = argv[1];

	input = openFile(fileName);
	if(input == 0)return 0;

	if(Endianness == BIG)printf("Reading as Big Endian\n");
	else printf("Reading as Little Endian\n");
	printf("Values in hexidecimal\n");
	printf("Atomic Element Size: %i\n", AtomicSize);
	printf("\n");

	getDump();
	fclose(input);
	viewDump();
	printf("\n");
}

FILE *openFile(char *fileName) {
	FILE* retVal = fopen(fileName, "r");
	if(retVal == 0) {
		printf("failed to open input file %s\n", fileName);
		return 0;
	}
	return retVal;
}

void getDump() {
	size_t result;
	fseek(input, 0, SEEK_END);
	lSize = ftell(input);
	rewind(input);

	buffer = (unsigned char*)malloc(sizeof(unsigned char) * lSize);
	if(buffer == NULL) {fputs("Memory error", stderr);exit(2);}

	result = fread(buffer, 1, lSize, input);
	//if(result != lSize) {fputs("Reading error", stderr);exit(3);}
}

void viewDump() {
	int index = 0;
	if(AtomicSize == BYTE_SIZE) {
		while(index < lSize) {
			unsigned char byte = 0;
			if(Endianness == BIG)byte = buffer[index];
			else byte = (buffer[index]>>4 | buffer[index]<<4);
			if(viewMode == HEX_AND_BINARY) {
				printf("0x%02X: ", byte);
				logByte(byte);
				printf("\n");
			}
			else if(viewMode == FULL_HEX_ONLY) {
				printf("0x%02X ", byte);
			}
			else if(viewMode == PARTIAL_HEX_ONLY) {
				printf("%02X ", byte);
			}
			else if(viewMode == BINARY_ONLY) {
				logByte(byte);
				printf("\n");
			}
			index++;
		}
	}
	else if(AtomicSize == WORD_SIZE) {
		while(index < lSize) {
			unsigned short word = 0;
			if(Endianness == BIG)word = ((buffer[index]<<8) | (buffer[index+1]));
			else word = ((buffer[index+1]<<8) | (buffer[index]));
			if(viewMode == HEX_AND_BINARY) {
				printf("0x%04X: ", word);
				logWord(word);
				printf("\n");
			}
			else if(viewMode == FULL_HEX_ONLY) {
				printf("0x%04X ", word);
			}
			else if(viewMode == PARTIAL_HEX_ONLY) {
				printf("%04X ", word);
			}
			else if(viewMode == BINARY_ONLY) {
				logWord(word);
				printf("\n");
			}
			index+=2;
		}
	}
}

void logByte(unsigned char byte) {
	for(int d=7;d>=0;d--)printf("%i", (byte&1<<d)?1:0);
}

void logWord(unsigned short word) {
	for(int d=15;d>=0;d--)printf("%i", (word&1<<d)?1:0);
}
