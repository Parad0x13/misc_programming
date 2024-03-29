#include "main.h"

void drawgrid(void);

float theta=0;

float starfield[5000];

GLubyte fire[] = { 0x00, 0x00, 0x00, 0x00,

                   0x00, 0x00, 0x00, 0x00,

                   0x00, 0x00, 0x00, 0x00,

                   0x00, 0x00, 0x00, 0x00,

                   0x00, 0x00, 0x00, 0x00,

                   0x00, 0x00, 0x00, 0x00,

                   0x00, 0x00, 0x00, 0xc0,

                   0x00, 0x00, 0x01, 0xf0,

                   0x00, 0x00, 0x07, 0xf0,

                   0x0f, 0x00, 0x1f, 0xe0,

                   0x1f, 0x80, 0x1f, 0xc0,

                   0x0f, 0xc0, 0x3f, 0x80,

                   0x07, 0xe0, 0x7e, 0x00,

                   0x03, 0xf0, 0xff, 0x80,

                   0x03, 0xf5, 0xff, 0xe0,

                   0x07, 0xfd, 0xff, 0xf8,

                   0x1f, 0xfc, 0xff, 0xe8,

                   0xff, 0xe3, 0xbf, 0x70,

                   0xde, 0x80, 0xb7, 0x00,

                   0x71, 0x10, 0x4a, 0x80,

                   0x03, 0x10, 0x4e, 0x40,

                   0x02, 0x88, 0x8c, 0x20,

                   0x05, 0x05, 0x04, 0x40,

                   0x02, 0x82, 0x14, 0x40,

                   0x02, 0x40, 0x10, 0x80,

                   0x02, 0x64, 0x1a, 0x80,

                   0x00, 0x92, 0x29, 0x00,

                   0x00, 0xb0, 0x48, 0x00,

                   0x00, 0xc8, 0x90, 0x00,

                   0x00, 0x85, 0x10, 0x00,

                   0x00, 0x03, 0x00, 0x00,

                   0x00, 0x00, 0x10, 0x00 };

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds The Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool		keys[256];		// Array Used For The Keyboard Routine
bool		active=TRUE;	// Window Active Flag Set To TRUE By Default
bool		fullscreen=TRUE;// Fullscreen Flag Set To Fullscreen Mode By Default

CCamera objCamera;          // Creates Camera?


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

void initstarfield()
{
	int posneg;
	for(int d=0;d<5000;d++)
	{
		starfield[d]=(float)(rand()%400);
		starfield[d]=starfield[d]-200;
	}
}

// Function to resize the GL Scene
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if(height==0)										// Prevent A Divide By Zero By
		height=1;										// Making Height Equal One

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);		// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,500.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

// OpenGL Init.
int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

							// Position      View		  Up(vector)
	objCamera.Position_Camera(0, 0.5f, 5,	0, 0.5f, 0,   0, 1, 0);     // Sets up the Camera's Position

	return TRUE;										// Initialization Went OK
}

// OpenGL Renderer
int DrawGLScene(GLvoid)									// Here's where all the drawing goes
{
	float x, y, z, angle;
	static GLfloat fElect1 = 0.0f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix

	// Use this function for OpenGL target camera
	gluLookAt(	objCamera.mPos.x,  objCamera.mPos.y,  objCamera.mPos.z,
				objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,
				objCamera.mUp.x,   objCamera.mUp.y,   objCamera.mUp.z);
	
	glPushMatrix();					// Experimenting with pixel sizes!!!
	glPointSize(10.0f);
	glBegin(GL_POINTS);
		glColor3f(1.0f, 0, 0);
		glVertex3f(0, 1.0f, 0);
	glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glBegin(GL_LINES);
		glColor3f(0, 1.0f, 0);
		z = 0.0f;
		for(angle = 0.0f; angle <= GL_PI; angle += (GL_PI/20.0f))
		{
		    // Top half of the circle
		    x = 2.0f*sin(angle);
		    y = 2.0f*cos(angle);
		    glVertex3f(x, y, z);        // First endpoint of line
		    // Bottom half of the circle
		    x = 2.0f*sin(angle + GL_PI);
		    y = 2.0f*cos(angle + GL_PI);
		    glVertex3f(x, y, z);        // Second endpoint of line
		}
	glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glLineWidth(5.0f);
	glBegin(GL_LINES);
		glColor3f(0, 1.0f, 0);
		z = -3.0f;
		for(angle = 0.0f; angle <= GL_PI; angle += (GL_PI/20.0f))
		{
		    // Top half of the circle
		    x = 2.0f*sin(angle);
		    y = 2.0f*cos(angle);
		    glVertex3f(x, y, z);        // First endpoint of line
		    // Bottom half of the circle
		    x = 2.0f*sin(angle + GL_PI);
		    y = 2.0f*cos(angle + GL_PI);
		    glVertex3f(x, y, z);        // Second endpoint of line
		}
	glEnd();
	glLineWidth(1.0f);
	glPopMatrix();
	
	glPushMatrix();
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x888);
	glBegin(GL_LINES);
		glColor3f(0, 1.0f, 0);
		z = -6.0f;
		for(angle = 0.0f; angle <= GL_PI; angle += (GL_PI/20.0f))
		{
		    // Top half of the circle
		    x = 2.0f*sin(angle);
		    y = 2.0f*cos(angle);
		    glVertex3f(x, y, z);        // First endpoint of line
		    // Bottom half of the circle
		    x = 2.0f*sin(angle + GL_PI);
		    y = 2.0f*cos(angle + GL_PI);
		    glVertex3f(x, y, z);        // Second endpoint of line
		}
	glEnd();
	glLineWidth(1.0f);
	glDisable(GL_LINE_STIPPLE);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glShadeModel(GL_FLAT);
	glBegin(GL_TRIANGLE_STRIP);
		glColor3f(1.0f, 0, 0);
		glVertex3i(-2, 0, 5);
		glColor3f(0, 1.0f, 0);
		glVertex3i(2, 0, 5);
		glColor3f(1.0f, 1.0f, 0);
		glVertex3i(0, 2, 5);
		glColor3f(0, 0, 1.0f);
		glVertex3i(2, 2, 5);
	glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glShadeModel(GL_SMOOTH);
	glBegin(GL_TRIANGLE_STRIP);
		glColor3f(1.0f, 0, 0);
		glVertex3i(3, 0, 5);
		glColor3f(0, 1.0f, 0);
		glVertex3i(3, 2, 5);
		glColor3f(1.0f, 1.0f, 0);
		glVertex3i(5, 0, 5);
		glColor3f(0, 0, 1.0f);
		glVertex3i(7, 2, 5);
	glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glEnable(GL_POLYGON_STIPPLE);
	glPolygonStipple(fire);
	glBegin(GL_POLYGON);
        glVertex3f(-2.0f, 5.0f, -10.0f);
        glVertex3f(2.0f, 5.0f, -10.0f);
        glVertex3f(5.0f, 2.0f, -10.0f);
        glVertex3f(5.0f, -2.0f, -10.0f);
        glVertex3f(2.0f, -5.0f, -10.0f);
        glVertex3f(-2.0f, -5.0f, -10.0f);
        glVertex3f(-5.0f, -2.0f, -10.0f);
        glVertex3f(-5.0f, 2.0f, -10.0f);
    glEnd();
    glDisable(GL_POLYGON_STIPPLE);
	glPopMatrix();
	
	glPushMatrix();													// Draw Starfield
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	glColor3f(0.5f, 0.5f, 0.5f);
	for(unsigned int d=0;d<5000;d+=3)
		glVertex3f(starfield[d], starfield[d+1], starfield[d+2]);
	glColor3f(1.0f, 1.0f, 0);
	glVertex2f(5.0f, 5.0f);
	glEnd();
	glPopMatrix();
	
	// Draw Grid
	for(float i = -500; i <= 500; i += 3)
	{
		glBegin(GL_LINES);
			glColor3ub(150, 190, 150);
			glVertex3f(-500, 0, i);
			glVertex3f(500, 0, i);
			glVertex3f(i, 0,-500);
			glVertex3f(i, 0, 500);
		glEnd();
	}
	
/*	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glScissor(0, 0, 200, 200);
	glEnable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glScissor(50, 50, 100, 100);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);*/
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_STENCIL);
	glEnable(GL_STENCIL_TEST);

	theta+=2;

	return TRUE;										// Keep Going
}

// Kill the OpenGL Window with LOTS of error checking
GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if(fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if(hRC)												// Do We Have A Rendering Context?
	{
		if(!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);

		if(!wglDeleteContext(hRC))						// Are We Able To Delete The Rendering Context
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);

		hRC=NULL;										// Sets the Rendering Context to NULL
	}

	if(hDC && !ReleaseDC(hWnd,hDC))						// Are We Able To Release The Device Context
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Sets Device Context to NULL
	}

	if(hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Sets hWnd To NULL
	}

	if(!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Sets hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

// Create the OpenGL Window
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","OPENGL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE

			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

// Window Procedure
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
				active=TRUE;						// Program Is Active
			else
				active=FALSE;						// Program Is No Longer Active

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:

		  switch(wParam)
		  {
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
		  }
		  break;

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

// WinMain has finally arrived!
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
		fullscreen=FALSE;							// Windowed Mode

	// Create Our OpenGL Window
	if (!CreateGLWindow("Bible",640,480,16,fullscreen))
		return 0;									// Quit If Window Was Not Created

	initstarfield();							// Who can resist a starfield?

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
				done=TRUE;							// If So done=TRUE
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}

		else										// If There Are No Messages
		{
			Keyboard_Input();                       // Scan the Keyboard

			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
				done=TRUE;							// ESC or DrawGLScene Signalled A Quit
			else									// Not Time To Quit, Update Screen
				SwapBuffers(hDC);					// Swap Buffers (Double Buffering)

			if (keys[VK_F1])						// Is F1 Being Pressed? If it is then toggle FullScreen
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("Bible",640,480,16,fullscreen))
					return 0;						// Quit If Window Was Not Created
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}

void Keyboard_Input()
{
	if(GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80)       // I'm not sure what the 0x80 is for, but apparantly I need it for something, gotta' find out what.
		objCamera.Move_Camera(CAMERASPEED);

	if(GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80)
		objCamera.Move_Camera(-CAMERASPEED);

    if(GetKeyState('A') & 0x80)
		objCamera.Strafe_Camera(-CAMERASPEED);

	if(GetKeyState('D') &0x80)
		objCamera.Strafe_Camera(CAMERASPEED);

    if((GetKeyState(VK_LEFT) & 0x80))
		objCamera.Rotate_View(-CAMERASPEED);

	if((GetKeyState(VK_RIGHT) & 0x80))
		objCamera.Rotate_View(CAMERASPEED);
}
