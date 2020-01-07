#ifndef _FIZIKS_FRM_
#define _FIZIKS_FRM_

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include "GLCanvas.h"
#include "PhysicsEngine.h"
#include <wx/thread.h>
//#include <wx/event.h>
//#include "wx/wxprec.h"

//wxDEFINE_EVENT(wxEVT_COMMAND_MYTHREAD_UPDATE, wxThreadEvent)
//wxDECLARE_EVENT(wxEVT_COMMAND_MYTHREAD_UPDATE, wxThreadEvent);

class Fiziks_Frm : public wxFrame, public wxThreadHelper
{
public:
	Fiziks_Frm(const wxString& title);
	~Fiziks_Frm(){m_mgr.UnInit();};

	//void OnThreadUpdate(wxThreadEvent& evt);
	//void OnClose(wxCloseEvent& evt);

//private:
	GLCanvas *m_GLCanvas;
	PhysicsEngine *m_PhysicsEngine;
	wxAuiManager m_mgr;

protected:
	virtual wxThread::ExitCode Entry(){return (wxThread::ExitCode)0;};

private:
	DECLARE_EVENT_TABLE()
};

#endif