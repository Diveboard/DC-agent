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

#include "MainFrame.h"
#include "DiveAgent.h"
#include "DiveAgentApp.h"

#include "AboutDialog.h"
#include <wx/snglinst.h>

namespace {
  UploadDivesProgressDialog*  uploadDivesProgressDialog=0;
  AboutDilog*                 aboutDilog=0;
  MainFrame*		              mainFrame=0;
  wxSingleInstanceChecker*    m_checker=0;

  void createDialogs()
  {
    uploadDivesProgressDialog = new UploadDivesProgressDialog;
    aboutDilog                = new AboutDilog();
  };

  void destroyDalogs()
  {
    if (uploadDivesProgressDialog)
      delete uploadDivesProgressDialog;
    if (aboutDilog)
      delete aboutDilog;
  };
}

void setCurrentDialog(wxDialog *d, bool show)
{
  // currentDialog = d;
  if (show)
  {
    d->Raise();
    d->Show();
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

enum
{
  PU_RESTORE = 10001,
  PU_EXIT
};

BEGIN_EVENT_TABLE(DiveAgentTaskBarIcon, wxTaskBarIcon)
EVT_MENU(PU_EXIT,    DiveAgentTaskBarIcon::OnMenuExit)
EVT_TASKBAR_LEFT_DOWN  (DiveAgentTaskBarIcon::OnLeftButtonDClick)
END_EVENT_TABLE()

void DiveAgentTaskBarIcon::OnMenuExit(wxCommandEvent& )
{
  wxExit();
}

// Overridables
wxMenu *DiveAgentTaskBarIcon::CreatePopupMenu()
{
  wxMenu *menu = new wxMenu;
  if ( !haveQuitMenuFromSystem() )
  {
    menu->AppendSeparator();
    menu->Append(PU_EXIT,    wxT("E&xit"));
  }
  return menu;
}

void DiveAgentTaskBarIcon::OnLeftButtonDClick(wxTaskBarIconEvent&)
{
  wxCommandEvent e;
  OnMenuUploadDives(e);
  SureProcessToForeground();
}

void DiveAgentTaskBarIcon::OnMenuUploadDives(wxCommandEvent&)
{
  mainFrame->Show(true);
  SureProcessToForeground();
};

// ----------------------------------------------------------------------------
// DiveAgentApp
// ----------------------------------------------------------------------------

IMPLEMENT_APP(DiveAgentApp)

DiveAgentApp::DiveAgentApp()
{
  //    RedirectIOToConsole();
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

  createDocIcon();
  createDialogs();
  mainFrame = new MainFrame();
  mainFrame->setProgressDialog(uploadDivesProgressDialog);
  mainFrame->Show(true);
  return true;
}

int DiveAgentApp::OnExit()
{
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

