/*
 Rocrail - Model Railroad Software

 Copyright (C) 2002-2007 - Rob Versluis <r.j.versluis@rocrail.net>

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
// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#ifdef __GNUG__
    #pragma implementation "rocgui.cpp"
    #pragma interface "rocgui.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


#include "rocs/public/node.h"
#include "rocs/public/thread.h"
#include "rocs/public/str.h"
#include "rocs/public/mem.h"
#include "rocs/public/system.h"
#include "rocview/public/planpanel.h"
#include "rocview/public/guiapp.h"
#include "rocview/public/item.h"
#include "rocview/dialogs/planpaneldlg.h"
#include "rocview/dialogs/selectdialog.h"
#include "rocview/dialogs/modulepropsdlg.h"

#include "rocview/wrapper/public/Gui.h"
#include "rocview/wrapper/public/PlanPanel.h"

#include "rocrail/wrapper/public/SysCmd.h"
#include "rocrail/wrapper/public/ModelCmd.h"
#include "rocrail/wrapper/public/Plan.h"
#include "rocrail/wrapper/public/Track.h"
#include "rocrail/wrapper/public/Route.h"
#include "rocrail/wrapper/public/Switch.h"
#include "rocrail/wrapper/public/Signal.h"
#include "rocrail/wrapper/public/Output.h"
#include "rocrail/wrapper/public/Block.h"
#include "rocrail/wrapper/public/Stage.h"
#include "rocrail/wrapper/public/Feedback.h"
#include "rocrail/wrapper/public/Loc.h"
#include "rocrail/wrapper/public/Turntable.h"
#include "rocrail/wrapper/public/SelTab.h"
#include "rocrail/wrapper/public/Text.h"
#include "rocrail/wrapper/public/Item.h"
#include "rocrail/wrapper/public/ZLevel.h"
#include "rocrail/wrapper/public/Module.h"

#include "rocrail/wrapper/public/TrackList.h"
#include "rocrail/wrapper/public/RouteList.h"
#include "rocrail/wrapper/public/SwitchList.h"
#include "rocrail/wrapper/public/SignalList.h"
#include "rocrail/wrapper/public/OutputList.h"
#include "rocrail/wrapper/public/BlockList.h"
#include "rocrail/wrapper/public/StageList.h"
#include "rocrail/wrapper/public/FeedbackList.h"
#include "rocrail/wrapper/public/LocList.h"
#include "rocrail/wrapper/public/TurntableList.h"
#include "rocrail/wrapper/public/SelTabList.h"
#include "rocrail/wrapper/public/TextList.h"
#include "rocrail/wrapper/public/Location.h"
#include "rocrail/wrapper/public/LocationList.h"
#include "rocrail/wrapper/public/Schedule.h"
#include "rocrail/wrapper/public/ScheduleList.h"
#include "rocrail/wrapper/public/Action.h"
#include "rocrail/wrapper/public/ActionList.h"

BEGIN_EVENT_TABLE(PlanPanel, wxScrolledWindow)

  EVT_MENU     (ME_ModuleRotate , PlanPanel::OnRotate )
  EVT_MENU     (ME_ModuleNorth , PlanPanel::OnRotate )
  EVT_MENU     (ME_ModuleEast , PlanPanel::OnRotate )
  EVT_MENU     (ME_ModuleSouth , PlanPanel::OnRotate )
  EVT_MENU     (ME_ModuleWest , PlanPanel::OnRotate )

  EVT_MOTION (PlanPanel::OnMotion    )
  EVT_SIZE (PlanPanel::OnSize )
  EVT_PAINT(PlanPanel::OnPaint)

  EVT_LEFT_DOWN (PlanPanel::OnLeftDown  )
  EVT_LEFT_UP (PlanPanel::OnLeftUp)
  EVT_RIGHT_UP(PlanPanel::OnPopup)

  EVT_MENU( ADDITEM_EVENT        , PlanPanel::addItemCmd  )
  EVT_MENU( UPDATEITEM_EVENT     , PlanPanel::updateItemCmd  )
  EVT_MENU( UPDATETT_EVENT       , PlanPanel::updateTTItemCmd  )
  EVT_MENU( ADDMULTIPLEITEM_EVENT, PlanPanel::addMultipleItem  )
  EVT_MENU( UPDATE4ROUTE_EVENT   , PlanPanel::update4Route  )

  EVT_MENU( SHOW_EVENT   , PlanPanel::OnShow   )

  EVT_MENU( ME_AddTrackStraight , PlanPanel::addTrackStraight )
  EVT_MENU( ME_AddTrackCurve    , PlanPanel::addTrackCurve )
  EVT_MENU( ME_AddTrackDir      , PlanPanel::addTrackDir )
  EVT_MENU( ME_AddTrackDirAll   , PlanPanel::addTrackDirAll )
  EVT_MENU( ME_AddTrackBuffer   , PlanPanel::addTrackBuffer )
  EVT_MENU( ME_AddTrackConnector, PlanPanel::addTrackConnector )

  EVT_MENU( ME_AddSwitchLeft     , PlanPanel::addSwitchLeft)
  EVT_MENU( ME_AddSwitchRight    , PlanPanel::addSwitchRight)
  EVT_MENU( ME_AddSwitchCrossing , PlanPanel::addSwitchCrossing)
  EVT_MENU( ME_AddSwitchDCrossing, PlanPanel::addSwitchDCrossing)
  EVT_MENU( ME_AddSwitchThreeway , PlanPanel::addSwitchThreeway)
  EVT_MENU( ME_AddSwitchDecoupler, PlanPanel::addSwitchDecoupler)
  EVT_MENU( ME_AddSwitchAccessory,PlanPanel::addSwitchAccessory)

  EVT_MENU( ME_AddSignal , PlanPanel::addSignal)
  EVT_MENU( ME_AddOutput , PlanPanel::addOutput)
  EVT_MENU( ME_AddBlock  , PlanPanel::addBlock )
  EVT_MENU( ME_AddStage  , PlanPanel::addStage )
  EVT_MENU( ME_AddFBack  , PlanPanel::addFBack )
  EVT_MENU( ME_AddRoute  , PlanPanel::addRoute )
  EVT_MENU( ME_AddTT     , PlanPanel::addTT    )
  EVT_MENU( ME_AddSelTab , PlanPanel::addSelTab)
  EVT_MENU( ME_AddText   , PlanPanel::addText  )

  EVT_MENU( ME_AddRoadStraight , PlanPanel::addTrackStraight )
  EVT_MENU( ME_AddRoadCurve    , PlanPanel::addTrackCurve )
  EVT_MENU( ME_AddRoadDir      , PlanPanel::addTrackDir )

  EVT_MENU( ME_AddRoadLeft     , PlanPanel::addSwitchLeft)
  EVT_MENU( ME_AddRoadRight    , PlanPanel::addSwitchRight)
  EVT_MENU( ME_AddRoadCrossing , PlanPanel::addSwitchCrossing)

  EVT_MENU( ME_AddRoadBlock    , PlanPanel::addBlock )
  EVT_MENU( ME_AddRoadFBack    , PlanPanel::addFBack )

  EVT_MENU( ME_PlanProps , PlanPanel::OnPanelProps )
  EVT_MENU( ME_ModProps , PlanPanel::OnModProps )
  EVT_MENU( ME_AddPlan   , PlanPanel::OnAddPanel )
  EVT_MENU( ME_PanelSelect    , PlanPanel::OnSelect )
  EVT_MENU( ME_RemovePlan, PlanPanel::OnRemovePanel )

END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// LcDlg
// ----------------------------------------------------------------------------

PlanPanel::PlanPanel(wxWindow *parent, int itemsize, double scale, double bktext, int z, iONode zlevel, bool showBorder )
                    :BasePanel( parent, (showBorder?wxSUNKEN_BORDER:wxBORDER_NONE) | wxWANTS_CHARS )
{
  m_ProcessingSelect = false;
  m_Scale = scale;
  m_Bktext = bktext;
  m_zLevel = zlevel;
  m_ItemSize = itemsize;
  m_lastAddedItem = NULL;
  m_ModViewLabel = NULL;
  m_Parent = parent;
  m_LockedRoutes = MapOp.inst();

  m_Z = z;
  m_Ori = wItem.west;
  m_Initialized = false;
  m_ChildTable = new wxHashTable( wxKEY_STRING );
  SetBackgroundColour( *wxWHITE );

  m_MultiAdd = true;

  iONode ini = wGui.getplanpanel(wxGetApp().getIni());
  TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "panel cx=%d cy=%d", wPlanPanel.getcx(ini), wPlanPanel.getcy(ini) );
  SetVirtualSize( (int)(m_ItemSize*m_Scale * wPlanPanel.getcx(ini)), (int)(m_ItemSize*m_Scale * wPlanPanel.getcy(ini)) );
  SetScrollRate( (int)(m_ItemSize*m_Scale), (int)(m_ItemSize*m_Scale) );

  SetToolTip( wxString(wZLevel.getmodid(m_zLevel),wxConvUTF8) + _T(" ") + wxString(wZLevel.gettitle(m_zLevel),wxConvUTF8) );
}

const char* PlanPanel::getZLevelTitle() {
  return wZLevel.gettitle( m_zLevel );
}

void PlanPanel::OnPaint(wxPaintEvent& event)
{
  TraceOp.trc( "plan", TRCLEVEL_DEBUG, __LINE__, 9999, "OnPaint() z=%d", m_Z );

  wxPaintDC dc(this);
  if( !wZLevel.isactive(m_zLevel) && !m_bModView)
    return;


  dc.SetPen( *wxLIGHT_GREY_PEN );

  int x, y;
  GetViewStart( &x, &y );
  TraceOp.trc( "planpanel", TRCLEVEL_DEBUG, __LINE__, 9999, "x_off=%d, y_off=%d", x, y );

  int cx, cy;
  GetClientSize( &cx, &cy );

  double itemsize = (double)(m_ItemSize * m_Scale);
  double dx = x;
  dx = dx / itemsize;
  x = (int)dx;
  double dy = y;
  dy = dy / itemsize;
  y = (int)dy;

  int xcnt = (int)(cx / itemsize);
  int ycnt = (int)(cy / itemsize);
  int i = 0;

  if( wxGetApp().getFrame()->isRaster() ) {
    for( i = 0; i <= ycnt; i++ ) {
      dc.DrawLine( x, (int)(i * itemsize), cx, int(i * itemsize) );
    }
    for( i = 0; i <= xcnt; i++ ) {
      dc.DrawLine( (int)(i * itemsize), y, (int)(i * itemsize), cy );
    }
  }
}

void PlanPanel::OnShow(wxCommandEvent& event) {
  Show( show );
}

void PlanPanel::OnPanelProps(wxCommandEvent& event) {
  iONode ini = (iONode)event.GetClientData();
  if( ini != NULL && StrOp.equals( wModule.name(), NodeOp.getName( ini ) ) ) {
    OnModProps(event);
  }
  else if( m_zLevel != NULL ) {
    PlanPanelProps* dlg = new PlanPanelProps( this, m_zLevel );
    if( wxID_OK == dlg->ShowModal() ) {
      SetToolTip( wxString(wZLevel.getmodid(m_zLevel),wxConvUTF8) + _T(" ") + wxString(wZLevel.gettitle(m_zLevel),wxConvUTF8) );
    }
    dlg->Destroy();
  }
}


void PlanPanel::OnModProps(wxCommandEvent& event) {
  // Get the copied node:
  iONode ini = (iONode)event.GetClientData();
  iONode zlevel = (ini!=NULL ? NodeOp.findNode(ini, wZLevel.name() ) : NULL);

  if( StrOp.equals( wModule.cmd_state, wModule.getcmd(ini) ) ) {
    if( StrOp.equals( wModule.state_shortcut, wModule.getstate(ini) ) && m_ShowSc ) {
      SetBackgroundColor( m_ScRed, m_ScGreen, m_ScBlue, false);
    }
    else {
      /* normal state */
      SetBackgroundColor( m_Red, m_Green, m_Blue, false);
    }
  }
  else if( ini != NULL && zlevel != NULL ) {
    wZLevel.setmodviewcx( m_zLevel, wZLevel.getmodviewcx(zlevel) );
    wZLevel.setmodviewcy( m_zLevel, wZLevel.getmodviewcy(zlevel) );
    reScale(m_Scale);
  }
  else if( ini != NULL && StrOp.equals( wModule.name(), NodeOp.getName( ini ) ) ) {
    // PropertiesDialog:
    ModulePropsDlg* dlg = new ModulePropsDlg( this, ini );
    if( wxID_OK == dlg->ShowModal() ) {
    }
    dlg->Destroy();
    dlg = NULL;
  }
  else {
    iONode cmd = NodeOp.inst( wSysCmd.name(), NULL, ELEMENT_NODE );
    wSysCmd.setcmd( cmd, wSysCmd.getmodule );
    wSysCmd.setid( cmd, wZLevel.getmodid(m_zLevel) );
    wxGetApp().sendToRocrail( cmd, false );
    cmd->base.del(cmd);
  }
}


void PlanPanel::moveSelection(iONode sel) {
  int x  = NodeOp.getInt(sel, "x", 0);
  int y  = NodeOp.getInt(sel, "y", 0);
  int z  = NodeOp.getInt(sel, "z", 0);
  int cx = NodeOp.getInt(sel, "cx", 0);
  int cy = NodeOp.getInt(sel, "cy", 0);
  int destx = NodeOp.getInt(sel, "destx", x);
  int desty = NodeOp.getInt(sel, "desty", y);
  int destz = NodeOp.getInt(sel, "destz", z);

  /* prepare notify RocRail */
  TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "move selection" );
  iONode cmd = NodeOp.inst( wModelCmd.name(), NULL, ELEMENT_NODE );
  wModelCmd.setcmd( cmd, wModelCmd.modify );
  NodeOp.setStr( cmd, "id", "move" );

  // iterate the items in this panel
  m_ChildTable->BeginFind();
  Symbol* item = NULL;
  wxNode* node = (wxNode*)m_ChildTable->Next();
  // model event node
  iONode move = NodeOp.inst( wItem.name(), NULL, ELEMENT_NODE );
  while( node != NULL ) {
    item = (Symbol*)node->GetData();
    iONode props = item->getProperties();
    if( wItem.getx(props) >= x && wItem.getx(props) < x+cx &&
        wItem.gety(props) >= y && wItem.gety(props) < y+cy )
    {
      // create and add a child for this item
      iONode moveditem = NodeOp.inst( NodeOp.getName( props ), NULL, ELEMENT_NODE );
      wItem.setid( moveditem, wItem.getid( props ) );
      NodeOp.setInt( moveditem, "pre_move_x", wItem.getx( props ) );
      NodeOp.setInt( moveditem, "pre_move_y", wItem.gety( props ) );
      NodeOp.setInt( moveditem, "pre_move_z", wItem.getz( props ) );

      wModelCmd.setcmd( moveditem, wModelCmd.move );

      // inform the item to move
      NodeOp.setName( move, NodeOp.getName(props) );
      wItem.setid( move, wItem.getid(props) );
      wItem.setx( move, wItem.getx(props) + destx - x );
      wItem.sety( move, wItem.gety(props) + desty - y );
      wItem.setz( move, destz );
      wModelCmd.setcmd( move, wModelCmd.move );
      item->modelEvent(move);

      // set the new position
      wItem.setx( moveditem, wItem.getx( props ) );
      wItem.sety( moveditem, wItem.gety( props ) );
      wItem.setz( moveditem, wItem.getz( props ) );
      NodeOp.addChild( cmd, moveditem );
    }
    node = (wxNode*)m_ChildTable->Next();
  }

  // TODO: if the move is to another level, remove from this child table and put it in the other

  /* notify RocRail */
  if( NodeOp.getChildCnt(cmd) > 0 ) {
    if( !wxGetApp().isOffline() )
      wxGetApp().sendToRocrail( cmd );

    wxGetApp().pushUndoItem( (iONode)NodeOp.base.clone( cmd ) );
  }

  cmd->base.del(cmd);

  NodeOp.base.del(move);

}


void PlanPanel::copySelection(iONode sel) {
  int x  = NodeOp.getInt(sel, "x", 0);
  int y  = NodeOp.getInt(sel, "y", 0);
  int z  = NodeOp.getInt(sel, "z", 0);
  int cx = NodeOp.getInt(sel, "cx", 0);
  int cy = NodeOp.getInt(sel, "cy", 0);
  int destx = NodeOp.getInt(sel, "destx", x);
  int desty = NodeOp.getInt(sel, "desty", y);
  int destz = NodeOp.getInt(sel, "destz", z);

  /* prepare notify RocRail */
  TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "copy selection" );

  // iterate the items in this panel
  m_ChildTable->BeginFind();
  Symbol* item = NULL;
  wxNode* node = (wxNode*)m_ChildTable->Next();

  while( node != NULL ) {
    item = (Symbol*)node->GetData();
    iONode props = item->getProperties();
    if( wItem.getx(props) >= x && wItem.getx(props) < x+cx &&
        wItem.gety(props) >= y && wItem.gety(props) < y+cy )
    {
      // create and add a child for this item
      iONode copy = (iONode)NodeOp.base.clone( props );
      char newid[256];
      StrOp.fmtb(newid, "%s-copy", wItem.getid( props ) );
      wItem.setid( copy, newid );
      wItem.setx( copy, wItem.getx(props) + destx - x );
      wItem.sety( copy, wItem.gety(props) + desty - y );
      wItem.setz( copy, destz );
      NodeOp.setBool( copy, "copy", True );

      TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "copy [%s] from(%d,%d,%d) to (%d,%d,%d)",
          wItem.getid(copy),
          wItem.getx(props), wItem.gety(props), wItem.getz(props),
          wItem.getx(copy), wItem.gety(copy), wItem.getz(copy) );
      addItemAttr( copy );
    }
    node = (wxNode*)m_ChildTable->Next();
  }

}


void PlanPanel::deleteSelection(iONode sel) {
  int x  = NodeOp.getInt(sel, "x", 0);
  int y  = NodeOp.getInt(sel, "y", 0);
  int cx = NodeOp.getInt(sel, "cx", 0);
  int cy = NodeOp.getInt(sel, "cy", 0);

  /* prepare notify RocRail */
  TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "delete selection" );
  iONode cmd = NodeOp.inst( wModelCmd.name(), NULL, ELEMENT_NODE );
  wModelCmd.setcmd( cmd, wModelCmd.remove );
  NodeOp.setStr( cmd, "id", "delete" );


  // iterate the items in this panel
  m_ChildTable->BeginFind();
  Symbol* item = NULL;
  wxNode* node = (wxNode*)m_ChildTable->Next();

  while( node != NULL ) {
    item = (Symbol*)node->GetData();
    iONode props = item->getProperties();
    if( wItem.getx(props) >= x && wItem.getx(props) < x+cx &&
        wItem.gety(props) >= y && wItem.gety(props) < y+cy )
    {
      NodeOp.addChild( cmd, (iONode)NodeOp.base.clone( props ) );

      if( wxGetApp().isOffline() )
        removeItemFromList( props );
    }
    node = (wxNode*)m_ChildTable->Next();
  }

  /* notify RocRail */
  if( NodeOp.getChildCnt(cmd) > 0 ) {
    wxGetApp().sendToRocrail( cmd );
    wxGetApp().pushUndoItem( (iONode)NodeOp.base.clone( cmd ) );
  }

  cmd->base.del(cmd);

}


void PlanPanel::processSelection(iONode sel) {
  // TODO: process the selection
  char* str = NodeOp.base.toString(sel);
  TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, str );
  StrOp.free(str);

  m_ProcessingSelect = true;

  // check
  int x  = NodeOp.getInt(sel, "x", 0);
  int y  = NodeOp.getInt(sel, "y", 0);
  int cx = NodeOp.getInt(sel, "cx", 0);
  int cy = NodeOp.getInt(sel, "cy", 0);

  if( x >= 0 && y >= 0 && cx >= 0 && cy >= 0 ) {

    switch( NodeOp.getInt(sel, "action", 0) ) {
      case 0:
        // move
        moveSelection(sel);
        break;
      case 1:
        // copy
        copySelection(sel);
        break;
      case 2:
        // delete
        deleteSelection(sel);
        break;
    }
  }

  NodeOp.base.del(sel);

  m_ProcessingSelect = false;
}


void PlanPanel::OnSelect(wxCommandEvent& event) {
  if( m_zLevel != NULL ) {
    iONode sel = (iONode)event.GetClientData();
    if( sel == NULL ) {
      sel = NodeOp.inst( "selection", NULL, ELEMENT_NODE );
      int x, y;
      GetViewStart( &x, &y );
      NodeOp.setInt( sel, "x", m_X+x );
      NodeOp.setInt( sel, "y", m_Y+y );
      NodeOp.setInt( sel, "z", m_Z );
      NodeOp.setInt( sel, "cx", 1 );
      NodeOp.setInt( sel, "cy", 1 );
      NodeOp.setStr( sel, "title", wZLevel.gettitle( m_zLevel ) );
    }

    SelectDialog* dlg = new SelectDialog( this, sel );
    if( wxID_OK == dlg->ShowModal() ) {
      processSelection(dlg->getSelection());
    }
    dlg->Destroy();
    NodeOp.base.del(sel);
  }
}

void PlanPanel::OnMotion(wxMouseEvent& event) {
  if( !wxGetApp().getFrame()->isEditModPlan() )
    return;

  m_mouseX = event.GetX();
  m_mouseY = event.GetY();

  m_X = (int)(m_mouseX / (m_ItemSize*m_Scale));
  m_Y = (int)(m_mouseY / (m_ItemSize*m_Scale));

  int x, y;
  GetViewStart( &x, &y );

  char* text = StrOp.fmt( "(%d,%d)", m_X+x, m_Y+y );
  wxGetApp().getFrame()->setInfoText( text );
  StrOp.free( text );

  wxGetMousePosition( &x, &y );
  TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "drag move x=%d(%d), y=%d(%d)", x, m_dragX, y, m_dragY );

  bool dragging = event.Dragging();


  if( m_bModView && dragging && event.m_leftDown && !m_isDragged ) {
    m_isDragged = true;
    CaptureMouse();
  }

  if( m_bModView && dragging && event.m_leftDown ) {
    int cx = x - m_dragX;
    int cy = y - m_dragY;
    if( cx != 0 || cy != 0 ) {
      wxPoint p = GetPosition();
      wxSize s = GetSize();
      p.x = p.x + cx;
      p.y = p.y + cy;
      SetSize( p.x, p.y, s.GetWidth(), s.GetHeight() );
      m_dragX = x;
      m_dragY = y;
    }
  }


}

void PlanPanel::OnAddPanel(wxCommandEvent& event) {
}

void PlanPanel::OnRemovePanel(wxCommandEvent& event) {
  int action = wxMessageDialog( this, wxGetApp().getMsg("removewarning"), _T("Rocrail"), wxYES_NO | wxICON_EXCLAMATION ).ShowModal();
  if( action == wxID_NO )
    return;

  iONode cmd = NodeOp.inst( wModelCmd.name(), NULL, ELEMENT_NODE );
  wModelCmd.setcmd( cmd, wModelCmd.remove );
  NodeOp.addChild( cmd, (iONode)NodeOp.base.clone( m_zLevel ) );
  wxGetApp().sendToRocrail( cmd );
  cmd->base.del(cmd);
}

void PlanPanel::OnSize(wxSizeEvent& event) {
  TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999,
      "new size cx=%d, cy=%d", event.GetSize().GetWidth(), event.GetSize().GetHeight() );
}

void PlanPanel::setPosition() {
  if( !this->m_bModView ) {
    return;
  }

  double c = m_ItemSize * m_Scale;
  wxPoint p = GetPosition();
  wxSize s = GetSize();

  int x_off, y_off;
  ((BasePanel*)m_Parent)->GetViewStart( &x_off, &y_off );
  int x = wZLevel.getmodviewx( m_zLevel ) - x_off;
  int y = wZLevel.getmodviewy( m_zLevel ) - y_off;

  TraceOp.trc( "panel", TRCLEVEL_INFO, __LINE__, 9999,
      "set pos/size to %d,%d %d,%d", (int)(x*c), (int)(y*c), s.GetWidth(), s.GetHeight() );
  SetSize( (int)(x*c), (int)(y*c), s.GetWidth(), s.GetHeight() );
}



void PlanPanel::OnLeftUp(wxMouseEvent& event) {
  m_mouseX = event.GetX();
  m_mouseY = event.GetY();

  if( m_bModView && m_isDragged ) {
    m_isDragged = false;
    ReleaseMouse();
    double c = m_ItemSize * m_Scale;
    wxPoint p = GetPosition();
    p.x = (int)((c/2+p.x) / c);
    p.y = (int)((c/2+p.y) / c);

    int x_off, y_off;
    ((BasePanel*)m_Parent)->GetViewStart( &x_off, &y_off );

    wxSize s = GetSize();

    int x = p.x - x_off;
    int y = p.y - y_off;

    int pre_move_x = wZLevel.getmodviewx( m_zLevel );
    int pre_move_y = wZLevel.getmodviewy( m_zLevel );
    wZLevel.setmodviewx( m_zLevel, p.x + x_off );
    wZLevel.setmodviewy( m_zLevel, p.y + y_off );

    setPosition();

    TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "drag end x=%d(%d), y=%d(%d)", p.x, m_dragX, p.y, m_dragY );

    /* Notify RocRail. */
    TraceOp.trc( "panel", TRCLEVEL_INFO, __LINE__, 9999,
        "Change position to %d,%d", p.x + x_off, p.y + y_off );
    iONode cmd = NodeOp.inst( wModelCmd.name(), NULL, ELEMENT_NODE );
    wModelCmd.setcmd( cmd, wModelCmd.modify );
    NodeOp.setStr( cmd, "id", "movemod" );

    iONode module = NodeOp.inst( wModule.name(), cmd, ELEMENT_NODE );
    wModule.setid( module, wZLevel.getmodid(m_zLevel) );
    wModule.setcmd( module, wModule.cmd_move );

    NodeOp.setInt( module, "pre_move_x", pre_move_x );
    NodeOp.setInt( module, "pre_move_y", pre_move_y );
    wModule.setx( module, p.x + x_off );
    wModule.sety( module, p.y + y_off );

    NodeOp.addChild( cmd, module );
    wxGetApp().sendToRocrail( cmd );
    wxGetApp().pushUndoItem( (iONode)NodeOp.base.clone( cmd ) );
    cmd->base.del(cmd);
  }
  else {
    m_X = (int)(m_mouseX / (m_ItemSize*m_Scale));
    m_Y = (int)(m_mouseY / (m_ItemSize*m_Scale));

    if( m_lastAddedItem != NULL && wxGetApp().getFrame()->isEditMode() ) {
      wItem.setori(m_lastAddedItem, m_Ori);
      addItemAttr(m_lastAddedItem);
    }
  }

}


void PlanPanel::OnRotate(wxCommandEvent& event) {

  /* Notify RocRail. */
  const char* ori = "north";
  iONode cmd = NodeOp.inst( wModelCmd.name(), NULL, ELEMENT_NODE );
  wModelCmd.setcmd( cmd, wModelCmd.modify );
  iONode module = NodeOp.inst( wModule.name(), cmd, ELEMENT_NODE );
  wModule.setid( module, wZLevel.getmodid(m_zLevel) );

  switch( event.GetId() ) {
    case ME_ModuleNorth:
      wModule.setcmd( module, wModule.cmd_north );
      break;
    case ME_ModuleEast:
      wModule.setcmd( module, wModule.cmd_east );
      break;
    case ME_ModuleSouth:
      wModule.setcmd( module, wModule.cmd_south );
      break;
    case ME_ModuleWest:
      wModule.setcmd( module, wModule.cmd_west );
      break;
  }

  TraceOp.trc( "panel", TRCLEVEL_INFO, __LINE__, 9999, "Rotate to %s", ori );
  NodeOp.addChild( cmd, module );
  wxGetApp().sendToRocrail( cmd );
  cmd->base.del(cmd);
}


void PlanPanel::OnPopup(wxMouseEvent& event) {
    m_mouseX = event.GetX();
    m_mouseY = event.GetY();

    m_X = (int)(m_mouseX / (m_ItemSize*m_Scale));
    m_Y = (int)(m_mouseY / (m_ItemSize*m_Scale));

    if( wxGetApp().getFrame()->isEditMode() ) {
      wxMenu* menuTrack = new wxMenu();
      menuTrack->Append( ME_AddTrackStraight , wxGetApp().getMenu("straight") );
      menuTrack->Append( ME_AddTrackCurve    , wxGetApp().getMenu("curve") );
      menuTrack->Append( ME_AddTrackDir      , wxGetApp().getMenu("dir") );
      menuTrack->Append( ME_AddTrackDirAll   , wxGetApp().getMenu("dirall") );
      menuTrack->Append( ME_AddTrackBuffer   , wxGetApp().getMenu("buffer") );
      menuTrack->Append( ME_AddTrackConnector, wxGetApp().getMenu("zconnector") );

      wxMenu* menuTurnout = new wxMenu();
      menuTurnout->Append( ME_AddSwitchRight    , wxGetApp().getMenu("rightturnout") );
      menuTurnout->Append( ME_AddSwitchLeft     , wxGetApp().getMenu("leftturnout") );
      menuTurnout->Append( ME_AddSwitchCrossing , wxGetApp().getMenu("crossing") );
      menuTurnout->Append( ME_AddSwitchDCrossing, wxGetApp().getMenu("dcrossing") );
      menuTurnout->Append( ME_AddSwitchThreeway , wxGetApp().getMenu("threeway") );
      menuTurnout->Append( ME_AddSwitchDecoupler, wxGetApp().getMenu("decoupler") );
      menuTurnout->Append( ME_AddSwitchAccessory, wxGetApp().getMenu("accessory") );

      wxMenu      menu( wxGetApp().getMenu("additem") );
      menu.Append( -1, wxGetApp().getMenu("track"), menuTrack );
      menu.Append( -1, wxGetApp().getMenu("turnout"), menuTurnout  );
      menu.Append( ME_AddSignal, wxGetApp().getMenu("signal")  );
      menu.Append( ME_AddOutput, wxGetApp().getMenu("output")  );
      menu.Append( ME_AddBlock , wxGetApp().getMenu("block") );
      menu.Append( ME_AddStage , wxGetApp().getMenu("stagingblock") );
      menu.Append( ME_AddFBack , wxGetApp().getMenu("sensor") );
      menu.Append( ME_AddRoute , wxGetApp().getMenu("route") );
      menu.Append( ME_AddTT    , wxGetApp().getMenu("turntable") );
      menu.Append( ME_AddSelTab, wxGetApp().getMenu("seltab") );
      menu.Append( ME_AddText  , wxGetApp().getMenu("text") );

      wxMenu* menuRoad = new wxMenu();
      menuRoad->Append( ME_AddRoadStraight , wxGetApp().getMenu("straight") );
      menuRoad->Append( ME_AddRoadCurve    , wxGetApp().getMenu("curve") );
      menuRoad->Append( ME_AddRoadDir      , wxGetApp().getMenu("dir") );
      menuRoad->Append( ME_AddRoadBlock    , wxGetApp().getMenu("block") );
      menuRoad->Append( ME_AddRoadFBack    , wxGetApp().getMenu("sensor") );
      wxMenu* menuTurnoff = new wxMenu();
      menuTurnoff->Append( ME_AddRoadRight    , wxGetApp().getMenu("right") );
      menuTurnoff->Append( ME_AddRoadLeft     , wxGetApp().getMenu("left") );
      menuTurnoff->Append( ME_AddRoadCrossing , wxGetApp().getMenu("crossing") );
      menuRoad->Append( -1, wxGetApp().getMenu("turnoff"), menuTurnoff );
      menu.Append( -1, wxGetApp().getMenu("road"), menuRoad );

      menu.AppendSeparator();
      menu.Append( ME_PanelSelect, wxGetApp().getMenu("select") );
      menu.AppendSeparator();
      menu.Append( ME_RemovePlan, wxGetApp().getMenu("removepanel") );

      PopupMenu(&menu, event.GetX(), event.GetY() );
    }
    else {
      wxMenu menu( wxGetApp().getMenu("panel") );
      if( this->m_bModView ) {
        wxMenu* menuOrientation = new wxMenu();
        menuOrientation->Append( ME_ModuleRotate, wxGetApp().getMenu("rotate") );
        menuOrientation->AppendSeparator();
        menuOrientation->Append( ME_ModuleNorth, wxGetApp().getMenu("north") );
        menuOrientation->Append( ME_ModuleEast, wxGetApp().getMenu("east") );
        menuOrientation->Append( ME_ModuleSouth, wxGetApp().getMenu("south") );
        menuOrientation->Append( ME_ModuleWest, wxGetApp().getMenu("west") );
        menu.Append( -1, wxGetApp().getMenu("orientation"), menuOrientation );
        menu.AppendSeparator();
        menu.Append( ME_RemovePlan, wxGetApp().getMenu("removepanel") );
        menu.AppendSeparator();
        menu.Append( ME_ModProps, wxGetApp().getMenu("modproperties") );
        menu.AppendSeparator();
      }
      menu.Append( ME_PlanProps, wxGetApp().getMenu("properties") );
      PopupMenu(&menu, event.GetX(), event.GetY() );
    }
}

void PlanPanel::removeItemFromList( iONode item ) {
  iONode model = wxGetApp().getModel();
  const char* name = NodeOp.getName( item );
  const char* dbkey = "";

  //if( !wxGetApp().isOffline() )
  //  return;

  if( StrOp.equals( wBlock.name(), name ) ) {
    dbkey = wBlockList.name();
  }
  else if( StrOp.equals( wStage.name(), name ) ) {
    dbkey = wStageList.name();
  }
  else if( StrOp.equals( wTurntable.name(), name ) ) {
    dbkey = wTurntableList.name();
  }
  else if( StrOp.equals( wSelTab.name(), name ) ) {
    dbkey = wSelTabList.name();
  }
  else if( StrOp.equals( wTrack.name(), name ) ) {
    dbkey = wTrackList.name();
  }
  else if( StrOp.equals( wFeedback.name(), name ) ) {
    dbkey = wFeedbackList.name();
  }
  else if( StrOp.equals( wLoc.name(), name ) ) {
    dbkey = wLocList.name();
  }
  else if( StrOp.equals( wRoute.name(), name ) ) {
    dbkey = wRouteList.name();
  }
  else if( StrOp.equals( wSwitch.name(), name ) ) {
    dbkey = wSwitchList.name();
  }
  else if( StrOp.equals( wSignal.name(), name ) ) {
    dbkey = wSignalList.name();
  }
  else if( StrOp.equals( wOutput.name(), name ) ) {
    dbkey = wOutputList.name();
  }
  else if( StrOp.equals( wText.name(), name ) ) {
    dbkey = wTextList.name();
  }

  iONode db = NodeOp.findNode( model, dbkey );
  if( db != NULL ) {
    int cnt = NodeOp.getChildCnt(db);
    for( int i = 0; i < cnt; i++ ) {
      iONode child = NodeOp.getChild(db, i);
      if( StrOp.equals( wItem.getid(child), wItem.getid(item) ) ) {
        NodeOp.removeChild( db, child );
        TraceOp.trc( "planpanel", TRCLEVEL_INFO, __LINE__, 9999, "item [%s] removed", wItem.getid(item) );
        break;
      }
    }
    //NodeOp.removeChild( db, item );
  }
}


/**
* Add item in localmodel.
*
*/
iONode PlanPanel::addItemInList( iONode item ) {
  iONode model = wxGetApp().getModel();
  const char* name = NodeOp.getName( item );
  const char* dbkey = NULL;

  /*
  if( !wxGetApp().isOffline() )
    return NULL;
  */

  TraceOp.trc( "planpanel", TRCLEVEL_DEBUG, __LINE__, 9999, "Adding %s to model", name );


  if( StrOp.equals( wBlock.name(), name ) ) {
    dbkey = wBlockList.name();
  }
  else if( StrOp.equals( wStage.name(), name ) ) {
    dbkey = wStageList.name();
  }
  else if( StrOp.equals( wTurntable.name(), name ) ) {
    dbkey = wTurntableList.name();
  }
  else if( StrOp.equals( wSelTab.name(), name ) ) {
    dbkey = wSelTabList.name();
  }
  else if( StrOp.equals( wTrack.name(), name ) ) {
    dbkey = wTrackList.name();
  }
  else if( StrOp.equals( wFeedback.name(), name ) ) {
    dbkey = wFeedbackList.name();
  }
  else if( StrOp.equals( wLoc.name(), name ) ) {
    dbkey = wLocList.name();
  }
  else if( StrOp.equals( wRoute.name(), name ) ) {
    dbkey = wRouteList.name();
  }
  else if( StrOp.equals( wSwitch.name(), name ) ) {
    dbkey = wSwitchList.name();
  }
  else if( StrOp.equals( wSignal.name(), name ) ) {
    dbkey = wSignalList.name();
  }
  else if( StrOp.equals( wOutput.name(), name ) ) {
    dbkey = wOutputList.name();
  }
  else if( StrOp.equals( wText.name(), name ) ) {
    dbkey = wTextList.name();
  }
  else if( StrOp.equals( wSchedule.name(), name ) ) {
    dbkey = wScheduleList.name();
  }
  else if( StrOp.equals( wLocation.name(), name ) ) {
    dbkey = wLocationList.name();
  }
  else if( StrOp.equals( wAction.name(), name ) ) {
    dbkey = wActionList.name();
  }

  const char* id = wItem.getid(item);

  if( dbkey != NULL ) {
    iONode db = NodeOp.findNode( model, dbkey );
    if( db == NULL ) {
      db = NodeOp.inst( dbkey, model, ELEMENT_NODE );
      NodeOp.addChild( model, db );
    }
    int cnt = NodeOp.getChildCnt( db );
    bool found = false;
    for( int i = 0; i < cnt && !found; i++ ) {
      iONode child = NodeOp.getChild( db, i );
      found = StrOp.equals( id, wItem.getid(child) );
    }
    if(!found) {
      NodeOp.addChild( db, item );
      wxGetApp().setLocalModelModified(true);
    }
    else
      TraceOp.trc( "planpanel", TRCLEVEL_DEBUG, __LINE__, 9999, "%s[%s] already in model", name,id );

  }
  else {
    TraceOp.trc( "planpanel", TRCLEVEL_WARNING, __LINE__, 9999, "Unknown item %s with dbkey %s",
                 name, dbkey );
  }

  return item;
}


void PlanPanel::addItemAttr( iONode node ) {

  iONode l_clone = (iONode)NodeOp.base.clone(node);

  if( StrOp.equals( wText.name(), NodeOp.getName( node ) ) &&
      (wText.gettext(node) == NULL || StrOp.len( wText.gettext(node) ) == 0) ) {

    wxTextEntryDialog* dlg = new wxTextEntryDialog(m_Parent, wxGetApp().getMenu("entertext") );
    if( wxID_OK == dlg->ShowModal() ) {
      wText.settext( node, dlg->GetValue().mb_str(wxConvUTF8) );
    }
    dlg->Destroy();
  }
  else if( !StrOp.equals( wTrack.name(), NodeOp.getName( node ) ) &&
      ( wItem.getid(node) == NULL || StrOp.len( wItem.getid(node) ) == 0 ) ) {

    wxTextEntryDialog* dlg = new wxTextEntryDialog(m_Parent, wxGetApp().getMenu("enterid") );

    Symbol* item = NULL;
    do {
      if( wxID_OK == dlg->ShowModal() ) {
        wItem.setid( node, dlg->GetValue().mb_str(wxConvUTF8) );
        char key[256];
        itemKey( node, key, NULL );
        item = (Symbol*)m_ChildTable->Get( wxString(key,wxConvUTF8) );
      }
      else {
        // TODO: cleanup node!
        return;
      }
    } while( item != NULL );
    dlg->Destroy();
  }

  // Make a unique ID!
  if( wItem.getid( node ) == NULL || StrOp.len( wItem.getid( node ) ) == 0 ) {
    char* stamp = StrOp.createStampNoDots();
    wItem.setid( node, StrOp.fmt( "%s%s", NodeOp.getName( node ), stamp ) );
    StrOp.free( stamp );
  }

  int x, y;
  GetViewStart( &x, &y );

  if( !NodeOp.getBool( node, "copy", False ) ) {
    wItem.setx( node, m_X+x );
    wItem.sety( node, m_Y+y );
    wItem.setz( node, m_Z );
  }
  else {
    TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "copied item added, z=%d", wItem.getz(node) );
  }

  if( wxGetApp().isOffline() ) {
    addItemInList( node );
  }


  wxCommandEvent evt( wxEVT_COMMAND_MENU_SELECTED, ADDITEM_EVENT );
  evt.SetClientData( node );
  wxPostEvent( this, evt );

  /* Notify RocRail. */
  if( !wxGetApp().isOffline() ) {
    iONode cmd = NodeOp.inst( wModelCmd.name(), NULL, ELEMENT_NODE );
    wModelCmd.setcmd( cmd, wModelCmd.add );
    iONode item = (iONode)NodeOp.base.clone( node );
    NodeOp.addChild( cmd, item );
    wxGetApp().sendToRocrail( cmd );
    cmd->base.del(cmd);
  }

  if( m_lastAddedItem != NULL && m_lastAddedItem != node )
    NodeOp.base.del( m_lastAddedItem );

  m_lastAddedItem = l_clone;

  char* clipboard = StrOp.fmt( "clipboard: %s-%s %s",
      NodeOp.getName(m_lastAddedItem), wItem.gettype(m_lastAddedItem), wItem.getori(m_lastAddedItem) );
  wxGetApp().getFrame()->setDigintText( clipboard );
  StrOp.free( clipboard );

}

void PlanPanel::addTrackStraight(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wTrack.name(), NULL, ELEMENT_NODE );
  wTrack.settype( node, wTrack.straight );
  wItem.setori(node, m_Ori);
  if( event.GetId() == ME_AddRoadStraight )
    wItem.setroad(node, True);
  addItemAttr( node );
}

void PlanPanel::addTrackCurve(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wTrack.name(), NULL, ELEMENT_NODE );
  wTrack.settype( node, wTrack.curve );
  wItem.setori(node, m_Ori);
  if( event.GetId() == ME_AddRoadCurve )
    wItem.setroad(node, True);
  addItemAttr( node );
}

void PlanPanel::addTrackBuffer(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wTrack.name(), NULL, ELEMENT_NODE );
  wTrack.settype( node, wTrack.buffer );
  addItemAttr( node );
}

void PlanPanel::addTrackConnector(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wTrack.name(), NULL, ELEMENT_NODE );
  wTrack.settype( node, wTrack.connector );
  addItemAttr( node );
}

void PlanPanel::addTrackDir(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wTrack.name(), NULL, ELEMENT_NODE );
  wTrack.settype( node, wTrack.dir );
  if( event.GetId() == ME_AddRoadDir )
    wItem.setroad(node, True);
  addItemAttr( node );
}

void PlanPanel::addTrackDirAll(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wTrack.name(), NULL, ELEMENT_NODE );
  wTrack.settype( node, wTrack.dirall );
  addItemAttr( node );
}

void PlanPanel::addSwitchLeft(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wSwitch.name(), NULL, ELEMENT_NODE );
  wSwitch.settype( node, wSwitch.left );
  if( event.GetId() == ME_AddRoadLeft )
    wItem.setroad(node, True);
  addItemAttr( node );
}

void PlanPanel::addSwitchRight(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wSwitch.name(), NULL, ELEMENT_NODE );
  wSwitch.settype( node, wSwitch.right );
  if( event.GetId() == ME_AddRoadRight )
    wItem.setroad(node, True);
  addItemAttr( node );
}

void PlanPanel::addSwitchCrossing(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wSwitch.name(), NULL, ELEMENT_NODE );
  wSwitch.settype( node, wSwitch.crossing );
  if( event.GetId() == ME_AddRoadCrossing )
    wItem.setroad(node, True);
  addItemAttr( node );
}

void PlanPanel::addSwitchDCrossing(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wSwitch.name(), NULL, ELEMENT_NODE );
  wSwitch.settype( node, wSwitch.dcrossing );
  addItemAttr( node );
}

void PlanPanel::addSwitchThreeway(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wSwitch.name(), NULL, ELEMENT_NODE );
  wSwitch.settype( node, wSwitch.threeway );
  addItemAttr( node );
}

void PlanPanel::addSwitchDecoupler(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wSwitch.name(), NULL, ELEMENT_NODE );
  wSwitch.settype( node, wSwitch.decoupler );
  addItemAttr( node );
}

void PlanPanel::addSwitchAccessory(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wSwitch.name(), NULL, ELEMENT_NODE );
  wSwitch.settype( node, wSwitch.accessory );
  addItemAttr( node );
}

void PlanPanel::addSignal(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wSignal.name(), NULL, ELEMENT_NODE );
  addItemAttr( node );
}

void PlanPanel::addOutput(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wOutput.name(), NULL, ELEMENT_NODE );
  addItemAttr( node );
}

void PlanPanel::addBlock(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wBlock.name(), NULL, ELEMENT_NODE );
  if( event.GetId() == ME_AddRoadBlock )
    wItem.setroad(node, True);
  addItemAttr( node );
}

void PlanPanel::addStage(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wStage.name(), NULL, ELEMENT_NODE );
  addItemAttr( node );
}

void PlanPanel::addFBack(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wFeedback.name(), NULL, ELEMENT_NODE );
  if( event.GetId() == ME_AddRoadFBack )
    wItem.setroad(node, True);
  addItemAttr( node );
}

void PlanPanel::addRoute(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wRoute.name(), NULL, ELEMENT_NODE );
  addItemAttr( node );
}

void PlanPanel::addTT(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wTurntable.name(), NULL, ELEMENT_NODE );
  addItemAttr( node );
}

void PlanPanel::addSelTab(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wSelTab.name(), NULL, ELEMENT_NODE );
  addItemAttr( node );
}

void PlanPanel::addText(wxCommandEvent& event) {
  iONode node = NodeOp.inst( wText.name(), NULL, ELEMENT_NODE );
  addItemAttr( node );
}

void PlanPanel::addItemCmd(wxCommandEvent& event) {
  iONode child = (iONode)event.GetClientData();
  addItem( child );
}

void PlanPanel::putChild(void* item) {
  TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "put moved Item at level %d", m_Z );

  Symbol* child = (Symbol*)item;
  char key[256];
  itemKey( child->getProperties(), key, NULL );
  m_ChildTable->Put( wxString(key,wxConvUTF8), child );
  child->setZ(m_Z);
  child->setPanel(this);
  child->Refresh();
}


void PlanPanel::updateItemCmd(wxCommandEvent& event) {
  // Get the copied node from the event object:
  iONode node = (iONode)event.GetClientData();

  TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "update ITEM" );

  char key[256];
  char prevkey[256];
  itemKey( node, key, prevkey );

  Symbol* item = (Symbol*)m_ChildTable->Get( wxString(key,wxConvUTF8) );

  if( item == NULL ) {
    TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "Item with id=%s at level %d not found!", key, wZLevel.getz(m_zLevel) );
  }

  if( node != NULL && StrOp.equals( wModelCmd.name(), NodeOp.getName(node) ) ) {
    TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "update model" );
    modelEvent( node );
  }
  else if( item != NULL ) {
    item->modelEvent( node );
    TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "Item with id=%s is informed", key );

    if( wItem.getz(item->getProperties()) != this->m_Z ) {
      m_ChildTable->Delete(wxString(prevkey,wxConvUTF8));
      // TODO: put it in the right z panel
      TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "move Item to level %d", wItem.getz(item->getProperties()) );
      wxGetApp().getFrame()->putChild(item, wItem.getz(item->getProperties()));
    }
  }
  else if( StrOp.len(prevkey) > 0 && (item = (Symbol*)m_ChildTable->Get( wxString(prevkey,wxConvUTF8) )) ) {
    if( item != NULL ) {
      item->modelEvent( node );
      m_ChildTable->Delete(wxString(prevkey,wxConvUTF8));
      // check the zlevel
      if( wItem.getz(item->getProperties()) == this->m_Z )
        m_ChildTable->Put( wxString(key,wxConvUTF8), item);
      else {
        // TODO: put it in the right z panel
        TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "move Item to level %d", wItem.getz(item->getProperties()) );
        wxGetApp().getFrame()->putChild(item, wItem.getz(item->getProperties()));
      }
    }
    else
      TraceOp.trc( "plan", TRCLEVEL_WARNING, __LINE__, 9999, "Item with id=%s not found!", key );
  }
  else if( !StrOp.equals(NodeOp.getName(node), wLoc.name()) && !StrOp.equals(NodeOp.getName(node), wRoute.name()) ){
    TraceOp.trc( "plan", TRCLEVEL_DEBUG, __LINE__, 9999, "Item with id=%s not found!", key );
  }
  // Cleanup copy:
  NodeOp.base.del( node );
}

void PlanPanel::updateTTItemCmd(wxCommandEvent& event) {
  // Get the copied node from the event object:
  iONode node = (iONode)event.GetClientData();

  m_ChildTable->BeginFind();
  Symbol* item = NULL;
  wxNode* tablenode = (wxNode*)m_ChildTable->Next();
  while( tablenode != NULL ) {
    item = (Symbol*)tablenode->GetData();
    const char* nodeName = NodeOp.getName( item->getProperties() );

    if( StrOp.equals( nodeName, wTurntable.name() ) ) {
      item->modelEvent( node );
    }
    else if( StrOp.equals( nodeName, wTrack.name() ) || StrOp.equals( nodeName, wSignal.name() ) ||
             StrOp.equals( nodeName, wFeedback.name() ) || StrOp.equals( nodeName, wSwitch.name() ) ) {
      TraceOp.trc( "plan", TRCLEVEL_DEBUG, __LINE__, 9999, "feedback event id=[%s]", wFeedback.getid(node) );
      item->blockEvent( wFeedback.getid(node));
    }
    tablenode = (wxNode*)m_ChildTable->Next();
  }

  // Cleanup copy:
  NodeOp.base.del( node );
}

bool PlanPanel::isRouteLocked(const char* id) {
  bool locked = MapOp.haskey( m_LockedRoutes, id );
  TraceOp.trc( "plan", TRCLEVEL_DEBUG, __LINE__, 9999, "Q: route %s is %s", id, locked?"locked":"free" );
  return locked;
}

void PlanPanel::update4Route(wxCommandEvent& event) {
  // Get the copied node from the event object:
  iONode node = (iONode)event.GetClientData();
  const char* routeId = wRoute.getid(node);
  bool locked = wRoute.getstatus(node) == wRoute.status_locked ? true:false;

  TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "event: route %s is %s", routeId, locked?"locked":"free" );

  if( locked && !MapOp.haskey( m_LockedRoutes, routeId ) ) {
    TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "event: add route %s to map", routeId );
    MapOp.put( m_LockedRoutes, routeId, NULL );
  }
  if( !locked && MapOp.haskey( m_LockedRoutes, routeId ) ) {
    TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "event: remove route %s from map", routeId );
    MapOp.remove( m_LockedRoutes, routeId );
  }

  m_ChildTable->BeginFind();
  Symbol* item = NULL;
  wxNode* tablenode = (wxNode*)m_ChildTable->Next();
  while( tablenode != NULL ) {
    item = (Symbol*)tablenode->GetData();
    item->routeEvent( routeId, locked );
    tablenode = (wxNode*)m_ChildTable->Next();
  }

  // Cleanup copy:
  NodeOp.base.del( node );
}

void PlanPanel::addItem( iONode child, bool add2list ) {
  const char* id = wItem.getid( child );
  int z = wItem.getz( child );
  if( id == NULL )
    return;

  if( z == m_Z ) {
    char key[256];
    itemKey( child, key, NULL );

    TraceOp.trc( "plan", TRCLEVEL_DEBUG, __LINE__, 9999, "addItem z=%d key=%s", m_Z, key );
    if( m_ChildTable->Get( wxString(key,wxConvUTF8) ) == NULL ) {

      if(wItem.isshow( child )) {
        Symbol* item = new Symbol( (PlanPanel*)this, child, m_ItemSize, m_Z, m_Scale, m_Bktext );
        item->SetFocus();
        m_ChildTable->Put( wxString(key,wxConvUTF8), item );
        item->SetBackgroundColour( GetBackgroundColour() );
      }
    }


    if( add2list ) {
      /* add the node to the plan... */
      TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "PlanPanel::addItem() addtolist=%s", NodeOp.getName(child) );
      addItemInList( (iONode)NodeOp.base.clone(child));
    }

  }
}

void PlanPanel::addItem( const char* nodename, const char* id, wxWindow* item ) {
  if( id != NULL ) {
    char key[256];
    itemKey( nodename, id, key );

    if( m_ChildTable->Get( wxString(key,wxConvUTF8) ) == NULL ) {
      m_ChildTable->Put( wxString(key,wxConvUTF8), item );
    }
  }
}

void PlanPanel::addMultipleItem(wxCommandEvent& event) {
  iONode node = (iONode)event.GetClientData();

  if( StrOp.equals( NodeOp.getName( node ), wModelCmd.name() ) ) {
    wxGetApp().sendToRocrail( node );
    NodeOp.base.del( node );
    return;
  }

  int cnt = NodeOp.getChildCnt( node );
  for( int i = 0; i < cnt; i++ ) {
    iONode child = NodeOp.getChild( node, i );
    addItem( child, false );
    //wxGetApp().Yield();
  }
  char* text = StrOp.fmt( "%d items", m_ChildTable->GetCount() );
  wxGetApp().getFrame()->setInfoText( text );
  StrOp.free( text );

  reScale( m_Scale );
}

void PlanPanel::modelEvent( iONode node ) {
  const char* name = NodeOp.getName( node );
  const char* id   = wItem.getid( node );
  TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "PlanPanel::modelEvent() name=%s", name );

  if( StrOp.equals( wModelCmd.name(), name ) ){
    const char* cmd = wModelCmd.getcmd( node );
    TraceOp.trc( "plan", TRCLEVEL_DEBUG, __LINE__, 9999, "PlanPanel::modelEvent() cmd=%s", cmd );

    if( StrOp.equals( wModelCmd.add, cmd ) ) {
      int cnt = NodeOp.getChildCnt( node );
      for( int i = 0; i < cnt; i++ ) {
        iONode child = NodeOp.getChild( node, i );

        // Make a copy of the node for using it out of this scope:
        child = (iONode)NodeOp.base.clone( child );

        if( StrOp.equals( wZLevel.name(), NodeOp.getName( child ) ) ) {
          wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, ZLEVEL_EVENT_ADD );
          event.SetClientData( child );
          wxPostEvent( wxGetApp().getFrame(), event );
        }
        else {
          wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, ADDITEM_EVENT );
          event.SetClientData( child );
          wxPostEvent( this, event );
        }
      }
      // clould be a loc or street:
      // ToDo: add

    }
    else if( StrOp.equals( wModelCmd.remove, cmd ) ) {
      int cnt = NodeOp.getChildCnt( node );
      int i = 0;
      TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "PlanPanel::modelEvent() remove childs=%d", cnt );
      for( i = 0; i < cnt; i++ ) {
        iONode child = NodeOp.getChild( node, i );

        if( StrOp.equals( wZLevel.name(), NodeOp.getName( child ) ) ) {
          wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, ZLEVEL_EVENT_REMOVE );
          event.SetClientData( NodeOp.base.clone( child ) );
          wxPostEvent( wxGetApp().getFrame(), event );
          continue;
        }

        char key[256];
        itemKey( child, key, NULL );

        TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "PlanPanel::modelEvent() id=%s", key );

        Symbol* item = (Symbol*)m_ChildTable->Get( wxString(key,wxConvUTF8) );
        if( item != NULL ) {
          TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "removing item id=%s from table...", key );
          m_ChildTable->Delete( wxString(key,wxConvUTF8) );
          item->Show( false );
          // TODO: when this item is destroyed a refresh will try to access it which crashes the GUI.
          // For now we will leave it heare as zombie.;-)
          //item->Destroy();
          removeItemFromList(child);
          Refresh();
          char* text = StrOp.fmt( "%d items", m_ChildTable->GetCount() );
          wxGetApp().getFrame()->setInfoText( text );
          StrOp.free( text );
        }
      }
    }
    else if( StrOp.equals( wModelCmd.modify, cmd ) ) {
      // could be a new item...
      // get child's id:
      int cnt = NodeOp.getChildCnt( node );
      for( int i = 0; i < cnt; i++ ) {
        iONode child = NodeOp.getChild( node, i );
        id = wItem.getid( child );
        TraceOp.trc( "plan", TRCLEVEL_DEBUG, __LINE__, 9999, "Item with id=%s will be informed", id );
        // check for loc or street nodes:
        // TODO: check for existence -> modify or add...
        wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, UPDATEITEM_EVENT );
        event.SetClientData( node->base.clone( child ) );
        wxPostEvent( this, event );
      }
    }
  }
  else if( id != NULL ) {
    char key[256];
    itemKey( node, key, NULL );
    TraceOp.trc( "plan", TRCLEVEL_DEBUG, __LINE__, 9999, "itemKey=\"%s\"", key );
    Symbol* item = (Symbol*)m_ChildTable->Get( wxString(key,wxConvUTF8) );
    if( item != NULL ) {
      TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "update item=[%s]", key );
      wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, UPDATEITEM_EVENT );
      // Make a copy of the node for using it out of this scope:
      event.SetClientData( node->base.clone( node ) );
      wxPostEvent( this, event );
    }
    if( StrOp.equals( wFeedback.name(), name ) ) {
      // could be invisible feedback for a turntable...
      TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "feedback event item=[%s]", key );
      wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, UPDATETT_EVENT );
      // Make a copy of the node for using it out of this scope:
      event.SetClientData( node->base.clone( node ) );
      wxPostEvent( this, event );
    }
    else if( StrOp.equals( wRoute.name(), name ) ) {
      // could be invisible feedback for a turntable...
      TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "route event item=[%s]", key );
      wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, UPDATE4ROUTE_EVENT );
      // Make a copy of the node for using it out of this scope:
      event.SetClientData( node->base.clone( node ) );
      wxPostEvent( this, event );
    }
  }
  else {
    TraceOp.trc( "plan", TRCLEVEL_WARNING, __LINE__, 9999, "PlanPanel::modelEvent() UNKNOWN name=\"%s\"", name );
  }
}


bool PlanPanel::isBlockOccupied( const char* id ) {
  char key[256];
  itemKey( wBlock.name(), id, key );
  Symbol* item = (Symbol*)m_ChildTable->Get( wxString(key,wxConvUTF8) );
  if( item != NULL ) {
    const char* locid = wBlock.getlocid( item->getProperties() );
    if( locid != NULL && StrOp.len( locid ) > 0 )
      return true;
  }

  itemKey( wFeedback.name(), id, key );
  item = (Symbol*)m_ChildTable->Get( wxString(key,wxConvUTF8) );
  if( item != NULL ) {
    if( StrOp.equals(wFeedback.name(), NodeOp.getName(item->getProperties())) && wFeedback.isstate(item->getProperties()) )
      return true;
  }

  return false;
}

bool PlanPanel::isBlockReserved( const char* id ) {
  char key[256];
  itemKey( wBlock.name(), id, key );
  Symbol* item = (Symbol*)m_ChildTable->Get( wxString(key,wxConvUTF8) );
  if( item != NULL ) {
    return wBlock.isreserved( item->getProperties() )?true:false;
  }
  return false;
}

void PlanPanel::addItems( iONode node ) {

  if( node == NULL )
    return;

  if( m_MultiAdd ) {
    wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, ADDMULTIPLEITEM_EVENT );
    event.SetClientData( node );
    // send in a thread-safe way
    wxPostEvent( this, event );
    ThreadOp.sleep( 10 );
  }
  else {
    if( StrOp.equals( NodeOp.getName( node ), wModelCmd.name() ) ) {
      wxGetApp().sendToRocrail( node );
      NodeOp.base.del( node );
      return;
    }

    int cnt = NodeOp.getChildCnt( node );
    for( int i = 0; i < cnt; i++ ) {
      iONode child = NodeOp.getChild( node, i );
      // create any type of command event here
      wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, ADDITEM_EVENT );
      event.SetClientData( child );
      // send in a thread-safe way
      wxPostEvent( this, event );
      ThreadOp.sleep( 0 );
    }
  }
}

void PlanPanel::setShow( bool showPanel ) {
  show = showPanel;
  // create any type of command event here
  wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, SHOW_EVENT );
  // send in a thread-safe way
  wxPostEvent( this, event );
}


static void initPlan( PlanPanel* o ) {
  iONode model = wxGetApp().getModel();
  if( model == NULL ) {
    TraceOp.trc( "plan", TRCLEVEL_WARNING, __LINE__, 9999, "initPlan() model == NULL" );
    return;
  }
  RocGuiFrame* frame = wxGetApp().getFrame();
  TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "Title = %s", wPlan.gettitle( model ) );
  TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "Level = %d(%d)", o->m_Z, wZLevel.getz(o->m_zLevel) );

  if( wPlan.gettxlist( model ) != NULL ) {
    iONode images = NodeOp.inst( wTextList.name(), NULL, ELEMENT_NODE);
    iONode texts  = NodeOp.inst( wTextList.name(), NULL, ELEMENT_NODE);

    iONode txlist = wPlan.gettxlist( model );
    int cnt = NodeOp.getChildCnt(txlist);
    for( int i = 0; i < cnt; i++ ) {
      iONode txt = NodeOp.getChild( txlist, i );
      if( StrOp.endsWithi(wText.gettext(txt), ".png") )
        NodeOp.addChild(images, txt);
      else
        NodeOp.addChild(texts, txt);

    }
    o->addItems( images );
    o->addItems( texts );
  }

  o->addItems( wPlan.gettklist( model ) );
  o->addItems( wPlan.getbklist( model ) );
  o->addItems( wPlan.getfblist( model ) );
  o->addItems( wPlan.getswlist( model ) );
  o->addItems( wPlan.getcolist( model ) );
  o->addItems( wPlan.getsglist( model ) );
  o->addItems( wPlan.getttlist( model ) );
  o->addItems( wPlan.getseltablist( model ) );
  o->addItems( wPlan.getstlist( model ) );
  o->addItems( wPlan.getsblist( model ) );

  iONode cmd = NodeOp.inst( wModelCmd.name(), NULL, ELEMENT_NODE );
  wModelCmd.setcmd( cmd, wModelCmd.fstat );
  o->addItems( cmd );

  TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "initPlan() READY" );
}

static void initRunner( void* threadinst ) {
  iOThread th = (iOThread)threadinst;
  PlanPanel* o = (PlanPanel*)ThreadOp.getParm( th );

  TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "initRunner() 0x%08X", o );
  initPlan( o );
}

void PlanPanel::init( bool modview ) {
  m_bModView = modview;
  m_InitThread = ThreadOp.inst( NULL, &initRunner, this );
  ThreadOp.start( m_InitThread );
}

void PlanPanel::clean() {
  if( !wxGetApp().isOffline() )
    return;

  Show(false);
  m_ChildTable->BeginFind();
  Symbol* item = NULL;
  wxNode* node = (wxNode*)m_ChildTable->Next();
  while( node != NULL ) {
    item = (Symbol*)node->GetData();
    item->disable();
    RemoveChild(item);
    node = (wxNode*)m_ChildTable->Next();
  }
  m_ChildTable->Clear();
  Refresh();
  Show(true);
}

void PlanPanel::blockEvent( const char* id ) {
  if( m_ProcessingSelect )
    return;

  m_ChildTable->BeginFind();
  Symbol* item = NULL;
  wxNode* node = (wxNode*)m_ChildTable->Next();
  while( node != NULL ) {
    item = (Symbol*)node->GetData();
    item->blockEvent( id );
    node = (wxNode*)m_ChildTable->Next();
  }
}

void PlanPanel::reScale( double scale ) {
  m_Scale = scale;
  Scroll( 0, 0 );

  iONode ini = wGui.getplanpanel(wxGetApp().getIni());
  SetVirtualSize( (int)(m_ItemSize*m_Scale * wPlanPanel.getcx(ini)), (int)(m_ItemSize*m_Scale * wPlanPanel.getcy(ini)) );
  SetScrollRate( (int)(m_ItemSize*m_Scale), (int)(m_ItemSize*m_Scale) );
  //Show(false);

  int cx = wZLevel.getmodviewcx( m_zLevel );
  int cy = wZLevel.getmodviewcy( m_zLevel );

  bool needSize = false;

  if( wZLevel.getmodviewcx( m_zLevel ) == 0 || wZLevel.getmodviewcy( m_zLevel ) == 0 )
    needSize = true;

  m_ChildTable->BeginFind();
  Symbol* item = NULL;
  wxNode* node = (wxNode*)m_ChildTable->Next();
  while( node != NULL ) {
    item = (Symbol*)node->GetData();
    item->SetBackgroundColour( GetBackgroundColour() );
    item->reScale( m_Scale );

    if( needSize ) {
      iONode iProps = item->getProperties();
      if( wItem.getx(iProps) > cx )
        cx = wItem.getx(iProps);
      if( wItem.gety(iProps) > cy )
        cy = wItem.gety(iProps);
    }

    node = (wxNode*)m_ChildTable->Next();
  }

  if( needSize ) {
    TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "needSize cx=%d, cy=%d", cx, cy );
    cx++; cy++;
  }


  TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "size2fit cx=%d, cy=%d", cx, cy );

  if( m_bModView ) {
  /*
    if( m_ModViewLabel == NULL )
      m_ModViewLabel = new wxStaticText( this, wxID_ANY, wxString(getZLevelTitle(),wxConvUTF8), wxPoint( 0, 0 ), wxDefaultSize, 0 );
  */

    double fcx = (double)m_ItemSize * m_Scale * (double)cx;
    double fcy = (double)m_ItemSize * m_Scale * (double)cy;
    TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "size2fit fcx=%.2f, fcy=%.2f", fcx, fcy );

    /* panel border correction */
    fcx += 5;
    fcy += 5;

    SetVirtualSize( 0,0 );
    //SetVirtualSize((int)fcx, (int)fcy);
    SetSize((int)fcx, (int)fcy);
    //SetClientSize((int)(m_ItemSize*m_Scale*cx), (int)(m_ItemSize*m_Scale*cy));
    EnableScrolling( false, false );
  }

  this->Refresh(true);
  //Show(true);
}

char* PlanPanel::itemKey( iONode item, char* key, char* prevkey ) {
  StrOp.fmtb( key, "%s_%s", NodeOp.getName(item), wItem.getid(item) );
  if( prevkey != NULL ) {
    prevkey[0] = '\0';
    if( StrOp.len(wItem.getprev_id(item)) > 0 )
      StrOp.fmtb( prevkey, "%s_%s", NodeOp.getName(item), wItem.getprev_id(item) );
  }
  return key;
}
char* PlanPanel::itemKey( const char* nodename, const char* id, char* key ) {
  StrOp.fmtb( key, "%s_%s", nodename, id );
  return key;
}

bool PlanPanel::SetBackgroundColor(int red, int green, int blue, bool savecolor) {
  if( savecolor ) {
    m_Red   = red;
    m_Green = green;
    m_Blue  = blue;
  }
  wxColor color((byte)red, (byte)green, (byte)blue);
  return SetBackgroundColour(color);
}

void PlanPanel::SetScBackgroundColor(int red, int green, int blue, bool showSc) {
  m_ScRed   = red;
  m_ScGreen = green;
  m_ScBlue  = blue;
  m_ShowSc  = showSc;
}

void PlanPanel::refresh(bool eraseBackground ) {
  Refresh(eraseBackground);
}

void PlanPanel::OnLeftDown(wxMouseEvent& event) {
  int x;
  int y;

  Raise();

  wxGetMousePosition( &x, &y );

  m_dragX = x;
  m_dragY = y;
  TraceOp.trc( "plan", TRCLEVEL_INFO, __LINE__, 9999, "drag start x=%d, y=%d", x, y );

}


BasePanel* PlanPanel::updateZLevel(iONode zlevel) {
  wZLevel.setmodviewx( m_zLevel, wZLevel.getmodviewx(zlevel) );
  wZLevel.setmodviewy( m_zLevel, wZLevel.getmodviewy(zlevel) );
  reScale( m_Scale );
  setPosition();
  return this;
}


const char* PlanPanel::getZID() {
  return wZLevel.getmodid( m_zLevel );
}
