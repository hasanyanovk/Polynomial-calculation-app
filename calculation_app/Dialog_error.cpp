#pragma once
#include "Dialog_error.h"


void dialog_error::button_okOnButtonClick(wxCommandEvent& event) { Close(); }

dialog_error::dialog_error(wxString name, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog(parent, id, title, pos, size, style) {
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer(wxVERTICAL);

	bSizer22->Add(0, 0, 2, wxEXPAND, 5);
	

	m_staticText8 = new wxStaticText(this, wxID_ANY, name, wxDefaultPosition, wxDefaultSize, 0);
	m_staticText8->Wrap(-1);
	m_staticText8->SetFont(wxFont(18, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Modern No. 20")));
	m_staticText8->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));

	bSizer22->Add(m_staticText8, 1, wxALIGN_CENTER_HORIZONTAL, 5);

	bSizer22->Add(0, 0, 1, wxEXPAND, 5);
	button_ok = new wxButton(this, wxID_ANY, wxT("Okay"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer22->Add(button_ok, 1, wxEXPAND, 5);


	this->SetSizer(bSizer22);
	this->Layout();

	this->Centre(wxBOTH);

	button_ok->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(dialog_error::button_okOnButtonClick), NULL, this);
}

dialog_error::~dialog_error() {
	button_ok->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(dialog_error::button_okOnButtonClick), NULL, this);

}