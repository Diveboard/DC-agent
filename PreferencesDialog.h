#ifndef __PREFERENCES_DIALOG_HPP__
#define __PREFERENCES_DIALOG_HPP__
#include "dive_agent_forms.h"

class PreferencesDialog:public PreferencesDialogBase
{
public:
  PreferencesDialog();
  virtual void FBconnectButtonOnButtonClick ( wxCommandEvent& event );
  virtual void loginButtonOnButtonClick     ( wxCommandEvent& event );
  virtual void unlinkButtonOnButtonClick    ( wxCommandEvent& event );
  virtual void unlinkButtonOnUpdateUI       ( wxUpdateUIEvent& event );
  virtual void loginButtonOnUpdateUI        ( wxUpdateUIEvent& event );
  virtual void FBconnectButtonOnUpdateUI    ( wxUpdateUIEvent& event );
protected:
  void showAccountInfo();
  void showSetAccount();
  
};

#endif//__PREFERENCES_DIALOG_HPP__