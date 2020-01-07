#ifndef COMPLEXMATH_H
#define COMPLEXMATH_H

struct ivar
{
	float real;
	float immaginary;
}z, c, n;		// ivars z, c, and n are common Complex Numbers for fractals so I added them here

using namespace std;

ivar iadd(ivar i1, ivar i2)		// Adds two complex numbers
{
	ivar answer;
	answer.real=i1.real+i2.real;
	answer.immaginary=i1.immaginary+i2.immaginary;
	return answer;
}

ivar imul(ivar i1, ivar i2)		// Multiplies an complex number
{
	ivar answer;
	answer.real=(i1.real*i2.real)-(i1.immaginary*i2.immaginary);
	answer.immaginary=(i1.real*i2.immaginary)+(i1.immaginary*i2.real);
	return answer;
}

ivar isquare(ivar i1, int power)		// Multiplies an complex number by a power
{
	int d;
	ivar answer, tempanswer;
	tempanswer.real=i1.real;
	tempanswer.immaginary=i1.immaginary;
	for(d=0;d<power-1;d++)
	{
		answer.real=(tempanswer.real*i1.real)-(tempanswer.immaginary*i1.immaginary);
		answer.immaginary=(tempanswer.real*i1.immaginary)+(tempanswer.immaginary*i1.real);
		tempanswer.real+=answer.real;
		tempanswer.immaginary+=answer.immaginary;
	}
	return answer;
}

ivar iequal(ivar i1, ivar i2)		// Makes i1 equal to i2
{
	i1.real=i2.real;
	i1.immaginary=i2.immaginary;
	return i1;
}

ivar ikill(ivar i1)		// Makes i1 equal to 0.0f
{
	i1.real=0.0f;
	i1.immaginary=0.0f;
	return i1;
}

ivar iscreen(float maxx, float minx, float maxi, float mini, int screenx, int screeny, int x, int y)
{
	ivar answer;
	answer.real=((maxx-minx)/screenx)*x+minx;
	answer.immaginary=((maxi-mini)/screeny)*y+mini;
	return answer;
}

ivar iantiscreen(float maxx, float minx, float maxy, float miny, int screenx, int screeny, int sx, int sy)		// sx and sy are screenx and screeny that need to be put into immaginary plane coordinates
{
	ivar answer;	// However the answer will be X, Y coordinates, and not a Complex Number
	answer.real=sx*((maxx-minx)/ScreenX)+minx;
	answer.immaginary=sy*((maxy-miny)/ScreenY)+miny;
//	printf("SX: %i, SY: %i, answer.real: %f, answer.immaginay: %f\n", sx, sy, answer.real, answer.immaginary);
	return(answer);			// answer is NOT a complex number
}

#endif
