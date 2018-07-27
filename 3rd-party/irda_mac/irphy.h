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

#ifndef _IRPHY_H
#define _IRPHY_H

#include <inttypes.h>
#include <termios.h>


uint8_t irphy_open();
uint8_t irphy_close();
uint8_t irphy_send(uint8_t v);
uint8_t irphy_wait(int16_t timeout);
uint8_t irphy_receive(uint8_t *data);
uint8_t irphy_set_baud(speed_t baud); //DIVEBOARD


#endif /* _IRPHY_H */
