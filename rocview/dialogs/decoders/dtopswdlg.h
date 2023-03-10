/*
 Rocrail - Model Railroad Software

 Copyright (C) Rob Versluis <r.j.versluis@rocrail.net>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef __dtopswdlg__
#define __dtopswdlg__

/**
@file
Subclass of dtopswdlg, which is generated by wxFormBuilder.
*/

#include "dtopswdlggen.h"
#include "wx/timer.h"

#include "rocs/public/node.h"
#include "rocs/public/queue.h"

#define DTOPSW_Timer 10199

/** Implementing dtopswdlg */
class DTOpSwDlg : public dtopswdlg
{
  void initLabels();
  void initValues();
  void onTimer(wxTimerEvent& event);
  void sendPacket();

  iOQueue m_Queue;
  iONode m_SendedCmd;
  wxTimer* m_Timer;
  int m_iOpSw[256];

  void evaluateEvent( int val );
  void onBoardType( wxCommandEvent& event );
  void onReadAll( wxCommandEvent& event );
  void onWriteAll( wxCommandEvent& event );
  void onOK( wxCommandEvent& event );

public:
  /** Constructor */
  DTOpSwDlg( wxWindow* parent );
  void event( iONode event );
};

#endif // __dtopswdlg__
