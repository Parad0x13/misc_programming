#include "wxVertigo_Frm.h"

#include "CalculateThread.h"

wxDEFINE_EVENT(wxEVT_COMMAND_MYTHREAD_UPDATE, wxThreadEvent)

// Declared in CalculateThread.h
//DEFINE_EVENT_TYPE(wxEVT_THREAD)

BEGIN_EVENT_TABLE(C_wxVertigo_Frm, wxFrame)
	EVT_MENU(ID_Exit, C_wxVertigo_Frm::OnExit)
END_EVENT_TABLE()

C_wxVertigo_Frm::C_wxVertigo_Frm(const wxString& title, C_Equations *pEquations)
: wxFrame(NULL, -1, title, wxDefaultPosition, wxDefaultSize)
{
	m_Equations = pEquations;
	// Tell wxAui to manage this window
	m_mgr.SetManagedWindow(this);

	menu_Create();
	SetMenuBar(menubar);
	toolbar_Create();
	CreateStatusBar();
	SetStatusText("Welcome to wxVertigo!");

	renderFrame_Create();

	m_mgr.Update();
};

void C_wxVertigo_Frm::renderFrame_Create()
{
	// For some reason when you move the AUI Pane really fast it won't update the canvas, but it will with a regular window
	m_RenderGLCanvas = new C_RenderGLCanvas(this, m_Equations, wxID_ANY, wxDefaultPosition, wxSize(320, 200));
	m_mgr.AddPane(m_RenderGLCanvas,
		wxAuiPaneInfo().
		Left().
		Caption(_("RenderGLCanvas")).
		Float().
		FloatingPosition(100, 100).
		Dockable(false).
		Layer(1).
		Position(1).
		MaximizeButton(true));
	m_mgr.Update();
};

void C_wxVertigo_Frm::menu_Create()
{
	// Begin Menu Construction
	menubar = new wxMenuBar;
	wxMenu *menuFile = new wxMenu;
	wxMenu *menuFractals = new wxMenu;
	wxMenu *menuIFS = new wxMenu;
	wxMenu *menuAutomata = new wxMenu;

	menubar->Append(menuFile, "&File");
		menuFile->Append(ID_Exit, "E&xit...");
	menubar->Append(menuFractals, "F&ractals");
		wxMenu *menuComplex = new wxMenu;
		menuFractals->AppendSubMenu(menuComplex, "&Complex");
			menuComplex->Append(ID_Mandelbrot, "&Mandelbrot");
			menuComplex->Enable(ID_Mandelbrot, false);
			Connect(ID_Mandelbrot, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(C_wxVertigo_Frm::OnMenuEquationSelect));
		menubar->Append(menuIFS, "&IFS");
			menuIFS->Append(ID_SierpinskiTriangle, "Sierpinski's &Triangle");
			Connect(ID_SierpinskiTriangle, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(C_wxVertigo_Frm::OnMenuEquationSelect));
			menuIFS->Append(ID_TriAngles, "Tri&Angles");
			Connect(ID_TriAngles, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(C_wxVertigo_Frm::OnMenuEquationSelect));
			menuIFS->Append(ID_Pentagon, "&Pentagon");
			Connect(ID_Pentagon, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(C_wxVertigo_Frm::OnMenuEquationSelect));
		menubar->Append(menuAutomata, "&Automata");
			menuAutomata->Append(ID_LangtonsAnt, "&Langtons Ant");
			Connect(ID_LangtonsAnt, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(C_wxVertigo_Frm::OnMenuEquationSelect));
	// End Menu Construction
};

void C_wxVertigo_Frm::toolbar_Create()
{
	// Begin Load of images from resource
	wxImage exit_icon(*GetResourceInputStream("stop_32", "PNG"));
	//wxImage wxVertigo_Icon_Blue(*GetResourceInputStream("wxVertigo_Icon_Blue", "PNG"));

	// Begin Toolbar Construction
	toolbar = CreateToolBar();
	toolbar->AddTool(ID_Exit, wxBitmap(exit_icon), _("Exit Application"));
	//toolbar->AddTool(ID_Test, wxBitmap(wxVertigo_Icon_Blue), _("Do Something?"));
	// End Toolbar Construction

	toolbar->Realize();

	// Enable commands for the tools, not at the EVT_TABLE
	Connect(ID_Exit, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(C_wxVertigo_Frm::OnExit));
};

void C_wxVertigo_Frm::OnMenuEquationSelect(wxCommandEvent& event)
{
/*	switch(event.GetId())
	{
	case ID_Mandelbrot:
		break;

	case ID_SierpinskiTriangle:
		m_Equations->equationName = ID_SierpinskiTriangle;
		m_Equations->equationType = ID_IFS;
		break;

	case ID_TriAngles:
		break;

	case ID_Pentagon:
		break;

	case ID_LangtonsAnt:
		break;

	default:
		break;
	}*/
	if(	event.GetId() == ID_SierpinskiTriangle ||
		event.GetId() == ID_TriAngles ||
		event.GetId() == ID_Pentagon){
			m_Equations->equationName = event.GetId();
			m_Equations->equationType = ID_IFS;
	}

	m_RenderGLCanvas->Calculate();
}