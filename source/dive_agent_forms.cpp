///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  6 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "dive_agent_forms.h"

#include "FBloginbutton.png.h"
#include "full_diveboard_grey.png.h"
#include "icon_about.png.h"
#include "icon_bw.png.h"
#include "Global.h"

///////////////////////////////////////////////////////////////////////////

UploadDivesProgressDialogBase::UploadDivesProgressDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 450,-1 ), wxDefaultSize );
	this->SetBackgroundColour( wxColour( 251, 175, 23 ) );
	
	wxBoxSizer* m_mainSizer;
	m_mainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_panel6 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel6->SetBackgroundColour( wxColour( 251, 221, 161 ) );
	
	wxBoxSizer* bSizer32;
	bSizer32 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_statusSizer;
	m_statusSizer = new wxBoxSizer( wxVERTICAL );
	
	
	m_statusSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_statusStatic = new wxStaticText( m_panel6, wxID_ANY, wxT("Status:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_statusStatic->Wrap( -1 );
	m_statusSizer->Add( m_statusStatic, 1, wxLEFT|wxEXPAND, 10 );
	
	
	bSizer32->Add( m_statusSizer, 4, wxEXPAND, 5 );
	
	wxBoxSizer* m_gaugeSizer;
	m_gaugeSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_uploadProgressGauge = new wxGauge( m_panel6, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	m_gaugeSizer->Add( m_uploadProgressGauge, 8, wxALL|wxEXPAND, 5 );
	
	m_uploadProgressStatic = new wxStaticText( m_panel6, wxID_ANY, wxT("100%"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	m_uploadProgressStatic->Wrap( -1 );
	m_gaugeSizer->Add( m_uploadProgressStatic, 1, wxALL, 5 );
	
	
	bSizer32->Add( m_gaugeSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_buttonSizer;
	m_buttonSizer = new wxBoxSizer( wxVERTICAL );
	
	
	m_buttonSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_actionButton = new wxButton( m_panel6, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonSizer->Add( m_actionButton, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	bSizer32->Add( m_buttonSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_buttonsSizer;
	m_buttonsSizer = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_buttonsSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_doneButton = new wxButton( m_panel6, wxID_ANY, wxT("Finalize on Diveboard"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonsSizer->Add( m_doneButton, 0, wxALL, 5 );
	
	
	bSizer32->Add( m_buttonsSizer, 1, wxEXPAND, 5 );
	
	
	m_panel6->SetSizer( bSizer32 );
	m_panel6->Layout();
	bSizer32->Fit( m_panel6 );
	m_mainSizer->Add( m_panel6, 1, wxEXPAND | wxALL, 20 );
	
	
	this->SetSizer( m_mainSizer );
	this->Layout();
	m_mainSizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_actionButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UploadDivesProgressDialogBase::actionButtonOnButtonClick ), NULL, this );
	m_doneButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UploadDivesProgressDialogBase::doneButtonOnButtonClick ), NULL, this );
}

UploadDivesProgressDialogBase::~UploadDivesProgressDialogBase()
{
	// Disconnect Events
	m_actionButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UploadDivesProgressDialogBase::actionButtonOnButtonClick ), NULL, this );
	m_doneButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UploadDivesProgressDialogBase::doneButtonOnButtonClick ), NULL, this );
	
}

AboutDialogBase::AboutDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 400,-1 ), wxDefaultSize );
	this->SetBackgroundColour( wxColour( 251, 175, 23 ) );
	
	wxBoxSizer* m_mainSizer;
	m_mainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_panel8 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel8->SetBackgroundColour( wxColour( 251, 221, 161 ) );
	
	wxBoxSizer* bSizer36;
	bSizer36 = new wxBoxSizer( wxVERTICAL );
	
	m_bitmap2 = new wxStaticBitmap( m_panel8, wxID_ANY, icon_bw_png_to_wx_bitmap(), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer36->Add( m_bitmap2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_aboutStatic = new wxStaticText( m_panel8, wxID_ANY, wxT("DiveboardAgent"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_aboutStatic->Wrap( -1 );
	m_aboutStatic->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	
	bSizer36->Add( m_aboutStatic, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText12 = new wxStaticText( m_panel8, wxID_ANY, wxT("Version 1.0.0 \nCopyright © 2014 Diveboard SARL or its subsidiaries\n\nDiveboard Agent detect compatible dive computers connected to the\ncomputer and transfers available dive profiles to \n"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText12->Wrap( -1 );
	bSizer36->Add( m_staticText12, 0, wxALL, 5 );
	
	m_hyperlink2 = new wxHyperlinkCtrl( m_panel8, wxID_ANY, wxT("http://www.diveboard.com"), wxT("http://www.diveboard.com/"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer36->Add( m_hyperlink2, 0, wxALL, 5 );
	
	m_okButton = new wxButton( m_panel8, wxID_OK, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	m_okButton->SetDefault(); 
	bSizer36->Add( m_okButton, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	
	m_panel8->SetSizer( bSizer36 );
	m_panel8->Layout();
	bSizer36->Fit( m_panel8 );
	m_mainSizer->Add( m_panel8, 1, wxEXPAND | wxALL, 20 );
	
	
	this->SetSizer( m_mainSizer );
	this->Layout();
	m_mainSizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_okButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialogBase::okButtonOnButtonClick ), NULL, this );
}

AboutDialogBase::~AboutDialogBase()
{
	// Disconnect Events
	m_okButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialogBase::okButtonOnButtonClick ), NULL, this );
	
}

MainDialogBase::MainDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 500,450 ), wxDefaultSize );
	this->SetBackgroundColour( wxColour( 251, 175, 23 ) );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer21->Add( 0, 0, 1, 0, 5 );
	
	m_login_panel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxNO_BORDER );
	m_login_panel->SetBackgroundColour( wxColour( 251, 221, 161 ) );
	
	wxBoxSizer* m_accountSetSizer;
	m_accountSetSizer = new wxBoxSizer( wxVERTICAL );
	
	m_bitmap4 = new wxStaticBitmap( m_login_panel, wxID_ANY, full_diveboard_grey_png_to_wx_bitmap(), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_accountSetSizer->Add( m_bitmap4, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 20 );
	
	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer33->Add( 20, 0, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_accountSetViaEmailSizer;
	m_accountSetViaEmailSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_EmailSizer;
	m_EmailSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_emailStatic = new wxStaticText( m_login_panel, wxID_ANY, wxT("E-Mail:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_emailStatic->Wrap( -1 );
	m_EmailSizer->Add( m_emailStatic, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_emailText = new wxTextCtrl( m_login_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_emailText->SetMaxLength( 0 ); 
	m_emailText->SetFont( wxFont( 13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL ) );
	
	m_EmailSizer->Add( m_emailText, 3, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	m_accountSetViaEmailSizer->Add( m_EmailSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_passwordSizer;
	m_passwordSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_paswordStatic = new wxStaticText( m_login_panel, wxID_ANY, wxT("Password:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_paswordStatic->Wrap( -1 );
	m_passwordSizer->Add( m_paswordStatic, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_passwordText = new wxTextCtrl( m_login_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD|wxTE_PROCESS_ENTER );
	m_passwordText->SetMaxLength( 0 ); 
	m_passwordText->SetFont( wxFont( 13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL ) );
	
	m_passwordSizer->Add( m_passwordText, 3, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	m_accountSetViaEmailSizer->Add( m_passwordSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_loginButtonSizer;
	m_loginButtonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_loginButton = new wxButton( m_login_panel, wxID_ANY, wxT("Login"), wxDefaultPosition, wxSize( -1,40 ), 0 );
	m_loginButtonSizer->Add( m_loginButton, 1, wxALIGN_LEFT|wxALL, 5 );
	
	m_FBconnectButton = new wxBitmapButton( m_login_panel, wxID_ANY, FBloginbutton_png_to_wx_bitmap(), wxDefaultPosition, wxSize( -1,40 ), wxBU_AUTODRAW );
	m_loginButtonSizer->Add( m_FBconnectButton, 1, wxALL, 5 );
	
	
	m_accountSetViaEmailSizer->Add( m_loginButtonSizer, 1, wxEXPAND|wxBOTTOM, 10 );
	
	
	bSizer33->Add( m_accountSetViaEmailSizer, 1, wxEXPAND, 5 );
	
	
	bSizer33->Add( 20, 0, 0, wxEXPAND, 5 );
	
	
	m_accountSetSizer->Add( bSizer33, 1, wxEXPAND, 5 );
	
	
	m_login_panel->SetSizer( m_accountSetSizer );
	m_login_panel->Layout();
	m_accountSetSizer->Fit( m_login_panel );
	bSizer21->Add( m_login_panel, 0, wxALIGN_CENTER|wxALL, 20 );
	
	m_upload_dive = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxNO_BORDER );
	m_upload_dive->SetBackgroundColour( wxColour( 251, 221, 161 ) );
	m_upload_dive->Hide();
	
	wxBoxSizer* bMainSizer;
	bMainSizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bAvatarSizer;
	bAvatarSizer = new wxBoxSizer( wxVERTICAL );
	
	m_avatar = new wxStaticBitmap( m_upload_dive, wxID_ANY, icon_about_png_to_wx_bitmap(), wxDefaultPosition, wxSize( 100,100 ), 0 );
	bAvatarSizer->Add( m_avatar, 0, wxEXPAND|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_login = new wxStaticText( m_upload_dive, wxID_ANY, wxT("Logged in as..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_login->Wrap( -1 );
	bAvatarSizer->Add( m_login, 0, wxALL|wxEXPAND, 5 );
	
	
	bMainSizer->Add( bAvatarSizer, 0, wxTOP|wxLEFT, 10 );
	
	m_staticline2 = new wxStaticLine( m_upload_dive, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bMainSizer->Add( m_staticline2, 0, wxALL, 5 );
	
	wxBoxSizer* bUploadSizer;
	bUploadSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_selectMakeSizer;
	m_selectMakeSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_selectMakeSizer->SetMinSize( wxSize( -1,35 ) ); 
	m_selecMakeStatic = new wxStaticText( m_upload_dive, wxID_ANY, wxT("Select Make:  "), wxDefaultPosition, wxDefaultSize, 0 );
	m_selecMakeStatic->Wrap( -1 );
	m_selectMakeSizer->Add( m_selecMakeStatic, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_selectMakeChoiceChoices;
	m_selectMakeChoice = new wxChoice( m_upload_dive, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_selectMakeChoiceChoices, 0 );
	m_selectMakeChoice->SetSelection( 0 );
	m_selectMakeSizer->Add( m_selectMakeChoice, 1, wxALL|wxEXPAND, 5 );
	
	
	bUploadSizer->Add( m_selectMakeSizer, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_selectModelSizer;
	m_selectModelSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_selectModelSizer->SetMinSize( wxSize( -1,35 ) ); 
	m_modelStatic = new wxStaticText( m_upload_dive, wxID_ANY, wxT("Select Model:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_modelStatic->Wrap( -1 );
	m_selectModelSizer->Add( m_modelStatic, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_selectModelChoiceChoices;
	m_selectModelChoice = new wxChoice( m_upload_dive, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_selectModelChoiceChoices, 0 );
	m_selectModelChoice->SetSelection( 0 );
	m_selectModelSizer->Add( m_selectModelChoice, 1, wxALL|wxEXPAND, 5 );
	
	
	bUploadSizer->Add( m_selectModelSizer, 0, wxEXPAND, 5 );
	
	m_selectPortPanel = new wxPanel( m_upload_dive, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_selectPortPanel->SetBackgroundColour( wxColour( 251, 221, 161 ) );
	
	wxBoxSizer* m_selectPortMainSizer;
	m_selectPortMainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_selectPortSizer;
	m_selectPortSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_selectPortSizer->SetMinSize( wxSize( -1,35 ) ); 
	m_selectPortStatic = new wxStaticText( m_selectPortPanel, wxID_ANY, wxT("Select port:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_selectPortStatic->Wrap( -1 );
	m_selectPortSizer->Add( m_selectPortStatic, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_selectPortChoiceChoices;
	m_selectPortChoice = new wxChoice( m_selectPortPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_selectPortChoiceChoices, 0 );
	m_selectPortChoice->SetSelection( 0 );
	m_selectPortSizer->Add( m_selectPortChoice, 3, wxALL|wxEXPAND, 5 );
	
	
	m_selectPortMainSizer->Add( m_selectPortSizer, 0, wxEXPAND, 5 );
	
	m_selectPortManualCheck = new wxCheckBox( m_selectPortPanel, wxID_ANY, wxT("Always select port manualy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_selectPortMainSizer->Add( m_selectPortManualCheck, 1, wxALL, 5 );
	
	
	m_selectPortPanel->SetSizer( m_selectPortMainSizer );
	m_selectPortPanel->Layout();
	m_selectPortMainSizer->Fit( m_selectPortPanel );
	bUploadSizer->Add( m_selectPortPanel, 1, wxEXPAND | wxALL, 5 );
	
	m_staticText21 = new wxStaticText( m_upload_dive, wxID_ANY, wxT("Connect your dive computer to your PC, put it in \"PC\" mode and then you are ready to hit \"Upload dives\"\nbutton."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	bUploadSizer->Add( m_staticText21, 0, wxALL, 5 );
	
	
	bUploadSizer->Add( 0, 20, 0, 0, 5 );
	
	m_uploadDivesButton = new wxButton( m_upload_dive, wxID_ANY, wxT("Upload Dives"), wxDefaultPosition, wxDefaultSize, 0 );
	bUploadSizer->Add( m_uploadDivesButton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	bUploadSizer->Add( 0, 20, 0, 0, 5 );
	
	
	bMainSizer->Add( bUploadSizer, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 10 );
	
	
	m_upload_dive->SetSizer( bMainSizer );
	m_upload_dive->Layout();
	bMainSizer->Fit( m_upload_dive );
	bSizer21->Add( m_upload_dive, 0, wxALIGN_CENTER|wxALL, 20 );
	
	
	bSizer21->Add( 0, 0, 1, 0, 5 );
	
	
	this->SetSizer( bSizer21 );
	this->Layout();
	bSizer21->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_passwordText->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainDialogBase::loginButtonOnButtonClick ), NULL, this );
	m_loginButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogBase::loginButtonOnButtonClick ), NULL, this );
	m_loginButton->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainDialogBase::loginButtonOnUpdateUI ), NULL, this );
	m_FBconnectButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogBase::FBconnectButtonOnButtonClick ), NULL, this );
	m_FBconnectButton->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainDialogBase::FBconnectButtonOnUpdateUI ), NULL, this );
	m_selectMakeChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainDialogBase::selectMakeChoiceOnChoice ), NULL, this );
	m_uploadDivesButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogBase::uploadDivesButtonOnButtonClick ), NULL, this );
}

MainDialogBase::~MainDialogBase()
{
	// Disconnect Events
	m_passwordText->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainDialogBase::loginButtonOnButtonClick ), NULL, this );
	m_loginButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogBase::loginButtonOnButtonClick ), NULL, this );
	m_loginButton->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainDialogBase::loginButtonOnUpdateUI ), NULL, this );
	m_FBconnectButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogBase::FBconnectButtonOnButtonClick ), NULL, this );
	m_FBconnectButton->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainDialogBase::FBconnectButtonOnUpdateUI ), NULL, this );
	m_selectMakeChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainDialogBase::selectMakeChoiceOnChoice ), NULL, this );
	m_uploadDivesButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogBase::uploadDivesButtonOnButtonClick ), NULL, this );
	
}

LogDialogBase::LogDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxColour( 251, 175, 23 ) );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer23->Add( 0, 0, 1, 0, 5 );
	
	m_panel6 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel6->SetBackgroundColour( wxColour( 251, 221, 161 ) );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer25->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText12 = new wxStaticText( m_panel6, wxID_ANY, wxT("Send Log"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText12->Wrap( -1 );
	m_staticText12->SetFont( wxFont( 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	
	bSizer25->Add( m_staticText12, 0, wxALL, 5 );
	
	
	bSizer25->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	bSizer24->Add( bSizer25, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText13 = new wxStaticText( m_panel6, wxID_ANY, wxT("Email"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer26->Add( m_staticText13, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_email = new wxTextCtrl( m_panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer26->Add( m_email, 3, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer24->Add( bSizer26, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText14 = new wxStaticText( m_panel6, wxID_ANY, wxT("Subject"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	bSizer27->Add( m_staticText14, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_subject = new wxTextCtrl( m_panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer27->Add( m_subject, 3, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer24->Add( bSizer27, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText15 = new wxStaticText( m_panel6, wxID_ANY, wxT("Message"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	bSizer28->Add( m_staticText15, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_message = new wxTextCtrl( m_panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	bSizer28->Add( m_message, 3, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer24->Add( bSizer28, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer29->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_send = new wxButton( m_panel6, wxID_ANY, wxT("Send Log"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer29->Add( m_send, 0, wxALL, 5 );
	
	
	bSizer24->Add( bSizer29, 1, wxEXPAND, 5 );
	
	
	m_panel6->SetSizer( bSizer24 );
	m_panel6->Layout();
	bSizer24->Fit( m_panel6 );
	bSizer23->Add( m_panel6, 1, wxALL|wxEXPAND, 10 );
	
	
	bSizer23->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer23 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_send->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LogDialogBase::OnClickSendLog ), NULL, this );
}

LogDialogBase::~LogDialogBase()
{
	// Disconnect Events
	m_send->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LogDialogBase::OnClickSendLog ), NULL, this );
	
}
