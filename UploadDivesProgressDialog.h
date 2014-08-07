#ifndef __UPLOAD_DIVES_PROGRESS_DIALOG_HPP__
#define __UPLOAD_DIVES_PROGRESS_DIALOG_HPP__
#include "dive_agent_forms.h"

#include <wx/timer.h>
class UploadDivesDialog;
class UploadDivesProgressDialog: public UploadDivesProgressDialogBase
{
public:  UploadDivesProgressDialog();
  void setMainDialog(UploadDivesDialog* d) { _main_dialog = d; }
  void enableMonitoring();
  void disableMonitoring(){ _monitoring = false;}
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
  UploadDivesDialog* _main_dialog;
  bool              _monitoring;
  bool              _wait_dive_xml;
  wxTimer*          _timer;
};

#endif
