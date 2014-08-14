#include "../stdafx.h"
#include <string>
#include "../DiveAgent.h"
#include <vector>
#include "../Logger.h"
// #include <userenv.h>
// #include <wincrypt.h>
// #include <sstream>
// #include <iomanip>

namespace
{
  std::string service("DiveAgent");
}

std::string DiveAgent::homeFolder()
{
  // HANDLE h_proccess = GetCurrentProcess();
  // HANDLE h_token = 0;
  // OpenProcessToken(h_proccess, TOKEN_QUERY, &h_token);
  // TCHAR buffer[MAX_PATH];
  // DWORD sz = MAX_PATH - 1;
  // std::string res;
  // if (GetUserProfileDirectory(h_token, buffer, &sz))
  //   res = ws2s(buffer) + std::string("\\");
  // CloseHandle(h_token);
  // CloseHandle(h_proccess);
  // return res;
  return "";
};

std::string DiveAgent::exeFolder()
{
  // TCHAR buffer[MAX_PATH];
  // if (GetModuleFileName(NULL, buffer, MAX_PATH))
  // {
  //   std::string exe_name = ws2s(buffer);
  //   size_t i = exe_name.size() - 1;
  //   while (i != 0 && exe_name[i]!='\\' ) i--;
  //   return exe_name.substr(0, i);
  // }
  return "";
};
int err =0;
void DiveAgent::writeSecureProfile(const std::string& key, const std::string& value)
{
  // std::vector<char> buffer(value.begin(), value.end());
  // DATA_BLOB DataIn = {buffer.size(),(BYTE*)&buffer[0]};
  // DATA_BLOB DataOut;
  // if (CryptProtectData(&DataIn, s2ws(key).c_str(), 0, 0, 0,  0,  &DataOut))
  // {
  //   std::stringstream s;
  //   for (size_t i=0; i< DataOut.cbData; ++i)
  //     s << std::setw(2) << std::setfill('0') << std::hex << (unsigned)DataOut.pbData[i] << " ";
  //   writeProfile(key, s.str());
  //   LocalFree((HLOCAL)DataOut.pbData);
  // }
};

std::string DiveAgent::readSecureProfile(const std::string& key)
{
  // std::string value = readProfile(key);
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
  return std::string();
};
