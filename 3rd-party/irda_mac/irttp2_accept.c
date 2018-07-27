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
  
int16_t irttp2_accept(IrLAP_Context *context_p,  
              uint8_t irlmp_service_hint1,  
              IrIAS_Node **ias_nodes_p)  
{  
  IrLAP_Frame frame;  
  IrLMP_Frame *req_p;  
  int16_t n;  
  
  req_p=(IrLMP_Frame *)&frame;  
  
  for(;;) {  
    if((n=irlmp2_receive_frame(context_p, &frame,  
                   0, 0,  
                   irlmp_service_hint1,  
                   ias_nodes_p))<0)  
      return -1;  
  
    if(req_p->head.dlsap_sel & IRLMP_DLSAP_C_MASK) {  
      /* link control frame */  
      irttp2_handle_ctl_frame(context_p, req_p);  
      if(req_p->u.ctl.opcode==IRLMP_OP_CONNECT)  
    return req_p->head.dlsap_sel & ~IRLMP_DLSAP_C_MASK;  
    }  
  }  
}  
