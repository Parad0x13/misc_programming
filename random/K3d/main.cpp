#include "main.h"

float floorRotation = 0.0f;
float starfield[5000];
float vdistance = 250.0f;

HDC			hDC = NULL;
HGLRC		hRC = NULL;
HWND		hWnd = NULL;
HINSTANCE	hInstance;

bool keys[256];
bool active = true;
bool fullscreen = true;
bool gameActive;

class CCamera {
public:
	tVector3 mPos;
	tVector3 mView;
	tVector3 mUp;

	void strafe(float speed) {
		tVector3 vVector = mView - mPos;
		tVector3 vOrthoVector;
		vOrthoVector.x = -vVector.z;
		vOrthoVector.z = vVector.x;
		mPos.x = mPos.x + vOrthoVector.x * speed;
		mPos.z = mPos.z + vOrthoVector.z * speed;
		mView.x = mView.x + vOrthoVector.x * speed;
		mView.z = mView.z + vOrthoVector.z * speed;
	}
	void Rotate_View(float speed) {
		tVector3 vVector = mView - mPos;
		mView.z = (float)(mPos.z + sin(speed) * vVector.x + cos(speed) * vVector.z);
		mView.x = (float)(mPos.x + cos(speed) * vVector.x - sin(speed) * vVector.z);
	}
	void Move_Camera(float speed) {
		tVector3 vVector=mView-mPos;
		mPos.x = mPos.x + vVector.x * speed;
		mPos.z = mPos.z + vVector.z * speed;
		mView.x = mView.x + vVector.x * speed;
		mView.z = mView.z + vVector.z * speed;
	}
	void Float_Camera(float speed) {
		tVector3 vVector = mView - mPos;
		tVector3 vOrthoVector;
		vOrthoVector.y = -vVector.z;
		vOrthoVector.z = vVector.y;
		mPos.y = mPos.y + vOrthoVector.y * speed;
		mPos.z = mPos.z + vOrthoVector.z * speed;
		mView.y = mView.y + vOrthoVector.y * speed;
		mView.z = mView.z + vOrthoVector.z * speed;
	}
	void Position_Camera(float pos_x, float pos_y, float pos_z, float view_x, float view_y, float view_z, float up_x, float up_y, float up_z) {
		mPos = tVector3(pos_x, pos_y, pos_z);
		mView = tVector3(view_x, view_y, view_z);
		mUp = tVector3(up_x, up_y, up_z);
	}
};
CCamera camera;

GLvoid ReSizeGLScene(GLsizei width, GLsizei height) {
	if(height==0)height = 1;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, vdistance);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int InitGL() {
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	camera.Position_Camera(0.0f, 0.5f, 5.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f);

	return true;
}

void initstarfield() {
	int range = 400;
	for(int d=0;d<5000;d++) {
		starfield[d] = (float)(rand()%range);
		starfield[d] -= range/2;
	}
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(camera.mPos.x, camera.mPos.y, camera.mPos.z, camera.mView.x, camera.mView.y, camera.mView.z, camera.mUp.x, camera.mUp.y, camera.mUp.z);

	drawgrid();
	drawmodels();
	glPushMatrix();

	glPointSize(3.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_POINTS);
		for(unsigned int d=0;d<5000;d+=3)
			glVertex3f(starfield[d], starfield[d+1], starfield[d+2]);
	glEnd();
	glPopMatrix();

	SwapBuffers(hDC);
}

GLvoid KillGLWindow() {
	if(fullscreen) {
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(true);
	}

	// do we have a rendering context?
	if(hRC) {
		if(!wglMakeCurrent(NULL, NULL))
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		if(!wglDeleteContext(hRC))
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hRC = NULL;
	}

	// device context hDC
	if(hDC && !ReleaseDC(hWnd,hDC)) {
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC=NULL;
	}

	// hWnd window
	if(hWnd && !DestroyWindow(hWnd)) {
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;
	}

	if(!UnregisterClass("OpenGL", hInstance)) {
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;
	}
}

BOOL CreateGLWindow(char *title, int width, int height, int bits, bool fullscreenflag) {
	GLuint		PixelFormat;
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;

	WindowRect.left = 0;
	WindowRect.right = width;
	WindowRect.top = 0;
	WindowRect.bottom = height;

	fullscreen = fullscreenflag;

	hInstance			= GetModuleHandle(NULL);
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC)WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "OpenGL";

	if(!RegisterClass(&wc)) {
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if(fullscreen) {
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmBitsPerPel = bits;
		dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		if(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			if(MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "OPENGL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
				fullscreen = false;
			else {
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK|MB_ICONSTOP);
				return false;
			}
		}
	}

	if(fullscreen) {
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor(false);
	}
	else {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	if(!(hWnd=CreateWindowEx(	dwExStyle,
								"OpenGL",
								title,
								dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
								0, 0,
								WindowRect.right-WindowRect.left,
								WindowRect.bottom-WindowRect.top,
								NULL,
								NULL,
								hInstance,
								NULL))) {
		KillGLWindow();
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	static PIXELFORMATDESCRIPTOR pfd = {
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

	if(!(hDC = GetDC(hWnd))) {
		KillGLWindow();
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if(!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) {
		KillGLWindow();
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if(!SetPixelFormat(hDC,PixelFormat, &pfd)) {
		KillGLWindow();
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (!(hRC = wglCreateContext(hDC))) {
		KillGLWindow();
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if(!wglMakeCurrent(hDC, hRC)) {
		KillGLWindow();
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	ReSizeGLScene(width, height);

	if(!InitGL()) {
		KillGLWindow();
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if(uMsg == WM_ACTIVATE) {
		if(!HIWORD(wParam))active = true;
			else active = false;
		return 0;
	}
	else if(uMsg == WM_SYSCOMMAND) {
		if(wParam == SC_SCREENSAVE) {}
		if(wParam == SC_MONITORPOWER) {}
	}
	else if(uMsg == WM_CLOSE) {
		PostQuitMessage(0);
		return 0;
	}
	else if(uMsg == WM_KEYDOWN)keys[wParam] = true;
	else if(uMsg == WM_KEYUP)keys[wParam] = false;
	else if(uMsg == WM_SIZE) {
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void handleWinMessages() {
	static MSG msg;
	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		if(msg.message == WM_QUIT)gameActive = false;
		else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void handleKeyboardInput() {
	if(keys['W'])camera.Move_Camera(CAMERASPEED);
	if(keys['S'])camera.Move_Camera(-CAMERASPEED);
	if(keys['A'])camera.strafe(-CAMERASPEED);
	if(keys['D'])camera.strafe(CAMERASPEED);
	if(keys[VK_LEFT])camera.Rotate_View(-CAMERASPEED);
	if(keys[VK_RIGHT])camera.Rotate_View(CAMERASPEED);
	if(keys[VK_ESCAPE])gameActive = false;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	if(MessageBox(NULL, "W A S D and Up Down Left Right are the Camera Controls\n\nRun In Fullscreen Mode?", "Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO)
		fullscreen = false;

	if(!CreateGLWindow("[OpenGL K3D] [Keikaku Studios and Kinzoku Studios Production] V1.2.6", 640, 480, 16, fullscreen))
		return 0;

	initstarfield();

	gameActive = true;
	while(gameActive) {
		handleWinMessages();
		handleKeyboardInput();
		render();
	}

	KillGLWindow();
	return false;
}

void drawgrid() {
	for(float i=-500;i<=500;i+=5) {
		glPushMatrix();
		glRotatef(floorRotation, 0.0f, 1.0f, 0.0f);
		glColor3ub(150, 190, 150);
		glBegin(GL_LINES);
			glVertex3f(-500, -10.0, i);
			glVertex3f(500, -10.0, i);
			glVertex3f(i, -10.0, -500);
			glVertex3f(i, -10.0, 500);
		glEnd();
		glPopMatrix();
		floorRotation += 0.0001f;
	}
}
