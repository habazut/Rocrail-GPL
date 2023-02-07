#ifndef __uhl68610dlg__
#define __uhl68610dlg__

/**
@file
Subclass of uhl68610dlggen, which is generated by wxFormBuilder.
*/

#include "uhl68610gen.h"
#include "wx/timer.h"

#include "rocs/public/node.h"
#include "rocs/public/queue.h"

#define LNCV_Timer 10199


/** Implementing uhl68610dlggen */
class Uhl68610Dlg : public uhl68610dlggen
{
  void initLabels();
  void initValues();
  void onTimer(wxTimerEvent& event);
  void sendPacket();
  void evaluateEvent( int cv, int val );
  int getFactor();

  iOQueue m_Queue;
  iONode m_SendedCmd;
  wxTimer* m_Timer;
  int m_iLnCv[256];


public:
	/** Constructor */
	Uhl68610Dlg( wxWindow* parent );

	void onReporting( wxCommandEvent& event );
  void onRead( wxCommandEvent& event );
  void onWrite( wxCommandEvent& event );
  void onCancel( wxCommandEvent& event );
  void onOK( wxCommandEvent& event );
  void event( iONode event );
  void onScale( wxCommandEvent& event );
  void onGap( wxSpinEvent& event );

};

#endif // __uhl68610dlg__