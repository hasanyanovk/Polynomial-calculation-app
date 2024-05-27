#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/dialog.h>

#include "polynom.h"
#include "database.h"

class dialog_download : public wxDialog {

protected:
	virtual void button_downloadOnButtonClick(wxCommandEvent& event);

public:
	wxStaticText* text_download;
	wxTextCtrl* m_textCtrl9;
	wxButton* button_download;
	wxStaticText* txt;
	Database<string, Polynom>* db;
	dialog_download(Database<string, Polynom>* data, wxWindow* parent = nullptr, wxWindowID id = wxID_ANY, const wxString& title = wxT("Download database"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(312, 94), long style = wxDEFAULT_DIALOG_STYLE);

	~dialog_download();

};