#ifndef __LOG_DIALOG_HPP__
#define __LOG_DIALOG_HPP__
#include "dive_agent_forms.h"

class LogDialog : public LogDialogBase
{
public:
  LogDialog();
  ~LogDialog();

 private:
  void OnClickSendLog(wxCommandEvent& event);
};

#endif
