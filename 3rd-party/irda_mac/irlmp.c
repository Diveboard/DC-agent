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
#ifdef __AVR__  
#include <pgmspace.h>  
#endif  
  
#include "irlmp.h"  
  
#ifdef __AVR__  
#define IRLMP_NICKNAME_TYPE         prog_uchar  
#define IRLMP_NICKNAME_COPY(dest, source)   memcpy_P((dest), (source), sizeof(source))  
#else  
#define IRLMP_NICKNAME_TYPE         uint8_t  
#define IRLMP_NICKNAME_COPY(dest, source)   memcpy((dest), (source), sizeof(source))  
#endif  

/*DIVEBOARD  
static IRLMP_NICKNAME_TYPE nickname[]={  
  'B', 'l', 'a', 'u', 'L', 'o', 'g', 'i', 'c'  
};  */

static IRLMP_NICKNAME_TYPE nickname[]={  
  'U', 'w', 'a', 't', 'e', 'c'
};  

  
uint16_t irlmp_get_dev_info(uint8_t *dev_info_p, uint8_t service_hint1)  
{  
  /*DIVEBOARD
  dev_info_p[0]=IRLMP_HINT0_PDA | IRLMP_HINT0_EXTENSION;  
  dev_info_p[1]=service_hint1;  
  dev_info_p[2]=IRLMP_CHARSET_ASCII;  

  IRLMP_NICKNAME_COPY(dev_info_p+3, nickname);  
  
  return sizeof(nickname)+3;  
  */

  dev_info_p[0]=IRLMP_HINT0_COMPUTER;  
  //dev_info_p[1]=0x0;  
  dev_info_p[1]=IRLMP_CHARSET_ASCII;  

  IRLMP_NICKNAME_COPY(dev_info_p+2, nickname);  
  
  return sizeof(nickname)+2;  
}  
