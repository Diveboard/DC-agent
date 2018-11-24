#ifndef __STDAFX_H__
#define __STDAFX_H__

#if defined(WIN32) || defined(__MINGW32__)
#if !defined(WINVER)
#define WINVER 0x0502
#endif
#define _UNICODE
#define UNICODE
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

#endif //__STDAFX_H__
