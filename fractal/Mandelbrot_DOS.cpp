/*	Bryan Tremblay 25Dec12@0454E

	I tried to make a Mandelbrot genertor for the command prompt
	but for some reason it only produces ovals... the equivalant
	of low-res mandelbrot iterations. I can't seem to get it to
	go higher. I submitted a request to stackoverflow.com:
	http://stackoverflow.com/questions/14029538/low-resolution-mandelbrot-fractal-not-high-enough-resolution
	TURNS OUT that I wasn't running the entire equation, guy name Potatoswatter fixed my code
*/

#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#include <iostream>

#define DOSWidth 80
#define DOSHeight 25

int iterations = 32;
float escapeValue = 3.0f;

struct ivar {
	ivar(float _x, float _i) {
		x = _x;
		i = _i;
	}
	void log() {printf("(%g%c%gi)", x, (i<0)?'-':'+', fabs(i));}
	float magnitude() {return sqrtf(x*x+i*i);}
	ivar square() {return ivar(x, i)*ivar(x, i);}

	ivar operator + (ivar v) {return ivar(x+v.x, i+v.i);};
	ivar operator - (ivar v) {return ivar(x-v.x, i-v.i);};
	ivar operator * (ivar v) {return ivar(x*v.x-(i*v.i), x*v.i+i*v.x);};

	float x, i;
};

struct rect {
	rect(float _x, float _y, float _width, float _height) {
		x = _x;y = _y;width = _width;height = _height;
	}

	void log() {printf("(%f, %f, %f, %f)", x-width/2.0f, y-height/2.0f, x+width/2.0f, y+height/2.0f);}

	float x, y;
	float width, height;
};

void render(rect region);

int main() {
	bool shouldContinue = true;
	float DOSRatio = DOSWidth/DOSHeight;
	rect region = rect(0, 0, 2*DOSRatio, 2);
	while(shouldContinue) {
		render(region);
		char input = getch();
		float xSize = region.height/10.0f;
		float ySize = region.width/(10.0f*DOSRatio);
		if(input == 'w')region.y += xSize;
		if(input == 's')region.y -= xSize;
		if(input == 'a')region.x += ySize;
		if(input == 'd')region.x -= ySize;
		if(input == '+' || input == '=')iterations+=10;
		if(input == '-')iterations-=10;
		if(input == 'e')region = rect(region.x, region.y, region.width*0.9f, region.height*0.9f);
		if(input == 'q')region = rect(region.x, region.y, region.width/0.9f, region.height/0.9f);
		if(input == 0x1b)shouldContinue = false;
	}
}

void render(rect region) {
	float xSize = region.width / (float)DOSWidth;
	float ySize = region.height / (float)DOSHeight;
	char colors[] = " .,xOX";
	for(int y=0;y<DOSHeight;y++) {
		for(int x=0;x<DOSWidth;x++) {
			ivar pos = ivar(x*xSize-(region.width/2.0f+region.x), y*ySize-(region.height/2.0f+region.y));
			ivar pos0 = pos;
			bool escapes = false;
			int i;
			for(i=0;i<iterations;i++) {
				if(pos.magnitude() > escapeValue) {
					escapes = true;
					break;
				}
				pos = pos.square()+pos0;
			}
			if(escapes)std::cout << colors[i%strlen(colors)];
			else std::cout << '$';
		}
	}
}
