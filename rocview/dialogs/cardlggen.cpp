///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 17 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "basenotebook.h"

#include "cardlggen.h"

///////////////////////////////////////////////////////////////////////////

cardlggen::cardlggen( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_CarImage = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 280,80 ), wxBU_AUTODRAW );
	bSizer1->Add( m_CarImage, 0, wxTOP|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_CarBook = new basenotebook( this, ID_CARBOOK, wxDefaultPosition, wxDefaultSize, 0 );
	m_IndexPanel = new wxPanel( m_CarBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_CarList = new wxListBox( m_IndexPanel, wxID_ANY, wxDefaultPosition, wxSize( -1,200 ), 0, NULL, 0 ); 
	bSizer2->Add( m_CarList, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_NewCar = new wxButton( m_IndexPanel, wxID_ANY, wxT("New"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_NewCar, 0, wxALL, 5 );
	
	m_DeleteCar = new wxButton( m_IndexPanel, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_DeleteCar, 0, wxALL, 5 );
	
	m_Import = new wxButton( m_IndexPanel, wxID_ANY, wxT("Import..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_Import, 0, wxALL, 5 );
	
	m_Copy = new wxButton( m_IndexPanel, wxID_ANY, wxT("Copy"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_Copy, 0, wxALL, 5 );
	
	bSizer2->Add( bSizer7, 1, wxEXPAND, 5 );
	
	m_IndexPanel->SetSizer( bSizer2 );
	m_IndexPanel->Layout();
	bSizer2->Fit( m_IndexPanel );
	m_CarBook->AddPage( m_IndexPanel, wxT("Index"), false );
	m_GeneralPanel = new wxPanel( m_CarBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_labID = new wxStaticText( m_GeneralPanel, wxID_ANY, wxT("ID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labID->Wrap( -1 );
	fgSizer1->Add( m_labID, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_ID = new wxTextCtrl( m_GeneralPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_ID, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_labIdent = new wxStaticText( m_GeneralPanel, wxID_ANY, wxT("Ident"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labIdent->Wrap( -1 );
	fgSizer1->Add( m_labIdent, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_Ident = new wxTextCtrl( m_GeneralPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_Ident, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_labLocation = new wxStaticText( m_GeneralPanel, wxID_ANY, wxT("Location"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labLocation->Wrap( -1 );
	fgSizer1->Add( m_labLocation, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_Location = new wxComboBox( m_GeneralPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer1->Add( m_Location, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_labRoadname = new wxStaticText( m_GeneralPanel, wxID_ANY, wxT("Roadname"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labRoadname->Wrap( -1 );
	fgSizer1->Add( m_labRoadname, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_Roadname = new wxTextCtrl( m_GeneralPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_Roadname, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_labNumber = new wxStaticText( m_GeneralPanel, wxID_ANY, wxT("Number"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labNumber->Wrap( -1 );
	fgSizer1->Add( m_labNumber, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_Number = new wxTextCtrl( m_GeneralPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_Number, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_labColor = new wxStaticText( m_GeneralPanel, wxID_ANY, wxT("Color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labColor->Wrap( -1 );
	fgSizer1->Add( m_labColor, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_Color = new wxTextCtrl( m_GeneralPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_Color, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	m_labImageName = new wxStaticText( m_GeneralPanel, wxID_ANY, wxT("Image"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labImageName->Wrap( -1 );
	fgSizer1->Add( m_labImageName, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ImageName = new wxTextCtrl( m_GeneralPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_ImageName, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	m_labOwner = new wxStaticText( m_GeneralPanel, wxID_ANY, wxT("Owner"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labOwner->Wrap( -1 );
	fgSizer1->Add( m_labOwner, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	m_Owner = new wxTextCtrl( m_GeneralPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_Owner, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer3->Add( fgSizer1, 0, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( m_GeneralPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer3->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxString m_StatusChoices[] = { wxT("empty"), wxT("loaded"), wxT("maintenance") };
	int m_StatusNChoices = sizeof( m_StatusChoices ) / sizeof( wxString );
	m_Status = new wxRadioBox( m_GeneralPanel, wxID_ANY, wxT("Status"), wxDefaultPosition, wxDefaultSize, m_StatusNChoices, m_StatusChoices, 1, wxRA_SPECIFY_ROWS );
	m_Status->SetSelection( 0 );
	bSizer3->Add( m_Status, 0, wxALL|wxEXPAND, 5 );
	
	m_GeneralPanel->SetSizer( bSizer3 );
	m_GeneralPanel->Layout();
	bSizer3->Fit( m_GeneralPanel );
	m_CarBook->AddPage( m_GeneralPanel, wxT("General"), false );
	m_DetailsPanel = new wxPanel( m_CarBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxString m_TypeChoices[] = { wxT("Freight"), wxT("Passenger") };
	int m_TypeNChoices = sizeof( m_TypeChoices ) / sizeof( wxString );
	m_Type = new wxRadioBox( m_DetailsPanel, wxID_ANY, wxT("Type"), wxDefaultPosition, wxDefaultSize, m_TypeNChoices, m_TypeChoices, 1, wxRA_SPECIFY_ROWS );
	m_Type->SetSelection( 0 );
	bSizer4->Add( m_Type, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->AddGrowableCol( 1 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_labSubtype = new wxStaticText( m_DetailsPanel, wxID_ANY, wxT("Subtype"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labSubtype->Wrap( -1 );
	fgSizer3->Add( m_labSubtype, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	m_SubType = new wxComboBox( m_DetailsPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer3->Add( m_SubType, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_labLength = new wxStaticText( m_DetailsPanel, wxID_ANY, wxT("Length"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labLength->Wrap( -1 );
	fgSizer3->Add( m_labLength, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_Length = new wxSpinCtrl( m_DetailsPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), wxSP_ARROW_KEYS, 0, 9999, 0 );
	fgSizer3->Add( m_Length, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_labManuId = new wxStaticText( m_DetailsPanel, wxID_ANY, wxT("Manufacturer ID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labManuId->Wrap( -1 );
	fgSizer3->Add( m_labManuId, 0, wxALIGN_RIGHT|wxBOTTOM|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ManuId = new wxTextCtrl( m_DetailsPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_ManuId, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer4->Add( fgSizer3, 0, wxEXPAND, 5 );
	
	wxString m_EraChoices[] = { wxT("I"), wxT("II"), wxT("III"), wxT("IV"), wxT("V"), wxT("VI") };
	int m_EraNChoices = sizeof( m_EraChoices ) / sizeof( wxString );
	m_Era = new wxRadioBox( m_DetailsPanel, wxID_ANY, wxT("Era"), wxDefaultPosition, wxDefaultSize, m_EraNChoices, m_EraChoices, 1, wxRA_SPECIFY_ROWS );
	m_Era->SetSelection( 0 );
	bSizer4->Add( m_Era, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_labRemark = new wxStaticText( m_DetailsPanel, wxID_ANY, wxT("Maintenance"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labRemark->Wrap( -1 );
	bSizer4->Add( m_labRemark, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_Remark = new wxTextCtrl( m_DetailsPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	bSizer4->Add( m_Remark, 1, wxALL|wxEXPAND, 5 );
	
	m_DetailsPanel->SetSizer( bSizer4 );
	m_DetailsPanel->Layout();
	bSizer4->Fit( m_DetailsPanel );
	m_CarBook->AddPage( m_DetailsPanel, wxT("Details"), true );
	
	bSizer1->Add( m_CarBook, 1, wxEXPAND | wxALL, 5 );
	
	m_stdButton = new wxStdDialogButtonSizer();
	m_stdButtonOK = new wxButton( this, wxID_OK );
	m_stdButton->AddButton( m_stdButtonOK );
	m_stdButtonApply = new wxButton( this, wxID_APPLY );
	m_stdButton->AddButton( m_stdButtonApply );
	m_stdButtonCancel = new wxButton( this, wxID_CANCEL );
	m_stdButton->AddButton( m_stdButtonCancel );
	m_stdButton->Realize();
	bSizer1->Add( m_stdButton, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_CarImage->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cardlggen::onCarImage ), NULL, this );
	m_CarList->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( cardlggen::onCarList ), NULL, this );
	m_NewCar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cardlggen::onNewCar ), NULL, this );
	m_DeleteCar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cardlggen::onDeleteCar ), NULL, this );
	m_Import->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cardlggen::onImport ), NULL, this );
	m_Copy->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cardlggen::OnCopy ), NULL, this );
	m_Type->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( cardlggen::onTypeSelect ), NULL, this );
	m_stdButtonApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cardlggen::onApply ), NULL, this );
	m_stdButtonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cardlggen::onCancel ), NULL, this );
	m_stdButtonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cardlggen::onOK ), NULL, this );
}

cardlggen::~cardlggen()
{
	// Disconnect Events
	m_CarImage->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cardlggen::onCarImage ), NULL, this );
	m_CarList->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( cardlggen::onCarList ), NULL, this );
	m_NewCar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cardlggen::onNewCar ), NULL, this );
	m_DeleteCar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cardlggen::onDeleteCar ), NULL, this );
	m_Import->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cardlggen::onImport ), NULL, this );
	m_Copy->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cardlggen::OnCopy ), NULL, this );
	m_Type->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( cardlggen::onTypeSelect ), NULL, this );
	m_stdButtonApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cardlggen::onApply ), NULL, this );
	m_stdButtonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cardlggen::onCancel ), NULL, this );
	m_stdButtonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cardlggen::onOK ), NULL, this );
}
