#include "stdafx.h"
#include "ComputerFactory.h"

#ifdef _WIN32
#include <tchar.h>
#include <initguid.h>
#include <setupapi.h>
#include <ntdef.h>
#endif

#include "Logger.h"

#include "ComputerSuunto.h"
#include "ComputerMares.h"
#include "ComputerLibdc.h"

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <errno.h>

#if defined(__MACH__) || defined(__linux__)
#include <dirent.h>
#endif



ComputerFactory::ComputerFactory(void)
{
	std::vector<ComputerSupport> *support_list;
	std::vector<ComputerSupport>::iterator it1;
	std::vector<std::string>::iterator it2;

	supported.clear();

	support_list = ComputerLibdc::support();
  for ( it1=support_list->begin() ; it1 < support_list->end(); it1++ ){
  	supported[it1->make].push_back(*it1);
	  for ( it2=it1->ports.begin() ; it2 < it1->ports.end(); it2++ ){
			recognisedPorts[it1->key_code].push_back(*it2);
		}
  }

	support_list = ComputerSuunto::support();
  for ( it1=support_list->begin() ; it1 < support_list->end(); it1++ ){
  	supported[it1->make].push_back(*it1);
	  for ( it2=it1->ports.begin() ; it2 < it1->ports.end(); it2++ ){
			recognisedPorts[it1->key_code].push_back(*it2);
		}
  }

	support_list = ComputerMares::support();
  for ( it1=support_list->begin() ; it1 < support_list->end(); it1++ ){
  	supported[it1->make].push_back(*it1);
	  for ( it2=it1->ports.begin() ; it2 < it1->ports.end(); it2++ ){
			recognisedPorts[it1->key_code].push_back(*it2);
		}
  }


}


ComputerFactory::~ComputerFactory(void)
{
}


bool IsNumeric(WCHAR *pszString, BOOL bIgnoreColon)
{
  size_t nLen = wcslen(pszString);
  if (nLen == 0)
    return false;

  //What will be the return value from this function (assume the best)
  BOOL bNumeric = true;

  for (size_t i=0; i<nLen && bNumeric; i++)
  {
    bNumeric = (isdigit(static_cast<int>(pszString[i])) != 0);
    if (bIgnoreColon && (pszString[i] == ':'))
      bNumeric = true;
  }

  return bNumeric;
}


#ifdef _WIN32

typedef HKEY (__stdcall SETUPDIOPENDEVREGKEY)(HDEVINFO, PSP_DEVINFO_DATA, DWORD, DWORD, DWORD, REGSAM);
typedef BOOL (__stdcall SETUPDICLASSGUIDSFROMNAME)(LPCTSTR, LPGUID, DWORD, PDWORD);
typedef BOOL (__stdcall SETUPDIDESTROYDEVICEINFOLIST)(HDEVINFO);
typedef BOOL (__stdcall SETUPDIENUMDEVICEINFO)(HDEVINFO, DWORD, PSP_DEVINFO_DATA);
typedef HDEVINFO (__stdcall SETUPDIGETCLASSDEVS)(LPGUID, LPCTSTR, HWND, DWORD);
typedef BOOL (__stdcall SETUPDIGETDEVICEREGISTRYPROPERTY)(HDEVINFO, PSP_DEVINFO_DATA, DWORD, PDWORD, PBYTE, DWORD, PDWORD);

//This function gets a list of all COM ports
//////
//Copyright (c) 1998 - 2010 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)
//
//All rights reserved.
//
//Copyright / Usage Details:
//
//You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise)
//when your product is released in binary form. You are allowed to modify the source code in any way you want
//except you cannot modify the copyright details at the top of each module. If you want to distribute source
//code with your application, then you are only allowed to distribute versions released by the author. This is
//to maintain a single distribution point for the source code.
//
void UsingSetupAPI1(std::vector<std::string>& ports, std::vector<std::string>& friendlyNames)
{
  //Make sure we clear out any elements which may already be in the array(s)
	ports.clear();
  friendlyNames.clear();

  //Get the various function pointers we require from setupapi.dll
  HINSTANCE hSetupAPI = LoadLibrary(_T("SETUPAPI.DLL"));
  if (hSetupAPI == NULL)
  {
    // Retrieve the system error message for the last-error code
  	LOGINFO("Failed to load SetupAPI");
  	throw DBException(str(boost::format("Error loading SetupAPI - Error code : %d") % GetLastError()));

  }

  SETUPDIOPENDEVREGKEY* lpfnLPSETUPDIOPENDEVREGKEY = reinterpret_cast<SETUPDIOPENDEVREGKEY*>(GetProcAddress(hSetupAPI, "SetupDiOpenDevRegKey"));
  SETUPDIGETCLASSDEVS* lpfnSETUPDIGETCLASSDEVS = reinterpret_cast<SETUPDIGETCLASSDEVS*>(GetProcAddress(hSetupAPI, "SetupDiGetClassDevsA"));
  SETUPDIGETDEVICEREGISTRYPROPERTY* lpfnSETUPDIGETDEVICEREGISTRYPROPERTY = reinterpret_cast<SETUPDIGETDEVICEREGISTRYPROPERTY*>(GetProcAddress(hSetupAPI, "SetupDiGetDeviceRegistryPropertyA"));
  SETUPDIDESTROYDEVICEINFOLIST* lpfnSETUPDIDESTROYDEVICEINFOLIST = reinterpret_cast<SETUPDIDESTROYDEVICEINFOLIST*>(GetProcAddress(hSetupAPI, "SetupDiDestroyDeviceInfoList"));
  SETUPDIENUMDEVICEINFO* lpfnSETUPDIENUMDEVICEINFO = reinterpret_cast<SETUPDIENUMDEVICEINFO*>(GetProcAddress(hSetupAPI, "SetupDiEnumDeviceInfo"));

  if ((lpfnLPSETUPDIOPENDEVREGKEY == NULL) || (lpfnSETUPDIDESTROYDEVICEINFOLIST == NULL) ||
      (lpfnSETUPDIENUMDEVICEINFO == NULL) || (lpfnSETUPDIGETCLASSDEVS == NULL) || (lpfnSETUPDIGETDEVICEREGISTRYPROPERTY == NULL))
  {
	try {
		//Unload the setup dll
		FreeLibrary(hSetupAPI);
		SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
	} catch (...) {
		LOGINFO("Warning : Error unloading SetupAPI.dll");
	}

	throw DBException("Error getting pointers from SetupAPI");
  }

  //Now create a "device information set" which is required to enumerate all the ports
  GUID guid = GUID_DEVINTERFACE_COMPORT;
  HDEVINFO hDevInfoSet = lpfnSETUPDIGETCLASSDEVS(NULL, NULL, NULL, DIGCF_ALLCLASSES|DIGCF_PRESENT); //&guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
  if (hDevInfoSet == INVALID_HANDLE_VALUE)
  {
	  DWORD dwLastError;
	  try {
		dwLastError = GetLastError();
		//Unload the setup dll
		FreeLibrary(hSetupAPI);
		SetLastError(dwLastError);
	  } catch (...) {
		LOGINFO("Warning : Error unloading SetupAPI.dll");
	  }

	  throw DBException(str(boost::format("Error creating information set from SetupAPI - %d") % dwLastError ));
  }

  //Finally do the enumeration
  BOOL bMoreItems = true;
  int nIndex = 0;
  SP_DEVINFO_DATA devInfo;
  while (bMoreItems)
  {
    //Enumerate the current device
    devInfo.cbSize = sizeof(SP_DEVINFO_DATA);
    bMoreItems = lpfnSETUPDIENUMDEVICEINFO(hDevInfoSet, nIndex, &devInfo);
    if (bMoreItems)
    {
      //Did we find a serial port for this device
      BOOL bAdded = false;
      int nPort = 0;

      LOGDEBUG("Getting informations for device %d", nIndex);

      //Get the registry key which stores the ports settings
      HKEY hDeviceKey = lpfnLPSETUPDIOPENDEVREGKEY(hDevInfoSet, &devInfo, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);
      if (hDeviceKey != INVALID_HANDLE_VALUE)
      {
        //Read in the name of the port
        TCHAR szPortName[256];
        szPortName[0] = _T('\0');
        DWORD dwSize = sizeof(szPortName);
        DWORD dwType = 0;

		if ((RegQueryValueEx(hDeviceKey, _T("PortName"), NULL, &dwType, reinterpret_cast<LPBYTE>(szPortName), &dwSize) == ERROR_SUCCESS) && (dwType == REG_SZ))
        {
          //If it looks like "COMX" then
          //add it to the array which will be returned
          size_t nLen = _tcslen(szPortName);

          LOGDEBUG("Port is named : '%s'",ws2s(szPortName).c_str());
          if (nLen > 3)
          {
            if (std::wstring(szPortName).substr(0, 3) == std::wstring(L"COM") &&
                 isdigit(szPortName[3]))
            {
              //Work out the port number
              nPort = _ttoi(&(szPortName[3]));
              ports.push_back(str(boost::format("\\\\.\\COM%1%") % nPort));
              bAdded = true;
            }
          }
        }

        //Close the key now that we are finished with it
        RegCloseKey(hDeviceKey);
      }

      //If the port was a serial port, then also try to get its friendly name
      if (bAdded)
      {
        BYTE szFriendlyName[256];
        szFriendlyName[0] = _T('\0');
        DWORD dwSize = sizeof(szFriendlyName);
        DWORD dwType = 0;
        std::string friendlyName = std::string("");
        if (lpfnSETUPDIGETDEVICEREGISTRYPROPERTY(hDevInfoSet, &devInfo, SPDRP_DEVICEDESC, &dwType, szFriendlyName, dwSize, &dwSize) && (dwType == REG_SZ))
        {
			//todo fix unicode support....
          friendlyName = std::string((char *)szFriendlyName);
        }
        if (friendlyName.find("COM") == std::string::npos) {
        	friendlyName += str(boost::format(" (COM%1%)") % nPort);
        }
        friendlyNames.push_back(friendlyName);
        LOGDEBUG("Port is friendly-named : '%s'", friendlyNames[friendlyNames.size()-1].c_str());
        LOGDEBUG("Port is friendly-named : '%s'", friendlyNames[friendlyNames.size()-1].c_str());
      }
    }

    ++nIndex;
  }

  //Free up the "device information set" now that we are finished with it
  lpfnSETUPDIDESTROYDEVICEINFOLIST(hDevInfoSet);

  //Unload the setup dll
  FreeLibrary(hSetupAPI);

  //for (unsigned int i=0; i< ports.size();i++)
  //  LOGINFO(str(boost::format("COM port found : %s - %s") % ports[i] % friendlyNames[i]));
}

void ComputerFactory::listPorts(std::string &a)
{
	std::vector<std::string> ports;
	std::vector<std::string> friendlyNames;
	unsigned int i;

	a.clear();
	UsingSetupAPI1(ports, friendlyNames);

	for (i=0; i< ports.size();i++)
		a += str(boost::format("%d : %s\n") % ports[i] % friendlyNames[i]);
}


#endif

#if defined(__MACH__) || defined(__linux__)

void ListTTY(std::vector<std::string>& files, std::vector<std::string>& friendlyNames, bool scanBT = false)
{
	int r;
	DIR *dp;
	struct dirent *dirp;

	friendlyNames.clear();
	files.clear();

	std::vector<BluetoothDevice> *btdevice_list = ComputerLibdc::btscan(scanBT);
	if (btdevice_list) {
		std::vector<BluetoothDevice>::iterator it1;
		for ( it1=btdevice_list->begin() ; it1 < btdevice_list->end(); it1++ ){
			files.push_back(it1->address);
			friendlyNames.push_back(it1->name);
		}
	}

	/* TODO: may avoid searching by storing last bt-device to INI-File...
	files.push_back("00:80:25:4A:E5:17");
	friendlyNames.push_back("OSTC4-XYZ");
	//*/

	if((dp = opendir("/dev")) == NULL)
		throw DBException(str(boost::format("Error (%1%) while opening /dev") % errno));

	while ((dirp = readdir(dp)) != NULL) {
		//todo : a NULL can also mean an error....
#if defined(__MACH__)
		//LOGDEBUG(str(boost::format("Filename : %1% %2%")     zxcvbnbm,/.% dirp->d_name % ((int)strncmp("tty.usbserial-", dirp->d_name, 14))));
		if (!strncmp("tty.", dirp->d_name, 4)) {
#elif defined(__linux__)
		if (!strncmp("ttyS", dirp->d_name, 4) || !strncmp("ttyUSB", dirp->d_name, 6) || !strncmp("ttyACM", dirp->d_name, 6) || !strncmp("rfcomm", dirp->d_name, 6)) {
#endif
			files.push_back(str(boost::format("/dev/%1%") % dirp->d_name));
			friendlyNames.push_back(std::string(dirp->d_name));
		}
	}

	r = closedir(dp);
	if (r) LOGINFO("Warning - Error while closing dir in ListTTY");
}

#endif



//This functions goes through all COM ports and checks if one is a known computer
std::string ComputerFactory::detectConnectedDevice(const std::string &computerType)
{
	std::map <std::string, std::string> ret;
	std::vector<std::string> fileNames;
	std::vector<std::string> friendlyNames;
	std::string driverName;
	unsigned int i;

	LOGINFO("Checking for interesting ports for %s", computerType.c_str());

	//1 list ports
#ifdef _WIN32
	LOGINFO("Using SetupAPI");
	UsingSetupAPI1(fileNames, friendlyNames);
#elif defined(__MACH__) || defined(__linux__)
	ListTTY(fileNames, friendlyNames);
#else
#error "Platform not supported"
#endif

	//2 filter interesting ones
	for (i=0; i< fileNames.size();i++) {
		LOGINFO("Checking port %s - %s", fileNames[i].c_str(), friendlyNames[i].c_str());

		for (unsigned int j=0; j<recognisedPorts[computerType].size(); j++)
		{
			LOGDEBUG("Comparing '%s' with '%s' : %d", friendlyNames[i].c_str(), recognisedPorts[computerType][j].c_str(),recognisedPorts[computerType][j].compare(friendlyNames[i]));
			if (!recognisedPorts[computerType][j].compare(NO_PORT_NEEDED))
					return(NO_PORT_NEEDED);
			if (!recognisedPorts[computerType][j].compare(friendlyNames[i]))
					return(fileNames[i]);
		}
	}

	LOGDEBUG("Checking if a computer without port is ok...");

	//OK no real port found... but maybe we don't need one ?
	for (unsigned int j=0; j<recognisedPorts[computerType].size(); j++)
	{
		LOGDEBUG("Checking '%s'", recognisedPorts[computerType][j].c_str());
		if (!recognisedPorts[computerType][j].compare(NO_PORT_NEEDED))
				return(NO_PORT_NEEDED);
	}


	LOGINFO("No interesting port found !");
	DBthrowError("Not found");
	return("");
}

std::map <std::string, std::string> ComputerFactory::allPorts(bool scanBT /*= false*/)
{
	std::vector<std::string> fileNames;
	std::vector<std::string> friendlyNames;
	std::map <std::string, std::string> ports;

	LOGINFO("Getting all ports");

#ifdef _WIN32
	LOGINFO("Using SetupAPI");
	UsingSetupAPI1(fileNames, friendlyNames);
#elif defined(__MACH__) || defined(__linux__)
	ListTTY(fileNames, friendlyNames, scanBT);
#else
#error "Platform not supported"
#endif

	for (int i=0; i < fileNames.size(); i++)
		ports[fileNames[i]] = friendlyNames[i];

	return(ports);
}

bool ComputerFactory::isComputerPluggedin()
{
	std::vector<std::string> fileNames;
	std::vector<std::string> friendlyNames;

	LOGINFO("Autodetect : Checking for potential known ports");

	//1 list ports
#ifdef _WIN32
	UsingSetupAPI1(fileNames, friendlyNames);
#elif defined(__MACH__) || defined(__linux__)
	ListTTY(fileNames, friendlyNames);
#else
#error "Platform not supported"
#endif

	//2 filter interesting ones
	for (unsigned int i=0; i< fileNames.size();i++) {
		//LOGINFO("Checking port %s - %s", fileNames[i].c_str(), friendlyNames[i].c_str());

		std::map <std::string, std::vector<std::string> >::const_iterator end = recognisedPorts.end();

		for (std::map <std::string, std::vector<std::string> >::const_iterator it = recognisedPorts.begin(); it != end; ++it)
			for (unsigned int j=0; j<it->second.size(); j++)
			{
				//LOGDEBUG("Comparing '%s' with '%s' : %d", friendlyNames[i].c_str(), it->second[j].c_str(),it->second[j].compare(friendlyNames[i]));
				//do not autodetect computers not needing any port
				//if (!it->second[j].compare(NO_PORT_NEEDED))
				//		return(NO_PORT_NEEDED);
				if (!it->second[j].compare(friendlyNames[i]))
				{
					LOGINFO("Autodetect : Found something : '%s' on '%s'", friendlyNames[i].c_str(), fileNames[i].c_str());
					return(true);
				}
			}
	}

	LOGINFO("Autodetect : No interesting port found !");
	return(false);

}

Computer *ComputerFactory::createComputer(const std::string &type, const std::string &filename)
{
	if (!type.compare("SUUNTO")){
		return new ComputerSuunto(filename);
	}
	else if (!type.compare("SUUNTO EMU")){
		return new ComputerSuunto("");
	}
	else if (!type.compare("MARES")){
		return new ComputerMares(filename);
	}
	else if (!type.compare("MARES EMU")){
		return new ComputerMares("");
	}
	else if (!type.compare(0,4,"LDC ")){
		return new ComputerLibdc(type, filename);
	}
	else throw DBException(std::string("ComputerFactory::createComputer : Computer requested unknown : ")+type);
}

