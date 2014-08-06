#include "stdafx.h"
#include "MainFrame.h"

MainFrame::MainFrame() : MainFrameBase(0)
{
  m_passwordText->SetHint("Password");
  m_emailText->SetHint("Email");
}

MainFrame::~MainFrame()
{
}

void MainFrame::OnLeftDown(wxMouseEvent& evt)
{
  CaptureMouse();
  wxPoint pos = ClientToScreen(evt.GetPosition());
  wxPoint origin = GetPosition();
  int dx =  pos.x - origin.x;
  int dy = pos.y - origin.y;
  m_delta = wxPoint(dx, dy);
}


void MainFrame::OnMouseMove(wxMouseEvent& evt)
{
  wxPoint pt = evt.GetPosition();
  if (evt.Dragging() && evt.LeftIsDown())
    {
      wxPoint pos = ClientToScreen(pt);
      Move(wxPoint(pos.x - m_delta.x, pos.y - m_delta.y));
    }
}

void MainFrame::OnLeftUp(wxMouseEvent& evt)
{
  if (HasCapture())
    {
      ReleaseMouse();
    }
}

