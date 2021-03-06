///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  6 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DIVE_AGENT_FORMS_H__
#define __DIVE_AGENT_FORMS_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/gauge.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/dialog.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/hyperlink.h>
#include <wx/textctrl.h>
#include <wx/bmpbuttn.h>
#include <wx/statline.h>
#include <wx/choice.h>
#include <wx/checkbox.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class UploadDivesProgressDialogBase
///////////////////////////////////////////////////////////////////////////////
class UploadDivesProgressDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel6;
		wxStaticText* m_statusStatic;
		wxGauge* m_uploadProgressGauge;
		wxStaticText* m_uploadProgressStatic;
		wxButton* m_actionButton;
		wxButton* m_doneButton;
		
		// Virtual event handlers, overide them in your derived class
		virtual void actionButtonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void doneButtonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		UploadDivesProgressDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Upload Dives"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~UploadDivesProgressDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class AboutDialogBase
///////////////////////////////////////////////////////////////////////////////
class AboutDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel8;
		wxStaticBitmap* m_bitmap2;
		wxStaticText* m_aboutStatic;
		wxStaticText* m_staticText12;
		wxHyperlinkCtrl* m_hyperlink2;
		wxButton* m_okButton;
		
		// Virtual event handlers, overide them in your derived class
		virtual void okButtonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		AboutDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About DiveboardAgent"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~AboutDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MainDialogBase
///////////////////////////////////////////////////////////////////////////////
class MainDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_login_panel;
		wxStaticBitmap* m_bitmap4;
		wxStaticText* m_emailStatic;
		wxTextCtrl* m_emailText;
		wxStaticText* m_paswordStatic;
		wxTextCtrl* m_passwordText;
		wxButton* m_loginButton;
		wxBitmapButton* m_FBconnectButton;
		wxPanel* m_upload_dive;
		wxStaticBitmap* m_avatar;
		wxStaticText* m_login;
		wxStaticLine* m_staticline2;
		wxStaticText* m_selecMakeStatic;
		wxChoice* m_selectMakeChoice;
		wxStaticText* m_modelStatic;
		wxChoice* m_selectModelChoice;
		wxPanel* m_selectPortPanel;
		wxStaticText* m_selectPortStatic;
		wxChoice* m_selectPortChoice;
		wxCheckBox* m_selectPortManualCheck;
		wxStaticText* m_staticText21;
		wxButton* m_uploadDivesButton;
		
		// Virtual event handlers, overide them in your derived class
		virtual void loginButtonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void loginButtonOnUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void FBconnectButtonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void FBconnectButtonOnUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void selectMakeChoiceOnChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void uploadDivesButtonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~MainDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class LogDialogBase
///////////////////////////////////////////////////////////////////////////////
class LogDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel6;
		wxStaticText* m_staticText12;
		wxStaticText* m_staticText13;
		wxTextCtrl* m_email;
		wxStaticText* m_staticText14;
		wxTextCtrl* m_subject;
		wxStaticText* m_staticText15;
		wxTextCtrl* m_message;
		wxButton* m_send;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClickSendLog( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		LogDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 450,400 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~LogDialogBase();
	
};

#endif //__DIVE_AGENT_FORMS_H__
