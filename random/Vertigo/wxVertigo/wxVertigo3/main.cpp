#include "main.h"
#include "rand.h"
#include "Equations.h"
#include "wxVertigo_Frm.h"

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit()
{
	initImageHandlers();
	seed();	// Seed random generator
	C_Equations *Equations = new C_Equations();
	C_wxVertigo_Frm *wxVertigo_Frm = new C_wxVertigo_Frm(_T("wxVertigo"), Equations);

	wxVertigo_Frm->Show(true);
	wxVertigo_Frm->Maximize(true);

	return true;
}

void MainApp::initImageHandlers()
{
	wxImage::AddHandler(new wxPNGHandler());
};