#ifndef __MAIN_FRAME_HPP__
#define __MAIN_FRAME_HPP__
#include "dive_agent_forms.h"

class MainFrame : public MainFrameBase
{
public:
  MainFrame();
  ~MainFrame();
  void OnLeftDown( wxMouseEvent& event );
  void OnLeftUp( wxMouseEvent& event );
  void OnMouseMove( wxMouseEvent& event );
  void loginButtonOnButtonClick( wxCommandEvent& event );
  virtual void FBconnectButtonOnButtonClick ( wxCommandEvent& event );
private:
  void InitLoginPanel();
  void loadUploadDivesPanel();
  void showAccountInfo();
  wxPoint m_delta;

};
#endif//__MAIN_FRAME_HPP__
