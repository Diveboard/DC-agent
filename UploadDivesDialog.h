#ifndef __UPLOAD_DIVES_DIALOG_HPP__
#define __UPLOAD_DIVES_DIALOG_HPP__
#include "dive-agent-forms.h"
class UploadDivesDialog: public UploadDivesDialogBase
{
public:
  UploadDivesDialog(): UploadDivesDialogBase(0){}
};
class UploadDivesProgressDialog: public UploadDivesProgressDialogBase
{
public:
  UploadDivesProgressDialog():UploadDivesProgressDialogBase(0){};
};
#endif//__UPLOAD_DIVES_DIALOG_HPP__