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

#ifndef _IRLMP_H
#define _IRLMP_H

#include <inttypes.h>

#include "irlap.h"

#define IRLMP_HINT0_PNP     0x01
#define IRLMP_HINT0_PDA     0x02
#define IRLMP_HINT0_COMPUTER    0x04
#define IRLMP_HINT0_PRINTER   0x08
#define IRLMP_HINT0_MODEM   0x10
#define IRLMP_HINT0_FAX     0x20
#define IRLMP_HINT0_LAN     0x40
#define IRLMP_HINT0_EXTENSION   0x80

#define IRLMP_HINT1_TELEPHONY   0x01
#define IRLMP_HINT1_FILE_SERVER   0x02
#define IRLMP_HINT1_COMM    0x04
#define IRLMP_HINT1_MESSAGE   0x08
#define IRLMP_HINT1_HTTP    0x10
#define IRLMP_HINT1_OBEX    0x20
#define IRLMP_HINT1_EXTENSION   0x80

#define IRLMP_CHARSET_ASCII   0x00
#define IRLMP_CHARSET_ISO8859_1   0x01
#define IRLMP_CHARSET_ISO8859_2   0x02
#define IRLMP_CHARSET_ISO8859_3   0x03
#define IRLMP_CHARSET_ISO8859_4   0x04
#define IRLMP_CHARSET_ISO8859_5   0x05
#define IRLMP_CHARSET_ISO8859_6   0x06
#define IRLMP_CHARSET_ISO8859_7   0x07
#define IRLMP_CHARSET_ISO8859_8   0x08
#define IRLMP_CHARSET_ISO8859_9   0x09
#define IRLMP_CHARSET_UNICODE   0xFF

#define IRLMP_DLSAP_C_MASK    0x80

#define IRLMP_OP_A_MASK     0x80
#define IRLMP_OP_CONNECT    0x01
#define IRLMP_OP_DISCONNECT   0x02
#define IRLMP_OP_ACCESS_MODE    0x03

#define IRIAP_CTL_LST_MASK    0x80
#define IRIAP_CTL_ACK_MASK    0x40

#define IRLMP_OP_LM_GETINFOBASEDETAILS  0x01
#define IRLMP_OP_LM_GETOBJECTS    0x02
#define IRLMP_OP_LM_GETVALUE    0x03
#define IRLMP_OP_LM_GETVALUEBYCLASS 0x04
#define IRLMP_OP_LM_GETOBJECTINFO 0x05
#define IRLMP_OP_LM_GETATTRIBUTENAMES 0x06

#define IRLMP_RESP_UNSUPPORTED    0xFF

#define IRIAP_RESP_GVBCL_SUCCESS  0x00
#define IRIAP_RESP_GVBCL_NOSUCHCLASS  0x01
#define IRIAP_RESP_GVBCL_NOSUCHATTR 0x02

#define IRIAP_VALUE_TYPE_MISSING  0x00
#define IRIAP_VALUE_TYPE_INTEGER  0x01
#define IRIAP_VALUE_TYPE_OCTET_SEQUENCE 0x02
#define IRIAP_VALUE_TYPE_USER_STRING  0x03

typedef struct IrIAS_Node {
  char *class_name;
  char *attr_name;
  uint16_t obj_id;
  uint16_t value;
} IrIAS_Node;

typedef struct IrLMP_Head {
  IrLAP_Head irlap;
  uint8_t dlsap_sel;
  uint8_t slsap_sel;
} IrLMP_Head;

typedef struct IrLMP_Ctl {
  uint8_t opcode;
  uint8_t info[0];
} IrLMP_Ctl;

typedef struct IrLMP_Frame {
  IrLMP_Head head;
  union {
    IrLMP_Ctl ctl;
    uint8_t info[0];
  } u;
} IrLMP_Frame;

uint16_t irlmp_get_dev_info(uint8_t *dev_info_p, uint8_t service_hint1);

uint8_t irlmp1_connect(IrLAP_Context *context_p, IrLAP_Frame *resp_p,
           IrLAP_Device_Addr addr_p,
           uint8_t service_hint1,
           char *ias_class_name, char *ias_lsap_name,
           uint8_t slsap_sel, uint8_t *dlsap_sel_p);
int16_t irlmp1_send_receive_i_frame(IrLAP_Context *context_p,
            IrLMP_Frame *req_p, uint16_t req_size,
            uint8_t slsap_sel, uint8_t dlsap_sel,
            IrLAP_Frame *resp_p);
int16_t irlmp1_continue_receive_i_frame(IrLAP_Context *context_p,
            uint8_t slsap_sel, uint8_t dlsap_sel,
            IrLAP_Frame *resp_p);

void irlmp2_reply(IrLAP_Context *context_p, IrLMP_Frame *req_p, IrLMP_Frame *resp_p, uint16_t size);
void irlmp2_handle_ctl_frame(IrLAP_Context *context_p, IrLMP_Frame *req_p);
int16_t irlmp2_receive_frame(IrLAP_Context *context_p, IrLAP_Frame *frame_p,
           IrLAP_Frame *last_resp_p, uint16_t last_resp_size,
           uint8_t service_hint1,
           IrIAS_Node **ias_nodes_p);

#endif /* _IRLMP_H */
