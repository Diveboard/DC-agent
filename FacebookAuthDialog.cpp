#include <wx/webview.h>
#include <wx/msgdlg.h>
#include "FacebookAuthDialog.h"

void FacbookAuthDialog::OnLoaded(wxWebViewEvent& event)
{

  std::string url = event.GetURL().utf8_str().data();
    /*wxString msg = wxString::FromUTF8("LOADED ") + event.GetURL().utf8_str();
    wxMessageDialog* dlg = new wxMessageDialog(this, msg, wxString::FromUTF8("Loaded"));
    dlg->ShowModal();
    dlg->Destroy();*/
  if (!_token.empty())
    {
      // request for token is done
      /*wxString msg = wxString::FromUTF8("FOUND token ") + _token;
      wxMessageDialog* dlg = new wxMessageDialog(this, msg, wxString::FromUTF8("Finishing"));
      dlg->ShowModal();
      dlg->Destroy();*/
      
      EndModal(wxID_OK);
    }
}
    // handling redirects
void FacbookAuthDialog::OnNaviagting(wxWebViewEvent& event)
{
  // check for access token in redirect url
  std::string url = event.GetURL().utf8_str().data();
    /*wxString msg = wxString::FromUTF8("Navigating to ") + event.GetURL().utf8_str();
    wxMessageDialog* dlg = new wxMessageDialog(this, msg, wxString::FromUTF8("Navigating"));
    dlg->ShowModal();
    dlg->Destroy();*/

  std::string key("access_token=");

  std::cout << "[" <<  url << "]" << std::endl;
  size_t b = url.find(key);
  if (b != std::string::npos)
    {
      b += key.size();
      size_t e = url.find("&", b);
      if ( e != std::string::npos)
        {
          _token = url.substr(b, e-b);
        }
    }
}

void FacbookAuthDialog::OnError(wxWebViewEvent&)
{
  _error = true;
}

void FacbookAuthDialog::OnInitDialog(wxInitDialogEvent& event)
{
  _error = false;
  _token.resize(0);
  _fbid.resize(0);
  // request facebook for access token
  m_WebView->LoadURL(wxT("https://www.facebook.com/dialog/oauth?client_id=193803977296892&response_type=token&redirect_uri=https://www.diveboard.com/connect/login_success.html"));
}
  
FacbookAuthDialog::FacbookAuthDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
  this->SetSizeHints( wxSize( 400,-1 ), wxDefaultSize );
  
  wxBoxSizer* m_mainSizer;
  m_mainSizer = new wxBoxSizer( wxVERTICAL );
  
  m_WebView = wxWebView::New(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(800, 600));
  Connect(m_WebView->GetId(), wxEVT_WEBVIEW_LOADED,
	  wxWebViewEventHandler(FacbookAuthDialog::OnLoaded), NULL, this);
  Connect(m_WebView->GetId(), wxEVT_WEBVIEW_NAVIGATING,
	  wxWebViewEventHandler(FacbookAuthDialog::OnNaviagting), NULL, this);
  Connect(m_WebView->GetId(), wxEVT_WEBVIEW_NAVIGATED,
    wxWebViewEventHandler(FacbookAuthDialog::OnNaviagting), NULL, this);
  Connect(m_WebView->GetId(), wxEVT_WEBVIEW_ERROR,
	  wxWebViewEventHandler(FacbookAuthDialog::OnError), NULL, this);
  
  Connect(this->GetId(), wxEVT_INIT_DIALOG,
	  wxInitDialogEventHandler(FacbookAuthDialog::OnInitDialog), NULL, this);
  
  m_mainSizer->Add( m_WebView, 3, wxALIGN_LEFT|wxALL, 1 );
  
  this->SetSizer( m_mainSizer );
  this->Layout();
  m_mainSizer->Fit( this );
  
  this->Centre( wxBOTH );    
}

FacbookAuthDialog::~FacbookAuthDialog()
{
}
