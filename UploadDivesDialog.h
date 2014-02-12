#ifndef __UPLOAD_DIVES_DIALOG_HPP__
#define __UPLOAD_DIVES_DIALOG_HPP__
#include "dive_agent_forms.h"

#include <wx/timer.h>

class UploadDivesProgressDialog;
class UploadDivesDialog: public UploadDivesDialogBase
{
public:
  UploadDivesDialog();
  void setProgressDialog(UploadDivesProgressDialog* d) { _progress_dialog = d; }
protected:
  virtual void selectComputerComboOnCombobox( wxCommandEvent& event );
  virtual void selectPortComboOnCombobox( wxCommandEvent& event );
  virtual void uploadDivesButtonOnButtonClick( wxCommandEvent& event );
private:
  UploadDivesProgressDialog* _progress_dialog;
};

class UploadDivesProgressDialog: public UploadDivesProgressDialogBase
{
public:
  UploadDivesProgressDialog();
  void setMainDialog(UploadDivesDialog* d) { _main_dialog = d; }
  void enableMonitoring() { _monitoring = true; m_uploadProgressGauge->SetValue(0);}
  void disableMonitoring(){ _monitoring = false;}
protected:
  enum
  {
    timer_id = 100
  };
  virtual void actionButtonOnButtonClick( wxCommandEvent& event );
  void onTimer( wxTimerEvent& event);
  UploadDivesDialog* _main_dialog;
  bool              _monitoring;
  wxTimer*          _timer;
};

#endif//__UPLOAD_DIVES_DIALOG_HPP__