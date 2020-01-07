#ifndef _RENDERGLCANVAS_
#define _RENDERGLCANVAS_

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"
#include "wx/glcanvas.h"
#include "Equations.h"

class C_RenderGLCanvas: public wxGLCanvas
{
public:
	C_RenderGLCanvas(wxWindow *parent, C_Equations *pEquations, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0, const wxString& name = _T("RenderGLCanvas"));
	~C_RenderGLCanvas(){};

	void InitGL();
	void Render();
	void RenderTitle();
	void Calculate();
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnEnterWindow(wxMouseEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
//	void OnExit(wxCommandEvent& event){exit(true);};

private:
	C_Equations *m_Equations;
	bool gl_init;
	bool sizeWithClient;

	DECLARE_EVENT_TABLE()
};

#endif