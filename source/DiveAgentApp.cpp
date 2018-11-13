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
#include "LogDialog.h"
#include <wx/snglinst.h>

#ifdef INDICATOR_ENABLED
#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>

GtkWidget *g_indicator_item_logout = NULL;

static GtkWidget* addMenuEntry (GtkWidget *menu, const char *label, GCallback callback, DiveAgentApp *app) {
  GtkWidget *indicator_item = gtk_menu_item_new_with_label (label);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), indicator_item);
  g_signal_connect (indicator_item, "activate", G_CALLBACK (callback), app);
  return indicator_item;
}
#endif
 
namespace {
  UploadDivesProgressDialog*  uploadDivesProgressDialog=0;
  AboutDilog*                 aboutDialog=0;
  MainFrame*                  mainFrame=0;
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

void refreshLoggedInMenuButton()
{
#ifdef INDICATOR_ENABLED
  gtk_menu_item_set_label((GtkMenuItem*)g_indicator_item_logout, "Logout");
#endif
}


static void doUploadDives(DiveAgentApp *app) {
  currentDialog->Raise();
  currentDialog->Show();
  app->SureProcessToForeground();
}

static void doAbout(DiveAgentApp *app) {
  aboutDialog->Raise();
  aboutDialog->Show();
  app->SureProcessToForeground();
}

static void doLogInOut(DiveAgentApp *app) {
  if (DiveAgent::instance().restore_login() && !DiveAgent::instance().getLoggedInUser().empty() && !DiveAgent::instance().isLoginExpired()) {
    DiveAgent::instance().logoff();
    mainFrame->InitLoginPanel();
    if (app->m_taskBarIcon && app->m_taskBarIcon->m_menu) {
      app->m_taskBarIcon->m_menu->SetLabel(PU_LOGOUT, wxT("Login"));
    }
#ifdef INDICATOR_ENABLED
    gtk_menu_item_set_label((GtkMenuItem*)g_indicator_item_logout, "Login");
#endif
  }
  else {
    doUploadDives(app);
  }
}

static void doLog(DiveAgentApp *app) {
  LogDialog *d = new LogDialog();

  d->Show();
  std::cout << Logger::toString() << std::endl;
}

static void doUpdate(DiveAgentApp *app) {
  std::string url = DiveAgent::instance().check_update();
  if (url.empty()){
    wxMessageDialog *dial = new wxMessageDialog(NULL,
                wxT("You already have the most recent agent version :)"), wxT("Update"),
                wxOK | wxICON_QUESTION);
    dial->ShowModal();
    dial->Destroy();
    return;
  }
  wxMessageDialog *dial = new wxMessageDialog(NULL,
                wxT("A new version can be downloaded, do you want to download it?"), wxT("Question"),
                wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

  int ret = dial->ShowModal();
  dial->Destroy();

  if (ret == wxID_YES) {
    wxLaunchDefaultBrowser(wxString::FromUTF8(url.c_str()));
  }
  std::cout << "check for update" << url << std::endl;
}

#ifdef INDICATOR_ENABLED
static void IndicatorOnUploadMenuItemSelected (GtkWidget *widget, DiveAgentApp *app) { doUploadDives(app); }
static void IndicatorOnAboutMenuItemSelected (GtkWidget *widget, DiveAgentApp *app) { doAbout(app); }
static void IndicatorOnLogoutMenuItemSelected (GtkWidget *widget, DiveAgentApp *app) { doLogInOut(app); }
static void IndicatorOnSendLogMenuItemSelected (GtkWidget *widget, DiveAgentApp *app) { doLog(app); }
static void IndicatorOnUpdateMenuItemSelected (GtkWidget *widget, DiveAgentApp *app) { doUpdate(app); }
static void IndicatorOnExitMenuItemSelected (GtkWidget *widget, DiveAgentApp *app) { wxExit(); }
#endif

// ----------------------------------------------------------------------------
// DiveAgentTaskBarIcon implementation
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(DiveAgentTaskBarIcon, wxTaskBarIcon)
EVT_MENU(PU_UPLOAD_DIVES,    DiveAgentTaskBarIcon::OnMenuUploadDives)
EVT_MENU(PU_ABOUT,    DiveAgentTaskBarIcon::OnMenuAbout)
EVT_MENU(PU_LOGOUT,    DiveAgentTaskBarIcon::OnMenuLogout)
EVT_MENU(PU_LOG,    DiveAgentTaskBarIcon::OnMenuLog)
EVT_MENU(PU_UPDATE,    DiveAgentTaskBarIcon::OnMenuUpdate)
EVT_MENU(PU_EXIT,    DiveAgentTaskBarIcon::OnMenuExit)
EVT_TASKBAR_LEFT_DOWN  (DiveAgentTaskBarIcon::OnLeftButtonDClick)
END_EVENT_TABLE()

void DiveAgentTaskBarIcon::OnMenuLogout(wxCommandEvent& e)
{
  doLogInOut(m_app);
}
void DiveAgentTaskBarIcon::OnMenuAbout(wxCommandEvent& )
{
  doAbout(m_app);
}
void DiveAgentTaskBarIcon::OnMenuExit(wxCommandEvent& )
{
  wxExit();
}
void DiveAgentTaskBarIcon::OnMenuLog(wxCommandEvent& )
{
  doLog(m_app);
}
void DiveAgentTaskBarIcon::OnMenuUpdate(wxCommandEvent& )
{
  doUpdate(m_app);
}

void DiveAgentTaskBarIcon::OnMenuUploadDives(wxCommandEvent&)
{
  doUploadDives(m_app);
}

// Overridables
wxMenu *DiveAgentTaskBarIcon::CreatePopupMenu()
{
  m_menu = new wxMenu();
  if ( !m_app->haveQuitMenuFromSystem() )
  {
    m_menu->Append(PU_UPLOAD_DIVES,    wxT("Upload Dives"));
    if (DiveAgent::instance().restore_login() && !DiveAgent::instance().getLoggedInUser().empty() && !DiveAgent::instance().isLoginExpired())
      m_menu->Append(PU_LOGOUT,    wxT("Logout"));
    else
      m_menu->Append(PU_LOGOUT,    wxT("Login"));
    m_menu->Append(PU_ABOUT,    wxT("About"));
    m_menu->Append(PU_LOG,    wxT("Send Log"));
    m_menu->Append(PU_UPDATE,    wxT("Check for update"));
    m_menu->Append(PU_EXIT,    wxT("Exit"));
  }
  m_menu->Enable(PU_LOGOUT, isLoginEnable);
  return m_menu;
}

void DiveAgentTaskBarIcon::OnLeftButtonDClick(wxTaskBarIconEvent&)
{
  wxCommandEvent e;
  OnMenuUploadDives(e);
  m_app->SureProcessToForeground();
}

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
            "There appears to be no system tray support in your current environment. This program may not behave as expected.",
            "Warning",
            wxOK | wxICON_EXCLAMATION
        );
  }

  m_taskBarIcon = new DiveAgentTaskBarIcon(this);
  wxBitmap icon_bitmap = wxBITMAP_PNG_FROM_DATA(icon_systrail);
  wxIcon icon;
  icon.CopyFromBitmap(icon_bitmap);
  if ( !m_taskBarIcon->SetIcon(icon, "DiveAgent") )
  {
    wxLogError(wxT("Could not set icon."));
  }

#ifdef INDICATOR_ENABLED
  AppIndicator *indicator = app_indicator_new("diveboard-appindicator", "diveboard", APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
  app_indicator_set_icon_full(indicator, "diveboard", "DiveboardAgent Systray Icon");
  app_indicator_set_title (indicator, "DiveboardAgent");
  app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);

  GtkWidget *menu = gtk_menu_new();
  addMenuEntry(menu, "Upload Dives", G_CALLBACK (IndicatorOnUploadMenuItemSelected), this);
  const char *labelText = (DiveAgent::instance().restore_login() && !DiveAgent::instance().getLoggedInUser().empty() && !DiveAgent::instance().isLoginExpired()) ? "Logout" : "Login";
  g_indicator_item_logout = addMenuEntry(menu, labelText, G_CALLBACK (IndicatorOnLogoutMenuItemSelected), this);
  addMenuEntry(menu, "About", G_CALLBACK (IndicatorOnAboutMenuItemSelected), this);
  addMenuEntry(menu, "Send Log", G_CALLBACK (IndicatorOnSendLogMenuItemSelected), this);
  addMenuEntry(menu, "Check for update", G_CALLBACK (IndicatorOnUpdateMenuItemSelected), this);
  addMenuEntry(menu, "Exit", G_CALLBACK (IndicatorOnExitMenuItemSelected), this);
  gtk_widget_show_all(menu);
  gtk_widget_set_sensitive(g_indicator_item_logout, isLoginEnable);

  app_indicator_set_menu (indicator, GTK_MENU (menu));
#endif

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
    if (DiveAgent::instance().isUploadDivesRuning()) return;

    //wxLogError("onTimerDiveAgentApp");
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

