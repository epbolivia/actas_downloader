#include "actas_downloaderApp.h"

//(*AppHeaders
#include "app/actas_downloaderMain.h"
#include <wx/image.h>
//*)

char *actas_downloaderDialog::msg = {"Actas Downloader is a multithreaded HTTP file downloader\n" \
                        "Copyright (c) 2020 Hiroshi Takey F. <htakey@gmail.com>\n" \
                        "This program comes with ABSOLUTELY NO WARRANTY.\n" \
                        "This is free software; licensed under GPLv3.\n" \
                        "See source distribution for detailed copyright notices.\n"};

IMPLEMENT_APP_NO_MAIN(actas_downloaderApp);

//UR_V16X *actas_downloaderDialog::v16x;

bool actas_downloaderApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
        Dlg = new actas_downloaderDialog(0);
        SetTopWindow(Dlg);
        Dlg->Show();
        //wxsOK = false;
    }
    //*)
    SHAL_SYSTEM::printf("APP OnInit\n");
    return wxsOK;

}

int actas_downloaderApp::OnExit()
{
    SHAL_SYSTEM::printf("APP OnExit\n");
    return 0;
}

void SHAL_SYSTEM::system_shutdown()
{
    if (sig_evt) {
        _isr_timer_running = false;
        SHAL_SYSTEM::delay_sec(2);
        //wxGetApp().v16x.shutdown_all();
        //wxGetApp().Dlg->v16x->shutdown_all();
        SHAL_SYSTEM::printf("Shutdown main\n");
        wxGetApp().Dlg->Close();
        wxGetApp().Dlg->Destroy();
    }
}

void main_entry()
{
    SHAL_SYSTEM::printf("Entry\n");
    wxEntry(system_argcv.argc, system_argcv.argv);
}

void configure()
{
/*
    wxGetApp().Dlg->msg = {"Actas Downloader is a multithreaded HTTP file downloader\n" \
                        "Copyright (c) 2020 Hiroshi Takey F. <htakey@gmail.com>\n" \
                        "This program comes with ABSOLUTELY NO WARRANTY.\n" \
                        "This is free software; licensed under AGPLv3.\n" \
                        "See source distribution for detailed copyright notices.\n"};
*/
    SHAL_SYSTEM::printf("%s\n", wxGetApp().Dlg->msg);
    SHAL_SYSTEM::printf("Configuring\n");

    SHAL_SYSTEM::init();
    SHAL_SYSTEM::run_thread_process(&main_entry);
    SHAL_SYSTEM::delay_sec(1);

    wxGetApp().Dlg->configure();

    SHAL_SYSTEM::printf("Started\n");
    fflush(stdout);
}

void loop()
{
    SHAL_SYSTEM::printf("Loop\n");
    SHAL_SYSTEM::delay_sec(5);
}
