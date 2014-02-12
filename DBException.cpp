#include "DBException.h"
#include <boost/format.hpp>

DBException::DBException(std::string t, std::exception e)
	: m_error((t + " --- " + e.what()).c_str())
{
	ex = e;
}

DBException::DBException(std::string t)
	: m_error(t.c_str())
{
}

DBException::DBException()
	: m_error("Unqualified DBO Exception")
{
}

DBException::~DBException(void) throw()
{
}
