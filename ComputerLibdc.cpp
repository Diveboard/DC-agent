#include "ComputerLibdc.h"
#include "stdafx.h"
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <tchar.h>
#endif

#ifdef __MACH__
#include <dlfcn.h>
#endif

#ifdef __linux__
#include <dlfcn.h>
#endif

#include "Logger.h"

#define MAX_TEMP 99999

#include <boost/thread/thread.hpp>

#include "DiveAgent.h"
#include "Global.h"

namespace {
  boost::mutex  _m;
};


#ifdef WIN32

HRESULT RegGetString(HKEY hKey, LPCTSTR szValueName, LPTSTR * lpszResult) {

  // Given a HKEY and value name returns a string from the registry.
  // Upon successful return the string should be freed using free()
  // eg. RegGetString(hKey, TEXT("my value"), &szString);

  DWORD dwType=0, dwDataSize=0, dwBufSize=0;
  LONG lResult;

  // Incase we fail set the return string to null...
  if (lpszResult != NULL) *lpszResult = NULL;

  // Check input parameters...
  if (hKey == NULL || lpszResult == NULL) return E_INVALIDARG;

  // Get the length of the string in bytes (placed in dwDataSize)...
  lResult = RegQueryValueEx(hKey, szValueName, 0, &dwType, NULL, &dwDataSize );

  // Check result and make sure the registry value is a string(REG_SZ)...
  if (lResult != ERROR_SUCCESS) return HRESULT_FROM_WIN32(lResult);
  else if (dwType != REG_SZ)    return DISP_E_TYPEMISMATCH;

  // Allocate memory for string - We add space for a null terminating character...
  dwBufSize = dwDataSize + (1 * sizeof(TCHAR));
  *lpszResult = (LPTSTR)malloc(dwBufSize);

  if (*lpszResult == NULL) return E_OUTOFMEMORY;

  // Now get the actual string from the registry...
  lResult = RegQueryValueEx(hKey, szValueName, 0, &dwType, (LPBYTE) *lpszResult, &dwDataSize );

  // Check result and type again.
  // If we fail here we must free the memory we allocated...
  if (lResult != ERROR_SUCCESS) { free(*lpszResult); return HRESULT_FROM_WIN32(lResult); }
  else if (dwType != REG_SZ)    { free(*lpszResult); return DISP_E_TYPEMISMATCH; }

  // We are not guaranteed a null terminated string from RegQueryValueEx.
  // Explicitly null terminate the returned string...
  (*lpszResult)[(dwBufSize / sizeof(TCHAR)) - 1] = TEXT('\0');

  return NOERROR;
}

#endif


static void logger_proxy(dc_context_t *context, dc_loglevel_t loglevel, const char *file, unsigned int line, const char *function, const char *message, void *userdata)
{
  const char *short_file  = file;

  for (const char *curs = file; *curs != 0 && curs-file < 1000; curs++)
    if ((*curs == '/' || *curs == '\\' ) && *(curs+1) != 0)
      short_file = curs+1;

  Logger::appendL((unsigned int)line, short_file, "LDC", "In %s: %s", function, message);
}


LIBTYPE openDLLLibrary()
{
#ifdef WIN32
  std::wstring dll_path = s2ws(DiveAgent::exeFolder());
  LOGINFO("Searching DLL at %s", ws2s(dll_path).c_str());

  SetDllDirectory(dll_path.c_str());

  HINSTANCE libdc = LoadLibrary(L"libdivecomputer-0.dll");
  if (!libdc)
  {
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    std::string msg = str(boost::format("Error loading DLL at %s : %d - %s") % ws2s(dll_path).c_str() % dw % (char*)lpMsgBuf);

    LocalFree(lpMsgBuf);

    DBthrowError(msg);
  }

  return libdc;
#elif defined(__MACH__) 

  void *libdc;
  std::string library_name = DiveAgent::exeFolder() + "libdivecomputer.dylib";
  libdc = dlopen(library_name.c_str(),RTLD_LAZY);
  if (!libdc) DBthrowError("Impossible to load library : %s", dlerror());

  return libdc;


#elif defined(__linux__)

  void *libdc;
  std::string library_name = std::string(LIBDIVE_SO) + "/libdivecomputer.so";
  libdc = dlopen(library_name.c_str(),RTLD_LAZY);
  if (!libdc)
  {
    std::string library_name = DiveAgent::exeFolder() + "/libdivecomputer.so";
    libdc = dlopen(library_name.c_str(),RTLD_LAZY);
    if (!libdc)
      DBthrowError("Impossible to load library : %s", dlerror());
  }
  return libdc;


#else
#error "not supported"
#endif
}

void closeDLLLibrary(LIBTYPE &lib)
{
#if defined(__MACH__) || defined(__linux__)
  if (!dlclose(lib))
    lib=NULL;
#elif _WIN32
  if (FreeLibrary(lib))
    lib=NULL;
#else
#error "Not supported"
#endif
}




void *getDLLFunction(LIBTYPE libdc, const char *function)
{
  void *ptr;

#ifdef WIN32
  ptr = reinterpret_cast<void*>(GetProcAddress(libdc, function));
#elif defined(__MACH__) || defined(__linux__)
  ptr = dlsym(libdc, function);
#else
#error "OS not supported"
#endif

  if (!ptr)
    DBthrowError("Error fetching DLL pointer to %s", function);

  return(ptr);
}

static void fillDLLPointers(LIBTYPE libdc, libdivecomputer_t *libdc_p)
{
  libdc_p->buffer_append = reinterpret_cast<typeof(&dc_buffer_append)>(getDLLFunction(libdc, "dc_buffer_append"));
  libdc_p->buffer_free = reinterpret_cast<typeof(&dc_buffer_free)>(getDLLFunction(libdc, "dc_buffer_free"));
  libdc_p->buffer_new = reinterpret_cast<typeof(&dc_buffer_new)>(getDLLFunction(libdc, "dc_buffer_new"));
  libdc_p->buffer_get_data = reinterpret_cast<typeof(&dc_buffer_get_data)>(getDLLFunction(libdc, "dc_buffer_get_data"));
  libdc_p->buffer_get_size = reinterpret_cast<typeof(&dc_buffer_get_size)>(getDLLFunction(libdc, "dc_buffer_get_size"));
  libdc_p->context_free = reinterpret_cast<typeof(&dc_context_free)>(getDLLFunction(libdc, "dc_context_free"));
  libdc_p->context_new = reinterpret_cast<typeof(&dc_context_new)>(getDLLFunction(libdc, "dc_context_new"));
  libdc_p->context_set_logfunc = reinterpret_cast<typeof(&dc_context_set_logfunc)>(getDLLFunction(libdc, "dc_context_set_logfunc"));
  libdc_p->context_set_loglevel = reinterpret_cast<typeof(&dc_context_set_loglevel)>(getDLLFunction(libdc, "dc_context_set_loglevel"));
  libdc_p->descriptor_free = reinterpret_cast<typeof(&dc_descriptor_free)>(getDLLFunction(libdc, "dc_descriptor_free"));
  libdc_p->descriptor_get_model = reinterpret_cast<typeof(&dc_descriptor_get_model)>(getDLLFunction(libdc, "dc_descriptor_get_model"));
  libdc_p->descriptor_get_product = reinterpret_cast<typeof(&dc_descriptor_get_product)>(getDLLFunction(libdc, "dc_descriptor_get_product"));
  libdc_p->descriptor_get_type = reinterpret_cast<typeof(&dc_descriptor_get_type)>(getDLLFunction(libdc, "dc_descriptor_get_type"));
  libdc_p->descriptor_get_vendor = reinterpret_cast<typeof(&dc_descriptor_get_vendor)>(getDLLFunction(libdc, "dc_descriptor_get_vendor"));
  libdc_p->descriptor_iterator = reinterpret_cast<typeof(&dc_descriptor_iterator)>(getDLLFunction(libdc, "dc_descriptor_iterator"));
  libdc_p->device_close = reinterpret_cast<typeof(&dc_device_close)>(getDLLFunction(libdc, "dc_device_close"));
  libdc_p->device_dump = reinterpret_cast<typeof(&dc_device_dump)>(getDLLFunction(libdc, "dc_device_dump"));
  libdc_p->device_foreach = reinterpret_cast<typeof(&dc_device_foreach)>(getDLLFunction(libdc, "dc_device_foreach"));
  libdc_p->device_open = reinterpret_cast<typeof(&dc_device_open)>(getDLLFunction(libdc, "dc_device_open"));
  libdc_p->device_set_cancel = reinterpret_cast<typeof(&dc_device_set_cancel)>(getDLLFunction(libdc, "dc_device_set_cancel"));
  libdc_p->device_set_events = reinterpret_cast<typeof(&dc_device_set_events)>(getDLLFunction(libdc, "dc_device_set_events"));
  libdc_p->iterator_free = reinterpret_cast<typeof(&dc_iterator_free)>(getDLLFunction(libdc, "dc_iterator_free"));
  libdc_p->iterator_next = reinterpret_cast<typeof(&dc_iterator_next)>(getDLLFunction(libdc, "dc_iterator_next"));
  libdc_p->parser_destroy = reinterpret_cast<typeof(&dc_parser_destroy)>(getDLLFunction(libdc, "dc_parser_destroy"));
  libdc_p->parser_get_datetime = reinterpret_cast<typeof(&dc_parser_get_datetime)>(getDLLFunction(libdc, "dc_parser_get_datetime"));
  libdc_p->parser_get_field = reinterpret_cast<typeof(&dc_parser_get_field)>(getDLLFunction(libdc, "dc_parser_get_field"));
  libdc_p->parser_new = reinterpret_cast<typeof(&dc_parser_new)>(getDLLFunction(libdc, "dc_parser_new"));
  libdc_p->parser_samples_foreach = reinterpret_cast<typeof(&dc_parser_samples_foreach)>(getDLLFunction(libdc, "dc_parser_samples_foreach"));
  libdc_p->parser_set_data = reinterpret_cast<typeof(&dc_parser_set_data)>(getDLLFunction(libdc, "dc_parser_set_data"));
  libdc_p->iostream_close = reinterpret_cast<typeof(&dc_iostream_close)>(getDLLFunction(libdc, "dc_iostream_close"));
  libdc_p->serial_open = reinterpret_cast<typeof(&dc_serial_open)>(getDLLFunction(libdc, "dc_serial_open"));
  libdc_p->bluetooth_open = reinterpret_cast<typeof(&dc_bluetooth_open)>(getDLLFunction(libdc, "dc_bluetooth_open"));
  libdc_p->bluetooth_iterator_new = reinterpret_cast<typeof(&dc_bluetooth_iterator_new)>(getDLLFunction(libdc, "dc_bluetooth_iterator_new"));
  libdc_p->bluetooth_device_get_address = reinterpret_cast<typeof(&dc_bluetooth_device_get_address)>(getDLLFunction(libdc, "dc_bluetooth_device_get_address"));
  libdc_p->bluetooth_device_get_name = reinterpret_cast<typeof(&dc_bluetooth_device_get_name)>(getDLLFunction(libdc, "dc_bluetooth_device_get_name"));
  libdc_p->bluetooth_device_free = reinterpret_cast<typeof(&dc_bluetooth_device_free)>(getDLLFunction(libdc, "dc_bluetooth_device_free"));
  libdc_p->bluetooth_addr2str = reinterpret_cast<typeof(&dc_bluetooth_addr2str)>(getDLLFunction(libdc, "dc_bluetooth_addr2str"));
  libdc_p->bluetooth_str2addr = reinterpret_cast<typeof(&dc_bluetooth_str2addr)>(getDLLFunction(libdc, "dc_bluetooth_str2addr"));
}


struct sample_cb_data {
  std::string sampleXML;
  unsigned int duration;
  double max_depth;
  double min_temp;
};


/*
 * libdivecomputer
 *
 * Copyright (C) 2009 Jef Driesen
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */
#include <string.h>
#include <signal.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif

#ifdef _MSC_VER
#define snprintf _snprintf
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

#ifdef _WIN32
#define DC_TICKS_FORMAT "%I64d"
#else
#define DC_TICKS_FORMAT "%lld"
#endif

static const char *g_cachedir = NULL;
//static int g_cachedir_read = 1;


const char *
errmsg (dc_status_t rc)
{
  switch (rc) {
  case DC_STATUS_SUCCESS:
    return "Success";
  case DC_STATUS_UNSUPPORTED:
    return "Unsupported operation";
  case DC_STATUS_INVALIDARGS:
    return "Invalid arguments";
  case DC_STATUS_NOMEMORY:
    return "Out of memory";
  case DC_STATUS_NODEVICE:
    return "No device found";
  case DC_STATUS_NOACCESS:
    return "Access denied";
  case DC_STATUS_IO:
    return "Input/output error";
  case DC_STATUS_TIMEOUT:
    return "Timeout";
  case DC_STATUS_PROTOCOL:
    return "Protocol error";
  case DC_STATUS_DATAFORMAT:
    return "Data format error";
  case DC_STATUS_CANCELLED:
    return "Cancelled";
  default:
    return "Unknown error";
  }
}

volatile sig_atomic_t g_cancel = 0;

void
sighandler (int signum)
{
#ifndef _WIN32
  // Restore the default signal handler.
  signal (signum, SIG_DFL);
#endif

  boost::lock_guard<boost::mutex> g(_m);
  g_cancel = 1;
}

static int
cancel_cb (void *userdata)
{
  return g_cancel;
}

void sample_cb (dc_sample_type_t type, dc_sample_value_t value, void *userdata)
{
  try {
    static const char *events[] = {
      "none", "DECO", "rbt", "ASCENT", "CEILING", "workload", "transmitter",
      "violation", "bookmark", "surface", "safety stop", "gaschange",
      "safety stop (voluntary)", "safety stop (mandatory)", "deepstop",
      "ceiling (safety stop)", "unknown", "divetime", "maxdepth",
      "OLF", "PO2", "airtime", "rgbm", "heading", "tissue level warning",
      "gaschange2"};
    static const char *decostop[] = {
      "ndl", "deco", "deep", "safety"};

    unsigned int nsamples = 0;
    struct sample_cb_data *data = (struct sample_cb_data *) userdata;
    std::string vendor;

    switch (type) {
    case DC_SAMPLE_TIME:
      LOGDEBUG("Parsing element of type DC_SAMPLE_TIME");
      nsamples++;
      if (data->duration < value.time) data->duration = value.time;
      data->sampleXML += str(boost::format("<t>%02u</t>") % value.time);
      break;
    case DC_SAMPLE_DEPTH:
      LOGDEBUG("Parsing element of type DC_SAMPLE_DEPTH");
      data->sampleXML += str(boost::format("<d>%.2f</d>") % value.depth);
      if (data->max_depth < value.depth) data->max_depth = value.depth;
      break;
    case DC_SAMPLE_PRESSURE:
      LOGDEBUG("Parsing element of type DC_SAMPLE_PRESSURE");
      //todo
      data->sampleXML += str(boost::format("<pressure tank=\"%u\">%.2f</pressure>") %  value.pressure.tank % value.pressure.value);
      break;
    case DC_SAMPLE_TEMPERATURE:
      LOGDEBUG("Parsing element of type DC_SAMPLE_TEMPERATURE");
      if (data->min_temp < value.temperature) data->min_temp = value.temperature;
      data->sampleXML += str(boost::format("<temperature>%.2f</temperature>") % value.temperature);
      break;
    case DC_SAMPLE_EVENT:
      LOGDEBUG("Parsing element of type DC_SAMPLE_EVENT");
      data->sampleXML += str(boost::format("<ALARM type=\"%u\" time=\"%u\" flags=\"%u\" value=\"%u\">%s</ALARM>")
        % value.event.type % value.event.time % value.event.flags % value.event.value % events[value.event.type]);
      break;
    case DC_SAMPLE_RBT:
      LOGDEBUG("Parsing element of type DC_SAMPLE_RBT");
      data->sampleXML += str(boost::format("<rbt>%u</rbt>") % value.rbt);
      break;
    case DC_SAMPLE_HEARTBEAT:
      LOGDEBUG("Parsing element of type DC_SAMPLE_HEARTBEAT");
      data->sampleXML += str(boost::format("<heartbeat>%u</heartbeat>") % value.heartbeat);
      break;
    case DC_SAMPLE_BEARING:
      LOGDEBUG("Parsing element of type DC_SAMPLE_BEARING");
      data->sampleXML += str(boost::format("<bearing>%u</bearing>") % value.bearing);
      break;
    case DC_SAMPLE_VENDOR:
      LOGDEBUG("Parsing element of type DC_SAMPLE_VENDOR");
      vendor += str(boost::format("vendor type=\"%u\" size=\"%u\"") % value.vendor.type % value.vendor.size);
      for (unsigned int i = 0; i < value.vendor.size; ++i)
        vendor += str(boost::format("%02X") % (((unsigned char *) value.vendor.data)[i]));
      LOGINFO(vendor);
      break;
    case DC_SAMPLE_SETPOINT:
      LOGDEBUG("Parsing element of type DC_SAMPLE_SETPOINT");
      data->sampleXML += str(boost::format("<setpoint>%.2f</setpoint>") % value.setpoint);
      break;
    case DC_SAMPLE_PPO2:
      LOGDEBUG("Parsing element of type DC_SAMPLE_PPO2");
      data->sampleXML += str(boost::format("<ppo2>%.2f</ppo2>") % value.ppo2);
      break;
    case DC_SAMPLE_CNS:
      LOGDEBUG("Parsing element of type DC_SAMPLE_CNS");
      data->sampleXML += str(boost::format("<cns>%.2f</cns>") % value.cns);
      break;
    case DC_SAMPLE_DECO:
      LOGDEBUG("Parsing element of type DC_SAMPLE_DECO");
      data->sampleXML += str(boost::format("<deco time=\"%u\" depth=\"%.2f\">%s</deco>") % value.deco.time % value.deco.depth % decostop[value.deco.type]);
      break;
    case DC_SAMPLE_GASMIX:
      LOGDEBUG("Parsing element of type DC_SAMPLE_GASMIX");
      data->sampleXML += str(boost::format("<gasmix>%u</gasmix>") % value.gasmix);
      break;
    default:
      LOGWARNING("Received an element of unknown type '%d'", type);
      break;
    }
  } catch (std::exception &e) {
    DBthrowError("Error in sample_cb : %s", e.what());
  } catch (...) {
    DBthrowError("Error in sample_cb");
  }
}



dc_status_t ComputerLibdc::doparse (const unsigned char data[], unsigned int size)
{
  // Create the parser.
  LOGINFO("starting doparse");
  dc_status_t rc = DC_STATUS_SUCCESS;

  dc_parser_t *parser = NULL;

  try {
    LOGINFO("Creating the correct parser");
    dc_status_t rc = libdc_p.parser_new (&parser, device);
    if (rc != DC_STATUS_SUCCESS) {
      LOGWARNING("Error creating the parser : %d", rc);
      return rc;
    }

    // Register the data.
    LOGINFO("Registering the data.");
    rc = libdc_p.parser_set_data (parser, data, size);
    if (rc != DC_STATUS_SUCCESS) {
      LOGDEBUG("Error registering the data");
      libdc_p.parser_destroy (parser);
      DBthrowError(str(boost::format("Error registering the data - Error code : %1%") % rc));
    }

    // Parse the datetime.
    LOGINFO("Parsing the datetime.");
    dc_datetime_t dt = {0};
    rc = libdc_p.parser_get_datetime (parser, &dt);
    if (rc != DC_STATUS_SUCCESS && rc != DC_STATUS_UNSUPPORTED) {
      LOGDEBUG("Error parsing the datetime.");
      libdc_p.parser_destroy (parser);
      DBthrowError(str(boost::format("Error parsing the datetime - Error code : %1%") % rc));
    }

    *out += str(boost::format("<DATE>%04i-%02i-%02i</DATE><TIME>%02i:%02i:%02i</TIME>")
      % dt.year % dt.month % dt.day
      % dt.hour % dt.minute % dt.second);

    // Parse the gas mixes.
    LOGINFO("Parsing the gas mixes.");
    unsigned int ngases = 0;
    rc = libdc_p.parser_get_field (parser, DC_FIELD_GASMIX_COUNT, 0, &ngases);
    if (rc != DC_STATUS_SUCCESS && rc != DC_STATUS_UNSUPPORTED) {
        LOGDEBUG ("Error parsing the gas mix count.");
        libdc_p.parser_destroy (parser);
        DBthrowError(str(boost::format("Error parsing the gas mix count - Error code : %1%") % rc));
    }

    LOGDEBUG ("Gas mixes found: %u", ngases);
    if (ngases > 0) *out += "<gases>";

    for (unsigned int i = 0; i < ngases; ++i) {
      dc_gasmix_t gasmix = {0};
      rc = libdc_p.parser_get_field (parser, DC_FIELD_GASMIX, i, &gasmix);
      if (rc != DC_STATUS_SUCCESS && rc != DC_STATUS_UNSUPPORTED) {
        LOGDEBUG ("Error parsing the gas mix.");
        libdc_p.parser_destroy (parser);
        DBthrowError(str(boost::format("Error parsing the gas mix - Error code : %1%") % rc));
      }

      *out += str(boost::format(
        "<mix><mixname>%u</mixname><o2>%.3f</o2><n2>%.3f</n2><he>%.3f</he></mix>")
        % i
        % (gasmix.oxygen)
        % (gasmix.nitrogen)
        % (gasmix.helium));
    }

    if (ngases > 0) *out += "</gases>";


    // Parse the salinity.
    LOGINFO ("Parsing the salinity.");
    dc_salinity_t salinity = {DC_WATER_FRESH, 0.0};
    rc = libdc_p.parser_get_field (parser, DC_FIELD_SALINITY, 0, &salinity);
    if (rc != DC_STATUS_SUCCESS && rc != DC_STATUS_UNSUPPORTED) {
      LOGDEBUG ("Error parsing the salinity.");
      libdc_p.parser_destroy (parser);
      DBthrowError(str(boost::format("Error parsing the salinity - Error code : %1%") % rc));
    }

    if (rc != DC_STATUS_UNSUPPORTED) {
      *out += str(boost::format("<salinity type=\"%u\">%.1f</salinity>") % salinity.type % salinity.density);
    }


    // Parse the atmospheric pressure.
    LOGINFO ("Parsing the atmospheric pressure.");
    double atmospheric = 0.0;
    rc = libdc_p.parser_get_field (parser, DC_FIELD_ATMOSPHERIC, 0, &atmospheric);
    if (rc != DC_STATUS_SUCCESS && rc != DC_STATUS_UNSUPPORTED) {
      LOGDEBUG ("Error parsing the atmospheric pressure.");
      libdc_p.parser_destroy (parser);
      DBthrowError(str(boost::format("Error parsing the atmospheric pressure - Error code : %1%") % rc));
    }

    if (rc != DC_STATUS_UNSUPPORTED) {
      *out += str(boost::format("<atmospheric>%.5f</atmospheric>") % atmospheric);
    }


    // Initialize the sample data.

    // Parse the sample data.
    LOGINFO("Parsing the sample data.");
    //LDCPARSERSAMPLESFOREACH* parser_samples_foreach = reinterpret_cast<LDCPARSERSAMPLESFOREACH*>(GetProcAddress(libdc, "parser_samples_foreach"));

    struct sample_cb_data cbd;
    cbd.sampleXML = "";
    cbd.duration = 0;
    cbd.max_depth = 0;
    cbd.min_temp = MAX_TEMP;
    rc = libdc_p.parser_samples_foreach (parser, sample_cb, &cbd);
    if (rc != DC_STATUS_SUCCESS) {
      LOGDEBUG("Error parsing the sample data.");
      libdc_p.parser_destroy (parser);
      DBthrowError(str(boost::format("Error parsing the sample data - Error code : %1%") % rc));
    }

    //adding calculated data
    //*out += str(boost::format("<MAXDEPTH???>%02u</MAXDEPTH???>") % cbd.max_depth);
    *out += str(boost::format("<DURATION>%02u</DURATION>") % cbd.duration);
    if (cbd.min_temp < MAX_TEMP) *out += str(boost::format("<TEMPERATURE>%.2f</TEMPERATURE>") % cbd.min_temp);

    *out += "<SAMPLES>";
    *out += cbd.sampleXML;
    *out += "</SAMPLES>";

  } catch(std::exception &e) {
    LOGWARNING("Caught Exception : %s",e.what());
    LOGINFO("Destroying the parser.");
    rc = libdc_p.parser_destroy (parser);
    if (rc != DC_STATUS_SUCCESS) {
      LOGINFO("WARNING - Error destroying the parser - Error %d", rc);
    }
    throw;
  } catch(...) {
    LOGINFO("Destroying the parser.");
    rc = libdc_p.parser_destroy (parser);
    if (rc != DC_STATUS_SUCCESS) {
      LOGINFO("WARNING - Error destroying the parser - Error %d", rc);
    }
    DBthrowError("Caught Exception Unknown");
  }

  // Destroy the parser.
  LOGINFO("Destroying the parser.");
  rc = libdc_p.parser_destroy (parser);
  if (rc != DC_STATUS_SUCCESS) {
    LOGINFO("WARNING - Error destroying the parser - Error %d", rc);
  }

  return DC_STATUS_SUCCESS;
}



static void event_cb (dc_device_t *device, dc_event_type_t event, const void *data, void *userdata)
{
  try {
    boost::lock_guard<boost::mutex> g(_m);

    const dc_event_progress_t *progress = (dc_event_progress_t *) data;
    const dc_event_devinfo_t *devinfo = (dc_event_devinfo_t *) data;
    const dc_event_clock_t *clock = (dc_event_clock_t *) data;
    const dc_event_vendor_t *vendor = (dc_event_vendor_t *) data;

    event_data_t *evdata = (event_data_t *)userdata;

    device_data_t *devdata = evdata->devdata;

    LOGDEBUG("Received event of type '%d'", event);

    switch (event) {
    case DC_EVENT_WAITING:
      LOGDEBUG("Event: waiting for user action");
      break;
    case DC_EVENT_PROGRESS:
      if (progress->maximum >0)
      {
        LOGDEBUG("Event: progress %3.2f%% (%u/%u)",
                 100.0 * (double) progress->current / (double) progress->maximum,
                 progress->current, progress->maximum);

        evdata->status->percent = 100.0 * (double) progress->current / (double) progress->maximum;
      }
      else
      {
        LOGWARNING("Event: progress negative or zero !");
      }
      break;
    case DC_EVENT_DEVINFO:
      devdata->devinfo = *devinfo;
      LOGINFO("Event: model=%u (0x%08x), firmware=%u (0x%08x), serial=%u (0x%08x)",
        devinfo->model, devinfo->model,
        devinfo->firmware, devinfo->firmware,
        devinfo->serial, devinfo->serial);
      //todo handle fingerprints
      /*if (g_cachedir && g_cachedir_read) {
        dc_buffer_t *fingerprint = fpread (g_cachedir, devdata->backend, devinfo->serial);
        device_set_fingerprint (device,
          dc_buffer_get_data (fingerprint),
          dc_buffer_get_size (fingerprint));
        dc_buffer_free (fingerprint);
      }*/
      break;
    case DC_EVENT_CLOCK:
      devdata->clock = *clock;
      LOGDEBUG("Event: systime=" DC_TICKS_FORMAT ", devtime=%u",
        clock->systime, clock->devtime);
      break;
    case DC_EVENT_VENDOR:
      LOGDEBUG ("Event: vendor=");
      for (unsigned int i = 0; i < vendor->size; ++i)
        LOGDEBUG ("%02X", vendor->data[i]);
      break;
    default:
      LOGWARNING("unsupported kind of event received '%d'", event);
      break;
    }

  LOGDEBUG("End of event handling");

  } catch(std::exception &e) {
    LOGWARNING("Caught Exception : %s",e.what());
    throw;
  } catch(...) {
    DBthrowError("Caught Exception Unknown");
  }
}

int G_dive_cb (const unsigned char *data, unsigned int size, const unsigned char *fingerprint, unsigned int fsize, void *userdata)
{
  ComputerLibdc *computer = (ComputerLibdc *) userdata;

  return(computer->dive_cb (data, size, fingerprint, fsize));
}

int ComputerLibdc::dive_cb (const unsigned char *data, unsigned int size, const unsigned char *fgprint, unsigned int fsize)
{
  try {
    number++;

    LOGINFO("Dive: number=%u, size=%u", number, size);
    for (unsigned int i = 0; i < fsize; ++i)
      LOGINFO("fingerprint byte %d :%02X", i, fgprint[i]);

    //update status
    status.nbDivesRead = number;

    if (number == 1) {
      fingerprint = libdc_p.buffer_new (fsize);
      libdc_p.buffer_append (fingerprint, fgprint, fsize);
    }

    *out += "<DIVE><PROGRAM><fingerprint>";
    for (unsigned int i = 0; i < fsize; ++i)
        *out += str(boost::format( "%02X") % (int)(fgprint[i]));
    *out += "</fingerprint></PROGRAM>";

    LOGINFO("Parsing the data");
    doparse (data, size);
    LOGINFO("Done with parsing the data");

    *out += "</DIVE>";

  } catch(std::exception &e) {
    LOGWARNING("Caught Exception : %s",e.what());
    throw;
  } catch(...) {
    DBthrowError("Caught Exception Unknown");
  }

  return 1;
}




dc_status_t ComputerLibdc::search (std::string stdname)
{
  dc_status_t rc = DC_STATUS_SUCCESS;
  const char *name = stdname.c_str();

  dc_iterator_t *iterator = NULL;
  rc = libdc_p.descriptor_iterator (&iterator);
  if (rc != DC_STATUS_SUCCESS) {
    LOGWARNING ("Error creating the device descriptor iterator.");
    return rc;
  }

  dc_descriptor_t *l_descriptor = NULL, *current = NULL;
  while ((rc = libdc_p.iterator_next (iterator, &l_descriptor)) == DC_STATUS_SUCCESS) {
    const char *vendor = libdc_p.descriptor_get_vendor (l_descriptor);
    const char *product = libdc_p.descriptor_get_product (l_descriptor);

    size_t n = strlen (vendor);
    if (strncasecmp (name, vendor, n) == 0 && name[n] == ' ' &&
      strcasecmp (name + n + 1, product) == 0)
    {
      current = l_descriptor;
      break;
    } else if (strcasecmp (name, product) == 0) {
      current = l_descriptor;
      break;
    }

    libdc_p.descriptor_free (l_descriptor);
  }

  if (rc != DC_STATUS_SUCCESS && rc != DC_STATUS_DONE) {
    libdc_p.descriptor_free (current);
    libdc_p.iterator_free (iterator);
    LOGWARNING ("Error iterating the device descriptors.");
    return rc;
  }

  libdc_p.iterator_free (iterator);

  descriptor = current;

  return DC_STATUS_SUCCESS;
}



void ComputerLibdc::dowork (std ::string *diveXML, std::string *dumpData)
{
  LOGINFO("Starting dowork");
  dc_status_t rc = DC_STATUS_SUCCESS;

  // Initialize the device data.
  device_data_t devdata;

    // Open the device.
    LOGINFO ("Opening the device (%s %s, %s).\n",
      libdc_p.descriptor_get_vendor (descriptor),
      libdc_p.descriptor_get_product (descriptor),
      devname.empty() ? devname.c_str() : "null");

		if (devname.compare(0, 5, "/dev/") == 0) {
		  rc = libdc_p.serial_open (&iostream, context, devname.c_str());
		  if (rc != DC_STATUS_SUCCESS) {
		    LOGWARNING ("Error opening I/O stream.");
		    DBthrowError(std::string("Error opening I/O stream - Error code : ") + errmsg(rc));
		  }
		}
		else {
		  dc_bluetooth_address_t address = 0;
		  address = libdc_p.bluetooth_str2addr(devname.c_str());
		  if (address == 0) {
		    LOGWARNING ("Invalid device address.");
		    DBthrowError(std::string("Invalid device address:") + devname.c_str());
		  }
		  rc = libdc_p.bluetooth_open (&iostream, context, address, 0);
		  if (rc != DC_STATUS_SUCCESS) {
		    LOGWARNING ("Error opening I/O stream.");
		    DBthrowError(std::string("Error opening I/O stream - Error code : ") + errmsg(rc));
		  }
		}
    
    rc = libdc_p.device_open (&device, context, descriptor, iostream);
    if (rc != DC_STATUS_SUCCESS) {
      LOGWARNING ("Error opening device.");
      libdc_p.iostream_close (iostream);
      DBthrowError(std::string("Error opening device - Error code : ") + errmsg(rc));
    }

    //Register the event handler.
    LOGINFO("Registering the event handler.");
    int events = DC_EVENT_WAITING | DC_EVENT_PROGRESS | DC_EVENT_DEVINFO | DC_EVENT_CLOCK | DC_EVENT_VENDOR;
    event_data_t evdata;
    evdata.devdata = &devdata;
    evdata.status = &status;
    rc = libdc_p.device_set_events (device, events, event_cb, &evdata);
    if (rc != DC_STATUS_SUCCESS) {
      LOGDEBUG("Error registering the event handler.");
      libdc_p.device_close (device);
      libdc_p.iostream_close (iostream);
      DBthrowError("Error registering the event handler ");
    }

    // Register the cancellation handler.
    LOGINFO("Registering the cancellation handler.");
    rc = libdc_p.device_set_cancel (device, cancel_cb, NULL);
    if (rc != DC_STATUS_SUCCESS) {
      LOGINFO("Error registering the cancellation handler.");
      libdc_p.device_close (device);
      libdc_p.iostream_close (iostream);
      DBthrowError("Error registering the cancellation handler ");
    }

    // todo Register the fingerprint data.
    /*if (fingerprint) {
      LOGINFO("Registering the fingerprint data.");
      rc = device_set_fingerprint (device, dc_buffer_get_data (fingerprint), dc_buffer_get_size (fingerprint));
      if (rc != DEVICE_STATUS_SUCCESS) {
        LOGINFO("Error registering the fingerprint data.");
        device_close (device);
        libdc_p.iostream_close (iostream);
        return rc;
      }
    }
    */


    if (dumpData) {
      //Dump the data
      // Allocate a memory buffer.
      dc_buffer_t *buffer = libdc_p.buffer_new (0);

      // Download the memory dump.
      LOGINFO ("Downloading the memory dump.");
      rc = libdc_p.device_dump (device, buffer);
      if (rc != DC_STATUS_SUCCESS) {
        LOGINFO ("Error downloading the memory dump");
        libdc_p.buffer_free (buffer);
        libdc_p.device_close (device);
        libdc_p.iostream_close (iostream);
        DBthrowError("Error downloading the memory dump");
      }

      unsigned char *data_p = libdc_p.buffer_get_data(buffer);
      unsigned long max_i = libdc_p.buffer_get_size(buffer);
      dumpData->resize(max_i*2);
      for (unsigned long i = 0; i < max_i; i++){
        unsigned char c = data_p[i];
        if ((c>>4) >= 10)
          dumpData->at(2*i) = 'A' - 10 + (c>>4);
        else
          dumpData->at(2*i) = '0' + (c>>4);

        if ((c & 0x0F) >= 10)
          dumpData->at(2*i+1) = 'A' - 10 + (c & 0x0F);
        else
          dumpData->at(2*i+1) = '0' + (c & 0x0F);
      }

      // Free the memory buffer.
      libdc_p.buffer_free (buffer);
    }

    if (diveXML) {
      // Initialize the dive data.
      //dive_data_t divedata = {0};
      //divedata.devdata = devdata;
      //divedata.device = device;
      fingerprint = NULL;
      number = 0;
      out = diveXML;
      //divedata.computer = this;

      diveXML->append("<profile udcf='1'><device><model>");
      // TODO ???????diveXML->append(lookup_name(backend));
      diveXML->append("</model></device><REPGROUP>");

      // Download the dives.
      LOGINFO("Downloading the dives.");

      //required for slow-startup connections like bt ...
      int retries = 5;
      do {
        retries--;
        rc = libdc_p.device_foreach (device, G_dive_cb, this);
      } while (rc != DC_STATUS_SUCCESS && retries);

      if (rc != DC_STATUS_SUCCESS) {
        LOGDEBUG("Error downloading the dives.");
        libdc_p.buffer_free (fingerprint);
        libdc_p.device_close (device);
        libdc_p.iostream_close (iostream);
        DBthrowError(std::string("Error opening device - Error code : ") + errmsg(rc));
      }

      // todo Store the fingerprint data.
      /*if (g_cachedir) {
        fpwrite (divedata.fingerprint, g_cachedir, devdata.backend, devdata.devinfo.serial);
      }*/

      // todo Free the fingerprint buffer.
      //dc_buffer_free (divedata.fingerprint);

      diveXML->append("</REPGROUP></profile>");
    }

    // Close the device.
    LOGINFO("Closing the device.");
    rc = libdc_p.device_close (device);
    if (rc != DC_STATUS_SUCCESS)
      LOGWARNING("Error closing the device. %s", errmsg(rc));
    rc = libdc_p.iostream_close (iostream);
    if (rc != DC_STATUS_SUCCESS)
      LOGWARNING("Error closing the I/O stream. %s", errmsg(rc));
}









int ComputerLibdc::_get_all_dives(std::string &diveXML)
{
  g_cancel = 0;

  LOGINFO(str(boost::format("Using backend on %1%") % devname));
  try
  {
    status.state = COMPUTER_RUNNING;
    dowork (&diveXML, NULL);
  }
  catch (...) {
    LOGINFO("Caught Exception in _get_all_dives");
    status.state = COMPUTER_FINISHED;
    throw;
  }

  status.state = COMPUTER_FINISHED;
  return(0);
}




int ComputerLibdc::_dump(std::string &data)
{
  g_cancel = 0;

  LOGINFO(str(boost::format("Using backend on %1%") % devname));
  try
  {
    status.state = COMPUTER_RUNNING;
    dowork (NULL, &data);
  }
  catch (...) {
    LOGINFO("Caught Exception in _dump");
    status.state = COMPUTER_FINISHED;
    throw;
  }

  status.state = COMPUTER_FINISHED;
  return(0);
}





ComputerLibdc::ComputerLibdc(std::string type, std::string file)
{
  LOGINFO("Creating ComputerLibdc");
  dc_status_t rc;

  status.state = COMPUTER_NOT_STARTED;

  LOGDEBUG("Opening LibDiveComputer");
  libdc = openDLLLibrary();
  fillDLLPointers(libdc, &libdc_p);


  LOGDEBUG("Trying to load backend for the asked computer");

  if (type.compare(0, 4, "LDC "))
    DBthrowError(std::string("You are not asking for a LDC device"));

  /* Search for a matching device descriptor. */
  rc = search (type.substr(4));
  if (rc != DC_STATUS_SUCCESS) {
    DBthrowError(std::string("Error searching for device : ") + errmsg(rc));
  }

  /* Fail if no device descriptor found. */
  if (descriptor == NULL) {
    DBthrowError(std::string("No matching device found."));
  }

  rc = libdc_p.context_new (&context);
  if (rc != DC_STATUS_SUCCESS) {
    DBthrowError(std::string("Error allocating context : ") + errmsg(rc));
  }


  LOGDEBUG("Setting up logger for libdivecomputer");
  if (Logger::logLevel.compare("DEBUG") == 0)
    libdc_p.context_set_loglevel(context, DC_LOGLEVEL_DEBUG);
  else if (Logger::logLevel.compare("INFO") == 0)
    libdc_p.context_set_loglevel(context, DC_LOGLEVEL_INFO);
  else if (Logger::logLevel.compare("WARNING") == 0)
    libdc_p.context_set_loglevel(context, DC_LOGLEVEL_WARNING);
  else if (Logger::logLevel.compare("ERROR") == 0)
    libdc_p.context_set_loglevel(context, DC_LOGLEVEL_ERROR);
  else if (Logger::logLevel.compare("CRITICAL") == 0)
    libdc_p.context_set_loglevel(context, DC_LOGLEVEL_ERROR);
  else
    libdc_p.context_set_loglevel(context, DC_LOGLEVEL_WARNING);
  libdc_p.context_set_logfunc(context, logger_proxy, NULL);


  LOGINFO(str(boost::format("Using type %1% on %2%") % type % file));
  devname = file;

  //message_set_logfile("d:\\temp\\libdc.log");
  //return COMPUTER_MODEL_UNKNOWN;
}


ComputerLibdc::~ComputerLibdc(void)
{
  libdc_p.descriptor_free (descriptor);
  libdc_p.context_free (context);
  //TODO???? message_set_logfile (NULL);
  closeDLLLibrary(libdc);
}



ComputerStatus ComputerLibdc::get_status()
{
  ComputerStatus res;
  {
    boost::lock_guard<boost::mutex> g(_m);
    res = status;
  }
  return res;
}

void ComputerLibdc::cancel()
{
  g_cancel = 1;
}

std::vector<ComputerSupport> *ComputerLibdc::support_list = NULL;

std::vector<ComputerSupport> *ComputerLibdc::support()
{
  if (support_list)
    return(support_list);

  dc_status_t rc = DC_STATUS_SUCCESS;
  LIBTYPE libdc;
  libdivecomputer_t libdc_p;

  LOGDEBUG("Opening LibDiveComputer");
  libdc = openDLLLibrary();
  fillDLLPointers(libdc, &libdc_p);

  support_list = new std::vector<ComputerSupport>;

  dc_iterator_t *iterator = NULL;
  rc = libdc_p.descriptor_iterator (&iterator);
  if (rc != DC_STATUS_SUCCESS) {
    DBthrowError("Error creating the device descriptor iterator.");
  }

  dc_descriptor_t *l_descriptor = NULL, *current = NULL;
  while ((rc = libdc_p.iterator_next (iterator, &l_descriptor)) == DC_STATUS_SUCCESS) {
    ComputerSupport sup;
    const char *make    = libdc_p.descriptor_get_vendor (l_descriptor);
    const char *model   = libdc_p.descriptor_get_product (l_descriptor);
    dc_family_t family  = libdc_p.descriptor_get_type (l_descriptor);

    sup.make      += make;
    sup.model     += model;
    sup.key_code  += "LDC ";
    sup.key_code  += sup.label();

#ifdef _WIN32
    switch(family)
    {
      case DC_FAMILY_SUUNTO_VYPER:
      case DC_FAMILY_SUUNTO_VYPER2:
      case DC_FAMILY_SUUNTO_D9:
        sup.ports.push_back("Suunto USB Serial Port");
        sup.ports.push_back("Prolific USB-to-Serial Comm Port");
        break;
      case DC_FAMILY_UWATEC_SMART:
        sup.ports.push_back(NO_PORT_NEEDED);
        break;
      case DC_FAMILY_OCEANIC_VTPRO:
      case DC_FAMILY_OCEANIC_VEO250:
      case DC_FAMILY_OCEANIC_ATOM2:
        sup.ports.push_back("2002 Design, Inc. USB Download Interface");
        break;
      case DC_FAMILY_MARES_NEMO:
      case DC_FAMILY_MARES_DARWIN:
        sup.ports.push_back("Silicon Labs CP210x USB to UART Bridge");
        sup.ports.push_back("CP210x USB to UART Bridge Controller");
        sup.ports.push_back("CP210x USB to UART Bridge");
        break;
      case DC_FAMILY_MARES_ICONHD:
        sup.ports.push_back("ICON HD COM");
        break;
    }
#elif defined(__MACH__) || defined(__linux__)
    switch(family)
    {
      case DC_FAMILY_SUUNTO_VYPER:
      case DC_FAMILY_SUUNTO_VYPER2:
      case DC_FAMILY_SUUNTO_D9:
      case DC_FAMILY_OCEANIC_VTPRO:
      case DC_FAMILY_OCEANIC_VEO250:
        sup.ports.push_back("tty.usbserial-PtTFP8W4");
        sup.ports.push_back("tty.usbserial-00004006");
        sup.ports.push_back("tty.usbserial-A60073WV");
        sup.ports.push_back("tty.usbserial-ST000001");
        sup.ports.push_back("tty.usbserial-00001004");
        sup.ports.push_back("tty.usbserial-0000101D");
        sup.ports.push_back("tty.usbserial-000013FA");
        sup.ports.push_back("tty.usbserial-000013FD");
        sup.ports.push_back("tty.usbserial-00004006");
        sup.ports.push_back("tty.usbserial-20030001");
        sup.ports.push_back("tty.usbserial-AE00BS2L");
        sup.ports.push_back("tty.usbserial-ST000001");
        sup.ports.push_back("tty.usbserial-STUBR4LH");
        sup.ports.push_back("tty.usbserial-STUFHKD3");
        sup.ports.push_back("tty.usbserial-STV66B8Y");
        break;
      case DC_FAMILY_UWATEC_SMART:
        sup.ports.push_back(NO_PORT_NEEDED);
        break;
      case DC_FAMILY_MARES_NEMO:
      case DC_FAMILY_MARES_DARWIN:
        sup.ports.push_back("tty.SLAB_USBtoUART");
        break;
      case DC_FAMILY_MARES_ICONHD:
        sup.ports.push_back("tty.usbmodem01234561");
        break;
      default:
        break;
    }
#else
#error Platform not supported
#endif

    support_list->push_back(sup);

    libdc_p.descriptor_free (l_descriptor);
  }

  if (rc != DC_STATUS_SUCCESS && rc != DC_STATUS_DONE) {
    libdc_p.descriptor_free (current);
    libdc_p.iterator_free (iterator);
    DBthrowError ("Error iterating the device descriptors.");
  }

  libdc_p.iterator_free (iterator);


  LOGDEBUG("Closing LibDiveComputer");
  closeDLLLibrary(libdc);

  return(support_list);
}


std::vector<BluetoothDevice> *ComputerLibdc::btdevice_list = NULL;

std::vector<BluetoothDevice> *ComputerLibdc::btscan(bool rescan)
{
  if (!rescan)
    return(btdevice_list);

  dc_status_t rc = DC_STATUS_SUCCESS;
  LIBTYPE libdc;
  libdivecomputer_t libdc_p;

  LOGDEBUG("Opening LibDiveComputer");
  libdc = openDLLLibrary();
  fillDLLPointers(libdc, &libdc_p);

  std::vector<BluetoothDevice> *btdevice_list_tmp = new std::vector<BluetoothDevice>;

	dc_context_t *context;
  rc = libdc_p.context_new (&context);
  if (rc != DC_STATUS_SUCCESS) {
    DBthrowError(std::string("Error allocating context : ") + errmsg(rc));
  }

  dc_iterator_t *iterator = NULL;
  rc = libdc_p.bluetooth_iterator_new (&iterator, context, NULL);
  if (rc != DC_STATUS_SUCCESS) {
    DBthrowError("Error creating the device descriptor iterator.");
  }

  dc_bluetooth_device_t *device = NULL;
  dc_bluetooth_address_t address = 0;
  char buffer[DC_BLUETOOTH_SIZE];
  while ((rc = libdc_p.iterator_next (iterator, &device)) == DC_STATUS_SUCCESS) {
    address = libdc_p.bluetooth_device_get_address(device);
    libdc_p.bluetooth_addr2str(address, buffer, sizeof(buffer));

    BluetoothDevice btd;
    const char *name   = libdc_p.bluetooth_device_get_name(device);

    btd.address = std::string(buffer);
    btd.name    = std::string(name);

		//printf("Found %s at %s\n", btd.name.c_str(), btd.address.c_str());
    btdevice_list_tmp->push_back(btd);

    libdc_p.bluetooth_device_free(device);
  }

  if (rc != DC_STATUS_SUCCESS && rc != DC_STATUS_DONE) {
    libdc_p.iterator_free (iterator);
    DBthrowError ("Error iterating the device descriptors.");
  }

  libdc_p.iterator_free (iterator);
  libdc_p.context_free (context);

  LOGDEBUG("Closing LibDiveComputer");
  closeDLLLibrary(libdc);

	if (btdevice_list) delete(btdevice_list);
	btdevice_list = btdevice_list_tmp;
	btdevice_list_tmp = NULL;

  return(btdevice_list);
}

