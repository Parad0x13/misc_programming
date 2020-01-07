#ifndef _GLCANVAS_
#define _GLCANVAS_

#include "wx/wxprec.h"
#include "wx/glcanvas.h"
#include "PhysicsEngine.h"

class GLCanvas : public wxGLCanvas
{
public:
	GLCanvas(wxWindow *parent, const wxPoint& pos, const wxSize& size, const wxString& title, PhysicsEngine *pPhysicsEngine);
	~GLCanvas(){};

	void Render();
	void InitGL();
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnEnterWindow(wxMouseEvent& event);
	void OnEraseBackground(wxEraseEvent& event);

private:
	PhysicsEngine *m_PhysicsEngine;
	bool gl_init;

	DECLARE_EVENT_TABLE()
};

#endif