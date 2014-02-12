#pragma once

#include "stdafx.h"
#include <string>

class DiveData{
public:
  double O2;
  double start_pressure;
  double end_pressure;
  double altitude;
  double max_temperature;
  double min_temperature;
  double air_temperature;
  double max_depth;
  int repnum;
  int surface_hour;
  int surface_min;
  long duration;
	std::string date;
	std::string time;
	std::string profile;
} ;

typedef enum {
	COMPUTER_NOT_STARTED,
	COMPUTER_RUNNING,
	COMPUTER_FINISHED
} ComputerState;

typedef struct {
	int nbDivesRead;
	int nbDivesTotal;
	ComputerState state;
	int percent;
} ComputerStatus;


#define NO_PORT_NEEDED "NO PORT NEEDED"

typedef struct {
  std::string label;
  std::string key_code;
  std::vector<std::string> ports;
} ComputerSupport;

class Computer
{
	HANDLE mutex;
protected:
  virtual int _get_all_dives(std::string &xml) {return(-1);};
  virtual int _dump(std::string &data) {return(-1);};
public:
	Computer(){};
	virtual ~Computer(void){};
	//virtual int test()=0;
  int get_all_dives(std::string &xml) {return(_get_all_dives(xml));};
  int dump(std::string &data) {return(_dump(data));};
  static std::vector<ComputerSupport> *support() { return(NULL); };
	virtual ComputerStatus get_status() {ComputerStatus t; t.nbDivesRead=0; t.nbDivesTotal=0; t.state = COMPUTER_NOT_STARTED; return(t);};
	virtual void cancel() { }
};

