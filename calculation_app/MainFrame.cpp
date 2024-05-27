#include "MainFrame.h"




void MainFrame::button_calculateOnButtonClick(wxCommandEvent& event) {
	wxString  s = input_text->GetLineText(0);
	if (!s.empty()) {
		try {	
			std::stringstream ss;
			output_text->ClearAll();
			if (mode == Mode::Polynomial) {
				polish_notation::Postfix<Polynom> postfix(s);
				auto map = postfix.get_operands();
				for (auto& a : map) {
					if (a.second.empty()) {
						if (database->include(a.first))
							a.second = database->operator[](a.first);
						else {
							bool flag = 1;
							while (flag) {
								try {
									dialog_variable* var = new dialog_variable(a.first);
									var->ShowModal();
									a.second = Polynom{ var->res };
									flag = 0;
									var->Destroy();
								}
								catch (...) {
									dialog_error* error = new dialog_error("Wrong expression!");
									error->ShowModal();
									error->Destroy();
								}
							}
						}
					}
				}
				ss << postfix(map);
				output_text->InsertItem(0, "Input: " + postfix.get_infix());
				output_text->InsertItem(1, "Postfix form: " + postfix.get_postfix());
				output_text->InsertItem(2, "Calculated value: " + ss.str());
				history_text->InsertItem(0, s + "  ->  " + ss.str() + ";");
			}
			else if (mode == Mode::Digit) {

				polish_notation::Postfix<double> postfix(s);
				auto map = postfix.get_operands();
				for (auto& op : map) {
					if (isalpha(op.first[0])) {
						bool flag = 1;
						while (flag) {
							try {
								dialog_variable* var = new dialog_variable(op.first);
								var->ShowModal();
								op.second = stod(string{ var->res });
								flag = 0;
								var->Destroy();
							}
							catch (...) {
								dialog_error* error = new dialog_error("Wrong expression!");
								error->ShowModal();
								error->Destroy();
							}
						}
					}
				}
				ss << postfix(map);
				output_text->InsertItem(0, "Input: " + postfix.get_infix());
				output_text->InsertItem(1, "Postfix form: " + postfix.get_postfix());
				output_text->InsertItem(2, "Calculated value: " + ss.str());
				history_text->InsertItem(0, s + "  ->  " + ss.str() + ";");
			}
		}
		catch (...) {
			dialog_error* error = new dialog_error("Wrong expression!");
			error->ShowModal();
			error->Destroy();
		}
	}
}

void MainFrame::menu_calcOnMenuSelection(wxCommandEvent& event) {
	if (mode == Mode::Polynomial) {
		dialog_calc_point* calc = new dialog_calc_point;
		calc->ShowModal();
		try {
			std::stringstream ss;
			output_text->ClearAll();
			polish_notation::Postfix<Polynom> postfix(calc->ex);
			auto map = postfix.get_operands();
			for (auto& a : map) {
				if (a.second.empty()) {
					if (database->include(a.first))
						a.second = database->operator[](a.first);
					else {
						bool flag = 1;
						while (flag) {
							try {
								dialog_variable* var = new dialog_variable(a.first);
								var->ShowModal();
								a.second = Polynom{ var->res };
								flag = 0;
								var->Destroy();
							}
							catch (...) {
								dialog_error* error = new dialog_error("Wrong expression!");
								error->ShowModal();
								error->Destroy();
							}
						}
					}
				}
				
			}
			const Polynom& p = postfix(map);
			Point point{ calc->X, calc->Y, calc->Z };
			ss << Polynom::ternary_operations["="](p, point);
			input_text->SetLabelText(postfix.get_infix());
			output_text->InsertItem(0, "Input: " + postfix.get_infix());
			output_text->InsertItem(1, "Postfix form: " + postfix.get_postfix());
			output_text->InsertItem(2, "Calculated value: " + ss.str());
			history_text->InsertItem(0, calc->ex + point.str() + +"  ->  " + ss.str() + ";");
		}
		catch (...) {
			dialog_error* error = new dialog_error("Wrong expression!");
			error->ShowModal();
			error->Destroy();
		}
		calc->Destroy();
	}
}

void MainFrame::DeleteOnMenuSelection(wxCommandEvent& event) {
	del->ShowModal();
}

void MainFrame::SaveOnMenuSelection(wxCommandEvent& event) {
	save->ShowModal();
}

void MainFrame::HistoryOnMenuSelection(wxCommandEvent& event) {
	history_text->ClearAll();
}

void MainFrame::DatabaseOnMenuSelection(wxCommandEvent& event) {
	Database<string, Polynom>* tmp = new Database<string, Polynom>;
	delete database;
	database = tmp;
	save->db = database;
	del->db = database;
	download->db = database;
	stringstream ss;
	ss << *database;
	database_text->Clear();
	database_text->WriteText("Database:\t");
	database_text->WriteText(ss.str());
}

void MainFrame::set_active(const wxString& s) {
	try {
		database->set_active(s);
		stringstream ss;
		ss << *database;
		database_text->Clear();
		database_text->WriteText("Database:\t");
		database_text->WriteText(ss.str());
	}
	catch (invalid_argument) {
		dialog_error* error = new dialog_error("Invalid name of table");
		error->Show();
	}
}

void MainFrame::DownloadOnMenuSelection(wxCommandEvent& event) {
	download->ShowModal();
}

void MainFrame::PolynomialOnMenuSelection(wxCommandEvent& event)  { 
	statusbar->SetStatusText("Polynomial mode"); 
	mode = Mode::Polynomial;
	m_menu2->Enable(404, 1);
}

void MainFrame::DigitsOnMenuSelection(wxCommandEvent& event) { 
	statusbar->SetStatusText("Digits mode"); 
	mode = Mode::Digit;
	m_menu2->Enable(404, 0);
}

MainFrame::MainFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style){
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	this->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));

	statusbar = this->CreateStatusBar(1, wxSTB_SIZEGRIP, wxID_ANY);
	statusbar->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
	statusbar->SetStatusText("Polynomial mode");

	wxBoxSizer* main_layout;
	main_layout = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* left_layout;
	left_layout = new wxBoxSizer(wxVERTICAL);
	wxPNGHandler* handler = new wxPNGHandler;
	wxImage::AddHandler(handler);
	main_image = new wxStaticBitmap(this, wxID_ANY, wxBitmap("image.png", wxBITMAP_TYPE_PNG), wxDefaultPosition, wxDefaultSize, 0);
	left_layout->Add(main_image, 42, wxEXPAND);
	

	label_input = new wxStaticText(this, wxID_ANY, wxT("Input expression:"), wxDefaultPosition, wxDefaultSize, 0);
	label_input->Wrap(-1);
	label_input->SetFont(wxFont(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial Rounded MT Bold")));
	label_input->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
	label_input->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVEBORDER));

	left_layout->Add(label_input, 1, wxEXPAND, 5);

	wxBoxSizer* layout_input;
	layout_input = new wxBoxSizer(wxHORIZONTAL);

	input_text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	layout_input->Add(input_text, 5, wxEXPAND, 5);

	button_calculate = new wxButton(this, wxID_ANY, wxT("Calculate"), wxDefaultPosition, wxDefaultSize, 0);
	layout_input->Add(button_calculate, 2, wxEXPAND, 5);


	left_layout->Add(layout_input, 1, wxEXPAND, 5);

	output_text = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_LIST);
	left_layout->Add(output_text, 13, wxEXPAND, 5);


	main_layout->Add(left_layout, 7, wxEXPAND, 5);

	wxBoxSizer* right_layout;
	right_layout = new wxBoxSizer(wxVERTICAL);

	m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("History"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(-1);
	m_staticText1->SetFont(wxFont(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial Unicode MS")));
	m_staticText1->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
	m_staticText1->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVEBORDER));

	right_layout->Add(m_staticText1, 0, wxEXPAND, 5);

	history_text = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_LIST);
	right_layout->Add(history_text, 3, wxEXPAND, 5);

	database_text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
	database_text->WriteText("Database: ");
	right_layout->Add(database_text, 1, wxEXPAND, 5);

	save = new dialog_save(database, database_text);
	
	del = new dialog_delete(database, database_text);

	download = new dialog_download(database);

	main_layout->Add(right_layout, 4, wxEXPAND, 5);


	this->SetSizer(main_layout);
	this->Layout();
	m_menubar1 = new wxMenuBar(0);
	m_menu2 = new wxMenu();
	wxMenuItem* Save;
	Save = new wxMenuItem(m_menu2, wxID_ANY, wxString(wxT("Save")), wxEmptyString, wxITEM_NORMAL);
	m_menu2->Append(Save);

	wxMenuItem* Delete;
	Delete = new wxMenuItem(m_menu2, wxID_ANY, wxString(wxT("Delete")), wxEmptyString, wxITEM_NORMAL);
	m_menu2->Append(Delete);

	wxMenuItem* Download;
	Download = new wxMenuItem(m_menu2, wxID_ANY, wxString(wxT("Download")), wxEmptyString, wxITEM_NORMAL);
	m_menu2->Append(Download);

	m_menu2->AppendSeparator();

	wxMenuItem* menu_calc;
	menu_calc = new wxMenuItem(m_menu2, 404, wxString(wxT("Calculate in point")), wxEmptyString, wxITEM_NORMAL);
	m_menu2->Append(menu_calc);

	m_menu2->AppendSeparator();

	wxMenuItem* Exit;
	Exit = new wxMenuItem(m_menu2, wxID_ANY, wxString(wxT("Exit")), wxEmptyString, wxITEM_NORMAL);
	m_menu2->Append(Exit);

	m_menubar1->Append(m_menu2, wxT("Tools"));

	menu_active = new wxMenu();
	wxMenuItem* ArrayTable;
	ArrayTable = new wxMenuItem(menu_active, wxID_ANY, wxString(wxT("ArrayTable")), wxEmptyString, wxITEM_NORMAL);
	menu_active->Append(ArrayTable);

	wxMenuItem* ListTable;
	ListTable = new wxMenuItem(menu_active, wxID_ANY, wxString(wxT("ListTable")), wxEmptyString, wxITEM_NORMAL);
	menu_active->Append(ListTable);

	wxMenuItem* SortedArrayTable;
	SortedArrayTable = new wxMenuItem(menu_active, wxID_ANY, wxString(wxT("SortedArrayTable")), wxEmptyString, wxITEM_NORMAL);
	menu_active->Append(SortedArrayTable);

	wxMenuItem* HashArrayTable;
	HashArrayTable = new wxMenuItem(menu_active, wxID_ANY, wxString(wxT("HashArrayTable")), wxEmptyString, wxITEM_NORMAL);
	menu_active->Append(HashArrayTable);


	wxMenuItem* HashListTable;
	HashListTable = new wxMenuItem(menu_active, wxID_ANY, wxString(wxT("HashListTable")), wxEmptyString, wxITEM_NORMAL);
	menu_active->Append(HashListTable);

	wxMenuItem* TreeTable;
	TreeTable = new wxMenuItem(menu_active, wxID_ANY, wxString(wxT("TreeTable")), wxEmptyString, wxITEM_NORMAL);
	menu_active->Append(TreeTable);

	m_menubar1->Append(menu_active, wxT("Active table"));

	m_menu1 = new wxMenu();
	wxMenuItem* Polynomial;
	Polynomial = new wxMenuItem(m_menu1, wxID_ANY, wxString(wxT("Polynomial")), wxEmptyString, wxITEM_NORMAL);
	m_menu1->Append(Polynomial);

	wxMenuItem* Digits;
	Digits = new wxMenuItem(m_menu1, wxID_ANY, wxString(wxT("Digits")), wxEmptyString, wxITEM_NORMAL);
	m_menu1->Append(Digits);

	m_menubar1->Append(m_menu1, wxT("Mode"));

	m_menu_clear = new wxMenu();
	wxMenuItem* History;
	History = new wxMenuItem(m_menu1, wxID_ANY, wxString(wxT("History")), wxEmptyString, wxITEM_NORMAL);
	m_menu_clear->Append(History);

	wxMenuItem* Database;
	Database = new wxMenuItem(m_menu1, wxID_ANY, wxString(wxT("Database")), wxEmptyString, wxITEM_NORMAL);
	m_menu_clear->Append(Database);

	m_menubar1->Append(m_menu_clear, wxT("Clear"));

	menu_help = new wxMenu();
	wxMenuItem* m_menuItem13;
	m_menuItem13 = new wxMenuItem(menu_help, wxID_ANY, wxString(wxT("Help polynom")), wxEmptyString, wxITEM_NORMAL);
	menu_help->Append(m_menuItem13);

	wxMenuItem* m_menuItem14;
	m_menuItem14 = new wxMenuItem(menu_help, wxID_ANY, wxString(wxT("Help digit")), wxEmptyString, wxITEM_NORMAL);
	menu_help->Append(m_menuItem14);

	m_menubar1->Append(menu_help, wxT("Help"));

	this->SetMenuBar(m_menubar1);


	this->Centre(wxBOTH);


	button_calculate->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::button_calculateOnButtonClick), NULL, this);
	m_menu2->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::SaveOnMenuSelection), this, Save->GetId());
	m_menu2->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::DeleteOnMenuSelection), this, Delete->GetId());
	m_menu2->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::DownloadOnMenuSelection), this, Download->GetId());
	m_menu2->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::ExitOnMenuSelection), this, Exit->GetId());
	m_menu2->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::menu_calcOnMenuSelection), this, menu_calc->GetId());
	menu_active->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::ArrayTableOnMenuSelection), this, ArrayTable->GetId());
	menu_active->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::ListTableOnMenuSelection), this, ListTable->GetId());
	menu_active->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::SortedArrayTableOnMenuSelection), this, SortedArrayTable->GetId());
	menu_active->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::HashArrayTableOnMenuSelection), this, HashArrayTable->GetId());
	menu_active->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::HashListTableOnMenuSelection), this, HashListTable->GetId());
	menu_active->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::TreeTableOnMenuSelection), this, TreeTable->GetId());
	m_menu_clear->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::HistoryOnMenuSelection), this, History->GetId());
	m_menu_clear->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::DatabaseOnMenuSelection), this, Database->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::PolynomialOnMenuSelection), this, Polynomial->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::DigitsOnMenuSelection), this, Digits->GetId());
	menu_help->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::m_menuItem13OnMenuSelection), this, m_menuItem13->GetId());
	menu_help->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::m_menuItem14OnMenuSelection), this, m_menuItem14->GetId());
}

MainFrame::~MainFrame() {

	button_calculate->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::button_calculateOnButtonClick), NULL, this);
}
