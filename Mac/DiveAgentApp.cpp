#include "../DiveAgentApp.h"
#include "../sample.xpm"
#include <wx/icon.h>

void DiveAgentApp::createDocIcon()
{
  m_dockIcon = new DiveAgentTaskBarIcon(wxTBI_DOCK);
  if ( !m_dockIcon->SetIcon(wxICON(sample)) )
  {
    wxLogError(wxT("Could not set icon."));
  }
};

bool DiveAgentTaskBarIcon::haveQuitMenuFromSystem()
{
  return !OSXIsStatusItem();
};
