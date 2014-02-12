#pragma once

#include "stdafx.h"
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
	std::map <std::string, std::string> allPorts();
	std::vector<ComputerSupport> supported;
protected:
	void listPorts(std::string &);
	bool mapDevice(std::string identifier, std::string &found);
	std::map <std::string, std::vector<std::string> > recognisedPorts;
};

