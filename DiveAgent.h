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
protected:
  DiveAgent();
  
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