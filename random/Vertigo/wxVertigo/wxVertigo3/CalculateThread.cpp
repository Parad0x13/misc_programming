#include "CalculateThread.h"

//wxDEFINE_EVENT(wxEVT_COMMAND_MYTHREAD_UPDATE, wxThreadEvent)
//wxDECLARE_EVENT(wxEVT_COMMAND_MYTHREAD_UPDATE, wxThreadEvent);

void *C_CalculateThread::Entry()
{
	// IMPORTANT:
	// this function gets executed in the secondary thread context!

	// here we do our long task, periodically calling TestDestroy():
	while (!GetThread()->TestDestroy())
	{
		// since this Entry() is implemented in MyFrame context we don't
		// need any pointer to access the m_data, m_processedData, m_dataCS
		// variables... very nice!

		// VERY IMPORTANT: do not call any GUI function inside this
		//                 function; rather use wxQueueEvent():
///		wxQueueEvent(this, new wxThreadEvent(wxEVT_COMMAND_MYTHREAD_UPDATE));
		// we used pointer 'this' assuming it's safe; see OnClose()
	}

	// TestDestroy() returned true (which means the main thread asked us
	// to terminate as soon as possible) or we ended the long task...
	return (wxThread::ExitCode)0;
}

void C_CalculateThread::OnClose(wxCloseEvent&)
{
	// important: before terminating, we _must_ wait for our joinable
	// thread to end, if it's running; in fact it uses variables of this
	// instance and posts events to *this event handler

	if (GetThread() &&      // DoStartALongTask() may have not been called
		GetThread()->IsRunning())
		GetThread()->Wait();

	//Destroy();
}