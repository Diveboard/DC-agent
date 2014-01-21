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
    #include "sample.xpm"
#endif

#include "smile.xpm"
#include "wx/taskbar.h"
#include "dive-agent.h"

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
// ----------------------------------------------------------------------------
// DiveAgentApp
// ----------------------------------------------------------------------------

IMPLEMENT_APP(DiveAgentApp)

DiveAgentApp::DiveAgentApp()
{
  m_taskBarIcon = 0;
#if defined(__WXCOCOA__)
  m_dockIcon = 0;
#endif
  
}

DiveAgentApp::~DiveAgentApp()
{
}

bool DiveAgentApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    if ( !wxTaskBarIcon::IsAvailable() )
    {
        wxMessageBox
        (
            "There appears to be no system tray support in your current environment. This sample may not behave as expected.",
            "Warning",
            wxOK | wxICON_EXCLAMATION
        );
    }


  m_taskBarIcon = new DiveAgentTaskBarIcon();
  
  // we should be able to show up to 128 characters on recent Windows versions
  // (and 64 on Win9x)
  if ( !m_taskBarIcon->SetIcon(wxICON(sample),
                               "wxTaskBarIcon Sample\n"
                               "With a very, very, very, very\n"
                               "long tooltip whose length is\n"
                               "greater than 64 characters.") )
  {
    wxLogError(wxT("Could not set icon."));
  }
  
#if defined(__WXOSX__) && wxOSX_USE_COCOA
  m_dockIcon = new DiveAgentTaskBarIcon(wxTBI_DOCK);
  if ( !m_dockIcon->SetIcon(wxICON(sample)) )
  {
    wxLogError(wxT("Could not set icon."));
  }
#endif

  createDialogs();
  return true;
}

int DiveAgentApp::OnExit()
{
  if (m_taskBarIcon)
  {
    delete m_taskBarIcon;
#if defined(__WXCOCOA__)
    delete m_dockIcon;
#endif
  }
  destroyDalogs();
  return wxApp::OnExit();
};

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
    EVT_MENU(PU_RESTORE, DiveAgentTaskBarIcon::OnMenuRestore)
    EVT_MENU(PU_EXIT,    DiveAgentTaskBarIcon::OnMenuExit)
    EVT_TASKBAR_LEFT_DCLICK  (DiveAgentTaskBarIcon::OnLeftButtonDClick)
    EVT_MENU(PU_UPLOAD_DIVES, DiveAgentTaskBarIcon::OnMenuUploadDives)
    EVT_MENU(PU_PREFERENCES, DiveAgentTaskBarIcon::OnMenuPreferences)
    EVT_MENU(PU_CHECK_FOR_UPDATES, DiveAgentTaskBarIcon::OnMenuCheckForUpdates)
    EVT_MENU(PU_ABOUT, DiveAgentTaskBarIcon::OnMenuAbout)
END_EVENT_TABLE()

void DiveAgentTaskBarIcon::OnMenuRestore(wxCommandEvent& )
{
  currentDialog->Show(true);
}

void DiveAgentTaskBarIcon::OnMenuExit(wxCommandEvent& )
{
  wxExit();
}

// Overridables
wxMenu *DiveAgentTaskBarIcon::CreatePopupMenu()
{
  wxMenu *menu = new wxMenu;
  menu->Append(PU_RESTORE, wxT("&Restore"));
  menu->AppendSeparator();
 
  menu->Append(PU_UPLOAD_DIVES, wxT("&Upload Dives"));
  menu->Append(PU_PREFERENCES, wxT("&Preferences"));
  menu->Append(PU_CHECK_FOR_UPDATES, wxT("&Check for updates"));
  menu->Append(PU_ABOUT, wxT("&About"));
  
    /* OSX has built-in quit menu for the dock menu, but not for the status item */
#ifdef __WXOSX__ 
    if ( OSXIsStatusItem() )
#endif
    {
        menu->AppendSeparator();
        menu->Append(PU_EXIT,    wxT("E&xit"));
    }
    return menu;
}

void DiveAgentTaskBarIcon::OnLeftButtonDClick(wxTaskBarIconEvent&)
{
  wxCommandEvent e;
  OnMenuRestore(e);
}

void DiveAgentTaskBarIcon::OnMenuUploadDives(wxCommandEvent&)
{
  currentDialog->Show(true);
};
void DiveAgentTaskBarIcon::OnMenuPreferences(wxCommandEvent&)
{
  preferencesDialog->Show(true);
};
void DiveAgentTaskBarIcon::OnMenuCheckForUpdates(wxCommandEvent&)
{
  
};
void DiveAgentTaskBarIcon::OnMenuAbout(wxCommandEvent&)
{
  aboutDilog->Show(true);
};

