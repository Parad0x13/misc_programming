/* Henon Attractor Fractal Generator Version 1.2, programmed by Bryan Tremblay of Keikaku Studios
	Mainly only the mathematics programming is myne, everything else I've used from elsewhere... need to fix that */

#include <windows.h>
#include <gl/gl.h>

void henon(void);

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);

//More variables here!!!

float theta = 0.0f;
float x=1, y=1;
float a=1.4f, b=0.3f;
int ScreenX=500, ScreenY=700;

/**************************
 * WinMain
 *
 **************************/

int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int iCmdShow)
{
    WNDCLASS wc;
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;        
    MSG msg;
    BOOL bQuit = FALSE;
    float GridX, GridY, GridZ=0.0f;

    /* register window class */
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "HenonAttractor";
    RegisterClass (&wc);

    /* create main window */
    hWnd = CreateWindow (
      "HenonAttractor", "[ Henon Attractor V1.2 ][ Keikaku Studios ]", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, ScreenX, ScreenY,
      NULL, NULL, hInstance, NULL);

    /* enable OpenGL for the window */
    EnableOpenGL (hWnd, &hDC, &hRC);

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
        else
        {
			henon();			//Yes, yes I know... We are constantly rendering... BOOHOO CPU!!!
			SwapBuffers(hDC);
			theta+=1.0f;
			Sleep (1);		/*Don't really know what it's for*/
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL (hWnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow (hWnd);

    return msg.wParam;
}

void henon()
{
	int iteration=10000, d, buffer=0;
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT);

	for(d=0;d<iteration;d++)
	{
		float nx, ny;
		nx=y+1-a*(x*x);
		ny=b*x;
		x=nx;
		y=ny;

		if(buffer>100)	/*Skips the first 100 iterations*/
		{
			glPushMatrix ();
			glRotatef (theta, 0.0f, 1.0f, 0.0f);
			glBegin (GL_POINTS);
				glColor3f (0.0f, 0.0f, 1.0f);
				glVertex3f (x/1.7f, y/0.4f, 0.0f);		//The division makes it look right
				glColor3f (1.0f, 0.0f, 0.0f);
				glVertex3f (-x/6.8f, y/1.6f, 0.0f);
			glEnd ();
			glBegin (GL_LINES);
				glColor3f (1.0f, 1.0f, 0.0f);
				glVertex3f (0.0f, 0.0f, -0.5f);
				glVertex3f (0.0f, 0.0f, 0.5f);
			glEnd ();
			glPopMatrix ();
		}
		buffer++;
	}
}

/********************
 * Window Procedure
 *
 ********************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage (0);
        return 0;

    case WM_DESTROY:
        return 0;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
        }
        return 0;

    default:
        return DefWindowProc (hWnd, message, wParam, lParam);
    }
}


/*******************
 * Enable OpenGL
 *
 *******************/

void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC (hWnd);

    /* set the pixel format for the DC */
    ZeroMemory (&pfd, sizeof (pfd));
    pfd.nSize = sizeof (pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
      PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat (*hDC, &pfd);
    SetPixelFormat (*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );

}


/******************
 * Disable OpenGL
 *
 ******************/

void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hRC);
    ReleaseDC (hWnd, hDC);
}
