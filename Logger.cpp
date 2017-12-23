#include "Logger.h"
#include <boost/format.hpp>
#include <boost/thread/thread.hpp>

#include <cstdio>

#ifdef __linux__
#include <stdarg.h>
#endif

namespace {
  boost::mutex  _m;
};

using boost::format;

unsigned long Logger::logSize = 1500000;

//allocate a static buffer which is huge enough to fit even ostc4 debug logs
static char buff[16384];

std::wstring s2ws(const std::string& s)
{
#ifdef WIN32
 int len;
 int slength = (int)s.length() + 1;
 len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
 wchar_t* buf = new wchar_t[len];
 MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
 std::wstring r(buf);
 delete[] buf;
 return r;
#else

	std::wstring ws(s.length(), L' ');
	std::copy(s.begin(),s.end(), ws.begin());
	return(ws);
#endif
}

std::string ws2s(const std::wstring& s)
{
#ifdef WIN32
 int len;
 int slength = (int)s.length() + 1;
 len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, NULL, NULL);
 char* buf = new char[len];
 WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, NULL, NULL);
 std::string r(buf);
 delete[] buf;
 return r;
#else

	std::string r(s.length(), L' ');
	std::copy(s.begin(),s.end(), r.begin());
	return(r);
#endif
}

Logger::Logger(void)
{
}


Logger::~Logger(void)
{
}

void Logger::append(const std::string &line)
{
  boost::lock_guard<boost::mutex> g(_m);
	logs.push_back(line);

    unsigned long logUsed = 0;
    int i;
    for (i = logs.size()-1 ; i>0 && logUsed < logSize ; i--) {
        logUsed += logs[i].capacity();
        //logUsed++;
    }

    if (i>0)
        logs.erase(logs.begin(), logs.begin()+i);
}

void Logger::append(const char *pstrFormat, ...)
{

    std::string str;

   // format and write the data we were given
    va_list args;
    va_start(args, pstrFormat);

	vsprintf(buff, pstrFormat, args);

	va_end(args);

	str = buff;

    append(str);
    return;
}


void Logger::appendL(int line, const char*file, const char * level, std::string s)
{
	if (!checkLevel(level)) return;
	appendL(line, file, level, "%s", s.c_str());
}

void Logger::appendL(int line, const char*file, const char *level, const char *pstrFormat, ...)
{
	std::string str;
	time_t t;
	tm * ptm;

	if (!checkLevel(level)) return;

	time(&t);
	ptm = gmtime(&t);

	sprintf(buff, "%04d%02d%02d %02d%02d%02d GMT - %-8s - %s @ %d - ", ptm->tm_year+1900, ptm->tm_mon, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, level, file, line);
	str = buff;

	// format and write the data we were given
	va_list args;
	va_start(args, pstrFormat);
	vsprintf(buff, pstrFormat, args);
	va_end(args);

	str += buff;

    append(str);
}

void Logger::appendF(int line, const char*file, const char * level, std::string filename)
{
    FILE *fp = std::fopen(filename.c_str(), "r");
    printf("-----------------%s----------------------\n", filename.c_str());
    if (!fp) return;
    while(std::fgets(buff, sizeof(buff), fp)) {
        // strip trailing '\n' if it exists
        int len = strlen(buff)-1;
        if(buff[len] == '\n' || buff[len] == '\r')
            buff[len] = 0;
        if(buff[len-1] == '\n' || buff[len-1] == '\r')
            buff[len-1] = 0;
        Logger::appendL(line, file, level, "|LDC| %s", buff);
    }
    std::fclose(fp);
    std::remove(filename.c_str());
}


std::vector<std::string> Logger::logs;
std::vector<BinaryData> Logger::binData;
std::string Logger::logLevel;

void Logger::binary(const std::string &type, unsigned char *data, unsigned int len)
{
	std::string buff;

    for (unsigned int i=0; i < len; i++)
		buff += str(boost::format("%02X") % ((unsigned int)(data[i])));

	binary(type, buff);
}

void Logger::addnthrow(int line, const char*file, const char * level, std::string s)
{
	addnthrow(line, file, level, "%s", s.c_str());
}

void Logger::addnthrow(int line, const char*file, const char *level, const char *pstrFormat, ...)
{
	std::string str;
	time_t t;
	tm * ptm;

	time(&t);
	ptm = gmtime(&t);

	sprintf(buff, "%04d%02d%02d %02d%02d%02d GMT - %-8s - %s @ %d - ", ptm->tm_year+1900, ptm->tm_mon, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, level, file, line);
	str = buff;

	// format and write the data we were given
	va_list args;
	va_start(args, pstrFormat);
	vsprintf(buff, pstrFormat, args);
	va_end(args);

	str += buff;

    append(str);
	throw DBException(buff);
}




void Logger::binary(const std::string &type, const std::string &data)
{
	BinaryData b;
	b.type = type;
	b.data = data;
	binData.push_back(b);
	return;
}

std::string Logger::getBinary()
{
	unsigned int i;
	std::string out;
	std::stringstream ssb;
	for (i=0; i<binData.size();i++)
	{
		ssb << binData[i].type << " - " << binData[i].data;
		ssb << std::endl;
	}
	out = ssb.str();
	//std::string out0 = CT2CA(out);
	return(out);
}

std::string Logger::toString()
{
	unsigned int i;
	std::string out;
	std::stringstream ssb;
	for (i=0; i<logs.size();i++)
	{
		ssb << logs[i];
		ssb << std::endl;
	}
	out = ssb.str();
	//std::string out0 = CT2CA(out);
	return(out);
}

void Logger::setLogLevel(const std::string &level)
{
	//myLogLevel.push_back(level);
	logLevel = level;
}

bool Logger::checkLevel(const std::string &req)
{
	try {
		if (logLevel.compare("") == 0)
		{
			logLevel = "DEBUG";
			return true;
		}

		if (logLevel.compare("DEBUG") == 0)
			return true;

		if (logLevel.compare("INFO") == 0)
			return (!req.compare("INFO") || !req.compare("WARNING") || !req.compare("ERROR") || !req.compare("CRITICAL"));

		if (logLevel.compare("WARNING") == 0)
			return (!req.compare("WARNING") || !req.compare("ERROR") || !req.compare("CRITICAL"));

		if (logLevel.compare("ERROR") == 0)
			return (!req.compare("ERROR") || !req.compare("CRITICAL"));

		if (logLevel.compare("CRITICAL") == 0)
			return (!req.compare("CRITICAL"));

	} catch(...) {}

	return(true);
}

void Logger::clear()
{
	logs.clear();
}
