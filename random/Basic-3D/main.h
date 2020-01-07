#ifndef _MAIN_H
#define _MAIN_H

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include<windows.h>
#include<math.h>
#include<gl/gl.h>                   // Header file for OpenGL32 Library
#include<gl/glu.h>                  // Header file for GLU32 Library

#define CAMERASPEED 0.025f

void drawgrid(void);

// This is the tVector3 Structure
struct tVector3	// Extended 3D Vector Struct
{
	tVector3() {} // Struct Constructor
	tVector3 (float new_x, float new_y, float new_z) // Init Constructor
	{ x = new_x; y = new_y; z = new_z; }
	// overload + operator
	tVector3 operator+(tVector3 vVector) {return tVector3(vVector.x+x, vVector.y+y, vVector.z+z);}
	// overload - operator
	tVector3 operator-(tVector3 vVector) {return tVector3(x-vVector.x, y-vVector.y, z-vVector.z);}
	// overload * operator
	tVector3 operator*(float number)	 {return tVector3(x*number, y*number, z*number);}
	// overload / operator
	tVector3 operator/(float number)	 {return tVector3(x/number, y/number, z/number);}

	float x, y, z;						// 3D vector coordinates
};

// This is the CCamera Class
class CCamera
{
	public:
		tVector3 mPos;      // Camera Position
		tVector3 mView;     // Camera View(Target)
		tVector3 mUp;       // Camera UpVector(Tilt)

        void Strafe_Camera(float speed);
		void Rotate_View(float speed);
		void Move_Camera(float speed);
		void Position_Camera(	float pos_x, float pos_y, float pos_z,
		                        float view_x, float view_y, float view_z,
		                        float up_x, float up_y, float up_z);
};                          // Belive it or not, but the ; in this line cost me about 2 hours of debugging };

// Global Variables go here!!!
extern	HDC			hDC;			// Device context
extern	HGLRC    	hRC;            // Permanant Rendering Context
extern	HWND     	hWnd;           // Holds The Window Handle
extern	HINSTANCE    hInstance;     // Holds The Instance of the Application

void Keyboard_Input();

#endif
