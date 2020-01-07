#include <stdio.h>
#include <string.h>

void zeroize(char *data) {memset(data, 0, sizeof(data));}

void TCP_Client() {
	printf("Acting as TCP Client\n");
}

void TCP_Server() {
	printf("Acting as TCP Server\n");
}

void UDP_Client() {
	printf("Acting as UDP Client\n");
}

void UDP_Server() {
	printf("Acting as UDP Server\n");
}

void main() {
	// Default to TCP client
	TCP_Client();
}
