#pragma once

#include "stdafx.h"
//#include <WinBase.h>

#include "Device.h"
#include "Computer.h"


#define SUUNTO_DIVE_FIRST 0x08
#define SUUNTO_DIVE_NEXT 0x09
#define TIMEOUT 500000
#define NOT_SERIAL_PORT -2



#define IS_SPYDER(model) (model==SUUNTO_MODEL_OLD_SPYDER||model==SUUNTO_MODEL_OLD_SPYDER)

enum {
  SUUNTO_COL_DATE=0,
  SUUNTO_COL_TIME,
  SUUNTO_COL_MAXDEPTH,
  SUUNTO_COL_DURATION,
  SUUNTO_COL_MAXTEMP,
  SUUNTO_COL_MINTEMP,
  SUUNTO_COL_DELETED,
  SUUNTO_COL_DATA,
  SUUNTO_NUM_COLS
};


typedef struct {
	double time;
	double depth;
	double temperature;
} ImportProfile;

#define NULL_PRESSURE 0
#define NULL_TEMPERATURE 0

typedef unsigned int UINT;

class DeviceSuunto : public ComDevice
{
public:
	DeviceSuunto(std::string filename);
	~DeviceSuunto();
	int open();
	int write_serial(unsigned char *,int);
	int read_serial(unsigned char * buff, unsigned int num = 1, int timeout=0);
};
