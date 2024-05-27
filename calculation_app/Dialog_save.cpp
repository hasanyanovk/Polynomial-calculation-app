#include "Dialog_save.h"
#include "Dialog_error.h"
#include <sstream>



void dialog_save::button_saveOnButtonClick(wxCommandEvent& event) {	

	wxString _val = value->GetLineText(0);
	wxString _name = name->GetLineText(0);
	if (!_val.empty() && !_name.empty()) {
		try {
			Polynom p(_val);
			db->emplace(name->GetLineText(0), p);
			stringstream ss;
			ss << *db;
			text->Clear();
			text->WriteText("Database:\t");
			text->WriteText( ss.str());
			
		}
		catch (invalid_argument) {
			dialog_error* error = new dialog_error("Wrong expression!");
			error->ShowModal();
			error->Destroy();
		}
		catch (runtime_error) {
			dialog_error* error = new dialog_error("Name already used!");
			error->ShowModal();
			error->Destroy();
		}
		catch (...) {
			dialog_error* error = new dialog_error("Error!");
			error->ShowModal();
			error->Destroy();
		}

	}
	name->Clear();
	value->Clear();
	Close();
}

dialog_save::dialog_save(Database<string, Polynom>* data, wxTextCtrl* txt, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog(parent, id, title, pos, size, style), db(data), text(txt) {
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* save_name;
	save_name = new wxBoxSizer(wxHORIZONTAL);

	text_name = new wxStaticText(this, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0);
	text_name->Wrap(-1);
	text_name->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));

	save_name->Add(text_name, 1, wxALL, 5);

	name = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	save_name->Add(name, 3, wxEXPAND, 5);


	bSizer14->Add(save_name, 1, wxEXPAND, 5);

	wxBoxSizer* save_value;
	save_value = new wxBoxSizer(wxHORIZONTAL);

	text_value = new wxStaticText(this, wxID_ANY, wxT("Value:"), wxDefaultPosition, wxDefaultSize, 0);
	text_value->Wrap(-1);
	text_value->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));

	save_value->Add(text_value, 1, wxALL, 5);

	value = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	save_value->Add(value, 3, wxEXPAND, 5);


	bSizer14->Add(save_value, 1, wxEXPAND, 5);

	button_save = new wxButton(this, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer14->Add(button_save, 1, wxEXPAND, 5);


	this->SetSizer(bSizer14);
	this->Layout();

	this->Centre(wxBOTH);


	button_save->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(dialog_save::button_saveOnButtonClick), NULL, this);
}

dialog_save::~dialog_save() {

	button_save->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(dialog_save::button_saveOnButtonClick), NULL, this);

}