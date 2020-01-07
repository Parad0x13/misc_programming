/**************************
 * Includes
 *
 **************************/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>

void matrix3d(void);

/**************************
 * Function Declarations
 *
 **************************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);

float matrix[12]={-0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5, 0.0f};
float theta = 0.0f;
float rotquad;


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
    
//       Variables here!!!

    float bgcolorR=0.0f, bgcolorG=0.0f, bgcolorB=0.0f;
    float px=0, py=0;
    float pixelwidth;
    float pointx=.5, pointy=0, pointz=0;
    int switcher=1, direction=0;

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
    wc.lpszClassName = "GLSample";
    RegisterClass (&wc);

    /* create main window */
    hWnd = CreateWindow (
      "GLSample", "3D FUN PROGRAM!!!", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, 512, 512,
      NULL, NULL, hInstance, NULL);

    pixelwidth=2.0f/512.0f;

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
            /* OpenGL animation code goes here */
// Bright white light – full intensity RGB values

    GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat gray[] = { 0.75f, 0.75f, 0.75f, 1.0f };
    // Enable lighting

    glEnable(GL_LIGHTING);
    // Set light model to use ambient light specified by ambientLight[]
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLight);


            glClearColor (bgcolorR, bgcolorG, bgcolorB, 1);
            glClear (GL_COLOR_BUFFER_BIT);

            glPushMatrix ();
//            glTranslatef(-0.5f,0.5f,0.0f);

            glRotatef (theta, 0.0f, 0.0f, 1.0f);
            
			glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
			glColor3f(0.75f, 0.75f, 0.75f);
            glBegin (GL_TRIANGLES);
//            	glColor3f (1.0f, 0.0f, 0.0f);
				glVertex2f (0.0f, 1.0f);
//            	glColor3f (0.0f, 1.0f, 0.0f);
				glVertex2f (-0.5f, 0.0f);
//            	glColor3f (0.0f, 0.0f, 1.0f);
				glVertex2f (0.5f, 0.0f);
            glEnd ();
            glPopMatrix ();
            
            glPushMatrix ();
            glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
			glColor3f(0.75f, 0.75f, 0.75f);
            glBegin (GL_LINES);
            matrix3d();
            glEnd ();
            glPopMatrix ();

            glPushMatrix ();
            glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
			glColor3f(0.75f, 0.75f, 0.75f);
            glBegin (GL_POINTS);
//            glColor3f (0.0f, 0.0f, 0.0f);   glVertex2f (px, py);
            glEnd ();
            glPopMatrix ();
            
            glPushMatrix ();
            glRotatef(theta, 0.0f, 1.0f, 0.0f);
            glBegin (GL_POINTS);
            glColor3f (1.0f, 1.0f, 0.0f);   glVertex3f (pointx, pointy, pointz);
            glEnd ();
            glPopMatrix ();
            
            glPushMatrix ();
            glRotatef(theta, 0.0f, 1.0f, 0.0f);
            glBegin (GL_POINTS);
            glColor3f (1.0f, 1.0f, 0.0f);   glVertex3f (-pointx, pointy, pointz);
            glEnd ();
            glPopMatrix ();
            
            glPushMatrix ();
            glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
			glColor3f(0.75f, 0.75f, 0.75f);
            glRotatef(theta, 0.5f, 1.0f, 0.25f);
            glBegin  (GL_QUADS);
            glVertex3f (-0.25f, 0.25f, 0.25f);
            glVertex3f (0.25f, 0.25f, 0.25f);
            glVertex3f (0.25f, -0.25f, 0.25f);
            glVertex3f (-0.25f, -0.25f, 0.25f);
            glEnd ();
            glBegin  (GL_LINES);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (-0.25f, 0.25f, -0.25f);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (-0.25f, -0.25f, -0.25f);
            glEnd ();
            glBegin  (GL_LINES);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (-0.25f, 0.25f, 0.25f);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (-0.25f, -0.25f, 0.25f);
            glEnd ();
            glBegin  (GL_LINES);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (0.25f, 0.25f, -0.25f);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (0.25f, -0.25f, -0.25f);
            glEnd ();
            glBegin  (GL_LINES);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (0.25f, 0.25f, 0.25f);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (0.25f, -0.25f, 0.25f);
            glEnd ();
            glBegin  (GL_LINE_LOOP);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (-0.25f, 0.25f, -0.25f);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (0.25f, 0.25f, -0.25f);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (0.25f, 0.25f, 0.25f);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (-0.25f, 0.25f, 0.25f);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (-0.25f, 0.25f, -0.25f);
            glEnd ();
            glBegin  (GL_LINE_LOOP);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (-0.25f, -0.25f, -0.25f);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (0.25f, -0.25f, -0.25f);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (0.25f, -0.25f, 0.25f);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (-0.25f, -0.25f, 0.25f);
            glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (-0.25f, -0.25f, -0.25f);
            glEnd ();
            glPopMatrix ();

            SwapBuffers (hDC);

            theta += 1.0f;
            px+=pixelwidth;
            py+=pixelwidth;
            
            if(switcher==1)
            {
             if(direction==0)
             {
              if(bgcolorR<1)
               bgcolorR+=.01;
              else
               direction=1;
             }
             if(direction==1)
             {
              if(bgcolorR>0)
               bgcolorR-=.01;
              else
              {
               direction=0;
               bgcolorR=0;
               switcher=2;
              }
             }
            }
            
            if(switcher==2)
            {
             if(direction==0)
             {
              if(bgcolorG<1)
               bgcolorG+=.01;
              else
               direction=1;
             }
             if(direction==1)
             {
              if(bgcolorG>0)
               bgcolorG-=.01;
              else
              {
               direction=0;
               bgcolorG=0;
               switcher=3;
              }
             }
            }
            
            if(switcher==3)
            {
             if(direction==0)
             {
              if(bgcolorB<1)
               bgcolorB+=.01;
              else
               direction=1;
             }
             if(direction==1)
             {
              if(bgcolorB>0)
               bgcolorB-=.01;
              else
              {
               direction=0;
               bgcolorB=0;
               switcher=1;
              }
             }
            }
            Sleep (1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL (hWnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow (hWnd);

    return msg.wParam;
}

void matrix3d()
{
	int x;
	for(x=0;x<36;x+=3)
	{
		glRotatef(theta, 0.0f, 1.0f, 0.0f);		//Not working!!!???
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(matrix[x], matrix[x+1], matrix[x+2]);
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
