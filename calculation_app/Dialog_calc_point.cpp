#include "Dialog_calc_point.h"
#include "Dialog_error.h"
#include <string>


void dialog_calc_point::m_button7OnButtonClick(wxCommandEvent& event) {
	ex = text_expr->GetLineText(0);
	bool flag = 1;
	try {
		X = std::stod(std::string{ text_x->GetLineText(0) });
		Y = std::stod(std::string{ text_y->GetLineText(0) });
		Z = std::stod(std::string{ text_z->GetLineText(0) });
	}
	catch (...) {
		dialog_error* error = new dialog_error("Wrong value of variables!");
		error->ShowModal();
		error->Destroy();
	}
	Close();
}

dialog_calc_point::dialog_calc_point(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog(parent, id, title, pos, size, style) {
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer(wxHORIZONTAL);

	expr = new wxStaticText(this, wxID_ANY, wxT("Expression:"), wxDefaultPosition, wxDefaultSize, 0);
	expr->Wrap(-1);
	bSizer14->Add(expr, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);

	text_expr = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer14->Add(text_expr, 3, wxALIGN_CENTER | wxALL, 5);

	bSizer13->Add(bSizer14, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer(wxHORIZONTAL);

	x = new wxStaticText(this, wxID_ANY, wxT("Enter value of X:"), wxDefaultPosition, wxDefaultSize, 0);
	x->Wrap(-1);
	bSizer15->Add(x, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);

	text_x = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0);
	bSizer15->Add(text_x, 3, wxALIGN_CENTER | wxALL, 5);

	bSizer13->Add(bSizer15, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer(wxHORIZONTAL);

	y = new wxStaticText(this, wxID_ANY, wxT("Enter value of Y:"), wxDefaultPosition, wxDefaultSize, 0);
	y->Wrap(-1);
	bSizer16->Add(y, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);

	text_y = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0);
	bSizer16->Add(text_y, 3, wxALIGN_CENTER | wxALL, 5);

	bSizer13->Add(bSizer16, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer(wxHORIZONTAL);

	z = new wxStaticText(this, wxID_ANY, wxT("Enter value of Z:"), wxDefaultPosition, wxDefaultSize, 0);
	z->Wrap(-1);
	bSizer17->Add(z, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);

	text_z = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0);
	bSizer17->Add(text_z, 3, wxALIGN_CENTER | wxALL, 5);

	bSizer13->Add(bSizer17, 1, wxEXPAND, 5);

	calc_button = new wxButton(this, wxID_ANY, wxT("Calculate"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer13->Add(calc_button, 0, wxEXPAND, 5);

	this->SetSizer(bSizer13);
	this->Layout();

	this->Centre(wxBOTH);

	calc_button->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(dialog_calc_point::m_button7OnButtonClick), NULL, this);
}

dialog_calc_point::~dialog_calc_point() {
	
	calc_button->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(dialog_calc_point::m_button7OnButtonClick), NULL, this);
}