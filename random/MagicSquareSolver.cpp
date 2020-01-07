/*	Saw a cool video from Numberphile that described Magic Squares: https://www.youtube.com/watch?v=aOT_bG-vWyg
	I decided I wanted to write a generator for it. So here goes!	*/
//	https://en.wikipedia.org/wiki/Magic_square

#include <iostream>
#include <vector>

using namespace std;

class Matrix {
public:
	Matrix(int _w, int _h = 0) {
		w = _w;
		h = _h;
		if(h == 0)h = w;

		data = new int[w * h]();
	}

	// [TODO] Can I make dynamic getter/setters instead?
	get_value(int x, int y) {
		return data[y * w + x];
	}

	set_value(int x, int y, int val) {
		data[y * w + x] = val;
	}

	int sum_row(int row) {
		int retVal = 0;

		for(int x = 0;x < w;x++)
			retVal += get_value(x, row);

		return retVal;
	}

	int sum_col(int col) {
		int retVal = 0;

		for(int y = 0;y < h;y++)
			retVal += get_value(col, y);

		return retVal;
	}

	// 0 for TL->BR or 1 for BL->TR. Only really valid if w == h
	int sum_diag(int direction) {
		int retVal = 0;

		if(w != h)return 0;

		for(int d = 0;d < w;d++) {
			if(direction == 0)
				retVal += get_value(d, d);
			if(direction == 1)
				retVal += get_value(d, (w - 1) - d);
		}

		return retVal;
	}

	int w, h;

private:
	friend ostream& operator<<(ostream&, Matrix&);

	int* data;	// [TODO] Is a 1D array really all that much faster than vectors?
};

ostream& operator<<(ostream& stream, Matrix& m) {
	for(int y = 0;y < m.h;y++) {
		for(int x = 0;x < m.w;x++)
			stream << m.get_value(x, y) << " ";
		if(y < m.h - 1)stream << endl;
	}

	return stream;
};

class Square {
public:
	Square(int _n) {
		n = _n;
	}

	~Square() {
		//
	}

	bool generate(int goal) {
		cout << "Generating Magic Square solutions for " << goal << endl;

		// [TODO] Add many more methods for generation
		/*	This method is my first attempt at the algorithm and of course is very rough.
			First we find every possible way of adding numbers to get the desired goal.
			Then we assort these equations until one of them actually works. This is very slow	*/
		cout << "Algorithm used will be Bryan_Stupid_Algorithm" << endl;
		vector<vector<int>> nondistinct_equations;

		// [TODO] Rename test to something better
		// [TODO] Maybe make test it's own nx1 Matrix?
		int* test = new int[n];		// [TODO] Eventually delete test
		// We initialize everything to 1 instead of zero, or whatever values are already in memory
		for(int d = 0;d < n;d++)test[d] = 1;

		bool moreIterations = true;
		while(moreIterations) {
			// Calculate the current equation, add to equations if needed
			int sum = 0;
			for(int d = 0;d < n;d++)
				sum += test[d];

			if(sum == goal) {
				vector<int> equation;
				for(int d = 0;d < n;d++)
					equation.push_back(test[d]);
				nondistinct_equations.push_back(equation);
			}

			test[n - 1]++;
			for(int d = n - 1;d >= 0;d--) {
				if(test[d] > goal) {
					// [TODO] Determine if this actually breaks here
					// Although it shouldn't matter since sums can't possibly
					// be correct if one of the values is above goal
					if(d == 0)moreIterations = false;

					test[d] = 1;
					test[d - 1]++;
				}
			}
		}

		cout << "Found " << nondistinct_equations.size() << " non-distinct equations using " << n << " numbers that sum to " << goal << endl;

		// Now we start combining equations to see what works
		// We shouldn't have to worry about rotating the array
		// since each possible combination of digits is cycled
		// This will find all non-distinct solutions
		vector<Matrix> nondistinct_solutions;

		int* indexes = new int[n]();

		moreIterations = true;
		while(moreIterations) {
			// Generate a Matrix with the current selected equations
			Matrix current_equation_matrix(n);
			vector<vector<int>> selected_equations;

			for(int equation_index = 0;equation_index < n;equation_index++)
				selected_equations.push_back(nondistinct_equations[indexes[equation_index]]);

			int y = 0;
			for(vector<int>& equation : selected_equations) {
				int x = 0;
				for(int value : equation) {
					current_equation_matrix.set_value(x, y, value);
					x++;
				}
				y++;
			}

			// Iterate the index up one and cycle if needed
			indexes[n - 1]++;	// [TODO] Eventually delete indexes
			for(int d = n - 1;d >= 0;d--) {
				if(indexes[d] > nondistinct_equations.size() - 1) {
					// [TODO] Determine if this actually breaks here
					if(d == 0)moreIterations = false;

					indexes[d] = 0;
					indexes[d - 1]++;
				}
			}

			// Now we test that equation to see if it is indeed a Magic Square
			bool sumValid = true;

			// Check Horizontals
				// We can skip calculating each equation again since we've already done so

			// Check Verticals
			for(int col = 0;col < current_equation_matrix.w;col++) {
				int sum = current_equation_matrix.sum_col(col);
				if(sum != goal) {
					sumValid = false;
					break;
				}
			}
			if(!sumValid)continue;

			// Check Diagonal 1
			int sum = current_equation_matrix.sum_diag(0);
			if(sum != goal) {
				sumValid = false;
				continue;
			}

			// Check Diagonal 2
			sum = current_equation_matrix.sum_diag(1);
			if(sum != goal) {
				sumValid = false;
				continue;
			}

			nondistinct_solutions.push_back(current_equation_matrix);
		}

		cout << "Found " << nondistinct_solutions.size() << " non-distinct solutions" << endl;

		// Here we need to identify all distinct solutions
		// This includes those that are mirrored or rotated

		// We can also weed out all solutions that repeat a digit more than once

		return true;
	}

private:
	friend ostream& operator<<(ostream&, Square&);

	vector<Matrix> solutions;
	int n;
};

ostream& operator<<(ostream& stream, Square& s) {
	for(Matrix& solution : s.solutions) {
		stream << solution;
		if(&solution != &s.solutions.back())stream << endl << endl;
	}

	return stream;
};

int main() {
	Square square(4);
	square.generate(15);
	//cout << square << endl;

	return false;
}