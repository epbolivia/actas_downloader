#include "actas_downloaderMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(actas_downloaderDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(actas_downloaderDialog)
//*)

BEGIN_EVENT_TABLE(actas_downloaderDialog,wxDialog)
    //(*EventTable(actas_downloaderDialog)
    //*)
END_EVENT_TABLE()

actas_downloaderDialog::actas_downloaderDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(actas_downloaderDialog)
    Create(parent, id, _("Actas Downloader"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    SetClientSize(wxSize(404,285));
    Center();
    //*)
}

actas_downloaderDialog::~actas_downloaderDialog()
{
    //(*Destroy(actas_downloaderDialog)
    //*)
}

void actas_downloaderDialog::OnQuit(wxCommandEvent& event)
{
    Close();
}

void actas_downloaderDialog::OnAbout(wxCommandEvent& event)
{
}
