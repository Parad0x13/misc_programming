// Bryan Tremblay, Keikaku Studios

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

void runWithValues(int theNumber, int theN, bool verbose);
void recurse(int correctNumber, int minRange, int maxRange, bool verbose);
void runWithRange(int maxN);

int number, N, count;

int main() {
	printf("This program will recursivly guess your number(within range 1->N) until it has reached it\n");
	printf("At max recursion the value will be proportional to log2(N) floored unless N is a power of 2\n");
	printf("If N is a power of 2 the max recursive value will be one less than the actual maximum required guesses\n\n");

	printf("Enter a number to Guess up to: ");
	scanf("%i", &N);
	printf("Enter the number to Guess at: ");
	scanf("%i", &number);

	runWithValues(number, N, true);

	char choice;
	printf("Run Range-Check? Y/N: ");
	scanf("%s", &choice);
	if(choice == 'y')
		runWithRange(100);
}

void runWithValues(int theNumber, int theN, bool verbose) {
	number = theNumber;
	N = theN;
	count = 0;

	recurse(number, 0, N, verbose);
	printf("floor(Log2(%i)) Predicts: %.0f\n", N, floor(log(N)/log(2)));
	printf("\n");
}

void recurse(int correctNumber, int minRange, int maxRange, bool verbose) {
	count++;

	int guess = minRange+ceil((float)(maxRange-minRange)/2);
	int possibilities;

	if(guess > correctNumber)
		possibilities = (guess-1)-minRange;
	if(guess < correctNumber)
		possibilities = (maxRange-1)-guess;

	if(verbose)
		printf("Guess: %i\tminRange: %d\tmaxRange: %d\tGives %i possible choices left\n", guess, minRange, maxRange, possibilities);

	if(possibilities == 1)
		guess = correctNumber;

	if(guess == correctNumber)
		printf("Recursion took: %i tries\n", count);
	else if(guess < correctNumber)
		recurse(correctNumber, guess, maxRange, verbose);
	else if(guess > correctNumber)
		recurse(correctNumber, minRange, guess, verbose);
}

void runWithRange(int maxN) {
	int d;
	for(d=2;d<maxN;d++)
		runWithValues(1, d, false);
}
