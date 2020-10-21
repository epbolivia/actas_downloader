#include "actas_downloaderApp.h"

//(*AppHeaders
#include "actas_downloaderMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(actas_downloaderApp);

bool actas_downloaderApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	actas_downloaderDialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)
    return wxsOK;

}
