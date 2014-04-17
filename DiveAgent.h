#ifndef __DIVE_AGENT_HPP__
#define __DIVE_AGENT_HPP__
#include <boost/thread/thread.hpp>

class Computer;

class DiveAgent
{
public:
  ~DiveAgent();
  static DiveAgent& instance() { return _instance; }
  void  startUploadDives(const std::string &dive_computer_type_id, const std::string &port);
  void  cancelUploadDives();
  int   uploadDivesProgress();
  bool  isUploadDivesRuning();
  std::string getErrors();
  bool login_email(const std::string& email, const std::string& password);
  bool login_fb(const std::string& fbid, const std::string& token);
  std::string getLogedUser() const;
  void logoff();
  static std::string exeFolder();
  static void writeProfile(const std::string& key, const std::string& value);
  static std::string readProfile(const std::string& key);
protected:
  DiveAgent();
  void uploadDivesToServer();
  static std::string homeFolder();
  static std::string configFile() { return homeFolder() + ".dive_agent.cfg"; }
  boost::mutex  _m;
  boost::thread _th;
  int           _upload_dives_progress;
  bool          _upload_dives_running;
  Computer*     _dive_computer;
  std::string   _errors;
  std::string   _xml;
  static void workingThread();
  static DiveAgent _instance;
};

#endif//__DIVE_AGENT_HPP__