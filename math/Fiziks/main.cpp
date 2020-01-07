#include "main.h"
#include "Fiziks_Frm.h"

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit()
{
	Fiziks_Frm *Fiziks = new Fiziks_Frm(_("Fiziks"));
	Fiziks->Show(true);
	//Fiziks->Maximize(true);

	while(true)
	{
		Fiziks->m_PhysicsEngine->RunEngine();
		Fiziks->m_GLCanvas->Render();
		wxMilliSleep(100);
	}

	return true;
}