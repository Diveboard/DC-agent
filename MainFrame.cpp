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
