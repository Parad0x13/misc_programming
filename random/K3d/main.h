#ifndef _MAIN_H
#define _MAIN_H

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include<windows.h>
#include<math.h>
#include<gl/gl.h>
#include<gl/glu.h>

#define CAMERASPEED 0.025f

void drawgrid(void);
void drawmodels(void);
void initstarfield(void);

struct tVector3 {
	tVector3() {}
	tVector3 (float new_x, float new_y, float new_z) {x = new_x; y = new_y; z = new_z;}
	tVector3 operator+(tVector3 vVector)	{return tVector3(vVector.x+x, vVector.y+y, vVector.z+z);}
	tVector3 operator-(tVector3 vVector)	{return tVector3(x-vVector.x, y-vVector.y, z-vVector.z);}
	tVector3 operator*(float number)		{return tVector3(x*number, y*number, z*number);}
	tVector3 operator/(float number)		{return tVector3(x/number, y/number, z/number);}

	float x, y, z;
};

extern	HDC			hDC;
extern	HGLRC		hRC;
extern	HWND		hWnd;
extern	HINSTANCE	hInstance;

void handleKeyboardInput();

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#endif
