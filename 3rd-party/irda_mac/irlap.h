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

#ifndef _IRLAP_H
#define _IRLAP_H

#include <inttypes.h>
#include <string.h>  


//DIVEBOARD #define IRLAP_DATA_SIZE     128
#define IRLAP_DATA_SIZE     2048
#define IRLAP_SMALL_DATA_SIZE   32

#define IRLAP_VERSION     0x00

#define IRLAP_ADDR_NULL     0x00
#define IRLAP_ADDR_BROADCAST    0xFE
#define IRLAP_ADDR_C_TEST   0x01

#define IRLAP_CTRL_I_TEST   0x01
#define IRLAP_CTRL_S_TEST   0x02
#define IRLAP_CTRL_P_F_MASK   0x10
#define IRLAP_CTRL_U_MASK   0xEF
#define IRLAP_CTRL_S_MASK   0x0F
#define IRLAP_CTRL_U_SNRM_CMD   0x83
#define IRLAP_CTRL_U_DISC_CMD   0x43
#define IRLAP_CTRL_U_UI_CMD   0x03
#define IRLAP_CTRL_U_XID_CMD    0x2F
#define IRLAP_CTRL_U_TEST_CMD   0xE3
#define IRLAP_CTRL_U_RNRM_RESP    0x83
#define IRLAP_CTRL_U_UA_RESP    0x63
#define IRLAP_CTRL_U_FRMR_RESP    0x87
#define IRLAP_CTRL_U_DM_RESP    0x0F
#define IRLAP_CTRL_U_RD_RESP    0x43
#define IRLAP_CTRL_U_UI_RESP    0x03
#define IRLAP_CTRL_U_XID_RESP   0xAF
#define IRLAP_CTRL_U_TEST_RESP    0xE3
#define IRLAP_CTRL_S_RR     0x01
#define IRLAP_CTRL_S_RNR    0x05
#define IRLAP_CTRL_S_REJ    0x09
#define IRLAP_CTRL_S_SREJ   0x0D

#define IRLAP_CTRL_NR_BIT_POS   5
#define IRLAP_CTRL_NS_BIT_POS   1
#define IRLAP_CTRL_N_MASK   7

#define IRLAP_XID_FMT_DISCOVERY   0x01

#define IRLAP_XID_FLAGS_SLOTS_MASK  0x03
#define IRLAP_XID_FLAGS_SLOTS_1   0x00
#define IRLAP_XID_FLAGS_SLOTS_6   0x01
#define IRLAP_XID_FLAGS_SLOTS_8   0x02
#define IRLAP_XID_FLAGS_SLOTS_16  0x03
#define IRLAP_XID_FLAGS_NEW_DEV   0x04

#define IRLAP_PAR_BAUD_RATE   0x01
#define IRLAP_PAR_MAX_TURN_TIME   0x82
#define IRLAP_PAR_DATA_SIZE   0x83
#define IRLAP_PAR_WINDOW_SIZE   0x84
#define IRLAP_PAR_ADD_BOFS    0x85
#define IRLAP_PAR_MIN_TURN_TIME   0x86
#define IRLAP_PAR_DISC_TIME   0x08

typedef uint8_t IrLAP_Device_Addr[4];

typedef struct IrLAP_Context {
  IrLAP_Device_Addr dev_addr;
  uint8_t conn_addr;
  uint8_t vr, vs;
} IrLAP_Context;

typedef struct IrLAP_Head {
  uint8_t addr;
  uint8_t ctrl;
} IrLAP_Head;

typedef struct IrLAP_Neg_Param {
  uint8_t id;
  uint8_t len;
  uint8_t value[0];
} IrLAP_Neg_Param;

typedef struct IrLAP_Test {
  IrLAP_Device_Addr source;
  IrLAP_Device_Addr dest;
  uint8_t info[0];
} IrLAP_Test;

typedef struct IrLAP_Discovery {
  uint8_t format;
  IrLAP_Device_Addr source;
  IrLAP_Device_Addr dest;
  uint8_t flags;
  uint8_t slot;
  uint8_t version;
  uint8_t info[0];
} IrLAP_Discovery;

typedef struct IrLAP_SNRM {
  IrLAP_Device_Addr source;
  IrLAP_Device_Addr dest;
  uint8_t conn_addr;
  uint8_t info[0];
} IrLAP_SNRM;

typedef struct IrLAP_UA {
  IrLAP_Device_Addr source;
  IrLAP_Device_Addr dest;
  uint8_t info[0];
} IrLAP_UA;

typedef struct IrLAP_Frame {
  IrLAP_Head head;
  union {
    IrLAP_Test test;
    IrLAP_Discovery discovery;
    IrLAP_SNRM snrm;
    IrLAP_UA ua;
    uint8_t data[IRLAP_DATA_SIZE+2];
  } u;
} IrLAP_Frame;

typedef struct IrLAP_Small_Frame {
  IrLAP_Head head;
  union {
    IrLAP_Test test;
    IrLAP_Discovery discovery;
    IrLAP_SNRM snrm;
    IrLAP_UA ua;
    uint8_t data[IRLAP_SMALL_DATA_SIZE];
  } u;
} IrLAP_Small_Frame;

uint16_t irlap_append_neg_params(uint8_t *info_p);
uint8_t irlap_matching_dest_addr(IrLAP_Context *context_p, IrLAP_Device_Addr addr_p);
uint16_t irlap_setup_connection(IrLAP_Neg_Param *param_p, size_t param_size);

void irlap_init_context(IrLAP_Context *context_p);

uint8_t irlap_send_frame(IrLAP_Frame *frame_p, uint16_t size);
int16_t irlap_receive_frame(IrLAP_Frame *frame_p);

uint8_t irlap1_connect(IrLAP_Context *context_p, IrLAP_Frame *resp_p,
           IrLAP_Device_Addr addr_p,
           uint16_t (*get_dev_info_cb)(uint8_t *dev_info_p, uint8_t user_data),
           uint8_t get_dev_info_user_data);
uint8_t irlap1_disconnect(IrLAP_Context *context_p, IrLAP_Frame *resp_p);
int16_t irlap1_send_receive_i_frame(IrLAP_Context *context_p,
            IrLAP_Frame *req_p, uint16_t req_size,
            IrLAP_Frame *resp_p);

int16_t irlap1_continue_receive_i_frame(IrLAP_Context *context_p,
            IrLAP_Frame *resp_p);



void irlap2_send_i_frame(IrLAP_Context *context_p, IrLAP_Frame *frame_p, uint16_t size);
int16_t irlap2_receive_i_frame(IrLAP_Context *context_p, IrLAP_Frame *frame_p,
             IrLAP_Frame *last_resp_p, uint16_t last_resp_size,
             uint16_t (*get_dev_info_cb)(uint8_t *dev_info_p, uint8_t user_data),
             uint8_t get_dev_info_user_data);

#endif /* _IRLAP_H */
