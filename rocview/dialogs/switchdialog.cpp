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
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "switchdialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "switchdialog.h"
#include "actionsctrldlg.h"

////@begin XPM images
////@end XPM images

#include "rocrail/wrapper/public/Switch.h"
#include "rocrail/wrapper/public/SwitchList.h"
#include "rocrail/wrapper/public/Item.h"
#include "rocrail/wrapper/public/Plan.h"
#include "rocrail/wrapper/public/Feedback.h"
#include "rocrail/wrapper/public/ModelCmd.h"
#include "rocrail/wrapper/public/Block.h"
#include "rocrail/wrapper/public/AccessoryCtrl.h"
#include "rocview/public/guiapp.h"

/*!
 * SwitchDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SwitchDialog, wxDialog )

/*!
 * SwitchDialog event table definition
 */

BEGIN_EVENT_TABLE( SwitchDialog, wxDialog )

////@begin SwitchDialog event table entries
    EVT_LISTBOX( ID_LISTBOX_SW, SwitchDialog::OnListboxSwSelected )

    EVT_BUTTON( ID_BUTTON_SW_NEW, SwitchDialog::OnButtonSwNewClick )

    EVT_BUTTON( ID_BUTTON_SW_DELETE, SwitchDialog::OnButtonSwDeleteClick )

    EVT_BUTTON( ID_BUTTON_SW_DOC, SwitchDialog::OnButtonSwDocClick )

    EVT_COMBOBOX( ID_COMBOBOX_SW_TYPE, SwitchDialog::OnComboboxSwTypeSelected )

    EVT_BUTTON( ID_SWITCH_ACTIONS, SwitchDialog::OnSwitchActionsClick )

    EVT_CHECKBOX( ID_SWITCH_SINGLEGATE, SwitchDialog::OnSwitchSinglegateClick )

    EVT_CHECKBOX( ID_CHECKBOX_SW_DELAY, SwitchDialog::OnCheckboxSwDelayClick )

    EVT_BUTTON( wxID_CANCEL, SwitchDialog::OnCancelClick )

    EVT_BUTTON( wxID_OK, SwitchDialog::OnOkClick )

    EVT_BUTTON( wxID_APPLY, SwitchDialog::OnApplyClick )

////@end SwitchDialog event table entries
    EVT_MENU( ID_PANEL_SW_GENERAL, SwitchDialog::OnSelectPage )

END_EVENT_TABLE()

/*!
 * SwitchDialog constructors
 */

SwitchDialog::SwitchDialog( )
{
}

SwitchDialog::SwitchDialog( wxWindow* parent, iONode p_Props )
{
  m_TabAlign = wxGetApp().getTabAlign();
  Create(parent, -1, wxGetApp().getMsg("turnouttable") );

  m_Props = p_Props;

  initLabels();
  m_IndexPanel->GetSizer()->Layout();
  m_GeneralPanel->GetSizer()->Layout();
  m_LocationPanel->GetSizer()->Layout();
  m_InterfacePanel->GetSizer()->Layout();
  m_WireringPanel->GetSizer()->Layout();
  m_Notebook->Fit();
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
  initIndex();

  if( m_Props != NULL ) {
    initValues();
    //m_Notebook->SetSelection( 1 );
    wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, ID_PANEL_SW_GENERAL );
    wxPostEvent( this, event );
  }

}

void SwitchDialog::OnSelectPage(wxCommandEvent& event) {
  m_Notebook->SetSelection( 1 );
}


void SwitchDialog::initLabels() {
  m_Notebook->SetPageText( 0, wxGetApp().getMsg( "index" ) );
  m_Notebook->SetPageText( 1, wxGetApp().getMsg( "general" ) );
  m_Notebook->SetPageText( 2, wxGetApp().getMsg( "position" ) );
  m_Notebook->SetPageText( 3, wxGetApp().getMsg( "interface" ) );
  m_Notebook->SetPageText( 4, wxGetApp().getMsg( "wiring" ) );

  // Index
  m_New->SetLabel( wxGetApp().getMsg( "new" ) );
  m_Delete->SetLabel( wxGetApp().getMsg( "delete" ) );
  m_Doc->SetLabel( wxGetApp().getMsg( "doc_report" ) );

  // General
  m_LabelID->SetLabel( wxGetApp().getMsg( "id" ) );
  m_labDesc->SetLabel( wxGetApp().getMsg( "description" ) );
  m_labBlockID->SetLabel( wxGetApp().getMsg( "blockid" ) );
  m_labRouteIDs->SetLabel( wxGetApp().getMsg( "routeids" ) );
  m_LabelLocked->SetLabel( wxGetApp().getMsg( "locked" ) );
  m_LabelState->SetLabel( wxGetApp().getMsg( "state" ) );
  m_LabelType->SetLabel( wxGetApp().getMsg( "type" ) );
  m_labAccNr->SetLabel( wxGetApp().getMsg( "accnr" ) );
  m_Dir->SetLabel( wxGetApp().getMsg( "dir" ) );
  m_Dir->SetString( 0, wxGetApp().getMsg( "left" ) );
  m_Dir->SetString( 1, wxGetApp().getMsg( "right" ) );
  m_Raster->SetLabel( wxGetApp().getMsg( "swtype_raster" ) );
  m_Road->SetLabel( wxGetApp().getMsg( "road" ) );
  m_Show->SetLabel( wxGetApp().getMsg( "show" ) );
  m_RectCrossing->SetLabel( wxGetApp().getMsg( "rectangular" ) );
  m_SavePosBox->SetLabel( wxGetApp().getMsg( "savepos" ) );
  m_SavePosBox->SetString( 0, wxGetApp().getMsg( "none" ) );
  m_SavePosBox->SetString( 1, wxGetApp().getMsg( "straight" ) );
  m_SavePosBox->SetString( 2, wxGetApp().getMsg( "thrown" ) );
  m_SavePosBox->SetString( 3, wxGetApp().getMsg( "left" ) );
  m_SavePosBox->SetString( 4, wxGetApp().getMsg( "right" ) );
  m_Actions->SetLabel( wxGetApp().getMsg( "actions" )+_T("...") );

  m_BlockID->Append( _T("") );
  iONode model = wxGetApp().getModel();
  if( model != NULL ) {
    iONode bklist = wPlan.getbklist( model );
    if( bklist != NULL ) {
      int cnt = NodeOp.getChildCnt( bklist );
      for( int i = 0; i < cnt; i++ ) {
        iONode bk = NodeOp.getChild( bklist, i );
        m_BlockID->Append( wxString(wBlock.getid( bk ),wxConvUTF8), bk );
      }
    }
    iONode fblist = wPlan.getfblist( model );
    if( fblist != NULL ) {
      int cnt = NodeOp.getChildCnt( fblist );
      for( int i = 0; i < cnt; i++ ) {
        iONode fb = NodeOp.getChild( fblist, i );
        m_BlockID->Append( wxString(wFeedback.getid( fb ),wxConvUTF8), fb );
      }
    }
  }

  m_Type->Append( wxGetApp().getMsg( "turnout" ), (void*)wSwitch.left );
  m_Type->Append( wxGetApp().getMsg( "crossing" ), (void*)wSwitch.crossing );
  m_Type->Append( wxGetApp().getMsg( "ccrossing" ), (void*)wSwitch.ccrossing );
  m_Type->Append( wxGetApp().getMsg( "dcrossing" ), (void*)wSwitch.dcrossing );
  m_Type->Append( wxGetApp().getMsg( "threeway" ), (void*)wSwitch.threeway );
  m_Type->Append( wxGetApp().getMsg( "decoupler" ), (void*)wSwitch.decoupler );
  m_Type->Append( wxGetApp().getMsg( "accessory" ), (void*)wSwitch.accessory );

  // Location
  m_LabelX->SetLabel( wxGetApp().getMsg( "x" ) );
  m_LabelY->SetLabel( wxGetApp().getMsg( "y" ) );
  m_LabelZ->SetLabel( wxGetApp().getMsg( "z" ) );
  m_ori->SetLabel( wxGetApp().getMsg( "orientation" ) );
  m_ori->SetString( 0, wxGetApp().getMsg( "north" ) );
  m_ori->SetString( 1, wxGetApp().getMsg( "east" ) );
  m_ori->SetString( 2, wxGetApp().getMsg( "south" ) );
  m_ori->SetString( 3, wxGetApp().getMsg( "west" ) );

  // Interface
  m_Labeliid->SetLabel( wxGetApp().getMsg( "iid" ) );
  m_Label_Bus->SetLabel( wxGetApp().getMsg( "bus" ) );
  m_LabelAddress1->SetLabel( wxGetApp().getMsg( "address" ) );
  m_LabelPort1->SetLabel( wxGetApp().getMsg( "port" ) );
  m_labGate->SetLabel( wxGetApp().getMsg( "gate" ) );
  m_Gate->SetString( 0, wxGetApp().getMsg( "red" ) );
  m_Gate->SetString( 1, wxGetApp().getMsg( "green" ) );

  m_SingleGate->SetLabel( wxGetApp().getMsg( "singlegate" ) );
  m_Invert1->SetLabel( wxGetApp().getMsg( "invert" ) );
  m_LabelAddress2->SetLabel( wxGetApp().getMsg( "address" ) );
  m_LabelPort2->SetLabel( wxGetApp().getMsg( "port" ) );
  m_Invert2->SetLabel( wxGetApp().getMsg( "invert" ) );
  m_LabelProt->SetLabel( wxGetApp().getMsg( "protocol" ) );
  m_ActDelay->SetLabel( wxGetApp().getMsg( "switchtime" ) );

  m_Prot->Clear();
  m_Prot->Append(_T("Default"));
  m_Prot->Append(_T("Motorola"));
  m_Prot->Append(_T("NMRA-DCC"));
  m_Prot->Append(_T("Dinamo MDD"));
  m_Prot->Append(_T("Dinamo OM32"));
  m_Prot->Append(_T("Dinamo DO"));
  m_Prot->Append(_T("Dinamo VO"));



  // Wirering
  m_LabelFb1R->SetLabel( wxGetApp().getMsg( "sensor_red" ) );
  m_LabelFb1G->SetLabel( wxGetApp().getMsg( "sensor_green" ) );
  m_LabelFb2R->SetLabel( wxGetApp().getMsg( "sensor_red" ) );
  m_LabelFb2G->SetLabel( wxGetApp().getMsg( "sensor_green" ) );
  m_Fb1Rinvert->SetLabel( wxGetApp().getMsg( "invert" ) );
  m_Fb1Ginvert->SetLabel( wxGetApp().getMsg( "invert" ) );
  m_Fb2Rinvert->SetLabel( wxGetApp().getMsg( "invert" ) );
  m_Fb2Ginvert->SetLabel( wxGetApp().getMsg( "invert" ) );

  m_labCTCMotor->SetLabel( wxGetApp().getMsg( "motor" ) );
  m_labCTCIID->SetLabel( wxGetApp().getMsg( "iid" ) );
  m_labCTCAddress->SetLabel( wxGetApp().getMsg( "address" ) );
  m_labCTCCmdAtOn->SetLabel( wxGetApp().getMsg( "command" ) );

  m_labCTCLED->SetLabel( wxGetApp().getMsg( "led" ) );
  m_labCTCIIDLED->SetLabel( wxGetApp().getMsg( "iid" ) );
  m_labCTCAddrLED->SetLabel( wxGetApp().getMsg( "address" ) );
  m_labCTCPortLED->SetLabel( wxGetApp().getMsg( "port" ) );
  m_labCTCGateLED->SetLabel( wxGetApp().getMsg( "gate" ) );
  m_labCTCSwitchLED->SetLabel( wxGetApp().getMsg( "turnout" ) );

  // Control
  m_ActivateCtrl->SetLabel( wxGetApp().getMsg( "activate" ) );
  m_labCtrlInterval->SetLabel( wxGetApp().getMsg( "interval" ) );
  m_labCtrlDelay->SetLabel( wxGetApp().getMsg( "delay" ) );
  m_labCtrlIntervalSec->SetLabel( wxGetApp().getMsg( "seconds" ) );
  m_labCtrlDelaySec->SetLabel( wxGetApp().getMsg( "seconds" ) );
  m_labCtrlRoutes->SetLabel( wxGetApp().getMsg( "routestolock" ) );

  // Buttons
  m_OK->SetLabel( wxGetApp().getMsg( "ok" ) );
  m_Cancel->SetLabel( wxGetApp().getMsg( "cancel" ) );
  m_Apply->SetLabel( wxGetApp().getMsg( "apply" ) );
}


void SwitchDialog::initIndex() {
  TraceOp.trc( "app", TRCLEVEL_INFO, __LINE__, 9999, "InitIndex" );
  iONode l_Props = m_Props;
  m_List->Clear();
  iONode model = wxGetApp().getModel();
  if( model != NULL ) {
    iONode swlist = wPlan.getswlist( model );
    if( swlist != NULL ) {
      int cnt = NodeOp.getChildCnt( swlist );
      for( int i = 0; i < cnt; i++ ) {
        iONode sw = NodeOp.getChild( swlist, i );
        const char* id = wSwitch.getid( sw );
        if( id != NULL ) {
          m_List->Append( wxString(id,wxConvUTF8) );
        }
      }
      if( l_Props != NULL ) {
        m_List->SetStringSelection( wxString(wSwitch.getid( l_Props ),wxConvUTF8) );
        m_List->SetFirstItem( wxString(wSwitch.getid( l_Props ),wxConvUTF8) );
        m_Props = l_Props;
      }

    }
  }
}


void SwitchDialog::initValues() {
  char* title = StrOp.fmt( "%s %s", (const char*)wxGetApp().getMsg("turnout").mb_str(wxConvUTF8), wSwitch.getid( m_Props ) );
  SetTitle( wxString(title,wxConvUTF8) );
  StrOp.free( title );

  // General
  m_ID->SetValue( wxString(wSwitch.getid( m_Props ),wxConvUTF8) );
  m_Desc->SetValue( wxString(wSwitch.getdesc( m_Props ),wxConvUTF8) );
  m_Locked->SetValue( wSwitch.getlocid( m_Props )==NULL ?_T("<free>"):wxString(wSwitch.getlocid( m_Props ),wxConvUTF8) );
  m_State->SetValue( wSwitch.getstate( m_Props )==NULL ?_T("<undef>"):wxString(wSwitch.getstate( m_Props ),wxConvUTF8)  );
  m_Switched->SetValue( wxString::Format(_T("%d"), wSwitch.getswitched( m_Props )) );
  m_Road->SetValue( wItem.isroad( m_Props )?true:false );
  m_Show->SetValue( wItem.isshow( m_Props )?true:false );
  m_RectCrossing->SetValue( wSwitch.isrectcrossing( m_Props )?true:false );
  m_BlockID->SetStringSelection( wSwitch.getblockid( m_Props ) == NULL ?
                                _T(""):wxString(wSwitch.getblockid( m_Props ),wxConvUTF8)  );
  m_RouteIDs->SetValue( wxString(wItem.getrouteids( m_Props ),wxConvUTF8) );

  int type = 0;
  if( StrOp.equals( wSwitch.right, wSwitch.gettype( m_Props ) ) )
    type = 0;
  else if( StrOp.equals( wSwitch.left, wSwitch.gettype( m_Props ) ) )
    type = 0;
  else if( StrOp.equals( wSwitch.crossing, wSwitch.gettype( m_Props ) ) )
    type = 1;
  else if( StrOp.equals( wSwitch.ccrossing, wSwitch.gettype( m_Props ) ) )
    type = 2;
  else if( StrOp.equals( wSwitch.dcrossing, wSwitch.gettype( m_Props ) ) )
    type = 3;
  else if( StrOp.equals( wSwitch.threeway, wSwitch.gettype( m_Props ) ) )
    type = 4;
  else if( StrOp.equals( wSwitch.decoupler, wSwitch.gettype( m_Props ) ) )
    type = 5;
  else if( StrOp.equals( wSwitch.accessory, wSwitch.gettype( m_Props ) ) )
    type = 6;

  m_Type->Select( type );

  m_AccNr->SetValue(wSwitch.getaccnr( m_Props ));

  m_Dir->SetSelection( 0 );
  if( wSwitch.isdir( m_Props ) || StrOp.equals( wSwitch.right, wSwitch.gettype( m_Props ) ) )
    m_Dir->SetSelection( 1 );

  m_Raster->SetValue( StrOp.equals( wSwitch.swtype_raster, wSwitch.getswtype(m_Props) ) );

  m_SavePosBox->SetSelection( 0 );
  if( StrOp.equals( wSwitch.straight, wSwitch.getsavepos( m_Props ) ) )
    m_SavePosBox->SetSelection( 1 );
  else if( StrOp.equals( wSwitch.turnout, wSwitch.getsavepos( m_Props ) ) )
    m_SavePosBox->SetSelection( 2 );
  else if( StrOp.equals( wSwitch.left, wSwitch.getsavepos( m_Props ) ) )
    m_SavePosBox->SetSelection( 3 );
  else if( StrOp.equals( wSwitch.right, wSwitch.getsavepos( m_Props ) ) )
    m_SavePosBox->SetSelection( 4 );

  // Location
  char* str = StrOp.fmt( "%d", wSwitch.getx( m_Props ) );
  m_x->SetValue( wxString(str,wxConvUTF8) ); StrOp.free( str );
  str = StrOp.fmt( "%d", wSwitch.gety( m_Props ) );
  m_y->SetValue( wxString(str,wxConvUTF8) ); StrOp.free( str );
  str = StrOp.fmt( "%d", wSwitch.getz( m_Props ) );
  m_z->SetValue( wxString(str,wxConvUTF8) ); StrOp.free( str );
  if( StrOp.equals( wItem.north, wSwitch.getori( m_Props ) ) )
    m_ori->SetSelection( 0 );
  else if( StrOp.equals( wItem.east, wSwitch.getori( m_Props ) ) )
    m_ori->SetSelection( 1 );
  else if( StrOp.equals( wItem.south, wSwitch.getori( m_Props ) ) )
    m_ori->SetSelection( 2 );
  else
    m_ori->SetSelection( 3 );

  // Interface
  m_iid->SetValue( wxString(wSwitch.getiid( m_Props ),wxConvUTF8) );
  str = StrOp.fmt( "%d", wSwitch.getbus(m_Props) );
  m_Bus->SetValue( wxString(str,wxConvUTF8) ); StrOp.free( str );
  str = StrOp.fmt( "%d", wSwitch.getaddr1(m_Props) );
  m_Address1->SetValue( wxString(str,wxConvUTF8) ); StrOp.free( str );
  str = StrOp.fmt( "%d", wSwitch.getport1(m_Props) );
  m_Port1->SetValue( wxString(str,wxConvUTF8) ); StrOp.free( str );
  m_Gate->SetSelection( wSwitch.getgate1(m_Props) );
  m_Invert1->SetValue( wSwitch.isinv(m_Props) );
  m_SingleGate->SetValue( wSwitch.issinglegate(m_Props) );
  m_Gate->Enable(m_SingleGate->IsChecked());

  str = StrOp.fmt( "%d", wSwitch.getaddr2(m_Props) );
  m_Address2->SetValue( wxString(str,wxConvUTF8) ); StrOp.free( str );
  str = StrOp.fmt( "%d", wSwitch.getport2(m_Props) );
  m_Port2->SetValue( wxString(str,wxConvUTF8) ); StrOp.free( str );
  m_Invert2->SetValue( wSwitch.isinv2(m_Props));

  // only threeways and double crossings have 2 units:
  if( type == 3 || type == 4 ) {
    m_Address2->Enable(true);
    m_Port2->Enable(true);
    m_Invert2->Enable(true);
  }
  else {
    m_Address2->Enable(false);
    m_Port2->Enable(false);
    m_Invert2->Enable(false);
  }

  str = StrOp.fmt( "%d", wSwitch.getdelay(m_Props) );
  m_Delay->SetValue( wxString(str,wxConvUTF8) ); StrOp.free( str );
  m_ActDelay->SetValue( wSwitch.isactdelay(m_Props));
  m_Delay->Enable( m_ActDelay->GetValue() );

  if( StrOp.equals( wSwitch.prot_DEF, wSwitch.getprot( m_Props ) ) )
    m_Prot->SetSelection( 0 );
  else if( StrOp.equals( wSwitch.prot_M, wSwitch.getprot( m_Props ) ) )
    m_Prot->SetSelection( 1 );
  else if( StrOp.equals( wSwitch.prot_N, wSwitch.getprot( m_Props ) ) )
    m_Prot->SetSelection( 2 );
  else if( StrOp.equals( wSwitch.prot_MDD, wSwitch.getprot( m_Props ) ) )
    m_Prot->SetSelection( 3 );
  else if( StrOp.equals( wSwitch.prot_OM32, wSwitch.getprot( m_Props ) ) )
    m_Prot->SetSelection( 4 );
  else if( StrOp.equals( wSwitch.prot_DO, wSwitch.getprot( m_Props ) ) )
    m_Prot->SetSelection( 5 );
  else if( StrOp.equals( wSwitch.prot_VO, wSwitch.getprot( m_Props ) ) )
    m_Prot->SetSelection( 6 );

  // Wirerings
  iONode model = wxGetApp().getModel();
  m_Fb1R->Append( _T("-") );
  m_Fb1G->Append( _T("-") );
  m_Fb2R->Append( _T("-") );
  m_Fb2G->Append( _T("-") );
  if( model != NULL ) {
    iONode fblist = wPlan.getfblist( model );
    if( fblist != NULL ) {
      int cnt = NodeOp.getChildCnt( fblist );
      for( int i = 0; i < cnt; i++ ) {
        iONode fb = NodeOp.getChild( fblist, i );
        const char* id = wFeedback.getid( fb );
        m_Fb1R->Append( wxString(id,wxConvUTF8) );
        m_Fb1G->Append( wxString(id,wxConvUTF8) );
        m_Fb2R->Append( wxString(id,wxConvUTF8) );
        m_Fb2G->Append( wxString(id,wxConvUTF8) );
      }
    }
  }

  if( wSwitch.getfbR (m_Props)==NULL || StrOp.len(wSwitch.getfbR (m_Props)) == 0 )
    m_Fb1R->SetStringSelection( _T("-") );
  else
    m_Fb1R->SetStringSelection( wxString(wSwitch.getfbR (m_Props),wxConvUTF8) );
  if( wSwitch.getfbG (m_Props)==NULL || StrOp.len(wSwitch.getfbG (m_Props)) == 0 )
    m_Fb1G->SetStringSelection( _T("-") );
  else
    m_Fb1G->SetStringSelection( wxString(wSwitch.getfbG (m_Props),wxConvUTF8) );

  if( wSwitch.getfb2R (m_Props)==NULL || StrOp.len(wSwitch.getfb2R (m_Props)) == 0 )
    m_Fb2R->SetStringSelection( _T("-") );
  else
    m_Fb2R->SetStringSelection( wxString(wSwitch.getfb2R (m_Props),wxConvUTF8) );
  if( wSwitch.getfb2G (m_Props)==NULL || StrOp.len(wSwitch.getfb2G (m_Props)) == 0 )
    m_Fb2G->SetStringSelection( _T("-") );
  else
    m_Fb2G->SetStringSelection( wxString(wSwitch.getfb2G (m_Props),wxConvUTF8) );

  m_Fb1Rinvert->SetValue( wSwitch.isfbRinv(m_Props) );
  m_Fb1Ginvert->SetValue( wSwitch.isfbGinv(m_Props) );
  m_Fb2Rinvert->SetValue( wSwitch.isfb2Rinv(m_Props) );
  m_Fb2Ginvert->SetValue( wSwitch.isfb2Ginv(m_Props) );
  
  m_CTCIID1->SetValue( wxString(wSwitch.getctciid1(m_Props),wxConvUTF8));
  m_CTCIID2->SetValue( wxString(wSwitch.getctciid2(m_Props),wxConvUTF8));
  m_CTCAddr1->SetValue( wSwitch.getctcaddr1(m_Props));
  m_CTCAddr2->SetValue( wSwitch.getctcaddr2(m_Props));
  if( wSwitch.isctcflip1(m_Props) )
    m_CTCOn1->SetSelection( 2 );
  else
    m_CTCOn1->SetSelection( wSwitch.isctccmdon1(m_Props)?0:1 );
  if( wSwitch.isctcflip2(m_Props) )
    m_CTCOn2->SetSelection( 2 );
  else
    m_CTCOn2->SetSelection( wSwitch.isctccmdon2(m_Props)?0:1 );

  m_CTCIIDLED1->SetValue( wxString(wSwitch.getctciidled1(m_Props),wxConvUTF8));
  m_CTCIIDLED2->SetValue( wxString(wSwitch.getctciidled2(m_Props),wxConvUTF8));
  m_CTCAddrLED1->SetValue( wSwitch.getctcaddrled1(m_Props));
  m_CTCAddrLED2->SetValue( wSwitch.getctcaddrled2(m_Props));
  m_CTCPortLED1->SetValue( wSwitch.getctcportled1(m_Props));
  m_CTCPortLED2->SetValue( wSwitch.getctcportled2(m_Props));
  m_CTCGateLED1->SetValue( wSwitch.getctcgateled1(m_Props));
  m_CTCGateLED2->SetValue( wSwitch.getctcgateled2(m_Props));
  m_CTCAsSwitchLED1->SetValue( wSwitch.isctcasswitchled1(m_Props)?true:false);
  m_CTCAsSwitchLED2->SetValue( wSwitch.isctcasswitchled2(m_Props)?true:false);


  
  // Accessory Control
  iONode accctrl = wSwitch.getaccessoryctrl(m_Props);
  if( accctrl == NULL ) {
    accctrl = NodeOp.inst( wAccessoryCtrl.name(), m_Props, ELEMENT_NODE);
    NodeOp.addChild( m_Props, accctrl );
  }
  m_ActivateCtrl->SetValue( wAccessoryCtrl.isactive(accctrl) ? true:false );
  m_CtrlInterval->SetValue( wAccessoryCtrl.getinterval(accctrl) );
  m_CtrlDelay->SetValue( wAccessoryCtrl.getdelay(accctrl) );
  m_CtrlRoutes->SetValue(wxString(wAccessoryCtrl.getlockroutes(accctrl),wxConvUTF8)  );
  

}


bool SwitchDialog::evaluate() {
  if( m_Props == NULL )
    return false;

  if( m_ID->GetValue().Len() == 0 ) {
    wxMessageDialog( this, wxGetApp().getMsg("invalidid"), _T("Rocrail"), wxOK | wxICON_ERROR ).ShowModal();
    m_ID->SetValue( wxString(wSwitch.getid( m_Props ),wxConvUTF8) );
    return false;
  }

  // General
  wItem.setprev_id( m_Props, wItem.getid(m_Props) );
  wSwitch.setid( m_Props, m_ID->GetValue().mb_str(wxConvUTF8) );
  wSwitch.setdesc( m_Props, m_Desc->GetValue().mb_str(wxConvUTF8) );
  wSwitch.setblockid( m_Props, m_BlockID->GetStringSelection().mb_str(wxConvUTF8) );
  wItem.setrouteids( m_Props, m_RouteIDs->GetValue().mb_str(wxConvUTF8) );

  int dir = m_Dir->GetSelection();

  int type = m_Type->GetSelection();
  switch( type ) {
    case 0:
      if( dir == 1 )
        wSwitch.settype( m_Props, wSwitch.right );
      else
        wSwitch.settype( m_Props, wSwitch.left );
      break;
    case 1:
      wSwitch.settype( m_Props, wSwitch.crossing );
      break;
    case 2:
      wSwitch.settype( m_Props, wSwitch.ccrossing );
      break;
    case 3:
      wSwitch.settype( m_Props, wSwitch.dcrossing );
      break;
    case 4:
      wSwitch.settype( m_Props, wSwitch.threeway );
      break;
    case 5:
      wSwitch.settype( m_Props, wSwitch.decoupler );
      break;
    case 6:
      wSwitch.settype( m_Props, wSwitch.accessory );
      break;
  }

  wSwitch.setaccnr( m_Props, m_AccNr->GetValue() );

  int savepos = m_SavePosBox->GetSelection();
  switch( savepos ) {
    case 0:
      wSwitch.setsavepos( m_Props, "none" );
      break;
    case 1:
      wSwitch.setsavepos( m_Props, wSwitch.straight );
      break;
    case 2:
      wSwitch.setsavepos( m_Props, wSwitch.turnout );
      break;
    case 3:
      wSwitch.setsavepos( m_Props, wSwitch.left );
      break;
    case 4:
      wSwitch.setsavepos( m_Props, wSwitch.right );
      break;
  }

  wSwitch.setdir( m_Props, dir==1?True:False );

  if( m_Raster->IsChecked() )
    wSwitch.setswtype( m_Props, wSwitch.swtype_raster );
  else
    wSwitch.setswtype( m_Props, wSwitch.swtype_default );

  wItem.setroad( m_Props, m_Road->IsChecked()?True:False );
  wItem.setshow( m_Props, m_Show->IsChecked()?True:False );
  wSwitch.setrectcrossing( m_Props, m_RectCrossing->IsChecked()?True:False );

  // Location
  wSwitch.setx( m_Props, atoi( m_x->GetValue().mb_str(wxConvUTF8) ) );
  wSwitch.sety( m_Props, atoi( m_y->GetValue().mb_str(wxConvUTF8) ) );
  wSwitch.setz( m_Props, atoi( m_z->GetValue().mb_str(wxConvUTF8) ) );
  int ori = m_ori->GetSelection();
  if( ori == 0 )
    wSwitch.setori( m_Props, wItem.north );
  else if( ori == 1 )
    wSwitch.setori( m_Props, wItem.east );
  else if( ori == 2 )
    wSwitch.setori( m_Props, wItem.south );
  else if( ori == 3 )
    wSwitch.setori( m_Props, wItem.west );

  // Interface
  wSwitch.setiid( m_Props, m_iid->GetValue().mb_str(wxConvUTF8) );
  wSwitch.setbus( m_Props, atoi( m_Bus->GetValue().mb_str(wxConvUTF8) ) );
  wSwitch.setaddr1( m_Props, atoi( m_Address1->GetValue().mb_str(wxConvUTF8) ) );
  wSwitch.setport1( m_Props, atoi( m_Port1->GetValue().mb_str(wxConvUTF8) ) );
  wSwitch.setgate1( m_Props, m_Gate->GetSelection() );
  wSwitch.setinv( m_Props, m_Invert1->GetValue()?True:False );
  wSwitch.setsinglegate( m_Props, m_SingleGate->GetValue()?True:False );
  wSwitch.setaddr2( m_Props, atoi( m_Address2->GetValue().mb_str(wxConvUTF8) ) );
  wSwitch.setport2( m_Props, atoi( m_Port2->GetValue().mb_str(wxConvUTF8) ) );
  wSwitch.setinv2( m_Props, m_Invert2->GetValue()?True:False );
  wSwitch.setactdelay( m_Props, m_ActDelay->GetValue()?True:False );
  wSwitch.setdelay( m_Props, atoi( m_Delay->GetValue().mb_str(wxConvUTF8) ) );

  if( m_Prot->GetSelection() == 0 )
    wSwitch.setprot( m_Props, wSwitch.prot_DEF );
  else if( m_Prot->GetSelection() == 1 )
    wSwitch.setprot( m_Props, wSwitch.prot_M );
  else if( m_Prot->GetSelection() == 2 )
    wSwitch.setprot( m_Props, wSwitch.prot_N );
  else if( m_Prot->GetSelection() == 3 )
    wSwitch.setprot( m_Props, wSwitch.prot_MDD );
  else if( m_Prot->GetSelection() == 4 )
    wSwitch.setprot( m_Props, wSwitch.prot_OM32 );
  else if( m_Prot->GetSelection() == 5 )
    wSwitch.setprot( m_Props, wSwitch.prot_DO );
  else if( m_Prot->GetSelection() == 6 )
    wSwitch.setprot( m_Props, wSwitch.prot_VO );

  // Wiring
  if( StrOp.equals( "-", m_Fb1R->GetStringSelection().mb_str(wxConvUTF8) ) )
    wSwitch.setfbR( m_Props, "" );
  else
    wSwitch.setfbR( m_Props, m_Fb1R->GetStringSelection().mb_str(wxConvUTF8) );
  if( StrOp.equals( "-", m_Fb1G->GetStringSelection().mb_str(wxConvUTF8) ) )
    wSwitch.setfbG( m_Props, "" );
  else
    wSwitch.setfbG( m_Props, m_Fb1G->GetStringSelection().mb_str(wxConvUTF8) );

  if( StrOp.equals( "-", m_Fb2R->GetStringSelection().mb_str(wxConvUTF8) ) )
    wSwitch.setfb2R( m_Props, "" );
  else
    wSwitch.setfb2R( m_Props, m_Fb2R->GetStringSelection().mb_str(wxConvUTF8) );
  if( StrOp.equals( "-", m_Fb2G->GetStringSelection().mb_str(wxConvUTF8) ) )
    wSwitch.setfb2G( m_Props, "" );
  else
    wSwitch.setfb2G( m_Props, m_Fb2G->GetStringSelection().mb_str(wxConvUTF8) );

  wSwitch.setfbRinv(m_Props, m_Fb1Rinvert->GetValue()?True:False );
  wSwitch.setfbGinv(m_Props, m_Fb1Ginvert->GetValue()?True:False );
  wSwitch.setfb2Rinv(m_Props, m_Fb2Rinvert->GetValue()?True:False );
  wSwitch.setfb2Ginv(m_Props, m_Fb2Ginvert->GetValue()?True:False );

  wSwitch.setctciid1(m_Props, m_CTCIID1->GetValue().mb_str(wxConvUTF8));
  wSwitch.setctciid2(m_Props, m_CTCIID2->GetValue().mb_str(wxConvUTF8));
  wSwitch.setctcaddr1(m_Props, m_CTCAddr1->GetValue());
  wSwitch.setctcaddr2(m_Props, m_CTCAddr2->GetValue());
  wSwitch.setctccmdon1(m_Props, m_CTCOn1->GetSelection()==0?True:False);
  wSwitch.setctccmdon2(m_Props, m_CTCOn2->GetSelection()==0?True:False);
  wSwitch.setctcflip1(m_Props, m_CTCOn1->GetSelection()==2?True:False);
  wSwitch.setctcflip2(m_Props, m_CTCOn2->GetSelection()==2?True:False);

  wSwitch.setctciidled1(m_Props, m_CTCIIDLED1->GetValue().mb_str(wxConvUTF8));
  wSwitch.setctciidled2(m_Props, m_CTCIIDLED2->GetValue().mb_str(wxConvUTF8));
  wSwitch.setctcaddrled1(m_Props, m_CTCAddrLED1->GetValue());
  wSwitch.setctcaddrled2(m_Props, m_CTCAddrLED2->GetValue());
  wSwitch.setctcportled1(m_Props, m_CTCPortLED1->GetValue());
  wSwitch.setctcportled2(m_Props, m_CTCPortLED2->GetValue());
  wSwitch.setctcgateled1(m_Props, m_CTCGateLED1->GetValue());
  wSwitch.setctcgateled2(m_Props, m_CTCGateLED2->GetValue());
  wSwitch.setctcasswitchled1(m_Props, m_CTCAsSwitchLED1->IsChecked()?True:False);
  wSwitch.setctcasswitchled2(m_Props, m_CTCAsSwitchLED2->IsChecked()?True:False);

  // Accessory Control
  iONode accctrl = wSwitch.getaccessoryctrl(m_Props);
  wAccessoryCtrl.setactive(accctrl, m_ActivateCtrl->IsChecked() ? True:False );
  wAccessoryCtrl.setinterval(accctrl, m_CtrlInterval->GetValue() );
  wAccessoryCtrl.setdelay(accctrl, m_CtrlDelay->GetValue() );
  wAccessoryCtrl.setlockroutes(accctrl, m_CtrlRoutes->GetValue().mb_str(wxConvUTF8) );
  
  return true;
}


/*!
 * SwitchDialog creator
 */

bool SwitchDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SwitchDialog member initialisation
    m_Notebook = NULL;
    m_IndexPanel = NULL;
    m_List = NULL;
    m_New = NULL;
    m_Delete = NULL;
    m_Doc = NULL;
    m_GeneralPanel = NULL;
    m_LabelID = NULL;
    m_ID = NULL;
    m_labDesc = NULL;
    m_Desc = NULL;
    m_labBlockID = NULL;
    m_BlockID = NULL;
    m_labRouteIDs = NULL;
    m_RouteIDs = NULL;
    m_LabelLocked = NULL;
    m_Locked = NULL;
    m_LabelState = NULL;
    m_State = NULL;
    m_labSwitched = NULL;
    m_Switched = NULL;
    m_LabelType = NULL;
    m_Type = NULL;
    m_labAccNr = NULL;
    m_AccNr = NULL;
    m_LabelDir = NULL;
    m_Dir = NULL;
    m_SavePosBox = NULL;
    m_Raster = NULL;
    m_Road = NULL;
    m_Show = NULL;
    m_RectCrossing = NULL;
    m_Actions = NULL;
    m_LocationPanel = NULL;
    m_LabelX = NULL;
    m_x = NULL;
    m_LabelY = NULL;
    m_y = NULL;
    m_LabelZ = NULL;
    m_z = NULL;
    m_ori = NULL;
    m_InterfacePanel = NULL;
    m_Labeliid = NULL;
    m_iid = NULL;
    m_Label_Bus = NULL;
    m_Bus = NULL;
    m_LabelProt = NULL;
    m_Prot = NULL;
    m_LabelAddress1 = NULL;
    m_Address1 = NULL;
    m_LabelPort1 = NULL;
    m_Port1 = NULL;
    m_labGate = NULL;
    m_Gate = NULL;
    m_SingleGate = NULL;
    m_Invert1 = NULL;
    m_LabelAddress2 = NULL;
    m_Address2 = NULL;
    m_LabelPort2 = NULL;
    m_Port2 = NULL;
    m_Invert2 = NULL;
    m_ActDelay = NULL;
    m_Delay = NULL;
    m_WireringPanel = NULL;
    m_WiringBox = NULL;
    m_LabelFb1R = NULL;
    m_Fb1R = NULL;
    m_Fb1Rinvert = NULL;
    m_LabelFb1G = NULL;
    m_Fb1G = NULL;
    m_Fb1Ginvert = NULL;
    m_LabelFb2R = NULL;
    m_Fb2R = NULL;
    m_Fb2Rinvert = NULL;
    m_LabelFb2G = NULL;
    m_Fb2G = NULL;
    m_Fb2Ginvert = NULL;
    m_CTCBox = NULL;
    m_labCTCMotor = NULL;
    m_labCTCIID = NULL;
    m_labCTCAddress = NULL;
    m_labCTCCmdAtOn = NULL;
    m_labCTCAddr1 = NULL;
    m_CTCIID1 = NULL;
    m_CTCAddr1 = NULL;
    m_CTCOn1 = NULL;
    m_labCTCAddr2 = NULL;
    m_CTCIID2 = NULL;
    m_CTCAddr2 = NULL;
    m_CTCOn2 = NULL;
    m_labCTCLED = NULL;
    m_labCTCIIDLED = NULL;
    m_labCTCAddrLED = NULL;
    m_labCTCPortLED = NULL;
    m_labCTCGateLED = NULL;
    m_labCTCSwitchLED = NULL;
    m_labCTCLED1 = NULL;
    m_CTCIIDLED1 = NULL;
    m_CTCAddrLED1 = NULL;
    m_CTCPortLED1 = NULL;
    m_CTCGateLED1 = NULL;
    m_CTCAsSwitchLED1 = NULL;
    m_labCTCLED2 = NULL;
    m_CTCIIDLED2 = NULL;
    m_CTCAddrLED2 = NULL;
    m_CTCPortLED2 = NULL;
    m_CTCGateLED2 = NULL;
    m_CTCAsSwitchLED2 = NULL;
    m_ControlPanel = NULL;
    m_ActivateCtrl = NULL;
    m_labCtrlInterval = NULL;
    m_CtrlInterval = NULL;
    m_labCtrlIntervalSec = NULL;
    m_labCtrlDelay = NULL;
    m_CtrlDelay = NULL;
    m_labCtrlDelaySec = NULL;
    m_labCtrlRoutes = NULL;
    m_CtrlRoutes = NULL;
    m_Cancel = NULL;
    m_OK = NULL;
    m_Apply = NULL;
////@end SwitchDialog member initialisation

////@begin SwitchDialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end SwitchDialog creation
    return true;
}

/*!
 * Control creation for SwitchDialog
 */

void SwitchDialog::CreateControls()
{
////@begin SwitchDialog content construction
    SwitchDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    m_Notebook = new wxNotebook( itemDialog1, ID_NOTEBOOK_SW, wxDefaultPosition, wxDefaultSize, m_TabAlign );

    m_IndexPanel = new wxPanel( m_Notebook, ID_PANEL_SW_INDEX, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    m_IndexPanel->SetSizer(itemBoxSizer5);

    wxArrayString m_ListStrings;
    m_List = new wxListBox( m_IndexPanel, ID_LISTBOX_SW, wxDefaultPosition, wxDefaultSize, m_ListStrings, wxLB_SINGLE|wxLB_ALWAYS_SB|wxLB_SORT );
    itemBoxSizer5->Add(m_List, 1, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer7 = new wxFlexGridSizer(0, 3, 0, 0);
    itemBoxSizer5->Add(itemFlexGridSizer7, 0, wxGROW|wxALL, 5);
    m_New = new wxButton( m_IndexPanel, ID_BUTTON_SW_NEW, _("New"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(m_New, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Delete = new wxButton( m_IndexPanel, ID_BUTTON_SW_DELETE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(m_Delete, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Doc = new wxButton( m_IndexPanel, ID_BUTTON_SW_DOC, _("Doc"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(m_Doc, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Notebook->AddPage(m_IndexPanel, _("Index"));

    m_GeneralPanel = new wxPanel( m_Notebook, ID_PANEL_SW_GENERAL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    m_GeneralPanel->SetSizer(itemBoxSizer12);

    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer12->Add(itemBoxSizer13, 0, wxALIGN_TOP|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer14 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer14->AddGrowableCol(1);
    itemBoxSizer13->Add(itemFlexGridSizer14, 0, wxGROW|wxALL, 5);
    m_LabelID = new wxStaticText( m_GeneralPanel, wxID_STATIC_SW_ID, _("id"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(m_LabelID, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_ID = new wxTextCtrl( m_GeneralPanel, ID_TEXTCTRL_SW_ID, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(m_ID, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_labDesc = new wxStaticText( m_GeneralPanel, wxID_ANY, _("Description"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(m_labDesc, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_Desc = new wxTextCtrl( m_GeneralPanel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(m_Desc, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_labBlockID = new wxStaticText( m_GeneralPanel, wxID_ANY, _("BlockID"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(m_labBlockID, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxArrayString m_BlockIDStrings;
    m_BlockID = new wxComboBox( m_GeneralPanel, ID_COMBOBOX_SW_BLOCKID, _T(""), wxDefaultPosition, wxDefaultSize, m_BlockIDStrings, wxCB_READONLY );
    itemFlexGridSizer14->Add(m_BlockID, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_labRouteIDs = new wxStaticText( m_GeneralPanel, wxID_ANY, _("Route ID's"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(m_labRouteIDs, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_RouteIDs = new wxTextCtrl( m_GeneralPanel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(m_RouteIDs, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_LabelLocked = new wxStaticText( m_GeneralPanel, wxID_STATIC_SW_LOCKED, _("locked"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(m_LabelLocked, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_Locked = new wxTextCtrl( m_GeneralPanel, ID_TEXTCTRL_SW_LOCKED, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
    m_Locked->Enable(false);
    itemFlexGridSizer14->Add(m_Locked, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_LabelState = new wxStaticText( m_GeneralPanel, wxID_STATIC_SW_STATE, _("state"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(m_LabelState, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_State = new wxTextCtrl( m_GeneralPanel, ID_TEXTCTRL_SW_STATE, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
    m_State->Enable(false);
    itemFlexGridSizer14->Add(m_State, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_labSwitched = new wxStaticText( m_GeneralPanel, wxID_STATIC, _("Switched"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(m_labSwitched, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_Switched = new wxTextCtrl( m_GeneralPanel, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE );
    m_Switched->Enable(false);
    itemFlexGridSizer14->Add(m_Switched, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_LabelType = new wxStaticText( m_GeneralPanel, wxID_STATIC_SW_TYPE, _("type"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(m_LabelType, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxArrayString m_TypeStrings;
    m_Type = new wxComboBox( m_GeneralPanel, ID_COMBOBOX_SW_TYPE, _T(""), wxDefaultPosition, wxDefaultSize, m_TypeStrings, wxCB_DROPDOWN );
    itemFlexGridSizer14->Add(m_Type, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_labAccNr = new wxStaticText( m_GeneralPanel, wxID_ANY, _("accnr"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(m_labAccNr, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_AccNr = new wxSpinCtrl( m_GeneralPanel, wxID_ANY, _T("1"), wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 1, 99, 1 );
    itemFlexGridSizer14->Add(m_AccNr, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_LabelDir = new wxStaticText( m_GeneralPanel, wxID_STATIC_SW_DIR, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(m_LabelDir, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_DirStrings;
    m_DirStrings.Add(_("&Left"));
    m_DirStrings.Add(_("&Right"));
    m_Dir = new wxRadioBox( m_GeneralPanel, ID_RADIOBOX_SW_DIR, _("Direction"), wxDefaultPosition, wxDefaultSize, m_DirStrings, 1, wxRA_SPECIFY_ROWS );
    m_Dir->SetSelection(0);
    itemFlexGridSizer14->Add(m_Dir, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxStaticLine* itemStaticLine35 = new wxStaticLine( m_GeneralPanel, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
    itemBoxSizer12->Add(itemStaticLine35, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer36 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer12->Add(itemBoxSizer36, 0, wxALIGN_TOP|wxALL, 5);
    wxArrayString m_SavePosBoxStrings;
    m_SavePosBoxStrings.Add(_("&none"));
    m_SavePosBoxStrings.Add(_("&straight"));
    m_SavePosBoxStrings.Add(_("&thrown"));
    m_SavePosBoxStrings.Add(_("&left"));
    m_SavePosBoxStrings.Add(_("&right"));
    m_SavePosBox = new wxRadioBox( m_GeneralPanel, wxID_ANY, _("Save position"), wxDefaultPosition, wxDefaultSize, m_SavePosBoxStrings, 1, wxRA_SPECIFY_COLS );
    m_SavePosBox->SetSelection(0);
    itemBoxSizer36->Add(m_SavePosBox, 0, wxALIGN_LEFT|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer38 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer36->Add(itemFlexGridSizer38, 0, wxGROW|wxALL, 5);
    m_Raster = new wxCheckBox( m_GeneralPanel, wxID_ANY, _("Raster"), wxDefaultPosition, wxDefaultSize, 0 );
    m_Raster->SetValue(false);
    itemFlexGridSizer38->Add(m_Raster, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Road = new wxCheckBox( m_GeneralPanel, wxID_ANY, _("Road"), wxDefaultPosition, wxDefaultSize, 0 );
    m_Road->SetValue(false);
    itemFlexGridSizer38->Add(m_Road, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Show = new wxCheckBox( m_GeneralPanel, ID_CHECKBOX_SW_SHOW, _("show"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE );
    m_Show->SetValue(false);
    itemFlexGridSizer38->Add(m_Show, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_RectCrossing = new wxCheckBox( m_GeneralPanel, wxID_ANY, _("Rectangular"), wxDefaultPosition, wxDefaultSize, 0 );
    m_RectCrossing->SetValue(false);
    itemFlexGridSizer38->Add(m_RectCrossing, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Actions = new wxButton( m_GeneralPanel, ID_SWITCH_ACTIONS, _("Actions..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer38->Add(m_Actions, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Notebook->AddPage(m_GeneralPanel, _("General"));

    m_LocationPanel = new wxPanel( m_Notebook, ID_PANEL_SW_LOCATION, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer45 = new wxBoxSizer(wxHORIZONTAL);
    m_LocationPanel->SetSizer(itemBoxSizer45);

    wxFlexGridSizer* itemFlexGridSizer46 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer45->Add(itemFlexGridSizer46, 0, wxALIGN_TOP|wxALL, 5);
    m_LabelX = new wxStaticText( m_LocationPanel, wxID_STATIC_SW_X, _("x"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer46->Add(m_LabelX, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_x = new wxTextCtrl( m_LocationPanel, ID_TEXTCTRL_SW_X, _("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE );
    itemFlexGridSizer46->Add(m_x, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_LabelY = new wxStaticText( m_LocationPanel, wxID_STATIC_SW_Y, _("y"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer46->Add(m_LabelY, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_y = new wxTextCtrl( m_LocationPanel, ID_TEXTCTRL_SW_Y, _("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE );
    itemFlexGridSizer46->Add(m_y, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_LabelZ = new wxStaticText( m_LocationPanel, wxID_STATIC_SW_Z, _("z"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer46->Add(m_LabelZ, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_z = new wxTextCtrl( m_LocationPanel, ID_TEXTCTRL_SW_Z, _("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE );
    itemFlexGridSizer46->Add(m_z, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_oriStrings;
    m_oriStrings.Add(_("&north"));
    m_oriStrings.Add(_("&east"));
    m_oriStrings.Add(_("&south"));
    m_oriStrings.Add(_("&west"));
    m_ori = new wxRadioBox( m_LocationPanel, ID_RADIOBOX_SW_ORI, _("Orientation"), wxDefaultPosition, wxDefaultSize, m_oriStrings, 1, wxRA_SPECIFY_COLS );
    m_ori->SetSelection(0);
    itemBoxSizer45->Add(m_ori, 0, wxALIGN_TOP|wxALL, 5);

    m_Notebook->AddPage(m_LocationPanel, _("Location"));

    m_InterfacePanel = new wxPanel( m_Notebook, ID_PANEL_SW_INTERFACE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxFlexGridSizer* itemFlexGridSizer55 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer55->AddGrowableCol(1);
    m_InterfacePanel->SetSizer(itemFlexGridSizer55);

    m_Labeliid = new wxStaticText( m_InterfacePanel, wxID_STATIC_SW_IID, _("iid"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer55->Add(m_Labeliid, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    m_iid = new wxTextCtrl( m_InterfacePanel, ID_TEXTCTRL_SW_IID, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer55->Add(m_iid, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    m_Label_Bus = new wxStaticText( m_InterfacePanel, wxID_STATIC_SW_BUS, _("Bus:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer55->Add(m_Label_Bus, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Bus = new wxTextCtrl( m_InterfacePanel, ID_TEXTCTRL_SW_BUS, _("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE );
    itemFlexGridSizer55->Add(m_Bus, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_LabelProt = new wxStaticText( m_InterfacePanel, wxID_STATIC_SW_PROT, _("Protocol"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer55->Add(m_LabelProt, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_ProtStrings;
    m_ProtStrings.Add(_("Motorola"));
    m_ProtStrings.Add(_("NMRA-DCC"));
    m_Prot = new wxChoice( m_InterfacePanel, ID_CHOICE_SW_PROT, wxDefaultPosition, wxDefaultSize, m_ProtStrings, 0 );
    m_Prot->SetStringSelection(_("Motorola"));
    itemFlexGridSizer55->Add(m_Prot, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_LabelAddress1 = new wxStaticText( m_InterfacePanel, wxID_STATIC_SW_ADDRESS1, _("address1"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer55->Add(m_LabelAddress1, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    m_Address1 = new wxTextCtrl( m_InterfacePanel, ID_TEXTCTRL_SW_ADDRESS1, _("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE );
    itemFlexGridSizer55->Add(m_Address1, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    m_LabelPort1 = new wxStaticText( m_InterfacePanel, wxID_STATIC_SW_PORT1, _("port1"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer55->Add(m_LabelPort1, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Port1 = new wxTextCtrl( m_InterfacePanel, ID_TEXTCTRL_SW_PORT1, _("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE );
    itemFlexGridSizer55->Add(m_Port1, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_labGate = new wxStaticText( m_InterfacePanel, wxID_STATIC_SW_GATE, _("gate"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer55->Add(m_labGate, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxArrayString m_GateStrings;
    m_GateStrings.Add(_("&Red"));
    m_GateStrings.Add(_("&Green"));
    m_Gate = new wxRadioBox( m_InterfacePanel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, m_GateStrings, 1, wxRA_SPECIFY_ROWS );
    m_Gate->SetSelection(0);
    itemFlexGridSizer55->Add(m_Gate, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    m_SingleGate = new wxCheckBox( m_InterfacePanel, ID_SWITCH_SINGLEGATE, _("single gate"), wxDefaultPosition, wxDefaultSize, 0 );
    m_SingleGate->SetValue(false);
    itemFlexGridSizer55->Add(m_SingleGate, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Invert1 = new wxCheckBox( m_InterfacePanel, ID_CHECKBOX_SW_INVERT1, _("invert1"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE );
    m_Invert1->SetValue(false);
    itemFlexGridSizer55->Add(m_Invert1, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_LabelAddress2 = new wxStaticText( m_InterfacePanel, wxID_STATIC_SW_ADDRESS2, _("address2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer55->Add(m_LabelAddress2, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_Address2 = new wxTextCtrl( m_InterfacePanel, ID_TEXTCTRL_SW_ADDRESS2, _("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE );
    itemFlexGridSizer55->Add(m_Address2, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_LabelPort2 = new wxStaticText( m_InterfacePanel, wxID_STATIC_SW_PORT2, _("port2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer55->Add(m_LabelPort2, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    m_Port2 = new wxTextCtrl( m_InterfacePanel, ID_TEXTCTRL_SW_PORT2, _("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE );
    itemFlexGridSizer55->Add(m_Port2, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxStaticText* itemStaticText74 = new wxStaticText( m_InterfacePanel, wxID_STATIC_SW_INVERT2, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer55->Add(itemStaticText74, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Invert2 = new wxCheckBox( m_InterfacePanel, ID_CHECKBOX_SW_INVERT2, _("invert2"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE );
    m_Invert2->SetValue(false);
    itemFlexGridSizer55->Add(m_Invert2, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_ActDelay = new wxCheckBox( m_InterfacePanel, ID_CHECKBOX_SW_DELAY, _("delay"), wxDefaultPosition, wxDefaultSize, 0 );
    m_ActDelay->SetValue(false);
    itemFlexGridSizer55->Add(m_ActDelay, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Delay = new wxTextCtrl( m_InterfacePanel, ID_TEXTCTRL_SW_DELAY, _("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE );
    itemFlexGridSizer55->Add(m_Delay, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Notebook->AddPage(m_InterfacePanel, _("Interface"));

    m_WireringPanel = new wxPanel( m_Notebook, ID_PANEL_SW_WIRERING, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer79 = new wxBoxSizer(wxVERTICAL);
    m_WireringPanel->SetSizer(itemBoxSizer79);

    m_WiringBox = new wxStaticBox(m_WireringPanel, wxID_ANY, _("Feedback"));
    wxStaticBoxSizer* itemStaticBoxSizer80 = new wxStaticBoxSizer(m_WiringBox, wxVERTICAL);
    itemBoxSizer79->Add(itemStaticBoxSizer80, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer81 = new wxFlexGridSizer(0, 3, 0, 0);
    itemFlexGridSizer81->AddGrowableCol(1);
    itemStaticBoxSizer80->Add(itemFlexGridSizer81, 0, wxGROW|wxALL, 5);
    m_LabelFb1R = new wxStaticText( m_WireringPanel, wxID_STATIC_SW_FB1R, _("sensor1R"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer81->Add(m_LabelFb1R, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_Fb1RStrings;
    m_Fb1R = new wxComboBox( m_WireringPanel, ID_COMBOBOX_SW_FB1R, _T(""), wxDefaultPosition, wxDefaultSize, m_Fb1RStrings, wxCB_DROPDOWN );
    itemFlexGridSizer81->Add(m_Fb1R, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Fb1Rinvert = new wxCheckBox( m_WireringPanel, wxID_ANY, _("Invert"), wxDefaultPosition, wxDefaultSize, 0 );
    m_Fb1Rinvert->SetValue(false);
    itemFlexGridSizer81->Add(m_Fb1Rinvert, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_LabelFb1G = new wxStaticText( m_WireringPanel, wxID_STATIC_SW_FB1G, _("sensor1G"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer81->Add(m_LabelFb1G, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxArrayString m_Fb1GStrings;
    m_Fb1G = new wxComboBox( m_WireringPanel, ID_COMBOBOX_SW_FB1G, _T(""), wxDefaultPosition, wxDefaultSize, m_Fb1GStrings, wxCB_DROPDOWN );
    itemFlexGridSizer81->Add(m_Fb1G, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_Fb1Ginvert = new wxCheckBox( m_WireringPanel, wxID_ANY, _("invert"), wxDefaultPosition, wxDefaultSize, 0 );
    m_Fb1Ginvert->SetValue(false);
    itemFlexGridSizer81->Add(m_Fb1Ginvert, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_LabelFb2R = new wxStaticText( m_WireringPanel, wxID_STATIC_SW_FB2R, _("sensor2R"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer81->Add(m_LabelFb2R, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxArrayString m_Fb2RStrings;
    m_Fb2R = new wxComboBox( m_WireringPanel, ID_COMBOBOX_SW_FB2R, _T(""), wxDefaultPosition, wxDefaultSize, m_Fb2RStrings, wxCB_DROPDOWN );
    itemFlexGridSizer81->Add(m_Fb2R, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_Fb2Rinvert = new wxCheckBox( m_WireringPanel, wxID_ANY, _("invert"), wxDefaultPosition, wxDefaultSize, 0 );
    m_Fb2Rinvert->SetValue(false);
    itemFlexGridSizer81->Add(m_Fb2Rinvert, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_LabelFb2G = new wxStaticText( m_WireringPanel, wxID_STATIC_SW_FB2G, _("sensor2G"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer81->Add(m_LabelFb2G, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxArrayString m_Fb2GStrings;
    m_Fb2G = new wxComboBox( m_WireringPanel, ID_COMBOBOX_SW_FB2G, _T(""), wxDefaultPosition, wxDefaultSize, m_Fb2GStrings, wxCB_DROPDOWN );
    itemFlexGridSizer81->Add(m_Fb2G, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_Fb2Ginvert = new wxCheckBox( m_WireringPanel, wxID_ANY, _("invert"), wxDefaultPosition, wxDefaultSize, 0 );
    m_Fb2Ginvert->SetValue(false);
    itemFlexGridSizer81->Add(m_Fb2Ginvert, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_CTCBox = new wxStaticBox(m_WireringPanel, wxID_ANY, _("CTC"));
    wxStaticBoxSizer* itemStaticBoxSizer94 = new wxStaticBoxSizer(m_CTCBox, wxVERTICAL);
    itemBoxSizer79->Add(itemStaticBoxSizer94, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer95 = new wxFlexGridSizer(0, 4, 0, 0);
    itemStaticBoxSizer94->Add(itemFlexGridSizer95, 0, wxGROW|wxALL, 5);
    m_labCTCMotor = new wxStaticText( m_WireringPanel, wxID_ANY, _("Motor"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer95->Add(m_labCTCMotor, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_labCTCIID = new wxStaticText( m_WireringPanel, wxID_ANY, _("IID"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer95->Add(m_labCTCIID, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_labCTCAddress = new wxStaticText( m_WireringPanel, wxID_STATIC, _("Address"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer95->Add(m_labCTCAddress, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_labCTCCmdAtOn = new wxStaticText( m_WireringPanel, wxID_STATIC, _("Command at ON"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer95->Add(m_labCTCCmdAtOn, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_labCTCAddr1 = new wxStaticText( m_WireringPanel, wxID_ANY, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer95->Add(m_labCTCAddr1, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_CTCIID1 = new wxTextCtrl( m_WireringPanel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer95->Add(m_CTCIID1, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_CTCAddr1 = new wxSpinCtrl( m_WireringPanel, ID_SPINCTRL, _T("0"), wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 0, 10000, 0 );
    itemFlexGridSizer95->Add(m_CTCAddr1, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxArrayString m_CTCOn1Strings;
    m_CTCOn1Strings.Add(_("&straight"));
    m_CTCOn1Strings.Add(_("&turnout"));
    m_CTCOn1Strings.Add(_("&flip"));
    m_CTCOn1 = new wxRadioBox( m_WireringPanel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, m_CTCOn1Strings, 1, wxRA_SPECIFY_ROWS );
    m_CTCOn1->SetSelection(0);
    itemFlexGridSizer95->Add(m_CTCOn1, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_labCTCAddr2 = new wxStaticText( m_WireringPanel, wxID_ANY, _("2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer95->Add(m_labCTCAddr2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_CTCIID2 = new wxTextCtrl( m_WireringPanel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer95->Add(m_CTCIID2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_CTCAddr2 = new wxSpinCtrl( m_WireringPanel, wxID_ANY, _T("0"), wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 0, 10000, 0 );
    itemFlexGridSizer95->Add(m_CTCAddr2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxArrayString m_CTCOn2Strings;
    m_CTCOn2Strings.Add(_("&straight"));
    m_CTCOn2Strings.Add(_("&turnout"));
    m_CTCOn2Strings.Add(_("&flip"));
    m_CTCOn2 = new wxRadioBox( m_WireringPanel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, m_CTCOn2Strings, 1, wxRA_SPECIFY_ROWS );
    m_CTCOn2->SetSelection(0);
    itemFlexGridSizer95->Add(m_CTCOn2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxFlexGridSizer* itemFlexGridSizer108 = new wxFlexGridSizer(0, 6, 0, 0);
    itemStaticBoxSizer94->Add(itemFlexGridSizer108, 0, wxGROW|wxALL, 5);
    m_labCTCLED = new wxStaticText( m_WireringPanel, wxID_ANY, _("LED"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer108->Add(m_labCTCLED, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_labCTCIIDLED = new wxStaticText( m_WireringPanel, wxID_ANY, _("IID"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer108->Add(m_labCTCIIDLED, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_labCTCAddrLED = new wxStaticText( m_WireringPanel, wxID_ANY, _("Address"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer108->Add(m_labCTCAddrLED, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_labCTCPortLED = new wxStaticText( m_WireringPanel, wxID_ANY, _("Port"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer108->Add(m_labCTCPortLED, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_labCTCGateLED = new wxStaticText( m_WireringPanel, wxID_ANY, _("Gate"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer108->Add(m_labCTCGateLED, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_labCTCSwitchLED = new wxStaticText( m_WireringPanel, wxID_ANY, _("Switch"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer108->Add(m_labCTCSwitchLED, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_labCTCLED1 = new wxStaticText( m_WireringPanel, wxID_ANY, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer108->Add(m_labCTCLED1, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_CTCIIDLED1 = new wxTextCtrl( m_WireringPanel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer108->Add(m_CTCIIDLED1, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_CTCAddrLED1 = new wxSpinCtrl( m_WireringPanel, wxID_ANY, _T("0"), wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 0, 10000, 0 );
    itemFlexGridSizer108->Add(m_CTCAddrLED1, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_CTCPortLED1 = new wxSpinCtrl( m_WireringPanel, wxID_ANY, _T("0"), wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 0, 10000, 0 );
    itemFlexGridSizer108->Add(m_CTCPortLED1, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_CTCGateLED1 = new wxSpinCtrl( m_WireringPanel, wxID_ANY, _T("0"), wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 0, 1, 0 );
    itemFlexGridSizer108->Add(m_CTCGateLED1, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_CTCAsSwitchLED1 = new wxCheckBox( m_WireringPanel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    m_CTCAsSwitchLED1->SetValue(false);
    itemFlexGridSizer108->Add(m_CTCAsSwitchLED1, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_labCTCLED2 = new wxStaticText( m_WireringPanel, wxID_ANY, _("2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer108->Add(m_labCTCLED2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_CTCIIDLED2 = new wxTextCtrl( m_WireringPanel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer108->Add(m_CTCIIDLED2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_CTCAddrLED2 = new wxSpinCtrl( m_WireringPanel, wxID_ANY, _T("0"), wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 0, 10000, 0 );
    itemFlexGridSizer108->Add(m_CTCAddrLED2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_CTCPortLED2 = new wxSpinCtrl( m_WireringPanel, wxID_ANY, _T("0"), wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 0, 10000, 0 );
    itemFlexGridSizer108->Add(m_CTCPortLED2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_CTCGateLED2 = new wxSpinCtrl( m_WireringPanel, wxID_ANY, _T("0"), wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 0, 1, 0 );
    itemFlexGridSizer108->Add(m_CTCGateLED2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_CTCAsSwitchLED2 = new wxCheckBox( m_WireringPanel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    m_CTCAsSwitchLED2->SetValue(false);
    itemFlexGridSizer108->Add(m_CTCAsSwitchLED2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_Notebook->AddPage(m_WireringPanel, _("Wiring"));

    m_ControlPanel = new wxPanel( m_Notebook, ID_PANEL_SWITCH_CONTROL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer128 = new wxBoxSizer(wxVERTICAL);
    m_ControlPanel->SetSizer(itemBoxSizer128);

    m_ActivateCtrl = new wxCheckBox( m_ControlPanel, wxID_ANY, _("Active"), wxDefaultPosition, wxDefaultSize, 0 );
    m_ActivateCtrl->SetValue(false);
    itemBoxSizer128->Add(m_ActivateCtrl, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer130 = new wxFlexGridSizer(0, 3, 0, 0);
    itemFlexGridSizer130->AddGrowableCol(1);
    itemBoxSizer128->Add(itemFlexGridSizer130, 0, wxALIGN_LEFT|wxALL, 5);
    m_labCtrlInterval = new wxStaticText( m_ControlPanel, wxID_ANY, _("Interval"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer130->Add(m_labCtrlInterval, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_CtrlInterval = new wxSpinCtrl( m_ControlPanel, wxID_ANY, _T("0"), wxDefaultPosition, wxSize(100, -1), wxSP_ARROW_KEYS, 0, 3600, 0 );
    itemFlexGridSizer130->Add(m_CtrlInterval, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_labCtrlIntervalSec = new wxStaticText( m_ControlPanel, wxID_ANY, _("sec."), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer130->Add(m_labCtrlIntervalSec, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_labCtrlDelay = new wxStaticText( m_ControlPanel, wxID_ANY, _("Delay"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer130->Add(m_labCtrlDelay, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_CtrlDelay = new wxSpinCtrl( m_ControlPanel, wxID_ANY, _T("0"), wxDefaultPosition, wxSize(100, -1), wxSP_ARROW_KEYS, 0, 3600, 0 );
    itemFlexGridSizer130->Add(m_CtrlDelay, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_labCtrlDelaySec = new wxStaticText( m_ControlPanel, wxID_ANY, _("sec."), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer130->Add(m_labCtrlDelaySec, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer137 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer137->AddGrowableCol(1);
    itemBoxSizer128->Add(itemFlexGridSizer137, 0, wxGROW|wxALL, 5);
    m_labCtrlRoutes = new wxStaticText( m_ControlPanel, wxID_ANY, _("Routes to lock"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer137->Add(m_labCtrlRoutes, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_CtrlRoutes = new wxTextCtrl( m_ControlPanel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer137->Add(m_CtrlRoutes, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Notebook->AddPage(m_ControlPanel, _("Control"));

    itemBoxSizer2->Add(m_Notebook, 1, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer140 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer140, 0, wxALIGN_RIGHT|wxALL, 5);
    m_Cancel = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer140->AddButton(m_Cancel);

    m_OK = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    m_OK->SetDefault();
    itemStdDialogButtonSizer140->AddButton(m_OK);

    m_Apply = new wxButton( itemDialog1, wxID_APPLY, _("&Apply"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer140->AddButton(m_Apply);

    itemStdDialogButtonSizer140->Realize();

////@end SwitchDialog content construction
}

/*!
 * Should we show tooltips?
 */

bool SwitchDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap SwitchDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin SwitchDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end SwitchDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon SwitchDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin SwitchDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end SwitchDialog icon retrieval
}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void SwitchDialog::OnCancelClick( wxCommandEvent& event )
{
  EndModal( 0 );
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void SwitchDialog::OnOkClick( wxCommandEvent& event )
{
  OnApplyClick(event);
  EndModal( wxID_OK );
}


/*!
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX
 */

void SwitchDialog::OnListboxSwSelected( wxCommandEvent& event )
{
  iONode model = wxGetApp().getModel();
  if( model != NULL ) {
    iONode swlist = wPlan.getswlist( model );
    if( swlist != NULL ) {
      int cnt = NodeOp.getChildCnt( swlist );
      for( int i = 0; i < cnt; i++ ) {
        iONode sw = NodeOp.getChild( swlist, i );
        const char* id = wSwitch.getid( sw );
        if( id != NULL && StrOp.equals( id, m_List->GetStringSelection().mb_str(wxConvUTF8) ) ) {
          m_Props = sw;
          initValues();
          break;
        }
      }
    }
  }
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_LOC_NEW
 */

void SwitchDialog::OnButtonSwNewClick( wxCommandEvent& event )
{
  int i = m_List->FindString( _T("NEW") );
  if( i == wxNOT_FOUND ) {
    m_List->Append( _T("NEW") );
    iONode model = wxGetApp().getModel();
    if( model != NULL ) {
      iONode swlist = wPlan.getswlist( model );
      if( swlist == NULL ) {
        swlist = NodeOp.inst( wSwitchList.name(), model, ELEMENT_NODE );
        NodeOp.addChild( model, swlist );
      }
      if( swlist != NULL ) {
        iONode sw = NodeOp.inst( wSwitch.name(), swlist, ELEMENT_NODE );
        NodeOp.addChild( swlist, sw );
        wSwitch.setid( sw, "NEW" );
        m_Props = sw;
        initValues();
      }
    }
  }
  m_List->SetStringSelection( _T("NEW") );
  m_List->SetFirstItem( _T("NEW") );
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DELETE
 */

void SwitchDialog::OnButtonSwDeleteClick( wxCommandEvent& event )
{
  if( m_Props == NULL )
    return;

  int action = wxMessageDialog( this, wxGetApp().getMsg("removewarning"), _T("Rocrail"), wxYES_NO | wxICON_EXCLAMATION ).ShowModal();
  if( action == wxID_NO )
    return;

  wxGetApp().pushUndoItem( (iONode)NodeOp.base.clone( m_Props ) );
  /* Notify RocRail. */
  iONode cmd = NodeOp.inst( wModelCmd.name(), NULL, ELEMENT_NODE );
  wModelCmd.setcmd( cmd, wModelCmd.remove );
  NodeOp.addChild( cmd, (iONode)m_Props->base.clone( m_Props ) );
  wxGetApp().sendToRocrail( cmd );
  cmd->base.del(cmd);

  iONode model = wxGetApp().getModel();
  if( model != NULL ) {
    iONode swlist = wPlan.getswlist( model );
    if( swlist != NULL ) {
      NodeOp.removeChild( swlist, m_Props );
      m_Props = NULL;
    }
  }

  initIndex();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_APPLY
 */

void SwitchDialog::OnApplyClick( wxCommandEvent& event )
{
  if( m_Props == NULL )
    return;
  if( !evaluate() )
    return;
  if( !wxGetApp().isStayOffline() ) {
    /* Notify RocRail. */
    iONode cmd = NodeOp.inst( wModelCmd.name(), NULL, ELEMENT_NODE );
    wModelCmd.setcmd( cmd, wModelCmd.modify );
    NodeOp.addChild( cmd, (iONode)m_Props->base.clone( m_Props ) );
    wxGetApp().sendToRocrail( cmd );
    cmd->base.del(cmd);
  }
  initIndex();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SW_DELAY
 */

void SwitchDialog::OnCheckboxSwDelayClick( wxCommandEvent& event )
{
  m_Delay->Enable( m_ActDelay->GetValue() );
}


/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_SW_TYPE
 */

void SwitchDialog::OnComboboxSwTypeSelected( wxCommandEvent& event )
{
  int type = m_Type->GetSelection();
  // Enable second address for threeways and double crossings
  if( type == 3 || type == 4 ) {
    m_Address2->Enable(true);
    m_Port2->Enable(true);
    m_Invert2->Enable(true);

  }
  else {
    m_Address2->Enable(false);
    m_Port2->Enable(false);
    m_Invert2->Enable(false);
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SWITCH_ACTIONS
 */

void SwitchDialog::OnSwitchActionsClick( wxCommandEvent& event )
{
  if( m_Props == NULL )
    return;

  ActionsCtrlDlg*  dlg = new ActionsCtrlDlg(this, m_Props );

  if( wxID_OK == dlg->ShowModal() ) {
    // TODO: inform
  }

  dlg->Destroy();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SWITCH_SINGLEGATE
 */

void SwitchDialog::OnSwitchSinglegateClick( wxCommandEvent& event )
{
  m_Gate->Enable(m_SingleGate->IsChecked());
}


void SwitchDialog::OnButtonSwDocClick( wxCommandEvent& event )
{
  doDoc( event, "switches");
}

