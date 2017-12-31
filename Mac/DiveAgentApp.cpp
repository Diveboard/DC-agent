#include "../DiveAgentApp.h"
#include "../icon_systrail.png.h"
#include <wx/icon.h>
#include <ApplicationServices/ApplicationServices.h>

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
