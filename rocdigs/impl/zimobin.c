/*
 Rocrail - Model Railroad Software

 Copyright (C) 2002-2009 - Rob Versluis <r.j.versluis@rocrail.net>

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


/*
Example of a reset message:

[22/01/2011 16:32:05] - Written data
soh soh  seq dle pc^0x20 msg dle crc8^0x20 eot
01  01   21  10  30      00  10  21        17

seq  = 0x21
info = 0x10 -> message sent by pc
msg  = 0x00 -> reset
crc8 = 0x01


Reply:

[22/01/2011 16:32:10] - Read data
soh soh seq ack pri seq crc8 eot
01  01  80  40  00  21  7e   17
*/


#include "rocdigs/impl/zimobin_impl.h"

#include "rocs/public/trace.h"
#include "rocs/public/node.h"
#include "rocs/public/attr.h"
#include "rocs/public/mem.h"
#include "rocs/public/str.h"
#include "rocs/public/strtok.h"
#include "rocs/public/system.h"

#include "rocrail/wrapper/public/DigInt.h"
#include "rocrail/wrapper/public/SysCmd.h"
#include "rocrail/wrapper/public/FunCmd.h"
#include "rocrail/wrapper/public/Loc.h"
#include "rocrail/wrapper/public/Feedback.h"
#include "rocrail/wrapper/public/Switch.h"
#include "rocrail/wrapper/public/Output.h"
#include "rocrail/wrapper/public/Signal.h"
#include "rocrail/wrapper/public/Program.h"
#include "rocrail/wrapper/public/Response.h"
#include "rocrail/wrapper/public/State.h"
#include "rocrail/wrapper/public/BinCmd.h"
#include "rocrail/wrapper/public/Clock.h"

#include "rocutils/public/addr.h"


static int instCnt = 0;

/** ----- OBase ----- */
static void __del( void* inst ) {
  if( inst != NULL ) {
    iOZimoBinData data = Data(inst);
    /* Cleanup data->xxx members...*/
    
    freeMem( data );
    freeMem( inst );
    instCnt--;
  }
  return;
}

static const char* __name( void ) {
  return name;
}

static unsigned char* __serialize( void* inst, long* size ) {
  return NULL;
}

static void __deserialize( void* inst,unsigned char* bytestream ) {
  return;
}

static char* __toString( void* inst ) {
  return NULL;
}

static int __count( void ) {
  return instCnt;
}

static struct OBase* __clone( void* inst ) {
  return NULL;
}

static Boolean __equals( void* inst1, void* inst2 ) {
  return False;
}

static void* __properties( void* inst ) {
  iOZimoBinData data = Data(inst);
  return NULL;
}

static const char* __id( void* inst ) {
  iOZimoBinData data = Data(inst);
  return NULL;
}

static void* __event( void* inst, const void* evt ) {
  iOZimoBinData data = Data(inst);
  return NULL;
}

/** ----- OZimoBin ----- */




/* Update 8-bit CRC value
��using polynomial� X^8 + X^5 + X^4 + 1 */
#define POLYVAL 0x8C
static void __updateCRC(byte new, byte* crc)
{
  int i;
  byte c = *crc;
  for (i = 0; i < 8; i++) {
    if ((c ^ new) & 1)
      c = (c >> 1 ) ^ POLYVAL;
    else
      c >>= 1;
    new >>= 1;
  }
  *crc = c;
}

/*
CRC-8-Dallas/Maxim
x8 + x5 + x4 + 1 (1-Wire bus)

Representations: normal / **reversed** / reverse of reciprocal
0x31 / 0x8C / 0x98

Initialized with 0xFF

 */
static byte __checkSum(byte* packet, int len) {
  byte checksum = 0xFF;
  int i = 0;
  for( i = 0; i < len; i++ ) {
    __updateCRC(packet[i], &checksum);
  }

  return checksum;
}





/*
 * Escape control bytes:
 * SOH -> DLE, SOH^0x20
 * EOT -> DLE, EOT^0x20
 * DLE -> DLE, DLE^0x20
 */
#define SOH 0x01
#define EOT 0x17
#define DLE 0x10

static int __escapePacket(byte* packet, int inlen) {
  byte buf[64];
  int len = inlen;
  int i = 0;
  int idx = 0;

  for( i = 0; i < inlen; i++ ) {
    if(  packet[i] == SOH || packet[i] == EOT || packet[i] == DLE ) {
      buf[idx] = DLE;
      idx++;
      buf[idx] = packet[i] ^ 0x20;
      idx++;
    }
    else {
      buf[idx] = packet[i];
      idx++;
    }
  }

  len = idx;
  MemOp.copy( packet, buf, len );

  return len;

}


static int __unescapePacket(byte* packet, int inlen) {
  byte buf[64];
  int len = inlen;
  int i = 0;
  int idx = 0;

  for( i = 0; i < inlen; i++ ) {
    if(  packet[i] == DLE ) {
      i++;
      buf[idx] = packet[i] ^ 0x20;
      idx++;
    }
    else {
      buf[idx] = packet[i];
      idx++;
    }
  }

  len = idx;
  MemOp.copy( packet, buf, len );

  return len;

}


static int __controlPacket(byte* packet, int inlen) {
  byte buf[256];
  int len = inlen + 3;

  buf[0] = SOH;
  buf[1] = SOH;
  MemOp.copy(buf+2, packet, inlen);
  buf[inlen+2] = EOT;

  MemOp.copy(packet, buf, len);
  return len;

}


static int __uncontrolPacket(byte* packet, int inlen) {
  byte buf[256];
  int len = inlen - 3;

  MemOp.copy(buf, packet+2, len);
  MemOp.copy(packet, buf, len);

  return len;
}


static int __dccSteps(int steps) {
  if( steps < 28 )
    return 0x04;
  if( steps > 28 )
    return 0x0C;
  return 0x08;
}


static iONode __translate( iOZimoBin zimobin, iONode node ) {
  iOZimoBinData data = Data(zimobin);
  iONode rsp = NULL;

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "translate %s", NodeOp.getName( node ) );

  /* Clock command. */
  if( StrOp.equals( NodeOp.getName( node ), wClock.name() ) ) {
  }

  /* Switch command. */
  else if( StrOp.equals( NodeOp.getName( node ), wSwitch.name() ) ) {

    int addr = wSwitch.getaddr1( node );
    int port = wSwitch.getport1( node );
    int gate = wSwitch.getgate1( node );

    int state = StrOp.equals( wSwitch.getcmd( node ), wSwitch.turnout ) ? 1:0;

    if( port == 0 ) {
      AddrOp.fromFADA( addr, &addr, &port, &gate );
    }
    else if( addr == 0 && port > 0 ) {
      AddrOp.fromPADA( port, &addr, &port );
    }

    if( port > 0 )
      port--;

    byte* outa = allocMem(32);
    outa[0] = 5;    /* packet length */
    outa[1] = 0x10; /* command station instruction */
    outa[2] = 7;    /* accessory command */
    outa[3] = addr / 256 | 0x80;
    outa[4] = addr % 256;
    outa[5] = (port+state) | 0x08; /* TODO: Reset thge other gate? */

    TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999, "switch %d:%d, %s", addr, port, state?"on":"off");

    ThreadOp.post( data->transactor, (obj)outa );
  }

  /* Output command. */
  else if( StrOp.equals( NodeOp.getName( node ), wOutput.name() ) ) {
    int action1 = 0x00;
    int action2 = 0x00;

    int addr   = wOutput.getaddr( node );
    int port   = wOutput.getport( node );
    int gate   = wOutput.getgate( node );

    int state = StrOp.equals( wSwitch.getcmd( node ), wSwitch.turnout ) ? 0x08:0x00;

    if( port == 0 )
      AddrOp.fromFADA( addr, &addr, &port, &gate );
    else if( addr == 0 && port > 0 )
      AddrOp.fromPADA( port, &addr, &port );

    if( port > 0 )
      port--;

    byte* outa = allocMem(32);
    outa[0] = 5;    /* packet length */
    outa[1] = 0x10; /* command station instruction */
    outa[2] = 7;    /* accessory command */
    outa[3] = addr / 256 | 0x80;
    outa[4] = addr % 256;
    outa[5] = (port+gate) | state;

    TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999, "output %d:%d, %s", addr, port, state?"on":"off");

    ThreadOp.post( data->transactor, (obj)outa );
  }


  /* Signal command. */
  else if( StrOp.equals( NodeOp.getName( node ), wSignal.name() ) ) {
    TraceOp.trc( name, TRCLEVEL_WARNING, __LINE__, 9999,
        "Signal commands are no longer supported at this level." );
  }

  /* Sensor command. */
  else if( StrOp.equals( NodeOp.getName( node ), wFeedback.name() ) ) {
    int addr = wFeedback.getaddr( node );
    Boolean state = wFeedback.isstate( node );

    TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "simulate fb addr=%d state=%s", addr, state?"true":"false" );
    rsp = (iONode)NodeOp.base.clone( node );
  }

  /* Loc command. */
  else if( StrOp.equals( NodeOp.getName( node ), wLoc.name() ) || StrOp.equals( NodeOp.getName( node ), wFunCmd.name() ) ) {
    int addr = wLoc.getaddr( node );
    int V = 0;
    int steps = wLoc.getspcnt( node );
    byte* outa = allocMem(32);

    if( wLoc.getV( node ) != -1 ) {
      if( StrOp.equals( wLoc.getV_mode( node ), wLoc.V_mode_percent ) )
        V = (wLoc.getV( node ) * steps) / 100;
      else if( wLoc.getV_max( node ) > 0 )
        V = (wLoc.getV( node ) * steps) / wLoc.getV_max( node );
    }

    Boolean f0 = wFunCmd.isf0( node );
    Boolean f1 = wFunCmd.isf1( node );
    Boolean f2 = wFunCmd.isf2( node );
    Boolean f3 = wFunCmd.isf3( node );
    Boolean f4 = wFunCmd.isf4( node );
    Boolean f5 = wFunCmd.isf5( node );
    Boolean f6 = wFunCmd.isf6( node );
    Boolean f7 = wFunCmd.isf7( node );
    Boolean f8 = wFunCmd.isf8( node );
    Boolean f9  = wFunCmd.isf9 ( node );
    Boolean f10 = wFunCmd.isf10( node );
    Boolean f11 = wFunCmd.isf11( node );
    Boolean f12 = wFunCmd.isf12( node );

    byte functions1 = (f1 ?0x01:0) + (f2 ?0x02:0) + (f3 ?0x04:0) + (f4 ?0x08:0) +
                      (f5 ?0x10:0) + (f6 ?0x20:0) + (f7 ?0x40:0) + (f8 ?0x80:0);
    byte functions2 = (f9 ?0x01:0) + (f10?0x02:0) + (f11?0x04:0) + (f12?0x08:0);


    outa[0] = 8;    /* packet length */
    outa[1] = 0x10; /* command station instruction */
    outa[2] = 3;    /* loco control */
    outa[3] = addr / 256 | 0x80; /* force dcc */
    outa[4] = addr % 256;
    outa[5] = V;
    outa[6] = (wLoc.isdir( node )?0x00:0x20) | (wLoc.isfn( node )?0x10:0x00) | __dccSteps(steps);
    outa[7] = functions1;
    outa[8] = functions2;

    TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999, "loco: V=%d, dir=%s, lights=%s",
        V, wLoc.isdir( node )?"fwd":"rev", wLoc.isfn( node )?"on":"off" );
    TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999,
        "f1=%s f2=%s f3=%s f4=%s f5=%s f6=%s f7=%s f8=%s f9=%s f10=%s f11=%s f12=%s",
        (f1?"ON":"OFF"), (f2?"ON":"OFF"), (f3?"ON":"OFF"), (f4?"ON":"OFF"),
        (f5?"ON":"OFF"), (f6?"ON":"OFF"), (f7?"ON":"OFF"), (f8?"ON":"OFF"),
        (f9?"ON":"OFF"), (f10?"ON":"OFF"), (f11?"ON":"OFF"), (f12?"ON":"OFF") );
    ThreadOp.post( data->transactor, (obj)outa );
  }


  /* System command. */
  else if( StrOp.equals( NodeOp.getName( node ), wSysCmd.name() ) ) {
    const char* cmd = wSysCmd.getcmd( node );
    byte* outa = allocMem(256);

    outa[0] = 3;    /* packet length */
    outa[1] = 0x10; /* command station instruction */
    outa[2] = 2;    /* track control */

    if( StrOp.equals( cmd, wSysCmd.stop ) ) {
      TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999, "Power OFF" );
      outa[3] = 1; /* switch track voltage OFF */
    }
    else if( StrOp.equals( cmd, wSysCmd.go ) ) {
      TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999, "Power ON" );
      outa[3] = 2; /* switch track voltage ON */
    }
    else if( StrOp.equals( cmd, wSysCmd.ebreak ) ) {
      TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999, "Power ON" );
      outa[3] = 0; /* stop broadcast */
    }
    ThreadOp.post( data->transactor, (obj)outa );

  }
  /* Program command. */
  else if( StrOp.equals( NodeOp.getName( node ), wProgram.name() ) ) {

    if( wProgram.getcmd( node ) == wProgram.get ) {
      int cv = wProgram.getcv( node );
      TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "get CV%d...", cv );
    }
    else if( wProgram.getcmd( node ) == wProgram.set ) {
      int cv = wProgram.getcv( node );
      int value = wProgram.getvalue( node );
      int decaddr = wProgram.getdecaddr( node );


      if( wProgram.ispom(node) ) {
        TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "POM: set CV%d of loc %d to %d...", cv, decaddr, value );
      }
    }
    else if(  wProgram.getcmd( node ) == wProgram.pton ) {
      /* CS will go ton Pt on on first programming request */
    }  /* PT off, send: All ON" */
    else if( wProgram.getcmd( node ) == wProgram.ptoff ) {
    }

  }


  return rsp;
}



/**  */
static iONode _cmd( obj inst ,const iONode nodeA ) {
  iOZimoBinData data = Data(inst);
  iONode rsp = NULL;

  rsp = __translate( (iOZimoBin)inst, nodeA );

  return rsp;
}


/**  */
static void _halt( obj inst, Boolean poweroff ) {
  iOZimoBinData data = Data(inst);
  data->run = False;
  return;
}


/**  */
static Boolean _setListener( obj inst ,obj listenerObj ,const digint_listener listenerFun ) {
  iOZimoBinData data = Data(inst);
  data->listenerObj = listenerObj;
  data->listenerFun = listenerFun;
  return True;
}


static Boolean _setRawListener(obj inst, obj listenerObj, const digint_rawlistener listenerFun ) {
  return True;
}

static byte* _cmdRaw( obj inst, const byte* cmd ) {
  return NULL;
}

/** external shortcut event */
static void _shortcut( obj inst ) {
  iOZimoBinData data = Data(inst);
  return;
}


/** bit0=power, bit1=programming, bit2=connection */
static int _state( obj inst ) {
  iOZimoBinData data = Data(inst);
  return 0;
}


/**  */
static Boolean _supportPT( obj inst ) {
  iOZimoBinData data = Data(inst);
  return 0;
}


static void __handleTrackSection(iOZimoBin zimobin, byte* packet) {
  iOZimoBinData data = Data(zimobin);
  int addr = packet[3] * 8 + packet[4];

  TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999, "tracksection: addr=%d, section=%d, status=0x%02X", packet[3], packet[4], packet[5] );

  /* inform listener: Node3 */
  iONode nodeC = NodeOp.inst( wFeedback.name(), NULL, ELEMENT_NODE );
  wFeedback.setaddr( nodeC, addr );
  wFeedback.setstate( nodeC, (packet[5]&0x18)?True:False );
  if( data->iid != NULL )
    wFeedback.setiid( nodeC, data->iid );

  data->listenerFun( data->listenerObj, nodeC, TRCLEVEL_INFO );
}


static Boolean __evaluatePacket(iOZimoBin zimobin, byte* packet, int len) {
  iOZimoBinData data    = Data(zimobin);
  Boolean ok = True;

  TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999, "sequenceID=%d message=0x%02X", packet[0], packet[1] );

  switch( packet[1] ) {
  case 0x0A: /* command station instruction */
    break;
  case 0x02: /* MX9 track section */
    if( packet[2] == 255 )
      __handleTrackSection(zimobin, packet);
    break;
  }

  return ok;
}


static void __transactor( void* threadinst ) {
  iOThread      th      = (iOThread)threadinst;
  iOZimoBin     zimobin = (iOZimoBin)ThreadOp.getParm(th);
  iOZimoBinData data    = Data(zimobin);

  byte out[256];
  obj post = NULL;
  int esqid = 0x20;

  ThreadOp.setDescription( th, "Transactor for ZimoBin" );
  TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999, "Transactor started." );

  /* send reset packet */
  {
    byte* outa = allocMem(32);

    outa[0] = 2;    /* packet length */
    outa[1] = 0x10; /* command station instruction */
    outa[2] = 0;    /* track control */
    ThreadOp.post( data->transactor, (obj)outa );
  }


  do {
    /* get next command only if the last command was successfull,
       otherwise work on the current node until the cs will answer, or give up after numtries */
    if (True) {
      post = ThreadOp.getPost( th );
      if (post != NULL) {
        int packetlen = ((byte*) post)[0] & 0xFF;
        esqid++;
        if( esqid > 255 )
          esqid = 1;

        /* sequence byte */
        out[0] = esqid;
        MemOp.copy( out+1, (byte*) post+1, packetlen);
        packetlen++; /* add one for the sequence byte */
        freeMem( post);

        out[packetlen] = __checkSum(out, packetlen);
        packetlen++;
        packetlen = __escapePacket(out, packetlen);
        packetlen = __controlPacket(out, packetlen);
        TraceOp.dump( NULL, TRCLEVEL_BYTE, (char*)out, packetlen );
        SerialOp.write( data->serial, (char*) out, packetlen );

      }

      int dataAvailable = SerialOp.available(data->serial);
      byte inbuf[256];
      int inIdx = 0;
      Boolean packetReceived = False;
      while( dataAvailable > 0 && inIdx < 256) {
        Boolean ok = SerialOp.read( data->serial, (char*) &inbuf[inIdx], 1 );
        if( !ok ) {
          TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999, "Read from port failed." );
          break;
        }
        /*
         * Escape control bytes:
         * SOH -> DLE, SOH^0x20
         * EOT -> DLE, EOT^0x20
         * DLE -> DLE, DLE^0x20
        #define SOH 0x01
        #define EOT 0x17
        #define DLE 0x10
        --------------------------------------------------------- |----------------|
        00000000: 01 01 00 0A 00 02 3D 17                         |......=.        |
        20101231.154747.106 r9999c transact OZimoBin 0524 No valid start sequence: idx=1 in=0A
        */
        if( inIdx == 1  && inbuf[inIdx] != SOH  && inbuf[inIdx-1] != SOH  ) {
          TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999, "No valid start sequence: idx=%d in=%02X", inIdx, inbuf[inIdx] );
          TraceOp.dump( NULL, TRCLEVEL_BYTE, (char*)inbuf, inIdx+1 );
          inIdx = 0;
          break;
        }

        if( inIdx > 1 ) {
          if( inbuf[inIdx] == EOT && inbuf[inIdx-1] != DLE ) {
            /* end of packet */
            packetReceived = True;
            inIdx++;
            TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999, "End of packet detected." );
            break;
          }
        }

        dataAvailable = SerialOp.available(data->serial);
        int waitformore = 10;
        while( dataAvailable == 0 && waitformore > 0) {
          ThreadOp.sleep(10);
          dataAvailable = SerialOp.available(data->serial);
          waitformore--;
        }

        inIdx++;
      };

      if( packetReceived ) {
        TraceOp.dump( NULL, TRCLEVEL_BYTE, (char*)inbuf, inIdx );
        inIdx = __unescapePacket(inbuf, inIdx);
        inIdx = __uncontrolPacket(inbuf, inIdx);
        __evaluatePacket(zimobin, inbuf, inIdx);
      }
      else if(inIdx > 0) {
        /* Invalid packet? */
        TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999, "Invalid packet." );
        TraceOp.dump( NULL, TRCLEVEL_BYTE, (char*)inbuf, inIdx );
      }


    }

    /* Give up timeslice:*/
    ThreadOp.sleep( 10 );

  } while( data->run );
}


/* VERSION: */
static int vmajor = 2;
static int vminor = 0;
static int patch  = 99;
static int _version( obj inst ) {
  iOZimoBinData data = Data(inst);
  return vmajor*10000 + vminor*100 + patch;
}



/**  */
static struct OZimoBin* _inst( const iONode ini ,const iOTrace trc ) {
  iOZimoBin __ZimoBin = allocMem( sizeof( struct OZimoBin ) );
  iOZimoBinData data = allocMem( sizeof( struct OZimoBinData ) );
  MemOp.basecpy( __ZimoBin, &ZimoBinOp, 0, sizeof( struct OZimoBin ), data );

  TraceOp.set( trc );

  /* Initialize data->xxx members... */
  data->ini = ini;
  data->dummyio = wDigInt.isdummyio( ini );

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "----------------------------------------" );
  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "ZimoBin %d.%d.%d", vmajor, vminor, patch );
  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "----------------------------------------" );

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "device          = %s", wDigInt.getdevice( ini ) );
  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "bps             = %d", wDigInt.getbps( ini ) );
  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "----------------------------------------" );

  data->serial = SerialOp.inst( wDigInt.getdevice( ini ) );
  SerialOp.setFlow( data->serial, cts );
  SerialOp.setLine( data->serial, wDigInt.getbps( ini ), 8, 1, 0, wDigInt.isrtsdisabled( ini ) );
  SerialOp.setTimeout( data->serial, wDigInt.gettimeout( ini ), wDigInt.gettimeout( ini ) );

  if( !SerialOp.open( data->serial ) && !data->dummyio ) {
    TraceOp.trc( name, TRCLEVEL_EXCEPTION, __LINE__, 9999, "Could not init ZimoBin port %s!", wDigInt.getdevice( ini ) );
  }
  else {
    data->run = True;
    data->transactor = ThreadOp.inst( "transactor", &__transactor, __ZimoBin );
    ThreadOp.start( data->transactor );
  }



  instCnt++;
  return __ZimoBin;
}


/* Support for dynamic Loading */
iIDigInt rocGetDigInt( const iONode ini ,const iOTrace trc )
{
  return (iIDigInt)_inst(ini,trc);
}



/* ----- DO NOT REMOVE OR EDIT THIS INCLUDE LINE! -----*/
#include "rocdigs/impl/zimobin.fm"
/* ----- DO NOT REMOVE OR EDIT THIS INCLUDE LINE! -----*/
