#ifndef __ABOUT_DIALOG_HPP__
#define __ABOUT_DIALOG_HPP__
#include "dive_agent_forms.h"
#include "Global.h"

class AboutDilog:public AboutDialogBase
{
public:
  AboutDilog():AboutDialogBase(0)
  {
  	m_staticText12->SetLabelText("Version " APPVERSION " build #" VERSION "\nCopyright © 2014 Diveboard SARL or its subsidiaries\n\nDiveboard Agent detect compatible dive computers connected to the\ncomputer and transfers available dive profiles to \n");
  };
};

#endif//__ABOUT_DIALOG_HPP__
