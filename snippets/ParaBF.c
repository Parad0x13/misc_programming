// ParaBF v1.0 Bryan A. Tremblay 02Dec2016
// Simple non-elegant BF Interpreter
// Several intended features are missing (logging cells for larger cell count), etc...
// I'm not really even sure if this works properly. Needs future testing.

// Some resources:
//	https://en.wikipedia.org/wiki/Brainfuck
//	http://www.muppetlabs.com/~breadbox/bf/
//	http://www.hevanet.com/cristofd/brainfuck/
//	http://stackoverflow.com/questions/4995624/source-for-simple-programs-in-brainfuck

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CELL_SIZE 20
char *cells;
char point = 0;

void view() {
	int d;
	for(d=0;d<CELL_SIZE;d++) {
		if(d == point) {
			printf("[%2X]", cells[d]);
		}
		else {
			printf(" %2X ", cells[d]);
		}
	}
	printf("\n");
}

int scopePairLocation(char *program, int instructionIndex) {
	// [TODO] Make this nicer, don't do 1000.
	static int openCount = 0;
	static int open[1000];
	static int close[1000];
	static int init = 0;
	if(!init) {
		int d;
		for(d = 0;d < strlen(program);d++) {
			if(program[d] == '[') {
				// Found open
				open[openCount] = d;

				int dd;
				int parity = 0;
				for(dd = d;dd < strlen(program);dd++) {
					if(program[dd] == '[') {
						parity++;
					}
					if(program[dd] == ']') {
						parity--;
						if(parity == 0) {
							// Found close
							close[openCount] = dd;
							break;
						}
					}
				}
				openCount++;
			}
		}
		//for(d = 0;d < openCount;d++) {printf("Found pairs at %i, %i\n", open[d], close[d]);}
		init = 1;
	}

	int d;
	int location = 0;
	for(d = 0;d < instructionIndex;d++) {
		if(program[d] == '[') {
			location++;
		}
	}

	if(program[instructionIndex] == '[') {
		return close[location - 1];
	}
	if(program[instructionIndex] == ']') {
		return open[location - 1];
	}

	return -1;
}

void execute(char *program, int debug) {
	printf("Running program:\n%s\n", program);

	int d;
	for(d=0;d<strlen(program);d++) {
		char command = program[d];

		if(command == '>') {
			point++;
		}
		if(command == '<') {
			point--;
		}
		if(command == '+') {
			cells[point]++;
		}
		if(command == '-') {
			cells[point]--;
		}
		if(command == '.') {
			putchar(cells[point]);
		}
		if(command == ',') {
			cells[point] = getchar();
		}
		if(command == '[') {
			if(cells[point] == 0) {
				// Skip to next scope
				int partner = scopePairLocation(program, d);
				d = partner;
			}
			else {
				// Continue forward
			}
		}
		if(command == ']') {
			if(cells[point] == 0) {
				// Continue forward
			}
			else {
				// Move back to beginning of scope
				int partner = scopePairLocation(program, d);
				d = partner;
			}
		}
		if(debug) {
			printf("%c ", program[d]);
			view();
			getchar();
		}
	}
}

int main() {
	char *simpleIndexAddition = "++ > +++++ [ <+ >- ] < .";
	char *helloWorld = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.";
	char *screenClear = "++++++++++[>++++++++++>+<<-]>[>.<-]";

	cells = (char *)malloc(CELL_SIZE);
	execute(helloWorld, 0);

	return 0;
}
