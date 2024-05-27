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

#include "polynom.h"

class dialog_variable : public wxDialog {

protected:
	wxStaticText* var_name;
	wxTextCtrl* m_textCtrl10;
	wxButton* var_button;
	wxString name;


	virtual void var_buttonOnButtonClick(wxCommandEvent& event);


public:
	wxString res;
	dialog_variable(wxString name_var, wxWindow* parent = nullptr, wxWindowID id = wxID_ANY, const wxString& title = wxT("Variable"), 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(350, 120), long style = wxDEFAULT_DIALOG_STYLE);

	~dialog_variable();

};