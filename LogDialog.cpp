#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <fstream>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include "DiveAgent.h"
#include "LogDialog.h"
#include "Logger.h"
#include "base64.h"

LogDialog::LogDialog() : LogDialogBase(0)
{
  std::string email_value = DiveAgent::readProfile("login_email");
  if ( !email_value.empty() )
  {
    m_email->ChangeValue(wxString::FromUTF8(email_value.c_str()));
  }
}

LogDialog::~LogDialog() {}

void LogDialog::OnClickSendLog( wxCommandEvent& event )
{
  if (m_email->GetValue().empty())
    {
      wxString msg = wxString::FromUTF8("Please specify an email");
      wxMessageDialog* dlg = new wxMessageDialog(this, msg, wxString::FromUTF8("Dive agent"));
      dlg->ShowModal();
      dlg->Destroy();
      return;
    }
  if (m_subject->GetValue().empty())
    {
      wxString msg = wxString::FromUTF8("Please specify a subject");
      wxMessageDialog* dlg = new wxMessageDialog(this, msg, wxString::FromUTF8("Dive agent"));
      dlg->ShowModal();
      dlg->Destroy();
      return;
    }
  if (m_message->GetValue().length() < 10)
    {
      wxString msg = wxString::FromUTF8("Please specify a message or type a more longer message");
      wxMessageDialog* dlg = new wxMessageDialog(this, msg, wxString::FromUTF8("Dive agent"));
      dlg->ShowModal();
      dlg->Destroy();
      return;
    }


  std::map< std::string, std::string > param;
  param["email"] = m_email->GetValue();
  param["ticket[subject]"] = m_subject->GetValue();
  param["ticket[message]"] = m_message->GetValue() + "\n User_id = " + DiveAgent::instance().getLogedUserId();
  param["ticket[attachments][][name]"] = "log.txt";
  std::string log  = Logger::toString();
  std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(log.c_str()), log.length());
  param["ticket[attachments][][data]"] = encoded;
  param["ticket[attachments][][content_type]"] = "text/plain";



  // rapidjson::StringBuffer strbuf;
  // rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
  // writer.StartObject();

  // writer.String("email");
  // writer.String(m_email->GetValue().c_str());
  // writer.String("ticket");
  // writer.StartObject();
  // writer.String("subject");
  // writer.String(m_subject->GetValue().c_str());
  // writer.String("message");
  // writer.String(m_message->GetValue().c_str());

  // writer.String("attachments");
  // writer.StartArray();
  // writer.StartObject();

  // writer.String("name");
  // writer.String("log.txt");
  // writer.String("data");
  // std::string log  = Logger::toString();
  // std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(log.c_str()), log.length());
  // writer.String(encoded.c_str());
  // writer.String("content_type");
  // writer.String("text/plain");
  // writer.EndObject();
  // writer.EndArray();
  // writer.EndObject();
  // writer.EndObject();
  DiveAgent::instance().send_log(param);
  // std::cout << strbuf.GetString() << std::endl;
}
