#ifndef __COMPUTER_FACTORY_HPP__
#define __COMPUTER_FACTORY_HPP__

#include "Computer.h"
#include <string>
#include <map>
#include <vector>

class ComputerFactory
{
public:
	ComputerFactory(void);
	~ComputerFactory(void);

	std::map <std::string, std::string>detectConnectedDevice();
	Computer *createComputer(const std::string &type, const std::string &filename);
	std::string detectConnectedDevice(const std::string &computerType);
	bool isComputerPluggedin();
	std::map <std::string, std::string> allPorts(bool scanBT = false);
	std::map<std::string, std::vector<ComputerSupport> > supported;
protected:
	void listPorts(std::string &);
	bool mapDevice(std::string identifier, std::string &found);
	std::map <std::string, std::vector<std::string> > recognisedPorts;
};

#endif
