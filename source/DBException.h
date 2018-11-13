#pragma once
#include <exception>
#include <string>

class DBException :
	public std::exception
{
private:
	std::exception ex;
	std::string m_error;
public:
	DBException(void);
	DBException(std::string t, std::exception e);
	DBException(std::string t);
	~DBException(void) throw();
	virtual const char* what() const throw() {
		return m_error.c_str();
	}
};

