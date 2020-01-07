#include <iostream>
#include <vector>
#include <random>

//#include "EasyBMP\EasyBMP.h"
#include "EasyBMP_minified.h"

#define width		800
#define height		600
#define iterations	1000000

using namespace std;

char board[width * height];

void setup() {
	for(int d = 0;d < width * height;d++) {
		board[d] = '.';
	}
}

struct Point {
	double x, y;
};

vector<Point> sierpinski = {
	{1, 1},
	{width - 1 - 1, 1},
	{1, height - 1 - 1},
};

vector<Point> square = {
	{1, 1},
	{width - 1 - 1, 1},
	{1, height - 1 - 1},
	{width - 1 - 1, height - 1 - 1},
};

int main() {
	BMP bmp;
	bmp.SetSize(width, height);
    bmp.SetBitDepth(24);

	setup();

	vector<Point> points = square;
	Point active = points.front();
	for(int iter = 0;iter < iterations;iter++) {
		int node = rand() % points.size();
		active.x += (points[node].x - active.x) / 2;
		active.y += (points[node].y - active.y) / 2;
		board[width * (int)active.y + (int)active.x] = 'x';

		bmp(active.x, active.y)->Red = 0;
		bmp(active.x, active.y)->Green = 0;
		bmp(active.x, active.y)->Blue = 0;
	}

	for(Point point : points)
		board[width * (int)point.y + (int)point.x] = 'A';

	bmp.WriteToFile("output.bmp");

	return 0;
}
