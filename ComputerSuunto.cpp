#include "stdafx.h"
#include "ComputerSuunto.h"
#include "DeviceSuuntoEmu.h"
#include "Logger.h"

static bool ifacealwaysechos;
static bool break_prof_read_early=true;


ComputerSuunto::ComputerSuunto(std::string filename)
	: Computer()
{
	LOGINFO("Creating Suunto device on %s", filename.c_str());
	
	status.state = COMPUTER_NOT_STARTED;
	status.nbDivesRead  = -1;
	status.nbDivesTotal = -1;

	if (filename.size()) device = new DeviceSuunto(filename);
	else device = new DeviceSuuntoEmu();
}


ComputerSuunto::~ComputerSuunto(void)
{
	LOGINFO("Deleting object device");
	delete device;
}


/*
static gboolean suunto_detect_interface(gint fd) 
{
  int rc=0;
  gboolean rval=false;
	
  set_rts(fd,RTS_STATUS_ON);
  usleep(300000);
  set_rts(fd,RTS_STATUS_OFF);
  if(suunto_send_testcmd(fd,"AT\r")) {

    suunto_read_serial(fd);
    suunto_read_serial(fd);
    suunto_read_serial(fd);
    rc=suunto_read_serial(fd);

    set_rts(fd,RTS_STATUS_ON);	// Try transfer mode now 
    if(!suunto_send_testcmd(fd,"AT\r")) g_printerr(_("Cannot detect Suunto interface.")); 
    else {
      rval=true;	
      set_rts(fd,RTS_STATUS_OFF);
      rc=suunto_read_serial(fd);
      if(rc==-1) {

        ifacealwaysechos=false;
      }
      else {

        suunto_read_serial(fd);
        suunto_read_serial(fd);
        suunto_read_serial(fd);
        ifacealwaysechos=true;
      }
    }
  }
  return rval;
}
*/

unsigned char ComputerSuunto::generate_crc(unsigned char *buffer,int len) 
{
  char crc=0;

  while(len--) crc^=*buffer++;
  return crc;
}

void ComputerSuunto::send_command(unsigned char *commbuffer,int len) 
{
  int rc;
  unsigned char buff;
  bool rval=true;
	std::string command;
  for (int i=0; i<len; i++)
	  command += str(boost::format(" %02X") % (int)(commbuffer[i]));

#ifdef _WIN32
  Sleep(800);
#elif defined(__MACH__) || defined(__linux__)
  usleep(800000);	
#else
#error unsoupported platform
#endif
	
  LOGINFO("Sending command to Suunto : %s", command.c_str());
  try {
	device->write_serial(commbuffer,len);
  } catch (...) {
	  //todo : handle errors on communications
	  throw;
  }


  if(ifacealwaysechos) {
    while(len--) {
	  try {
		  rc=device->read_serial(&buff);
		  if(rc==-1 || buff!=*commbuffer++) rval=false;
	  } catch (...) {
		  //todo : handle errors on communications  
		  rval = false;
	  }
    }
  }
  if (!rval) throw DBException("Error while sending the command");
}

void ComputerSuunto::read(int start,char *retbuffer,int len) 
{

  unsigned char command[]={0x05,
                    0x00, // high 
                    0x00, //* low   
                    0x01, //* count 
                    0x00  //* CRC   
                   },
         reply[4]={0,0,0,0},
         crc=0x00;
  int i;
  bool rval=0; 
  unsigned char read;
	std::string data;
  data += "Data read :";

  command[1]=(start>>8)&0xff;
  command[2]=(start)&0xff;
  command[3]=len;
  command[4]=generate_crc(command,4);

  send_command(command,5);
#ifdef _WIN32
	  Sleep(800);
#elif defined(__MACH__) || defined(__linux__)
	  usleep(8000);	
#else
#error unsupported platform
#endif
	
    LOGINFO("Reading from Suunto");
	
	for(i=0;i<4;i++) {
		device->read_serial(&read, 1, 10);
		data += str(boost::format(" %02X") % (int)read);
		reply[i]=read;
	}

    if(reply[0]==command[0] && reply[1]==command[1] && reply[2]==command[2] && reply[3]==command[3]) {
      crc=command[0]^command[1]^command[2]^command[3];
      for(i=0;i<len;i++) {
		device->read_serial((unsigned char*)retbuffer+i);
        data += str(boost::format(" %02X") % (int)(retbuffer[i]));
        crc^=retbuffer[i];
      }

	  device->read_serial(&read);
	  data += str(boost::format(" %02X") % (int)read);
      if(crc==read) rval=true;
      else {
		  LOGINFO("CRC Error -- %s", data.c_str());
		  throw DBException("CRC Error while reading");
	  }
    }
    else if(reply[0]==255) {
		LOGINFO("The interface appears to be present, but the dive computer is not responding.");
        throw DBException("Computer seems unconnected");
    }
   
	LOGINFO("Data read correctly -- %s", data.c_str());
}

ComputerModel ComputerSuunto::_get_model()
{
	ComputerModel model=COMPUTER_MODEL_UNKNOWN;
	char rbuf[2];

	LOGINFO("Reading model digits");

	read(0x24,rbuf,1);
	//todo : if exception ? return COMPUTER_MODEL_UNKNOWN;

	LOGINFO("Model digits : %d %d", rbuf[0], rbuf[1]);
    switch(rbuf[0]) {
      case 40 :
        read(0x16,rbuf,2);
		//todo : if exception ? return COMPUTER_MODEL_UNKNOWN;
        if(rbuf[0]==0x01&&rbuf[1]==0x01) model=SUUNTO_MODEL_OLD_SPYDER;
        else if(rbuf[0]==0x01&&rbuf[1]==0x02) model=SUUNTO_MODEL_NEW_SPYDER;
        break;
      case 0x0c:
        model=SUUNTO_MODEL_OLD_COBRA_OR_VYPER;
        break;
      case 0x0a:
        model=SUUNTO_MODEL_NEW_VYPER;
        break;
      case 0x0b:
        model=SUUNTO_MODEL_VYTEC;
        break;
      case 0x03:
        model=SUUNTO_MODEL_STINGER;
        break;
      case 0x04:
        model=SUUNTO_MODEL_MOSQUITO;
        break;
    }
  
	return model;
}

int ComputerSuunto::get_dive(char suunto_dive_which,unsigned char *divebuf,int len) 
{
  unsigned char command[]={0x08,0xa5,0x00};
  int i,j,rc,crc,packet_len;
	std::string data;
  unsigned char read;

  command[0]=(char)suunto_dive_which;
  command[2]=generate_crc(command,2);

  send_command(command,3);

  for(i=0;i<len;) {
      rc = device->read_serial(&read, 1, 2);
	  data += str(boost::format(" %02X") % (int)read);
      if(read!=command[0]) {
          if(rc==-1) break;
		  DBthrowError("Illegal start of packet. --- Read data : %s", data.c_str());
      }
      crc=read;
      rc=device->read_serial(&read);
	  data += str(boost::format(" %02X") % (int)read);
	  packet_len = read;
      if(rc<0 || packet_len>32) {
		  DBthrowError("Illegal packet length. --- Read data : %s", data.c_str());
      }
      crc^=read;
      for(j=0;j<packet_len&&i<len;j++,i++) {
          rc=device->read_serial(&read);
		  data += str(boost::format(" %02X") % (int)read);
          if(rc<0) {
			  DBthrowError("Unexpected end of packet --- Read data : %s", data.c_str());
          }
          divebuf[i]=read;
		  data += str(boost::format(" %02X") % (int)divebuf[i]);
          crc^=divebuf[i];
      }
      rc = device->read_serial(&read);
      data += str(boost::format(" %02X") % (int)read);
	  crc^= read;
      if(crc!=0) {
		  DBthrowError("CRC check failure. --- Read data : %s", data.c_str());
      }
  
	  if(break_prof_read_early && packet_len!=32) break; 
    }
    LOGINFO("Data read : %s", data.c_str());

	return i;
}

void ComputerSuunto::parse_dive(unsigned char *divebuf,int len,ComputerModel model,DiveData &dive)
{
  int i,interval=1;
  double depth=0.0;
  ImportProfile profile_segment;
  unsigned char lwr=0x79,upr=0x87; // profile bytes between lwr & upr are alarms, not depth deltas 
  
  i=len-1;
  if(IS_SPYDER(model)) {
    lwr=0x7d;
    upr=0x82;
    dive.min_temperature=divebuf[i-1];
    dive.max_temperature=dive.min_temperature;
    i-=2;
  }
  else {
	  dive.date = str(boost::format("%04d-%02d-%02d") %
      (divebuf[i-9]+(divebuf[i-9]>89?1900:2000)) %
      (int)(divebuf[i-10]) %
      (int)(divebuf[i-11])
	  );
	  dive.time = str(boost::format("%02d:%02d:00") %
      (int)(divebuf[i-12]) %
      (int)(divebuf[i-13])
      );
/*    if(last_dive_datetime) {
      if(g_utf8_collate(dive.datetime,last_dive_datetime)<0) {
        g_free(dive.datetime);
        return false;
      }
    }*/
    interval=divebuf[i-3];
    dive.start_pressure=divebuf[i-5]?divebuf[i-5]*2:NULL_PRESSURE;
    dive.O2=divebuf[i-6]?divebuf[i-6]:21.0;
    dive.min_temperature=divebuf[3];
    dive.max_temperature=divebuf[2];
	dive.air_temperature=divebuf[i-8];
	dive.surface_hour=divebuf[i-1];
	dive.surface_min=divebuf[i];
	dive.repnum=divebuf[i-3];
	dive.altitude = divebuf[i-4];
    i-=14;	
  }

  profile_segment.time=0;
  profile_segment.depth=0;
  profile_segment.temperature=NULL_TEMPERATURE;	
	dive.profile = str(boost::format("<t>%f</t><d>%f</d>") % profile_segment.time % profile_segment.depth);

  dive.duration=0;
  dive.max_depth=0.0;
  
  //0x80 marks end of dive
  while(divebuf[i]!=0x80) {

	  //Samples
	  if(divebuf[i]<lwr || divebuf[i]>upr) {
		  depth+=((char)(divebuf[i]))*0.3048 ;
		  if(depth>dive.max_depth) dive.max_depth=depth;
		  dive.duration+=interval;
		  profile_segment.time=dive.duration;
		  profile_segment.depth=depth;
		  profile_segment.temperature=NULL_TEMPERATURE;
		  dive.profile += str(boost::format("<t>%f</t><d>%f</d>") % profile_segment.time % profile_segment.depth);
	  }
	
	  //Special events
	  else if (divebuf[i] == 0x7a) dive.profile += "<ALARM>ASCENT</ALARM>";   //GOING UP TOO QUICKLY
	  else if (divebuf[i] == 0x7b) dive.profile += "<ALARM>ERROR</ALARM>";    //ATTENTION VIOLATION	
	  else if (divebuf[i] == 0x7c) dive.profile += "<ALARM>BOOKMARK</ALARM>"; //BOOKMARK	
	  else if (divebuf[i] == 0x7d) dive.profile += "<ALARM>SURFACE</ALARM>";  //SURFACE	
	  else if (divebuf[i] == 0x7e) dive.profile += "<ALARM>DECO</ALARM>";     //DECO CEILING APPEARED
	  else if (divebuf[i] == 0x7f) dive.profile += "<ALARM>ERROR</ALARM>";    //GOING ABOVE CEILING
	  else if (divebuf[i] == 0x80) {}                                              //END OF DIVE
	  else if (divebuf[i] == 0x81) dive.profile += "<ALARM>ERROR</ALARM>";    //GOING ABOVE SAF STOP
	  else if (divebuf[i] == 0x87) dive.profile += str(boost::format("<SWITCH>%f</SWITCH>")% divebuf[--i]); // SWITCH GAS

      i--;
  }

  //TODO : handle spyder
/*  if(lwr==0x7d) { // is spyder 
    dive.datetime=g_strdup_printf("%04d-%02d-%02d %02d:%02d:00",
      divebuf[i-5]+(divebuf[i-5]>89?1900:2000),
      divebuf[i-4],
      divebuf[i-3],
      divebuf[i-2],
      divebuf[i-1]
    );
    if(g_utf8_collate(dive.datetime,last_dive_datetime)<0) {
      g_free(dive.datetime);
      g_array_free(dive.profile,true);
      return false;
    }
    interval=divebuf[i-8];
    for(j=1;j<dive.profile->len;j++) {
      profile_segmentptr=&g_array_index(dive.profile,ImportProfile,j);
      profile_segmentptr->time=profile_segmentptr->time*interval;
    }
    dive.duration=dive.duration*interval;
  }
  else {
    dive.min_temperature=divebuf[i-1];
    dive.max_temperature=divebuf[i-2];
    if(dive.max_temperature<dive.min_temperature) {
      dive.min_temperature=dive.max_temperature;
      dive.max_temperature=divebuf[i-1];
    }
    dive.end_pressure=divebuf[i-3]?divebuf[i-3]*2:NULL_PRESSURE;
  }
  */
  //g_array_append_val(dive_array,dive);
}




int ComputerSuunto::_get_all_dives(std::string &xml)
{
    LOGDEBUG("Starting _get_all_dives");
	unsigned char buff2[4048];
	DiveData dive;
	int length;

	char step = SUUNTO_DIVE_FIRST;
	status.nbDivesRead = -1;
	status.nbDivesTotal = -1;

	status.state = COMPUTER_RUNNING;

	xml += "<profile udcf='1'><device><vendor>Suunto</vendor><model>Vyper</model></device><REPGROUP>";

	while (true)
	{
		try {
			LOGINFO("Downloading a dive");
			length = get_dive(step,buff2,4048);

			if (length == 0)
			{
				LOGINFO("End of data or timeout");
				break;
			}

			LOGDEBUG("Now parsing the data");
			parse_dive(buff2,length,SUUNTO_MODEL_NEW_VYPER,dive);

			if (status.nbDivesRead<0) status.nbDivesRead = 1;
			else status.nbDivesRead++;

			LOGDEBUG("Formatting the data");
			xml += "<DIVE>";
			xml += str(boost::format("<DATE>%s</DATE><TIME>%s</TIME><DURATION>%.0f</DURATION><TEMPERATURE>%f</TEMPERATURE><ALTITUDE>%d</ALTITUDE><SURFACEINTERVAL>%0.1f</SURFACEINTERVAL>") 
				% dive.date 
				% dive.time
				% (((float)dive.duration)/60)
				% dive.min_temperature
				% dive.altitude
				% (dive.surface_hour*3600+dive.surface_min*60));
			xml += str(boost::format("<GASES><MIX><MIXNAME>1</MIXNAME><O2>%f</O2><N2>%f</N2><HE>0.0</HE></MIX></GASES>") % (dive.O2/100.0) % (1.0-dive.O2/100.0) );

			xml += str(boost::format("<PROGRAM><PRESSTART>%f</PRESSTART><PRESEND>%f</PRESEND><TEMPEND>%f</TEMPEND><REPDIVENUM>%d</REPDIVENUM><TEMPAIR>%f</TEMPAIR></PROGRAM>")
				% dive.start_pressure
				% dive.end_pressure
				% dive.max_temperature
				% dive.repnum
				% dive.air_temperature
				);
			xml += "<SAMPLES>"+ dive.profile +"</SAMPLES>";
			xml += "</DIVE>";

			step = SUUNTO_DIVE_NEXT;
		} catch (std::exception &e) {
				LOGERROR("Caught Exception while downloading dive : %s", e.what());
				xml = "";
				device->close();
				throw;
		} catch (...) {
				LOGERROR("Caught Unknown Exception while downloading dive");
				xml = "";
				device->close();
				throw;
		}
	}
	xml += "</REPGROUP></profile>";

	status.nbDivesTotal = status.nbDivesRead;
	status.state = COMPUTER_FINISHED;
	LOGINFO("Found %d dives", status.nbDivesRead);

	device->close();

    LOGDEBUG("device closed", "");
	Logger::appendL(12, "toto", "warn", "hell");
	return(0);
}

ComputerStatus ComputerSuunto::get_status()
{
	return(status);
}


std::vector<ComputerSupport> *ComputerSuunto::support_list = NULL;
std::vector<ComputerSupport> *ComputerSuunto::support()
{
  if (support_list)
    return(support_list);

  support_list = new std::vector<ComputerSupport>;

  ComputerSupport sup;
  sup.make = "Suunto";
  sup.model = "Vyper (Diveboard internal driver)";
  sup.key_code = "SUUNTO";
#ifdef _WIN32
  sup.ports.push_back("Suunto USB Serial Port");
  sup.ports.push_back("Prolific USB-to-Serial Comm Port");
#elif defined(__MACH__) || defined(__linux__)
  sup.ports.push_back("tty.usbserial-PtTFP8W4");
#else
#error Platform not supported
#endif

  ComputerSupport emu;
  emu.make = "Suunto";
  emu.model = "Emulator";
  emu.key_code = "SUUNTO EMU";
  emu.ports.push_back(NO_PORT_NEEDED);

  support_list->push_back(sup);
  support_list->push_back(emu);

  return(support_list);
}

