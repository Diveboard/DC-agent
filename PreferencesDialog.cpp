#include "PreferencesDialog.h"
#include "FacebookAuthDialog.h"
#include "DiveAgent.h"
#include <wx/msgdlg.h>
#include <wx/filename.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <fstream>
#include "DiveAgentApp.h"


PreferencesDialog::PreferencesDialog():
  PreferencesDialogBase(0)
{
  std::string email_value = DiveAgent::readProfile("login_email");
  if ( !email_value.empty() )
  {
    m_emailText->ChangeValue(wxString::FromUTF8(email_value.c_str()));
  }
  showSetAccount();
  try
  {
    if (DiveAgent::instance().restore_login())
    {
      showAccountInfo();
    }
  }
  catch (std::exception& e)
  {
    wxString msg = wxString::FromUTF8((std::string("Unexpected error:\n") + e.what()).c_str());
    wxMessageDialog* dlg = new wxMessageDialog(this, msg, wxString::FromUTF8("Dive agent"));
    dlg->ShowModal();
    dlg->Destroy();
  };
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
    try
    {
      if ( !DiveAgent::instance().login_fb(id, token) )
      {
        wxString msg = wxString::FromUTF8((std::string("Login errors: ") + DiveAgent::instance().getErrors()).c_str());
        wxMessageDialog* dlg = new wxMessageDialog(this, msg, wxString::FromUTF8(DiveAgent::AppName().c_str()));
        dlg->ShowModal();
        dlg->Destroy();
      }
      else
      {
        showAccountInfo();
      }
    }
    catch (std::exception& e)
    {
      reportError(e.what());
    };
  }
  else
  {
    wxString msg = wxString::FromUTF8("Login have not been completed.");
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
    wxString tmp_file = wxFileName::CreateTempFileName("DiveboardAgent");
    std::ofstream ofs(tmp_file.utf8_str().data(), std::ios::out | std::ios::binary);
    ofs.write(&DiveAgent::instance().getLogedUserPicture()[0], DiveAgent::instance().getLogedUserPicture().size());
    ofs.close();
    wxImage img(tmp_file.utf8_str().data());
    if (!img.IsOk())
    {
      reportError("unable to display user picture");
    }
    else
    {
      wxSize sz_image  = img.GetSize();
      wxSize sz_bitmap = m_accountBitmap->GetSize();
      double scale = std::min((double)sz_bitmap.x / (double) sz_image.x, (double)sz_bitmap.y / (double) sz_image.y);
      img.Rescale(sz_image.x * scale, sz_image.y * scale);
      wxBitmap p(img);
      m_accountBitmap->SetBitmap(p);
    }
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
  try
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
  catch (std::exception& e)
  {
    reportError(e.what());
  };
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
