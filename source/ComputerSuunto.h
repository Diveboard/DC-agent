#pragma once

#include "Device.h"
#include "DeviceSuunto.h"
#include "Computer.h"


typedef enum {
  COMPUTER_MODEL_UNKNOWN=0,
  SUUNTO_MODEL_OLD_SPYDER,
  SUUNTO_MODEL_NEW_SPYDER,
  SUUNTO_MODEL_OLD_COBRA_OR_VYPER,
  SUUNTO_MODEL_NEW_VYPER,
  SUUNTO_MODEL_VYTEC,
  SUUNTO_MODEL_STINGER,
  SUUNTO_MODEL_MOSQUITO,
} ComputerModel;



class ComputerSuunto : public Computer
{
private:
	Device *device;
	bool isConnected;
	unsigned char generate_crc(unsigned char *buffer,int len) ;
	void send_command(unsigned char *commbuffer,int len) ;
	void read(int start,char *retbuffer,int len) ;
    void parse_dive(unsigned char *divebuf,int len,ComputerModel model,DiveData &dive);
	int get_dive(char suunto_dive_which,unsigned char *divebuf,int len) ;
	ComputerStatus status;
	static std::vector<ComputerSupport> *support_list;
public:
	ComputerSuunto(std::string filename);
	virtual ~ComputerSuunto(void);
	ComputerModel _get_model();
	int _get_all_dives(std::string &xml);
	ComputerStatus get_status();
  static std::vector<ComputerSupport> *support();
};

