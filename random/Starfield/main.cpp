#include "main.h"

bool debugmode=1;
float theta=0.0f;
float vdistance=100.0f;
float distance, colmul;			// colmul is a Color Multiplyer
float mx=0, my=0, mz=0;
float tx=0, ty=0, tz=0;			// Theta X, Y, and Z

static char  vars[200]={0};

struct starfield{
	float x, y, z;
}starfield[5000];

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds The Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool		keys[256];		// Array Used For The Keyboard Routine
bool		active=TRUE;	// Window Active Flag Set To TRUE By Default
bool		fullscreen=TRUE;// Fullscreen Flag Set To Fullscreen Mode By Default

CCamera objCamera;          // Creates Camera?

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

// Function to resize the GL Scene
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if(height==0)										// Prevent A Divide By Zero By
		height=1;										// Making Height Equal One

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,vdistance);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void initstarfield()
{
	for(int d=0;d<5000;d++)
	{
		starfield[d].x=(float)(rand()%200);
		starfield[d].x-=100;
		starfield[d].y=(float)(rand()%200);
		starfield[d].y-=100;
		starfield[d].z=(float)(rand()%200);
		starfield[d].z-=100;
	}
}

void movestars()
{
	float keep;
	for(int d=0;d<5000;d++)
	{
		starfield[d].x-=mx;
		starfield[d].y-=my;
		starfield[d].z-=mz;
		distance=sqrt(starfield[d].x*starfield[d].x+starfield[d].y*starfield[d].y+starfield[d].z*starfield[d].z);
		
		glPushMatrix();													// Draw Starfield
		glPointSize(3.0f);
		glBegin(GL_POINTS);
		colmul=(vdistance/2)/distance;
//		colmul/=2;
		glColor3f(colmul, colmul, colmul);
		glVertex3f(starfield[d].x, starfield[d].y, starfield[d].z);
		glEnd();
		glPopMatrix();
		
		keep=starfield[d].y*cos(tx)-starfield[d].z*sin(tx);
		starfield[d].z=starfield[d].z*cos(tx)+starfield[d].y*sin(tx);
		starfield[d].y=keep;
		keep=starfield[d].z*cos(ty)-starfield[d].x*sin(ty);
		starfield[d].x=starfield[d].x*cos(ty)+starfield[d].z*sin(ty);
		starfield[d].z=keep;				/* Final Z */
		keep=starfield[d].x*cos(tz)-starfield[d].y*sin(tz);
		starfield[d].y=starfield[d].y*cos(tz)+starfield[d].x*sin(tz);		/* Final Y */
		starfield[d].x=keep;				/* Final X */
//		npx[ccc]=x;
//		npy[ccc]=y;
//		npz[ccc]=z;

		
		if(distance>100)
		{
			starfield[d].x=(float)(rand()%200);
			starfield[d].x-=100;
			starfield[d].y=(float)(rand()%200);
			starfield[d].y-=100;
			starfield[d].z=(float)(rand()%200);
			starfield[d].z-=100;
		}
		
	}
	if(debugmode==1)
	{
		sprintf(vars, "Variables: mx: %f, my: %f, mz: %f, tx: %f, ty: %f ,tz: %f", mx, my, mz, tx, ty, tz);
		SetWindowText(hWnd, vars);
	}
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix

	// Use this function for OpenGL target camera
	gluLookAt(	objCamera.mPos.x,  objCamera.mPos.y,  objCamera.mPos.z,
				objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,
				objCamera.mUp.x,   objCamera.mUp.y,   objCamera.mUp.z);

	// I need to Re-Write this code down here
	
//	drawgrid();

	/*glPushMatrix();													// Draw Starfield
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	glColor3f(0.5f, 0.5f, 0.5f);
	for(unsigned int d=0;d<5000;d+=3)
		glVertex3f(starfield[d].x, starfield[d].y, starfield[d].z);
	glEnd();
	glPopMatrix();*/

	movestars();

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
	if (MessageBox(NULL,"Up Down Left Right Shift and End Control Dimensional Movement\nW A S D Q and E Control Dimensional Rotation\nR Resets all Movement and Rotation Values\nPlay around with the numbers for different settings\n\nRun In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
		fullscreen=FALSE;							// Windowed Mode

	// Create Our OpenGL Window
	if(fullscreen==TRUE)
	{
		if (!CreateGLWindow("[Starfield 3D] [Keikaku Studios]",1024,768,32,fullscreen))
			return 0;									// Quit If Window Was Not Created
	}

	if(fullscreen==FALSE)
	{
		if (!CreateGLWindow("[Starfield 3D] [Keikaku Studios] Version 1.2.3",800,600,32,fullscreen))
			return 0;									// Quit If Window Was Not Created
	}

	initstarfield();

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
				if (!CreateGLWindow("APRON TUTORIALS",640,480,16,fullscreen))
					return 0;						// Quit If Window Was Not Created
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}

void drawgrid()
{

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
}

// Keyboard Input
void Keyboard_Input()
{
	if(GetKeyState(VK_UP) & 0x80)       // I'm not sure what the 0x80 is for, but apparantly I need it for something, gotta' find out what.
		my+=.01;

	if(GetKeyState(VK_DOWN) & 0x80)
		my-=.01;

    if(GetKeyState(VK_LEFT) & 0x80)
		mx-=.01;

	if(GetKeyState(VK_RIGHT) & 0x80)
		mx+=.01;

	if(GetKeyState(VK_SHIFT) & 0x80)
		mz+=.01;

	if(GetKeyState(VK_END) & 0x80)
		mz-=.01;
	
	if(GetKeyState('W') & 0x80)
		tx-=.0001f;
	
	if(GetKeyState('S') & 0x80)
		tx+=.0001f;
		
	if(GetKeyState('A') & 0x80)
		ty-=.0001f;
		
	if(GetKeyState('D') & 0x80)
		ty+=.0001f;
	
	if(GetKeyState('Q') & 0x80)
		tz-=.0001f;

	if(GetKeyState('E') & 0x80)
		tz+=.0001f;

	if(GetKeyState('R') & 0x80)				// Reset
	{
		mx=0;
		my=0;
		mz=0;
		tx=0;
		ty=0;
		tz=0;
	}
	
	if(GetKeyState('1') &0x80)
	{
		mx=0;
		my=0;
		mz=5.0f;
		tx=.03f;
		ty=.03f;
		tz=0;
	}
	
	if(GetKeyState('2') &0x80)
	{
		mx=0;
		my=0;
		mz=2.979998f;
		tx=0;
		ty=.0159f;
		tz=.0494f;
	}
	
	if(GetKeyState('3') &0x80)
	{
		mx=0;
		my=1.19f;
		mz=0;
		tx=.0256f;
		ty=0;
		tz=0;
	}
	
	if(GetKeyState('4') &0x80)
	{
		mx=0;
		my=1.19f;
		mz=0;
		tx=.0256f;
		ty=0;
		tz=.0324f;
	}
	
	if(GetKeyState('5') &0x80)
	{
		mx=0;
		my=1.19f;
		mz=0;
		tx=.0256f;
		ty=-.0367;
		tz=0;
	}
	
	if(GetKeyState(VK_SHIFT) & 0x80 && GetKeyState('D') & 0x80)
		debugmode=!debugmode;
}
