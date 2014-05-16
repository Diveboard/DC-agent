#include "PreferencesDialog.h"

#include <wx/webview.h>
#include "DiveAgent.h"
#include <wx/msgdlg.h>
#include <wx/filename.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <fstream>

namespace
{
  class FacbookAuthDialog : public wxDialog
  {
	private:
    
	protected:
		wxWebView* m_WebView;
    
	public:
		
		FacbookAuthDialog(wxWindow* parent,
                      wxWindowID id = wxID_ANY,
                      const wxString& title = wxT("Facebook Authorization"),
                      const wxPoint& pos = wxDefaultPosition,
                      const wxSize& size = wxDefaultSize,
                      long style = wxDEFAULT_DIALOG_STYLE );
		~FacbookAuthDialog();
    bool IsError() const { return  _error; }
    const std::string Token() const { return _token; }
    const std::string Id() const { return _fbid; }
  protected:
    // handling loading url is completed
    void OnLoaded(wxWebViewEvent& event)
    {
      if (!_token.empty())
      {
        // request for token is done
        if (_fbid.empty())
        {
          // request for id
          _fbid = "undef";
          m_WebView->LoadURL(wxString::FromUTF8((std::string("https://graph.facebook.com/me?access_token=") + _token).c_str()));
        }
        else
        {
          // request for id is done
          std::string txt = m_WebView->GetPageText().utf8_str().data();
          rapidjson::Document d;
          d.Parse<0>(txt.c_str());
          if ( !d["id"].IsNull())
          {
            _fbid = d["id"].GetString();
            EndModal(wxID_OK);
          }
          else
          {
            EndModal(wxID_CANCEL);
          }
        }
      }
    };
    // handling redirects
    void OnNaviagting(wxWebViewEvent& event)
    {
      // check for access token in redirect url
      std::string url = event.GetURL().utf8_str().data();
      std::string key("access_token=");
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
    };
    void OnError(wxWebViewEvent&)
    {
      _error = true;
    };
    void OnInitDialog(wxInitDialogEvent& event)
    {
      _error = false;
      _token.resize(0);
      _fbid.resize(0);
      // request facebook for access token
      m_WebView->LoadURL(wxT("https://www.facebook.com/dialog/oauth?client_id=145634995501895&response_type=token&redirect_uri=https://www.facebook.com/connect/login_success.html"));
    }
    bool        _error;
    std::string _token;
    std::string _fbid;
    int i;
  };
  
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
  
}
PreferencesDialog::PreferencesDialog():
  PreferencesDialogBase(0)
{
  std::string email_value = DiveAgent::readProfile("login_email");
  if ( !email_value.empty() )
  {
    m_emailText->ChangeValue(wxString::FromUTF8(email_value.c_str()));
  }
  showSetAccount();
}
void PreferencesDialog::FBconnectButtonOnButtonClick( wxCommandEvent& event )
{
  std::string token;
  std::string id;
  FacbookAuthDialog* dlg = new FacbookAuthDialog(0,  wxID_ANY);
  dlg->ShowModal();
  token = dlg->Token();
  id  = dlg->Id();
  dlg->Destroy();
  if ( !token.empty() && ! id.empty())
  {
    if ( !DiveAgent::instance().login_fb(id, token) )
    {
      wxString msg = wxString::FromUTF8((std::string("Login errors: ") + DiveAgent::instance().getErrors()).c_str());
      wxMessageDialog* dlg = new wxMessageDialog(this, msg, wxString::FromUTF8("Dive agent"));
      dlg->ShowModal();
      dlg->Destroy();
    }
    else
    {
      showAccountInfo();
    }
  }
  else
  {
    wxString msg = wxString::FromUTF8("");
    wxMessageDialog* dlg = new wxMessageDialog(this, msg, wxString::FromUTF8("Dive agent"));
    dlg->ShowModal();
    dlg->Destroy();
    
  }
}

void PreferencesDialog::showAccountInfo()
{
  m_accountStatic->SetLabel(wxString::FromUTF8((std::string("Diveboard account: ") + DiveAgent::instance().getLogedUser()).c_str()));
  if (!DiveAgent::instance().getLogedUserPicture().empty())
  {
    // ToDo: implement load directly from std::vector<char*>
    wxString tmp_file = wxFileName::CreateTempFileName("DiveAgent");
    std::ofstream ofs(tmp_file.utf8_str().data(), std::ios::out | std::ios::binary);
    ofs.write(&DiveAgent::instance().getLogedUserPicture()[0], DiveAgent::instance().getLogedUserPicture().size());
    ofs.close();
    wxImage img(tmp_file.utf8_str().data());
    wxSize sz_image  = img.GetSize();
    wxSize sz_bitmap = m_accountBitmap->GetSize();
    double scale = std::min((double)sz_bitmap.x / (double) sz_image.x, (double)sz_bitmap.y / (double) sz_image.y);
    img.Rescale(sz_image.x * scale, sz_image.y * scale);
    wxBitmap p(img);
    m_accountBitmap->SetBitmap(p);
  }
  m_accauntInfoPanel->Show(true);
  m_accountSetPanel->Show(false);
  GetSizer()->Fit(this);
};

void PreferencesDialog::showSetAccount()
{
  m_accountStatic->SetLabel(wxString::FromUTF8("Diveboard account: "));
  m_accauntInfoPanel->Show(false);
  m_accountSetPanel->Show(true);
  GetSizer()->Fit(this);
};


void PreferencesDialog::loginButtonOnButtonClick( wxCommandEvent& event )
{
  if ( !DiveAgent::instance().login_email(m_emailText->GetValue().utf8_str().data(),
                                           m_passwordText->GetValue().utf8_str().data()) )
  {
    wxString msg = wxString::FromUTF8((std::string("Login errors: ") + DiveAgent::instance().getErrors()).c_str());
    wxMessageDialog* dlg = new wxMessageDialog(this, msg, wxString::FromUTF8("Dive agent"));
    dlg->ShowModal();
    dlg->Destroy();
  }
  else
  {
    showAccountInfo();
    DiveAgent::writeProfile("login_email", m_emailText->GetValue().utf8_str().data());
  }
}

void PreferencesDialog::unlinkButtonOnButtonClick( wxCommandEvent& event )
{
  DiveAgent::instance().logoff();
  showSetAccount();
}

void PreferencesDialog::unlinkButtonOnUpdateUI( wxUpdateUIEvent& e )
{
  e.Enable(!DiveAgent::instance().isUploadDivesRuning());
};

void PreferencesDialog::loginButtonOnUpdateUI( wxUpdateUIEvent& e )
{
  e.Enable(!DiveAgent::instance().isUploadDivesRuning());
};

void PreferencesDialog::FBconnectButtonOnUpdateUI( wxUpdateUIEvent& e )
{
  e.Enable(!DiveAgent::instance().isUploadDivesRuning());
};
