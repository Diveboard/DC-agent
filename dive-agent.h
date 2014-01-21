#ifndef _DIVE_AGENT_HPP_
#define _DIVE_AGENT_HPP_

class DiveAgentTaskBarIcon : public wxTaskBarIcon
{
public:
#if defined(__WXOSX__) && wxOSX_USE_COCOA
    DiveAgentTaskBarIcon(wxTaskBarIconType iconType = wxTBI_DEFAULT_TYPE)
    :   wxTaskBarIcon(iconType)
#else
    DiveAgentTaskBarIcon()
#endif
    {}

    void OnLeftButtonDClick(wxTaskBarIconEvent&);
    void OnMenuRestore(wxCommandEvent&);
    void OnMenuExit(wxCommandEvent&);

    void OnMenuUploadDives(wxCommandEvent&);
    void OnMenuPreferences(wxCommandEvent&);
    void OnMenuCheckForUpdates(wxCommandEvent&);
    void OnMenuAbout(wxCommandEvent&);
 
    virtual wxMenu *CreatePopupMenu();

    DECLARE_EVENT_TABLE()
protected:
  
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
  DiveAgentTaskBarIcon   *m_taskBarIcon;
#if defined(__WXOSX__) && wxOSX_USE_COCOA
  DiveAgentTaskBarIcon   *m_dockIcon;
#endif
};

#endif//_DIVE_AGENT_HPP_