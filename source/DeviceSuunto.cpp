/*
    Copyright (c) 2005 Simon Naunton

    A lot of the coded needed to download a Suunto dive computer was
    cribbed from Andreas Beck's vyperlink, so...

    Copyright (c) 2002 Andreas Beck
    Copyright (c) 2002 Mike Brodbelt

	...and lessons learned about the importance of setting DTR and RTS
	correctly were garnered from Uwe Ohse's mosquitoget.

    This file is part of gdivelog.

    gdivelog is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    gdivelog is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with gdivelog; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "DeviceSuunto.h"
#include "Logger.h"

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
/* for strptime prototype - see man strptime */
#ifndef __USE_XOPEN
#define __USE_XOPEN
#endif
#include <time.h>

#if defined(__MACH__) || defined(__linux__)
#include <sys/ioctl.h>
#include <sys/termios.h>
#include <stdio.h>
#endif

/*
static const char *suunto_model_names[]={
  "Unknown",
  "Spyder (old)",
  "Spyder (new)",
  "Cobra/Vyper (old)",
  "Vyper (new)",
  "Vytec",
  "Stinger",
  "Mosquito"
};
*/
/*
static void suunto_load_devices_liststore(GtkListStore *suunto_device_list_store, GtkComboBox *suunto_device_combobox)
{
  gchar **suunto_devices,*env;
  gint i=0;
  GtkTreeIter iter;

  suunto_device_list_store=gtk_list_store_new(1,G_TYPE_STRING);
  env=(gchar*)g_getenv("SUUNTO_DEVICE");
  if(env) {
    suunto_devices=g_strsplit(env,":",0);
    while(suunto_devices[i]) {
      gtk_list_store_append(GTK_LIST_STORE(suunto_device_list_store),&iter);
      gtk_list_store_set(GTK_LIST_STORE(suunto_device_list_store),&iter,0,suunto_devices[i],-1);
      i++;
    }
    g_strfreev(suunto_devices);
  }
  gtk_combo_box_set_model(suunto_device_combobox,GTK_TREE_MODEL(suunto_device_list_store));
  g_object_unref(suunto_device_list_store);
  gtk_combo_box_set_active(suunto_device_combobox,0);
}
*/
DeviceSuunto::DeviceSuunto(std::string name)
{
	filename = name;
	hCom = NULL;
	open();
}

DeviceSuunto::~DeviceSuunto()
{
	close();
}

#define SecureZeroMemory(p,s) RtlFillMemory((p),(s),0);
int DeviceSuunto::open()
{
#ifdef _WIN32
  //struct termios settings;
  bool fSuccess;
  DCB dcb;

  LOGINFO("Opening %s", filename.c_str());

  hCom = CreateFile(s2ws(filename).c_str(),
    GENERIC_READ | GENERIC_WRITE,
    0,
    NULL,
    OPEN_EXISTING,
    0,
    NULL
    );

   if (hCom == INVALID_HANDLE_VALUE)
   {
       //  Handle the error.
       hCom = NULL;
	   DBthrowError("CreateFile on %s failed with error %d.", filename.c_str(), GetLastError());
   }

   //  Initialize the DCB structure.
   SecureZeroMemory(&dcb, sizeof(DCB));
   dcb.DCBlength = sizeof(DCB);
   //  Build on the current configuration by first retrieving all current
   //  settings.
   fSuccess = GetCommState(hCom, &dcb);

   	dcb.BaudRate = CBR_2400;
	dcb.ByteSize = 8;
	dcb.Parity = ODDPARITY;
	dcb.StopBits = ONESTOPBIT;

  dcb.fBinary=1;
  dcb.fParity=1;
  dcb.fOutxCtsFlow=false;
  dcb.fOutxDsrFlow=false;
  dcb.fDtrControl=DTR_CONTROL_DISABLE;
  dcb.fDsrSensitivity=0;
  dcb.fTXContinueOnXoff=0;
  dcb.fRtsControl=RTS_CONTROL_DISABLE;

   fSuccess = SetCommState(hCom, &dcb);

   if (!fSuccess)
   {
		//  Handle the error.
		hCom = NULL;
		DBthrowError("SetCommState failed with error %d.", GetLastError());
   }


   try {
	   set_dtr(DTR_STATUS_ON);
	   set_rts(RTS_STATUS_OFF);
       Sleep(100);
	   return (0);
   } catch (...) {
	   hCom = NULL;
	   return (SUUNTO_ERR_SETSIG);
   }

#elif defined(__MACH__) || defined(__linux__)

	struct termios options;

	hCom = ::open(filename.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
	fcntl(hCom, F_SETFL, 0);

	/* get the current options */
	tcgetattr(hCom, &options);

	cfsetspeed(&options, B2400);

	/* set raw input, 1 second timeout */
	options.c_cflag = CS8|CREAD|PARENB|PARODD|CLOCAL;
	options.c_iflag = IGNBRK; /* Ignore parity checking */
	options.c_lflag=0;
	options.c_oflag=0;

	/* Setup blocking, return on 1 character */
	options.c_cc[VMIN] = 0;
	options.c_cc[VTIME] = 0;

	/* Clear the line */
	tcflush(hCom,TCIFLUSH);

	if(tcsetattr(hCom,TCSANOW,&options)!=-1) {
		//todo : should be in try/catch ?
		set_dtr(DTR_STATUS_ON);
		set_rts(RTS_STATUS_OFF);
		usleep(100000);
	}
	else {
		LOGINFO("Error while setting properties of Com port");
		return(-1);
	}

	unsigned char b;
	while(read_serial(&b,1,2)>0){}

#else
#error Platform not supported

#endif
	return(0);
}



/*static bool suunto_send_testcmd(char *cmd) {

  int len;
  DWORD  out;
  bool rval=false;

  len=strlen(cmd);

  rval = WriteFile(hCom, cmd, len, &out, NULL);
  FlushFileBuffers(hCom);

  return rval;
}*/


int DeviceSuunto::read_serial(unsigned char * buff, unsigned int num, int timeoutmod)
{
#ifdef _WIN32
  DWORD  out;
  bool rval;

  if (!hCom)
  {
	  int ret = open();
	  if (ret < 0) return(ret);
  }

 /*----------------------------------*/
 /*    Définition des timeouts       */
 /*----------------------------------*/
 COMMTIMEOUTS tTimeout;
 tTimeout.ReadIntervalTimeout = MAXWORD;
 tTimeout.ReadTotalTimeoutMultiplier = 0;
 tTimeout.ReadTotalTimeoutConstant = 10000; // pas de time out = 0
 tTimeout.WriteTotalTimeoutMultiplier = 0;
 tTimeout.WriteTotalTimeoutConstant = 0;

// configurer le timeout
 SetCommTimeouts(hCom,&tTimeout);

  rval = ReadFile(hCom, buff, 1, &out, NULL);
  if (rval && out > 0)
  {
	  Logger::binary("READ", buff, out);
	  return rval;
  }

#elif defined(__MACH__) || defined(__linux__)

	fd_set fds;
	struct timeval tv;
	int rval;

	FD_ZERO(&fds);
	FD_SET(hCom,&fds);
	tv.tv_sec = timeoutmod;
	tv.tv_usec = TIMEOUT;
	if(select(hCom+1,&fds,NULL,NULL,&tv)==1) {
		rval = read(hCom,buff,1);
		Logger::binary("READ", buff, 1);
		return rval;
	}

#else
#error Platform not supported
#endif

	return SUUNTO_ERR_READ;
}

int DeviceSuunto::write_serial(unsigned char *buffer,int len)
{
	int rc;

	set_rts(RTS_STATUS_ON);



#ifdef _WIN32
	DWORD  out;
	rc = WriteFile(hCom, buffer, len, &out, NULL);
	Logger::binary("WRITE", buffer, out);
	FlushFileBuffers(hCom);
	Sleep(200);

#elif defined(__MACH__) || defined(__linux__)
	rc = write(hCom,buffer,len);
	Logger::binary("WRITE", buffer, len);
	tcdrain(hCom);
	usleep(200000);

#else
#error Platform not supported
#endif

	set_rts(RTS_STATUS_OFF);
	return rc;
}

