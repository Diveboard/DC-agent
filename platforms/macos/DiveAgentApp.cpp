#include <wx/icon.h>
#include <ApplicationServices/ApplicationServices.h>

#include "../../source/DiveAgentApp.h"
#include "../../source/icon_systrail.png.h"

void DiveAgentApp::createDocIcon()
{
  m_dockIcon = new DiveAgentTaskBarIcon(wxTBI_DOCK);
  wxBitmap icon_bitmap = wxBITMAP_PNG_FROM_DATA(icon_systrail);
  wxIcon icon;
  icon.CopyFromBitmap(icon_bitmap);
  if ( !m_dockIcon->SetIcon(icon) )
  {
    wxLogError(wxT("Could not set icon."));
  }
}

bool DiveAgentApp::haveQuitMenuFromSystem()
{
  return !OSXIsStatusItem();
}
