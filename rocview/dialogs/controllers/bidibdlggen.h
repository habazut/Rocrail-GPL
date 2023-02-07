///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 17 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __bidibdlggen__
#define __bidibdlggen__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/radiobox.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class bidibdlggen
///////////////////////////////////////////////////////////////////////////////
class bidibdlggen : public wxDialog 
{
	private:
	
	protected:
		wxNotebook* m_Notebook;
		wxPanel* m_General;
		wxStaticText* m_labIID;
		wxTextCtrl* m_IID;
		wxStaticText* m_labDevice;
		wxTextCtrl* m_Device;
		wxRadioBox* m_BPS;
		wxRadioBox* m_SubLib;
		wxPanel* m_Options;
		wxCheckBox* m_SecAck;
		wxStaticText* m_labSecAckInt;
		wxSpinCtrl* m_SecAckInt;
		wxStaticText* m_labMs10;
		wxPanel* m_Nodes;
		wxListBox* m_NodeList;
		wxStaticText* m_labUID;
		wxTextCtrl* m_UID;
		wxStaticText* m_labOffset;
		wxSpinCtrl* m_Offset;
		wxButton* m_AddNode;
		wxButton* m_ModifyNode;
		wxButton* m_DeleteNode;
		wxStdDialogButtonSizer* m_StdButton;
		wxButton* m_StdButtonOK;
		wxButton* m_StdButtonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnNodeList( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnAddNode( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnModifyNode( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnDeleteNode( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnOK( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		bidibdlggen( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("BiDiB Setup"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~bidibdlggen();
	
};

#endif //__bidibdlggen__
