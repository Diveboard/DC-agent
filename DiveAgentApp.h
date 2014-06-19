#ifndef _DIVE_AGENT_APP_HPP_
#define _DIVE_AGENT_APP_HPP_

#include "wx/app.h"
#include "wx/taskbar.h"

// ----------------------------------------------------------------------------
// DiveAgentTaskBarIcon
// ----------------------------------------------------------------------------
class DiveAgentTaskBarIcon : public wxTaskBarIcon
{
public:
  DiveAgentTaskBarIcon(wxTaskBarIconType iconType = wxTBI_DEFAULT_TYPE): wxTaskBarIcon(iconType)
  {}
  
  void OnLeftButtonDClick(wxTaskBarIconEvent&);
  void OnMenuExit(wxCommandEvent&);
  
  void OnMenuUploadDives(wxCommandEvent&);
  void OnMenuPreferences(wxCommandEvent&);
  void OnMenuCheckForUpdates(wxCommandEvent&);
  void OnMenuAbout(wxCommandEvent&);
  
  virtual wxMenu *CreatePopupMenu();
  
  DECLARE_EVENT_TABLE()
protected:
  void SureProcessToForeground();
  bool haveQuitMenuFromSystem();
};

// Define a new application
class DiveAgentApp : public wxApp
{
public:
  DiveAgentApp();
  ~DiveAgentApp();
  virtual bool  OnInit();
  virtual int   OnExit();
protected:
  void createDocIcon();
  DiveAgentTaskBarIcon   *m_taskBarIcon;
  DiveAgentTaskBarIcon   *m_dockIcon;
};
class wxDialog;
void setCurrentDialog(wxDialog *d, bool show=true);
void reportError(const std::string&);

#endif//_DIVE_AGENT_APP_HPP