#pragma once

#include <UR_V16X/UR_V16X.h>
#include "UR_HTTP_Types.h"

class UR_HTTP_Dl
{
public:

    UR_HTTP_Dl();
    ~UR_HTTP_Dl();

    void init();
    void configure();

    void OnInternalIdle();
    static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
    //static int progress_callback(void *clientp,   double dltotal,   double dlnow,   double ultotal,   double ulnow);
    void OnUpdateUI();
    bool OnInit();
    int OnExit();
    void OnClose();

    UR_V16X *v16x;

    static actas_config_t *get_config()
    {
        return &actas_config;
    }

    void OnQuit();
    void Onstart_curl_bClick();
    void Ondpto_radioctrlSelect(int selct);
    void Onbase_url_txtctrlText(char *base_url);
    void Onindex_slider_ctrlCmdScroll(int valint);
    void Onindex_slider_ctrlCmdScrollThumbRelease(int valint);
    void Onserver_mode_chkboxClick();
    void Onstop_download_bClick();
    void system_shutdown();
    void ur_configure();
    void ur_main_entry();
    void ur_loop();

private:

    static actas_config_t actas_config;
    void fire_process(void);
    //void add_transfer(CURLM *cm, int i, FILE *pagefile, char *urls);
    void generate_filetest(int cnt);
    void curl_main(uint32_t base_num);
    void curl_main();
    uint32_t _get_base_dpto_prefix(int index);
    uint32_t _read_file_to_buff(const char file_conf[], uint8_t **cbuff);
    void _read_url(char *url);
};
