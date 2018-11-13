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
  
#include "irlmp.h"  
  
static inline uint8_t iap_append_name(uint8_t *dest_p, char *name)  
{  
  uint8_t len;  
  
  len=strlen(name);  
  
  dest_p[0]=len;  
  memcpy(dest_p+1, name, len);  
  
  return len+1;  
}  
  
static uint8_t iap_getvaluebyclass_int_result(uint16_t *dest_p, uint8_t *info_p, uint16_t n)  
{  
  uint16_t i;  
  
  i=0;  
  
  if(i>=n ||  
     info_p[i++]!=IRIAP_RESP_GVBCL_SUCCESS)  
    return 0;  
  
  /* len */  
  if(i+1>=n ||  
     (info_p[i]==0 && info_p[i+1]==0))  
    return 0;  
  
  /* seek beyond len & obj_id */  
  i+=4;  
  
  if(i+4>=n ||  
     info_p[i++]!=IRIAP_VALUE_TYPE_INTEGER)  
    return 0;  
  
  /* assert MSBs are zero */  
  if(info_p[i] || info_p[i+1])  
    return 0;  
  
  *dest_p=  
    (uint16_t)info_p[i+2]<<8 |  
    (uint16_t)info_p[i+3];  
  
  return 1;  
}  
  
static uint8_t lmp_connect(IrLAP_Context *context_p, IrLMP_Frame *resp_p,  
               char *ias_class_name, char *ias_lsap_name,  
               uint8_t slsap_sel, uint8_t *dlsap_sel_p)  
{  
  IrLAP_Small_Frame frame;  
  IrLMP_Frame *req_p=(IrLMP_Frame *)&frame;  
  int16_t n;  
  uint16_t dlsap_sel;  
  
  //DIVEBOARD req_p->head.dlsap_sel=IRLMP_DLSAP_C_MASK;  
  req_p->head.dlsap_sel=*dlsap_sel_p | IRLMP_DLSAP_C_MASK;  
  req_p->head.slsap_sel=slsap_sel;  
  req_p->u.ctl.opcode=IRLMP_OP_CONNECT;  
  
  if((n=irlap1_send_receive_i_frame(context_p,  
                    (IrLAP_Frame *)req_p, sizeof(IrLMP_Head)+sizeof(IrLMP_Ctl),  
                    (IrLAP_Frame *)resp_p))<sizeof(IrLMP_Head)+sizeof(IrLMP_Ctl))  
    return 0;  
  
  //printf("first comm LMP_CONNECT received...\n");

  if(resp_p->u.ctl.opcode!=(IRLMP_OP_CONNECT | IRLMP_OP_A_MASK))  
    return 0;  

  //printf("first comm LMP_CONNECT WORKED\n");
  
  /*DIVEBOARD
  //DIVEBOARD req_p->head.dlsap_sel=0;  
  req_p->head.dlsap_sel=1;  

  req_p->head.slsap_sel=slsap_sel;  
  req_p->u.ctl.opcode=IRLMP_OP_LM_GETVALUEBYCLASS | IRIAP_CTL_LST_MASK;  
  
  n=iap_append_name(req_p->u.ctl.info, ias_class_name);  
  n+=iap_append_name(req_p->u.ctl.info+n, ias_lsap_name);  
  
  if((n=irlap1_send_receive_i_frame(context_p,  
                    (IrLAP_Frame *)req_p, sizeof(IrLMP_Head)+sizeof(IrLMP_Ctl)+n,  
                    (IrLAP_Frame *)resp_p))<sizeof(IrLMP_Head)+sizeof(IrLMP_Ctl))  
    return 0;  
  
  printf("second comm LMP_CONNECT received...\n");

  if(resp_p->u.ctl.opcode!=(IRLMP_OP_LM_GETVALUEBYCLASS | IRIAP_CTL_LST_MASK))  
    return 0;  

  printf("second comm LMP_CONNECT seems to be good\n");
    
  if(!iap_getvaluebyclass_int_result(&dlsap_sel, resp_p->u.ctl.info, n-sizeof(IrLMP_Head)-sizeof(IrLMP_Ctl)))  
    return 0;  
  *dlsap_sel_p=dlsap_sel;  

  printf("second comm LMP_CONNECT WORKED\n");
  */

  //DIVEBOARD
  *dlsap_sel_p=1;  

  
  return 1;  
}  
  
uint8_t irlmp1_connect(IrLAP_Context *context_p, IrLAP_Frame *resp_p,  
               IrLAP_Device_Addr addr_p,  
               uint8_t service_hint1,  
               char *ias_class_name, char *ias_lsap_name,  
               uint8_t slsap_sel, uint8_t *dlsap_sel_p)  
{  
  if(!irlap1_connect(context_p, resp_p, addr_p,  
             irlmp_get_dev_info, service_hint1))  
    return 0;  
  
  return lmp_connect(context_p, (IrLMP_Frame *)resp_p,  
             ias_class_name, ias_lsap_name,  
             slsap_sel, dlsap_sel_p);  
}  
  
int16_t irlmp1_send_receive_i_frame(IrLAP_Context *context_p,  
                    IrLMP_Frame *req_p, uint16_t req_size,  
                    uint8_t slsap_sel, uint8_t dlsap_sel,  
                    IrLAP_Frame *resp_p)  
{  
  req_p->head.dlsap_sel=dlsap_sel;  
  req_p->head.slsap_sel=slsap_sel;  
  
  return irlap1_send_receive_i_frame(context_p,  
                     (IrLAP_Frame *)req_p, req_size,  
                     resp_p);  
}  

  
int16_t irlmp1_continue_receive_i_frame(IrLAP_Context *context_p,  
                    uint8_t slsap_sel, uint8_t dlsap_sel,  
                    IrLAP_Frame *resp_p)  
{  
  return irlap1_continue_receive_i_frame(context_p,  
                     resp_p);  
}  



