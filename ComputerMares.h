#pragma once

#include "Computer.h"
#include "Device.h"
#include "stdafx.h"


class ComputerMares : public Computer
{
protected:
	Device *device;
	bool isConnected;
	unsigned char generate_crc(unsigned char *buffer,int len) ;
	bool send_command(unsigned char *commbuffer,int len) ;
	int read(int start,unsigned char *retbuffer,int len) ;
	bool ifacealwaysechos;
	int list_dives(std::vector<DiveData> &dives);
	unsigned char map[256];
	void convertHex2Bin(unsigned char *dst, unsigned char *src, int dst_len);
	void format_dives(std::vector<DiveData> dives, std::string &xml);
	ComputerStatus status;
	static std::vector<ComputerSupport> *support_list;
public:
	ComputerMares(std::string filename);
	virtual ~ComputerMares(void);
	int _get_all_dives(std::string &xml);
	ComputerStatus get_status();
  static std::vector<ComputerSupport> *support();
};