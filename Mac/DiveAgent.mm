#import <Foundation/Foundation.h>
#import <Security/Security.h>
#include <string>
#include "../DiveAgent.h"
#include <vector>
namespace
{
  std::string service("DiveAgent");
}

std::string DiveAgent::homeFolder()
{
  return std::string([NSHomeDirectory() UTF8String]) + "/";
};

std::string DiveAgent::exeFolder()
{
  NSBundle* bundle = [NSBundle mainBundle];
  std::string res = [[bundle executablePath] UTF8String];
  size_t pos = res.size() - 1;
  while (res[pos] != '/' && pos != 0) --pos;
  return res.substr(0, pos + 1);
};

void DiveAgent::writeSecureProfile(const std::string& key, const std::string& value)
{
  SecKeychainAddGenericPassword(NULL,
                                service.size(),
                                service.c_str(),
                                key.size(),
                                key.c_str(),
                                value.size(),
                                value.c_str(),
                                NULL);
};

std::string DiveAgent::readSecureProfile(const std::string& key)
{
  UInt32 l = 0;
  void*  p = 0;
  OSStatus s = SecKeychainFindGenericPassword(NULL,
                                              (UInt32)service.size(),
                                              service.c_str(),
                                              (UInt32)key.size(),
                                              key.c_str(),
                                              &l,
                                              &p,
                                              NULL);
  return s == errSecSuccess && p ? std::string((char *)p, l) : std::string();
};
