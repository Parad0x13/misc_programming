/*	There are many things really wrong with this program... but right now I am
	waaaaaaaaaaay too annoyed to fix it, ITS BEEN LIKE 5 HOURS!!! So I'm going
	to quit programming for awhile, I need to rewrite the whole physics
	algorithm... :( and the only thing keeping me alive is Wierd Al Yankovic...	*/


#include "main.h"

float theta=0.0f;
float vdistance=500.0f;
float distance;
//float time=0;
double G=0.0000000000667;
int xdiv=800, ydiv=600;
bool debugmode=1;

static char  vars[200]={0};

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds The Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool		keys[256];		// Array Used For The Keyboard Routine
bool		active=TRUE;	// Window Active Flag Set To TRUE By Default
bool		fullscreen=TRUE;// Fullscreen Flag Set To Fullscreen Mode By Default

CCamera objCamera;          // Creates Camera?

struct sun{
	float x, y, z;
	float r, g, b;
	float radius, mass;
	float fx, fy, fz;		// ForceX, ForceY, and ForceZ in Newtons
	float ax, ay, az;		// Acceleration in X, Y, and Z in Meters/Second^2
}sun[2];

void initsuns()
{
	sun[0].r=1.0f;
	sun[0].g=1.0f;
	sun[0].b=0;
	sun[0].x=-10.0f;
	sun[0].y=25.0f;
	sun[0].z=-100.0f;
	sun[0].radius=5.0f;
	sun[0].mass=100000000000.0f;
	sun[0].fx=0;
//	sun[0].fx=-1000000.0f;
	sun[0].fy=0;
	sun[0].fz=0;
	sun[0].ax=0;
	sun[0].ay=0;
	sun[0].az=0;
	
	sun[1].r=1.0f;
	sun[1].g=0;
	sun[1].b=0;
	sun[1].x=10.0f;
	sun[1].y=0;
	sun[1].z=-100.0f;
	sun[1].radius=5.0f;
	sun[1].mass=100000000000.0f;
	sun[1].fx=0;
	sun[1].fy=0;
	sun[1].fz=0;
	sun[1].ax=0;
	sun[1].ay=0;
	sun[1].az=0;
}

void drawsuns()
{
	for(int d=0;d<2;d++)
	{
		glPushMatrix();
		glColor3f(sun[d].r, sun[d].g, sun[d].b);
		glTranslatef((float)sun[d].x, (float)sun[d].y, (float)sun[d].z);
		glutSolidSphere(sun[d].radius, xdiv, ydiv);
		glPopMatrix();
	}
	
	glPushMatrix();							// Draw a line down the center
	glColor3f(1.0f, 1.0f, 0);
	glBegin(GL_LINES);
		glVertex3f(0, 0, 100.0f);
		glVertex3f(0, 0, -100.0f);
	glEnd();
	glPopMatrix();
}

void physics()
{
	float xsqrdis, ysqrdis, zsqrdis;
	float xdis, ydis, zdis;
	xdis=sun[1].x-sun[0].x;
	ydis=sun[1].y-sun[0].y;
	zdis=sun[1].z-sun[0].z;
	xsqrdis=xdis*xdis;
	if(xdis<0)
		xsqrdis=-xsqrdis;
	ysqrdis=ydis*ydis;
	if(ydis<0)
		ysqrdis=-ysqrdis;
	zsqrdis=zdis*zdis;
	if(zdis<0)
		zsqrdis=-zsqrdis;
	if(xsqrdis!=0)
	{
		sun[0].fx=(G*sun[0].mass*sun[1].mass)/xsqrdis;
		sun[0].ax=sun[0].fx/sun[0].mass;
	}
	if(ysqrdis!=0)
	{
		sun[0].fy+=(G*sun[0].mass*sun[1].mass)/ysqrdis;
		sun[0].ay=sun[0].fy/sun[0].mass;
	}
	if(zsqrdis!=0)
	{
		sun[0].fz+=(G*sun[0].mass*sun[1].mass)/zsqrdis;
		sun[0].az=sun[0].fz/sun[0].mass;
	}
	
	xdis=sun[0].x-sun[1].x;
	ydis=sun[0].y-sun[1].y;
	zdis=sun[0].z-sun[1].z;
	xsqrdis=xdis*xdis;
	if(xdis<0)
		xsqrdis=-xsqrdis;
	ysqrdis=ydis*ydis;
	if(ydis<0)
		ysqrdis=-ysqrdis;
	zsqrdis=zdis*zdis;
	if(zdis<0)
		zsqrdis=-zsqrdis;
	if(xsqrdis!=0)
	{
		sun[1].fx+=(G*sun[1].mass*sun[0].mass)/xsqrdis;
		sun[1].ax=sun[1].fx/sun[1].mass;
	}
	if(ysqrdis!=0)
	{
		sun[1].fy+=(G*sun[1].mass*sun[0].mass)/ysqrdis;
		sun[1].ay=sun[1].fy/sun[1].mass;
	}
	if(zsqrdis!=0)
	{
		sun[1].fz+=(G*sun[1].mass*sun[0].mass)/zsqrdis;
		sun[1].az=sun[1].fz/sun[1].mass;
	}
	
	if(debugmode==1)
	{
		sprintf(vars, "First Stars ForceX: %.11f Second Stars ForceX: %.11f in Newtons, Acceleration in DimX: %.11f Distance: %f X: %f X2: %f", sun[0].fx, sun[1].fx, sun[0].ax, distance, sun[0].x, sun[1].x);
		SetWindowText(hWnd, vars);
	}
	
	sun[0].x+=sun[0].ax;
//	sun[1].x+=sun[1].ax;
//	sun[0].y+=sun[0].ay;
//	sun[1].y+=sun[1].ay;
//	sun[0].z+=sun[0].az;
//	sun[1].z+=sun[1].az;
	
//	Sleep(1000);
	
//	time=time+1.0f;
}

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
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f, vdistance);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix

	// Use this function for OpenGL target camera
	gluLookAt(	objCamera.mPos.x,  objCamera.mPos.y,  objCamera.mPos.z,
				objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,
				objCamera.mUp.x,   objCamera.mUp.y,   objCamera.mUp.z);

	// I need to Re-Write this code down here
	
	drawgrid();
	
	drawsuns();
	
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
	if (!CreateGLWindow("Gravity",xdiv,ydiv,32,fullscreen))
		return 0;									// Quit If Window Was Not Created

	initsuns();

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

			physics();

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
	
	if((GetKeyState('R') & 0x80))
	{
		sun[0].x*=-1;
		sun[1].x*=-1;
	}
}




