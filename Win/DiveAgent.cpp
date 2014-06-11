#include <string>
#include "../DiveAgent.h"
#include <vector>
namespace
{
  std::string service("DiveAgent");
}

std::string DiveAgent::homeFolder()
{
  // stub implemetation
  return "C:\\Users\\andrew";
};

std::string DiveAgent::exeFolder()
{
  return "";
};

void DiveAgent::writeSecureProfile(const std::string& key, const std::string& value)
{
};

std::string DiveAgent::readSecureProfile(const std::string& key)
{
  return std::string();
};
