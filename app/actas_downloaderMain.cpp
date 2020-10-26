#include <curl/curl.h>

//#include "actas_downloaderApp.h"
#include "actas_downloaderMain.h"
#include <wx/msgdlg.h>
//#include <wx/wx.h>
#include <wx/dcclient.h>

#define MAX_PARALLEL 5
#define NUM_URLS 100
#define FILE_EXTENSION ".jpg"

actas_config_t actas_downloaderDialog::actas_config;

//(*InternalHeaders(actas_downloaderDialog)
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(actas_downloaderDialog)
const long actas_downloaderDialog::ID_BUTTON1 = wxNewId();
const long actas_downloaderDialog::ID_BUTTON2 = wxNewId();
const long actas_downloaderDialog::ID_RADIOBOX1 = wxNewId();
const long actas_downloaderDialog::ID_SLIDER1 = wxNewId();
const long actas_downloaderDialog::ID_STATICTEXT2 = wxNewId();
const long actas_downloaderDialog::ID_STATICTEXT3 = wxNewId();
const long actas_downloaderDialog::ID_GAUGE1 = wxNewId();
const long actas_downloaderDialog::ID_STATICTEXT4 = wxNewId();
const long actas_downloaderDialog::ID_STATICTEXT5 = wxNewId();
const long actas_downloaderDialog::ID_STATICTEXT6 = wxNewId();
const long actas_downloaderDialog::ID_STATICTEXT7 = wxNewId();
const long actas_downloaderDialog::ID_BUTTON3 = wxNewId();
const long actas_downloaderDialog::ID_PANEL1 = wxNewId();
const long actas_downloaderDialog::ID_TEXTCTRL1 = wxNewId();
const long actas_downloaderDialog::ID_STATICTEXT1 = wxNewId();
const long actas_downloaderDialog::ID_CHECKBOX1 = wxNewId();
const long actas_downloaderDialog::ID_PANEL2 = wxNewId();
const long actas_downloaderDialog::ID_TEXTCTRL2 = wxNewId();
const long actas_downloaderDialog::ID_PANEL3 = wxNewId();
const long actas_downloaderDialog::ID_NOTEBOOK1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(actas_downloaderDialog,wxDialog)
    //(*EventTable(actas_downloaderDialog)
    //*)
END_EVENT_TABLE()

actas_downloaderDialog::actas_downloaderDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(actas_downloaderDialog)
    Create(parent, wxID_ANY, _("Actas Downloader"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxWANTS_CHARS, _T("wxID_ANY"));
    SetClientSize(wxSize(480,360));
    Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxSize(479,359), 0, _T("ID_NOTEBOOK1"));
    Panel1 = new wxPanel(Notebook1, ID_PANEL1, wxDefaultPosition, wxSize(479,359), 0, _T("ID_PANEL1"));
    start_curl_b = new wxButton(Panel1, ID_BUTTON1, _("Descargar"), wxPoint(16,296), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    quit_b = new wxButton(Panel1, ID_BUTTON2, _("Cerrar"), wxPoint(384,296), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    wxString __wxRadioBoxChoices_1[9] =
    {
    	_("Chuquisaca        "),
    	_("La Paz"),
    	_("Cochabamba"),
    	_("Oruro        "),
    	_("Potosi"),
    	_("Tarija"),
    	_("Santa Cruz"),
    	_("Beni"),
    	_("Pando")
    };
    dpto_radioctrl = new wxRadioBox(Panel1, ID_RADIOBOX1, _("Departamento:"), wxPoint(16,176), wxSize(440,103), 9, __wxRadioBoxChoices_1, 3, 0, wxDefaultValidator, _T("ID_RADIOBOX1"));
    dpto_radioctrl->SetSelection(0);
    index_slider_ctrl = new wxSlider(Panel1, ID_SLIDER1, 100, 100, 10000, wxPoint(16,32), wxSize(448,27), 0, wxDefaultValidator, _T("ID_SLIDER1"));
    index_slider_ctrl->SetPageSize(100);
    index_slider_ctrl->SetTick(100);
    index_slider_name_txtctrl = new wxStaticText(Panel1, ID_STATICTEXT2, _("Rastreo Max.:"), wxPoint(24,16), wxDefaultSize, wxST_NO_AUTORESIZE, _T("ID_STATICTEXT2"));
    index_slider_value_lblctrl = new wxStaticText(Panel1, ID_STATICTEXT3, _("100"), wxPoint(128,16), wxSize(56,15), wxST_NO_AUTORESIZE, _T("ID_STATICTEXT3"));
    cnt_download_gauge = new wxGauge(Panel1, ID_GAUGE1, 100, wxPoint(24,120), wxSize(432,8), 0, wxDefaultValidator, _T("ID_GAUGE1"));
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT4, _("Descargados:"), wxPoint(24,96), wxDefaultSize, wxST_NO_AUTORESIZE, _T("ID_STATICTEXT4"));
    cnt_download_lblctrl = new wxStaticText(Panel1, ID_STATICTEXT5, _("0"), wxPoint(120,96), wxSize(56,15), wxST_NO_AUTORESIZE, _T("ID_STATICTEXT5"));
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT6, _("Total:"), wxPoint(208,96), wxDefaultSize, wxST_NO_AUTORESIZE, _T("ID_STATICTEXT6"));
    total_lblctrl = new wxStaticText(Panel1, ID_STATICTEXT7, _("0"), wxPoint(248,96), wxSize(72,15), wxST_NO_AUTORESIZE, _T("ID_STATICTEXT7"));
    stop_download_b = new wxButton(Panel1, ID_BUTTON3, _("Detener"), wxPoint(104,296), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    Panel2 = new wxPanel(Notebook1, ID_PANEL2, wxPoint(374,11), wxDefaultSize, 0, _T("ID_PANEL2"));
    base_url_txtctrl = new wxTextCtrl(Panel2, ID_TEXTCTRL1, _("http://localhost:9998/test/"), wxPoint(16,42), wxSize(449,25), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    base_url_label = new wxStaticText(Panel2, ID_STATICTEXT1, _("Base URL:"), wxPoint(16,24), wxDefaultSize, wxST_NO_AUTORESIZE, _T("ID_STATICTEXT1"));
    server_mode_chkbox = new wxCheckBox(Panel2, ID_CHECKBOX1, _("Enable Server"), wxPoint(344,80), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    server_mode_chkbox->SetValue(false);
    Panel3 = new wxPanel(Notebook1, ID_PANEL3, wxPoint(258,10), wxDefaultSize, 0, _T("ID_PANEL3"));
    about_txtctrl = new wxTextCtrl(Panel3, ID_TEXTCTRL2, _("Text"), wxPoint(0,16), wxSize(479,100), wxTE_NO_VSCROLL|wxTE_MULTILINE|wxTE_READONLY|wxTE_AUTO_URL|wxTE_CENTRE|wxBORDER_NONE, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    about_txtctrl->Disable();
    about_txtctrl->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
    Notebook1->AddPage(Panel1, _("Principal"), true);
    Notebook1->AddPage(Panel2, _("Advanced"), false);
    Notebook1->AddPage(Panel3, _("Acerca de..."), false);
    Center();

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&actas_downloaderDialog::Onstart_curl_bClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&actas_downloaderDialog::OnQuit);
    Connect(ID_RADIOBOX1,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&actas_downloaderDialog::Ondpto_radioctrlSelect);
    Connect(ID_SLIDER1,wxEVT_SCROLL_TOP|wxEVT_SCROLL_BOTTOM|wxEVT_SCROLL_LINEUP|wxEVT_SCROLL_LINEDOWN|wxEVT_SCROLL_PAGEUP|wxEVT_SCROLL_PAGEDOWN|wxEVT_SCROLL_THUMBTRACK|wxEVT_SCROLL_THUMBRELEASE|wxEVT_SCROLL_CHANGED,(wxObjectEventFunction)&actas_downloaderDialog::Onindex_slider_ctrlCmdScroll);
    Connect(ID_SLIDER1,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&actas_downloaderDialog::Onindex_slider_ctrlCmdScroll);
    Connect(ID_SLIDER1,wxEVT_SCROLL_THUMBRELEASE,(wxObjectEventFunction)&actas_downloaderDialog::Onindex_slider_ctrlCmdScrollThumbRelease);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&actas_downloaderDialog::Onstop_download_bClick);
    Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&actas_downloaderDialog::Onbase_url_txtctrlText);
    Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&actas_downloaderDialog::Onserver_mode_chkboxClick);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&actas_downloaderDialog::OnClose);
    Connect(wxEVT_PAINT,(wxObjectEventFunction)&actas_downloaderDialog::OnPaint);
    Connect(wxEVT_KEY_UP,(wxObjectEventFunction)&actas_downloaderDialog::OnKeyUp);
    //*)

    Connect(wxEVT_CHAR_HOOK,(wxObjectEventFunction)&actas_downloaderDialog::OnKeyDown);
#ifdef __MSYS__
    Connect(wxEVT_CHAR_HOOK,(wxObjectEventFunction)&actas_downloaderDialog::OnKeyUp);
#endif // __MSYS__
    int selct = dpto_radioctrl->GetSelection();
    actas_config.base_dpto_prefix = _get_base_dpto_prefix(selct);
    actas_config.cnt_upto = index_slider_ctrl->GetValue();
    cnt_download_gauge->SetRange(actas_config.cnt_upto);
    _read_url(actas_config.base_url);
    base_url_txtctrl->SetValue(wxString::FromAscii(actas_config.base_url));
    Notebook1->SetFocus();
    stop_download_b->Enable(false);
#ifndef __MSYS__
    Panel2->Show(false);
#else
    Notebook1->SetPageText(1, _(""));
    base_url_label->Show(false);
    base_url_txtctrl->Show(false);
    server_mode_chkbox->Show(false);
#endif
    actas_config.stop_download = false;
}

actas_downloaderDialog::~actas_downloaderDialog()
{
    //(*Destroy(actas_downloaderDialog)
    //*)
}

void actas_downloaderDialog::OnQuit(wxCommandEvent& event)
{
    Close();
    event.Skip();
}

void actas_downloaderDialog::OnAbout(wxCommandEvent& event)
{
}

void actas_downloaderDialog::OnClose(wxCloseEvent& event)
{
    if (!sig_evt) {
        SHAL_SYSTEM::printf("Close form\n");
        fflush(stdout);
        v16x->shutdown_all();
        SHAL_SYSTEM::system_shutdown();
    }
    event.Skip();
}

void actas_downloaderDialog::fire_process(void)
{
    //actas_downloaderApp *app = (actas_downloaderApp*)wxApp::GetInstance();
    while(!sig_evt) {
        //app->v16x.update();
        v16x->update();
        if (!sig_evt) {
            SHAL_SYSTEM::run_thread_process(*v16x);
        }
    }
    SHAL_SYSTEM::printf("[ Exit fire process ]\n");
    fflush(stdout);
}

void actas_downloaderDialog::configure()
{
    about_txtctrl->SetValue(wxString::FromAscii(msg));
#ifdef SERVER_TEST
    if (server_mode_chkbox->IsChecked() && !actas_config.is_server_running) {
        mkdir("test", 0775);
        if (!v16x) {
            v16x = new UR_V16X;
        }
        //actas_downloaderApp *app = (actas_downloaderApp*)wxApp::GetInstance();
        v16x->init();
        SHAL_SYSTEM::run_thread_process(FUNCTOR_BIND_MEMBER(&actas_downloaderDialog::fire_process, void));
        actas_config.is_server_running = true;
    }
#endif // SERVER_TEST
#ifdef ACTAS_TEST
    generate_filetest(50);
#endif // ACTAS_TEST

}

size_t actas_downloaderDialog::write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    FILE *pagefile = (FILE *)stream;
    size_t written = fwrite(ptr, size, nmemb, pagefile);

    return written;
}

int progress_callback(void *clientp,   curl_off_t dltotal,   curl_off_t dlnow,   curl_off_t ultotal,   curl_off_t ulnow)
{
    //xfr_progress_t *xfr_prog = (xfr_progress_t*)clientp;

    if ((long)dltotal > 0) {
        if (dlnow == dltotal) {
            //fclose(xfr_prog->pagefile);
            //SHAL_SYSTEM::printf("File: %s dltotal: %" CURL_FORMAT_CURL_OFF_T " dlnow: %" CURL_FORMAT_CURL_OFF_T " ultotal: %" CURL_FORMAT_CURL_OFF_T " ulnow: %" CURL_FORMAT_CURL_OFF_T"\n", xfr_prog->filename, dltotal, dlnow, ultotal, ulnow);
        }
    }

    actas_config_t *config = actas_downloaderDialog::get_config();
    if (config->stop_download) {
        return 1;
    }

    return 0;
}

void add_transfer(CURLM *cm, int i, FILE *pagefile, char *urls, char *pagefilename)
{
    xfr_progress_t *progress = new xfr_progress_t;
    sprintf(progress->filename, "%s", pagefilename);

    CURL *eh = curl_easy_init();
    curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, actas_downloaderDialog::write_data);
    curl_easy_setopt(eh, CURLOPT_URL, urls);
    curl_easy_setopt(eh, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(eh, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(eh, CURLOPT_TIMEOUT, 30L);

    if(pagefile) {
        // write the page body to this file handle
        curl_easy_setopt(eh, CURLOPT_WRITEDATA, pagefile);
        progress->pagefile = pagefile;
        sprintf(progress->filename, "%s", pagefilename);
        curl_easy_setopt(eh, CURLOPT_XFERINFOFUNCTION, progress_callback);
        curl_easy_setopt(eh, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(eh, CURLOPT_XFERINFODATA, progress);
    }

    curl_easy_setopt(eh, CURLOPT_PRIVATE, progress);

    curl_multi_add_handle(cm, eh);
}

void actas_downloaderDialog::generate_filetest(int cnt)
{
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

    CURLM *cm;
    CURLMsg *msg;
    unsigned int transfers = 0;
    int msgs_left = -1;
    int still_alive = 1;
    int cnt_completed = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    cm = curl_multi_init();

    // Limit the amount of simultaneous connections curl should allow:
    curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, (long)MAX_PARALLEL);

    FILE *pagefile[NUM_URLS];
    char *full_url[NUM_URLS];
    char *filename[NUM_URLS];
    char *pagefilename[NUM_URLS];
    uint32_t base_transfers = actas_config.base_dpto_prefix + base_num;

    for (transfers = 0; transfers < MAX_PARALLEL; transfers++) {
        full_url[transfers] = new char[512];
        filename[transfers] = new char[50];
        sprintf(filename[transfers], "%u%s", transfers + base_transfers, FILE_EXTENSION);
        sprintf(full_url[transfers], "%s%s", actas_config.base_url, filename[transfers]);

        pagefilename[transfers] = new char[50];
        sprintf(pagefilename[transfers], "actas/%s/%u%s", actas_config.dpto, transfers + base_transfers, FILE_EXTENSION);
        pagefile[transfers] = new FILE;
        pagefile[transfers] = fopen(pagefilename[transfers], "wb");

        add_transfer(cm, transfers, pagefile[transfers], full_url[transfers], filename[transfers]);
        cnt_completed++;
        actas_config.cnt_completed++;
    }

    do {
        curl_multi_perform(cm, &still_alive);

        while ((msg = curl_multi_info_read(cm, &msgs_left))) {
            xfr_progress_t *xfr_prog;
            CURL *e = msg->easy_handle;
            curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &xfr_prog);
            if (msg->msg == CURLMSG_DONE) {
                SHAL_SYSTEM::printf("Response: %d - %s <%s> ", msg->data.result, curl_easy_strerror(msg->data.result), xfr_prog->filename);
                curl_multi_remove_handle(cm, e);
                curl_easy_cleanup(e);
                cnt_completed--;
                if (msg->data.result == 0) {
                    fprintf(stdout, "%s\n", "downloaded ok");
                    fflush(stdout);
                    actas_config.cnt_download++;
                } else {
                    SHAL_SYSTEM::printf("%s\n", " warning!");
                    fflush(stdout);
                }
            } else {
                SHAL_SYSTEM::printf("\nError: message: (%d)\n", msg->msg);
                fflush(stdout);
            }

            if (xfr_prog->pagefile != NULL) {
                fclose(xfr_prog->pagefile);
            }

            if (transfers < NUM_URLS) {
                full_url[transfers] = new char[512];
                filename[transfers] = new char[50];
                sprintf(filename[transfers], "%u%s", transfers + base_transfers, FILE_EXTENSION);
                sprintf(full_url[transfers], "%s%s", actas_config.base_url, filename[transfers]);

                pagefilename[transfers] = new char[50];
                sprintf(pagefilename[transfers], "actas/%s/%u%s", actas_config.dpto, transfers + base_transfers, FILE_EXTENSION);
                pagefile[transfers] = new FILE;
                pagefile[transfers] = fopen(pagefilename[transfers], "wb");

                add_transfer(cm, transfers, pagefile[transfers], full_url[transfers], filename[transfers]);
                transfers++;
                cnt_completed++;
                actas_config.cnt_completed++;
             }
        }

        if (still_alive) {
            curl_multi_wait(cm, NULL, 0, 1000, NULL);
        }

    } while (still_alive || (transfers < NUM_URLS));

    curl_multi_cleanup(cm);
    curl_global_cleanup();

}

void actas_downloaderDialog::Onstart_curl_bClick(wxCommandEvent& event)
{
    start_curl_b->Enable(false);
    base_url_txtctrl->Enable(false);
    index_slider_ctrl->Enable(false);
    dpto_radioctrl->Enable(false);
    stop_download_b->Enable(true);
    actas_config.stop_download = false;
    SHAL_SYSTEM::run_thread_process(FUNCTOR_BIND_MEMBER(&actas_downloaderDialog::curl_main, void));
}

void actas_downloaderDialog::curl_main()
{
    actas_config.cnt_completed = 0;
    actas_config.cnt_download = 0;

    for (uint32_t i = 0; i * NUM_URLS < NUM_URLS * (actas_config.cnt_upto / NUM_URLS); i++) {
        if (actas_config.stop_download) {
            break;
        }
        curl_main(i * NUM_URLS);
        SHAL_SYSTEM::delay_ms(1000);
    }

    SHAL_SYSTEM::printf("Finish download\n");
    fflush(stdout);

    start_curl_b->Enable(true);
    base_url_txtctrl->Enable(true);
    index_slider_ctrl->Enable(true);
    dpto_radioctrl->Enable(true);
    stop_download_b->Enable(false);
}

void actas_downloaderDialog::Ondpto_radioctrlSelect(wxCommandEvent& event)
{
    int selct = event.GetSelection();
    actas_config.base_dpto_prefix = _get_base_dpto_prefix(selct);
    SHAL_SYSTEM::printf("Selection: %d\n", (selct + 1) * 10000);
    fflush(stdout);
}

uint32_t actas_downloaderDialog::_get_base_dpto_prefix(int index)
{
    mkdir("actas", 0775);
    index = index + 1;

    memset(actas_config.dpto, '\0', sizeof(actas_config.dpto));
    switch(index) {
        case 1: {
            mkdir("actas/chuquisaca", 0775);
            sprintf(actas_config.dpto, "%s", "chuquisaca");
            break;
        }
        case 2: {
            mkdir("actas/lapaz", 0775);
            sprintf(actas_config.dpto, "%s", "lapaz");
            break;
        }
        case 3: {
            mkdir("actas/cochabamba", 0775);
            sprintf(actas_config.dpto, "%s", "cochabamba");
            break;
        }
        case 4: {
            mkdir("actas/oruro", 0775);
            sprintf(actas_config.dpto, "%s", "oruro");
            break;
        }
        case 5: {
            mkdir("actas/potosi", 0775);
            sprintf(actas_config.dpto, "%s", "potosi");
            break;
        }
        case 6: {
            mkdir("actas/tarija", 0775);
            sprintf(actas_config.dpto, "%s", "tarija");
            break;
        }
        case 7: {
            mkdir("actas/santacruz", 0775);
            sprintf(actas_config.dpto, "%s", "santacruz");
            break;
        }
        case 8: {
            mkdir("actas/beni", 0775);
            sprintf(actas_config.dpto, "%s", "beni");
            break;
        }
        case 9: {
            mkdir("actas/pando", 0775);
            sprintf(actas_config.dpto, "%s", "pando");
            break;
        }
    }

    return index * 10000;
}

void actas_downloaderDialog::Onbase_url_txtctrlText(wxCommandEvent& event)
{
    sprintf(actas_config.base_url, "%s",base_url_txtctrl->GetValue().mb_str().data());
}

void actas_downloaderDialog::Onindex_slider_ctrlCmdScroll(wxScrollEvent& event)
{
    uint16_t val = (uint16_t)round(event.GetInt() / 100) * 100;
    actas_config.cnt_upto = val;
    cnt_download_gauge->SetValue(0);
    actas_config.cnt_download = 0;
    actas_config.cnt_completed = 0;
    cnt_download_lblctrl->SetLabel(wxString::Format(_("%i"), 0));
    cnt_download_gauge->SetRange(val);
    index_slider_value_lblctrl->SetLabel(wxString::Format(_("%i"), val));
}

void actas_downloaderDialog::Onindex_slider_ctrlCmdScrollThumbRelease(wxScrollEvent& event)
{
    uint16_t val = (uint16_t)round(event.GetInt() / 100) * 100;
    actas_config.cnt_upto = val;
    cnt_download_gauge->SetValue(0);
    actas_config.cnt_download = 0;
    actas_config.cnt_completed = 0;
    cnt_download_gauge->SetRange(val);
    cnt_download_lblctrl->SetLabel(wxString::Format(_("%i"), 0));
    index_slider_value_lblctrl->SetLabel(wxString::Format(_("%i"), val));
    index_slider_ctrl->SetValue(val);
}

uint32_t actas_downloaderDialog::_read_file_to_buff(const char file_conf[], uint8_t **cbuff)
{
    uint32_t size_conf = 0;

    FILE *fconf= fopen(file_conf, "r");
    if (!fconf) {
        SHAL_SYSTEM::printf("Config not found!\n");
        fflush(stdout);
        return 0;
    }

    fseek(fconf, 0, SEEK_END);
    size_conf = ftell(fconf);
    fseek(fconf, 0, SEEK_SET);

    cbuff[0] = new uint8_t[size_conf + 1];
    memset(cbuff[0], 0, size_conf + 1);
    fread(cbuff[0], sizeof(uint8_t), size_conf, fconf);

    fclose(fconf);

    return size_conf;
}

void actas_downloaderDialog::_read_url(char *url)
{
    uint8_t *cbuff[1] = {nullptr};
    uint32_t size_conf;
    uint32_t size_tmp;
    size_conf = _read_file_to_buff("config.dat", cbuff);

    if (cbuff[0] != nullptr) {
        size_tmp = strlen((char*)cbuff[0]);
        if (cbuff[0][size_tmp - 1] != '/') {
            cbuff[0][size_tmp] = '/';
            size_tmp++;
            cbuff[0][size_tmp] = '\0';
        }
        size_conf = strlen((char*)cbuff[0]);
    } else {
        size_tmp = 0;
        cbuff[0] = (uint8_t*)"";
    }

    char *saveptr = nullptr;
    char *urltmp[2];

    if (size_conf > 0) {
        urltmp[0] = strtok_r((char*)cbuff[0], "\n", &saveptr);
        urltmp[1] = strtok_r(nullptr, "\n", &saveptr);
        sprintf(url, "%s", urltmp[0]);
    } else {
        char urldefault[] = "http://127.0.0.1:9998/test/";
        sprintf(url, "%s", urldefault);
        urltmp[0] = (char*)"";
    }

    //SHAL_SYSTEM::printf("Len: %u strlen: %u\n", size_conf, size_tmp);
    //SHAL_SYSTEM::printf("Default URL:\n---\n%s\n---\nTerminator: %c\n", urltmp[0], cbuff[0][size_tmp]);
    //fflush(stdout);
}

void actas_downloaderDialog::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    event.Skip();
}

void actas_downloaderDialog::OnUpdateUI()
{
    OnInternalIdle();
}

void actas_downloaderDialog::OnInternalIdle()
{
    if (!sig_evt) {
        if (wxUpdateUIEvent::CanUpdate(this)) {
            SHAL_SYSTEM::delay_ms(20);
            UpdateWindowUI(wxUPDATE_UI_FROMIDLE);
            char cntdownload[10];
            char cntcompleted[10];
            sprintf(cntdownload, "%u", actas_config.cnt_download);
            cnt_download_lblctrl->SetLabel(wxString::FromAscii(cntdownload));
            cnt_download_gauge->SetValue(actas_config.cnt_completed);
            sprintf(cntcompleted, "%u", actas_config.cnt_completed);
            total_lblctrl->SetLabel(wxString::FromAscii(cntcompleted));
        }
    }
}

void actas_downloaderDialog::OnKeyUp(wxKeyEvent& event)
{
    uint32_t now = SHAL_SYSTEM::millis32() - actas_config.key_timeout;
    uint32_t delta_factor = 100;
#ifdef __MSYS__
    delta_factor = delta_factor * 30;
#else
    delta_factor = delta_factor * 8;
#endif // __MSYS__
    switch (event.GetKeyCode()) {
        case 83: {
            if (now < delta_factor) {
                actas_config.show_url = actas_config.show_url | (0x01 << 1);
            }
        }
        break;
        case 306: {
            if (now < delta_factor) {
                actas_config.show_url = actas_config.show_url | (0x01 << 2);
            }
        }
        break;
        case 307:{
            if (now < delta_factor) {
                actas_config.show_url = actas_config.show_url | (0x01 << 3);
            }
        }
        break;
    }

    if (actas_config.show_url == 0x0e) {
        actas_config.is_show_url = !actas_config.is_show_url;
        SHAL_SYSTEM::printf("KEY: %d Time: %u dat: 0x%02x IsShow: %d\n", event.GetKeyCode(), now, actas_config.show_url, actas_config.is_show_url);
        fflush(stdout);
        actas_config.show_url = 0;
#ifndef __MSYS__
        Panel2->Show(actas_config.is_show_url);
#else
        if (actas_config.is_show_url) {
            Notebook1->SetPageText(1, _("Advanced"));
        } else {
            Notebook1->SetPageText(1, _(""));
        }
        base_url_label->Show(actas_config.is_show_url);
        base_url_txtctrl->Show(actas_config.is_show_url);
        server_mode_chkbox->Show(actas_config.is_show_url);
#endif
    }
    event.Skip();
}

void actas_downloaderDialog::OnKeyDown(wxKeyEvent& event)
{
    actas_config.key_timeout = SHAL_SYSTEM::millis32();
    event.Skip();
}

void actas_downloaderDialog::Onserver_mode_chkboxClick(wxCommandEvent& event)
{
#ifdef SERVER_TEST
    if (server_mode_chkbox->IsChecked() && !actas_config.is_server_running) {
        mkdir("test", 0775);
        //actas_downloaderApp *app = (actas_downloaderApp*)wxApp::GetInstance();
        if (!v16x) {
            v16x = new UR_V16X;
        }
        v16x->init();
        SHAL_SYSTEM::run_thread_process(FUNCTOR_BIND_MEMBER(&actas_downloaderDialog::fire_process, void));
        actas_config.is_server_running = true;
        SHAL_SYSTEM::printf("Checked Dialog\n");
        fflush(stdout);
    }
#endif // SERVER_TEST
}

void actas_downloaderDialog::Onstop_download_bClick(wxCommandEvent& event)
{
    actas_config.stop_download = true;
}
