#ifndef MAIN_H
#define MAIN_H

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "fractref.h"

#define WIN32_LEAN_AND_MEAN

struct RGBquad
{
	int R;
	int G;
	int B;
	int A;
};

/* It is a HUDGE blessing that we can define functions here for the other .h files */
GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
GLvoid KillGLWindow(GLvoid);
BOOL CreateGLWindow(char* title, int width, int height, int offsetX, int offsetY, int bits, bool fullscreenflag);
float shade(int bail, int finiteration, int power, float dist, int shadetype);
void colorRamp(float value, float maximum);
void changeGraph(int movetype);
RGBquad colorArray(int escape);
int rnd(int range);
void seed();

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds The Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

int theta=0;
int fullscreen=0;
int ScreenX=300, ScreenY=300;
int OffsetX=200, OffsetY=100;

int bailout=5, depth=32, diterations=800000;
int fractal_type=TEST_MANDELBROT;
float maxX=3.0f, minX=-3.0f, maxY=3.0f, minY=-3.0f;
float nmaxX, nminX, nmaxY, nminY;

bool active=TRUE;			// Is screen active? Variable

int shadeT=NORMALIZE_1;

int chunks=5;
int chunk_size[5]={16, 8, 4, 2, 1};

bool reDisplay=true;

char ProgramHeader[100]={"WIN32 Keikaku Mandelbrot V2.0.2"};

void reset(void)
{
	theta=0;
	maxX=3.0f;
	minX=-3.0f;
	maxY=3.0f;
	minY=-3.0f;
	reDisplay=true;
}

#endif
