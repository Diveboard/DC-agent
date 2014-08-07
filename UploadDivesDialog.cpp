#include "stdafx.h"
#include "UploadDivesDialog.h"
#include "ComputerFactory.h"
#include "DiveAgent.h"
#include "DiveAgentApp.h"
#include <wx/msgdlg.h>
#include "PreferencesDialog.h"

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

UploadDivesDialog::UploadDivesDialog():
  UploadDivesDialogBase(0),
  _expect_port_selected_manualy(false),
  _f( * new ComputerFactory())
{
  _timer = new wxTimer(this, timer_id);
  Connect(_timer->GetId(), wxEVT_TIMER, wxTimerEventHandler(UploadDivesDialog::onTimer), NULL, this );
  _timer->Start(timer_timeout);

  m_selectPortManualCheck->SetValue(DiveAgent::readProfile("select_port_manual") == "1");
  m_selectPortPanel->Show(m_selectPortManualCheck->GetValue());
  GetSizer()->Fit(this);

  std::map <std::string, std::string> ports = _f.allPorts();
  m_selectPortChoice->Clear();
  for (auto it=ports.begin(); it!= ports.end(); ++it)
  {
    // it->first device, it->second device name
    m_selectPortChoice->Append(wxString::FromUTF8(it->second.c_str()),
                              new wxStringClientData(wxString::FromUTF8(it->first.c_str())));
  }
  if (ports.size() == 0)
    m_selectPortChoice->Append(wxString::FromUTF8(""), new wxStringClientData(wxString::FromUTF8("")));
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
}

UploadDivesDialog::~UploadDivesDialog()
{
  delete &_f;
}
void UploadDivesDialog::selectMakeChoiceOnChoice( wxCommandEvent& )
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

void UploadDivesDialog::uploadDivesButtonOnButtonClick( wxCommandEvent& event)
{
  // check is loged in and not expired
  if (DiveAgent::instance().getLogedUser().empty() ||
      (!DiveAgent::instance().getLogedUser().empty() && DiveAgent::instance().isLoginExpired()))
  {
    wxString msg = wxString::FromUTF8("User is not logged in or login is expired.\nPlease, try to login.");
    wxMessageDialog* dlg = new wxMessageDialog(this, msg, wxString::FromUTF8(DiveAgent::AppName().c_str()));
    dlg->ShowModal();
    dlg->Destroy();
    Hide();
    _preferences_dialog->Show();
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
  this->Hide();
  setCurrentDialog(_progress_dialog);
  _progress_dialog->enableMonitoring();
}

void UploadDivesDialog::onTimer( wxTimerEvent& event)
{
  std::string port_selected = getItemId(m_selectPortChoice, m_selectPortChoice->GetCurrentSelection());
  if (++_timer_counter >= timer_mx_count)
  {
    _timer_counter = 0;
    // update port choice control
    std::map <std::string, std::string> ports = _f.allPorts();
    m_selectPortChoice->Clear();
    for (auto it=ports.begin(); it!= ports.end(); ++it)
    {
      // it->first device, it->second device name
      m_selectPortChoice->Append(wxString::FromUTF8(it->second.c_str()),
                                 new wxStringClientData(wxString::FromUTF8(it->first.c_str())));
    }
    if (ports.size() == 0)
      m_selectPortChoice->Append(wxString::FromUTF8(""), new wxStringClientData(wxString::FromUTF8("")));

    // make some default port selection
    m_selectPortChoice->SetSelection(0);
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

