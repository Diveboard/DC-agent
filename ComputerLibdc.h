#pragma once
#include "Computer.h"
#include <libdivecomputer/context.h>
#include <libdivecomputer/device.h>
#include <libdivecomputer/parser.h>
#include <libdivecomputer/buffer.h>
#include <libdivecomputer/serial.h>
#include <libdivecomputer/bluetooth.h>

#ifdef WIN32
#define typeof decltype
#endif

#ifdef WIN32
#define LIBTYPE HINSTANCE
#elif defined(__MACH__) || defined(__linux__)
#define __cdecl
#define LIBTYPE void*
#endif


#define DEVICE_TYPE_MARES_DARWIN_AIR (device_type_t)999

//local types
typedef struct device_data_t {
	dc_family_t backend;
	dc_event_devinfo_t devinfo;
	dc_event_clock_t clock;
} device_data_t;

typedef struct libdivecomputer_t{
	typeof(&dc_buffer_append) buffer_append;
	typeof(&dc_buffer_free) buffer_free;
	typeof(&dc_buffer_new) buffer_new;
	typeof(&dc_buffer_get_data) buffer_get_data;
	typeof(&dc_buffer_get_size) buffer_get_size;
	typeof(&dc_context_free) context_free;
	typeof(&dc_context_new) context_new;
	typeof(&dc_context_set_logfunc) context_set_logfunc;
	typeof(&dc_context_set_loglevel) context_set_loglevel;
	typeof(&dc_descriptor_free) descriptor_free;
	typeof(&dc_descriptor_get_model) descriptor_get_model;
	typeof(&dc_descriptor_get_product) descriptor_get_product;
	typeof(&dc_descriptor_get_type) descriptor_get_type;
	typeof(&dc_descriptor_get_vendor) descriptor_get_vendor;
	typeof(&dc_descriptor_iterator) descriptor_iterator;
	typeof(&dc_device_close) device_close;
	typeof(&dc_device_dump) device_dump;
	typeof(&dc_device_foreach) device_foreach;
	typeof(&dc_device_open) device_open;
	typeof(&dc_device_set_cancel) device_set_cancel;
	typeof(&dc_device_set_events) device_set_events;
	typeof(&dc_iterator_free) iterator_free;
	typeof(&dc_iterator_next) iterator_next;
	typeof(&dc_parser_destroy) parser_destroy;
	typeof(&dc_parser_get_datetime) parser_get_datetime;
	typeof(&dc_parser_get_field) parser_get_field;
	typeof(&dc_parser_new) parser_new;
	typeof(&dc_parser_samples_foreach) parser_samples_foreach;
	typeof(&dc_parser_set_data) parser_set_data;
	typeof(&dc_iostream_close) iostream_close;
	typeof(&dc_serial_open) serial_open;
	typeof(&dc_bluetooth_open) bluetooth_open;
	typeof(&dc_bluetooth_iterator_new) bluetooth_iterator_new;
	typeof(&dc_bluetooth_device_get_address) bluetooth_device_get_address;
	typeof(&dc_bluetooth_device_get_name) bluetooth_device_get_name;
	typeof(&dc_bluetooth_device_free) bluetooth_device_free;
	typeof(&dc_bluetooth_addr2str) bluetooth_addr2str;
	typeof(&dc_bluetooth_str2addr) bluetooth_str2addr;
} libdivecomputer_t;

//For DLL loading
typedef struct {
  std::string address;
  std::string name;
} BluetoothDevice;

class ComputerLibdc : public Computer
{
protected:
	std::string devname;
	dc_context_t *context;
	dc_descriptor_t *descriptor;
	ComputerStatus status;
	LIBTYPE libdc;
	libdivecomputer_t libdc_p;
	void dowork (std::string *diveXML, std::string *dumpData);
	dc_status_t doparse (const unsigned char data[], unsigned int size);
	dc_status_t search (std::string stdname);

	unsigned int number;
	dc_buffer_t *fingerprint;
	std::string *out;
	dc_device_t *device;
	dc_iostream_t *iostream;

	static std::vector<ComputerSupport> *support_list;
	static std::vector<BluetoothDevice> *btdevice_list;

public:
	int dive_cb (const unsigned char *data, unsigned int size, const unsigned char *fingerprint, unsigned int fsize);
	ComputerLibdc(std::string type, std::string filename);
	virtual ~ComputerLibdc(void);
	int _get_all_dives(std::string &xml);
	int _dump(std::string &data);
	virtual ComputerStatus get_status(); // thread save
	virtual void cancel(); // thread save
  static std::vector<ComputerSupport> *support();
  static std::vector<BluetoothDevice> *btscan(bool rescan);
};


/*
typedef struct dive_data_t {
	device_data_t *devdata;
	dc_device_t *device;
	//FILE* fp;
	std::string *out;
	unsigned int number;
	dc_buffer_t *fingerprint;
	ComputerStatus *status;
	ComputerLibdc *computer;
} dive_data_t;
*/


typedef struct event_data_t {
	ComputerStatus *status;
	device_data_t *devdata;
} event_data_t;

