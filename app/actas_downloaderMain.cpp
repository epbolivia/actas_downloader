#include "actas_downloaderMain.h"
#include "actas_downloaderApp.h"
#include <wx/msgdlg.h>

#define MAX_PARALLEL 2
#define NUM_URLS 5
#define FILE_EXTENSION ".jpg"

//(*InternalHeaders(actas_downloaderDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(actas_downloaderDialog)
const long actas_downloaderDialog::ID_BUTTON1 = wxNewId();
const long actas_downloaderDialog::ID_BUTTON2 = wxNewId();
const long actas_downloaderDialog::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(actas_downloaderDialog,wxDialog)
    //(*EventTable(actas_downloaderDialog)
    //*)
END_EVENT_TABLE()

actas_downloaderDialog::actas_downloaderDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(actas_downloaderDialog)
    Create(parent, id, _("Actas Downloader"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    SetClientSize(wxSize(397,275));
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(0,0), wxSize(400,280), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    start_curl_b = new wxButton(Panel1, ID_BUTTON1, _("Iniciar"), wxPoint(24,232), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    quit_b = new wxButton(Panel1, ID_BUTTON2, _("Cerrar"), wxPoint(296,232), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    Center();

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&actas_downloaderDialog::Onstart_curl_bClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&actas_downloaderDialog::OnQuit);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&actas_downloaderDialog::OnClose);
    //*)
    actas_config.base_dpto_prefix = 70000;
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

void actas_downloaderDialog::OnClose(wxCloseEvent& event)
{
    SHAL_SYSTEM::printf("Close form\n");
    event.Skip();
    if (!sig_evt) {
        SHAL_SYSTEM::system_shutdown();
    }
}

void actas_downloaderDialog::fire_process(void)
{
    actas_downloaderApp *app = (actas_downloaderApp*)wxApp::GetInstance();
    while(!sig_evt) {
        app->v16x.update();
        if (!sig_evt) {
            SHAL_SYSTEM::run_thread_process(app->v16x);
        }
    }
    SHAL_SYSTEM::printf("[ Exit fire process ]\n");
    fflush(stdout);
}

void actas_downloaderDialog::configure()
{
#ifdef SERVER_TEST
    actas_downloaderApp *app = (actas_downloaderApp*)wxApp::GetInstance();
    app->v16x.init();
    SHAL_SYSTEM::run_thread_process(FUNCTOR_BIND_MEMBER(&actas_downloaderDialog::fire_process, void));
#endif // SERVER_TEST
#ifdef ACTAS_TEST
    generate_filetest(50);
#endif // ACTAS_TEST

}

size_t actas_downloaderDialog::write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

void actas_downloaderDialog::add_transfer(CURLM *cm, int i, FILE *pagefile, char *urls)
{
    CURL *eh = curl_easy_init();
    curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(eh, CURLOPT_URL, urls);
    curl_easy_setopt(eh, CURLOPT_PRIVATE, urls);
    curl_easy_setopt(eh, CURLOPT_FOLLOWLOCATION, 1L);

    if(pagefile) {
        /* write the page body to this file handle */
        curl_easy_setopt(eh, CURLOPT_WRITEDATA, pagefile);
    }

    curl_multi_add_handle(cm, eh);
}

void actas_downloaderDialog::generate_filetest(int cnt)
{
    mkdir("test", 0775);
    chdir("test");

    for (int i = 0; i < cnt; i++) {
        char pagefilename[50];
        sprintf(pagefilename, "%u%s", i + actas_config.base_dpto_prefix, FILE_EXTENSION);
        FILE *pagefile;
        pagefile = fopen(pagefilename, "wb");
        fprintf(pagefile, "test file: %u\n", i + actas_config.base_dpto_prefix);
        fclose(pagefile);
    }
    chdir("../");
}

void actas_downloaderDialog::curl_main(uint32_t base_num)
{
    mkdir("out", 0775);
    CURLM *cm;
    CURLMsg *msg;
    unsigned int transfers = 0;
    int msgs_left = -1;
    int still_alive = 1;
    int cnt_completed = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    cm = curl_multi_init();

    /* Limit the amount of simultaneous connections curl should allow: */
    curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, (long)MAX_PARALLEL);

    FILE *pagefile[NUM_URLS];
    char base_url[] = "http://localhost:9998/test/";
    char *full_url;
    char *filename;
    char *pagefilename[NUM_URLS];
    uint32_t base_transfers = actas_config.base_dpto_prefix + base_num;

    for (int i = 0; i < NUM_URLS; i++) {
        pagefilename[i] = new char[50];
        sprintf(pagefilename[i], "out/%u%s", i + base_transfers, FILE_EXTENSION);
        pagefile[i] = fopen(pagefilename[i], "wb");
    }

    for (transfers = 0; transfers < MAX_PARALLEL; transfers++) {
        full_url = new char[256];
        filename = new char[30];
        sprintf(filename, "%u%s", transfers + base_transfers, FILE_EXTENSION);
        sprintf(full_url, "%s%s", base_url, filename);
        add_transfer(cm, transfers, pagefile[transfers], full_url);
        cnt_completed++;
    }

    do {
        curl_multi_perform(cm, &still_alive);

        while((msg = curl_multi_info_read(cm, &msgs_left))) {
            if(msg->msg == CURLMSG_DONE) {
                char *url;
                CURL *e = msg->easy_handle;
                curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
                fprintf(stdout, "R: %d - %s <%s>\n", msg->data.result, curl_easy_strerror(msg->data.result), url);
                curl_multi_remove_handle(cm, e);
                curl_easy_cleanup(e);
                cnt_completed--;
            } else {
                fprintf(stdout, "E: CURLMsg (%d)\n", msg->msg);
            }

            if(transfers < NUM_URLS) {
                full_url = new char[256];
                filename = new char[30];
                sprintf(filename, "%u%s", transfers + base_transfers, FILE_EXTENSION);
                sprintf(full_url, "%s%s", base_url, filename);
                add_transfer(cm, transfers, pagefile[transfers], full_url);
                transfers++;
                cnt_completed++;
                sleep(1);
            }
        }

        if(still_alive) {
            curl_multi_wait(cm, NULL, 0, 1000, NULL);
        }

    } while(still_alive || (transfers < NUM_URLS));

    for (int i = 0; i < NUM_URLS; i++) {
        fclose(pagefile[i]);
    }

    curl_multi_cleanup(cm);
    curl_global_cleanup();
}

void actas_downloaderDialog::Onstart_curl_bClick(wxCommandEvent& event)
{
    start_curl_b->Enable(false);
    SHAL_SYSTEM::run_thread_process(FUNCTOR_BIND_MEMBER(&actas_downloaderDialog::curl_main, void));
}

void actas_downloaderDialog::curl_main()
{
    uint32_t cnt_upto = 10;
    for (uint32_t i = 0; i * NUM_URLS < NUM_URLS * (cnt_upto / NUM_URLS); i++) {
        curl_main(i * NUM_URLS);
        sleep(5);
    }

    start_curl_b->Enable(true);
}
