#ifndef __UPLOAD_DIVES_DIALOG_HPP__
#define __UPLOAD_DIVES_DIALOG_HPP__
#include "dive_agent_forms.h"
#include "UploadDivesProgressDialog.h"
#include <wx/timer.h>

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

#endif//__UPLOAD_DIVES_DIALOG_HPP__
