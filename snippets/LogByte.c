#include <stdio.h>

void logByte(unsigned char byte) {
	int d;
	printf("0x%x: ", byte);
	for(d=7;d>=0;d--)printf("%i", (byte&1<<d)?1:0);
}

void main() {
	logByte(0xd0|0x02);
}
