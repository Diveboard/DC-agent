#include "stdafx.h"
#include "MainFrame.h"
#include "DiveAgent.h"
#include <wx/msgdlg.h>
#include <wx/filename.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <fstream>
#include "DiveAgentApp.h"
#include "FacebookAuthDialog.h"

MainFrame::MainFrame() : MainFrameBase(0)
{
  InitLoginPanel();
}

void MainFrame::InitLoginPanel()
{
  std::string email_value = DiveAgent::readProfile("login_email");
  if ( !email_value.empty() )
  {
    m_emailText->ChangeValue(wxString::FromUTF8(email_value.c_str()));
  }
  // showSetAccount();
  try
  {
    if (DiveAgent::instance().restore_login())
    {
      loadUploadDivesPanel();
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



MainFrame::~MainFrame()
{
}

void MainFrame::OnLeftDown(wxMouseEvent& evt)
{
  CaptureMouse();
  wxPoint pos = ClientToScreen(evt.GetPosition());
  wxPoint origin = GetPosition();
  int dx =  pos.x - origin.x;
  int dy = pos.y - origin.y;
  m_delta = wxPoint(dx, dy);
}


void MainFrame::OnMouseMove(wxMouseEvent& evt)
{
  wxPoint pt = evt.GetPosition();
  if (evt.Dragging() && evt.LeftIsDown())
    {
      wxPoint pos = ClientToScreen(pt);
      Move(wxPoint(pos.x - m_delta.x, pos.y - m_delta.y));
    }
}

void MainFrame::OnLeftUp(wxMouseEvent& evt)
{
  if (HasCapture())
    {
      ReleaseMouse();
    }
}

void MainFrame::loadUploadDivesPanel()
{
  m_upload_dive->Show();
  m_login_panel->Hide();
  m_login_panel->Layout();
  showAccountInfo();
  this->Layout();
}

void MainFrame::loginButtonOnButtonClick( wxCommandEvent& event )
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
      loadUploadDivesPanel();
      // showAccountInfo();
      DiveAgent::writeProfile("login_email", m_emailText->GetValue().utf8_str().data());
    }
  }
  catch (std::exception& e)
  {
    reportError(e.what());
  };
}

void MainFrame::FBconnectButtonOnButtonClick( wxCommandEvent& event )
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
      loadUploadDivesPanel();
        // showAccountInfo();
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

void MainFrame::showAccountInfo()
{
  m_login->SetLabel(wxString::FromUTF8((std::string("Diveboard account: ") + DiveAgent::instance().getLogedUser()).c_str()));
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
      wxSize sz_bitmap = m_avatar->GetSize();
      double scale = std::min((double)sz_bitmap.x / (double) sz_image.x, (double)sz_bitmap.y / (double) sz_image.y);
      img.Rescale(sz_image.x * scale, sz_image.y * scale);
      wxBitmap p(img);
      m_avatar->SetBitmap(p);
    }
  }
  // m_accauntInfoPanel->Show(true);
  // m_accountSetPanel->Show(false);
  GetSizer()->Fit(this);
}
