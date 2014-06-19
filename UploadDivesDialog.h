#ifndef __UPLOAD_DIVES_DIALOG_HPP__
#define __UPLOAD_DIVES_DIALOG_HPP__
#include "dive_agent_forms.h"

#include <wx/timer.h>

class UploadDivesProgressDialog;
class PreferencesDialog;
class ComputerFactory;
class UploadDivesDialog: public UploadDivesDialogBase
{
public:
  UploadDivesDialog();
  virtual ~UploadDivesDialog();
  void setProgressDialog(UploadDivesProgressDialog* d) { _progress_dialog = d; }
  void setPreferencesDialog(PreferencesDialog* d) { _preferences_dialog = d; }
protected:
  virtual void selectMakeChoiceOnChoice       ( wxCommandEvent& event );
  virtual void uploadDivesButtonOnButtonClick ( wxCommandEvent& event );
private:
  enum
  {
    timer_id = 200,
    timer_timeout = 1000,
    timer_mx_count = 10
  };
  void onTimer( wxTimerEvent& event);
  UploadDivesProgressDialog* _progress_dialog;
  PreferencesDialog*         _preferences_dialog;
  bool                       _expect_port_selected_manualy;
  wxTimer*                   _timer;
  unsigned                   _timer_counter;
  ComputerFactory&           _f;
};

class UploadDivesProgressDialog: public UploadDivesProgressDialogBase
{
public:
  UploadDivesProgressDialog();
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
  virtual void openInBrowserButtonOnButtonClick( wxCommandEvent& event );
  virtual void doneButtonOnButtonClick( wxCommandEvent& event );
  void onTimer( wxTimerEvent& event);
  void hideProgressGauge();
  void showProgressGauge();
  UploadDivesDialog* _main_dialog;
  bool              _monitoring;
  bool              _wait_dive_xml;
  wxTimer*          _timer;
};

#endif//__UPLOAD_DIVES_DIALOG_HPP__
