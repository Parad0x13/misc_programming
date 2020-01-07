#include<stdio.h>
#include<conio.h>
#include<time.h>
#include<dos.h>

#define TRUE 1
#define FALSE 0
#define bX 80
#define bY 25
#define halfX (bX/2)
#define halfY (bY/2)
#define resolution (bX*bY)

void initBoard();
void seed();
int rnd(int range);

void Game_Loop();
void render();
void SaveBoard();
void printBoard();

void GOL_Logic();

char far *Screen_base = (char far*)0xB8000000;

char *board;
char *buffer;
int bufferCount = 0;

void main() {
	seed();
	initBoard();
	Game_Loop();
}

void initBoard() {
	int d;
	board = (char *)malloc(resolution * sizeof(char));
	for(d=0;d<resolution;d++)
		board[d] = rnd(2);
}

void seed() {
	srand((unsigned)time(NULL));
}

int rnd(int range) {
	int retVal;
	retVal = rand()%range;
	return retVal;
}

void Game_Loop() {
	char input;
	do {
		if(kbhit())
			input = getch();
		render();
		GOL_Logic();
	}while(input != 'p');
}

void render() {
	SaveBoard();
	printBoard();
}

void SaveBoard() {
	// I don't know how to do this correctly...
	int d;
	char *target;
	int newBufferSize, oldBufferSize;
	oldBufferSize = bufferCount * resolution * sizeof(char);
	bufferCount++;
	newBufferSize = bufferCount * resolution * sizeof(char);
//	buffer = (char *)realloc(buffer, newBufferSize);
//	target = buffer + oldBufferSize;
//	for(d=0;d<resolution*sizeof(char);d++){
//		*target = board[d];
//		++target;
//	}
}

void printBoard() {
	int d;
	char far *target = Screen_base;
	for(d=0;d<resolution;d++) {
		if(board[d] == FALSE)
			*target = 32;
		else if(board[d] == TRUE)
			*target = 0x58;
		else
			*target = 0x77;
		*(target+1) = LIGHTGRAY;
		++target;
		++target;
	}
}

void GOL_Logic() {
	int x, y, d;
	char *tempBoard = (char *)malloc(resolution * sizeof(char));

	for(y=0;y<bY;y++) {
		for(x=0;x<bX;x++) {
			int friends = 0;
			if(board[(y-1)*bX+(x-1)] == TRUE)
				friends++;
			if(board[(y-1)*bX+x] == TRUE)
				friends++;
			if(board[(y-1)*bX+(x+1)] == TRUE)
				friends++;
			if(board[y*bX+(x-1)] == TRUE)
				friends++;
			if(board[y*bX+(x+1)] == TRUE)
				friends++;
			if(board[(y+1)*bX+(x-1)] == TRUE)
				friends++;
			if(board[(y+1)*bX+x] == TRUE)
				friends++;
			if(board[(y+1)*bX+(x+1)] == TRUE)
				friends++;

			if(board[y*bX+x] == TRUE) {
				if(friends < 2)
					tempBoard[y*bX+x] = FALSE;
				if(friends == 2 || friends == 3)
					tempBoard[y*bX+x] = TRUE;
				if(friends > 3)
					tempBoard[y*bX+x] = FALSE;
			}
			if(board[y*bX+x] == FALSE) {
				if(friends == 3)
					tempBoard[y*bX+x] = TRUE;
				else
					tempBoard[y*bX+x] = FALSE;
			}
		}
	}

	memcpy(board, tempBoard, resolution);
	free(tempBoard);
}
