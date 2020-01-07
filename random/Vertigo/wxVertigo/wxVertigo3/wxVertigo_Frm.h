#ifndef _WXVERTIGO_FRM_
#define _WXVERTIGO_FRM_

#include <wx.h>
#include <wx/aui/aui.h>
#include "ResourceStream.h"
#include "RenderGLCanvas.h"
#include "Equations.h"
#include "enum.h"

//#include "wx/thread.h"

class C_wxVertigo_Frm : public wxFrame
{
public:

	C_wxVertigo_Frm(const wxString& title, C_Equations *pEquations);
	~C_wxVertigo_Frm(){m_mgr.UnInit();}	// Tell wxAUI to unmanage this frame

	wxMenuBar *menubar;
	wxToolBar *toolbar;

	void menu_Create();
	void toolbar_Create();
	void renderFrame_Create();
	void OnExit(wxCommandEvent& event){exit(true);};
	void OnMenuEquationSelect(wxCommandEvent& event);

private:
	wxAuiManager m_mgr;
	C_Equations *m_Equations;
	C_RenderGLCanvas *m_RenderGLCanvas;

DECLARE_EVENT_TABLE()
};

#endif