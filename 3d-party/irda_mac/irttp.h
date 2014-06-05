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

#ifndef _IRTTP_H
#define _IRTTP_H

#include <inttypes.h>

#include "irlmp.h"

//DIVEBOARD #define IRTTP_CREDITS     1
#define IRTTP_CREDITS     8

#define IRTTP_CREDIT_P_MASK   0x80

typedef struct IrTTP_Head {
  IrLMP_Head irlmp;
  uint8_t credit;
} IrTTP_Head;

typedef struct IrTTP_Frame {
  IrTTP_Head head;
  uint8_t info[0];
} IrTTP_Frame;

typedef struct IrTTP_Connect_Frame {
  IrLMP_Head head;
  IrLMP_Ctl ctl;
  uint8_t rsvd;
  uint8_t credit;
} IrTTP_Connect_Frame;

uint8_t irttp1_connect(IrLAP_Context *context_p, IrLAP_Frame *resp_p,
           IrLAP_Device_Addr addr_p,
           uint8_t irlmp_service_hint1,
           char *ias_class_name, char *ias_lsap_name,
           uint8_t slsap_sel, uint8_t *dlsap_sel_p);
int16_t irttp1_send_receive_i_frame(IrLAP_Context *context_p,
            IrTTP_Frame *req_p, uint16_t req_size,
            uint8_t slsap_sel, uint8_t dlsap_sel,
            IrLAP_Frame *resp_p);
int16_t irttp1_continue_receive_i_frame(IrLAP_Context *context_p,
            uint8_t slsap_sel, uint8_t dlsap_sel,
            IrLAP_Frame *resp_p);

void irttp2_reply(IrLAP_Context *context_p, IrTTP_Frame *req_p, IrTTP_Frame *resp_p, uint16_t size);
void irttp2_handle_ctl_frame(IrLAP_Context *context_p, IrLMP_Frame *req_p);
int16_t irttp2_accept(IrLAP_Context *context_p,
          uint8_t irlmp_service_hint1,
          IrIAS_Node **ias_nodes_p);
int16_t irttp2_receive_frame(IrLAP_Context *context_p, IrLAP_Frame *frame_p,
           IrLAP_Frame *last_resp_p, uint16_t last_resp_size,
           uint8_t irlmp_service_hint1,
           IrIAS_Node **ias_nodes_p);

#endif /* _IRTTP_H */
