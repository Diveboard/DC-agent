#ifndef BASE_SCREEN_H
#define BASE_SCREEN_H
 
#include <wx/wx.h>
#include <wx/custombgwin.h>
 
class BaseScreen : public wxCustomBackgroundWindow<wxFrame>
{
   private:
 
   protected:
      wxBitmap m_bgImage;
      bool m_drawImage;
 
   public:
 	  BaseScreen();
      BaseScreen( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL );
      virtual ~BaseScreen();
 
};
 
#endif //BASE_SCREEN_H