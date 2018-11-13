#pragma once

#include "stdafx.h"
#include "Device.h"

class DeviceSuuntoEmu : public Device
{
public:
	int open() ;
	void close(); 
	int write_serial(unsigned char *,int);
	int read_serial(unsigned char *buffer, unsigned int nb=1, int timeout=0);
	DeviceSuuntoEmu() {};
protected:
	std::vector<unsigned char> answers;
};
