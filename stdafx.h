//#define _WIN32_WINNT 0x601

#if defined(WIN32) || defined(__MINGW32__)
#define WINVER 0x0502
#include <windows.h>
#endif

#include <stdio.h>


#include <string>
#include <vector>
#include <sstream>

#include <boost/format.hpp>

//for Mac
//#define			 WCHAR char;
//#define			 BOOL char;

#if defined(__MACH__) || defined(__linux__)
typedef unsigned int UINT;
typedef char BOOL;
typedef wchar_t WCHAR;
typedef unsigned int DWORD;
typedef const char * LPCTSTR;
typedef void * LPVOID;
typedef char * TCHAR;
typedef int HANDLE;
#endif


#include "DBException.h"


#define TRACE __noop
