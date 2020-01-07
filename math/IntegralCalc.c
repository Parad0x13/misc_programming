// I don't remember writing this, but it's possible I did
#include<stdio.h>
#include<conio.h>
#include<math.h>

float m, b;
float Xmax, deltaX;

void initEquation() {
	// y = mx+b
	m = 0.0f;
	b = 0.0f;

	Xmax = 2.0f;
}

float runEquationOn(float currentX) {
	// 7x-8.5x^2+3x^3
	return 7*currentX - 8.5f*pow(currentX, 2) + 3*pow(currentX, 3);
}

float sumForNOf(int currentN) {
	float sum = 0, i;

	deltaX = Xmax/(float)currentN;

	for(i=1;i<currentN+1;i++)
		sum+=runEquationOn(i*deltaX-deltaX/2)*deltaX;

	return sum;
}

char equation[] = {
	"sig(i=1, N, f(i)*delta(X))"
};

void main() {
	int n = 1;
	float sum = 0;

	clrscr();
	initEquation();
	printf("This program will calculate the approximated integral of the equation:\n\t%s\n", equation);
	printf("Function will be in y = mx+b format with:\n\tf(y) = %.1fx+%.1f\n\n", m, b);

	printf("Calculating...\n");
	while(!kbhit()) {
		sum = sumForNOf(n);
		n++;
		if(n%10 == 0)
			printf("With N=%i, the current estimated sum comes to: %f\n", n, sum);
	}
}
