#include "Dialog_variable.h"
#include "Dialog_error.h"

void dialog_variable::var_buttonOnButtonClick(wxCommandEvent& event) {

	wxString value = m_textCtrl10->GetLineText(0);
	if (value.empty()) throw invalid_argument("Empty value");
	res = value;
	Close();
}

dialog_variable::dialog_variable( wxString name_var, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog(parent, id, title, pos, size, style) , name(name_var){
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* var_sizer;
	var_sizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer(wxHORIZONTAL);

	var_name = new wxStaticText(this, wxID_ANY, wxT("Enter value of " + name), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	var_name->Wrap(-1);
	bSizer19->Add(var_name, 2, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_textCtrl10 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer19->Add(m_textCtrl10, 3, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);


	var_sizer->Add(bSizer19, 1, wxEXPAND, 5);

	var_button = new wxButton(this, wxID_ANY, wxT("Accept"), wxDefaultPosition, wxDefaultSize, 0);
	var_sizer->Add(var_button, 1, wxALL | wxEXPAND, 5);


	this->SetSizer(var_sizer);
	this->Layout();

	this->Centre(wxBOTH);

	var_button->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(dialog_variable::var_buttonOnButtonClick), NULL, this);
}

dialog_variable::~dialog_variable() {
	var_button->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(dialog_variable::var_buttonOnButtonClick), NULL, this);

}