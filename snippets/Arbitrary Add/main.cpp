#include <iostream>
#include <string>
#include <cstdlib>

// 2017.09.13.1936.EST
// Why? I want to learn. I know there are readily available options out there
// 2017.10.08.1531.EST Haven't made much progress though...

using namespace std;

class Number {
public:
	Number(string _digits = "0") {
		digits = _digits;
	}

	~Number() {
		//
	}

	friend Number operator+(const Number &a, const Number &b) {
		size_t lenA = a.digits.size();
		size_t lenB = b.digits.size();
		return Number("666");
	}

	friend ostream& operator<<(ostream& os, const Number& obj);

	string digits;
};

ostream& operator<<(ostream& os, const Number& obj) {
	os << obj.digits;
	return os;
}

int main() {
	Number a("123");
	Number b("456");
	Number c = a + b;
	cout << c << endl;

	return false;
}
