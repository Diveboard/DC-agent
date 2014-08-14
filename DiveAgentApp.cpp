/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// the application icon (under Windows and OS/2 it is in resources)
//#include "icon_bw.xpm"
//#include "new_icon_yellow.xpm"
#include "icon_systrail.png.h"
//#include "Win/Guicon.h"
#include "MainFrame.h"
#include "DiveAgent.h"
#include "DiveAgentApp.h"
#include "Logger.h"
#include "AboutDialog.h"
#include <wx/snglinst.h>
 
namespace {
  UploadDivesProgressDialog*  uploadDivesProgressDialog=0;
  AboutDilog*                 aboutDialog=0;
  MainFrame*		              mainFrame=0;
  wxSingleInstanceChecker*    m_checker=0;

  void createDialogs()
  {
    Logger::append("Start loading all Dialog in startup");
    mainFrame = new MainFrame();
    uploadDivesProgressDialog = new UploadDivesProgressDialog;
    aboutDialog                = new AboutDilog();
    
    setCurrentDialog(mainFrame);

    uploadDivesProgressDialog->setMainFrame(mainFrame);
    mainFrame->setProgressDialog(uploadDivesProgressDialog);
    mainFrame->Raise();
    mainFrame->Show();
    Logger::append("Finish loading all Dialog in startup");
  };

  void destroyDalogs()
  {
    if (uploadDivesProgressDialog)
      delete uploadDivesProgressDialog;
    if (aboutDialog)
      delete aboutDialog;
  };
}

wxDialog *currentDialog;
bool isLoginEnable = true;

void setIsLoginEnable(bool e)
{
  isLoginEnable = e;
}

void setCurrentDialog(wxDialog *d, bool show)
{
   currentDialog = d;
  if (show)
  {
    d->Show();
    d->Raise();
  }
  else
    d->Hide();
};

void reportError(const std::string& error)
{
    wxString msg = wxString::FromUTF8((std::string("Unexpected error:\n") + error).c_str());
    wxMessageDialog* dlg = new wxMessageDialog(NULL, msg, wxString::FromUTF8(DiveAgent::AppName().c_str()));
    dlg->ShowModal();
    dlg->Destroy();
}
// ----------------------------------------------------------------------------
// DiveAgentTaskBarIcon implementation
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(DiveAgentTaskBarIcon, wxTaskBarIcon)
EVT_MENU(PU_UPLOAD_DIVES,    DiveAgentTaskBarIcon::OnMenuUploadDives)
EVT_MENU(PU_ABOUT,    DiveAgentTaskBarIcon::OnMenuAbout)
EVT_MENU(PU_LOGOUT,    DiveAgentTaskBarIcon::OnMenuLogout)
EVT_MENU(PU_LOG,    DiveAgentTaskBarIcon::OnMenuLog)
EVT_MENU(PU_EXIT,    DiveAgentTaskBarIcon::OnMenuExit)
EVT_TASKBAR_LEFT_DOWN  (DiveAgentTaskBarIcon::OnLeftButtonDClick)
END_EVENT_TABLE()

void DiveAgentTaskBarIcon::OnMenuLogout(wxCommandEvent& e)
{
  if (DiveAgent::instance().restore_login())
    {
      DiveAgent::instance().logoff();
      mainFrame->InitLoginPanel();
      m_menu->SetLabel(PU_LOGOUT, wxT("Login"));
    }
  else
    {
      OnMenuUploadDives(e);
    }
}
void DiveAgentTaskBarIcon::OnMenuAbout(wxCommandEvent& )
{
  aboutDialog->Raise();
  aboutDialog->Show();
  SureProcessToForeground();
}
void DiveAgentTaskBarIcon::OnMenuExit(wxCommandEvent& )
{
  wxExit();
}
void DiveAgentTaskBarIcon::OnMenuLog(wxCommandEvent& )
{
  std::cout << Logger::toString() << std::endl;
}

// Overridables
wxMenu *DiveAgentTaskBarIcon::CreatePopupMenu()
{
  m_menu = new wxMenu();
  if ( !haveQuitMenuFromSystem() )
  {
    m_menu->Append(PU_UPLOAD_DIVES,    wxT("Upload Dives"));
    if (DiveAgent::instance().restore_login())
      m_menu->Append(PU_LOGOUT,    wxT("Logout"));
    else
      m_menu->Append(PU_LOGOUT,    wxT("Login"));
    m_menu->Append(PU_ABOUT,    wxT("About"));
    m_menu->Append(PU_LOG,    wxT("Log"));
    m_menu->Append(PU_EXIT,    wxT("Exit"));
  }
  m_menu->Enable(PU_LOGOUT, isLoginEnable);
  return m_menu;
}

void DiveAgentTaskBarIcon::OnLeftButtonDClick(wxTaskBarIconEvent&)
{
  wxCommandEvent e;
  OnMenuUploadDives(e);
  SureProcessToForeground();
}

void DiveAgentTaskBarIcon::OnMenuUploadDives(wxCommandEvent&)
{
  currentDialog->Raise();
  currentDialog->Show();
  SureProcessToForeground();
};

// ----------------------------------------------------------------------------
// DiveAgentApp
// ----------------------------------------------------------------------------

IMPLEMENT_APP(DiveAgentApp)

DiveAgentApp::DiveAgentApp()
{
  Logger::append("Start application");

  //  RedirectIOToConsole();
  m_taskBarIcon = 0;
  m_dockIcon = 0;
}

DiveAgentApp::~DiveAgentApp()
{
}

bool DiveAgentApp::OnInit()
{
  m_checker = new wxSingleInstanceChecker;
  if ( m_checker->IsAnotherRunning() )
    {
      Logger::append("Another program instance is already running, aborting.");
      wxLogError(_("Another program instance is already running, aborting."));
      delete m_checker; // OnExit() won't be called if we return false
      m_checker = NULL;
      return false;
    }

  wxInitAllImageHandlers();
  if ( !wxApp::OnInit() )
    return false;
  if ( !wxTaskBarIcon::IsAvailable() )
  {
    wxMessageBox(
            "There appears to be no system tray support in your current environment. This sample may not behave as expected.",
            "Warning",
            wxOK | wxICON_EXCLAMATION
        );
  }

  m_taskBarIcon = new DiveAgentTaskBarIcon();
  wxBitmap icon_bitmap = wxBITMAP_PNG_FROM_DATA(icon_systrail);
  wxIcon icon;
  icon.CopyFromBitmap(icon_bitmap);
  if ( !m_taskBarIcon->SetIcon(icon, "DiveAgent") )
  {
    wxLogError(wxT("Could not set icon."));
  }
  InitStartOnLogin();

  createDocIcon();
  createDialogs();
  SureProcessToForeground();
  Logger::append("Start the check for connected device timer");

  _timer = new wxTimer(this, timer_id);
  Connect(_timer->GetId(), wxEVT_TIMER, wxTimerEventHandler(DiveAgentApp::onTimer), NULL, this );
  _timer->Start(timer_timeout);

  return true;
}
void DiveAgentApp::onTimer( wxTimerEvent& event)
{
    bool port_detected;
    try
    {
      port_detected = mainFrame->getComputerFactory().isComputerPluggedin();
    }
    catch (std::exception&)
    {
    };
    if (port_detected && !mainFrame->IsShown() && !_alreadyDetected)
    {
      Logger::append("Connected device timer find a new device");

      _alreadyDetected = true;
      mainFrame->Show();
      mainFrame->Raise();
      SureProcessToForeground();
    }
    else if (!port_detected)
      _alreadyDetected = false;
}

int DiveAgentApp::OnExit()
{
  Logger::append("Exit from application");

  delete m_checker;
  // dive computer instance must be deleted befor system call to static objects distructor.
  // It is becouse dive computer implementation is using static objects from Logger.cpp in its destructor
  DiveAgent::instance().deleteDiveComputerInstance();

  if (m_taskBarIcon)
    delete m_taskBarIcon;
  if (m_dockIcon)
    delete m_dockIcon;
  destroyDalogs();
  return wxApp::OnExit();
};

