/*
 Rocrail - Model Railroad Software

 Copyright (C) Rob Versluis <r.j.versluis@rocrail.net>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "rocdigs/impl/xpressnet_impl.h"
#include "rocdigs/impl/xpressnet/elite.h"
#include "rocdigs/impl/xpressnet/li101.h"

Boolean eliteConnect(obj xpressnet) {
  return li101Connect(xpressnet);
}

void eliteDisConnect(obj xpressnet) {
  li101DisConnect(xpressnet);
}

Boolean eliteAvail(obj xpressnet) {
  return li101Avail(xpressnet);
}

void eliteInit(obj xpressnet) {
  iOXpressNetData data = Data(xpressnet);

  /* Asking for CS version */
  byte* out = allocMem(32);
  out[0] = 0x21;
  out[1] = 0x21;
  out[2] = 0x00;
  ThreadOp.post( data->transactor, (obj)out );



  if( data->startpwstate) {
    /* ALL ON */
    out = allocMem(32);
    out[0] = 0x21;
    out[1] = 0x81;
    out[2] = 0xA0;
    ThreadOp.post( data->transactor, (obj)out );
  } else {
    /* ALL OFF*/
    out = allocMem(32);
    out[0] = 0x21;
    out[1] = 0x80;
    out[2] = 0xA1;
    ThreadOp.post( data->transactor, (obj)out );
  }
}

int eliteRead(obj xpressnet, byte* in, Boolean* rspreceived) {
  int len = li101Read(xpressnet, in, rspreceived);
  if( len > 0 ) {
  /* Nasty Elite, response on loc command or loc operated on elite*/
     if (in[0] == 0xE3 || in[0] == 0xE4 || in[0] == 0xE5 ) {
       TraceOp.trc( name, TRCLEVEL_DEBUG, __LINE__, 9999, "Elite: Loc command");
       *rspreceived = True;
     }
  }
  return len;
}

Boolean eliteWrite(obj xpressnet, byte* out, Boolean* rspexpected) {
  iOXpressNetData data = Data(xpressnet);
  Boolean rc = False;
  byte b1 = 0;
  byte b2 = 0;
  
  data->interfaceVersion = 1;


  /* when sending to elite we have to correct for elite (version 1.3) addressing fault
     address 1, port 1 does not exist in elite, address 1 port 2 becomes decoder 1 port 1,
     address 1 port 3 becomes decoder 1 port 2, address 2 port 1 becomes decoder 1 port 4
   */

  /* add port hack */
  if( out[0] == 0x52 ) {
    int port = (out[2] >> 1) & 0x03;
    int addr = out[1];
    b1 = out[1];
    b2 = out[2];
    port++;
    if( port > 3 ) {
      port = 0;
      addr++;
    }
    out[1] = addr;
    out[2] = out[2] & 0xF9;
    out[2] |= (port << 1);
  }

  rc =  li101Write(xpressnet, out, rspexpected);

  if ( out[0] == 0x22 && (out[1] == 0x11 || out[1] == 0x14 || out[1] == 0x15)) {
    *rspexpected = False;
    ThreadOp.sleep( 9000 );
  }
  if (out[0] == 0x23 && (out[1] == 0x12 || out[1] == 0x16 || out[1] == 0x17)) {
    *rspexpected = False;
    ThreadOp.sleep( 9000 );
  }
  if (out[0] == 0x21 && (out[1] == 0x80 || out[1] == 0x81)) {
    *rspexpected = False;
  }

  /* check that, when an answer is expected, there is one available. Sometimes the elite
     does not answer and the command has te be resend to get an answer.
  */
  if( *rspexpected ) {
    ThreadOp.sleep(10);
    if( !eliteAvail( xpressnet ) ) {
      Boolean avail = False;
      int repeat    = 0;
      while( repeat < 5 && !avail ) {
        repeat++;
        rc = li101Write(xpressnet, out, rspexpected);
        TraceOp.trc( name, TRCLEVEL_WARNING, __LINE__, 9999, "no response received on command, resend %d times", repeat );
        ThreadOp.sleep(100);
        avail = eliteAvail( xpressnet );
      };
      if( !avail ) {
        TraceOp.trc( name, TRCLEVEL_WARNING, __LINE__, 9999, "no response received on command, continue with next command" );
        *rspexpected = False;
      }
    }
  }

  /* remove port hack */
  if( out[0] == 0x52 ) {
    out[1] = b1;
    out[2] = b2;
  }  

  return rc;

}
