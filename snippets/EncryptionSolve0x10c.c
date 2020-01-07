#include <stdio.h>

#define false 0
#define true 1

unsigned char code[] = {
	0x20, 0x20, 0x0E, 0x00, 0x65, 0x65, 0x0E, 0x0B, 0x65, 0x20, 0x11, 0x48,
	0x6F, 0x20, 0x4F, 0x54, 0x6F, 0x77, 0x1D, 0x05, 0x20, 0x61, 0x41, 0x0F,
	0x6B, 0x20, 0x04, 0x4F, 0x20, 0x74, 0x00, 0x3D, 0x65, 0x6E, 0x11, 0x06,
	0x74, 0x20, 0x16, 0x55, 0x2C, 0x20, 0x43, 0x55, 0x20, 0x79, 0x4B, 0x1C,
	0x6C, 0x69, 0x1F, 0x49, 0x69, 0x75, 0x0C, 0x1B, 0x20, 0x67, 0x00, 0x06,
	0x62, 0x65, 0x0D, 0x45, 0x20, 0x74, 0x45, 0x10, 0x75, 0x73, 0x3C, 0x53
};
unsigned short code2[] = {
	0x2020, 0x0E00, 0x6565, 0x0E0B, 0x6520, 0x1148,
	0x6F20, 0x4F54, 0x6F77, 0x1D05, 0x2061, 0x410F,
	0x6B20, 0x044F, 0x2074, 0x003D, 0x656E, 0x1106,
	0x7420, 0x1655, 0x2C20, 0x4355, 0x2079, 0x4B1C,
	0x6C69, 0x1F49, 0x6975, 0x0C1B, 0x2067, 0x0006,
	0x6265, 0x0D45, 0x2074, 0x4510, 0x7573, 0x3C53
};
int codeLength = 72;
int codeLength2 = 36;
unsigned char *attempt;

unsigned char icons[] = {
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ',', ' ', '.', ' ',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
	' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '/', ']', '^', '_',
	' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
	'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', ' '
};

void tryToSolve();
void printTry();
void revPrintTry();

int main() {
	printf("Starting\n");
	tryToSolve();
	return false;
}

void tryToSolve() {
	attempt = (unsigned char*)malloc(sizeof(unsigned char) * codeLength);
	for(int d=0;d<codeLength;d++)attempt[d] = code[d];	// Looped copy for testing

	// Algorithm 1 tries to XOR first octet with next octet
/*	for(int c=0;c<codeLength;c++) {
		int shiftBy = c;for(int d=0;d<codeLength;d++) {
			unsigned char one = code[d % codeLength], two;
			two = code[(d+shiftBy) % codeLength];attempt[d] = one ^ two;
		}printTry();printf("\n");
	}*/
	// Algorithm 2 tries to XOR first octet with previous octet
/*	for(int c=0;c<codeLength;c++) {
		int shiftBy = c;for(int d=0;d<codeLength;d++) {
			unsigned char one = code[d % codeLength], two;
			two = code[(d+(codeLength-shiftBy)) % codeLength];attempt[d] = one ^ two;
		}printTry();printf("\n");
	}*/
	// Algorithm 3 tries to XOR first word with next word
/*	for(int d=0;d<codeLength;d+=2) {
		unsigned char one, two;
		one = code[d % codeLength];
		two = code[(d+2) % codeLength];
		attempt[d] = one ^ two;
	}
	printTry();
	printf("\n");*/
	for(int d=0;d<codeLength;d+=4) {
		unsigned char one, two, three, four;
//		one = code[d % codeLength];
//		two = code[(d+1) % codeLength];
//		three = code[(d+2) % codeLength];
//		four = code[(d+3) % codeLength];
//		attempt[d] = two;
//		attempt[d+1] = one;
//		attempt[d+2] = four ^ two;
//		attempt[d+3] = three ^ one;

		int index = d;
		unsigned char *solve = attempt;
		unsigned char *unsolved = code;
		solve[index] = unsolved[(index+1)%72];
		solve[index+1] = unsolved[index%72];
		solve[index+2] = unsolved[(index+3)%72]^unsolved[(index+1)%72];
		solve[index+3] = unsolved[(index+2)%72]^unsolved[index%72];

//		if(d%2 == 0) {
//			attempt[d] = one;
//			attempt[d+1] = two;
//		}
//		else {
//			attempt[d] = one ^ three;
//			attempt[d+1] = two ^ four;
//		}
	}
	revPrintTry();
	printf("\n");
}

void printTry() {
	for(int d=0;d<codeLength;d++)
		printf("%c", icons[attempt[d]]);
}

void revPrintTry() {
	for(int d=codeLength-1;d>-1;d--)
		printf("%c", icons[attempt[d]]);
}
