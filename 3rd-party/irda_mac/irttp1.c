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
  
#include "irttp.h"  
  
static uint8_t ttp_connect(IrLAP_Context *context_p, IrTTP_Connect_Frame *resp_p,  
               uint8_t slsap_sel, uint8_t dlsap_sel)  
{  
  IrTTP_Connect_Frame req;  
  
  req.head.dlsap_sel=dlsap_sel | IRLMP_DLSAP_C_MASK;  
  req.head.slsap_sel=slsap_sel;  
  req.ctl.opcode=IRLMP_OP_CONNECT;  
  
  req.rsvd=0;  
  req.credit=IRTTP_CREDITS;  
  
  if(irlap1_send_receive_i_frame(context_p,  
                 (IrLAP_Frame *)&req, sizeof(IrTTP_Connect_Frame),  
                 (IrLAP_Frame *)resp_p)<sizeof(IrTTP_Connect_Frame))  
    return 0;  
  
  if(resp_p->ctl.opcode!=(IRLMP_OP_CONNECT | IRIAP_CTL_LST_MASK))  
    return 0;  
  
  return 1;  
}  
  
uint8_t irttp1_connect(IrLAP_Context *context_p, IrLAP_Frame *resp_p,  
               IrLAP_Device_Addr addr_p,  
               uint8_t irlmp_service_hint1,  
               char *ias_class_name, char *ias_lsap_name,  
               uint8_t slsap_sel, uint8_t *dlsap_sel_p)  
{  
  if(!irlmp1_connect(context_p, resp_p, addr_p,  
             irlmp_service_hint1,  
             ias_class_name, ias_lsap_name,  
             slsap_sel, dlsap_sel_p))  
    return 0;  
  
  return ttp_connect(context_p, (IrTTP_Connect_Frame *)resp_p, slsap_sel, *dlsap_sel_p);  
}  
  
int16_t irttp1_send_receive_i_frame(IrLAP_Context *context_p,  
                    IrTTP_Frame *req_p, uint16_t req_size,  
                    uint8_t slsap_sel, uint8_t dlsap_sel,  
                    IrLAP_Frame *resp_p)  
{  
  int16_t n;
  int16_t tries=0;
  req_p->head.credit=IRTTP_CREDITS;  
  return irlmp1_send_receive_i_frame(context_p,  
                     (IrLMP_Frame *)req_p, req_size,  
                     slsap_sel, dlsap_sel,  
                     resp_p);  

} 

int16_t irttp1_continue_receive_i_frame(IrLAP_Context *context_p,  
                    uint8_t slsap_sel, uint8_t dlsap_sel,  
                    IrLAP_Frame *resp_p)  
{  
  int16_t n;
  int16_t tries=0;
  n = irlmp1_continue_receive_i_frame(context_p,  
                     slsap_sel, dlsap_sel,  
                     resp_p);
  return(n);
} 


