#include "UR_HTTP_Dl.h"
#include <curl/curl.h>

#include <sys/stat.h>

#define MAX_PARALLEL 5
#define NUM_URLS 100
#define FILE_EXTENSION ".jpg"

actas_config_t UR_HTTP_Dl::actas_config;

UR_HTTP_Dl::UR_HTTP_Dl()
{
}

UR_HTTP_Dl::~UR_HTTP_Dl()
{
}

void UR_HTTP_Dl::init()
{
    actas_config.base_dpto_prefix = _get_base_dpto_prefix(1);
    actas_config.cnt_upto = 100;
    _read_url(actas_config.base_url);
    actas_config.stop_download = false;
}

bool UR_HTTP_Dl::OnInit()
{
    bool wxsOK = true;
    SHAL_SYSTEM::printf("APP OnInit\n");
    return wxsOK;
}

int UR_HTTP_Dl::OnExit()
{
    SHAL_SYSTEM::printf("APP OnExit\n");
    return 0;
}

void UR_HTTP_Dl::system_shutdown()
{
    if (sig_evt) {
        SHAL_SYSTEM::_isr_timer_running = false;
        SHAL_SYSTEM::delay_sec(2);
        SHAL_SYSTEM::printf("Shutdown main\n");
        OnClose();
    }
}

void UR_HTTP_Dl::ur_main_entry()
{
    SHAL_SYSTEM::printf("Entry\n");
    //wxEntry(system_argcv.argc, system_argcv.argv);
}

void UR_HTTP_Dl::ur_configure()
{
    SHAL_SYSTEM::printf("Configuring\n");
    fflush(stdout);

    SHAL_SYSTEM::init();
    SHAL_SYSTEM::run_thread_process(FUNCTOR_BIND_MEMBER(&UR_HTTP_Dl::ur_main_entry, void));
    SHAL_SYSTEM::delay_sec(1);

    configure();

    SHAL_SYSTEM::printf("Started\n");
    fflush(stdout);
}

void UR_HTTP_Dl::ur_loop()
{
#if V16X_DEBUG >= 1
    SHAL_SYSTEM::printf("Loop\n");
#endif // V16X_DEBUG
#ifdef __MSYS__
    SHAL_SYSTEM::delay_ms(200);
    Dlg->OnUpdateUI();
#else
    SHAL_SYSTEM::delay_sec(1);
#endif // __MSYS__
}

void UR_HTTP_Dl::OnQuit()
{
    OnClose();
}

void UR_HTTP_Dl::OnClose()
{
    if (!sig_evt) {
        SHAL_SYSTEM::printf("Close form\n");
        fflush(stdout);
        v16x->shutdown_all();
        SHAL_SYSTEM::system_shutdown();
    }
}

void UR_HTTP_Dl::fire_process(void)
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

void UR_HTTP_Dl::configure()
{
#ifdef SERVER_TEST
    if (actas_config.server_enabled && !actas_config.is_server_running) {
        mkdir("test", 0775);
        if (!v16x) {
            v16x = new UR_V16X;
        }
        //actas_downloaderApp *app = (actas_downloaderApp*)wxApp::GetInstance();
        v16x->init();
        SHAL_SYSTEM::run_thread_process(FUNCTOR_BIND_MEMBER(&UR_HTTP_Dl::fire_process, void));
        actas_config.is_server_running = true;
    }
#endif // SERVER_TEST
#ifdef ACTAS_TEST
    generate_filetest(50);
#endif // ACTAS_TEST

}

size_t UR_HTTP_Dl::write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    FILE *pagefile = (FILE *)stream;
    size_t written = fwrite(ptr, size, nmemb, pagefile);

    return written;
}

int ur_progress_callback(void *clientp,   curl_off_t dltotal,   curl_off_t dlnow,   curl_off_t ultotal,   curl_off_t ulnow)
{
    //xfr_progress_t *xfr_prog = (xfr_progress_t*)clientp;

    if ((long)dltotal > 0) {
        if (dlnow == dltotal) {
            //fclose(xfr_prog->pagefile);
            //SHAL_SYSTEM::printf("File: %s dltotal: %" CURL_FORMAT_CURL_OFF_T " dlnow: %" CURL_FORMAT_CURL_OFF_T " ultotal: %" CURL_FORMAT_CURL_OFF_T " ulnow: %" CURL_FORMAT_CURL_OFF_T"\n", xfr_prog->filename, dltotal, dlnow, ultotal, ulnow);
        }
    }

    actas_config_t *config = UR_HTTP_Dl::get_config();
    if (config->stop_download) {
        return 1;
    }

    return 0;
}

void ur_add_transfer(CURLM *cm, int i, FILE *pagefile, char *urls, char *pagefilename)
{
    xfr_progress_t *progress = new xfr_progress_t;
    sprintf(progress->filename, "%s", pagefilename);

    CURL *eh = curl_easy_init();
    curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, UR_HTTP_Dl::write_data);
    curl_easy_setopt(eh, CURLOPT_URL, urls);
    curl_easy_setopt(eh, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(eh, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(eh, CURLOPT_TIMEOUT, 30L);

    if(pagefile) {
        // write the page body to this file handle
        curl_easy_setopt(eh, CURLOPT_WRITEDATA, pagefile);
        progress->pagefile = pagefile;
        sprintf(progress->filename, "%s", pagefilename);
        curl_easy_setopt(eh, CURLOPT_XFERINFOFUNCTION, ur_progress_callback);
        curl_easy_setopt(eh, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(eh, CURLOPT_XFERINFODATA, progress);
    }

    curl_easy_setopt(eh, CURLOPT_PRIVATE, progress);

    curl_multi_add_handle(cm, eh);
}

void UR_HTTP_Dl::generate_filetest(int cnt)
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

void UR_HTTP_Dl::curl_main(uint32_t base_num)
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

        ur_add_transfer(cm, transfers, pagefile[transfers], full_url[transfers], filename[transfers]);
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

                ur_add_transfer(cm, transfers, pagefile[transfers], full_url[transfers], filename[transfers]);
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

void UR_HTTP_Dl::Onstart_curl_bClick()
{
    actas_config.stop_download = false;
    SHAL_SYSTEM::run_thread_process(FUNCTOR_BIND_MEMBER(&UR_HTTP_Dl::curl_main, void));
}

void UR_HTTP_Dl::curl_main()
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
}

void UR_HTTP_Dl::Ondpto_radioctrlSelect(int selct)
{
    actas_config.base_dpto_prefix = _get_base_dpto_prefix(selct);
    SHAL_SYSTEM::printf("Selection: %d\n", (selct + 1) * 10000);
    fflush(stdout);
}

uint32_t UR_HTTP_Dl::_get_base_dpto_prefix(int index)
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

void UR_HTTP_Dl::Onbase_url_txtctrlText(char *base_url)
{
    sprintf(actas_config.base_url, "%s", base_url);
}

void UR_HTTP_Dl::Onindex_slider_ctrlCmdScroll(int valint)
{
    uint16_t val = (uint16_t)round(valint / 100) * 100;
    actas_config.cnt_upto = val;
    actas_config.cnt_download = 0;
    actas_config.cnt_completed = 0;
}

uint32_t UR_HTTP_Dl::_read_file_to_buff(const char file_conf[], uint8_t **cbuff)
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

void UR_HTTP_Dl::_read_url(char *url)
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

void UR_HTTP_Dl::OnUpdateUI()
{
    OnInternalIdle();
}

void UR_HTTP_Dl::Onserver_mode_chkboxClick()
{
#ifdef SERVER_TEST
    if (actas_config.server_enabled && !actas_config.is_server_running) {
        mkdir("test", 0775);
        //actas_downloaderApp *app = (actas_downloaderApp*)wxApp::GetInstance();
        if (!v16x) {
            v16x = new UR_V16X;
        }
        v16x->init();
        SHAL_SYSTEM::run_thread_process(FUNCTOR_BIND_MEMBER(&UR_HTTP_Dl::fire_process, void));
        actas_config.is_server_running = true;
        SHAL_SYSTEM::printf("Checked Dialog\n");
        fflush(stdout);
    }
#endif // SERVER_TEST
}

void UR_HTTP_Dl::Onstop_download_bClick()
{
    actas_config.stop_download = true;
}

