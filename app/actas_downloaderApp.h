#ifndef ACTAS_DOWNLOADERAPP_H
#define ACTAS_DOWNLOADERAPP_H

#include <wx/app.h>

class actas_downloaderApp : public wxApp
{
    public:
        virtual bool OnInit();
        virtual int OnExit();
        //actas_downloaderDialog *Dlg;
        //UR_V16X v16x;
};

#endif // ACTAS_DOWNLOADERAPP_H
