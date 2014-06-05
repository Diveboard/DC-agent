/* 
  Copyright (C) 2002-2003 Gerd Rausch, BlauLogic (http://blaulogic.com) 
  All rights reserved. 
 
  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met: 
 
  1. Redistributions of source code must retain the above copyright 
     notice, this list of conditions and the following disclaimer. 
 
  2. Redistributions in binary form must reproduce the above copyright 
     notice, this list of conditions and the following disclaimer in the 
     documentation and/or other materials provided with the distribution. 
 
  3. Except as contained in this notice, neither the name of BlauLogic 
     nor the name(s) of the author(s) may be used to endorse or promote 
     products derived from this software without specific prior written 
     permission. 
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
  IN NO EVENT SHALL THE AUTHOR(S) OR BLAULOGIC BE LIABLE FOR ANY CLAIM, 
  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
  THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
*/  
  
#include <string.h>  
#include <termios.h>
  
#include "irlap.h"  
#include "irphy.h"  
  
#define IRLAP_TIMEOUT1          500  
#define IRLAP_TIMEOUT2          20  
  
#define IRLAP_PSEUDO_RANDOM_DEV_ADDR    0x18021967  
  
#define IRLAP_PAR_BAUD_RATE_VAL     0x3f //all <= 115200 //DIVEBOARD 0x02 /* 9600 bps */  
#define IRLAP_PAR_MAX_TURN_TIME_VAL 0x01 /* 500 ms */  
#define IRLAP_PAR_DATA_SIZE_VAL     ((IRLAP_DATA_SIZE-1)>>5)  
#define IRLAP_PAR_WINDOW_SIZE_VAL   0x01 /* 1 frame */  
#define IRLAP_PAR_ADD_BOFS_VAL      0x80 /* 0 BOFs */  
#define IRLAP_PAR_MIN_TURN_TIME_VAL 0x80 /* 0 ms */  
#define IRLAP_PAR_DISC_TIME_VAL     0x01 /* 3 s */  
  
#define IRLAP_XBOF_COUNT        10  
  
#define IRLAP_XBOF          0xFF  
#define IRLAP_BOF           0xC0  
#define IRLAP_EOF           0xC1  
#define IRLAP_CE            0x7D  
#define IRLAP_TRANS         0x20  
#define IRLAP_FCS_INIT          0xFF  
#define IRLAP_FCS_GOOD0         0xB8  
#define IRLAP_FCS_GOOD1         0xF0  
  
static inline void send_byte(uint8_t v)  
{  
  if(v==IRLAP_BOF || v==IRLAP_EOF || v==IRLAP_CE) {  
    irphy_send(IRLAP_CE);  
    irphy_send(v^IRLAP_TRANS);  
  } else  
    irphy_send(v);  
}  
  
static IrLAP_Neg_Param *set_neg_param_1(IrLAP_Neg_Param *param_p, uint8_t id, uint8_t value)  
{  
  param_p->id=id;  
  param_p->len=1;  
  param_p->value[0]=value;  
  
  return (IrLAP_Neg_Param *)((uint8_t *)param_p+sizeof(IrLAP_Neg_Param)+param_p->len);  
}  

speed_t irlap_get_neg_baud(IrLAP_Neg_Param *param_p_neg, size_t param_size){
  size_t pos;
  uint16_t param_val;
  uint8_t param_id, param_len;
  uint8_t *param_p = (uint8_t*)param_p_neg;

  for(pos=0;pos < param_size;){
    param_id = param_p[pos];
    param_len = param_p[pos+1];
    if (param_len == 2)
      param_val = *(uint16_t*)(param_p+pos+2);
    else
      param_val = *(param_p+pos+2) << 8;
    if (param_id == IRLAP_PAR_BAUD_RATE) break;
    pos += 2+param_len;
  }
  if (pos >= param_size)
    return(B9600);

  if (param_val & 0x2000) return(B115200);
  if (param_val & 0x1000) return(B57600);
  if (param_val & 0x0800) return(B38400);
  if (param_val & 0x0400) return(B19200);
  if (param_val & 0x0200) return(B9600);
  if (param_val & 0x0100) return(B2400);
  return(B9600);
}

uint16_t irlap_setup_connection(IrLAP_Neg_Param *param_p, size_t param_size)
{
  speed_t bauds;
  bauds = irlap_get_neg_baud(param_p, param_size);
  //printf("Setting up connection speed to %lu\n", bauds);
  return(irphy_set_baud(bauds));
}
  
uint16_t irlap_append_neg_params(uint8_t *info_p)  
{  
  IrLAP_Neg_Param *param_p;  
  
  param_p=(IrLAP_Neg_Param *)info_p;  
  
  param_p=set_neg_param_1(param_p, IRLAP_PAR_BAUD_RATE, IRLAP_PAR_BAUD_RATE_VAL);  
  param_p=set_neg_param_1(param_p, IRLAP_PAR_MAX_TURN_TIME, IRLAP_PAR_MAX_TURN_TIME_VAL);  
  param_p=set_neg_param_1(param_p, IRLAP_PAR_DATA_SIZE, IRLAP_PAR_DATA_SIZE_VAL);  
  param_p=set_neg_param_1(param_p, IRLAP_PAR_WINDOW_SIZE, IRLAP_PAR_WINDOW_SIZE_VAL);  
  param_p=set_neg_param_1(param_p, IRLAP_PAR_ADD_BOFS, IRLAP_PAR_ADD_BOFS_VAL);  
  param_p=set_neg_param_1(param_p, IRLAP_PAR_MIN_TURN_TIME, IRLAP_PAR_MIN_TURN_TIME_VAL);  
  param_p=set_neg_param_1(param_p, IRLAP_PAR_DISC_TIME, IRLAP_PAR_DISC_TIME_VAL);  
  
  return (uint8_t *)param_p-info_p;  
}  
  
uint8_t irlap_matching_dest_addr(IrLAP_Context *context_p, IrLAP_Device_Addr addr_p)  
{  
  return  
    memcmp(addr_p, context_p->dev_addr, sizeof(IrLAP_Device_Addr))==0 ||  
    (addr_p[0]==0xFF && addr_p[1]==0xFF && addr_p[2]==0xFF && addr_p[3]==0xFF);  
}  
  
void irlap_init_context(IrLAP_Context *context_p)  
{  
  context_p->dev_addr[0]=(uint8_t)IRLAP_PSEUDO_RANDOM_DEV_ADDR;  
  context_p->dev_addr[1]=(uint8_t)(IRLAP_PSEUDO_RANDOM_DEV_ADDR>>8);  
  context_p->dev_addr[2]=(uint8_t)(IRLAP_PSEUDO_RANDOM_DEV_ADDR>>16);  
  context_p->dev_addr[3]=(uint8_t)(IRLAP_PSEUDO_RANDOM_DEV_ADDR>>24);  
  context_p->conn_addr=IRLAP_ADDR_NULL;  
  context_p->vr=0;  
  context_p->vs=0;  
}  
  
uint8_t irlap_send_frame(IrLAP_Frame *frame_p, uint16_t size)  
{  
  uint8_t r = irphy_send_frame((void*)frame_p, size);
  if (r) return(r);
  //printf("\nSENDING (%lu): ", size);
  uint16_t i;
  /* DIVEBOARD
  for (i=0; i<size; i++)
    printf("%.2x ", ((uint8_t*) frame_p)[i]);
  printf("\n");
  uint16_t i;  
  uint8_t fcs0, fcs1, v;  
  
  for(i=0; i<IRLAP_XBOF_COUNT; i++)  
    irphy_send(IRLAP_XBOF);  
  
  irphy_send(IRLAP_BOF);  
  
  fcs0=IRLAP_FCS_INIT;  
  fcs1=IRLAP_FCS_INIT;  
  for(i=0; i<size; i++) {  
    v=((uint8_t *)frame_p)[i];  
    send_byte(v);  
  
    v^=fcs0;  
    fcs0=fcs1;  
    fcs1=v;  
  
    fcs1^=fcs1<<4;  
    fcs0^=fcs1>>4;  
    fcs0^=fcs1<<3;  
    fcs1^=fcs1>>5;  
  }  
  
  send_byte(~fcs0);  
  send_byte(~fcs1);  
  
  irphy_send(IRLAP_EOF);
  */
}  
  
int16_t irlap_receive_frame(IrLAP_Frame *frame_p)  
{  
  int16_t n;  
  uint8_t fcs0, fcs1, v;  
  uint8_t r, received;
  do {  
    if(!irphy_wait(IRLAP_TIMEOUT1))  
      return -1;  
    r = irphy_receive(&received);
    if (r) return(-2);
  } while(received!=IRLAP_BOF);  

  //printf("\nRECEIVED: ");

  fcs0=IRLAP_FCS_INIT;  
  fcs1=IRLAP_FCS_INIT;  
  n=0;  
  while(n<sizeof(IrLAP_Frame)) {  
    if(!irphy_wait(IRLAP_TIMEOUT2))  
      return -1;  
    r=irphy_receive(&v);  
    if (r) return(-2);
  

    if(v==IRLAP_BOF) {  
      if(n>0)  
        return -1;  
      continue;  
    }  
  
    if(v==IRLAP_EOF) { 
      if(n<2 || fcs0!=IRLAP_FCS_GOOD0 || fcs1!=IRLAP_FCS_GOOD1)  
    return -1;  
      //printf("\n");
      return n-2;  
    }  
  
    if(v==IRLAP_CE) {  
      if(!irphy_wait(IRLAP_TIMEOUT2))  
    return -1;  
      r=irphy_receive(&v);
      v=v ^ IRLAP_TRANS;  
      if (r) return(-2);
    }  
  
    //printf("%.2x ", v);

    ((uint8_t *)frame_p)[n++]=v;  
  
    v^=fcs0;  
    fcs0=fcs1;  
    fcs1=v;  
  
    fcs1^=fcs1<<4;  
    fcs0^=fcs1>>4;  
    fcs0^=fcs1<<3;  
    fcs1^=fcs1>>5;  
  }  
  
  return -1;  
}  
