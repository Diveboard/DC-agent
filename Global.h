#pragma once

#define VERSION "20140914"

#ifdef WIN32
#define OS_CHECK_UPDATE "agent_win32"
#elif __MACH__
#define OS_CHECK_UPDATE "agent_osx"
#elif __linux__
#define OS_CHECK_UPDATE "agent_linux"
#define LIBDIVE_SO "/usr/lib/diveboard"
#endif

#define APIBASEURL "https://www.diveboard.com/api/"
#define APIKEY "BVu3iqQKTeB7iI3T"
