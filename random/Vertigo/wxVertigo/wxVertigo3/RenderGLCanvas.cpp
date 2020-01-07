#include "RenderGLCanvas.h"

C_RenderGLCanvas::C_RenderGLCanvas(wxWindow *parent, C_Equations *pEquations, wxWindowID id,
	const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxGLCanvas(parent, (wxGLCanvas*) NULL, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name)
{
	m_Equations = pEquations;

	gl_init = false;
	sizeWithClient = true;
}

BEGIN_EVENT_TABLE(C_RenderGLCanvas, wxGLCanvas)
	//EVT_SIZE(C_RenderGLCanvas::OnSize)
	EVT_PAINT(C_RenderGLCanvas::OnPaint)
	EVT_ENTER_WINDOW(C_RenderGLCanvas::OnEnterWindow)
	EVT_ERASE_BACKGROUND(C_RenderGLCanvas::OnEraseBackground)
END_EVENT_TABLE()

void C_RenderGLCanvas::Render()
{
	SetCurrent();
	// Init OpenGL once, but after SetCurrent
	if(!gl_init)
	{
		InitGL();
		gl_init = true;
	}

//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glFrustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 3.0f);
//	glMatrixMode(GL_MODELVIEW);

	// clear color and depth buffers, MAGIC LINE! Gets rid of dirty windows
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(sizeWithClient)
	{
		wxSize ClientSize = GetClientSize();
		glViewport(0, 0, ClientSize.x, ClientSize.y);
	}

	switch(m_Equations->equationType)
	{
	case ID_Title:
		RenderTitle();
		break;
	case ID_COMPLEX:
		break;
	case ID_IFS:
		m_Equations->IFS->CoordsGraph(m_Equations->equationName);
		break;
	case ID_AUTOMATA:
		break;
	default:
		break;
	}

	glFlush();
	SwapBuffers();
}

void C_RenderGLCanvas::Calculate()
{
	switch(m_Equations->equationName)
	{
	case ID_SierpinskiTriangle:
		m_Equations->IFS->SierpinskiTriangleCoords();
		break;
	case ID_TriAngles:
		m_Equations->IFS->TriAnglesCoords();
		break;
	case ID_Pentagon:
		m_Equations->IFS->PentagonCoords();
		break;
	default:
		break;
	}

	Render();
}

void C_RenderGLCanvas::InitGL()
{
	SetCurrent();
    // set viewing projection
	glMatrixMode(GL_PROJECTION);
	glFrustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 3.0f);

    // position viewer
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0.0f, 0.0f, -2.0f);

    // position object
//	glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
//	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);

//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
}

void C_RenderGLCanvas::RenderTitle()
{
	glBegin(GL_LINES);
		glColor3ub(255, 0, 0);
		glVertex3f(-1.5f, 0, 0);
		glVertex3f(1.5f, 0, 0);
		glColor3ub(0, 255, 0);
		glVertex3f(0, -1.5f, 0);
		glVertex3f(0, 1.5f, 0);
		glColor3ub(0, 0, 255);
		glVertex3f(0, 0, -1.5f);
		glVertex3f(0, 0, 1.5f);
	glEnd();
	glEndList();
}

void C_RenderGLCanvas::OnEnterWindow(wxMouseEvent& WXUNUSED(event))
{
	SetFocus();
}

void C_RenderGLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	// empty function, to avoid flashing.
}

void C_RenderGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxPaintDC dc(this);
	Render();
}

void C_RenderGLCanvas::OnSize(wxSizeEvent& event)
{
	// Reference to OpenGL sample source to find how to have better cross platform support for resizing
}