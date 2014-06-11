/*
 * libdivecomputer
 *
 * Copyright (C) 2010 Jef Driesen
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

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "irda.h"
#include "irttp.h"
#include "irphy.h"

#include "context-private.h"
#include "device-private.h"


#define MY_LSAP_SEL_VAL 0x12


struct irda_t {
	int fd;
  IrLAP_Context ctx;  
  uint8_t dlsap_sel;  
	long timeout;
};

static IrLAP_Frame response_buffer;
static int16_t response_buffer_len=-1;
static int16_t response_buffer_pos = 0;

static unsigned long total_resp=0;
static unsigned long total_served=0;


int
irda_socket_open (irda_t **out, dc_context_t *context)
{
  uint8_t r;
	irda_t *irda;
  IrIAS_Node *ias_nodes[3];  
  
  r = irphy_open();
  if (r) return(-1);
  irda = malloc(sizeof(irda_t));
  if (!irda) return(-1);
  irda->fd = 1;
  printf("reset done\n");

  irlap_init_context(&(irda->ctx));  
  printf("irlap init done\n");

  IrLAP_Frame frame;  

  IrTTP_Connect_Frame ttp_resp;
  irda->dlsap_sel = 1;  
  if (!irttp1_connect (&(irda->ctx), &frame, 0/*addr_p*/, 0 /*unused: IRLMP_HINT1_COMM*/, "CLASSNAME", "LSAP_NAME", MY_LSAP_SEL_VAL, &(irda->dlsap_sel))){
    printf("irttp1_connect failed\n");
    irphy_close();
    irda->fd=-1;
    return(-1);
  }
  printf("irttp1_connect WORKED !!!\n");

  *out = irda;
  return 0;
}


int
irda_socket_close (irda_t *device)
{
  irphy_close();
	device->fd=-1;
	return 0;
}


int
irda_socket_set_timeout (irda_t *device, long timeout)
{
	return 0;
}


int
irda_socket_discover (irda_t *device, irda_callback_t callback, void *userdata)
{
	if (callback)
		callback(1, "UWATEC Galileo Sol", 0, 0, userdata);
	return 0;
}


int
irda_socket_connect_name (irda_t *device, unsigned int address, const char *name)
{
	return 0;
}


int
irda_socket_connect_lsap (irda_t *device, unsigned int address, unsigned int lsap)
{
	return 0;
}


int
irda_socket_available (irda_t *device)
{
	return response_buffer_len;
}


int
irda_socket_read (irda_t *device, void *data, unsigned int size)
{
	unsigned int to_copy, tries=0;
	int served = 0;

	printf("\n\nSIZE:%u\n", size);

	to_copy = 1; //not null

	while (served < size && tries < 10 ) {
		printf("Served: %u/%u  (last: %u)\nTry: %u/10\n", served, size, to_copy, tries);
		if (to_copy == 0){
			usleep(500000);
		}
		if (response_buffer_len == 0){
			if (irda_socket_write(device, "", 0) < 0) return(-1);
		}

		IrTTP_Frame *ttp_resp = (IrTTP_Frame*)&response_buffer;

		if (size >= response_buffer_len+served) {
			to_copy = response_buffer_len;
			memcpy(data+served, ttp_resp->info+response_buffer_pos, to_copy);
		  response_buffer_pos = 0;
		  response_buffer_len = 0;
		}
		else {
			to_copy = size-served;
			memcpy(data+served, ttp_resp->info+response_buffer_pos, to_copy);
		  response_buffer_pos += to_copy;
		  response_buffer_len -= to_copy;
		}
		served += to_copy;
		tries++;
	}
	printf("Served: %u/%u\nTry: %u/10\n", served, size, tries);
	total_served += served;
  return(served);
}


int
irda_socket_write (irda_t *device, const void *data, unsigned int size)
{
  IrLAP_Frame request;
  IrTTP_Frame *ttp_req = (IrTTP_Frame*)&request;
  uint16_t request_size;
  memcpy(ttp_req->info, data, size);
  request_size = sizeof(IrTTP_Frame)+size;
  printf("\n######################\n");
  response_buffer_len = irttp1_send_receive_i_frame(&(device->ctx), ttp_req, request_size, MY_LSAP_SEL_VAL, device->dlsap_sel, &response_buffer);
  response_buffer_pos = 0;
	response_buffer_len -= sizeof(IrTTP_Head);
	if (response_buffer_len < 0) response_buffer_len = 0;

	total_resp += response_buffer_len;
  printf(" >>>%d<<< %lu --> %lu\n", response_buffer_len, total_resp, total_served);

	return(size);
}

/*
int
irda_socket_read_continue (irda_t *device)
{
  IrLAP_Frame request;
  response_buffer_len = irttp1_continue_receive_i_frame(&(device->ctx), MY_LSAP_SEL_VAL, device->dlsap_sel, &response_buffer);
  response_buffer_pos = 0;
	response_buffer_len -= sizeof(IrTTP_Head);
	if (response_buffer_len < 0) response_buffer_len = 0;

	total_resp += response_buffer_len;
  printf(" >>>%d<<< %lu --> %lu\n", response_buffer_len, total_resp, total_served);

	return(response_buffer_len);
}

*/
