#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "../../source/stdafx.h"
#include "../../source/DiveAgent.h"
#include "../../source/Logger.h"

namespace
{
  std::string service("DiveAgent");
}

std::string DiveAgent::homeFolder()
{
  struct passwd *pw = getpwuid(getuid());
  const char *homedir = pw->pw_dir;
  return std::string(homedir) + "/";
};

std::string DiveAgent::exeFolder()
{
  return std::string(get_current_dir_name()) + "/";
};

int err =0;

void DiveAgent::writeSecureProfile(const std::string& key, const std::string& value)
{
  writeProfile(key, value);
  // std::vector<char> buffer(value.begin(), value.end());
  // DATA_BLOB DataIn = {buffer.size(),(BYTE*)&buffer[0]};
  // DATA_BLOB DataOut;
  // if (CryptProtectData(&DataIn, s2ws(key).c_str(), 0, 0, 0,  0,  &DataOut))
  // {
  //   std::stringstream s;
  //   for (size_t i=0; i< DataOut.cbData; ++i)
  //     s << std::setw(2) << std::setfill('0') << std::hex << (unsigned)DataOut.pbData[i] << " ";
  //   LocalFree((HLOCAL)DataOut.pbData);
  // }
};

std::string DiveAgent::readSecureProfile(const std::string& key)
{
  std::string value = readProfile(key);
  return value;
  // if (!value.empty())
  // {
  //   std::stringstream s(value);
  //   std::vector<BYTE> data;
  //   unsigned b;
  //   while (true)
  //   {
  //     s << std::setw(2) << std::setfill('0') << std::hex;
  //     s >> b;
  //     if (s)
  //       data.push_back(b);
  //     else
  //       break;
  //   }
  //   DATA_BLOB DataIn = {data.size(),(BYTE*)&data[0]};
  //   DATA_BLOB DataOut;
  //   TCHAR* Description = 0;
  //   if (CryptUnprotectData(&DataIn, &Description, 0, 0, 0,  0,  &DataOut))
  //   {
  //     std::string res = std::string((char *)DataOut.pbData, DataOut.cbData);
  //     LocalFree((HLOCAL)DataOut.pbData);
  //     LocalFree((HLOCAL)Description);
  //     return res;
  //   }
  // }
  //  return std::string();
};
