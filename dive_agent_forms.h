///////////////////////////////////////////////////////////////////////////// C++ code generated with wxFormBuilder (version Nov 27 2012)// http://www.wxformbuilder.org///// PLEASE DO "NOT" EDIT THIS FILE!///////////////////////////////////////////////////////////////////////////#ifndef __DIVE_AGENT_FORMS_H__#define __DIVE_AGENT_FORMS_H__#include <wx/artprov.h>#include <wx/xrc/xmlres.h>#include <wx/string.h>#include <wx/stattext.h>#include <wx/gdicmn.h>#include <wx/font.h>#include <wx/colour.h>#include <wx/settings.h>#include <wx/combobox.h>#include <wx/sizer.h>#include <wx/button.h>#include <wx/dialog.h>#include <wx/gauge.h>#include <wx/bitmap.h>#include <wx/image.h>#include <wx/icon.h>#include <wx/statbmp.h>#include <wx/panel.h>#include <wx/textctrl.h>#include <wx/bmpbuttn.h>///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Class UploadDivesDialogBase///////////////////////////////////////////////////////////////////////////////class UploadDivesDialogBase : public wxDialog {	private:		protected:		wxStaticText* m_selectComputerStatic;		wxComboBox* m_selectComputerCombo;		wxStaticText* m_selectPortStatic;		wxComboBox* m_selectPortCombo;		wxStaticText* m_dialogDescriptionStatic;		wxButton* m_uploadDivesButton;				// Virtual event handlers, overide them in your derived class		virtual void selectComputerComboOnCombobox( wxCommandEvent& event ) { event.Skip(); }		virtual void selectPortComboOnCombobox( wxCommandEvent& event ) { event.Skip(); }		virtual void uploadDivesButtonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }				public:				UploadDivesDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Upload Dives"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 		~UploadDivesDialogBase();	};////////////////////////////////////////////////////////////////////////////////// Class UploadDivesProgressDialogBase///////////////////////////////////////////////////////////////////////////////class UploadDivesProgressDialogBase : public wxDialog {	private:		protected:		wxStaticText* m_statusStatic;		wxGauge* m_uploadProgressGauge;		wxStaticText* m_uploadProgressStatic;		wxButton* m_actionButton;				// Virtual event handlers, overide them in your derived class		virtual void actionButtonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }				public:				UploadDivesProgressDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Upload Dives"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 		~UploadDivesProgressDialogBase();	};////////////////////////////////////////////////////////////////////////////////// Class PreferencesDialogBase///////////////////////////////////////////////////////////////////////////////class PreferencesDialogBase : public wxDialog {	private:		protected:		wxPanel* m_accauntInfoPanel;		wxStaticBitmap* m_accountBitmap;		wxStaticText* m_accountStatic;		wxButton* m_unlinkButton;		wxPanel* m_accountSetPanel;		wxStaticText* m_emailStatic;		wxTextCtrl* m_emailText;		wxStaticText* m_paswordStatic;		wxTextCtrl* m_passwordText;		wxButton* m_loginButton;		wxBitmapButton* m_FBconnectButton;				// Virtual event handlers, overide them in your derived class		virtual void unlinkButtonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }		virtual void unlinkButtonOnUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }		virtual void loginButtonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }		virtual void loginButtonOnUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }		virtual void FBconnectButtonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }		virtual void FBconnectButtonOnUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }				public:				PreferencesDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 		~PreferencesDialogBase();	};////////////////////////////////////////////////////////////////////////////////// Class AboutDialogBase///////////////////////////////////////////////////////////////////////////////class AboutDialogBase : public wxDialog {	private:		protected:		wxStaticText* m_aboutStatic;		wxButton* m_okButton;		public:				AboutDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About DiveAgent"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 		~AboutDialogBase();	};#endif //__DIVE_AGENT_FORMS_H__