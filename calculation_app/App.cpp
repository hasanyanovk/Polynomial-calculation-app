#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP_CONSOLE(App);

bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame;
	mainFrame->SetClientSize(1200, 800);
	mainFrame->Center();
	mainFrame->Show();
	return true;
}