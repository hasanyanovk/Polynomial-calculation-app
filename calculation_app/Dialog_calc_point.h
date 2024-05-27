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


class dialog_calc_point : public wxDialog {
private:

protected:
	wxStaticText* expr;
	wxTextCtrl* text_expr;
	wxStaticText* x;
	wxTextCtrl* text_x;
	wxStaticText* y;
	wxTextCtrl* text_y;
	wxStaticText* z;
	wxTextCtrl* text_z;
	wxButton* calc_button;

public:
	wxString ex;
	double X = 0 ,Y = 0, Z = 0;
protected:
	virtual void m_button7OnButtonClick(wxCommandEvent& event);


public:

	dialog_calc_point( wxWindow* parent = nullptr , wxWindowID id = wxID_ANY, const wxString& title = wxT("Calculation in point"),
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(370, 200), long style = wxDEFAULT_DIALOG_STYLE);

	~dialog_calc_point();

};
