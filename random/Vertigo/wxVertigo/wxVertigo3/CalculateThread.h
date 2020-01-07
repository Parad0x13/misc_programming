#ifndef _CALCULATETHREAD_
#define _CALCULATETHREAD_

#include "wx.h"
#include "enum.h"
#include "Equations.h"
#include "wx/thread.h"

//#include "wx/event.h"

//#define wxEVT_COMMAND_MYTHREAD_UPDATE -1

//DECLARE_EVENT_TYPE(wxEVT_THREAD, -1)
//wxDEFINE_EVENT(wxEVT_COMMAND_MYTHREAD_UPDATE, wxThreadEvent)
//wxDECLARE_EVENT(wxEVT_COMMAND_MYTHREAD_UPDATE, wxThreadEvent);

class C_CalculateThread : public wxThreadHelper
{
public:
	C_CalculateThread(wxEvtHandler *ehParent, C_Equations *p_Equations, int equation)
	{
		m_ehParent = ehParent;
		m_Equations = p_Equations;
		Equation = equation;
	};
	~C_CalculateThread(){};

	//void OnThreadUpdate(wxThreadEvent& evt){};
	//void OnThreadUpdate(wxEVT_THREAD& evt){};
	void OnExit()
	{
		wxCommandEvent evt(wxEVT_THREAD, wxID_ANY);
		evt.SetId(ID_Thread_Exit);
		m_ehParent->AddPendingEvent(evt);
	}
	void OnClose(wxCloseEvent&);

private:
	int Equation;
	void *Entry();
	C_Equations *m_Equations;
	wxEvtHandler *m_ehParent;

//DECLARE_EVENT_TABLE()
};

#endif