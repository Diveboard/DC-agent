#include "../DiveAgentApp.h"
#include <wx/bitmap.h>
#include "../icon_bw.xpm"
#include <wx/icon.h>

void DiveAgentApp::createDocIcon()
{
  m_dockIcon = new DiveAgentTaskBarIcon(wxTBI_DOCK);
  wxBitmap icon_bitmap = wxBITMAP_PNG_FROM_DATA(icon_bw);
  wxIcon icon;
  icon.CopyFromBitmap(icon_bitmap);
  if ( !m_dockIcon->SetIcon(icon) )
  {
    wxLogError(wxT("Could not set icon."));
  }
};

bool DiveAgentTaskBarIcon::haveQuitMenuFromSystem()
{
  return false;
};
