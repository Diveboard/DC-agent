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
  bool  isDivesXmlReady();
  std::string getErrors();
  bool login_email(const std::string& email, const std::string& password);
  bool login_fb(const std::string& fbid, const std::string& token);
  bool restore_login();
  std::string getLoggedInUser() const;
  bool  isLoginExpired() const;
  const std::vector<char> &getLoggedInUserPicture();
  const std::string getLoggedInUserId();
  void logoff();
  std::string check_update();
  void send_log(const std::map<std::string, std::string>& param);
  static std::string exeFolder();
  static void writeProfile(const std::string& key, const std::string& value);
  static std::string readProfile(const std::string& key);
  static void writeSecureProfile(const std::string& key, const std::string& value);
  static std::string readSecureProfile(const std::string& key);
  std::string completionURL();
  // used to delete dive computer instance on application exit before system call to static objects destructors;
  // It is becouse dive computer implementation is using static objects from Logger.cpp in its destructor
  void deleteDiveComputerInstance();
  static std::string AppName() { return "DiveboardAgent"; };
  std::string get_id_from_token(const std::string& token);
protected:
  DiveAgent();
  void uploadDivesToServer();
  static std::string homeFolder();
  static std::string configFile() { return homeFolder() + "." + AppName() + ".cfg"; }
  boost::mutex  _m;
  boost::thread _th;
  int           _upload_dives_progress;
  bool          _upload_dives_running;
  Computer*     _dive_computer;
  std::string   _dive_computer_type_id;
  std::string   _errors;
  std::string   _xml;
  std::string   _completion_url;
  static void workingThread();
  static DiveAgent _instance;
};

#endif//__DIVE_AGENT_HPP__
