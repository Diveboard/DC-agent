/*
 *  DeviceMares.cpp
 *  FireBreath
 *
 *  Created by Pascal Manchon on 27/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DeviceMares.h"

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


DeviceMares::DeviceMares(std::string name)
{
	LOGDEBUG("Creating DeviceMares on %s", name.c_str());
	filename = name;
	hCom = NULL;
	open();
}

DeviceMares::~DeviceMares()
{
	close();
}

#define SecureZeroMemory(p,s) RtlFillMemory((p),(s),0);

int DeviceMares::open()
{
#ifdef _WIN32
	//struct termios settings;
	bool fSuccess;
	DCB dcb;

	LOGINFO("Opening %s", filename.c_str());

	//todo fix unicode
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
		LOGINFO ("CreateFile failed with error %d.", GetLastError());
		hCom = NULL;
		return (SUUNTO_ERR_CREATEFILE);
	}

	//  Initialize the DCB structure.
	SecureZeroMemory(&dcb, sizeof(DCB));
	dcb.DCBlength = sizeof(DCB);
	//  Build on the current configuration by first retrieving all current
	//  settings.
	fSuccess = GetCommState(hCom, &dcb);

   	dcb.BaudRate = CBR_9600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	dcb.fBinary=1;
	dcb.fParity=0;
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
		LOGINFO("SetCommState failed with error %d.", GetLastError());
		hCom = NULL;
		return (SUUNTO_ERR_SETCOMMSTATE);
	}

	PurgeComm(hCom, PURGE_RXABORT|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_TXCLEAR);

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

	cfsetspeed(&options, B9600);

	/* set raw input, 1 second timeout */
	options.c_iflag &= ~(IGNBRK | BRKINT | ISTRIP | INLCR | IGNCR | ICRNL);
	options.c_oflag &= ~(OPOST);
	options.c_lflag &= ~(ICANON | ECHO | ISIG | IEXTEN);

	options.c_cflag &= ~(PARENB | PARODD);
	options.c_iflag &= ~(IGNPAR | PARMRK | INPCK);
	options.c_iflag |= IGNPAR;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CRTSCTS;
	options.c_iflag &= ~(IXON | IXOFF | IXANY);

	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8|CREAD|CLOCAL;

	/* Setup blocking, return on 1 character */
	options.c_cc[VMIN] = 0;
	options.c_cc[VTIME] = 0;

	/* Clear the line */
	tcflush(hCom,TCIFLUSH);

	if(tcsetattr(hCom,(long)TCSANOW,&options)!=-1) {
		//todo : should be in a try/catch ?
		set_dtr(DTR_STATUS_ON);
		set_rts(RTS_STATUS_OFF);
		usleep(100000);
	}
	else {
		LOGINFO("Error while setting properties of Com port");
		return(-1);
	}

	unsigned char b;
    try {
        while(read_serial(&b,1,2)>0){}
    } catch(DBException e) {
    }

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


int DeviceMares::read_serial(unsigned char * buff, unsigned int num, int timeoutmod)
{
#ifdef _WIN32
	DWORD  out;
	bool rval;

	if (!hCom)
	{
		int ret = open();
		if (ret < 0) DBthrowError("Error while opening device : %d", ret);
	}

	/*----------------------------------*/
	/*    DÈfinition des timeouts       */
	/*----------------------------------*/
	COMMTIMEOUTS tTimeout;
	tTimeout.ReadIntervalTimeout = MAXWORD;
	tTimeout.ReadTotalTimeoutMultiplier = 0;
	tTimeout.ReadTotalTimeoutConstant = 2000; // pas de time out = 0
	tTimeout.WriteTotalTimeoutMultiplier = 0;
	tTimeout.WriteTotalTimeoutConstant = 0;

	// configurer le timeout
	SetCommTimeouts(hCom,&tTimeout);

	rval = ReadFile(hCom, buff, num, &out, NULL);
	if (rval && out > 0) return rval;

#elif defined(__MACH__) || defined(__linux__)

	fd_set fds;
	struct timeval tv;
	int rval;

	FD_ZERO(&fds);
	FD_SET(hCom,&fds);
	tv.tv_sec = 0;
	tv.tv_usec = TIMEOUT;
	int cr = select(hCom+1,&fds,NULL,NULL,&tv);
	if (cr <  0) DBthrowError("Error while reading Mares device");
	if (cr == 0) DBthrowError("Timeout while reading Mares device");
	else {
		rval = read(hCom,buff,num);
		return rval;
	}

#else
#error Platform not supported
#endif

	DBthrowError("Error while reading Mares device");
	return(-1);
}

int DeviceMares::write_serial(unsigned char *buffer,int len)
{
	int rc;

	set_rts(RTS_STATUS_ON);

#ifdef _WIN32
	DWORD  out;
	rc = WriteFile(hCom, buffer, len, &out, NULL);
	FlushFileBuffers(hCom);
	Sleep(300);

#elif defined(__MACH__) || defined(__linux__)
	tcflush(hCom,TCIFLUSH);
	rc = write(hCom,buffer,len);
	tcdrain(hCom);
	usleep(300000);

#else
#error Platform not supported
#endif

	set_rts(RTS_STATUS_OFF);
	return rc;
}
