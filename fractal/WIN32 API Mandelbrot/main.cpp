/* Programmed Specifically for DevCpp 4.9.9.2 Compiler */
/* Not sure what -lwinmm -lstdc++ are that were passed to the old GLUT Mandelbrot program, may/not need them later */
/* Make sure that Dependancy Checking, Makefile Generation, is unchecked within the Compiler Options for correct compiling */
/* -s for linker is enabled, so no debugging information will be available untill this is released */
#include "resource.h"
#include "InitOGL.h"
#include "InitWIN32.h"
#include "fractref.h"
#include "ComplexMath.h"
#include "fractals.h"
#include "coloring.h"
#include "jpeg.h"
#include "main.h"

void zoom(void);

bool DrawGLScene(GLvoid);
void mouse(MSG state);

POINT mousePos;

ivar zoomdown, zoomup;

int raster=0;

/*
	WinMain
			*/

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Create Our OpenGL Window
	if (!CreateGLWindow(ProgramHeader,ScreenX, ScreenY, OffsetX, OffsetY, 16, fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}
	seed();		// Seed Random Values for now on
/*	if(read_jpeg_file("test.jpg")!=true)
				printf("No ready jpeg :(...\n");
	if(write_jpeg_file("test_write.jpg")!=true)
				printf("No writy jpeg :(...\n");*/
	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}

/*
	Within this else loop is where all code for normal program operation will go
																				*/
// All Keystoke Logging will be taken care of directly within the WndProc

		else										// If There Are No Messages
		{
			mouse(msg);
			// Apparantly rasterizing will slow the process down due more calculations, however the results are worth it
			if(reDisplay==true)
			{
				reDisplay=false;
				for(raster=0;raster<chunks;raster++)
				{
					printf("Raster: %i\n", raster);
					// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
					if ((active && !DrawGLScene()))			// Active?  Was There A Quit Received?
						done=TRUE;							// ESC or DrawGLScene Signalled A Quit
					else									// Not Time To Quit, Update Screen
					SwapBuffers(hDC);						// Swap Buffers (Double Buffering)
				}
			}
			Sleep(1);	// No more 68%CPU on this program lol, yeay processor management!!! Do not remove this line
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}

bool DrawGLScene(GLvoid)
{
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);			// Black was the OLD Mandelbrot Generator
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	switch(fractal_type)
	{
		case FRACT_HENON:
			fractal_henon();
		break;

		case FRACT_TINKERBELL:
			fractal_tinkerbell();
		break;

		case FRACT_LORENZ:
			fractal_lorenz();
		break;

		case FRACT_RABINOVICHFABRIKANT:
			fractal_rabinovichfabrikant();
		break;

		case FRACT_HADLEY:
			fractal_hadley();
		break;

		case FRACT_ROSSLER:
			fractal_rossler();
		break;

		case FRACT_ACT:
			fractal_act();
		break;

		case FRACT_RUCKLIDGE:
			fractal_rucklidge();
		break;

		case FRACT_IFS_SIERPINSKI:
			fractal_IFS_sierpinski();
		break;

		case FRACT_IFS_TRIANGLES:
			fractal_IFS_triangles();
		break;

		case FRACT_IFS_PENTAGON:
			fractal_IFS_pentagon();
		break;

		case FRACT_MANDELBROT:
			fractal_mandelbrot(shadeT);
		break;

		case FRACT_MANDELBAR:
			fractal_mandelbar(shadeT);
		break;

		case FRACT_JULIA:
			fractal_julia(shadeT);
		break;

		case TEST_MANDELBROT:
			test_mandelbrot(shadeT, raster);
		break;
	}
	return(true);
}

void mouse(MSG state)
{
	GetCursorPos(&mousePos);
//	SetCursorPos(0, 0);						// Could do this if you wanted to be a jerk
	int tx, ty, tRx, tRy;				// TempX and TempY and TempReleaseX and TempReleaseY
	if(state.message==WM_LBUTTONDOWN)
	{
		tx=mousePos.x;ty=mousePos.y;
//		printf("\nLeft Button Pressed at (%i, %i)\n", tx, ty);
		zoomdown=iantiscreen(maxX, minX, maxY, minY, ScreenX, ScreenY, tx, ty);
	}
	if(state.message==WM_LBUTTONUP)
	{
		tRx=mousePos.x;tRy=mousePos.y;
//		printf("\nLeft Button Lifted at (%i, %i)\n", tRx, tRy);
		zoomup=iantiscreen(maxX, minX, maxY, minY, ScreenX, ScreenY, tRx, tRy);

		zoom();
		reDisplay=true;
	}

	if(state.message==WM_RBUTTONDOWN)
	{
		tx=mousePos.x;ty=mousePos.y;
//		printf("Right Button Pressed at (%i, %i)\n", tx, ty);
	}
	if(state.message==WM_RBUTTONUP)
	{
		tRx=mousePos.x;tRy=mousePos.y;
//		printf("Right Button Lifted at (%i, %i)\n", tRx, tRy);
	}
}

void zoom()
{
	if(zoomdown.real<zoomup.real)
	{
		maxX=zoomup.real;
		minX=zoomdown.real;
	}
	if(zoomup.real<zoomdown.real)
	{
		maxX=zoomdown.real;
		minX=zoomup.real;
	}
	if(zoomdown.immaginary<zoomup.immaginary)
	{
		maxY=zoomdown.immaginary;
		minY=zoomup.immaginary;
	}
	if(zoomup.immaginary<zoomdown.immaginary)
	{
		maxY=zoomup.immaginary;
		minY=zoomdown.immaginary;
	}
}

void changeGraph(int movetype)
{
	switch(movetype)
	{
		case 1:				// ZoomIn
			nmaxX=maxX+((maxX-minX)*.1);
			nminX=minX-((maxX-minX)*.1);
			nmaxY=maxY+((maxY-minY)*.1);
			nminY=minY-((maxY-minY)*.1);
			maxX=nmaxX;
			minX=nminX;
			maxY=nmaxY;
			minY=nminY;
			reDisplay=true;
		break;
		case 2:				// ZoomOut
			nmaxX=maxX-((maxX-minX)*.1);
			nminX=minX+((maxX-minX)*.1);
			nmaxY=maxY-((maxY-minY)*.1);
			nminY=minY+((maxY-minY)*.1);
			maxX=nmaxX;
			minX=nminX;
			maxY=nmaxY;
			minY=nminY;
			reDisplay=true;
		break;
		case 3:				// PanUp
			nmaxY=maxY+.1*(maxY-minY);
			nminY=minY+.1*(maxY-minY);
			maxY=nmaxY;
			minY=nminY;
			reDisplay=true;
		break;
		case 4:				// PanDown
			nmaxY=maxY-.1*(maxY-minY);
			nminY=minY-.1*(maxY-minY);
			maxY=nmaxY;
			minY=nminY;
			reDisplay=true;
		break;
		case 5:				// PanLeft
			nmaxX=maxX-.1*(maxX-minX);
			nminX=minX-.1*(maxX-minX);
			maxX=nmaxX;
			minX=nminX;
			reDisplay=true;
		break;
		case 6:				// PanRight
			nmaxX=maxX+.1*(maxX-minX);
			nminX=minX+.1*(maxX-minX);
			maxX=nmaxX;
			minX=nminX;
			reDisplay=true;
		break;
	}
}

int rnd(int range)
{
	int r;

	r=rand()%range;
	return(r);
}

void seed()
{
	srand((unsigned)time(NULL));
}
