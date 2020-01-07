// Vector mathematics by Bryan Tremblay of Keikaku Studios
// V1.1
#ifndef __VECT__
#define __VECT__

#include <stdio.h>
#include <math.h>

struct vect2f {
	vect2f() {}
	vect2f(float _x, float _y) {x = _x;y = _y;}
	double DotProduct(vect2f v) {return x * v.x + y * v.y;}
	vect2f Perpendicular() {return vect2f(-y, x);}
	void print() {printf("(%g, %g)", x, y);}
	void minClamp(vect2f v) {x =  (x < v.x) ? x:v.x;y =  (y < v.y) ? y:v.y;}
	void maxClamp(vect2f v) {x = !(x < v.x) ? x:v.x;y = !(y < v.y) ? y:v.y;}
	// absMinClamp returns a minimized clamp to zero in either direction.
	// E.g. (-10, 11) of (2, 2) will return (-2, 2)
	void absMinClamp(vect2f v) {vect2f _v = v.abs();minClamp(_v);maxClamp(-_v);}
	//void absMaxClamp(vect2f v){};	// Don't know mathematically how to do this
	vect2f MajorAxis() {return (fabs(x)>fabs(y))?vect2f(x / fabs(x), 0):vect2f(0, y / fabs(y));}
	vect2f abs() {return vect2f(fabs(x), fabs(y));}
	vect2f roundUpDown() {
		float xCheck = floor(x) + 0.5f, yCheck = floor(y) + 0.5f;
		float newX, newY;
		if(x<xCheck) newX = xCheck - 0.5f;
		else newX = xCheck + 0.5f;
		if(y<yCheck) newY = yCheck - 0.5f;
		else newY = yCheck + 0.5f;
		return vect2f(newX, newY);
	}
	bool operator == (vect2f v) {return x == v.x && y == v.y;}
	vect2f operator - () {return vect2f(-x, -y);}
	vect2f operator + (vect2f v) {return vect2f(x + v.x, y + v.y);}
	vect2f operator + (double f) {return vect2f(x + f, y + f);}
	vect2f operator += (vect2f v) {x += v.x;y += v.y;}
	vect2f operator += (double f) {x += f;y += f;}
	vect2f operator ++ () {++x;++y;}		// Prefix
	vect2f operator ++ (int) {x++;y++;}		// Postfix
	vect2f operator - (vect2f v) {return vect2f(x - v.x, y - v.y);}
	vect2f operator - (double f) {return vect2f(x - f, y - f);}
	vect2f operator -= (vect2f v) {x -= v.x;y -= v.y;}
	vect2f operator -= (double f) {x -= f;y -= f;}
	vect2f operator -- () {--x;--y;}		// Prefix
	vect2f operator -- (int) {x--;y--;}		// Postfix
	vect2f operator * (vect2f v) {return vect2f(x * v.x, y * v.y);}
	vect2f operator * (double f) {return vect2f(x * f, y * f);}
	vect2f operator / (vect2f v) {return vect2f(x / v.x, y / v.y);}
	vect2f operator / (double f) {return vect2f(x / f, y / f);}
	float x, y;
};

#endif
