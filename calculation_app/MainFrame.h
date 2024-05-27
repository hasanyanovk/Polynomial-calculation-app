#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/listctrl.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/frame.h>


#include "Dialog_delete.h"
#include "Dialog_save.h"
#include "Dialog_error.h"
#include "Dialog_download.h"
#include "Dialog_calc_point.h"
#include "Dialog_variable.h"
#include "postfix.h"
#include "database.h"
#include <fstream>
#include <sstream>



class MainFrame : public wxFrame {
protected:
	enum Mode {
		Polynomial,
		Digit
	};
	wxStaticBitmap* main_image;
	wxStaticText* label_input;
	wxTextCtrl* input_text;
	wxButton* button_calculate;
	wxListCtrl* output_text;
	wxStaticText* m_staticText1;
	wxListCtrl* history_text;
	wxTextCtrl* database_text;
	wxMenuBar* m_menubar1;
	wxMenu* m_menu2;
	wxMenu* menu_active;
	wxMenu* m_menu1;
	wxMenu* m_menu_clear;
	wxMenu* menu_help;
	wxStatusBar* statusbar;
	Mode mode = Mode::Polynomial;
	Database<string, Polynom>* database = new Database<string, Polynom>;
	
	dialog_save* save;
	dialog_delete* del;
	dialog_download* download;
	
	void set_active(const wxString& s);

	virtual void button_calculateOnButtonClick(wxCommandEvent& event);
	virtual void SaveOnMenuSelection(wxCommandEvent& event);
	virtual void DeleteOnMenuSelection(wxCommandEvent& event);
	virtual void DownloadOnMenuSelection(wxCommandEvent& event);
	virtual void ExitOnMenuSelection(wxCommandEvent& event) { Destroy(); }
	virtual void ArrayTableOnMenuSelection(wxCommandEvent& event) { set_active("ArrayTable"); }
	virtual void ListTableOnMenuSelection(wxCommandEvent& event) { set_active("ListTable");; }
	virtual void SortedArrayTableOnMenuSelection(wxCommandEvent& event) { set_active("SortedArrayTable");; }
	virtual void HashArrayTableOnMenuSelection(wxCommandEvent& event) { set_active("HashArrayTable");; }
	virtual void HashListTableOnMenuSelection(wxCommandEvent& event) { set_active("HashListTable");; }
	virtual void TreeTableOnMenuSelection(wxCommandEvent& event) { set_active("TreeTable");; }
	virtual void PolynomialOnMenuSelection(wxCommandEvent& event);
	virtual void DigitsOnMenuSelection(wxCommandEvent& event);
	virtual void m_menuItem13OnMenuSelection(wxCommandEvent& event) { system("start helper_polynom.txt"); }
	virtual void m_menuItem14OnMenuSelection(wxCommandEvent& event) { system("start helper_double.txt"); }
	virtual void menu_calcOnMenuSelection(wxCommandEvent& event);
	virtual void HistoryOnMenuSelection(wxCommandEvent& event);
	virtual void DatabaseOnMenuSelection(wxCommandEvent& event);

public:

	MainFrame(wxWindow* parent = nullptr, wxWindowID id = wxID_ANY, const wxString& title = wxT("Polynomial calculator"), 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(1100, 700), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);

	~MainFrame();

};