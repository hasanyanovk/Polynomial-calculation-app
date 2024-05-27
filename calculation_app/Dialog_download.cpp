#include "Dialog_download.h"
#include "Dialog_error.h"

void dialog_download::button_downloadOnButtonClick(wxCommandEvent& event) {
	wxString name = m_textCtrl9->GetLineText(0);
	if (!name.empty()) {
		try {
			ofstream fout(string{ name + ".txt" });
			fout << *db;
			fout.close();
		}
		catch (...) {
			dialog_error* error = new dialog_error("Error with file!");
			error->ShowModal();
			error->Destroy();
		}
		m_textCtrl9->Clear();
		Close();
	}
}

dialog_download::dialog_download(Database<string, Polynom>* data, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog(parent, id, title, pos, size, style), db(data) {
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer(wxHORIZONTAL);

	text_download = new wxStaticText(this, wxID_ANY, wxT("Name of file:"), wxDefaultPosition, wxDefaultSize, 0);
	text_download->Wrap(-1);
	bSizer21->Add(text_download, 1, wxALL, 5);

	m_textCtrl9 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer21->Add(m_textCtrl9, 3, wxEXPAND, 5);

	txt = new wxStaticText(this, wxID_ANY, wxT(".txt"), wxDefaultPosition, wxDefaultSize, 0);
	txt->Wrap(-1);
	bSizer21->Add(txt, 0, wxALL, 5);

	bSizer20->Add(bSizer21, 1, wxEXPAND, 5);

	button_download = new wxButton(this, wxID_ANY, wxT("Download"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer20->Add(button_download, 1, wxEXPAND, 5);


	this->SetSizer(bSizer20);
	this->Layout();

	this->Centre(wxBOTH);

	button_download->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(dialog_download::button_downloadOnButtonClick), NULL, this);
}

dialog_download::~dialog_download() {

	button_download->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(dialog_download::button_downloadOnButtonClick), NULL, this);

}