#include "BaseScreen.h"

BaseScreen::BaseScreen( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, title, pos, size, style );
    wxImage::AddHandler( new wxPNGHandler );
 
    m_drawImage = m_bgImage.LoadFile( "forms/map_bg.png", wxBITMAP_TYPE_PNG );
 
    if (m_drawImage) {
        SetBackgroundBitmap(m_bgImage);
    }
 
}
 
BaseScreen::~BaseScreen()
{
}