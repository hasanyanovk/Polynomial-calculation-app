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
#include <wx/listctrl.h>
#include "database.h"
#include "polynom.h"
class dialog_save : public wxDialog {

protected:
	virtual void button_saveOnButtonClick(wxCommandEvent& event);

public:
	wxStaticText* text_name;
	wxTextCtrl* name;
	wxStaticText* text_value;
	wxTextCtrl* value;
	wxButton* button_save;
	Database<string, Polynom>* db;
	wxTextCtrl* text;
	dialog_save(Database<string, Polynom>* data, wxTextCtrl* txt, wxWindow* parent = nullptr, wxWindowID id = wxID_ANY, const wxString& title = wxT("Save polynom"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(353, 120), long style = wxDEFAULT_DIALOG_STYLE) ;
	~dialog_save();

};