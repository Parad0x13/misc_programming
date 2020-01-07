// [TODO] Add floating point random generation
// [TODO] Ensure this is actually properly random
// [TODO] Use CPP's newer random functions, not rand from ctime

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
	srand(time(0));
	cout << "Random: " << rand()%100 << endl;
	return false;
}
