/*
 *  DeviceMares.h
 *  FireBreath
 *
 *  Created by Pascal Manchon on 27/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "stdafx.h"
//#include <WinBase.h>

#include "Device.h"
#include "Computer.h"


#define TIMEOUT 500000
#define NOT_SERIAL_PORT -2




typedef struct {
	double time;
	double depth;
	double temperature;
} ImportProfile;

#define NULL_PRESSURE 0
#define NULL_TEMPERATURE 0

typedef unsigned int UINT;

class DeviceMares : public ComDevice
{
public:
	DeviceMares(std::string filename);
	~DeviceMares();
	int open();
	int write_serial(unsigned char *,int);
	int read_serial(unsigned char * buff, unsigned int num = 1, int timeout=10);
};
