#include "DiveAgent.h"
#include "ComputerFactory.h"
#include "Logger.h"
#include <curl/curl.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <fstream>

#include <libconfig.h++>
#include "ComputerMares.h"
#include "Logger.h"
#include <boost/date_time/posix_time/posix_time.hpp>

namespace {
  
  class ServerAPI
  {
    struct CurlInitializer
    {
      CurlInitializer()
      {
        if (curl_global_init(CURL_GLOBAL_ALL) !=0)
          Logger::append("Unable initialize Curl");
      }
      ~CurlInitializer()
      {
        curl_global_cleanup();
      }
    };
  public:
    ServerAPI():
      _curl(0),
      _req_header(0),
      _req_form(0),
      _follow_location(1)
    {
    };
    ~ServerAPI()
    {
      // free request header
      if (_req_header)
        curl_slist_free_all(_req_header);
      // free request form
      if (_req_form)
        curl_formfree(_req_form);
      // free curl handle
      if (_curl)
        curl_easy_cleanup(_curl);
    }
    void logoff()
    {
      _token.resize(0);
      _user.resize(0);
      _user_id.resize(0);
      _user_picture.resize(0);
      _expiration = boost::posix_time::ptime();
    };
    bool is_login_expired() const
    {
      return !(boost::posix_time::ptime(boost::posix_time::second_clock::universal_time()) < _expiration);
    }
    const std::string user() const { return _user;}
    const std::vector<char>& user_picture() const { return _user_picture; }
    void login_fb(const std::string fbid, const std::string& fbtoken)
    {
      std::map< std::string, std::string > param;
      param["fbid"]     = fbid;
      param["fbtoken"]  = fbtoken;
      param["apikey"]   = apiKey();
      shttp_post(apiBaseURL() + "login_fb", param);
      get_user_info();
    };
    void login_email( const std::string& email, const std::string& password)
    {
      std::map< std::string, std::string > param;
      param["email"]    = email;
      param["password"] = password;
      param["apikey"]   = apiKey();
      shttp_post(apiBaseURL() + "login_email", param);
      get_user_info();
    }
    std::string computerupload(const std::string& computer,
                               const std::string& xml,
                               const std::string& log)
    {
      std::map< std::string, std::string > param;
      param["user_id"] = _user_id;
      param["auth_token"] = _token;
      param["apikey"] = apiKey();
      param["xmlFormSend"] = xml;
      param["logFormSend"] = log;
      param["verFormSend"] = "DiveboardAgent v1.0.1";
      param["computer_model"] = computer;

      shttp_post(apiBaseURL() + "computerupload.json", param);
      rapidjson::Document d;
      d.Parse<0>(_resp_body.c_str());
      if (!d["success"].GetBool())
        throw std::string(d["message"].GetString());
      return d["completion_form_url"].GetString();;
    }
  private:
    void get_user_info()
    {
      rapidjson::Document d;
      d.Parse<0>(_resp_body.c_str());
      if (!d["success"].GetBool())
        throw std::string(d["message"].GetString());
      _token = d["token"].GetString();
      _user = d["user"]["nickname"].GetString();
      unsigned id = d["user"]["id"].GetUint();
      std::stringstream s;
      s <<  id;
      _user_id= s.str();

      shttp_get(d["user"]["picture"].GetString());
      _user_picture = std::vector<char>(_resp_body.begin(), _resp_body.end());
      
      boost::posix_time::time_input_facet* tif = new boost::posix_time::time_input_facet("%Y-%m-%dT%H:%M:%SZ");
      std::string time_str = d["expiration"].GetString();
      s.str(d["expiration"].GetString());
      s.imbue(std::locale(std::locale::classic(), tif));
      s >> _expiration;
    };
    std::string apiBaseURL() const {  return "https://stage.diveboard.com/api/"; }
    std::string apiKey()     const {  return "BVu3iqQKTeB7iI3T"; }
    void reinit_curl_handle()
    {
      if (_curl)
        curl_easy_cleanup(_curl);
      _curl = curl_easy_init();
      if (_curl == 0)
        Logger::append("Unable initialize Curl handle");
    };
    void reset_req_header()
    {
      if (_req_header)
        curl_slist_free_all(_req_header);
      _req_header = 0;
    }
    void reset_req_form()
    {
      if (_req_form)
        curl_formfree(_req_form);
      _req_form = 0;
      _req_form_last = 0;
    }
    void add_header_param(const std::string& name, const std::string& value)
    {
      _req_header = curl_slist_append(_req_header, (name + ": " + value).c_str());
    }
    void add_form_param(const std::string& name, const std::string& value)
    {
      curl_formadd(&_req_form,
                   &_req_form_last,
                   CURLFORM_COPYNAME, name.c_str(),
                   CURLFORM_COPYCONTENTS, value.c_str(),
                   CURLFORM_END);
    }
    void prepare_request(const std::string& url)
    {
      reinit_curl_handle();
      // debug mode
      curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1);
      // set url
      _url = url;
      if (curl_easy_setopt(_curl, CURLOPT_URL, _url.c_str()) != 0)
          throw std::string("Curl error: Unable to set url");
      // set responce callback
      if (curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, common_writer) != 0)
        throw std::string("Curl error: Unable to set writer callback");
      // set param
      if (curl_easy_setopt(_curl, CURLOPT_WRITEDATA, this) != 0)
        throw std::string("Curl error: Unable to set writer param");
      // set header callback
      if (curl_easy_setopt(_curl, CURLOPT_HEADERFUNCTION, common_header_writer) != 0)
        throw std::string("Curl error: Unable to set header writer callback");
      // set header callback param
      if (curl_easy_setopt(_curl, CURLOPT_WRITEHEADER, this) != 0)
        throw std::string("Curl error: Unable to set header writer param");
      // clear responce
      _resp_body.resize(0);
      _resp_header.resize(0);
      // set agent
      if (curl_easy_setopt(_curl, CURLOPT_USERAGENT, agent) != 0)
        throw std::string("Curl error: Unable to set agent");
      // set https options
      if (curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L) != 0)
        throw std::string("Curl error: Unable to set ssl option peer");
      if (curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 0L) != 0)
        throw std::string("Curl error: Unable to set ssl option host");
      // set not follow redirect
      if (curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, _follow_location) != 0)
        throw std::string("Curl error: Unable to set folow redirect option");
      // set cookie
      if (curl_easy_setopt(_curl, CURLOPT_COOKIEJAR, "/tmp/culr_cookie.txt") != 0)
        throw std::string("Curl error: Unable to set cookie option");
      if (curl_easy_setopt(_curl, CURLOPT_COOKIEFILE, "/tmp/culr_cookie.txt") != 0)
        throw std::string("Curl error: Unable to set cookie option");
      
    }
    int shttp_post(const std::string url, const std::map<std::string, std::string>& params)
    {
      prepare_request(url);
      reset_req_header();
      // prevent Expect 100-continue
      add_header_param("Expect", "");
      // set param
      if (curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, _req_header) != 0)
        throw std::string("Curl error: Unable to set http header for post");
      // prepare post
      if (curl_easy_setopt(_curl, CURLOPT_POST, 1L) != 0)
        throw std::string("Curl error: Unable to set post option");
      // prepare form data
      reset_req_form();
      for ( auto it = params.begin(); it!=params.end(); ++it)
        add_form_param(it->first, it->second);
      if (curl_easy_setopt(_curl, CURLOPT_HTTPPOST, _req_form) != 0)
        throw std::string("Curl error: Unable to set form data");
      // make request
      if (curl_easy_perform(_curl) != 0)
        throw std::string("Curl error: Unable to perform post");
      // get redirect
      _redirect_url.resize(0);
      char *redirect_url_ptr=0;
      if (curl_easy_getinfo(_curl, CURLINFO_REDIRECT_URL, &redirect_url_ptr) !=0)
        throw std::string("Unable to get redirect url");
      if (redirect_url_ptr)
        _redirect_url = redirect_url_ptr;
      // get responce code
      int res = 0;
      if (curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &res) != 0)
        throw std::string("Curl error: Unable to get responce code");
      
      reinit_curl_handle();
      return res;
    }
    
    int shttp_get(const std::string& url)
    {
      prepare_request(url);
      // make request
      if (curl_easy_perform(_curl) != 0)
        throw std::string("Curl error: Unable to perform post");
      // get responce code
      int res = 0;
      if (curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &res) != 0)
        throw std::string("Curl error: Unable to get responce code");
      
      reinit_curl_handle();
      return res;
    };
    size_t writer(void* data, size_t size, size_t nmemb)
    {
      size_t sz = size*nmemb;
      _resp_body += std::string((char*)data, sz);
      return sz;
    };
    size_t header_writer(void* data, size_t size, size_t nmemb)
    {
      size_t sz = size*nmemb;
      _resp_header += std::string((char*)data, sz);
      return sz;
    };
    
    CURL*           _curl;
    curl_slist*     _req_header;
    curl_httppost*  _req_form;
    curl_httppost*  _req_form_last;
    std::string     _url;
    std::string     _resp_header;
    std::string     _resp_body;
    std::string     _redirect_url;
    std::string     _token;
    std::string     _user;
    std::string     _user_id;
    std::vector<char> _user_picture;
    boost::posix_time::ptime  _expiration;
    long            _follow_location;
    
    static CurlInitializer  curl_initializer;
    static const char*      agent;
    static size_t common_writer(void* data, size_t size, size_t nmemb, void* instance);
    static size_t common_header_writer(void* data, size_t size, size_t nmemb, void* instance);
  };
  
  ServerAPI::CurlInitializer  ServerAPI::curl_initializer;
  const char* ServerAPI::agent = "DiveAgent";
  
  size_t ServerAPI::common_writer(void* data, size_t size, size_t nmemb, void* instance)
  {
    ServerAPI* api = reinterpret_cast<ServerAPI*>(instance);
    return api->writer(data, size, nmemb);
  };
  size_t ServerAPI::common_header_writer(void* data, size_t size, size_t nmemb, void* instance)
  {
    ServerAPI* api = reinterpret_cast<ServerAPI*>(instance);
    return api->header_writer(data, size, nmemb);
  };
  // api instance
  ServerAPI api;
  // config instance
  libconfig::Config conf;
}

// ----------------------------------------------------------------------------
// DiveAgent
// ----------------------------------------------------------------------------
DiveAgent DiveAgent::_instance;

std::string tmp;
DiveAgent::DiveAgent():
  _dive_computer(0)
{
  try
  {
    conf.readFile(configFile().c_str());
  }
  catch (libconfig::FileIOException&)
  {// todo:: report warning
  }
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
      instance()._completion_url.resize(0);
      instance()._upload_dives_running = true;
    }
    std::string xml;
    instance()._dive_computer->get_all_dives(xml);
    if (instance()._dive_computer->get_status().state == COMPUTER_FINISHED)
    {
      boost::lock_guard<boost::mutex> g(instance()._m);
      instance()._xml = xml;
    }
    // todo: make sure Logger is thread safe
    std::string log = Logger::toString();
    
    std::string completion_url = api.computerupload(instance()._dive_computer_type_id, xml, log);
    {
      boost::lock_guard<boost::mutex> g(instance()._m);
      instance()._completion_url = completion_url;
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
  catch (std::string& e)
  {
    boost::lock_guard<boost::mutex> g(instance()._m);
    instance()._errors += e;
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
  //_dive_computer = f.createComputer(dive_computer_type_id, port);
  //_dive_computer_type_id = dive_computer_type_id;
  _dive_computer = new ComputerMares(""); // test
  //_dive_computer_type_id =
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

void DiveAgent::uploadDivesToServer()
{
  
};

void DiveAgent::writeProfile(const std::string& key, const std::string& value)
{
  try
  {
    libconfig::Setting& root = conf.getRoot();
    if ( root.exists(key) )
      root[key] = value;
    else
      root.add(key, libconfig::Setting::TypeString) = value;
    conf.writeFile(configFile().c_str());
  }
  catch (libconfig::FileIOException& e)
  {
  }
  catch (libconfig::SettingNameException& )
  {
  }
  catch (libconfig::SettingTypeException&)
  {
  }
  catch (libconfig::SettingNotFoundException&)
  {
  }
  catch (libconfig::ParseException& e)
  {
  }
  
};

std::string DiveAgent::readProfile(const std::string& key)
{
  std::string res;
  try
  {
    conf.getRoot().lookupValue(key, res);
  }
  catch (libconfig::FileIOException&)
  {
    
  }
  return res;
};

bool DiveAgent::login_email(const std::string& email, const std::string& password)
{
  if (isUploadDivesRuning())
  {
    _errors += std::string(" The command is disabled while uploading dives is in progress");
    return false;
  }
  
  try
  {
    _errors.resize(0);
    api.login_email(email, password);
  }
  catch (std::string& e)
  {
    _errors = e;
    return false;
  }
  return true;
};

bool DiveAgent::login_fb(const std::string& fbid, const std::string& token)
{
  if (isUploadDivesRuning())
  {
    _errors += std::string(" The command is disabled while uploading dives is in progress");
    return false;
  }
  try
  {
    _errors.resize(0);
    api.login_fb(fbid, token);
  }
  catch (std::string& e)
  {
    _errors = e;
    return false;
  }
  return true;
};

std::string DiveAgent::getLogedUser() const
{
  return api.user();
};

void DiveAgent::logoff()
{
  api.logoff();
};

std::string DiveAgent::completionURL()
{
  std::string res;
  {
    boost::lock_guard<boost::mutex> g(instance()._m);
    res = _completion_url;
  }
  return res;
  
};

const std::vector<char>& DiveAgent::getLogedUserPicture()
{
  return api.user_picture();
};

bool  DiveAgent::isLoginExpired() const
{
  return api.is_login_expired();
};

