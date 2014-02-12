#include "stdafx.h"
#include "ComputerMares.h"
#include "DeviceMaresEmu.h"
#include "DeviceMares.h"
#include "Logger.h"
#include <boost/format.hpp>



ComputerMares::ComputerMares(std::string filename)
	: Computer()
{
	LOGINFO("Creating Mares device on %s", filename.c_str());
	if (filename.size())
		device = new DeviceMares(filename);
	else
		device = new DeviceMaresEmu();

	ifacealwaysechos=true;

	status.state = COMPUTER_NOT_STARTED;
	status.nbDivesRead  = -1;
	status.nbDivesTotal = -1;

	memset(map, 0, 256);
	map[(int)'0'] = 0;
	map[(int)'1'] = 1;
	map[(int)'2'] = 2;
	map[(int)'3'] = 3;
	map[(int)'4'] = 4;
	map[(int)'5'] = 5;
	map[(int)'6'] = 6;
	map[(int)'7'] = 7;
	map[(int)'8'] = 8;
	map[(int)'9'] = 9;
	map[(int)'A'] = 10;
	map[(int)'B'] = 11;
	map[(int)'C'] = 12;
	map[(int)'D'] = 13;
	map[(int)'E'] = 14;
	map[(int)'F'] = 15;
}


ComputerMares::~ComputerMares(void)
{
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

unsigned char ComputerMares::generate_crc(unsigned char *buffer,int len) 
{
  unsigned int crc=0;

  while(len--) crc += *buffer++;
  return crc;
}

bool ComputerMares::send_command(unsigned char *commbuffer,int len) 
{
  bool rval=true;
	std::string command;
  for (int i=0; i<len; i++)
	  command.append(" %02X", (int)commbuffer[i]);

	std::string out;
	out.assign((char*)commbuffer, len);
  LOGINFO("Sending command to Mares : %s", command.c_str());
  LOGINFO("Cleartext sent to Mares : %s", out.c_str());
  device->write_serial(commbuffer,len);

  return rval;
}

int ComputerMares::read(int start,unsigned char *retbuffer,int len) 
{

  unsigned char command[13],
         reply[13],
         crc=0x00, addr_low, addr_high, readcrc, buffcrc[2];
  bool rval=0; 
  char rc;
  int r;
	std::string data;
  data.append("Data read :");

  addr_low = start % 256;
  addr_high = ((start - addr_low) / 256) % 256;

  sprintf((char*)command, "<51%02X%02X%02X%02X>", (int)addr_low, (int)addr_high, len, 0);
  crc = generate_crc(command+1,8);
  sprintf((char*)command, "<51%02X%02X%02X%02X>", (int)addr_low, (int)addr_high, len, (int)crc);

  LOGINFO("Reading from Mares");
  int retry_count = 0;
  while(retry_count < 5) {
    try {
      if(send_command(command,12)) {

            LOGDEBUG("Read attempt #%d", retry_count);
            r = device->read_serial(reply, 13, 10);
            if (r < 0) {
                LOGINFO("Error reading (%d) - %s", r, data.c_str());
                return(r);
            }

            for (int i=0;i<13;i++) 
                data += str(boost::format(" %02X") % (int)reply[i]);

            if(!memcmp(command, reply, 12)) {
            }

            rc = device->read_serial(retbuffer, len*2, 10);
            for (int i=0; i<len*2; i++) data += str(boost::format(" %02X") % (int)retbuffer[i]);


              r = device->read_serial(buffcrc, 2, 10);
              if (r < 0) {
                LOGINFO("Error reading (%d) - %s", r, data.c_str());
                return(r);
              }
            data += str(boost::format(" %02X %02X") % (int)buffcrc[0] % (int)buffcrc[1]);

            crc = generate_crc(retbuffer, 2*len);
            readcrc = map[buffcrc[0]]*16 + map[buffcrc[1]];

            rc = device->read_serial(reply, 1);
            data += str(boost::format(" %02X") % (int)reply[0]);

            if(crc==readcrc && rc >=0 ) 
                rval=true;
            else {
                LOGINFO("CRC Error -- %s", data.c_str());
                LOGINFO("CRC calculated : %02X - CRC read (string) : %.2s - CRC read (int) : %2X", (int)crc, buffcrc, (int)readcrc);
                std::string out;
                out.append((char*)retbuffer, 2*len);
                LOGINFO("Text Data read -- %s", out.c_str());
                return(SUUNTO_ERR_CRC);	
            }
            LOGINFO("Data read correctly -- %s", data.c_str());
            std::string out;
            out.append((char*)retbuffer, 2*len);
            LOGINFO("Text Data read correctly -- %s", out.c_str());
            return 0;
        }
      } catch(DBException e) {
          LOGDEBUG("Error while reading");
          retry_count++;
          if (retry_count >= 5) throw e;
          else {
              device->close();
              device->open();
          }
      }
	}
		
	return(-1);
}


int ComputerMares::list_dives(std::vector<DiveData> &dives)
{
	unsigned char buffer[52*2+16];
	unsigned char profile[13520*4];
	unsigned char *profilePos = profile;
	DiveData *dive;
	int interval = 20;
	DWORD addProfile=0x0B30;

	//todo : read the 10 last bytes
	//for (DWORD address=0x0B30; address < 0x3FC0; address += 64)
	//	read(address, profile+address-0x0B30, 64);

	status.nbDivesRead = 0;

	for (DWORD address=0x100; address < 0xB00 ;  address += 0x34)
	{
		unsigned int nbsamples;
		read(address, buffer, 0x34);
		std::string out;
		out.append((char*)buffer, 104);
		LOGINFO("Reading at %02X : %.104s", (int)address, out.c_str());

		if (!memcmp(buffer, "07D",3))
		{
			//todo : delete sometimes
			dive = new DiveData();
			dive->date += str(boost::format("%04d-%02d-%02d") %
			(int)(map[buffer[3]]+2000) %
			(int)(map[buffer[5]]) %
			(int)(map[buffer[6]]*16+map[buffer[7]])
			);
			dive->time += str(boost::format("%02d:%02d:00") %
			(int)(map[buffer[8]]*16+map[buffer[9]]) %
			(int)(map[buffer[10]]*16+map[buffer[11]]));

			LOGINFO("Currently on dive : %s %s", dive->date.c_str(), dive->time.c_str());

			nbsamples = map[buffer[14]]*16+map[buffer[15]];
			LOGINFO("nbsamples : %d (%X %X)", nbsamples, (int)buffer[14], (int)buffer[15]);

			dive->max_depth=(map[buffer[16]]*16*16*16 + map[buffer[17]]*16*16 + map[buffer[18]]*16 + map[buffer[19]])/10;
			dive->duration=(nbsamples*interval)/60;
			dive->min_temperature=map[buffer[20]]*16+map[buffer[21]];
			dive->start_pressure=map[buffer[54]]*16*16*16 + map[buffer[55]]*16*16 + map[buffer[56]]*16 + map[buffer[57]];

			dive->air_temperature = 0;
			dive->altitude = 0;
			dive->end_pressure=0;
			dive->max_temperature=0;
			dive->O2=0;
			dive->repnum=0;
			dive->surface_hour=0;
			dive->surface_min=0;

			//read enough data for the dive
			LOGINFO("%X %X %d %X -- %d %d", profilePos, profile, nbsamples, addProfile, profilePos-profile+nbsamples*4, addProfile-0xB30);
			for (; profilePos-profile+nbsamples*4 > addProfile-0xB30 ; addProfile+= 64)
			{
				LOGINFO("+");
				read(addProfile, profile+2*(addProfile-0x0B30), 64);
			}

			for (unsigned int i=0; i<nbsamples; i++)
			{
				//todo : what to do with profilePos[2] ???
				double depth = (map[profilePos[3]] & 3)*16*16 + map[profilePos[0]]*16 + map[profilePos[1]];
				//LOGINFO("%1X%1X%1X%1X   -- %d *256 + %d * 16 + %d = %f", map[profilePos[0]], map[profilePos[1]], map[profilePos[2]], map[profilePos[3]], (map[profilePos[3]] & 3), map[profilePos[0]], map[profilePos[1]], depth);
				dive->profile += str(boost::format("<t>%d</t><d>%.1f</d>")% (i*interval) % (depth/10));


				std::string alarm;
				if (map[profilePos[2]]>0) alarm += str(boost::format("%d") % (int)(map[profilePos[2]]));
				if (map[profilePos[3]] & 8) alarm += "  F_8  ";
				if (map[profilePos[3]] & 4) alarm += "  F_4  ";
				if (alarm.size() > 0) dive->profile += "<ALARM>" + alarm + "</ALARM>";

				profilePos+=4;
			}

			dives.push_back(*dive);
			status.nbDivesRead++;
		}
		//Todo : else retry ?
	}

	LOGINFO("Found %d dives", status.nbDivesRead);
	status.nbDivesTotal = status.nbDivesRead ;
	return(0);
}


void ComputerMares::format_dives(std::vector<DiveData> dives, std::string &xml)
{
	xml += "<profile udcf='1'><device><vendor>Mares</vendor><model>M2</model><REPGROUP>";
	for (unsigned int i=0; i<dives.size();i++)
	{
		xml += "<DIVE>";
		if (dives[i].date.length()) xml += "<DATE>"+ dives[i].date +"</DATE>";
		if (dives[i].time.length()) xml += "<TIME>"+ dives[i].time +"</TIME>";
		if (dives[i].duration)          xml += str(boost::format("<DURATION>%1%</DURATION>") % dives[i].duration );
		if (dives[i].min_temperature)   xml += str(boost::format("<TEMPERATURE>%1%</TEMPERATURE>") % dives[i].min_temperature);
		if (dives[i].altitude)         xml += str(boost::format("<ALTITUDE>%1%</ALTITUDE>") % dives[i].altitude);
		if (dives[i].surface_hour || dives[i].surface_min) xml += str(boost::format("<SURFACEINTERVAL>%1%</SURFACEINTERVAL>") % (dives[i].surface_hour*3600+dives[i].surface_min*60));
		if (dives[i].O2)               xml += str(boost::format("<GASES><MIX>%1%</MIX></GASES>") % dives[i].O2);

		xml += "<PROGRAM>";
		if (dives[i].start_pressure) xml += str(boost::format("<PRESSTART>%1%</PRESSTART>") % dives[i].start_pressure);
		if (dives[i].end_pressure) xml += str(boost::format("<PRESEND>%1%</PRESEND>") % dives[i].end_pressure);
		if (dives[i].max_temperature) xml += str(boost::format("<TEMPEND>%1%</TEMPEND>") % dives[i].max_temperature);
		if (dives[i].repnum) xml += str(boost::format("<REPDIVENUM>%1%</REPDIVENUM>") % dives[i].repnum);
		if (dives[i].air_temperature) xml += str(boost::format("<TEMPAIR>%1%</TEMPAIR>") % dives[i].air_temperature);
		xml += "</PROGRAM>";
		if (dives[i].profile.size()>0) xml += "<SAMPLES>"+ dives[i].profile +"</SAMPLES>";
		xml += "</DIVE>";
	}
	xml += "</REPGROUP></profile>";
}

int ComputerMares::_get_all_dives(std::string &xml)
{
	status.state = COMPUTER_RUNNING;
	std::vector<DiveData> dives;
	list_dives(dives);
	format_dives(dives, xml);
	status.state = COMPUTER_FINISHED;
	return(0);
}

ComputerStatus ComputerMares::get_status()
{
	return(status);
}


std::vector<ComputerSupport> *ComputerMares::support_list = NULL;
std::vector<ComputerSupport> *ComputerMares::support()
{
	if (support_list)
		return(support_list);

	support_list = new std::vector<ComputerSupport>;
	ComputerSupport sup;
	sup.label = "Mares M2 (DB internal driver)";
	sup.key_code = "MARES";
#ifdef _WIN32
	sup.ports.push_back("Silicon Labs CP210x USB to UART Bridge");
	sup.ports.push_back("CP210x USB to UART Bridge Controller");
#elif defined(__MACH__) || defined(__linux__)
	sup.ports.push_back("tty.SLAB_USBtoUART");
#else
#error Platform not supported
#endif

  ComputerSupport emu;
  emu.label = "Mares M2 Emulator";
  emu.key_code = "MARES EMU";
  emu.ports.push_back(NO_PORT_NEEDED);

  support_list->push_back(sup);
  support_list->push_back(emu);

	return(support_list);
}




