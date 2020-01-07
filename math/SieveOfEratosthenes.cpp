// Can only calculate up to a certin limit without issues of course. By far not a great sieve
#include <iostream>
#include <cmath>
#include <vector>
#include <time.h>

using namespace std;

vector<long long> removeMultiplesOf(vector<long long>values, long long multiple) {
	vector<long long>retVal;
	for(int d=0;d<values.size();d++) {
		int number = values[d];
		if(number <= multiple)retVal.push_back(number);
		else if((long long)number/(long long)multiple != (double)number/(double)multiple)
			retVal.push_back(number);
	}
	return retVal;
}

vector<long long> primesUpToValue(long long value) {
	vector<long long>numbers;
	for(long long d=2;d<=value;d++)numbers.push_back(d);
	for(long long d=2;d<=sqrt(value);d++) {
		numbers = removeMultiplesOf(numbers, d);
	}
	return numbers;
}

int main() {
	int valueMax;
	cout << "Max value to calculate to?: ";
	cin >> valueMax;

	clock_t start = clock();
	vector<long long>primes = primesUpToValue(valueMax);
	cout << "There are " << primes.size() << " primes before the number " << valueMax << endl;

	clock_t stop = clock();
	double delta = (stop-start) / (float)CLOCKS_PER_SEC;
	cout << "Took: " << delta << " seconds to calculate" << endl;
	return false;
}
