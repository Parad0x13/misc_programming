#include "Fiziks_Frm.h"

//wxDECLARE_EVENT(wxEVT_COMMAND_MYTHREAD_UPDATE, wxThreadEvent);

BEGIN_EVENT_TABLE(Fiziks_Frm, wxFrame)
	//EVT_COMMAND(wxID_ANY, wxEVT_COMMAND_MYTHREAD_UPDATE, Fiziks_Frm::OnThreadUpdate)
	//EVT_CLOSE(Fiziks_Frm::OnClose)
END_EVENT_TABLE()

Fiziks_Frm::Fiziks_Frm(const wxString& title)
: wxFrame(NULL, -1, title, wxDefaultPosition, wxDefaultSize)
{
	m_mgr.SetManagedWindow(this);

	m_PhysicsEngine = new PhysicsEngine();

	m_GLCanvas = new GLCanvas(this, wxDefaultPosition, wxSize(300, 300), _("GLCanvas"), m_PhysicsEngine);
	m_mgr.AddPane(m_GLCanvas, wxAuiPaneInfo().Dockable(false).Caption(_("GLCanvas")).Float());

	m_mgr.Update();

	// This is where I would create a seperate thread to run the Physics Engine
}

//void Fiziks_Frm::OnClose(wxCloseEvent&)
//{
	// important: before terminating, we _must_ wait for our joinable
	// thread to end, if it's running; in fact it uses variables of this
	// instance and posts events to *this event handler

//	if (GetThread() &&      // DoStartALongTask() may have not been called
//		GetThread()->IsRunning())
//		GetThread()->Wait();

//	Destroy();
//}

//void Fiziks_Frm::OnThreadUpdate(wxThreadEvent& evt)
//{
	// Do nothing
//}