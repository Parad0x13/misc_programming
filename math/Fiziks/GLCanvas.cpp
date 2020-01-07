#include "GLCanvas.h"

GLCanvas::GLCanvas(wxWindow *parent, const wxPoint& pos, const wxSize& size, const wxString& title, PhysicsEngine *pPhysicsEngine)
: wxGLCanvas(parent, (wxGLCanvas*) NULL, wxID_ANY, pos, size, wxFULL_REPAINT_ON_RESIZE, title)
{
	gl_init = false;
	m_PhysicsEngine = pPhysicsEngine;
}

BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
	EVT_SIZE(GLCanvas::OnSize)
	EVT_PAINT(GLCanvas::OnPaint)
	EVT_ENTER_WINDOW(GLCanvas::OnEnterWindow)
	EVT_ERASE_BACKGROUND(GLCanvas::OnEraseBackground)
END_EVENT_TABLE()

void GLCanvas::InitGL()
{
	SetCurrent();
    // set viewing projection
	glMatrixMode(GL_PROJECTION);
	glFrustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 3.0f);

    // position viewer
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0.0f, 0.0f, -2.0f);
}

void GLCanvas::Render()
{
	SetCurrent();
	// Init OpenGL once, but after SetCurrent
	if(!gl_init)
	{
		InitGL();
		gl_init = true;
	}

	// This part is redundant, initGL is supposed to do this!
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glFrustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 3.0f);
//	glMatrixMode(GL_MODELVIEW);

	// clear color and depth buffers, MAGIC LINE! Gets rid of dirty windows
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float x = 0.2f, y = 0.0f;
	glBegin(GL_POINTS);
		glColor3ub(255, 255, 0);
		glVertex2f(0, 0);
		for(int d=0;d<m_PhysicsEngine->ObjectCount;d++)
		{
			glColor3ub(0, 255, 0);
			glVertex2f(m_PhysicsEngine->Stars[d].x, m_PhysicsEngine->Stars[d].y);
		}
	glEnd();

	glFlush();
	SwapBuffers();
}

void GLCanvas::OnSize(wxSizeEvent& event)
{
	// Reference to OpenGL sample source to find how to have better cross platform support for resizing
	wxSize ClientSize = GetClientSize();
	glViewport(0, 0, ClientSize.x, ClientSize.y);
}

void GLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxPaintDC dc(this);
	Render();
}

void GLCanvas::OnEnterWindow(wxMouseEvent& WXUNUSED(event))
{
	SetFocus();
}

void GLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	// empty function, to avoid flashing.
}