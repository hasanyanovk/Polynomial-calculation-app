#pragma once
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/dialog.h>


class dialog_error : public wxDialog {

protected:
	wxStaticText* m_staticText8;
	wxButton* button_ok;

	virtual void button_okOnButtonClick(wxCommandEvent& event);


public:

	dialog_error(wxString name = "Error!!!", wxWindow* parent = nullptr, wxWindowID id = wxID_ANY, const wxString& title = wxT("Error"),
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(381, 276), long style = wxDEFAULT_DIALOG_STYLE);

	~dialog_error();

};
