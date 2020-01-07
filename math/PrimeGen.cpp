// Doesn't work, was a work in progress
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <windows.h>

#define UPPER_LIMIT 100001
#define PASSES 50

FILE *fp;
long long sum;
bool benchmark = false;
bool file_wrote = false;

class Timer {
public:
    Timer() {
		QueryPerformanceFrequency(&frequency);
		startCount.QuadPart = 0;
		endCount.QuadPart = 0;

		stopped = 0;
		startTimeInMicroSec = 0;
		endTimeInMicroSec = 0;
	}
    ~Timer(){};

    void start() {
		stopped = 0;
		QueryPerformanceCounter(&startCount);
	}
    void stop() {
		stopped = 1;
		QueryPerformanceCounter(&endCount);
	}
    double getElapsedTime() {return this->getElapsedTimeInSec();}
    double getElapsedTimeInSec() {return this->getElapsedTimeInMicroSec() * 0.000001;}
    double getElapsedTimeInMilliSec() {return this->getElapsedTimeInMicroSec() * 0.001;}
    double getElapsedTimeInMicroSec() {
		if(!stopped)QueryPerformanceCounter(&endCount);

		startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
		endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);

		return endTimeInMicroSec - startTimeInMicroSec;
	}

    double startTimeInMicroSec;
    double endTimeInMicroSec;
    int stopped;

    LARGE_INTEGER frequency;                    // ticks per second
    LARGE_INTEGER startCount;
    LARGE_INTEGER endCount;
};

double m_ceil(double value)
{
	// Uses the fact that (int)floatVal returns floor of floatVal
	if(value-(int)value > 0)
		return (int)value+1;
	else
		return (int)value;
}

void calculatePrimeNumbers(int numPrimes)
{
	int i = 1;
	int primeNumberCounter = 0;
	bool isPrimeNumber;

	sum = 0;

	// For that one darn Prime that is even
	if(!file_wrote)
		fprintf(fp, "[P1]: 2\n");
	sum += 2;
	primeNumberCounter++;

	while(primeNumberCounter <= numPrimes-1)
	{
		if(!primeNumberCounter%100000)
			printf("100000 More Calculated\n");
		// We can stop calculating after sqrt(i)
		//int i1 = (int)ceil(sqrt((float)i));
		int i1 = (int)m_ceil(sqrt((float)i));
		isPrimeNumber = false;
		
		// i&(1) AND operates by one, gives remainder as if i%2. Any returned value will be greater than 0 so true
		//while(i1 > 1 && i&(1))
		while(i1 > 1)
		{
			if(i!=i1 && i%i1==0)
			//if((i!=i1) && i1*(i/i1)==i)
			{
				isPrimeNumber = false;
				break;
			}
			else if(!isPrimeNumber)
				isPrimeNumber = true;
			i1--;
		}
		if(isPrimeNumber)
		{
			if(!file_wrote)
				fprintf(fp, "[P%i]: %i\n", primeNumberCounter+1, i);
			primeNumberCounter++;
			sum += i;
		}
		i+=2;	// Skips all even numbers, as long as 'i' is initially set to 1
	}
	file_wrote = true;
}

// Does NOT work, needs much editing
void Eratosthenes()
{
	// Allocate memory
	int ArraySize = 100000000;
	//double *s = (double*)malloc(ArraySize * sizeof(double));
	long *s = (long*)malloc(ArraySize*sizeof(long));
	if(s==NULL)exit(1);
	for(int d=0;d<ArraySize;d++)
		s[d] = d;

	int d = 2;
	for(int y=0;y<ArraySize;y++)
	{
		for(int dd=0;d<ArraySize;dd++)
		{
			if(s[dd]%d == 0)
				s[dd] = 0;
		}
		d++;
	}
	free(s);
}

void main()
{
	Timer *timer = new Timer();

	fopen_s(&fp, "PrimeList_Output.txt", "w");
	fprintf_s(fp, "List of first %i Primes\n", UPPER_LIMIT);

	printf("Finding the first %i primes\n", UPPER_LIMIT);
	if(benchmark)
	{
		double average = 0;
		printf("Benchmarking %i passes...\n", PASSES);
		for(int d=0;d<PASSES;d++)
		{
			timer->start();
			calculatePrimeNumbers(UPPER_LIMIT);
			timer->stop();
			average += timer->getElapsedTimeInSec();
			printf("Took %f seconds\n", timer->getElapsedTimeInSec());
			printf("Primes Calculated, sum of primes is %lld\n", sum);
		}
		average/=(float)PASSES;
		printf("Average Took %f seconds\n", average);
	}
	else if(!benchmark)
	{
		timer->start();
		calculatePrimeNumbers(UPPER_LIMIT);
		timer->stop();
		printf("Took %f seconds\n", timer->getElapsedTimeInSec());
		printf("Primes Calculated, sum of primes is %lld\n", sum);
	}

	printf("\n\\\\\\\\\\Testing Stuff//////////\n");
	printf("Size: %i\n", sizeof(_int64));

	double test = 5.3454532f;
	double test2;
	double first;
	printf("Benchmarking Different Ceil functions\n");
	long int iter = 10000000;
	timer->start();
	for(long d=0;d<iter;d++)
		test2 = ceil(test);
	timer->stop();
	first = timer->getElapsedTimeInMilliSec();
	printf("Regular Ceil benchmark took %f milliSeconds\n", timer->getElapsedTimeInMilliSec());
	timer->start();
	for(long d=0;d<iter;d++)
		test2 = m_ceil(test);
	timer->stop();
	printf("My Ceil benchmark took %f milliSeconds\n", timer->getElapsedTimeInMilliSec());
	printf("My Ceil benchmark is %.0fX faster\n", first/timer->getElapsedTimeInMilliSec());

	printf("\n");

	printf("Benchmarking Different Floor functions\n");
	long int iterations = 100000;
	timer->start();
	for(long int d=0;d<iterations;d++)
		test2 = floor(test);
	timer->stop();
	printf("Regular Floor benchmark took %f milliSeconds\n", timer->getElapsedTimeInMilliSec());
	timer->start();
	for(long int d=0;d<iterations;d++)
		test2 = (int)test;
	timer->stop();
	printf("My Floor benchmark took %f milliSeconds\n", timer->getElapsedTimeInMilliSec());

	printf("Benchmarking Different boolean Modulus functions\n");
	int iterz = 100000000;
	int i = 1, i1 = 1;
	timer->start();
	for(int d=0;d<iterz;d++){
		if(i1%i == 0){}i++;i1+=2;}
	timer->stop();
	printf("Regular full modulus took %f milliSeconds\n", timer->getElapsedTimeInMilliSec());
	timer->start();
	for(int d=0;d<iterz;d++){
		if(i*(i1/i) == i1){}i++;i1+=2;}
	timer->stop();
	printf("My boolean modulus took %f milliSeconds\n", timer->getElapsedTimeInMilliSec());

	fclose(fp);
	_getch();
	//delete s;
	//free(s);
}
