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
  OSStatus s = SecKeychainAddGenericPassword(NULL,
                                             service.size(),
                                             service.c_str(),
                                             key.size(),
                                             key.c_str(),
                                             value.size(),
                                             value.c_str(),
                                             NULL);
  if (s == errSecDuplicateItem)
  {
    SecKeychainItemRef item;
    UInt32 l = 0;
    void*  p = 0;
    s = SecKeychainFindGenericPassword(NULL,
                                                (UInt32)service.size(),
                                                service.c_str(),
                                                (UInt32)key.size(),
                                                key.c_str(),
                                                &l,
                                                &p,
                                                &item);
    if ( s == errSecSuccess)
    {
      SecKeychainItemModifyAttributesAndData(item, NULL, value.size(), value.c_str());
      CFRelease(item);
    }
  }
  
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
