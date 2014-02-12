#pragma once

#include "stdafx.h"
#include "Device.h"

class DeviceMaresEmu : public Device
{
public:
	DeviceMaresEmu();
	int open() ;
	void close(); 
	int write_serial(unsigned char *buffer, int nb);
	int read_serial(unsigned char *buffer, unsigned int nb=1, int timeout=0);
protected:
	unsigned int position;
	//unsigned char memory[32800];
	unsigned char memory[90000];
	unsigned char map[256];
	unsigned char command[50];
	unsigned char generate_crc(unsigned char *string, int len);
	std::vector<unsigned char> answers;
};
