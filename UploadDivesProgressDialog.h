#ifndef __UPLOAD_DIVES_PROGRESS_DIALOG_HPP__
#define __UPLOAD_DIVES_PROGRESS_DIALOG_HPP__
#include "dive_agent_forms.h"

#include <wx/timer.h>
class MainFrame;
class UploadDivesProgressDialog: public UploadDivesProgressDialogBase
{
public:  UploadDivesProgressDialog();
  void enableMonitoring();
  void disableMonitoring(){ _monitoring = false;}
  void setMainFrame(MainFrame *m) {mainFrame = m;}
protected:
  enum
  {
    timer_id = 100,
    timer_timeout = 100
  };
  virtual void actionButtonOnButtonClick( wxCommandEvent& event );
  virtual void doneButtonOnButtonClick( wxCommandEvent& event );
  void onTimer( wxTimerEvent& event);
  void hideProgressGauge();
  void showProgressGauge();
  bool              _monitoring;
  bool              _wait_dive_xml;
  wxTimer*          _timer;
  MainFrame	    *mainFrame;
};

#endif
