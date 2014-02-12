#include "UploadDivesDialog.h"
#include "ComputerFactory.h"
#include "DiveAgent.h"
#include "DiveAgentApp.h"

namespace
{
  ComputerFactory f;
  // get custorm item data as string id
  std::string getItemId(wxComboBox* c, int i)
  {
    wxClientData* data = c->GetClientObject(i);
    wxStringClientData* string_data = data ? dynamic_cast<wxStringClientData*>(data) : 0;
    if (string_data)
      return string_data->GetData().utf8_str().data();
    return std::string();
  };

};

UploadDivesDialog::UploadDivesDialog(): UploadDivesDialogBase(0)
{
  
  std::map <std::string, std::string> ports = f.allPorts();
  m_selectPortCombo->Clear();
  for (auto it=ports.begin(); it!= ports.end(); ++it)
  {
    // it->first device, it->second device name
    m_selectPortCombo->Append(wxString::FromUTF8(it->second.c_str()),
                              new wxStringClientData(wxString::FromUTF8(it->first.c_str())));
  }
  
  
  m_selectComputerCombo->Clear();
  for (auto it=f.supported.begin(); it!=f.supported.end();++it)
  {
    m_selectComputerCombo->Append(wxString::FromUTF8((it->label + " : " + it->key_code).c_str()),
                                  new wxStringClientData(wxString::FromUTF8(it->key_code.c_str())));
  }
}

void UploadDivesDialog::selectComputerComboOnCombobox( wxCommandEvent& )
{
  int selection = m_selectComputerCombo->GetCurrentSelection();
  std::string key_code = getItemId(m_selectComputerCombo, selection);
  // find item
  for (auto it=f.supported.begin(); it!=f.supported.end();++it)
    if (it->key_code == key_code)
      { // try to gess port
        auto ports = it->ports;
        for (auto it_ports = ports.begin(); it_ports != ports.end(); ++it_ports)
          for (int i=0; i < m_selectPortCombo->GetCount(); ++i)
          { // find port in port combo
            std::string port = getItemId(m_selectPortCombo, i);
            if (port == *it_ports)
            { // select
              m_selectPortCombo->SetSelection(i);
              return;
            }
          }
      }
}

void UploadDivesDialog::selectPortComboOnCombobox( wxCommandEvent& event)
{
  event.Skip();
}

void UploadDivesDialog::uploadDivesButtonOnButtonClick( wxCommandEvent& event)
{
  int selection = m_selectComputerCombo->GetCurrentSelection();
  wxClientData* data = m_selectComputerCombo->GetClientObject(selection);
  wxStringClientData* string_data = data ? dynamic_cast<wxStringClientData*>(data) : 0;
  assert(string_data);
  
  std::string c = getItemId(m_selectComputerCombo, m_selectComputerCombo->GetCurrentSelection());
  std::string p = getItemId(m_selectPortCombo, m_selectPortCombo->GetCurrentSelection());
  DiveAgent::instance().startUploadDives(c, p);
  assert(_progress_dialog);
  setCurrentDialog(_progress_dialog);
  _progress_dialog->enableMonitoring();
}

UploadDivesProgressDialog::UploadDivesProgressDialog():
  UploadDivesProgressDialogBase(0),
  _monitoring(false)
{
  _timer = new wxTimer(this, timer_id);
  _timer->Connect( wxEVT_TIMER, wxTimerEventHandler(UploadDivesProgressDialog::onTimer), NULL, this );
  _timer->Start(100);
  m_uploadProgressGauge->SetRange(100);
};

void UploadDivesProgressDialog::actionButtonOnButtonClick( wxCommandEvent& event )
{
  DiveAgent::instance().cancelUploadDives();
  assert(_main_dialog);
  setCurrentDialog(_main_dialog);
  disableMonitoring();
};

void UploadDivesProgressDialog::onTimer( wxTimerEvent& event)
{
  if (_monitoring)
  {
    m_uploadProgressGauge->SetValue(DiveAgent::instance().uploadDivesProgress());
    if (!DiveAgent::instance().isUploadDivesRuning())
    {
      std::string error = DiveAgent::instance().getErrors();
      if (!error.empty())
      {
        wxMessageOutputMessageBox().Output(wxString::FromUTF8((std::string("There was errors while uploding dives: ") + error).c_str()));
      }
      setCurrentDialog(_main_dialog);
      disableMonitoring();
    }
  }
};
