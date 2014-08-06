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

private:
  wxPoint m_delta;

};
#endif//__MAIN_FRAME_HPP__
