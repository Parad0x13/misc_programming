// Attempt to implement a more advanced primality test but it won't work for larger values...
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "ProjectEulerTools.h"

using namespace std;
using namespace PET;

template<class T>
T fasterMod(T factor, T power, T modulus) {
	T result = 1;
	while(power > 0) {
		if(power % 2 == 1) {
			result = (result * factor) % modulus;
			power--;
		}
		power /= 2;
		factor = (factor * factor) % modulus;
	}
	return result;
}

template<class T>
bool primalityTest(T p) {
	int repeatCount = 100;
	for(int d=0;d<repeatCount;d++) {
		T a = rand()%(p-1) + 1;
		//if(GCD(a, p) == 1) {
			///if(power(a, input-1)%input == 1)return true;
			///if(power(a, p-1)%p == 1){}
			//if(fasterMod<long>(a, p-1, p)){}
			//else return false;
		//}
		//else return false;
		if(GCD(a, p) != 1)return false;
		if(fasterMod<T>(a, p-1, p) != 1)return false;
	}
	return true;
}

int main() {
	srand(time(0));

	unsigned long long primeMax = 100000000;

	vector<unsigned long long>testPrimes;
	for(unsigned long long d=2;d<=primeMax;d++) {
		if(primalityTest(d))
			testPrimes.push_back(d);
	}
	//log(testPrimes);
	cout << sum(testPrimes) << endl;

	vector<unsigned long long>actualPrimes = primesUpTo<unsigned long long>(primeMax);
	//log(actualPrimes);
	cout << sum(actualPrimes) << endl;

	return false;
}
