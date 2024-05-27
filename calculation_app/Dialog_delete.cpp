#include "Dialog_delete.h"
#include "Dialog_error.h"
#include <sstream>

void dialog_delete::m_button13OnButtonClick(wxCommandEvent& event) {
	wxString _name = name->GetLineText(0);
	if (_name.empty()) return;
	try {
		db->remove(_name);
		stringstream ss;
		ss << *db;
		text->Clear();
		text->WriteText("Database:\t");
		text->WriteText(ss.str());
	}
	catch (...) {
		dialog_error* error = new dialog_error;
		error->ShowModal();
		error->Destroy();
	}
	name->Clear();
	Close();
}

dialog_delete::dialog_delete(Database<string, Polynom>* data, wxTextCtrl* txt, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog(parent, id, title, pos, size, style), db(data) , text(txt) {
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer(wxHORIZONTAL);

	text_delete = new wxStaticText(this, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0);
	text_delete->Wrap(-1);
	text_delete->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));

	bSizer19->Add(text_delete, 1, wxALL, 5);

	name = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer19->Add(name, 3, wxEXPAND, 5);


	bSizer18->Add(bSizer19, 1, wxEXPAND, 5);

	button_delete = new wxButton(this, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer18->Add(button_delete, 1, wxEXPAND, 9);


	this->SetSizer(bSizer18);
	this->Layout();

	this->Centre(wxBOTH);

	button_delete->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(dialog_delete::m_button13OnButtonClick), NULL, this);
}

dialog_delete::~dialog_delete() {
	button_delete->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(dialog_delete::m_button13OnButtonClick), NULL, this);

}