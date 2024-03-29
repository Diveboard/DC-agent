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
#include "ImageSize.h"
#include "Logger.h"

namespace
{
  // get custorm item data as string id
  std::string getItemId(wxChoice* c, int i)
  {
    wxClientData* data = c->GetClientObject(i);
    wxStringClientData* string_data = data ? dynamic_cast<wxStringClientData*>(data) : 0;
    if (string_data)
      return string_data->GetData().utf8_str().data();
    return std::string();
  };

  void setSelectionFromProfile(wxChoice* c, const std::string& setting_name)
  {
    const int default_index = 0;
    std::string setting_value = DiveAgent::readProfile(setting_name);
    if ( !setting_value.empty() )
    {
      int index = c->FindString(wxString::FromUTF8(setting_value.c_str()));
      index = index != wxNOT_FOUND ? index : default_index;
      c->SetSelection(index);
    }
    else
      c->SetSelection(default_index);
  }
};

ComputerFactory& MainFrame::_f( * new ComputerFactory());
bool MainFrame::scanBT = true;

MainFrame::MainFrame() : MainDialogBase(0)
{
  InitLoginPanel();
}

void MainFrame::InitUploadDivesPanel()
{
  scaleFactor = GetContentScaleFactor();
  _expect_port_selected_manualy = false;
  showAccountInfo();
  _timer = new wxTimer(this, timer_id);
  Connect(_timer->GetId(), wxEVT_TIMER, wxTimerEventHandler(MainFrame::onTimer), NULL, this );
  _timer->Start(timer_timeout);

  m_selectPortManualCheck->SetValue(DiveAgent::readProfile("select_port_manual") == "1");
  m_selectPortPanel->Show(m_selectPortManualCheck->GetValue());
  GetSizer()->Fit(this);
  std::map <std::string, std::string> ports = _f.allPorts();
  m_selectPortChoice->Clear();
  for (auto it=ports.begin(); it!= ports.end(); ++it)
  {
    // it->first device, it->second device name
    m_selectPortChoice->Append(it->second.c_str(),
                               new wxStringClientData(it->first.c_str()));
  }
  if (ports.size() == 0)
    m_selectPortChoice->Append("", new wxStringClientData(""));
  m_selectMakeChoice->Clear();
  for (auto it=_f.supported.begin(); it!=_f.supported.end();++it)
  {
    m_selectMakeChoice->Append(wxString::FromUTF8(it->first.c_str()));
  }

  // resotore previos selections
  // set port
  setSelectionFromProfile(m_selectPortChoice, "dive_computer_port");
  // set make
  setSelectionFromProfile(m_selectMakeChoice, "dive_computer_make");
  // update select Model combo
  wxCommandEvent e;
  selectMakeChoiceOnChoice(e);

  _th = boost::thread(&workingThread);
}

void MainFrame::loadUploadDivesPanel()
{
  refreshLoggedInMenuButton();
  m_upload_dive->Show();
  m_login_panel->Hide();
  m_upload_dive->Layout();
  this->Layout();
  GetSizer()->Fit(this);
  InitUploadDivesPanel();
}

void MainFrame::InitLoginPanel()
{
  Logger::append("Init Login panel");
  m_login_panel->Show();
  m_upload_dive->Hide();
  m_login_panel->Layout();
  m_login_panel->Fit();
  this->Layout();
  GetSizer()->Fit(this);
  std::string email_value = DiveAgent::readProfile("login_email");
  if ( !email_value.empty() )
  {
    m_emailText->ChangeValue(wxString::FromUTF8(email_value.c_str()));
  }
  // showSetAccount();
  try
  {
    if (DiveAgent::instance().restore_login() && !DiveAgent::instance().getLoggedInUser().empty() && !DiveAgent::instance().isLoginExpired())
    {
      Logger::append("User already logged in");

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
  scanBT = false;
  /* no need to wait for the bt thread // _th.join(); */
  delete &_f;
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

void MainFrame::loginButtonOnButtonClick( wxCommandEvent& event )
{
  Logger::append("Click on login button");

  try
  {
    if ( !DiveAgent::instance().login_email(m_emailText->GetValue().utf8_str().data(),
                                            m_passwordText->GetValue().utf8_str().data()) )
    {
      Logger::append((std::string("Login errors: ") + DiveAgent::instance().getErrors()).c_str());
      wxString msg = wxString::FromUTF8((std::string("Login errors: ") + DiveAgent::instance().getErrors()).c_str());
      wxMessageDialog* dlg = new wxMessageDialog(this, msg, wxString::FromUTF8("Dive agent"));
      dlg->ShowModal();
      dlg->Destroy();
    }
    else
    {
      Logger::append("Success login");
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
  Logger::append("Click on facebook login button");

  std::string token;
  std::string id;
  FacbookAuthDialog* dlg = new FacbookAuthDialog(0,  wxID_ANY);
  dlg->ShowModal();
  token = dlg->Token();
  //TODO CURL to get token
  //id  = dlg->Id();
  //https://www.diveboard.com/connect/login_success.html?#access_token=CAACwQ4E5CZCwBAMCj5rrYCyAEXZC2lvbc6gCZCFZBDgzmWUqz3IMoabR2HkL7UO8jENpZC2OYCDNWXxfD5YDbZBuK9pYAtHacLPAet6CHmjGLjHKbIZCyL5Ly3AZCvZCVN1g3ZAIZBwnkZByfdWxFle0N8783q0EvwPEg5p9kf6rfnjSoEdxH792P1Y5Sq0j3AOBZAuJ9pMqOdeF0caZBaBusEVmYT&expires_in=7147
  dlg->Destroy();
  id = DiveAgent::instance().get_id_from_token(token);

  if ( !token.empty() && ! id.empty())
  {
    try
    {
      if ( !DiveAgent::instance().login_fb(id, token) )
      {
        Logger::append((std::string("Login errors: ") + DiveAgent::instance().getErrors()).c_str());
        wxString msg = wxString::FromUTF8((std::string("Login errors: ") + DiveAgent::instance().getErrors()).c_str());
        wxMessageDialog* dlg = new wxMessageDialog(this, msg, wxString::FromUTF8(DiveAgent::AppName().c_str()));
        dlg->ShowModal();
        dlg->Destroy();
      }
      else
      {
      Logger::append("Success facebook login");
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
    Logger::append("Login have not been completed");
    wxString msg = wxString::FromUTF8("Login have not been completed.");
    wxMessageDialog* dlg = new wxMessageDialog(this, msg, wxString::FromUTF8("Dive agent"));
    dlg->ShowModal();
    dlg->Destroy();
    
  }
}

void MainFrame::showAccountInfo()
{
  Logger::append("Load user info");
  m_login->SetLabel(wxString::FromUTF8((std::string("Logged in as: ") + DiveAgent::instance().getLoggedInUser()).c_str()));
  if (!DiveAgent::instance().getLoggedInUserPicture().empty())
  {
    // ToDo: implement load directly from std::vector<char*>
    wxString tmp_file = wxFileName::CreateTempFileName("DiveboardAgent");
    std::ofstream ofs(tmp_file.utf8_str().data(), std::ios::out | std::ios::binary);
    ofs.write(&DiveAgent::instance().getLoggedInUserPicture()[0], DiveAgent::instance().getLoggedInUserPicture().size());
    ofs.close();
    wxImage img(tmp_file.utf8_str().data());
    if (!img.IsOk())
    {
      Logger::append("Unable to display user picture");
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


void MainFrame::onTimer( wxTimerEvent& event)
{
  if (DiveAgent::instance().isUploadDivesRuning()) return;

  //wxLogError("onTimerMainFrame");
  std::string port_selected = getItemId(m_selectPortChoice, m_selectPortChoice->GetCurrentSelection());
  if (++_timer_counter >= timer_mx_count)
  {
    _timer_counter = 0;
    // update port choice control
    std::map <std::string, std::string> ports = _f.allPorts();
    m_selectPortChoice->Clear();
    int selection = 0;

    //auto-select newly detected "native" bluetooth devices
    for (auto it=ports.begin(); it!= ports.end(); ++it)
    {
      // it->first device, it->second device name
      m_selectPortChoice->Append(it->second.c_str(),
                                 new wxStringClientData(it->first.c_str()));
      if (port_selected.compare(0, 5, "/dev/") != 0 && port_selected.compare(0, 7, "\\\\.\\COM") != 0) {
        //only auto-change to a bt-device *once*
      }
      else {
        if (it->first.compare(0, 5, "/dev/") != 0 && it->first.compare(0, 7, "\\\\.\\COM") != 0) {
          selection = m_selectPortChoice->GetCount()-1;
          port_selected = it->first;
        }
      }
    }
    if (ports.size() == 0)
      m_selectPortChoice->Append("", new wxStringClientData(""));

    // make some default port selection
    m_selectPortChoice->SetSelection(selection);
    // try to restore previos port selection
    for (int i=0; i < m_selectPortChoice->GetCount(); ++i)
    { // find port in port combo
      std::string port = getItemId(m_selectPortChoice, i);
      if (port_selected == port)
      { // select
        m_selectPortChoice->SetSelection(i);
        break;
      }
    }
  }
}

void MainFrame::selectMakeChoiceOnChoice( wxCommandEvent& )
{
  m_selectModelChoice->Clear();
  std::string make = m_selectMakeChoice->GetStringSelection().utf8_str().data();
  // create sorted models storage
  std::map< std::string, std::string> models;
  auto& models_unsorted = _f.supported[make];
  for (auto it=models_unsorted.begin(); it!=models_unsorted.end();++it)
    models[it->model] = it->key_code;
  // update model choice UI
  for (auto it=models.begin(); it!=models.end();++it)
  {
    m_selectModelChoice->Append(wxString::FromUTF8(it->first.c_str()),
                              new wxStringClientData(wxString::FromUTF8(it->second.c_str())));

  }
  // set model
  setSelectionFromProfile(m_selectModelChoice, "dive_computer_model");
}

void MainFrame::uploadDivesButtonOnButtonClick( wxCommandEvent& event)
{
    Logger::append("Start uploading dives");

  // check if the user is logged in and the login is not expired
  if (DiveAgent::instance().getLoggedInUser().empty() ||
      (!DiveAgent::instance().getLoggedInUser().empty() && DiveAgent::instance().isLoginExpired()))
  {
    Logger::append("User is not logged in or login is expired");

    wxString msg = wxString::FromUTF8("User is not logged in or login is expired.\nPlease, try to login.");
    wxMessageDialog* dlg = new wxMessageDialog(this, msg, wxString::FromUTF8(DiveAgent::AppName().c_str()));
    dlg->ShowModal();
    dlg->Destroy();
    // Hide();
    // _preferences_dialog->Show();
    return;
  }
  std::string c = getItemId(m_selectModelChoice, m_selectModelChoice->GetCurrentSelection());
  // try to gess port
  if (!m_selectPortManualCheck->GetValue() && !_expect_port_selected_manualy)
  {
    bool is_port_autoselected = false;
    std::string port_detected;
    try
    {
      port_detected = _f.detectConnectedDevice(c);
    }
    catch (std::exception&)
    {
    };
    if ( !port_detected.empty())
    {
      for (int i=0; i < m_selectPortChoice->GetCount(); ++i)
      { // find port in port combo
        std::string port = getItemId(m_selectPortChoice, i);
        if (port_detected == port)
        { // select
          m_selectPortChoice->SetSelection(i);
          is_port_autoselected = true;
        }
      }
    }
    if (!is_port_autoselected)
    {
      m_selectPortPanel->Show();
      GetSizer()->Fit(this);
      _expect_port_selected_manualy = true;
      Logger::append("Unable to detect port autmaticaly.\nPlease, select it manualy and try again.");

      wxString msg = wxString::FromUTF8("Unable to detect port autmaticaly.\nPlease, select it manualy and try again.");
      wxMessageDialog* dlg = new wxMessageDialog(this, msg, wxString::FromUTF8("Dive agent"));
      dlg->ShowModal();
      dlg->Destroy();
      return;
    }
    else
    {
      m_selectPortPanel->Hide();
      GetSizer()->Fit(this);
    }
  }
  _expect_port_selected_manualy = false;
  if (!m_selectPortManualCheck->GetValue())
  {
    m_selectPortPanel->Hide();
    GetSizer()->Fit(this);
  }

  scanBT = false;
  _th.join();

  // save selection
  DiveAgent::writeProfile("dive_computer_port", m_selectPortChoice->GetStringSelection().utf8_str().data());
  DiveAgent::writeProfile("dive_computer_make", m_selectMakeChoice->GetStringSelection().utf8_str().data());
  DiveAgent::writeProfile("dive_computer_model", m_selectModelChoice->GetStringSelection().utf8_str().data());
  std::string select_port_manual = m_selectPortManualCheck->GetValue() ? "1" : "0";
  DiveAgent::writeProfile("select_port_manual", select_port_manual);
  //  start upload
  std::string p = getItemId(m_selectPortChoice, m_selectPortChoice->GetCurrentSelection());
  DiveAgent::instance().startUploadDives(c, p);
  assert(_progress_dialog);
  setCurrentDialog(_progress_dialog);
  _progress_dialog->enableMonitoring();
  this->Hide();
  setIsLoginEnable(false);
}

void MainFrame::onClose( wxCloseEvent& event ) 
{
  this->Hide();
}

void MainFrame::workingThread()
{
  try
  {
    while(scanBT) {
    	if (!DiveAgent::instance().isUploadDivesRuning()) {
      	_f.allPorts(true); //(re-)scan for bt devices
      }
      boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    }
  }
  catch( boost::thread_interrupted& )
  {
  }
  catch (std::exception& e)
  {
    reportError(e.what());
  }
};

