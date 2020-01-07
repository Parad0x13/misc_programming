// This is supposed to show off that sending a pointer to the vector allows faster access than copying the vector every time
#include <iostream>
#include <vector>
#include "ProjectEulerTools.h"

using namespace std;
using namespace PET;

template<class T>
void methodA(vector<T>input) {
	for(T i=T();i<input.size();i++) {
		T a = input[i];
		T b = input[input.size()-1-i];
		for(T ii=T();ii<a;ii++)T c = a*b;
	}
}

template<class T>
void methodB(vector<T> &input) {
	for(T i=T();i<input.size();i++) {
		T a = input[i];
		T b = input[input.size()-1-i];
		for(T ii=T();ii<a;ii++)T c = a*b;
	}
}

int main() {
	vector<unsigned long long>numbers;
	for(unsigned long long d=0;d<50000;d++)numbers.push_back(d);
	Timer *timer = new Timer();

	cout << "Calculating assignment vs pointer speed in functions..." << endl;

	timer->start();
	methodA(numbers);
	timer->stop();
	timer->log("Via copy: ");

	timer->start();
	methodB(numbers);
	timer->stop();
	timer->log("Via pointer: ");

	return false;
}
