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
#include "database.h"
#include "polynom.h"

class dialog_delete : public wxDialog {

protected:

	virtual void m_button13OnButtonClick(wxCommandEvent& event);

public:
	wxStaticText* text_delete;
	wxTextCtrl* name;
	wxButton* button_delete;

	Database<string, Polynom>* db;
	wxTextCtrl* text;

	dialog_delete(Database<string, Polynom>* data, wxTextCtrl* txt, wxWindow* parent = nullptr, wxWindowID id = wxID_ANY, const wxString& title = wxT("Delete polynom"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(284, 90), long style = wxDEFAULT_DIALOG_STYLE);

	~dialog_delete();

};
