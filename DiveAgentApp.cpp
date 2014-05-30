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
#ifndef wxHAS_IMAGES_IN_RESOURCES
  #include "icon_bw.xpm"
#endif

#include "DiveAgent.h"
#include "DiveAgentApp.h"

#include "UploadDivesDialog.h"
#include "PreferencesDialog.h"
#include "AboutDialog.h"

namespace {
  UploadDivesDialog*          uploadDivesDialog=0;
  UploadDivesProgressDialog*  uploadDivesProgressDialog=0;
  PreferencesDialog*          preferencesDialog=0;
  AboutDilog*                 aboutDilog=0;
  wxDialog*                   currentDialog=0;
  
  void createDialogs()
  {
    uploadDivesDialog         = new UploadDivesDialog();
    uploadDivesProgressDialog = new UploadDivesProgressDialog;
    preferencesDialog         = new PreferencesDialog();
    aboutDilog                = new AboutDilog();
    
    uploadDivesDialog->setProgressDialog(uploadDivesProgressDialog);
    uploadDivesDialog->setPreferencesDialog(preferencesDialog);
    uploadDivesProgressDialog->setMainDialog(uploadDivesDialog);
    
    currentDialog = uploadDivesDialog;
  };
  
  void destroyDalogs()
  {
    if (uploadDivesDialog)
      delete uploadDivesDialog;
    if (uploadDivesProgressDialog)
      delete uploadDivesProgressDialog;
    if (preferencesDialog)
      delete preferencesDialog;
    if (aboutDilog)
      delete aboutDilog;
  };
}

void setCurrentDialog(wxDialog *d)
{
  currentDialog = d;
  d->Raise();
  d->Show();
};

void reportError(const std::string& error)
{
    wxString msg = wxString::FromUTF8((std::string("Unexpected error:\n") + error).c_str());
    wxMessageDialog* dlg = new wxMessageDialog(NULL, msg, wxString::FromUTF8("Dive agent"));
    dlg->ShowModal();
    dlg->Destroy();
}
// ----------------------------------------------------------------------------
// DiveAgentTaskBarIcon implementation
// ----------------------------------------------------------------------------

enum
{
  PU_RESTORE = 10001,
  PU_EXIT,
  PU_UPLOAD_DIVES,
  PU_PREFERENCES,
  PU_CHECK_FOR_UPDATES,
  PU_ABOUT
};

BEGIN_EVENT_TABLE(DiveAgentTaskBarIcon, wxTaskBarIcon)
EVT_MENU(PU_EXIT,    DiveAgentTaskBarIcon::OnMenuExit)
EVT_TASKBAR_LEFT_DCLICK  (DiveAgentTaskBarIcon::OnLeftButtonDClick)
EVT_MENU(PU_UPLOAD_DIVES, DiveAgentTaskBarIcon::OnMenuUploadDives)
EVT_MENU(PU_PREFERENCES, DiveAgentTaskBarIcon::OnMenuPreferences)
EVT_MENU(PU_CHECK_FOR_UPDATES, DiveAgentTaskBarIcon::OnMenuCheckForUpdates)
EVT_MENU(PU_ABOUT, DiveAgentTaskBarIcon::OnMenuAbout)
END_EVENT_TABLE()

void DiveAgentTaskBarIcon::OnMenuExit(wxCommandEvent& )
{
  wxExit();
}

// Overridables
wxMenu *DiveAgentTaskBarIcon::CreatePopupMenu()
{
  wxMenu *menu = new wxMenu;
  menu->Append(PU_UPLOAD_DIVES, wxT("&Upload Dives"));
  menu->Append(PU_PREFERENCES, wxT("&Preferences"));
  menu->Append(PU_CHECK_FOR_UPDATES, wxT("&Check for updates"));
  menu->Append(PU_ABOUT, wxT("&About"));
  
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
}

void DiveAgentTaskBarIcon::OnMenuUploadDives(wxCommandEvent&)
{
  currentDialog->Raise();
  currentDialog->Show(true);
};
void DiveAgentTaskBarIcon::OnMenuPreferences(wxCommandEvent&)
{
  preferencesDialog->Raise();
  preferencesDialog->Show(true);
};
void DiveAgentTaskBarIcon::OnMenuCheckForUpdates(wxCommandEvent&)
{
  
};
void DiveAgentTaskBarIcon::OnMenuAbout(wxCommandEvent&)
{
  aboutDilog->Raise();
  aboutDilog->Show(true);
};

// ----------------------------------------------------------------------------
// DiveAgentApp
// ----------------------------------------------------------------------------

IMPLEMENT_APP(DiveAgentApp)

DiveAgentApp::DiveAgentApp()
{
  m_taskBarIcon = 0;
  m_dockIcon = 0;
}

DiveAgentApp::~DiveAgentApp()
{
}

bool DiveAgentApp::OnInit()
{
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
  
  wxBitmap icon_bitmap = wxBITMAP_PNG_FROM_DATA(icon_bw);
  wxIcon icon;
  icon.CopyFromBitmap(icon_bitmap);
  if ( !m_taskBarIcon->SetIcon(icon,
                               "DiveAgent") )
  {
    wxLogError(wxT("Could not set icon."));
  }
  
  createDocIcon();
  createDialogs();
  return true;
}

int DiveAgentApp::OnExit()
{
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

