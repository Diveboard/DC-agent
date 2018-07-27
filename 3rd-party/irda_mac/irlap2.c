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
  
#include "irlap.h"  
  
void irlap2_send_i_frame(IrLAP_Context *context_p, IrLAP_Frame *frame_p, uint16_t size)  
{  
  frame_p->head.addr=context_p->conn_addr;  
  frame_p->head.ctrl=IRLAP_CTRL_P_F_MASK | context_p->vr<<IRLAP_CTRL_NR_BIT_POS | context_p->vs<<IRLAP_CTRL_NS_BIT_POS;  
  
  irlap_send_frame(frame_p, size);  
}  
  
int16_t irlap2_receive_i_frame(IrLAP_Context *context_p, IrLAP_Frame *frame_p,  
                   IrLAP_Frame *last_resp_p, uint16_t last_resp_size,  
                   uint16_t (*get_dev_info_cb)(uint8_t *dev_info_p, uint8_t user_data),  
                   uint8_t get_dev_info_user_data)  
{  
  IrLAP_Small_Frame resp;  
  int16_t n;  
  uint16_t param_len;  
  uint8_t old_vs;  
  
  old_vs=context_p->vs;  
  
  for(;;) {  
    if((n=irlap_receive_frame(frame_p))<0)  
      return -1;  
  
    if(!(frame_p->head.addr & IRLAP_ADDR_C_TEST))  
      /* discard response frames in secondary mode */  
      continue;  
  
    /* command frame */  
    if(frame_p->head.ctrl & IRLAP_CTRL_I_TEST) {  
      /* U or S format */  
      if(frame_p->head.ctrl & IRLAP_CTRL_S_TEST) {  
    /* U format */  
  
    switch(frame_p->head.ctrl & IRLAP_CTRL_U_MASK) {  
    case IRLAP_CTRL_U_SNRM_CMD:  
      if(frame_p->head.addr==(IRLAP_ADDR_BROADCAST | IRLAP_ADDR_C_TEST) &&  
         n>=sizeof(IrLAP_Head)+sizeof(IrLAP_SNRM) &&  
         irlap_matching_dest_addr(context_p, frame_p->u.snrm.dest)) {  
        context_p->conn_addr=frame_p->u.snrm.conn_addr;  
        context_p->vr=0;  
        context_p->vs=0;  
  
        resp.head.addr=context_p->conn_addr;  
        resp.head.ctrl=IRLAP_CTRL_U_UA_RESP | IRLAP_CTRL_P_F_MASK;  
        memcpy(resp.u.ua.source, context_p->dev_addr, sizeof(IrLAP_Device_Addr));  
        memcpy(resp.u.ua.dest, frame_p->u.snrm.source, sizeof(IrLAP_Device_Addr));  
  
        param_len=irlap_append_neg_params(resp.u.ua.info);  
  
        irlap_send_frame((IrLAP_Frame *)&resp, sizeof(IrLAP_Head)+sizeof(IrLAP_UA)+param_len);  
      }  
      break;  
    case IRLAP_CTRL_U_DISC_CMD:  
      if((frame_p->head.addr & ~IRLAP_ADDR_C_TEST)==context_p->conn_addr) {  
        resp.head.addr=context_p->conn_addr;  
        resp.head.ctrl=IRLAP_CTRL_U_UA_RESP | IRLAP_CTRL_P_F_MASK;  
        memcpy(resp.u.ua.source, context_p->dev_addr, sizeof(IrLAP_Device_Addr));  
        memcpy(resp.u.ua.dest, frame_p->u.snrm.source, sizeof(IrLAP_Device_Addr));  
  
        irlap_send_frame((IrLAP_Frame *)&resp, sizeof(IrLAP_Head)+sizeof(IrLAP_UA));  
      }  
      break;  
    case IRLAP_CTRL_U_UI_CMD:  
      break;  
    case IRLAP_CTRL_U_XID_CMD:  
      if(frame_p->head.addr==(IRLAP_ADDR_BROADCAST | IRLAP_ADDR_C_TEST) &&  
         n>=sizeof(IrLAP_Head)+sizeof(IrLAP_Discovery) &&  
         frame_p->u.discovery.format==IRLAP_XID_FMT_DISCOVERY &&  
         irlap_matching_dest_addr(context_p, frame_p->u.discovery.dest)) {  
        if(frame_p->u.discovery.slot==((frame_p->u.discovery.flags&IRLAP_XID_FLAGS_SLOTS_MASK)==IRLAP_XID_FLAGS_SLOTS_1 ? 0 : 1)) {  
          resp.head.addr=IRLAP_ADDR_BROADCAST;  
          resp.head.ctrl=IRLAP_CTRL_U_XID_RESP | IRLAP_CTRL_P_F_MASK;  
          resp.u.discovery.format=IRLAP_XID_FMT_DISCOVERY;  
          memcpy(resp.u.discovery.source, context_p->dev_addr, sizeof(IrLAP_Device_Addr));  
          memcpy(resp.u.discovery.dest, frame_p->u.discovery.source, sizeof(IrLAP_Device_Addr));  
          resp.u.discovery.flags=frame_p->u.discovery.flags;  
          resp.u.discovery.slot=0;  
          resp.u.discovery.version=IRLAP_VERSION;  
  
          if(get_dev_info_cb)  
        param_len=(*get_dev_info_cb)(resp.u.discovery.info, get_dev_info_user_data);  
          else  
        param_len=0;  
  
          irlap_send_frame((IrLAP_Frame *)&resp, sizeof(IrLAP_Head)+sizeof(IrLAP_Discovery)+param_len);  
        }  
      }  
      break;  
    case IRLAP_CTRL_U_TEST_CMD:  
      if(n>=sizeof(IrLAP_Head)+sizeof(IrLAP_Test) &&  
         irlap_matching_dest_addr(context_p, frame_p->u.test.dest)) {  
        resp.head.addr=IRLAP_ADDR_NULL;  
        resp.head.ctrl=IRLAP_CTRL_U_TEST_RESP | IRLAP_CTRL_P_F_MASK;  
        memcpy(resp.u.test.source, context_p->dev_addr, sizeof(IrLAP_Device_Addr));  
        memcpy(resp.u.test.dest, frame_p->u.test.source, sizeof(IrLAP_Device_Addr));  
  
        irlap_send_frame((IrLAP_Frame *)&resp, sizeof(IrLAP_Head)+sizeof(IrLAP_Test));  
      }  
      break;  
    }  
      } else {  
    /* S format */  
    switch(frame_p->head.ctrl & IRLAP_CTRL_S_MASK) {  
    case IRLAP_CTRL_S_RR:  
      if((frame_p->head.addr & ~IRLAP_ADDR_C_TEST)==context_p->conn_addr) {  
        context_p->vs=(frame_p->head.ctrl>>IRLAP_CTRL_NR_BIT_POS)&IRLAP_CTRL_N_MASK;  
  
        if(last_resp_p && context_p->vs==old_vs) {  
          /* resend last response */  
          irlap2_send_i_frame(context_p, last_resp_p, last_resp_size);  
        } else {  
          /* send RR response */  
          resp.head.addr=context_p->conn_addr;  
          resp.head.ctrl=IRLAP_CTRL_S_RR | IRLAP_CTRL_P_F_MASK | context_p->vr<<IRLAP_CTRL_NR_BIT_POS;  
  
          irlap_send_frame((IrLAP_Frame *)&resp, sizeof(IrLAP_Head));  
        }  
      }  
      break;  
    case IRLAP_CTRL_S_RNR:  
      break;  
    case IRLAP_CTRL_S_REJ:  
      break;  
    case IRLAP_CTRL_S_SREJ:  
      break;  
    }  
      }  
    } else {  
      /* I format */  
      if((frame_p->head.addr & ~IRLAP_ADDR_C_TEST)==context_p->conn_addr &&  
     context_p->vr==((frame_p->head.ctrl>>IRLAP_CTRL_NS_BIT_POS)&IRLAP_CTRL_N_MASK)) {  
    context_p->vs=(frame_p->head.ctrl>>IRLAP_CTRL_NR_BIT_POS)&IRLAP_CTRL_N_MASK;  
  
    if(last_resp_p && context_p->vs==old_vs) {  
      /* reject received frame */  
      resp.head.addr=context_p->conn_addr;  
      resp.head.ctrl=IRLAP_CTRL_S_REJ | IRLAP_CTRL_P_F_MASK | context_p->vr<<IRLAP_CTRL_NR_BIT_POS;  
  
      irlap_send_frame((IrLAP_Frame *)&resp, sizeof(IrLAP_Head));  
    } else {  
      context_p->vr=(context_p->vr+1)&IRLAP_CTRL_N_MASK;  
      return n;  
    }  
      }  
    }  
  }  
}  
