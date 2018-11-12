#include <wx/icon.h>
#include <ApplicationServices/ApplicationServices.h>

#include "../../source/DiveAgentApp.h"
#include "../../source/icon_systrail.png.h"

#if wxOSX_USE_COCOA
  bool OSXIsStatusItem() { return true; }
#else
  bool OSXIsStatusItem() { return false; }
#endif

void DiveAgentApp::createDocIcon()
{
  m_dockIcon = new DiveAgentTaskBarIcon(this, wxTBI_DOCK);
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
