#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define initITERATIONS 32
#define ESCAPEBOUNDS 2.0

#define WIDTH 80
#define HEIGHT 20
#define initLXBounds -2
#define initUXBounds 2
#define initLYBounds -1
#define initUYBounds 1

#define OUTBOUNDS 0
#define INBOUNDS 1

void setup();
void render();
void render2();
void render3();
void calculate();
void calculate2();
double screenXtoGraphX(int screenX);
double screenYtoGraphY(int screenY);
bool isPointTrapped(double x, double y);
int isPointTrappedCount(double x, double y);

int array[HEIGHT][WIDTH];
char array2[HEIGHT][WIDTH];
char array3[HEIGHT][WIDTH];

int ITERATIONS=initITERATIONS;
double LXBounds=initLXBounds, UXBounds=initUXBounds, LYBounds=initLYBounds, UYBounds=initUYBounds;
double initCx=-2.5, initCi=.334;

int main() {
	int d;
	for(d=0;d<25;d++) {
		initCx+=0.1;
		setup();
		calculate();
		initCx+=0.1;
		calculate2();
		render3();
	}
}

void setup() {
	int x, y;

	for(y=0;y<HEIGHT;y++) {
		for(x=0;x<WIDTH;x++) {
			array[y][x] = OUTBOUNDS;
			array2[y][x] = 'Z';
			array3[y][x] = 'Z';
		}
	}
}

void calculate() {
	int w, h;
	double x, y;

	for(h=0;h<HEIGHT;h++) {
		for(w=0;w<WIDTH;w++) {
			x = screenXtoGraphX(w);
			y = screenYtoGraphY(h);
			if(isPointTrapped(x, y))
				array[h][w] = INBOUNDS;
			array2[h][w] = isPointTrappedCount(x, y);
		}
	}
}

void calculate2() {
	int w, h;
	double x, y;

	for(h=0;h<HEIGHT;h++) {
		for(w=0;w<WIDTH;w++) {
			x = screenXtoGraphX(w);
			y = screenYtoGraphY(h);
			if(isPointTrapped(x, y))
				array[h][w] = INBOUNDS;
			array3[h][w] = isPointTrappedCount(x, y);
		}
	}
}

void render() {
	int h, w, d;

	for(d=0;d<WIDTH+2;d++)
		printf("-");
	printf("\n");

	for(h=0;h<HEIGHT;h++) {
		printf("|");
		for(w=0;w<WIDTH;w++) {
			if(array[h][w] == OUTBOUNDS)
				printf(" ");
			else if(array[h][w] == INBOUNDS)
				printf("X");
		}
		printf("|");
		printf("\n");
	}

	for(d=0;d<WIDTH+2;d++)
		printf("-");
	printf("\n");
}

void render2() {
	int h, w, d;

	for(d=0;d<WIDTH+2;d++)
		printf("-");
	printf("\n");

	for(h=0;h<HEIGHT;h++) {
		printf("|");
		for(w=0;w<WIDTH;w++) {
			printf("%c", array2[h][w]);
		}
		printf("|");
		printf("\n");
	}

	for(d=0;d<WIDTH+2;d++)
		printf("-");
	printf("\n");
}

void render3() {
	int h, w, d;

	for(d=0;d<WIDTH*2+3;d++)
		printf("-");
	printf("\n");

	for(h=0;h<HEIGHT;h++) {
		printf("|");
		for(w=0;w<WIDTH;w++)
			printf("%c", array2[h][w]);
		printf("|");
		for(w=0;w<WIDTH;w++)
			printf("%c", array3[h][w]);
		printf("|");
		printf("\n");
	}

	for(d=0;d<WIDTH*2+3;d++)
		printf("-");
	printf("\n");
}

double screenXtoGraphX(int screenX) {
	double retVal;

	double range = UXBounds-LXBounds;
	double fragments = range/WIDTH;
	retVal = fragments*(screenX+1)+LXBounds;

	return retVal;
}

double screenYtoGraphY(int screenY) {
	double retVal;

	double range = UYBounds-LYBounds;
	double fragments = range/HEIGHT;
	retVal = fragments*(screenY+1)+LYBounds;

	return retVal;
}

bool isPointTrapped(double x, double y) {
	// Zn+1 = Zn^2+C, where C changes
	int currentIter;
	double distance;
	double Zx=0, Zi=0;
	double Cx=x, Ci=y;

	for(currentIter=0;currentIter<ITERATIONS;currentIter++) {
		double nZx, nZi;
		nZx = Zx*Zx-(Zi*Zi);
		nZi = Zx*Zi+(Zi*Zx);
		Zx = nZx+Cx;
		Zi = nZi+Ci;
		distance = sqrt((Zx*Zx)+(Zi*Zi));
		if(distance>ESCAPEBOUNDS)
			break;
	}

	if(distance<=ESCAPEBOUNDS)
		return 1;
	else
		return 0;
}

int isPointTrappedCount(double x, double y) {
	// Zn+1 = Zn^2+C
	int currentIter;
	double distance;
	double Zx=x, Zi=y;
	double Cx=initCx, Ci=initCi;

	for(currentIter=0;currentIter<ITERATIONS;currentIter++) {
		double nZx, nZi;
		nZx = Zx*Zx-(Zi*Zi);
		nZi = Zx*Zi+(Zi*Zx);
		Zx = nZx+Cx;
		Zi = nZi+Ci;
		distance = sqrt((Zx*Zx)+(Zi*Zi));
		if(distance>ESCAPEBOUNDS)
			break;
	}

	return currentIter+32;
}
