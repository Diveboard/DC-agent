#pragma once
#include "stdafx.h"
#include <string>
#include <string.h>

#ifdef WIN32
#define __THIS_FILE__ ((strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\'): (__FILE__ - 1)) + 1)
#else
#define __THIS_FILE__ ((strrchr(__FILE__, '/') ? strrchr(__FILE__, '/'): (__FILE__ - 1)) + 1)
#endif

#define LOGCRITICAL(...) Logger::appendL(__LINE__, __THIS_FILE__, "CRITICAL", __VA_ARGS__)
#define LOGERROR(...) Logger::appendL(__LINE__, __THIS_FILE__, "ERROR", __VA_ARGS__)
#define LOGWARNING(...) Logger::appendL(__LINE__, __THIS_FILE__, "WARNING", __VA_ARGS__)
#define LOGINFO(...) Logger::appendL(__LINE__, __THIS_FILE__, "INFO", __VA_ARGS__)
#define LOGDEBUG(...) Logger::appendL(__LINE__, __THIS_FILE__, "DEBUG", __VA_ARGS__)
#define LOGFILE(...) Logger::appendF(__LINE__, __THIS_FILE__, "DEBUG", __VA_ARGS__)


#define DBthrowError(...) Logger::addnthrow(__LINE__, __THIS_FILE__, "ERROR", __VA_ARGS__)


std::wstring s2ws(const std::string& s);
std::string ws2s(const std::wstring& s);

typedef struct {
	std::string type;
	std::string data;
} BinaryData;

class Logger
{
private:
	Logger(void);
	~Logger(void);
	
public:
	static std::string logLevel;
	static unsigned long logSize;
	static std::vector<std::string> logs;
	static std::vector<BinaryData> binData;
	static std::string toString();
	static std::string getBinary();
	static void append(const std::string&);
	static void append(const char *pstrFormat, ...);
	static void appendL(int line, const char*file, const char * level, std::string s);
	static void appendL(int line, const char*file, const char * level, const char *pstrFormat, ...);
	static void appendF(int line, const char*file, const char * level, std::string filename);
	static void addnthrow(int line, const char*file, const char * level, std::string s);
	static void addnthrow(int line, const char*file, const char * level, const char *pstrFormat, ...);
	static void binary(const std::string &type, unsigned char *data, unsigned int len);
	static void binary(const std::string &type, const std::string &data);
	static void setLogLevel(const std::string&);
	static bool checkLevel(const std::string &req);
	static void clear();
};


