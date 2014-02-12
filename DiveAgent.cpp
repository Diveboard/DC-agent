#include "DiveAgent.h"
#include "ComputerFactory.h"

// ----------------------------------------------------------------------------
// DiveAgent
// ----------------------------------------------------------------------------
DiveAgent DiveAgent::_instance;

DiveAgent::DiveAgent():
  _dive_computer(0)
{
};

DiveAgent::~DiveAgent()
{
  if (isUploadDivesRuning())
    cancelUploadDives();
  if (_dive_computer)
    delete _dive_computer;
};

void DiveAgent::workingThread()
{
  try
  {
    {
      boost::lock_guard<boost::mutex> g(instance()._m);
      instance()._errors.resize(0);
      instance()._xml.resize(0);
      instance()._upload_dives_running = true;
    }
    std::string xml;
    instance()._dive_computer->get_all_dives(xml);
    if (instance()._dive_computer->get_status().state == COMPUTER_FINISHED)
    {
      boost::lock_guard<boost::mutex> g(instance()._m);
      instance()._xml = xml;
    }
    instance()._upload_dives_running = false;
  }
  catch( boost::thread_interrupted& )
  {
    instance()._upload_dives_running = false;
  }
  catch (std::exception& e)
  {
    boost::lock_guard<boost::mutex> g(instance()._m);
    instance()._errors += std::string(" ") + e.what();
    instance()._upload_dives_running = false;
  }
};

void  DiveAgent::startUploadDives(const std::string &dive_computer_type_id, const std::string &port)
{
  if (isUploadDivesRuning())
    cancelUploadDives();
  if (_dive_computer)
    delete _dive_computer;
  ComputerFactory f;
  _dive_computer = f.createComputer(dive_computer_type_id, port);
  _th = boost::thread(workingThread);
};

void  DiveAgent::cancelUploadDives()
{
  instance()._dive_computer->cancel();
  _th.join();
  _upload_dives_progress = 0;
}

int   DiveAgent::uploadDivesProgress()
{
  int res = 0;
  {
    boost::lock_guard<boost::mutex> g(instance()._m);
    if (instance()._dive_computer)
      res = instance()._dive_computer->get_status().percent;
  }
  return res;
};

bool  DiveAgent::isUploadDivesRuning()
{
  bool res = false;
  {
    boost::lock_guard<boost::mutex> g(instance()._m);
    res = _upload_dives_running;
  }
  return res;
};

std::string DiveAgent::getErrors()
{
  std::string res;
  {
    boost::lock_guard<boost::mutex> g(instance()._m);
    res = _errors;
  }
  return res;
};

