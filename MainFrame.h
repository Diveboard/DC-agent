#ifndef __MAIN_FRAME_HPP__
#define __MAIN_FRAME_HPP__
#include "dive_agent_forms.h"
#include "UploadDivesProgressDialog.h"
#include <wx/timer.h>
#include "ComputerFactory.h"
#include "AboutDialog.h"
#include <boost/thread/thread.hpp>

class MainFrame : public MainDialogBase
{
public:
  MainFrame();
  ~MainFrame();
  void OnLeftDown( wxMouseEvent& event );
  void OnLeftUp( wxMouseEvent& event );
  void OnMouseMove( wxMouseEvent& event );
  void loginButtonOnButtonClick( wxCommandEvent& event );
  virtual void FBconnectButtonOnButtonClick ( wxCommandEvent& event );
  void onClose( wxCloseEvent& event );
  void setProgressDialog(UploadDivesProgressDialog* d) { _progress_dialog = d; }
  void InitLoginPanel();
  static ComputerFactory& getComputerFactory() {return _f;}
private:
  // Panel Login
  void loadUploadDivesPanel();
  void showAccountInfo();
  wxPoint m_delta;

  // Panel Upload Dive
  void InitUploadDivesPanel();
  void selectMakeChoiceOnChoice       ( wxCommandEvent& event );
  void uploadDivesButtonOnButtonClick(wxCommandEvent& event);
  enum
  {
    timer_id = 200,
    timer_timeout = 1000,
    timer_mx_count = 10
  };
  void onTimer( wxTimerEvent& event);
  bool                       _expect_port_selected_manualy;
  wxTimer*                   _timer;
  unsigned                   _timer_counter;
  UploadDivesProgressDialog* _progress_dialog;
  static ComputerFactory&    _f;

protected:
  static bool scanBT;
  boost::thread _th;
  static void workingThread();
};
#endif//__MAIN_FRAME_HPP__
