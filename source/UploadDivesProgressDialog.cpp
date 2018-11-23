#include "stdafx.h"
#include "UploadDivesProgressDialog.h"
#include "ComputerFactory.h"
#include "DiveAgent.h"
#include "DiveAgentApp.h"
#include "MainFrame.h"
#include "Logger.h"
#include <wx/msgdlg.h>

UploadDivesProgressDialog::UploadDivesProgressDialog():
  UploadDivesProgressDialogBase(0),
  _monitoring(false)
{
  m_uploadProgressGauge->SetRange(100);
  _timer = new wxTimer(this, timer_id);
  Connect(_timer->GetId(), wxEVT_TIMER, wxTimerEventHandler(UploadDivesProgressDialog::onTimer), NULL, this );
  _timer->Start(timer_timeout);
};

void UploadDivesProgressDialog::actionButtonOnButtonClick( wxCommandEvent& event )
{
  DiveAgent::instance().cancelUploadDives();
  this->Hide();
  setCurrentDialog(mainFrame);
  setIsLoginEnable(true);
  mainFrame->Show();
  disableMonitoring();
};
void UploadDivesProgressDialog::hideProgressGauge()
{
  m_uploadProgressGauge->Hide();
  m_uploadProgressStatic->Hide();
};
void UploadDivesProgressDialog::showProgressGauge()
{
  m_uploadProgressGauge->Show();
  m_uploadProgressStatic->Show();
};

void UploadDivesProgressDialog::onTimer( wxTimerEvent& event)
{
  if (_monitoring)
  {
    int uploadDivesProgress = DiveAgent::instance().uploadDivesProgress();
    if (1 <= uploadDivesProgress && uploadDivesProgress <= 100) {
      m_uploadProgressGauge->SetValue(uploadDivesProgress);
      std::stringstream s;
      s << m_uploadProgressGauge->GetValue() << " %";
      m_uploadProgressStatic->SetLabel(wxString::FromUTF8(s.str().c_str()));
    }

    if (DiveAgent::instance().isDivesXmlReady() && _wait_dive_xml)
    {
      _wait_dive_xml = false;
      m_statusStatic->SetLabel(wxString::FromUTF8("Status: uploading dives to Diveboard ..."));
      GetSizer()->Fit(this);
    }
    if (!DiveAgent::instance().isUploadDivesRuning())
    {
      std::string error = DiveAgent::instance().getErrors();
      if (!error.empty())
      {
        wxMessageOutputMessageBox().Output(wxString::FromUTF8((std::string("There was errors while uploading dives: ") + error).c_str()));
        Logger::append((std::string("There was errors while uploading dives: ") + error).c_str());
      }
      disableMonitoring();
      if (error.empty())
      {
        m_statusStatic->SetLabel(wxString::FromUTF8("Your dive computer data have been successfully sent to Diveboard.\nTo complete the process you must now click on the button below.\n"));
        m_doneButton->Show();
        m_actionButton->Hide();
        hideProgressGauge();
        GetSizer()->Fit(this);
      }
      else
      {
        setIsLoginEnable(true);
        mainFrame->Show();
        setCurrentDialog(mainFrame);
        Hide();
      }
    }
  }
};

void UploadDivesProgressDialog::enableMonitoring()
{
  _monitoring = true;
  _wait_dive_xml = true;
  m_uploadProgressGauge->SetValue(0);
  m_uploadProgressStatic->SetLabel(wxString::FromUTF8("0 %"));
  m_statusStatic->SetLabel(wxString::FromUTF8("Status: uploading dives ..."));
  // m_FinalURLhyperlink->Hide();
  m_doneButton->Hide();
  m_actionButton->Show();
  showProgressGauge();
  GetSizer()->Fit(this);
}

void UploadDivesProgressDialog::doneButtonOnButtonClick( wxCommandEvent& event )
{
  std::string url = DiveAgent::instance().completionURL();
  if (!url.empty())
  {
    wxLaunchDefaultBrowser(wxString::FromUTF8(url.c_str()));
    // if something's wrong with the user's browser then
    // let's give the user a chance to copy the string
    // from stdout
    printf("%s\n",url.c_str());
    Logger::append((std::string("Finalize on Diveboard: ") + url).c_str());
  }
  setIsLoginEnable(true);
  setCurrentDialog(mainFrame);
  Hide();
}

