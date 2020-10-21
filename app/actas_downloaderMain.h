#ifndef ACTAS_DOWNLOADERMAIN_H
#define ACTAS_DOWNLOADERMAIN_H

//(*Headers(actas_downloaderDialog)
#include <wx/dialog.h>
//*)

class actas_downloaderDialog: public wxDialog
{
    public:

        actas_downloaderDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~actas_downloaderDialog();

    private:

        //(*Handlers(actas_downloaderDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        //(*Identifiers(actas_downloaderDialog)
        //*)

        //(*Declarations(actas_downloaderDialog)
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // ACTAS_DOWNLOADERMAIN_H
