/**************************
 * Includes
 *
 **************************/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

void Keyboard_Input(void);

/**************************
 * Function Declarations
 *
 **************************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);

bool keys[256];
float theta = 0.0f;
float xtheta=0.0f, ytheta=0.0f, ztheta=0.0f;


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
    int ScreenX=512, ScreenY=512;

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
    wc.lpszClassName = "RotCube";
    RegisterClass (&wc);

    /* create main window */
    hWnd = CreateWindow (
      "RotCube", "Rotate Cube Program", 
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
			Keyboard_Input();
            /* OpenGL animation code goes here */

            glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
            glClear (GL_COLOR_BUFFER_BIT);

            glPushMatrix ();
            glRotatef (xtheta, 1.0f, 0.0f, 0.0f);
            glRotatef (ytheta, 0.0f, 1.0f, 0.0f);
            glRotatef (ztheta, 0.0f, 0.0f, 1.0f);
            glBegin (GL_QUADS);
				glColor3f (1.0f, 0.0f, 0.0f);
				glVertex3f (-0.5f, 0.5f, 0.5f);
				glVertex3f (0.5f, 0.5f, 0.5f);
				glVertex3f (0.5f, 0.5f, -0.5f);
				glVertex3f (-0.5f, 0.5f, -0.5f);

				glColor3f (1.0f, 1.0f, 0.0f);
				glVertex3f (-0.5f, 0.5f, -0.5f);
				glVertex3f (0.5f, 0.5f, -0.5f);
				glVertex3f (0.5f, -0.5f, -0.5f);
				glVertex3f (-0.5f, -0.5f, -0.5f);

				glColor3f (0.0f, 1.0f, 0.0f);
				glVertex3f (-0.5f, 0.5f, 0.5f);
				glVertex3f (0.5f, 0.5f, 0.5f);
				glVertex3f (0.5f, -0.5f, 0.5f);
				glVertex3f (-0.5f, -0.5f, 0.5f);

				glColor3f (0.0f, 1.0f, 1.0f);
				glVertex3f (-0.5f, -0.5f, 0.5f);
				glVertex3f (0.5f, -0.5f, 0.5f);
				glVertex3f (0.5f, -0.5f, -0.5f);
				glVertex3f (-0.5f, -0.5f, -0.5f);
				
				glColor3f (0.0f, 0.0f, 1.0f);
				glVertex3f (-0.5f, 0.5f, 0.5f);
				glVertex3f (-0.5f, 0.5f, -0.5f);
				glVertex3f (-0.5f, -0.5f, -0.5f);
				glVertex3f (-0.5f, -0.5f, 0.5f);

				glColor3f (1.0f, 0.0f, 1.0f);
				glVertex3f (0.5f, 0.5f, 0.5f);
				glVertex3f (0.5f, 0.5f, -0.5f);
				glVertex3f (0.5f, -0.5f, -0.5f);
				glVertex3f (0.5f, -0.5f, 0.5f);
            glEnd ();
            glBegin (GL_LINES);
            	glColor3f (1.0f, 1.0f, 0.0f);
            	glVertex3f (-0.75, 0.0f, 0.0f);
            	glVertex3f (0.75f, 0.0f, 0.0f);

            	glVertex3f (0.0f, -0.75f, 0.0f);
            	glVertex3f (0.0f, 0.75f, 0.0f);

				glVertex3f (0.0f, 0.0f, -0.75f);
				glVertex3f (0.0f, 0.0f, 0.75f);
            glEnd ();
            glPopMatrix ();

            SwapBuffers (hDC);

//            theta += 1.0f;
            Sleep (1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL (hWnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow (hWnd);

    return msg.wParam;
}

void Keyboard_Input()
{
	if(GetKeyState('W')&0x80 || GetKeyState(VK_UP)&0x80)
		xtheta-=2.0f;
	if(GetKeyState('S')&0x80 || GetKeyState(VK_DOWN)&0x80)
		xtheta+=2.0f;

	if(GetKeyState('A')&0x80 || GetKeyState(VK_LEFT)&0x80)
		ytheta-=2.0f;
	if(GetKeyState('D')&0x80 || GetKeyState(VK_RIGHT)&0x80)
		ytheta+=2.0f;

	if(GetKeyState('Q')&0x80 || GetKeyState(VK_SHIFT)&0x80)
		ztheta-=2.0f;
	if(GetKeyState('E')&0x80 || GetKeyState(VK_END)&0x80)
		ztheta+=2.0f;
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
