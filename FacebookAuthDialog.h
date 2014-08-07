#ifndef __FACEBOOK_AUTH_DIALOG_HPP__
#define __FACEBOOK_AUTH_DIALOG_HPP__
#include "dive_agent_forms.h"
#include <wx/webview.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <fstream>

class FacbookAuthDialog : public wxDialog
{
 private:
  
 protected:
  wxWebView* m_WebView;
  
 public:
  
  FacbookAuthDialog(wxWindow* parent,
		    wxWindowID id = wxID_ANY,
		    const wxString& title = wxT("Facebook Authorization"),
		    const wxPoint& pos = wxDefaultPosition,
		    const wxSize& size = wxDefaultSize,
		    long style = wxDEFAULT_DIALOG_STYLE );
  ~FacbookAuthDialog();
  bool IsError() const { return  _error; }
  const std::string Token() const { return _token; }
  const std::string Id() const { return _fbid; }
 protected:
  void OnLoaded(wxWebViewEvent& event);
  void OnNaviagting(wxWebViewEvent& event);
  void OnError(wxWebViewEvent&);
  void OnInitDialog(wxInitDialogEvent& event);
  bool        _error;
  std::string _token;
  std::string _fbid;
  int i;
};

#endif
