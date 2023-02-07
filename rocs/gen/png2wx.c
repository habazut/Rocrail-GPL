/*
 * png2wx.c
 *
 *  Created on: Oct 23, 2009
 *      Author: rob
 */

/* ------------------------------------------------------------
 * libc interfaces.
 */
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

/* ------------------------------------------------------------
 * platform dependent interfaces.
 */
#if defined __linux__ || defined __aix__
#include <sys/resource.h>
#endif

/* ------------------------------------------------------------
 * rocs interfaces.
 */
#include "rocs/public/mem.h"
#include "rocs/public/trace.h"
#include "rocs/public/file.h"
#include "rocs/public/dir.h"
#include "rocs/public/system.h"

#define BUFFERSIZE 1024

char* addPNG( iOFile png, char* initcode, char* bitmapname );

/** ------------------------------------------------------------
  * public main()
  *
  * @param  argc Number of commanline arguments.
  * @param  argv Commanline arguments.
  * @return      Applications exit code.
  */
int main( int argc, const char* argv[] ) {
  int rc = 0;

  iOTrace trc = TraceOp.inst( TRCLEVEL_INFO, "png2wx", True );
  TraceOp.setAppID( trc, "x" );

  /* Enable coredump for Linux platforms. */
  #if defined __linux__
  {
    struct rlimit rl;
    getrlimit( RLIMIT_CORE, &rl );
    /* Default = 0 */
    rl.rlim_cur = 10240 * 10240;
    setrlimit( RLIMIT_CORE, &rl );
  }
  #endif

  /* Resets memory statistics. */
  MemOp.resetDump();

  /* Read const.xml */
  if( argc >= 3 && FileOp.exist(argv[1]) ) {
    iODir            dir = DirOp.inst( argv[1] );
    const char* fileName = NULL;
    iOFile csrc = NULL;
    iOFile chdr = NULL;
    char* s = NULL;
    char* initcode = NULL;

    if( dir == NULL ) {
      /* Invalid path. */
      TraceOp.trc( "png2wx", TRCLEVEL_WARNING, __LINE__, 9999, "png path [%s] not found", argv[1] );
      return 1;
    }

    TraceOp.println( "Converting all png's found in [%s] into byte arrays in [%s.cpp]", argv[1], argv[2] );

    s = StrOp.fmt( "%s.cpp", argv[2]);
    csrc = FileOp.inst(s, OPEN_WRITE);
    s = StrOp.fmt( "%s.hpp", argv[2]);
    chdr = FileOp.inst(s, OPEN_WRITE);

    FileOp.writeStr( csrc, "/* Autogenerated by png2wx */\n" );
    FileOp.writeStr( chdr, "/* Autogenerated by png2wx */\n" );

    FileOp.fmt( csrc, "#include <wx/wxprec.h>\n#ifndef WX_PRECOMP\n# include <wx/wx.h>\n#endif\n#include <wx/mstream.h>\n#include \"%s.hpp\"\n\n", FileOp.ripPath(argv[2]));

    FileOp.fmt( chdr, "#ifndef __%s\n#define __%s 1\n\n#include <wx/wxprec.h>\n#ifndef WX_PRECOMP\n#include <wx/wx.h>\n#endif\nextern void initialize_images(void);\n", FileOp.ripPath(argv[2]), FileOp.ripPath(argv[2]) );



    /* Get the first directory entry. */
    fileName = DirOp.read( dir );

    /* Iterate all directory entries. */
    while( fileName != NULL ) {

      /* Check for recover files. */
      if( StrOp.endsWithi( fileName, ".png" ) ) {
        char* bitmapname = StrOp.dup(fileName);
        char* pathfileName = StrOp.fmt( "%s%c%s", argv[1], SystemOp.getFileSeparator(), fileName );
        iOFile png = FileOp.inst(pathfileName, OPEN_READONLY);
        TraceOp.trc( "png2wx", TRCLEVEL_INFO, __LINE__, 9999, "processing %s", pathfileName );

        StrOp.replaceAll( bitmapname, '.', '\0');
        StrOp.replaceAll( bitmapname, '-', '_');
        FileOp.fmt( chdr, "extern wxBitmap *_img_%s;\n", bitmapname );
        FileOp.fmt( csrc, "wxBitmap *_img_%s;\n", bitmapname );

        initcode = addPNG( png, initcode, bitmapname );
        TraceOp.trc( "png2wx", TRCLEVEL_INFO, __LINE__, 9999, "initcode len %d", StrOp.len(initcode) );

        StrOp.free(bitmapname);

        FileOp.base.del(png);
      }

      /* Get the next directory entry. */
      fileName = DirOp.read( dir );
    }

    FileOp.fmt( chdr, "\n#endif /* __%s */\n", FileOp.ripPath(argv[2]));

    FileOp.writeStr( csrc, "\nvoid initialize_images(void)\n{\n" );
    FileOp.writeStr( csrc, initcode );
    FileOp.writeStr( csrc, "\nreturn;\n}\n" );


    FileOp.base.del(csrc);
    FileOp.base.del(chdr);


  }
  else {
    TraceOp.println( "Usage: png2wx path sourcename" );
  }

  return rc;
}

char* addPNG( iOFile png, char* initcode, char* bitmapname ) {
  long l = FileOp.size(png);
  long i = 0;
  int lines = l / 80 + (l%80 > 0 ? 1:0);
  int lastline = l%80;
  initcode = StrOp.cat( initcode, "  {\nunsigned char __img[] = {" );

  for( i = 0; i < lines; i++ ) {
    char c[100];
    char s[80 * 6 + 32];
    int len = i+1==lines?lastline:80;
    int n = 0;
    char* sline = NULL;
    FileOp.read( png, c, len );
    for( n=0; n < len; n++ ) {
      StrOp.fmtb( s, "%s0x%.2X", (i==0&&n==0)?"":",", c[n]&0xff );
      sline = StrOp.cat(sline, s);
    }
    sline = StrOp.cat(sline, "\n");
    initcode = StrOp.cat( initcode, sline );
    StrOp.free(sline);
  }
  initcode = StrOp.cat( initcode, "  \n};" );

  initcode = StrOp.cat( initcode, "  wxMemoryInputStream sm(__img, sizeof(__img)" );
  initcode = StrOp.cat( initcode,");\n _img_");
  initcode = StrOp.cat( initcode, bitmapname );
  initcode = StrOp.cat( initcode, " = new wxBitmap(wxImage(sm,wxBITMAP_TYPE_PNG));\n  }\n");
  TraceOp.trc( "png2wx", TRCLEVEL_INFO, __LINE__, 9999, "initcode len %d", StrOp.len(initcode) );

  return initcode;
}

