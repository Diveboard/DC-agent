#import <Foundation/Foundation.h>
#include <string>
#include "../DiveAgent.h"

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