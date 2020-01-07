// Started 13Oct13 by Bryan Tremblay
/*	Resources: http://en.wikibooks.org/wiki/Efficient_Prime_Number_Generating_Algorithms	*/
// To be honest I don't quite understand the last two algorithms on that page, although I implemented them
/* To Implement:
		Sieve of Eratosthenes
		Sieve of Atkin	*/
#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include <algorithm>

using namespace std;

class Timer {
public:
	Timer() {}
	double delta() {return (stopTick-startTick) / (float)CLOCKS_PER_SEC;}
	void start() {startTick = clock();}
	void stop() {stopTick = clock();}
	void log() {cout << "Took " << delta() << " seconds" << endl;}
	void stopAndLog() {stop();log();}
	clock_t startTick, stopTick;
};

// Brute force everything
vector<unsigned long long> algorithm1_primesUpTo(unsigned long long max) {
	cout << __FUNCTION__ << endl;
	vector<unsigned long long>retVal;

	for(unsigned long long d=2;d<=max;d++) {
		bool isPrime = true;
		for(unsigned long long dd=0;dd<retVal.size();dd++) {
			if(d%retVal[dd] == 0) {
				isPrime = false;
				break;
			}
		}
		if(isPrime)retVal.push_back(d);
	}
	return retVal;
}

// Skips odd numbers
vector<unsigned long long> algorithm2_primesUpTo(unsigned long long max) {
	cout << __FUNCTION__ << endl;
	vector<unsigned long long>retVal;
	retVal.push_back(2);

	for(unsigned long long d=3;d<=max;d+=2) {
		bool isPrime = true;
		for(unsigned long long dd=0;dd<retVal.size();dd++) {
			if(d%retVal[dd] == 0) {
				isPrime = false;
				break;
			}
		}
		if(isPrime)retVal.push_back(d);
	}
	return retVal;
}

// Only checks primes up to sqrt(max)
vector<unsigned long long> algorithm3_primesUpTo(unsigned long long max) {
	cout << __FUNCTION__ << endl;
	vector<unsigned long long>retVal;

	unsigned long long root = floor(sqrt(max));
	for(unsigned long long d=2;d<=max;d++) {
		bool isPrime = true;
		for(unsigned long long dd=0;dd<retVal.size();dd++) {
			if(retVal[dd] > root)break;
			if(d%retVal[dd] == 0) {
				isPrime = false;
				break;
			}
		}
		if(isPrime)retVal.push_back(d);
	}
	return retVal;
}

// Only checks primes up to sqrt(max) and seperates primes into two catagories and skips accordingly
vector<unsigned long long> algorithm4_primesUpTo(unsigned long long max) {
	cout << __FUNCTION__ << endl;

	unsigned long long potentialPrime = 2;
	vector<unsigned long long>exceptedPrimes;
	exceptedPrimes.push_back(potentialPrime);
	potentialPrime++;
	vector<unsigned long long>testPrimes;
	testPrimes.push_back(potentialPrime);
	vector<unsigned long long>skipSet;
	skipSet.push_back(2);
	while(potentialPrime < max) {
		potentialPrime += skipSet[0];
		bool isPrime = true;
		unsigned long long sqrtPotentialPrime = sqrt(potentialPrime);
		for(int d=0;d<testPrimes.size();d++) {
			if(testPrimes[d] > sqrtPotentialPrime)break;
			if(potentialPrime%testPrimes[d] == 0) {
				isPrime = false;
				break;
			}
		}
		if(isPrime)testPrimes.push_back(potentialPrime);
	}
	reverse(exceptedPrimes.begin(), exceptedPrimes.end());
	for(int d=0;d<exceptedPrimes.size();d++)testPrimes.push_back(exceptedPrimes[d]);
	return testPrimes;

	// This seems to actually work:
	// Removed the reverse()
	//for(int d=0;d<testPrimes.size();d++)exceptedPrimes.push_back(testPrimes[d]);
	//return exceptedPrimes;
}

// I'm not even going to pretent like I understand this algorithm...
// Spent so long converting this from pseudo-code to cpp... and it doesn't even work...
vector<unsigned long long> algorithm5_primesUpTo(unsigned long long max) {
	cout << __FUNCTION__ << endl;

	unsigned long long sqrtMax = sqrt(max);
	unsigned long long potentialPrime = 2;
	vector<unsigned long long>skipSet;
	skipSet.push_back(potentialPrime);
	vector<unsigned long long>exceptedPrimes;
	exceptedPrimes.push_back(potentialPrime);
	potentialPrime++;
	vector<unsigned long long>rangeSkipSet;
	rangeSkipSet.push_back(skipSet[0]);
	vector<unsigned long long>testPrimes;
	testPrimes.push_back(potentialPrime);
	unsigned long long currentSkipSetRange = 0;
	rangeSkipSet.push_back(rangeSkipSet[currentSkipSetRange]*testPrimes[0]);
	vector<unsigned long long>extendedPrimes;
	potentialPrime += skipSet[0];
	unsigned long long nextPotentialPrime = potentialPrime;
	testPrimes.push_back(nextPotentialPrime);
	while(nextPotentialPrime < max) {
		currentSkipSetRange++;
		while(nextPotentialPrime < rangeSkipSet[currentSkipSetRange]+1) {
			for(int n=0;n<skipSet.size();n++) {
				nextPotentialPrime = potentialPrime + n;
				if(nextPotentialPrime > max)break;
				if(nextPotentialPrime <= rangeSkipSet[currentSkipSetRange]+1)
					potentialPrime = nextPotentialPrime;
				unsigned long long sqrtNextPotentialPrime = sqrt(nextPotentialPrime);
				bool isPrime = true;
				for(unsigned long long q=0;q<testPrimes.size();q++) {
					if(sqrtNextPotentialPrime < q)break;
					else if(nextPotentialPrime%q == 0) {
						isPrime = false;
						break;
					}
				}
				if(isPrime) {
					if(nextPotentialPrime <= sqrtMax)testPrimes.push_back(nextPotentialPrime);
					else extendedPrimes.push_back(nextPotentialPrime);
				}
			}
			if(nextPotentialPrime > max)break;
		}
		if(nextPotentialPrime > max)break;
		unsigned long long lastRelevantPotentialPrime = potentialPrime;
		vector<unsigned long long>nextSkipSet;
		while(potentialPrime < (rangeSkipSet[currentSkipSetRange]+1)*2-1) {
			for(unsigned long long n=0;n<skipSet.size();n++) {
				nextPotentialPrime = potentialPrime + n;
				if(nextPotentialPrime > max)break;
				unsigned long long sqrtNextPotentialPrime = sqrt(nextPotentialPrime);
				bool isPrime = true;
				for(unsigned long long q=0;q<testPrimes.size();q++) {
					if(sqrtNextPotentialPrime < q)break;
					else if(nextPotentialPrime%q == 0) {
						isPrime = false;
						break;
					}
				}
				if(isPrime) {
					if(nextPotentialPrime <= sqrtMax)
						testPrimes.push_back(nextPotentialPrime);
					else
						extendedPrimes.push_back(nextPotentialPrime);
				}
				if(nextPotentialPrime%testPrimes[0] != 0) {
					nextSkipSet.push_back(nextPotentialPrime - lastRelevantPotentialPrime);
					lastRelevantPotentialPrime = nextPotentialPrime;
				}
			}
			if(nextPotentialPrime > max)break;
		}
		if(nextPotentialPrime > max)break;
		skipSet = nextSkipSet;
		exceptedPrimes.push_back(testPrimes[0]);
		testPrimes.erase(testPrimes.begin(), testPrimes.begin());
		rangeSkipSet.push_back(rangeSkipSet[currentSkipSetRange] * testPrimes[0]);
		nextPotentialPrime = lastRelevantPotentialPrime;
	}
	reverse(exceptedPrimes.begin(), exceptedPrimes.end());
	for(unsigned long long d;d<exceptedPrimes.size();d++)testPrimes.push_back(exceptedPrimes[d]);
	reverse(testPrimes.begin(), testPrimes.end());
	for(unsigned long long d;d<testPrimes.size();d++)extendedPrimes.push_back(testPrimes[d]);
	return extendedPrimes;
}

unsigned long long sumOfPrimes(vector<unsigned long long>primes) {
	unsigned long long retVal = 0;
	for(unsigned int d=0;d<primes.size();d++)
		retVal += primes[d];
	return retVal;
}

void testAlgorithm(vector<unsigned long long>(*algorithm)(unsigned long long), unsigned long long max) {
	Timer *timer = new Timer();
	timer->start();
	vector<unsigned long long>primes = algorithm(max);
	timer->stopAndLog();
	bool calculationVerified = false;
	if(max == 10000 && sumOfPrimes(primes) == 5736396)calculationVerified = true;
	else if(max == 100000 && sumOfPrimes(primes) == 454396537)calculationVerified = true;
	else if(max == 1000000 && sumOfPrimes(primes) == 37550402023)calculationVerified = true;

	if(calculationVerified)cout << "Algorithm verified to calculate correctly" << endl;
	else cout << "Algorithm has not been verified to calculate correctly" << endl;
	cout << endl;
}

int main(int argc, char **argv) {
	unsigned long long max = 1000000;

	//testAlgorithm(algorithm1_primesUpTo, max);
	//testAlgorithm(algorithm2_primesUpTo, max);
	testAlgorithm(algorithm3_primesUpTo, max);
	testAlgorithm(algorithm4_primesUpTo, max);
	//testAlgorithm(algorithm5_primesUpTo, max);

	return false;
}
