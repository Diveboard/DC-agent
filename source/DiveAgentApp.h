#ifndef _DIVE_AGENT_APP_HPP_
#define _DIVE_AGENT_APP_HPP_

#include "wx/app.h"
#include "wx/taskbar.h"
#include <wx/timer.h>

class DiveAgentApp;

// ----------------------------------------------------------------------------
// DiveAgentTaskBarIcon
// ----------------------------------------------------------------------------
class DiveAgentTaskBarIcon : public wxTaskBarIcon
{
public:
  wxMenu *m_menu;
  DiveAgentTaskBarIcon(DiveAgentApp *app, wxTaskBarIconType iconType = wxTBI_DEFAULT_TYPE): wxTaskBarIcon(iconType) {
  	m_app = app;
  	m_menu = NULL;
  }
  
  void OnLeftButtonDClick(wxTaskBarIconEvent&);
  void OnMenuExit(wxCommandEvent&);
  
  void OnMenuUploadDives(wxCommandEvent&);
  void OnMenuLogout(wxCommandEvent&);
  void OnMenuLog(wxCommandEvent&);
  void OnMenuUpdate(wxCommandEvent&);
  void OnMenuAbout(wxCommandEvent&);
  
  virtual wxMenu *CreatePopupMenu();
  
  DECLARE_EVENT_TABLE()
protected:
	DiveAgentApp *m_app;
};
  enum
  {
    PU_RESTORE = 10001,
    PU_UPLOAD_DIVES,
    PU_LOGOUT,
    PU_ABOUT,
    PU_LOG,
    PU_UPDATE,
    PU_EXIT
  };

// Define a new application
class DiveAgentApp : public wxApp
{
public:
  DiveAgentApp();
  ~DiveAgentApp();
  virtual bool  OnInit();
  virtual int   OnExit();

  void SureProcessToForeground();
  bool haveQuitMenuFromSystem();
  DiveAgentTaskBarIcon   *m_taskBarIcon;

protected:
  enum
  {
    timer_id = 200,
    timer_timeout = 1000,
    timer_mx_count = 10
  };
  void onTimer( wxTimerEvent& event);
  wxTimer*                   _timer;
  bool			     _alreadyDetected = false;

  void InitStartOnLogin();
  void createDocIcon();
  DiveAgentTaskBarIcon   *m_dockIcon;
};
class wxDialog;
void setCurrentDialog(wxDialog *d, bool show=true);
void reportError(const std::string&);
void setIsLoginEnable(bool e);
void refreshLoggedInMenuButton();

#endif//_DIVE_AGENT_APP_HPP
